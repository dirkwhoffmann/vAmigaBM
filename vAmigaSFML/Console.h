//
//  Console.hpp
//  vAmigaSFML
//
//  Created by Dirk Hoffmann on 26.01.21.
//

#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Console {
    
    static const int numRows = 25;
    static const int numCols = 40;
    
    static const int fontSize = 24;
    static const int lineSkip = 8;
    
    // Font
    sf::Font font;
    
    // Text storage;
    sf::Text text[numRows];
    
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
    
    bool init();
    
    int hposForRow(int i) { return 10; }
    int vposForRow(int i) { return (fontSize + lineSkip) * i; }
        
    void newline(); 
    void scroll();
    void add(char c);
    
    void render(sf::RenderWindow &window);
};
