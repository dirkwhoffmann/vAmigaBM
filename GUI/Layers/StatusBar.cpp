// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

StatusBar::StatusBar(Application &ref) : Layer(ref) 
{
    
}

StatusBar::~StatusBar()
{
    
}

void StatusBar::init()
{
    Layer::init();
    
    delay = 0.5;
    auto grey6 = sf::Color(0x60,0x60,0x60);
    auto greyD = sf::Color(0xD0,0xD0,0xD0);
    auto greyF = sf::Color(0xF0,0xF0,0xF0);
    
    bar.setColors(greyD, greyD, greyF, greyF);

    powerLed.init(app.assets.get(TextureID::ledBlack));
    powerLed.setW(scale(24));

    for (int i = 0; i < 4; i++) {
        
        driveLed[i].init(app.assets.get(TextureID::ledGreen));
        driveLed[i].setW(scale(24));
        cylinder[i].setStyle(app.assets.get(FontID::sans_r), scale(14), grey6);
        cylinder[i].setString("00");
        disk[i].init(app.assets.get(TextureID::disk));
        disk[i].setW(scale(18));
        disk[i].rectangle.setFillColor(grey6);
        spin[i].init(app.assets);
        spin[i].setW(scale(18));
        spin[i].rectangle.setFillColor(grey6);
    }
    
    for (int i = 0; i < 2; i++) {
        
        port[i].init(app.assets.get(TextureID::none));
        port[i].setW(scale(18));
        port[i].rectangle.setFillColor(grey6);
        port[i].setFlags(Align::Top | Align::Right);
        portNr[i].setStyle(app.assets.get(FontID::sans_r), scale(14), grey6);
        portNr[i].setString("0");
        portNr[i].setFlags(Align::Top | Align::Right);
    }
    
    mute.setFlags(Align::Top | Align::Right);
    mute.init(app.assets.get(TextureID::mute));
    mute.setW(scale(18));
    mute.rectangle.setFillColor(grey6);
    mhz.setStyle(app.assets.get(FontID::sans_sb), scale(12), grey6);
    mhz.setFlags(Align::Top | Align::Right);
    state.init(app.assets.get(TextureID::sync));
    state.setW(scale(18));
    state.rectangle.setFillColor(grey6);
    state.setFlags(Align::Top | Align::Right);
    
    needsUpdate = (u32)-1;
}

void
StatusBar::awake()
{

}

void
StatusBar::update(u64 frames, sf::Time dt)
{
    Layer::update(frames, dt);
 
    if (needsUpdate) refresh();
    for (isize i = 0; i < 4; i++) spin[i].step();
}

void
StatusBar::render()
{
    bar.draw(app.window);
    
    powerLed.draw(app.window);

    for (int i = 0; i < 4; i++) {
        
        driveLed[i].draw(app.window);
        cylinder[i].draw(app.window);
        disk[i].draw(app.window);
        spin[i].draw(app.window);
    }
    port[0].draw(app.window);
    portNr[0].draw(app.window);
    port[1].draw(app.window);
    portNr[1].draw(app.window);
    mute.draw(app.window);
    mhz.draw(app.window);
    state.draw(app.window);
}

void
StatusBar::respond(const sf::Event &event)
{
    
}

void
StatusBar::resize(float width, float height)
{
    auto size = app.window.getSize();
    
    bar.setSize(size.x, scale(26));
    bar.setPosition(0, size.y - scale(26));

    float pad = scale(6);
    float pos = scale(9);
    float y = size.y - scale(26);
    
    //
    // Left aligned items
    //
        
    powerLed.setPosition(pos, y + scale(7));  pos += powerLed.getW() + pad;
    
    for (int i = 0; i < 4; i++) {
        
        driveLed[i].setPosition(pos, y + scale(7)); pos += driveLed[i].getW() + pad;
        cylinder[i].setPosition(pos, y + scale(3)); pos += cylinder[i].getW() + pad;
        disk[i].setPosition(pos, y + scale(4)); pos += disk[i].getW() + pad;
        spin[i].setPosition(pos, y + scale(4)); pos += spin[i].getW() + pad;
    }
    
    //
    // Right aligned items
    //
    
    pos = app.window.getSize().x - scale(18);

    portNr[1].setPosition(pos, y + scale(4)); pos -= portNr[1].getW() + pad / 2;
    port[1].setPosition(pos, y + scale(4)); pos -= port[1].getW() + pad;
    port[0].setPosition(pos, y + scale(4)); pos -= port[0].getW() + pad / 2;
    portNr[0].setPosition(pos, y + scale(4)); pos -= portNr[1].getW() + pad;
    pos -= pad;
    state.setPosition(pos, y + scale(4)); pos -= state.getW() + pad;
    mhz.setPosition(pos, y + scale(5)); pos -= scale(64) + pad;
    mute.setPosition(pos, y + scale(4));
}

void
StatusBar::alphaDidChange()
{
    bar.setAlpha(alpha);
    powerLed.setAlpha(alpha);
    
    for (int i = 0; i < 4; i++) {
        
        driveLed[i].setAlpha(alpha);
        disk[i].setAlpha(alpha);
        spin[i].setAlpha(alpha);
        cylinder[i].setAlpha(alpha);
    }
    
    mhz.setAlpha(alpha);
    state.setAlpha(alpha);
    port[0].setAlpha(alpha);
    port[1].setAlpha(alpha);
    portNr[0].setAlpha(alpha);
    portNr[1].setAlpha(alpha);
}

