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

enum class InputDeviceType { NULLDEVICE, MOUSE, JOYSTICK, KEYSET };

class InputDevice : public GUIComponent {
    
protected:
    
    isize nr;
    string name;

public:
    
    InputDevice(class Application &ref, isize n) : GUIComponent(ref), nr(n) { }
    
    // Getters and setters
    isize getNr() { return nr; }
    const string &getName() { return name; }
    void setName(const string &s) { name = s; }
    
    // Informs about the device type
    virtual InputDeviceType type() = 0;
    bool isMouse() { return type() == InputDeviceType::MOUSE; }
    bool isJoystick() { return type() == InputDeviceType::JOYSTICK; }
    bool isKeyset() { return type() == InputDeviceType::KEYSET; }
    
    // Polls a device and updates the Amiga control port accordingly
    virtual void poll(ControlPort &port) { };
};

class MouseDevice : public InputDevice {
        
public:
        
    bool leftButton = false;
    bool rightButton = false;
    int mouseDX = 0;
    int mouseDY = 0;
    
public:
    
    using InputDevice::InputDevice;
    
    InputDeviceType type() override { return InputDeviceType::MOUSE; }
    void poll(ControlPort &port) override;
};

class NullDevice : public InputDevice {

public:
    
    using InputDevice::InputDevice;
    
    InputDeviceType type() override { return InputDeviceType::NULLDEVICE; }
};

class JoystickDevice : public InputDevice {

public:
    
    using InputDevice::InputDevice;
    
    InputDeviceType type() override { return InputDeviceType::JOYSTICK; }
    void poll(ControlPort &port) override;
};

class KeysetDevice : public InputDevice {
    
public:
    
    sf::Keyboard::Key left, right, up, down, fire;

    using InputDevice::InputDevice;
    
    InputDeviceType type() override { return InputDeviceType::KEYSET; }
    void poll(ControlPort &port) override;
};


//
// Input manager
//

class InputManager : public GUIComponent {
  
    static const isize numMouseSlots = 3;
    static const isize numJoystickSlots = 3;
    static const isize numKeysetSlots = 2;

    // Available input devices (all types)
    std::vector<InputDevice *> devices;
    
    // Available input devices of a specific type
    std::vector<MouseDevice *> mice;
    std::vector<JoystickDevice *> joysticks;
    std::vector<KeysetDevice *> keysets;

    // Connected devices
    isize connectedDevice[2] = { 0, 0 };

    sf::Vector2i mouseCenter;

public:

    bool gotMouse = false;

public:
    
    InputManager(Application &ref);
    
    // Returns the number of available devices
    isize numMice() { return mice.size(); }
    isize numJoysticks() { return joysticks.size(); }
    isize numKeysets() { return keysets.size(); }
    
    // Returns the currently connected device
    InputDevice &device(PortNr port);
    
    // Connects a device
    void connect(InputDeviceType type, isize nr, PortNr port) throws;
    void disconnect(PortNr port) { connect(InputDeviceType::NULLDEVICE, 0, port); }
    void flipPortDevice(PortNr port);
        
    // Returns the name of the connected device
    [[deprecated]] string getName(PortNr port);
    
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
