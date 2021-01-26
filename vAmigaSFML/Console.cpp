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
        row[i] = &text[i];
    }
}

bool
Console::init()
{
//    if (!font.loadFromFile("/tmp/Inkspot.ttf")) {
    if (!font.loadFromFile("/tmp/DroidSansMono.ttf")) {
        printf("Error loading font\n");
        return false;
    }
    
    for (int i = 0; i < numRows; i++) {
        
        text[i].setFont(font);
        text[i].setString("");
        text[i].setCharacterSize(fontSize);
        text[i].setFillColor(sf::Color::Yellow);
        text[i].setPosition(hposForRow(i), vposForRow(i));
        text[i].setOutlineColor(sf::Color::Red);
        text[i].setOutlineThickness(4.0);
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
