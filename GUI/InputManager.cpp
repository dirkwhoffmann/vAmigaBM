// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

#ifdef __MACH__
#include <ApplicationServices/ApplicationServices.h>
#endif

void
MouseDevice::poll(ControlPort &port)
{
    if (mouseDX != 0 || mouseDY != 0) {
        
        printf("dx: %d dy: %d b: %d %d\n", mouseDX, mouseDY, leftButton, rightButton);
        port.mouse.setDeltaXY(mouseDX, mouseDY);
    }
    port.mouse.setLeftButton(leftButton);
    port.mouse.setRightButton(rightButton);
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
    
    switch (mouseEmulation) {
            
        case MouseEmulation::MANY:
        {
            int numMice = ManyMouse_Init();
            
            if (numMice < 0) {
                throw std::runtime_error("Unable to initialize manymouse library");
            }
            
            printf("ManyMouse driver: %s\n", ManyMouse_DriverName());
            for (int i = 0; i < numMice && i < numMouseSlots; i++) {

                mouse[i].isPresent = true;
                mouse[i].name = ManyMouse_DeviceName(i);
                printf("%d: %s\n", i, mouse[i].name.c_str());
            }
            
            break;
        }
        case MouseEmulation::MACH:
        {
            mouse[0].isPresent = true;
            mouse[0].name = "Mac Mouse";
            break;
        }
        case MouseEmulation::SFML:
        {
            mouse[0].isPresent = true;
            mouse[0].name = "Mouse";
            break;
        }
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
    // Only proceed if own the mouse
    if (!gotMouse) return;
    
    switch (mouseEmulation) {
            
        case MouseEmulation::MANY:
        {
            ManyMouseEvent event;
            while (ManyMouse_PollEvent(&event)) {
                
                switch (event.type) {
                        
                    case MANYMOUSE_EVENT_RELMOTION:
                        
                        if (event.device < numMouseSlots) {
                            if (event.item == 0) {
                                printf("MM: dx(%u) = %d\n", event.device, event.value);
                                mouse[event.device].mouseDX = event.value;
                            } else {
                                printf("MM: dy(%u) = %d\n", event.device, event.value);
                                mouse[event.device].mouseDY = event.value;
                            }
                        }
                        break;
                        
                    case MANYMOUSE_EVENT_BUTTON:
                        
                        if (event.item == 0) {
                            printf("MM: Left button = %d\n", event.item);
                            mouse[event.device].leftButton = event.value;
                        } else {
                            printf("MM: Right button = %d\n", event.item);
                            mouse[event.device].rightButton = event.value;
                        }
                        break;
                        
                    default:
                        break;
                }
            }
            break;
        }
        case MouseEmulation::MACH:
        {
#ifdef __MACH__
            auto state = kCGEventSourceStateCombinedSessionState;
            CGGetLastMouseDelta(&mouse[0].mouseDX, &mouse[0].mouseDY);
            mouse[0].leftButton = CGEventSourceButtonState(state, kCGMouseButtonLeft);
            mouse[0].rightButton = CGEventSourceButtonState(state, kCGMouseButtonRight);
            mouse[0].leftButton = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            mouse[0].rightButton = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
#endif
            break;
        }
        case MouseEmulation::SFML:
        {
            auto current = sf::Mouse::getPosition(app.window);
            auto center = app.inputManager.mouseCenter;
            
            mouse[0].mouseDX = current.x - center.x;
            mouse[0].mouseDY = current.y - center.y;
            mouse[0].leftButton = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            mouse[0].rightButton = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            
            if (mouse[0].mouseDX || mouse[0].mouseDY) {
                printf("SFML: %d %d\n", mouse[0].mouseDX, mouse[0].mouseDY);
                sf::Mouse::setPosition(center, app.window);
            }
            break;
        }
    }
            
    if (port1) port1->poll(amiga.controlPort1);
    if (port2) port2->poll(amiga.controlPort2);
}

void
InputManager::retainMouse()
{
    // Only proceed if we haven't gotten the mouse yet
    if (gotMouse) return;
            
    // Place the cursor in the middle of the screen
    mouseCenter = sf::Vector2i(app.window.getSize().x / 2,
                               app.window.getSize().y / 2);
    sf::Mouse::setPosition(mouseCenter, app.window);
    
    // Hide and disconnect the mouse cursor
    hideMouse(true);
    lockMouse(true);
    
    gotMouse = true;
}

void
InputManager::releaseMouse()
{
    // Only proceed if we've got the mouse
    if (!gotMouse) return;
    
    // Show and reconnect the mouse cursor
    hideMouse(false);
    lockMouse(false);

    gotMouse = false;
}

void
InputManager::lockMouse(bool value)
{
    if (mouseEmulation != MouseEmulation::MACH) {
        
        app.window.setMouseCursorGrabbed(value);
        return;
    }
    
#ifdef __MACH__
    CGAssociateMouseAndMouseCursorPosition(!value);
#endif
}

void
InputManager::hideMouse(bool value)
{
    if (mouseEmulation != MouseEmulation::MACH) {
        
        app.window.setMouseCursorVisible(!value);
        return;
    }
    
#ifdef __MACH__
    if (value) {
        CGDisplayHideCursor(kCGNullDirectDisplay);
    } else {
        CGDisplayShowCursor(kCGNullDirectDisplay);
    }
#endif
}
