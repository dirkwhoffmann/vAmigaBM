// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

Console::Console(Application &ref) : application(ref)
{
    for (int i = 0; i < numRows; i++) {
        row[i] = new sf::Text();
    }
    
    if (!texture.create(width, height)) {
        throw std::runtime_error("Console: Can't allocate render texture");
    }
    drawable.setTextureRect(sf::IntRect(0, height, width, -height));
    drawable.setTexture(texture.getTexture());
            
    updateTexture();
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
    application.interpreter.execute(input);

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
Console::type(char c)
{
    printf("char: %d\n", c);
    
    switch (c) {
            
        case '\n':

            printf("RETURN\n");
            newline();
            break;
            
        case '\b':
            
            printf("BACKSPACE\n");
            if (hpos > 0) {
                input.erase(input.begin() + --hpos);
                printf("length = %lu str = %s\n", input.length(), input.c_str());
                row[vpos]->setString(input);
            }
            break;
            
        default:
            
            if (input.length() < numCols) {
                
                input.insert(input.begin() + hpos, c);
                hpos++;
                printf("input: %s\n", input.c_str());
                row[vpos]->setString(input);
            }
    }
    
    updateTexture();
}

void
Console::keyPressed(sf::Keyboard::Key& key)
{
    switch (key) {
            
        case sf::Keyboard::Up:

            // TODO: Iterate through history buffer
            printf("Cursor up\n");
            break;

        case sf::Keyboard::Down:

            // TODO: Iterate through history buffer
            printf("Cursor down\n");
            break;
            
        case sf::Keyboard::Left:

            printf("Cursor left\n");
            if (hpos > 0) {
                hpos--;
            }
            printf("hpos = %d\n", hpos);
            break;
            
        case sf::Keyboard::Right:
            
            printf("Cursor right\n");
            if (hpos < input.length()) {
                hpos++;
            }
            printf("hpos = %d\n", hpos);
            break;
            
        default:
            return;
    }

    updateTexture();
}

void
Console::render(sf::RenderWindow &window)
{
    window.draw(drawable);
}

void
Console::updateTexture()
{
    printf("updateTexture\n");
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
