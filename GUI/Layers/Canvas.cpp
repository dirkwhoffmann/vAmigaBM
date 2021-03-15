// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Application.h"

Canvas::Canvas(Application &ref) : Layer(ref)
{
    
}

Canvas::~Canvas()
{
    
}

void Canvas::init()
{
    Layer::init();

    // Configure the animation speed
    delay = 0.5;
    
    // Create the noise texture
    if (!noiseTexture.create(TEX_NOISE_W, TEX_NOISE_H)) {
        throw Exception("Can't create the noise texture");
    }
    noiseTextureRect.setSize(sf::Vector2f(TEX_MERGE_W, TEX_MERGE_H));
    noiseTextureRect.setTexture(&noiseTexture);

    // Create the long frame texture
    if (!longFrameTexture.create(TEX_FRAME_W, TEX_FRAME_H)) {
        throw Exception("Can't create the long frame texture");
    }
    longFrameRect.setSize(sf::Vector2f(TEX_MERGE_W, TEX_MERGE_H));
    longFrameRect.setTexture(&longFrameTexture);

    // Create the short frame texture
    if (!shortFrameTexture.create(TEX_FRAME_W, TEX_FRAME_H)) {
        throw Exception("Can't create the short frame texture");
    }
    shortFrameRect.setSize(sf::Vector2f(TEX_MERGE_W, TEX_MERGE_H));
    shortFrameRect.setTexture(&shortFrameTexture);

    // Create the merge texture (combining two frame textures)
    if (!mergeTexture.create(TEX_MERGE_W, TEX_MERGE_H)) {
        throw Exception("Can't create the merge texture");
    }
    mergeTextureRect.setSize(sf::Vector2f(textureRect.width, textureRect.height));
    mergeTextureRect.setTexture(&mergeTexture.getTexture());
        
    // Creates the merge shader and the bypass shader
    mergeBypassShader = &app.assets.get(ShaderID::mergeBypass);
    mergeShader = &app.assets.get(ShaderID::merge);
    mergeShader->setUniform("textureSize", sf::Glsl::Vec2(TEX_FRAME_W, TEX_FRAME_H));
    
    sf::IntRect flippedRect = textureRect;
    flippedRect.top = TEX_MERGE_H - flippedRect.top;
    flippedRect.height = - flippedRect.height;
    
    mergeTextureRect.setTextureRect(flippedRect);
}

void
Canvas::awake()
{
    auto size = app.window.getSize();
    baseX = size.x / 2;
    baseY = size.y / 2;
}

void
Canvas::respond(const sf::Event &event)
{
    switch (event.type) {
            
        /*
        case sf::Event::JoystickButtonPressed:
        {
            printf("BUTTON\n");
            app.amiga.controlPort2.joystick.trigger(PRESS_FIRE);
            break;
        }
        case sf::Event::JoystickButtonReleased:
        {
            printf("BUTTON (RELEASED)\n");
            app.amiga.controlPort2.joystick.trigger(RELEASE_FIRE);
            break;
        }
        case sf::Event::JoystickMoved:
        {
            if (event.joystickMove.axis == sf::Joystick::X)
            {
                if (event.joystickMove.position > 10.0) {
                    app.amiga.controlPort2.joystick.trigger(PULL_RIGHT);
                } else if (event.joystickMove.position < -10.0) {
                    app.amiga.controlPort2.joystick.trigger(PULL_LEFT);
                } else {
                    app.amiga.controlPort2.joystick.trigger(RELEASE_X);
                }
            }
            if (event.joystickMove.axis == sf::Joystick::Y)
            {
                if (event.joystickMove.position > 10.0) {
                    app.amiga.controlPort2.joystick.trigger(PULL_DOWN);
                } else if (event.joystickMove.position < -10.0) {
                    app.amiga.controlPort2.joystick.trigger(PULL_UP);
                } else {
                    app.amiga.controlPort2.joystick.trigger(RELEASE_Y);
                }
            }
            break;
        }
        */
        case sf::Event::MouseButtonPressed:
        {
            if (!inputManager.gotMouse) {
                if (!statusBar.mouseButtonPressed(0)) {
                    app.inputManager.retainMouse();
                }
            }
            
            /*
            if (event.mouseButton.button == sf::Mouse::Left) {
                printf("Pressed left\n");
                app.amiga.controlPort1.mouse.setLeftButton(true);
            } else if (event.mouseButton.button == sf::Mouse::Right) {
                printf("Pressed right\n");
                app.amiga.controlPort1.mouse.setRightButton(true);
            }
            */
            break;
        }
        /*
        case sf::Event::MouseButtonReleased:
        {
            if (!app.os.gotMouse) { return; }
            
            if (event.mouseButton.button == sf::Mouse::Left) {
                printf("Released left\n");
                app.amiga.controlPort1.mouse.setLeftButton(false);
            } else if (event.mouseButton.button == sf::Mouse::Right) {
                printf("Released right\n");
                app.amiga.controlPort1.mouse.setRightButton(false);
            }
            break;
        }
        */
        default:
            break;
    }
}

