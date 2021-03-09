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
    port[0].init(app.assets.get(TextureID::spin));
    port[0].setW(2 * 18);
    port[0].rectangle.setFillColor(grey6);
    port[1].init(app.assets.get(TextureID::spin));
    port[1].setW(2 * 18);
    port[1].rectangle.setFillColor(grey6);
    mute.init(app.assets.get(TextureID::mute));
    mute.setW(2 * 18);
    mute.rectangle.setFillColor(grey6);
    mhz.setStyle(app.assets.get(FontID::sans_r), 24, grey6);
    mhz.setString("999.99 MHz");
    mhz.setW(2 * 18);
    state.init(app.assets.get(TextureID::sync));
    state.setW(2 * 18);
    state.rectangle.setFillColor(grey6);
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
    
    for (int i = 0; i < 4; i++) {
        
        driveLed[i].draw(app.window);
        cylinder[i].draw(app.window);
        disk[i].draw(app.window);
        spin[i].draw(app.window);
    }
    powerLed.draw(app.window);
    // port[0].draw(app.window);
    // port[1].draw(app.window);
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
    float pos = pad;
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
    
    pos = app.window.getSize().x - state.w - pad;
    
    // port[0].setPosition(pos, y + 8); pos -= port[0].w + pad;
    // port[1].setPosition(pos, y + 8); pos -= port[1].w + pad;
    state.setPosition(pos, y + 8); pos -= 2 * 64 + pad;
    mhz.setPosition(pos, y + 10); pos -= mute.w + pad;
    mute.setPosition(pos, y + 8);
}

void
StatusBar::refresh()
{
    char tmp[16];
    
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

    if (needsUpdate & StatusBarItem::MHZ) {
        sprintf(tmp, "%3.2f Mhz", 99.9);
        mhz.setString(string(tmp));
    }

    if (needsUpdate & StatusBarItem::STATE) {
        state.rectangle.setTexture(&app.assets.get(TextureID::sync));
    }
    
    needsUpdate = 0;
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

/*
let config = amiga.diskController.getConfig()
let connected0 = config.connected.0
let connected1 = config.connected.1
let connected2 = config.connected.2
let connected3 = config.connected.3

let motor0 = amiga.df0.motor
let motor1 = amiga.df1.motor
let motor2 = amiga.df2.motor
let motor3 = amiga.df3.motor
let hasDisk0 = amiga.df0.hasDisk
let hasDisk1 = amiga.df1.hasDisk
let hasDisk2 = amiga.df2.hasDisk
let hasDisk3 = amiga.df3.hasDisk

let running = amiga.running
let debug = amiga.debugMode
let halted = amiga.cpu.halted
let warp = amiga.warp

// Cylinders
refreshStatusBar(drive: 0, cylinder: amiga.df0.cylinder)
refreshStatusBar(drive: 1, cylinder: amiga.df1.cylinder)
refreshStatusBar(drive: 2, cylinder: amiga.df2.cylinder)
refreshStatusBar(drive: 3, cylinder: amiga.df3.cylinder)
refreshStatusBar(writing: nil)

// Animation
refreshStatusBar(drive: 0, motor: motor0)
refreshStatusBar(drive: 1, motor: motor1)
refreshStatusBar(drive: 2, motor: motor2)
refreshStatusBar(drive: 3, motor: motor3)

// Drive icons
df0Disk.image = amiga.df0.icon
df1Disk.image = amiga.df1.icon
df2Disk.image = amiga.df2.icon
df3Disk.image = amiga.df3.icon

// Warp mode icon
warpIcon.image = hourglassIcon

// Visibility
let items: [NSView: Bool] = [
    
    powerLED: true,
    
    df0LED: connected0,
    df1LED: connected1,
    df2LED: connected2,
    df3LED: connected3,
    df0Disk: connected0 && hasDisk0,
    df1Disk: connected1 && hasDisk1,
    df2Disk: connected2 && hasDisk2,
    df3Disk: connected3 && hasDisk3,
    df0Cylinder: connected0,
    df1Cylinder: connected1,
    df2Cylinder: connected2,
    df3Cylinder: connected3,
    df0DMA: motor0,
    df1DMA: motor1,
    df2DMA: motor2,
    df3DMA: motor3,

    haltIcon: halted,
    cmdLock: myAppDelegate.mapCommandKeys,
    debugIcon: debug,
    muteIcon: warp || muted,

    clockSpeed: running,
    clockSpeedBar: running,
    warpIcon: running
]

for (item, visible) in items {
    item.isHidden = !visible || !statusBar
}
}

*/
