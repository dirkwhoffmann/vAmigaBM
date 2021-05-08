// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Browser.h"
#include "Application.h"
#include "IO.h"

const isize Browser::numRows;
isize Browser::w;
isize Browser::h;
isize Browser::pad;
isize Browser::icn;

Browser::Browser(Application &ref) : Layer(ref)
{
    w = scale(508);
    h = scale(416);
    pad = scale(16);
    icn = scale(64);
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
    icon.setW(icn);
    icon.setPosition(pad, pad);
    path.setStyle(app.assets.get(FontID::console), scale(18), sf::Color::White);
    name.setStyle(app.assets.get(FontID::console), scale(22), sf::Color::White);
    selector.init(w - icn - 2.5 * pad, scale(27), sf::Color(0x80,0x80,0x80));
    line.setFillColor(sf::Color::White);
    line.setSize(sf::Vector2f { (float)w - 2 * pad, (float)scale(2) });
        
    for (int i = 0; i < numRows; i++) {
        
        item[i].setStyle(app.assets.get(FontID::console), scale(22), sf::Color::White);
    }
}

void
Browser::awake()
{
 
}

void
Browser::open(isize dfn)
{
    assert(dfn >= 0 && dfn <= 3);
 
    // Assign action function
    action = [this, dfn](const string &s) {
        amiga.paula.diskController.insertDisk(amiga.paula.diskController.getSearchPath(dfn) + "/" + s, dfn);
    };
    
    // Get the media directory for this drive
    auto searchPath = amiga.paula.diskController.getSearchPath(dfn);
    path.setString(searchPath);

    // Scan directory
    std::vector <string> allowedTypes { "adf", "dms", "exe", "img" };
    files = util::files(searchPath, allowedTypes);
    std::sort(files.begin(), files.end());
    
    input = "";
    cursor = "_";
    refresh();
    
    Layer::open(0.5);
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
                    
                    action(filtered[highlightedRow()]);
                    input = filtered[highlightedRow()];
                    cursor = "";
                    refresh();
                    close(1.0);
                    break;

                case sf::Keyboard::Escape:
                    
                    close(0.2);
                    break;
                    
                default:
                    break;
            }
            break;
            
        case sf::Event::TextEntered:
            
            if (isprint(event.text.unicode)) {
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
    printf("w = %zd h = %zd dx = %zd dy = %zd\n", w, h, dx, dy);
    background.setPosition(dx, dy);
    icon.setPosition(dx + pad, dy + pad);
    path.setPosition(dx + pad + icn + pad, dy + pad);
    name.setPosition(dx + pad + icn + pad, dy + pad + scale(35));
    line.setPosition(dx + pad, dy + pad + icn + pad);
    
    for (isize i = 0; i < numRows; i++) {
        item[i].setPosition(dx + icn + 2 * pad,
                            dy + icn + 2 * pad + scale(24) * i);
    }
}

void
Browser::alphaDidChange()
{    
    background.setAlpha(std::min((isize)0xD0, alpha));
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
            it = prefix.substr(0, 28 - suffix.size()) + "." + suffix;
        }
        filtered.push_back(it);
    }
    
    name.setString(input + cursor);
    
    for (int i = 0; i < numRows; i++) {
                        
        item[i].setString(i < filtered.size() ? filtered[indexForRow(i)] : "");
    }

    selector.setPosition(dx + icn + 1.5 * pad, 
                         dy + icn + 2 * pad + scale(1) + scale(24) * highlightedRow());
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
