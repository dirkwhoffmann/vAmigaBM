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
    
    int x1 = HBLANK_CNT * 4;
    int x2 = HPOS_CNT * 4;
    int y1 = VBLANK_CNT;
    int y2 = VPOS_CNT - 1;
    
    foreground.init(w, h, emuTex);
    foreground.setTextureRect(sf::IntRect(x1, y1, x2 - x1, y2 - y1));

    click.setBuffer(app.assets.get(SoundID::click));
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
            if (!OS::gotMouse) { OS::retainMouse(); return; }
            
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
            if (!OS::gotMouse) { return; }
            
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
    if (OS::gotMouse && OS::mouseMoved()) {
        mouseMoved(OS::mouseDX, OS::mouseDY);
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
    foreground.setFillColor(sf::Color(0xFF,0xFF,0xFF,alpha));
    
    app.window.draw(foreground);
}

void
Canvas::mouseMoved(int dx, int dy)
{
    app.amiga.controlPort1.mouse.setDeltaXY(dx, dy);
}
