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
    
    //
    // Constants
    //
    
    // Texture dimensions
    static const int width = 1024;
    static const int height = 768;
    
    // Text window size
    static const int numRows = 25;
    static const int numCols = 40;
    
    // Caligrahpic properties
    static const int fontSize = 25;
    static const int lineSkip = 8;
    
    
    //
    // Variables
    //
    
    // The final render texture
    sf::RenderTexture texture;
    
    // Rendered text, one object for each row
    sf::Text *row[numRows];

    // Font properties
    int glyphWidth;
    
    // A sprite holding the render texture
    sf::Sprite drawable;
    
    // The current cursor position
    int hpos = 0;
    int vpos = 0;

    // The cursor's visual shape
    sf::RectangleShape cursor;
        
    // The currenr input string
    std::string input = "";
        
    
    //
    // Initializing
    //

public:

    Console();
    ~Console();
    
    bool init();
    
    
    //
    //
    //
    
    int hposForCol(int i) { return 10 + i * glyphWidth; }
    int vposForRow(int i) { return (fontSize + lineSkip) * i; }
        
    void newline(); 
    void scroll();
    void add(char c);
    
    void render(sf::RenderWindow &window);
    
private:
    
    // Redraws the render texture
    void draw();
};
