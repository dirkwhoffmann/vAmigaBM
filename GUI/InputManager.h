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

public:
    
    InputDevice(class Application &ref, isize n) : GUIComponent(ref), nr(n) { }
    
    // Informs about the device type
    virtual bool isMouse() { return false; }
    virtual bool isJoystick() { return false; }
    virtual bool isKeyset() { return false; }

    // Polls a device and updates the Amiga control port accordingly
    virtual void poll(ControlPort &port) = 0;
};

class MouseDevice : public InputDevice {
        
public:
        
    bool leftButton = false;
    bool rightButton = false;
    int mouseDX = 0;
    int mouseDY = 0;
    
public:
    
    using InputDevice::InputDevice;
    
    bool isMouse() override { return true; }
    void poll(ControlPort &port) override;
};

class JoystickDevice : public InputDevice {

public:
    
    using InputDevice::InputDevice;
    
    bool isJoystick() override { return true; }
    void poll(ControlPort &port) override;
};

class KeysetDevice : public InputDevice {
    
public:
    
    sf::Keyboard::Key left, right, up, down, fire;

    using InputDevice::InputDevice;
    
    bool isKeyset() override { return true; }
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

    // Number of available devices
    isize numMice = 0;
    isize numJoysticks = 0;
    isize numKeysets = 0;

    // Connected devices
    InputDevice *port1 = nullptr;
    InputDevice *port2 = nullptr;

    sf::Vector2i mouseCenter;

public:

    bool gotMouse = false;

public:
    
    InputManager(Application &ref);
    
    // Returns the number of available devices
    isize getNumMice() { return numMice; }
    isize getNumJoysticks() { return numJoysticks; }
    isize getNumKeysets() { return numKeysets; }
    
    // Connects a device
    void connect(InputDevice *device, PortNr port);
    void connectMouse(isize nr, PortNr port) throws;
    void connectJoystick(isize nr, PortNr port) throws;
    void connectKeyset(isize nr, PortNr port) throws;
    void flipPortDeviceType(PortNr port);
    void flipPortDeviceNumber(PortNr port);
    
    // Checks the type of the connected device
    bool isMouse(PortNr port);
    bool isJoystick(PortNr port);
    bool iskeyset(PortNr port);
    
    // Disconnects a device
    void disconnect(PortNr port) { connect(nullptr, port); }

    // Returns the name of the connected device
    string getName(PortNr port);
    
    // Polls the currently connected devices
    void poll();
    
    // Prints a list of available devices in the debug console
    void listDevices();
    
    // Retains or releases the mouse
    void retainMouse();
    void releaseMouse();
    
private:
    
    // Locks or hides the mouse
    void lockMouse(bool value);
    void hideMouse(bool value);
};
