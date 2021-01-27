// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Console.h"
#include "AssetManager.h"

Console::Console()
{
    for (int i = 0; i < numRows; i++) {
        row[i] = new sf::Text();
    }
    
    if (!texture.create(width, height)) {
        throw std::runtime_error("Console: Can't allocate render texture");
    }
    drawable.setTextureRect(sf::IntRect(0, height, width, -height));
    drawable.setTexture(texture.getTexture());
            
    draw();
}

Console::~Console()
{
    for (int i = 0; i < numRows; i++) {
        delete row[i];
    }
}

bool
Console::init()
{
    sf::Font& font = Assets::get(FontID::console);

    // Initialize cursor
    glyphWidth = font.getGlyph(0, fontSize, false).advance;
    // cursorHeight = fontSize + lineSkip;
    
    cursor.setSize(sf::Vector2f(glyphWidth + 2, fontSize + 3));
    cursor.setFillColor(sf::Color(0xFF,0xFF,0xFF,0x80));

    // Initialize Text objects
    for (int i = 0; i < numRows; i++) {
        
        row[i]->setFont(font);
        row[i]->setString("");
        row[i]->setCharacterSize(fontSize);
        row[i]->setFillColor(sf::Color::White);
        row[i]->setPosition(hposForCol(0), vposForRow(i));
    }

    return true;
}

void
Console::newline()
{
    input = "";
    hpos = 0;
    if (vpos < numRows - 1) {
        vpos++;
    } else {
        scroll();
    }
}

void
Console::scroll()
{
    // Scroll one line up (rotate the pointer array)
    sf::Text *first = row[0];
    for (int i = 0; i < numRows - 1; i++) {
        row[i] = row[i + 1];
    }
    row[numRows - 1] = first;

    for (int i = 0; i < numRows; i++) {
        row[i]->setPosition(hposForCol(0), vposForRow(i));
    }

    row[numRows - 1]->setString("");
}

void
Console::add(char c)
{
    if (c == '\n') {

        printf("RETURN\n");
        newline();
        
    } else if (input.length() < numCols) {
        
        hpos++;
        input += c;
        printf("input: %s\n", input.c_str());
        row[vpos]->setString(input);
    }

    draw();
}

void
Console::render(sf::RenderWindow &window)
{
    window.draw(drawable);
}

void
Console::draw()
{
    texture.clear(sf::Color(0x21,0x50,0x9F,0xA0));
    
    for (int i = 0; i < numRows; i++) {
        texture.draw(*row[i]);
    }
    
    // Draw cursor
    int cursorX = hposForCol(hpos);
    int cursorY = vposForRow(vpos) + 3;
    cursor.setPosition(cursorX, cursorY);
    texture.draw(cursor);
}
