// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Browser.h"
#include "Application.h"
#include "IO.h"

Browser::Browser(Application &ref) : Layer(ref)
{
    
}

Browser::~Browser()
{
    
}

void Browser::init()
{
    Layer::init();

    auto grey1 = sf::Color(0x30,0x30,0x30,0x80);
    auto grey2 = sf::Color(0x50,0x50,0x50,0x80);
    background.setColors(grey1, grey1, grey2, grey2);
    
    icon.init(app.assets.get(TextureID::disk));
    path.setStyle(app.assets.get(FontID::sans_r), 22, sf::Color::White);
    name.setStyle(app.assets.get(FontID::sans_r), 44, sf::Color::White);
    line.setFillColor(sf::Color::White);
    line.setSize(sf::Vector2f { 800, 4 });
        
    for (int i = 0; i < 16; i++) {
        item[i].setStyle(app.assets.get(FontID::sans_r), 44, sf::Color::White);
    }

    selectedItem = 8;
    input = "Foo";
}

void
Browser::awake()
{
 
}

void
Browser::open()
{
    Layer::open();
    
    files = util::files("/tmp/", "adf");
    
    // Scan directory
    printf("ADF files in /tmp/\n");
    for (auto &it : files) {
        printf("%s\n", it.c_str());
    }
    
    refresh();
}

void
Browser::update(u64 frames, sf::Time dt)
{
    Layer::update(frames, dt);
}

void
Browser::render()
{
    background.draw(app.window);
    icon.draw(app.window);
    path.draw(app.window);
    name.draw(app.window);
    app.window.draw(line);
    
    for (int i = 0; i < 16; i++) {
        item[i].draw(app.window);
    }
}

void
Browser::respond(const sf::Event &event)
{
    switch (event.type) {
            
        case sf::Event::KeyPressed:
            
            printf("keycode: %d\n", event.key.code);
            break;
            
        case sf::Event::TextEntered:
            
            if (event.text.unicode < 128) {
                printf("Character: %c\n", static_cast<char>(event.text.unicode));
            }
            break;
            
        default:
            break;
    }
}

void
Browser::resize(float width, float height)
{
    // auto size = app.window.getSize();
    
    background.setSize(800, 800);
    icon.setPosition(0, 0);
    path.setPosition(100, 0);
    name.setPosition(100, 20);
    line.setPosition(0, 80);
    
    for (isize i = 0; i < 16; i++) {
        item[i].setPosition(100, 100 + 48 * i);
    }
}

void
Browser::alphaDidChange()
{
    printf("Browser alpha: %zd\n", alpha);
    
    background.setAlpha(alpha);
    icon.setAlpha(alpha);
    name.setAlpha(alpha);
    path.setAlpha(alpha);
    // line.setAlpha(alpha);
    for (isize i = 0; i < 16; i++) {
        item[i].setAlpha(alpha);
    }
}

void
Browser::refresh()
{
    input = "Foo";
    
    path.setString("/tmp/");
    name.setString(input);
    
    for (int i = 0; i < 16; i++) {
        if (i < files.size()) {
            item[i].setString(files[i]);
        } else {
            item[i].setString("row " + std::to_string(i));
        }
    }
}
