//
//  Console.cpp
//  vAmigaSFML
//
//  Created by Dirk Hoffmann on 26.01.21.
//

#include "Console.h"

Console::Console()
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
    if (!font.loadFromFile("DroidSansMono.ttf")) {
        printf("Error loading font\n");
        return false;
    }
    
    
    for (int i = 0; i < numRows; i++) {
        
        row[i]->setFont(font);
        row[i]->setString("");
        row[i]->setCharacterSize(fontSize);
        row[i]->setFillColor(sf::Color::Yellow);
        row[i]->setPosition(hposForRow(i), vposForRow(i));
        row[i]->setOutlineColor(sf::Color::Red);
        row[i]->setOutlineThickness(4.0);
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
        return;
    }
    
    if (input.length() < numCols) {
        input += c;
        printf("input: %s\n", input.c_str());
        row[vpos]->setString(input);
    }
}

void
Console::render(sf::RenderWindow &window)
{
    for (int i = 0; i < numRows; i++) {
        
        window.draw(*row[i]);
    }
}
