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
#include <stack>

class Console {
    
    // Reference to the associated application
    class Application &application;


    //
    // Constants
    //
    
    // Texture dimensions
    static const int width = 1024;
    static const int height = 768;
    
    // Text window size
    static const int numRows = 23;
    static const int numCols = 72;
    
    // Caligrahpic properties
    static const int fontSize = 24;
    static const int lineSkip = 9;
    
    // Input prompt
    static const std::string& prompt;

    
    //
    // Variables
    //
    
    // The text storage
    std::vector<std::string> storage;
    
    // The number of the first displayed line
    int first = 0;
    
    // The render texture
    sf::RenderTexture texture;
    
    // A drawable holding the render texture
    sf::RectangleShape drawable;

    // Indicates if the render texture needs to be redrawn
    bool isDirty = true;
    
    // The rendered text rows
    sf::Text row[numRows];

    // Font properties
    int glyphWidth;
    
    // The render texture
    
    // The current cursor position
    int hpos = 0;
    int vpos = 0;

    // The cursor's visual shape
    sf::RectangleShape cursor;
        
    // The input history buffer
    std::vector<std::string> input;
        
    // The currently active input string
    int index = 0;
    
    // Alpha channel parameters
    int alpha = 0;
    int targetAlpha = 0;
        
    
    //
    // Initializing
    //

public:

    Console(class Application &ref);
    ~Console();
    
    bool init();
    
    
    //
    // Properties
    //
    
    int hposForCol(int i) { return 10 + i * glyphWidth; }
    int vposForRow(int i) { return 5 + (fontSize + lineSkip) * i; }
        
    bool isVisible() { return alpha > 0; }
    bool isAnimating() { return alpha != targetAlpha; }
    bool isOpening() { return targetAlpha > alpha; }
    bool isClosing() { return targetAlpha < alpha; }

    
    //
    // Managing the console window
    //
    
public:
    
    // Opens or closes the console
    void open() { targetAlpha = 0xFF; }
    void close() { targetAlpha = 0x00; }
    void toggle() { isVisible() ? close() : open(); }
    
    
    //
    // Working with the text storage
    //
    
public:
    
    // Prints a message
    void print(const std::string& text);
    
    // Replaces the last line
    void replace(const std::string& text,
                 const std::string& prefix = std::string(prompt));

    // Prints the text storage (for debugging)
    void list();
    
    // Selects the displayed part of the text storage
    void scrollToLine(int line);
    void scrollToTop();
    void scrollToBottom();

    // Returns the row number of the last displayed line
    int rowOfLastLine();

    
    //
    // Processing input
    //
    
public:
    
    // Processes a user typed character
    void type(char c);

    // Processes special keys
    void keyPressed(sf::Keyboard::Key& key);
    void keyReleased(sf::Keyboard::Key& key);

    
    //
    // Rendering
    //
    
    void render(sf::RenderWindow &window);
    
    
private:
    
    // Redraws the render texture
    void updateTexture();
};
