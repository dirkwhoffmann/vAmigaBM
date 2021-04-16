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

#ifdef __MACH__
#include <ApplicationServices/ApplicationServices.h>
#endif

void
MouseDevice::poll(ControlPort &port)
{
    if (mouseDX != 0 || mouseDY != 0) {
        
        printf("dx: %d dy: %d b: %d %d\n", mouseDX, mouseDY, leftButton, rightButton);
        port.mouse.setDxDy(mouseDX, mouseDY);
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
    // Add a placeholder for an unconnected control port
    devices.push_back(new NullDevice(app, 0));
    devices.back()->setName("Null device");
    
    //
    // Initialize mice
    //
    
    switch (mouseEmulation) {
            
        case MouseEmulation::MANY:
        {
            int count = ManyMouse_Init();
            
            if (count < 0) {
                throw std::runtime_error("Unable to initialize manymouse library");
            }
            
            printf("ManyMouse driver: %s\n", ManyMouse_DriverName());
            for (int i = 0; i < numMouseSlots; i++) {

                if (i >= count) break;
                
                auto device = new MouseDevice(app, i + 1);
                device->setName(ManyMouse_DeviceName(i + 1));
                devices.push_back(device);
                mice.push_back(device);
                
                printf("%d: %s\n", i + 1, devices.back()->getName().c_str());
            }
            break;
        }
        case MouseEmulation::MACH:
        {
            auto device = new MouseDevice(app, 1);
            device->setName("Mac Mouse");
            devices.push_back(device);
            mice.push_back(device);
            break;
        }
        case MouseEmulation::SFML:
        {
            auto device = new MouseDevice(app, 1);
            device->setName("Mouse");
            devices.push_back(device);
            mice.push_back(device);
            break;
        }
    }

    //
    // Initialize joysticks
    //
    
    for (int i = 0; i < numJoystickSlots; i++) {
        
        if (!sf::Joystick::isConnected(i)) break;
        
        auto properties = sf::Joystick::getIdentification(i);
        auto device = new JoystickDevice(app, i + 1);
        device->setName(properties.name);
        devices.push_back(device);
        joysticks.push_back(device);
    }
    
    //
    // Initialize keysets
    //

    auto keyset1 = new KeysetDevice(app, 1);
    keyset1->setName("Joystick keyset 1");
    keyset1->left = sf::Keyboard::Key::Left;
    keyset1->right = sf::Keyboard::Key::Right;
    keyset1->up = sf::Keyboard::Key::Up;
    keyset1->down = sf::Keyboard::Key::Down;
    keyset1->fire = sf::Keyboard::Key::Space;
    devices.push_back(keyset1);
    keysets.push_back(keyset1);
    
    auto keyset2 = new KeysetDevice(app, 2);
    keyset2->setName("Joystick keyset 2");
    keyset2->left = sf::Keyboard::Key::A;
    keyset2->right = sf::Keyboard::Key::S;
    keyset2->up = sf::Keyboard::Key::W;
    keyset2->down = sf::Keyboard::Key::Y;
    keyset2->fire = sf::Keyboard::Key::X;
    devices.push_back(keyset2);
    keysets.push_back(keyset1);
}

InputDevice &
InputManager::device(PortNr port)
{
    auto i = port == PORT_1 ? 0 : 1;
    return *devices[connectedDevice[i]];
}

void
InputManager::connect(InputDeviceType type, isize nr, PortNr port)
{
    assert_enum(PortNr, port);
    
    for (isize i = 0; i < devices.size(); i++) {
        if (devices[i]->type() == type && devices[i]->getNr() == nr) {
            connectedDevice[port == PORT_1 ? 0 : 1] = i;
            return;
        }
    }
        
    throw ConfigArgError("No such device");
}

void
InputManager::flipPortDevice(PortNr port)
{
    isize i = port == PORT_1 ? 0 : 1;
    
    if (connectedDevice[i] + 1 < devices.size()) {
        connectedDevice[i]++;
    } else {
        connectedDevice[i] = 0;
    }
}

string
InputManager::getName(PortNr port)
{
    auto nr = connectedDevice[port == PORT_1 ? 0 : 1];
    return devices[nr]->getName();
}

void
InputManager::poll()
{
    // Only proceed if we own the mouse
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
                                mice[event.device]->mouseDX = event.value;
                            } else {
                                printf("MM: dy(%u) = %d\n", event.device, event.value);
                                mice[event.device]->mouseDY = event.value;
                            }
                        }
                        break;
                        
                    case MANYMOUSE_EVENT_BUTTON:
                        
                        if (event.item == 0) {
                            printf("MM: Left button = %d\n", event.item);
                            mice[event.device]->leftButton = event.value;
                        } else {
                            printf("MM: Right button = %d\n", event.item);
                            mice[event.device]->rightButton = event.value;
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
            CGGetLastMouseDelta(&mice[0]->mouseDX, &mice[0]->mouseDY);
            mice[0]->leftButton = CGEventSourceButtonState(state, kCGMouseButtonLeft);
            mice[0]->rightButton = CGEventSourceButtonState(state, kCGMouseButtonRight);
            mice[0]->leftButton = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            mice[0]->rightButton = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
#endif
            break;
        }
        case MouseEmulation::SFML:
        {
            auto current = sf::Mouse::getPosition(app.window);
            auto center = app.inputManager.mouseCenter;
            
            mice[0]->mouseDX = current.x - center.x;
            mice[0]->mouseDY = current.y - center.y;
            mice[0]->leftButton = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
            mice[0]->rightButton = sf::Mouse::isButtonPressed(sf::Mouse::Button::Right);
            
            if (mice[0]->mouseDX || mice[0]->mouseDY) {
                printf("SFML: %d %d\n", mice[0]->mouseDX, mice[0]->mouseDY);
                sf::Mouse::setPosition(center, app.window);
            }
            break;
        }
    }
     
    devices[connectedDevice[0]]->poll(amiga.controlPort1);
    devices[connectedDevice[1]]->poll(amiga.controlPort2);
}

void
InputManager::listDevices()
{
    for (isize i = 0; i < devices.size(); i++) {
    
        printf("Device %zd: %s\n", i, devices[i]->getName().c_str());
        amiga.retroShell << "Device " << i << ": " << devices[i]->getName();
        
        if (connectedDevice[0] == i) {
            amiga.retroShell << " (connected to port 1)";
        }
        if (connectedDevice[1] == i) {
            amiga.retroShell << " (connected to port 2)";
        }
        amiga.retroShell << '\n';
    }
}

void
InputManager::retainMouse()
{
    // Only proceed if we haven't gotten the mouse yet
    if (gotMouse) return;
            
    // Place the cursor in the middle of the screen
    mouseCenter = sf::Vector2i((app.window.getSize().x / 2) & ~1,
                               (app.window.getSize().y / 2) & ~1);
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
