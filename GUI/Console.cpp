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

    // Initialize the input buffer
    input.push_back("Retro Shell v0.1: ");
    
    // Initialize the cursor
    glyphWidth = font.getGlyph(0, fontSize, false).advance;
    cursor.setSize(sf::Vector2f(glyphWidth + 2, fontSize + 3));
    cursor.setFillColor(sf::Color(0xFF,0xFF,0xFF,0x80));

    // Initialize the render target
    if (!texture.create(width, height)) {
        throw std::runtime_error("Console: Can't allocate render texture");
    }
    drawable.setTextureRect(sf::IntRect(0, height, width, -height));
    drawable.setTexture(texture.getTexture());
            
    // Initialize render items
    for (int i = 0; i < numRows; i++) {
        
        row[i]->setFont(font);
        row[i]->setString("");
        row[i]->setCharacterSize(fontSize);
        row[i]->setFillColor(sf::Color::White);
        row[i]->setPosition(hposForCol(0), vposForRow(i));
    }
    
    updateTexture();
    return true;
}

void
Console::newline()
{
    // Move the cursor
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

            printf("RETURN %s\n", input[index].c_str());
            
            // Execute the command
            application.interpreter.execute(input[index]);

            // Move cursor
            newline();
            
            // Add a new entry to the input buffer
            input.push_back("");
            index = (int)input.size() - 1;
            break;
            
        case '\b':
            
            if (hpos > 0) {
                input[index].erase(input[index].begin() + --hpos);
                printf("length = %lu str = %s\n",
                       input[index].length(), input[index].c_str());
            }
            break;
            
        default:
            
            if (input[index].length() < numCols - promptWidth - 1) {
                
                input[index].insert(input[index].begin() + hpos++, c);
                printf("input: %s\n", input[index].c_str());
            }
    }
    
    updateTexture();
}

void
Console::keyPressed(sf::Keyboard::Key& key)
{
    printf("keyPressed\n");

    switch (key) {
            
        case sf::Keyboard::Up:

            printf("Cursor up %d\n", index);
            if (index > 0) {
                index--;
                hpos = (int)input[index].size();
            }
            break;

        case sf::Keyboard::Down:

            printf("Cursor down %d\n", index);
            if (index < input.size() - 1) {
                index++;
                hpos = (int)input[index].size();
            }
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
            if (hpos < input[index].length()) {
                hpos++;
            }
            printf("hpos = %d\n", hpos);
            break;
            
        case sf::Keyboard::Home:
            
            hpos = 0;
            break;

        case sf::Keyboard::End:
            
            hpos = (int)input[index].length();
            break;

        default:
            return;
    }

    updateTexture();
}

void
Console::keyReleased(sf::Keyboard::Key& key)
{
    /*
    printf("keyReleased\n");

    switch (key) {
            
        case sf::Keyboard::LControl:
            break;

        default:
            break;
    }
    */
}
            
void
Console::render(sf::RenderWindow &window)
{
    window.draw(drawable);
}

void
Console::updateTexture()
{
    printf("Update texture (%s)\n", input[index].c_str());
    
    texture.clear(sf::Color(0x21,0x50,0x9F,0xD0));
    
    row[vpos]->setString("vAmiga\% " + input[index]);
    for (int i = 0; i < numRows; i++) {
        texture.draw(*row[i]);
    }
    
    // Draw cursor
    int cursorX = hposForCol(hpos + promptWidth);
    int cursorY = vposForRow(vpos) + 3;
    cursor.setPosition(cursorX, cursorY);
    texture.draw(cursor);
}
