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
    
    if (!renderTexture.create(width, height)) {
        throw std::runtime_error("Console: Can't allocate render texture");
    }
    sprite.setTextureRect(sf::IntRect(0, height, width, -height));
    sprite.setTexture(renderTexture.getTexture());
            
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
    if (!font.loadFromFile("DroidSansMono.ttf")) {
        printf("Error loading font\n");
        return false;
    }
    advance = font.getGlyph(0, fontSize, false).advance;
    auto bounds = font.getGlyph(0,fontSize,false).bounds;
    printf("%f %f %f %f\n", bounds.left, bounds.top, bounds.width, bounds.height);
    printf("advance = %f\n", advance);
    
    for (int i = 0; i < numRows; i++) {
        
        row[i]->setFont(font);
        row[i]->setString("");
        row[i]->setCharacterSize(fontSize);
        row[i]->setFillColor(sf::Color::White);
        row[i]->setPosition(hposForRow(i), vposForRow(i));
        // row[i]->setOutlineColor(sf::Color::Red);
        // row[i]->setOutlineThickness(4.0);
    }

    cursor.setSize(sf::Vector2f(advance + 2, fontSize + 3));
    cursor.setFillColor(sf::Color(0xFF,0xFF,0xFF,0x80));

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
        row[i]->setPosition(hposForRow(i), vposForRow(i));
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
    window.draw(sprite);
}

void
Console::draw()
{
    renderTexture.clear(sf::Color(0x21,0x50,0x9F,0xA0));
    
    for (int i = 0; i < numRows; i++) {
        renderTexture.draw(*row[i]);
    }
    
    // Draw cursor
    int cursorX = hposForRow(vpos) + hpos * advance;
    int cursorY = vposForRow(vpos) + 3;
    cursor.setPosition(cursorX, cursorY);
    renderTexture.draw(cursor);
}
