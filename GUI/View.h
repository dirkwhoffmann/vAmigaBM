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

struct Align {

    static const usize Centered     = 0b0000000;
    static const usize Left         = 0b0000001;
    static const usize Right        = 0b0000010;
    static const usize Top          = 0b0000100;
    static const usize Bottom       = 0b0001000;
    
    static const usize UpperLeft    = 0b0000101;
    static const usize LowerLeft    = 0b0001001;
    static const usize UpperRight   = 0b0000110;
    static const usize LowerRight   = 0b0001010;

    static const usize Proportional = 0b0010000;
    static const usize FlippedX     = 0b0100000;
    static const usize FlippedY     = 0b1000000;
};

class View {
 
public:
 
    //
    // Members
    //
    
// protected:
public:
    
    // Position and size
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;

    // Alignment flags
    usize flags = 0;
    
    //
    // Methods
    //
    
public:
    
    View(usize flags);
    
    // Initializers
    void init(float x, float y, float w, float h);
    virtual void update() = 0;

    // Getters
    float x1();
    float x2();
    float y1();
    float y2();
    
    // Modifiers
    void setX(float x) { this->x = x; update(); }
    void setY(float y) { this->y = y; update(); }
    void setW(float w);
    void setH(float h);
    
    void setPosition(float x, float y) { setX(x); setY(y); update(); }
    void setSize(float w, float h) { setW(w); setH(h); update(); }
};


//
// Image view
//

class ImageView : public View {
    
public:
    
    sf::RectangleShape rectangle;
    
    ImageView() : View(Align::UpperLeft | Align::Proportional) { };
    ImageView(usize flags) : View(flags) { };

    // Initializers
    void init(float x, float y, const sf::Texture &tex);
    void init(const sf::Texture &tex);
        
    // Delegation methods
    void update();
    
    // Wrappers
    void draw(sf::RenderWindow &window);
};


//
// Gradient view
//

class GradientView : public View {
    
public:
    
    sf::Vertex rectangle[4];
    
    GradientView() : View(Align::UpperLeft) { };
    GradientView(usize flags) : View(flags) { };

    // Initializers
    void init(float x, float y, float w, float h,
              sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr);
    void init(float w, float h,
              sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr);

    // Setters
    void setColors(sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr);

    // Delegation methods
    void update();
    
    // Wrappers
    void draw(sf::RenderWindow &window);
};


//
// Text view
//

class TextView : public View {

public:
    
    sf::Text text;
    
    TextView() : View(Align::UpperLeft) { };
    TextView(usize flags) : View(flags) { };

    // Delegation methods
    void update();
    
    // Wrappers
    void setString(const string &str);
    void setFontSize(unsigned size);
    void setStyle(const sf::Font &font, unsigned int size, const sf::Color &color);
    void draw(sf::RenderWindow &window);
};
