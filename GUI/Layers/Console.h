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
    
    //
    // Constants
    //
          
    // Layout properties
    int padx     = scale(5);
    int pady     = scale(2);
    int fontSize = scale(14);
    int lineSkip = scale(5);
    
    
    //
    // Dimensions
    //
    
    // Number of text rows and text columns
    isize numRows = 25;
    isize numCols = 80;

    
    //
    // Text storage
    //
    
    // The number of the first displayed line
    isize vpos = 0;
    
    // The render texture
    sf::RenderTexture texture;
    
    // The image view holding the texture
    ImageView view = ImageView(Align::UpperLeft | Align::FlippedY);

    // The rendered text rows
    std::vector<sf::Text> text;
    
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
    void awake();


    //
    // Performing continuous tasks
    //
    
public:
    
    void update(u64 frames, sf::Time dt) override;
    void render() override;
    
    
    //
    // Responding to events
    //
    
public:
    
    void respond(const sf::Event &event) override;
    void resize(float width, float height) override;
    void alphaDidChange() override;

    
    //
    // Adjusting the layout
    //
    
private:
    
    void setNumRows(isize value);
    void setNumCols(isize value);

    
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

    // Processes a mouse scrolling event
    void scroll(float delta);
    
    
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
