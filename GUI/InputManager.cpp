// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

void
MouseDevice::poll(ControlPort &port)
{
    if (!manyMouse) {
        
        auto current = sf::Mouse::getPosition(app.window);
        auto center = app.inputManager.mouseCenter;

        mouseDX = current.x - center.x;
        mouseDY = current.y - center.y;
        
        sf::Mouse::setPosition(center, app.window);
    }
    
    if (mouseDX != 0 || mouseDY != 0) {
        
        printf("dx: %d dy: %d\n", mouseDX, mouseDY);
        port.mouse.setDeltaXY(mouseDX, mouseDY);
    }
}

void
JoystickDevice::poll(ControlPort &port)
{
    if (!sf::Joystick::isConnected((int)nr)) return;
    
    auto x =
    sf::Joystick::getAxisPosition((int)nr, sf::Joystick::Axis::X);
    auto y =
    sf::Joystick::getAxisPosition((int)nr, sf::Joystick::Axis::Y);
    bool fire =
    sf::Joystick::isButtonPressed((int)nr, 0) ||
    sf::Joystick::isButtonPressed((int)nr, 1);
    
    port.joystick.trigger(fire ? PRESS_FIRE : RELEASE_FIRE);
    port.joystick.trigger(x < -10 ? PULL_LEFT : x > 10 ? PULL_RIGHT : RELEASE_X);
    port.joystick.trigger(y < -10 ? PULL_UP : y > 10 ? PULL_DOWN : RELEASE_Y);
}

void
KeysetDevice::poll(ControlPort &port)
{
    port.joystick.trigger(sf::Keyboard::isKeyPressed(left) ? PULL_LEFT :
                          sf::Keyboard::isKeyPressed(right) ? PULL_RIGHT : RELEASE_X);
    port.joystick.trigger(sf::Keyboard::isKeyPressed(up) ? PULL_UP :
                          sf::Keyboard::isKeyPressed(down) ? PULL_DOWN : RELEASE_Y);
    port.joystick.trigger(sf::Keyboard::isKeyPressed(fire) ? PRESS_FIRE : RELEASE_FIRE);
}

InputManager::InputManager(Application &ref) : GUIComponent(ref)
{
    //
    // Initialize mice
    //
    
    if (manyMouse) {
        
        int numMice = ManyMouse_Init();

        if (numMice < 0) {
            throw std::runtime_error("Unable to initialize manymouse library");
        }

        for (int i = 0; i < numMice && i < numMouseSlots; i++) {

            mouse[i].isPresent = true;
            mouse[i].name = ManyMouse_DeviceName(i);
        }

        printf("ManyMouse driver: %s\n", ManyMouse_DriverName());

    } else {
        
        mouse[0].isPresent = true;
        mouse[0].name = "Mouse";
    }

    //
    // Initialize joysticks
    //

    for (int i = 0; i < numJoystickSlots; i++) {
        
        if (sf::Joystick::isConnected(i)) {

            auto properties = sf::Joystick::getIdentification(i);

            joystick[i].isPresent = true;
            joystick[i].name = properties.name;
        }
    }
    
    //
    // Initialize keysets
    //

    keyset[0].isPresent = true;
    keyset[0].name = "Joystick keyset 1";
    keyset[0].left = sf::Keyboard::Key::Left;
    keyset[0].right = sf::Keyboard::Key::Right;
    keyset[0].up = sf::Keyboard::Key::Up;
    keyset[0].down = sf::Keyboard::Key::Down;
    keyset[0].fire = sf::Keyboard::Key::Space;
}

isize
InputManager::mouseCount()
{
    isize count;
    for (count = 0; count < numMouseSlots && mouse[count].isPresent; count++);
    return count;
}

isize
InputManager::joystickCount()
{
    isize count;
    for (count = 0; count < numJoystickSlots && joystick[count].isPresent; count++);
    return count;
}

isize
InputManager::keysetCount()
{
    isize count;
    for (count = 0; count < numKeysetSlots && keyset[count].isPresent; count++);
    return count;
}

