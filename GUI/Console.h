// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Console {
    
    static const int width = 1024;
    static const int height = 768;
    
    static const int numRows = 25;
    static const int numCols = 40;
    
    static const int fontSize = 25;
    static const int lineSkip = 8;
    
    // The render texture
    sf::RenderTexture renderTexture;
        
    // A sprite holding the render texture
    sf::Sprite sprite;

    // Console font
    sf::Font font;
    float advance;
    
    // Cursor
    sf::RectangleShape cursor; //  (sf::Vector2f(25,25));
    
    // Mapping from row indices to text objects
    sf::Text *row[numRows];
    
    // Cursor position
    int hpos = 0;
    int vpos = 0;
    
    // The currenr input string
    std::string input = "";
    
    
    //
    //
    //
    
public:
    
    Console();
    ~Console();
    
    bool init();
    
    int hposForRow(int i) { return 10; }
    int vposForRow(int i) { return (fontSize + lineSkip) * i; }
        
    void newline(); 
    void scroll();
    void add(char c);
    
    void render(sf::RenderWindow &window);
    
private:
    
    // Redraws the render texture
    void draw();
};
