// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "GUIComponent.h"
#include "ControlPort.h"

//
// Input devices
//

class InputDevice : public GUIComponent {

protected:
    
    isize nr;
    
public:
    
    string name;
    bool isPresent = false;

public:
    
    InputDevice(class Application &ref, isize n) : GUIComponent(ref), nr(n) { }
    
    // Polls a device and updates the Amiga control port accordingly
    virtual void poll(ControlPort &port) = 0;
};

class MouseDevice : public InputDevice {
        
public:
    
    int mouseCenterX = 200;
    int mouseCenterY = 200;
    
    int mouseDX = 0;
    int mouseDY = 0;
    
public:
    
    using InputDevice::InputDevice;
    
    void poll(ControlPort &port) override;
};

class JoystickDevice : public InputDevice {

public:
    
    using InputDevice::InputDevice;
    
    void poll(ControlPort &port) override;
};

class KeysetDevice : public InputDevice {
    
public:
    
    sf::Keyboard::Key left, right, up, down, fire;

    using InputDevice::InputDevice;
    
    void poll(ControlPort &port) override;
};


//
// Input manager
//

class InputManager : public GUIComponent {
  
    static const isize numMouseSlots = 3;
    static const isize numJoystickSlots = 3;
    static const isize numKeysetSlots = 2;

    MouseDevice mouse[numMouseSlots] = {
        MouseDevice(app, 0),
        MouseDevice(app, 1),
        MouseDevice(app, 2)
    };
    
    JoystickDevice joystick[numJoystickSlots] = {
        JoystickDevice(app, 0),
        JoystickDevice(app, 1),
        JoystickDevice(app, 2),
    };
    
    KeysetDevice keyset[numKeysetSlots] = {
        KeysetDevice(app, 0),
        KeysetDevice(app, 1)
    };

    // Connected devices
    InputDevice *port1 = nullptr;
    InputDevice *port2 = nullptr;

public:
    
    InputManager(Application &ref);
    
    // Returns the number of available devices
    isize mouseCount();
    isize joystickCount();
    isize keysetCount();
    
    // Connects a device
    void connect(InputDevice *device, PortNr port);
    void connectMouse(isize nr, PortNr port);
    void connectJoystick(isize nr, PortNr port);
    void connectKeyset(isize nr, PortNr port);
    
    // Disconnects a device
    void disconnect(PortNr port) { connect(nullptr, port); }

    // Polls the currently connected devices
    void poll();
    
};