void
InputManager::connect(InputDevice *device, PortNr port)
{
    assert_enum(PortNr, port);

    if (port == PORT_1) {
        
        port1 = device;
        if (port1 == port2) port2 = nullptr;
    
    } else {
        
        port2 = device;
        if (port1 == port2) port1 = nullptr;
    }
}

void
InputManager::connectMouse(isize nr, PortNr port)
{
    assert((usize)nr < numMouseSlots);
    connect(&mouse[nr], port);
}

void
InputManager::connectJoystick(isize nr, PortNr port)
{
    assert((usize)nr < numJoystickSlots);
    connect(&joystick[nr], port);
}

void
InputManager::connectKeyset(isize nr, PortNr port)
{
    assert((usize)nr < numKeysetSlots);
    connect(&keyset[nr], port);
}

string
InputManager::getName(PortNr port)
{
    if (port == PORT_1) return port1 ? port1->name : "None";
    if (port == PORT_2) return port2 ? port2->name : "None";

    assert(false);
    return "";
}

void
InputManager::poll()
{
    if (manyMouse) {
        ManyMouseEvent mmevent;
        while (ManyMouse_PollEvent(&mmevent)) {
            if (mmevent.type == MANYMOUSE_EVENT_RELMOTION) {
                printf("Mouse #%u relative motion %s %d\n", mmevent.device, mmevent.item == 0 ? "X" : "Y", mmevent.value);
                
                if (mmevent.device < numMouseSlots) {
                    if (mmevent.item == 0) {
                        mouse[mmevent.device].mouseDX = mmevent.value;
                    } else {
                        mouse[mmevent.device].mouseDY = mmevent.value;
                    }
                }
                
            } else if (mmevent.type == MANYMOUSE_EVENT_ABSMOTION) { // I have never witnessed this event
                printf("Mouse #%u absolute motion %s %d\n", mmevent.device, mmevent.item == 0 ? "X" : "Y", mmevent.value);
            } else if (mmevent.type == MANYMOUSE_EVENT_BUTTON) {
                printf("Mouse #%u button %u %s\n", mmevent.device, mmevent.item, mmevent.value ? "down" : "up");
            } else if (mmevent.type == MANYMOUSE_EVENT_DISCONNECT) {
                printf("Mouse #%u disconnect\n", mmevent.device);
            } else {
                printf("Mouse #%u unhandled event type %d\n", mmevent.device, mmevent.type);
            }
        }
    }
    
    if (port1) port1->poll(amiga.controlPort1);
    if (port2) port2->poll(amiga.controlPort2);
}

void
InputManager::retainMouse()
{
    gotMouse = true;
    
    app.window.setMouseCursorGrabbed(true);
    app.window.setMouseCursorVisible(false);
    
    mouseCenter = sf::Vector2i(app.window.getSize().x / 2,
                               app.window.getSize().y / 2);
    
    printf("mouseCenter: x = %d y = %d\n", mouseCenter.x, mouseCenter.y);
    sf::Mouse::setPosition(mouseCenter, app.window);
    
    /*
#ifdef __MACH__
    if (!gotMouse) {
    
        CGDisplayHideCursor(kCGNullDirectDisplay);
        CGEventErr err = CGAssociateMouseAndMouseCursorPosition(false);
        if (err != CGEventNoErr) {
            printf("CGAssociateMouseAndMouseCursorPosition returned %d\n", err);
        }
        gotMouse = true;
    }
#endif
     */
}

void
InputManager::releaseMouse()
{
    if (gotMouse) {

        app.window.setMouseCursorGrabbed(false);
        app.window.setMouseCursorVisible(true);
        gotMouse = false;
    }
    /*
#ifdef __MACH__
    
    if (gotMouse) {
        
        CGDisplayShowCursor(kCGNullDirectDisplay);
        CGAssociateMouseAndMouseCursorPosition(true);
        gotMouse = false;
    }
#endif
     */
}
