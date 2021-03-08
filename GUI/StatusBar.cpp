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

    const float h = 40.0;
    
    bar.setColors(sf::Color(0xD0,0xD0,0xD0), sf::Color(0xD0,0xD0,0xD0),
                  sf::Color(0xB0,0xB0,0xB0), sf::Color(0xB0,0xB0,0xB0));
    
    for (int i = 0; i < 4; i++) {
        
        driveLed[i].init(app.assets.get(TextureID::ledGreen));
        driveLed[i].setH(h);
        cylinder[i].setStyle(app.assets.get(FontID::sans_r), 32, sf::Color::Red);
        cylinder[i].setString("00");
        cylinder[i].setH(h);
        disk[i].init(app.assets.get(TextureID::disk));
        disk[i].setH(h);
        spin[i].init(app.assets);
        spin[i].setH(h);
        spin[i].rectangle.setOrigin(20, 20);
    }
    powerLed.init(app.assets.get(TextureID::ledRed));
    powerLed.setH(h);
    port[0].init(app.assets.get(TextureID::spin));
    port[0].setH(h);
    port[1].init(app.assets.get(TextureID::spin));
    port[1].setH(h);
    mute.init(app.assets.get(TextureID::spin));
    mute.setH(h);
    mhz.setStyle(app.assets.get(FontID::sans_r), 32, sf::Color::Red);
    mhz.setString("00");
    mhz.setH(h);
    state.init(app.assets.get(TextureID::spin));
    state.setH(h);    
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
    
    if (frames % 5 == 0) {
        for (isize i = 0; i < 4; i++) spin[i].step(app.assets);
    }
}

void
StatusBar::render()
{
    bar.draw(app.window);
    
    for (int i = 0; i < 4; i++) {
        
        spin[i].rectangle.rotate(3.0);
        driveLed[i].draw(app.window);
        cylinder[i].draw(app.window);
        disk[i].draw(app.window);
        spin[i].draw(app.window);
    }
    powerLed.draw(app.window);
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
    
    bar.setSize(size.x, 50);
    bar.setPosition(0, size.y - 50);
    
    float pos = 5;
    float delta = 50;
    float y = size.y - delta;
    
    //
    // Left aligned items
    //
        
    powerLed.setPosition(pos, y);  pos += delta;
    
    for (int i = 0; i < 4; i++) {
        
        driveLed[i].setPosition(pos, y); pos += delta;
        cylinder[i].setPosition(pos, y); pos += delta;
        disk[i].setPosition(pos, y); pos += delta;
        spin[i].setPosition(pos, y); pos += delta;
    }
    
    //
    // Right aligned items
    //
    
    pos = app.window.getSize().x - 5 - delta;
    
    port[0].setPosition(pos, y); pos -= delta;
    port[1].setPosition(pos, y); pos -= delta;
    mute.setPosition(pos, y); pos -= delta;
    mhz.setPosition(pos, y); pos -= delta;
    state.setPosition(pos, y); pos -= delta;
}