void
Canvas::update(u64 frames, sf::Time dt)
{
    Layer::update(frames, dt);
    
    // Update the mouse location
    if (app.os.gotMouse && app.os.mouseMoved()) {
        mouseMoved(app.os.mouseDX, app.os.mouseDY);
    }
            
    // Update the texture
    if (app.amiga.isPoweredOff()) {
        
        noiseTexture.update((u8 *)app.amiga.denise.pixelEngine.getNoise());
        
    } else if (app.amiga.isRunning()) {
            
        ScreenBuffer buffer = app.amiga.denise.pixelEngine.getStableBuffer();
                
        // Only proceed if the emulator delivers a new texture
        if (prevBuffer.data == buffer.data) return;
        prevBuffer = buffer;

        // Determine if the new texture is a long frame or a short frame
        prevLOF = currLOF;
        currLOF = buffer.longFrame;
        
        // Update the GPU texture
        if (currLOF) {
            // printf("Updating long frame texture\n")
            longFrameTexture.update((u8 *)(buffer.data + 4 * HBLANK_MIN));
        } else {
            // printf("Updating short frame texture\n")
            shortFrameTexture.update((u8 *)(buffer.data + 4 * HBLANK_MIN));
        }
    }
}

void
Canvas::render()
{    
    if (app.amiga.isPoweredOff()) {
        
        printf("draw Noise\n");
        app.window.draw(noiseTextureRect);
        return;
    }
    
    if (app.amiga.isPaused()) {

        mergeTextureRect.setFillColor(sf::Color(0xA0,0xA0,0xA0,alpha));

    } else {

        mergeTextureRect.setFillColor(sf::Color(0xFF,0xFF,0xFF,alpha));
        
        if (currLOF != prevLOF) {
            
            // Case 1: Interlace drawing
            mergeShader->setUniform("texture1", longFrameTexture);
            mergeShader->setUniform("texture2", shortFrameTexture);
            mergeTexture.draw(shortFrameRect, mergeShader);
            
        } else if (currLOF) {
            
            // Case 2: Non-interlace drawing (two long frames in a row)
            mergeBypassShader->setUniform("texture", longFrameTexture);
            mergeTexture.draw(longFrameRect, mergeBypassShader);
            
        } else {
            
            // Case 3: Non-interlace drawing (two short frames in a row)
            mergeBypassShader->setUniform("texture", shortFrameTexture);
            mergeTexture.draw(shortFrameRect, mergeBypassShader);
        }
    }
    
    app.window.draw(mergeTextureRect);
}

void
Canvas::resize(float width, float height)
{
    float newWidth;
    float newHeight;

    // Resize the noise texture rect
    noiseTextureRect.setSize(sf::Vector2f(width, height));

    // Resize the emulator texture rect
    if (letterbox) {
 
        float ratio = (float)textureRect.width / (float)textureRect.height;
        newWidth  = width / height > ratio ? height * ratio : width;
        newHeight = width / height > ratio ? height : width / ratio;

    } else {

        newWidth  = width;
        newHeight = height;
    }
    
    mergeTextureRect.setSize(sf::Vector2f(newWidth, newHeight));
    mergeTextureRect.setPosition((width - newWidth) / 2, (height - newHeight) / 2);
}

void
Canvas::mouseMoved(int dx, int dy)
{
    app.amiga.controlPort1.mouse.setDeltaXY(dx, dy);
}

void
Canvas::powerOn()
{
    mergeTexture.clear();
    open();
}

void
Canvas::powerOff()
{

}
