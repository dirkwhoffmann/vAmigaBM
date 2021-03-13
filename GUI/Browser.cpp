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
    path.setStyle(app.assets.get(FontID::console), 22, sf::Color::White);
    path.setSize(800, 54);
    path.setPads(24, -4);
    path.drawBackground = false;
    name.setStyle(app.assets.get(FontID::console), 44, sf::Color::White);
    name.setSize(800, 54);
    name.setPads(24, -4);
    name.drawBackground = false;
    line.setFillColor(sf::Color::White);
    line.setSize(sf::Vector2f { 800, 4 });
        
    for (int i = 0; i < 16; i++) {
        item[i].setStyle(app.assets.get(FontID::console), 44, sf::Color::White);
        item[i].setSize(800, 54);
        item[i].setPads(24, -4);
        item[i].drawBackground = false;
    }
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
            
            switch (event.key.code) {
                    
                case sf::Keyboard::Up:

                    if (selectedItem > 0) selectedItem--;
                    refresh();
                    break;

                case sf::Keyboard::Down:

                    if (selectedItem + 1 < filtered.size()) selectedItem++;
                    refresh();
                    break;
                    
                case sf::Keyboard::Backspace:
                    
                    if (input.size() > 0) input.pop_back();
                    refresh();
                    break;
                    
                default:
                    break;
            }
            break;
            
        case sf::Event::TextEntered:
            
            if (isprint(event.text.unicode)) {
                printf("Character: %c\n", static_cast<char>(event.text.unicode));
                if (input.size() < 32) {
                    input += static_cast<char>(event.text.unicode);
                    selectedItem = 0;
                    refresh();
                }
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
    
    background.setAlpha(std::min((isize)0x80, alpha));
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
    // Filter out files to display
    filtered.clear();
    for (auto &it : files) {
        
        bool matches = true;
        for (isize i = 0; i < input.size(); i++) {
            if (toupper(input[i]) != toupper(it[i])) { matches = false; break; }
        }
        if (!matches) continue;
        
        if (it.size() > 16) {
            auto prefix = util::extractName(it);
            auto suffix = util::extractSuffix(it);
            it = prefix.substr(0, 16 - suffix.size()) + "." + suffix;
        }
        filtered.push_back(it);
    }
    
    path.setString("/tmp/");
    name.setString(input + "_");
    
    isize highlightedItem = std::min((isize)16, selectedItem);

    for (int i = 0; i < 16; i++) {
                
        isize index = i + selectedItem - highlightedItem;
        
        item[i].setString(i < filtered.size() ? filtered[index] : "");
        if (i == selectedItem) {
            item[i].box.setFillColor(sf::Color(0x40,0x40,0x40));
            item[i].drawBackground = true;
        } else {
            item[i].box.setFillColor(sf::Color::Transparent);
            item[i].drawBackground = false;
        }
    }
}
