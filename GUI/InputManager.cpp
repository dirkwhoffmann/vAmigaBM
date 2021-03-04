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
    sf::Vector2i current = sf::Mouse::getPosition(app.window);
    
    mouseDX = current.x - mouseCenterX;
    mouseDY = current.y - mouseCenterY;
    
    if (mouseDX != 0 || mouseDY != 0) {

        // printf("lockX: %d lockY: %d current: (%d,%d)\n",
        //        mouseLockX, mouseLockY, current.x, current.y);
        printf("dx: %d dy: %d\n", mouseDX, mouseDY);
        
        sf::Mouse::setPosition(sf::Vector2i(mouseCenterX, mouseCenterY), app.window);
    }
}

void
JoystickDevice::poll(ControlPort &port)
{
    if (sf::Joystick::isConnected((int)nr)) {
        if (sf::Joystick::isButtonPressed((int)nr, 0) ||
            sf::Joystick::isButtonPressed((int)nr, 1)) {
            printf("BUTTON %zd\n", nr);
        }
    }
    auto x = sf::Joystick::getAxisPosition((int)nr, sf::Joystick::Axis::X);
    auto y = sf::Joystick::getAxisPosition((int)nr, sf::Joystick::Axis::Y);
    
    if (abs(x) > 10) {
        if (x < 0) printf("LEFT\n");
        if (x > 0) printf("RIGHT\n");
    }
    if (abs(y) > 10) {
        if (y < 0) printf("UP\n");
        if (y > 0) printf("DOWN\n");
    }
}

void
KeysetDevice::poll(ControlPort &port)
{
    if (sf::Keyboard::isKeyPressed(left)) {
        printf("KEY LEFT\n");
    }
    if (sf::Keyboard::isKeyPressed(right)) {
        printf("KEY RIGHT\n");
    }
    if (sf::Keyboard::isKeyPressed(up)) {
        printf("KEY UP\n");
    }
    if (sf::Keyboard::isKeyPressed(down)) {
        printf("KEY DOWN\n");
    }
    if (sf::Keyboard::isKeyPressed(fire)) {
        printf("KEY FIRE\n");
    }
}

InputManager::InputManager(Application &ref) : GUIComponent(ref)
{
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

void
InputManager::poll()
{
    if (port1) port1->poll(amiga.controlPort1);
    if (port2) port2->poll(amiga.controlPort2);
}
