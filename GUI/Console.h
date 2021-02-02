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
#include <sstream>

class Console {
    
    // Reference to the associated application
    class Application &application;


    //
    // Constants
    //
    
    // Texture dimensions
    static const int width = 1060;
    static const int height = 785;
    
    // Text window size
    static const int numRows = 25;
    static const int numCols = 80;
    
    // Caligrahpic properties
    static const int fontSize = 22;
    static const int lineSkip = 9;
    
    // Input prompt
    static const std::string& prompt;

    
    //
    // Variables
    //
    
    // The text storage
    std::vector<std::string> storage;
    
    // The input history buffer
    std::vector<std::string> input;

    // The number of the first displayed line
    isize vpos = 0;
    
    // The current cursor position
    isize cpos = 0;
        
    // The currently active input string
    isize ipos = 0;

    // The render texture
    sf::RenderTexture texture;
    
    // A drawable holding the render texture
    sf::RectangleShape drawable;

    // The rendered text rows
    sf::Text text[numRows];

    // Indicates if the render texture needs to be redrawn
    bool isDirty = true;
    
    // Font properties
    int glyphWidth = 0;
        
    // The cursor's visual shape
    sf::RectangleShape cursor;
            
    // Alpha channel parameters
    int alpha = 0, targetAlpha = 0;
        
    
    //
    // Initializing
    //

public:

    Console(class Application &ref);
    ~Console();
    
    bool init();


    //
    // Managing the console window
    //
    
public:
    
    // Opens or closes the console
    void open() { targetAlpha = 0xFF; }
    void close() { targetAlpha = 0x00; }
    void toggle() { isVisible() ? close() : open(); }
    
    // Analyzing the window state
    bool isVisible() { return alpha > 0; }
    bool isAnimating() { return alpha != targetAlpha; }
    bool isOpening() { return targetAlpha > alpha; }
    bool isClosing() { return targetAlpha < alpha; }

    
    //
    // Working with the text storage
    //
    
public:
        
    // Clears the console window
    void clear();
    
    // Moves to a certain line in the console window
    void scrollTo(isize line);
    void scrollToTop() { scrollTo(0); }
    void scrollUp(isize delta) { scrollTo(vpos - delta); }
    void scrollDown(isize delta) { scrollTo(vpos + delta); }
    void makeLastLineVisible();
    
    // Prints a message
    Console& operator<<(char value);
    Console& operator<<(const std::string& value);
    Console& operator<<(int value);
    
    // Clears the current line
    void clearLine() { *this << '\r'; }

    // Moves the cursor forward to a certain column
    void tab(int hpos);

    // Replaces the last line
    void replace(const std::string& text,
                 const std::string& prefix = std::string(prompt));

    // Prints some debug output
    void list();

    // Returns the row number of the last displayed line
    isize rowOfLastLine();

    // Checks if the last line is visible
    bool lastLineIsVisible() { return rowOfLastLine() < numRows; }
    
    
    //
    // Processing input
    //
    
public:
    
    // Processes a user typed character
    void type(char c);

    // Processes special keys
    void keyPressed(const sf::Keyboard::Key& key);
    void keyReleased(const sf::Keyboard::Key& key);

    // Processes a mouse scrolling event
    void scroll(float delta);
    
    
    //
    // Rendering
    //
    
    void render(sf::RenderWindow &window);
    
private:

    // Translates rows and columns to drawing coordinates
    isize hposForCol(isize i) { return 10 + i * glyphWidth; }
    isize vposForRow(isize i) { return 5 + (fontSize + lineSkip) * i; }

    // Redraws the render texture
    void updateTexture();
};
