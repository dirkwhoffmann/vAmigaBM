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
    powerLed.setW(2 * 24);

    for (int i = 0; i < 4; i++) {
        
        driveLed[i].init(app.assets.get(TextureID::ledGreen));
        driveLed[i].setW(2 * 24);
        cylinder[i].setStyle(app.assets.get(FontID::sans_r), 28, grey6);
        cylinder[i].setString("00");
        cylinder[i].setW(2 * 16);
        disk[i].init(app.assets.get(TextureID::disk));
        disk[i].setW(2 * 18);
        disk[i].rectangle.setFillColor(grey6);
        spin[i].init(app.assets);
        spin[i].setW(2 * 18);
        spin[i].rectangle.setFillColor(grey6);
    }
    
    for (int i = 0; i < 2; i++) {
        
        port[i].init(app.assets.get(TextureID::none));
        port[i].setW(2 * 18);
        port[i].rectangle.setFillColor(grey6);
        port[i].flags = Align::Top | Align::Right;
    }
    
    mute.flags = Align::Top | Align::Right;
    mute.init(app.assets.get(TextureID::mute));
    mute.setW(2 * 18);
    mute.rectangle.setFillColor(grey6);
    mhz.setStyle(app.assets.get(FontID::sans_sb), 24, grey6);
    mhz.setW(2 * 18);
    mhz.flags = Align::Top | Align::Right;
    state.init(app.assets.get(TextureID::sync));
    state.setW(2 * 18);
    state.rectangle.setFillColor(grey6);
    state.flags = Align::Top | Align::Right;
}

void
StatusBar::awake()
{
    // auto size = app.window.getSize();
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
    port[1].draw(app.window);
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
    
    bar.setSize(size.x, 26 * 2);
    bar.setPosition(0, size.y - 26 * 2);

    float pad = 12;
    float pos = 18;
    float y = size.y - 26 * 2;
    
    //
    // Left aligned items
    //
        
    powerLed.setPosition(pos, y + 14);  pos += powerLed.w + pad;
    
    for (int i = 0; i < 4; i++) {
        
        driveLed[i].setPosition(pos, y + 14); pos += driveLed[i].w + pad;
        cylinder[i].setPosition(pos, y + 6); pos += cylinder[i].w + pad;
        disk[i].setPosition(pos, y + 8); pos += disk[i].w + pad;
        spin[i].setPosition(pos, y + 8); pos += spin[i].w + pad;
    }
    
    //
    // Right aligned items
    //
    
    pos = app.window.getSize().x - 18;

    port[1].setPosition(pos, y + 8); pos -= port[1].w + pad;
    port[0].setPosition(pos, y + 8); pos -= port[0].w + pad;
    state.setPosition(pos, y + 8); pos -= state.w + pad;
    mhz.setPosition(pos, y + 10); pos -= 2 * 64 + pad;
    mute.setPosition(pos, y + 8);
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
        state.rectangle.setTexture(&app.assets.get(TextureID::sync));
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
    disk[nr].isVisible = connected && amiga.df[nr]->hasDisk();
    spin[nr].isVisible = connected && amiga.df[nr]->getMotor();

    // Update the head position
    if (needsUpdate & (StatusBarItem::DRIVE_CYL << nr)) {

        sprintf(tmp, "%02d", amiga.df[nr]->getCylinder());
        cylinder[nr].setString(string(tmp));
    }
}

void
StatusBar::refreshPort(PortNr nr)
{
    isize i = (nr == PORT_1) ? 0 : 1;

    if (inputManager.isMouse(nr)) {
        port[i].rectangle.setTexture(&app.assets.get(TextureID::mouse));
    } else if (inputManager.isJoystick(nr)) {
        port[i].rectangle.setTexture(&app.assets.get(TextureID::joystick));
    } else if (inputManager.iskeyset(nr)) {
        port[i].rectangle.setTexture(&app.assets.get(TextureID::keyset));
    } else {
        port[i].rectangle.setTexture(&app.assets.get(TextureID::none));
    }
}
