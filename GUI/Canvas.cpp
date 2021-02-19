// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

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
    
    if (!emuTex.create(HPIXELS, VPIXELS)) {
        throw Exception("Can't create emulator texture");
    }
        
    view.init(emuTex);
    // view.rectangle.setTextureRect(sf::IntRect(texX1, texY1, texW, texH));
    view.rectangle.setTextureRect(sf::IntRect(texX1, texY1, texW, texH));
}

void
Canvas::configure()
{
    auto size = app.window.getSize();
    baseX = size.x / 2;
    baseY = size.y / 2;
}

void
Canvas::handle(const sf::Event &event)
{
    switch (event.type) {
            
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
        case sf::Event::MouseButtonPressed:
        {
            if (!app.os.gotMouse) {
                app.os.retainMouse();
                return;
            }
            
            if (event.mouseButton.button == sf::Mouse::Left) {
                printf("Pressed left\n");
                app.amiga.controlPort1.mouse.setLeftButton(true);
            } else if (event.mouseButton.button == sf::Mouse::Right) {
                printf("Pressed right\n");
                app.amiga.controlPort1.mouse.setRightButton(true);
            }
            break;
        }
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
        default:
            break;
    }
}

void
Canvas::update(sf::Time dt)
{
    Layer::update(dt);
    
    // Update the mouse location
    if (app.os.gotMouse && app.os.mouseMoved()) {
        mouseMoved(app.os.mouseDX, app.os.mouseDY);
    }
    
    // Update the texture
    if (app.amiga.isPaused()) {
        
        emuTex.update((u8 *)app.amiga.denise.pixelEngine.getNoise());
        
    } else if (app.amiga.isRunning()) {
            
        ScreenBuffer current = app.amiga.denise.pixelEngine.getStableBuffer();
        if (screenBuffer.data != current.data) {
            screenBuffer = current;
            emuTex.update((u8 *)(screenBuffer.data + 4 * HBLANK_MIN));
        }
    }
}

void
Canvas::render()
{
    view.rectangle.setFillColor(sf::Color(0xFF,0xFF,0xFF,alpha));
    view.draw(app.window);
}

void
Canvas::resize(float width, float height)
{
    float newWidth;
    float newHeight;
    
    if (letterbox) {
 
        float ratio = (float)texW / (float)(2 * texH);
        newWidth  = width / height > ratio ? height * ratio : width;
        newHeight = width / height > ratio ? height : width / ratio;

    } else {

        newWidth  = width;
        newHeight = height;
    }
    
    view.setSize(newWidth, newHeight);
    view.setPosition((width - newWidth) / 2, (height - newHeight) / 2);
}

void
Canvas::mouseMoved(int dx, int dy)
{
    app.amiga.controlPort1.mouse.setDeltaXY(dx, dy);
}
