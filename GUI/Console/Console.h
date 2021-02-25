// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Layer.h"

#include <stack>
#include <sstream>

class Console : public Layer {
    
    // Interpreter for commands typed into the debug console
    Interpreter interpreter = Interpreter(app);

    //
    // Constants
    //
  
    // The maximum number of rows
    // static const int maxRows = 40;
        
    // Layout properties
    int padx     = OS::scale(5);
    int pady     = OS::scale(2);
    int fontSize = OS::scale(14);
    int lineSkip = OS::scale(5);
    
    // Input prompt
    static const std::string& prompt;

    
    //
    // Dimensions
    //
    
    // Number of text rows and text columns
    isize numRows = 25;
    isize numCols = 80;

    
    //
    // Text storage
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
    
    // The image view holding the texture
    ImageView view = ImageView(Align::UpperLeft | Align::FlippedY);

    // The rendered text rows
    std::vector<sf::Text> text;

    // Indicates if the render texture needs to be redrawn
    bool isDirty = true;
    
    // Font properties
    int glyphWidth = 0;
        
    // The cursor's visual shape
    sf::RectangleShape cursor;
                    
    
    //
    // Initializing
    //

public:

    Console(class Application &ref);
    ~Console();
    
    // Delegation methods
    void init();
    void configure();


    //
    // Methods from Layer class
    //
    
    void handle(const sf::Event &event) override;
    void render() override;
    void resize(float width, float height) override;
    
    
    //
    // Opening and closing
    //
    
public:
    
    // Opens or closes the console
    void open() { setTargetAlpha(0xFF, 0.2); }
    void close() { setTargetAlpha(0x00, 0.2); }
    void toggle() { printf("toggle\n"); isVisible() ? close() : open(); }
    
 
    //
    // Adjusting the layout
    //
    
    void setNumRows(isize value);
    void setNumCols(isize value);

    
    //
    // Working with the text storage
    //
    
public:

    // Returns a reference to the last line in the text storage
    string &lastLine() { return storage.back(); }
            
    // Clears the console window
    void clear();
    
    // Prints a help line
    void printHelp();

    // Prints the input prompt
    void printPrompt();
        
    // Moves to a certain line in the console window
    void scrollTo(isize line);
    void scrollToTop() { scrollTo(0); }
    void scrollUp(isize delta) { scrollTo(vpos - delta); }
    void scrollDown(isize delta) { scrollTo(vpos + delta); }
    void makeLastLineVisible();
    
    // Prints a message
    Console &operator<<(char value);
    Console &operator<<(const string &value);
    Console &operator<<(int value);
    Console &operator<<(long value);

    // Shortens the text storage if it grows too large
    void shorten();
    
    // Clears the current line
    void clearLine() { *this << '\r'; }

    // Moves the cursor forward to a certain column
    void tab(int hpos);

    // Replaces the last line
    void replace(const string &text,
                 const string &prefix = string(prompt));

    // Prints some debug output
    void list();

    // Returns the row number of the last displayed line
    isize rowOfLastLine();

    // Checks if the last line is visible
    bool lastLineIsVisible();
    
    
    //
    // Processing input
    //
    
public:
    
    // Processes a user typed character
    void type(char c);

    // Processes special keys
    void keyPressed(const sf::Keyboard::Key &key);
    void keyReleased(const sf::Keyboard::Key &key);

    // Processes a mouse scrolling event
    void scroll(float delta);
    
    // Executes a user command
    bool exec(const string &command, bool verbose = false);
    
    // Executes a user script
    void exec(std::istream &stream) throws;

    
    //
    // Rendering
    //
        
private:

    // Translates rows and columns to drawing coordinates
    isize hposForCol(isize i) { return padx + i * glyphWidth; }
    isize vposForRow(isize i) { return pady + (fontSize + lineSkip) * i; }

    // Redraws the render texture
    void updateTexture();
};
