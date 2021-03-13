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

const isize Browser::numRows;
const isize Browser::w;
const isize Browser::h;
const isize Browser::pad;
const isize Browser::icn;

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
    background.setSize(w, h);
    
    icon.init(app.assets.get(TextureID::diskLarge));
    icon.setPosition(pad, pad);
    path.setStyle(app.assets.get(FontID::console), 36, sf::Color::White);
    name.setStyle(app.assets.get(FontID::console), 44, sf::Color::White);
    selector.init(w - icn - 2.5 * pad, 54, sf::Color(0x80,0x80,0x80));
    line.setFillColor(sf::Color::White);
    line.setSize(sf::Vector2f { w - 2 * pad, 4 });
        
    for (int i = 0; i < numRows; i++) {
        
        item[i].setStyle(app.assets.get(FontID::console), 44, sf::Color::White);
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
    delay = 0.5;
    
    files = util::files("/tmp/", "adf");
    
    // Scan directory
    printf("ADF files in /tmp/\n");
    for (auto &it : files) {
        printf("%s\n", it.c_str());
    }
    
    refresh();
}

void
Browser::close()
{
    Layer::close();
    delay = 1.0;
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
    selector.draw(app.window);
    icon.draw(app.window);
    path.draw(app.window);
    name.draw(app.window);
    app.window.draw(line);
    
    for (int i = 0; i < numRows; i++) {
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

                case sf::Keyboard::Return:
                    
                    printf("highlightedRow: %zd\n", highlightedRow());
                    printf("%s\n", filtered[highlightedRow()].c_str());
                    input = filtered[highlightedRow()];
                    refresh();
                    close();
                    break;

                default:
                    break;
            }
            break;
            
        case sf::Event::TextEntered:
            
            if (isprint(event.text.unicode)) {
                printf("Character: %c\n", static_cast<char>(event.text.unicode));
                if (input.size() < 30) {
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
    dx = (width - w) / 2;
    dy = (height - h) / 2;
    
    background.setPosition(dx, dy);
    icon.setPosition(dx + pad, dy + pad);
    path.setPosition(dx + pad + icn + pad, dy + pad);
    name.setPosition(dx + pad + icn + pad, dy + pad + 70);
    line.setPosition(dx + pad, dy + pad + icn + pad);
    
    for (isize i = 0; i < numRows; i++) {
        item[i].setPosition(dx + icn + 3 * pad, dy + icn + 3 * pad + 48 * i);
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
    selector.setAlpha(alpha);
    // line.setAlpha(alpha);
    for (isize i = 0; i < numRows; i++) {
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
        
        if (it.size() > 30) {
            auto prefix = util::extractName(it);
            auto suffix = util::extractSuffix(it);
            it = prefix.substr(0, 16 - suffix.size()) + "." + suffix;
        }
        filtered.push_back(it);
    }
    
    path.setString("/tmp/");
    name.setString(input + "_");
    
    for (int i = 0; i < numRows; i++) {
                        
        item[i].setString(i < filtered.size() ? filtered[indexForRow(i)] : "");
    }

    selector.setPosition(dx + pad + icn + pad / 2,
                         dy + 3 * pad + icn + 2 + 48 * highlightedRow());
}

isize
Browser::highlightedRow() const
{
    return std::min(numRows - 1, selectedItem);
}

isize
Browser::indexForRow(isize row) const
{
    return row + selectedItem - highlightedRow();
}
