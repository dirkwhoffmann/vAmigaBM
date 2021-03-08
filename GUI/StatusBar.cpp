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

    powerLed.init(app.assets.get(TextureID::ledRed));
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