bool
StatusBar::mouseButtonPressed(isize button)
{
    // Get the relative mouse position
    auto position = sf::Mouse::getPosition(app.window);

    // Only proceed if the mouse is located inside the status bar
    if (!isVisible() || !bar.contains(position)) return false;

    if (button == 0) {

        for (isize i = 0; i < 4; i++) {
            if (disk[i].isVisible && disk[i].contains(position)) {
                fileBrowser.open(i);
                return true;
            }
        }

        if (state.contains(position)) {
            controller.flipWarpMode();
            return true;
        }

        if (port[0].contains(position)) {
            inputManager.flipPortDevice(PORT_1);
            needsUpdate |= StatusBarItem::PORTS;
            return true;
        }

        if (port[1].contains(position)) {
            inputManager.flipPortDevice(PORT_2);
            needsUpdate |= StatusBarItem::PORTS;
            return true;
        }
    }
    
    return true;
}

void
StatusBar::refresh()
{
    if (needsUpdate & StatusBarItem::POWER_LED) {
        if (amiga.isPoweredOff()) {
            powerLed.rectangle.setTexture(&app.assets.get(TextureID::ledBlack));
        } else if (amiga.ciaA.getPA() & 2) {
            powerLed.rectangle.setTexture(&app.assets.get(TextureID::ledGrey));
        } else {
            powerLed.rectangle.setTexture(&app.assets.get(TextureID::ledRed));
        }
    }

    refreshDrive(0);
    refreshDrive(1);
    refreshDrive(2);
    refreshDrive(3);
    
    if (needsUpdate & StatusBarItem::MUTE) {
        mute.isVisible = amiga.inWarpMode();
    }

    if (needsUpdate & StatusBarItem::STATE) {
        refreshState();
    }
    
    if (needsUpdate & StatusBarItem::PORTS) {
        refreshPort(PORT_1);
        refreshPort(PORT_2);
    }

    needsUpdate = 0;
}

void
StatusBar::refreshMhz(float value)
{
    char tmp[16];
    sprintf(tmp, "%3.2f MHz", value);

    if (mhz.text.getString() != tmp) {
        mhz.setString(string(tmp));
    }
}

void
StatusBar::refreshDrive(isize nr)
{
    char tmp[16];
    
    // Only proceed if there is something to update
    if (!needsUpdate) return;
    
    // Update visibility of all elements
    bool connected = amiga.paula.diskController.getConfigItem(OPT_DRIVE_CONNECT, nr);
    driveLed[nr].isVisible = connected;
    cylinder[nr].isVisible = connected;
    disk[nr].isVisible = connected;
    spin[nr].isVisible = connected && amiga.df[nr]->getMotor();

    // Update the drive icon
    if (amiga.df[nr]->hasDisk()) {
        disk[nr].rectangle.setTexture(&app.assets.get(TextureID::disk));
    } else {
        disk[nr].rectangle.setTexture(&app.assets.get(TextureID::none));
    }
    
    // Update the head position
    if (needsUpdate & (StatusBarItem::DRIVE_CYL << nr)) {

        sprintf(tmp, "%02d", amiga.df[nr]->getCylinder());
        cylinder[nr].setString(string(tmp));
    }
}

void
StatusBar::refreshPort(PortNr nr)
{
    auto i = nr == PORT_1 ? 0 : 1;
    InputDevice &device = inputManager.device(nr);
    isize devNr = device.getNr();
    
    portNr[i].setString("");
    
    switch (device.type()) {

        case InputDeviceType::NULLDEVICE:
            port[i].rectangle.setTexture(&app.assets.get(TextureID::none));
            portNr[i].setString("");
            break;
            
        case InputDeviceType::MOUSE:
            port[i].rectangle.setTexture(&app.assets.get(TextureID::mouse));
            if (inputManager.numMice() > 1) portNr[i].setString(std::to_string(devNr));
            break;

        case InputDeviceType::JOYSTICK:
            port[i].rectangle.setTexture(&app.assets.get(TextureID::joystick));
            if (inputManager.numJoysticks() > 1) portNr[i].setString(std::to_string(devNr));
            break;

        case InputDeviceType::KEYSET:
            port[i].rectangle.setTexture(&app.assets.get(TextureID::keyset));
            if (inputManager.numKeysets() > 1) portNr[i].setString(std::to_string(devNr));
            break;
    }
}

void
StatusBar::refreshState()
{
    TextureID id;
    
    switch (controller.warpActivation) {
            
        case Controller::WarpActivation::never:
            id = TextureID::syncLock;
            break;
            
        case Controller::WarpActivation::always:
            id = TextureID::warpLock;
            break;
            
        case Controller::WarpActivation::automatic:
            id = amiga.inWarpMode() ? TextureID::warp : TextureID::sync;
            break;

        default:
            assert(false);
    }
    
    state.rectangle.setTexture(&app.assets.get(id));
}
