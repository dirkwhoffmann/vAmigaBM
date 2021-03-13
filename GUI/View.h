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

    //
    // Members
    //

public:
     
    // Indicates if the object is hidden
    bool isVisible = true;
    
protected:
 
    // Position and size
    float x = 0;
    float y = 0;
    float w = 0;
    float h = 0;

    // Alignment flags
    usize flags = 0;
    
    // Render position (final object position)
    float rx = 0;
    float ry = 0;
    
    
    //
    // Methods
    //
    
public:
    
    View(usize flags);
    
    // Initializers
    void init(float w, float h);

    // Getters
    float getX() { return x; }
    float getY() { return y; }
    float getW() { return w; }
    float getH() { return h; }

    // Adjusts the view position
    void setX(float x);
    void setY(float y);
    void setPosition(float x, float y);
    void move(float x, float y);
    
    // Adjusts the view position
    void setW(float w);
    void setH(float h);
    void setSize(float w, float h);
    
    // Adjusts the alignment flags
    void setFlags(usize value) { flags = value; }
    void addFlags(usize value) { flags |= value; }

    // Asjusts the transparency
    virtual void setAlpha(u8 value) = 0;
    
    // Computes the render position and updates the texture
    virtual void update();
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
    void init(const sf::Texture &tex);
        
    // Modifiers
    void setAlpha(u8 value) override;

    // Delegation methods
    void update() override;
    
    // Wrappers
    void draw(sf::RenderWindow &window, const sf::Shader *shader = nullptr);
    
    bool contains(const sf::Vector2i &pos);
};


//
// Solid view
//

class SolidView : public View {
    
public:
    
    sf::RectangleShape rectangle;
    
    SolidView() : View(Align::UpperLeft | Align::Proportional) { };
    SolidView(usize flags) : View(flags) { };

    // Initializers
    void init(float w, float h, const sf::Color &col);
        
    // Modifiers
    void setAlpha(u8 value) override;

    // Delegation methods
    void update() override;
    
    // Wrappers
    void draw(sf::RenderWindow &window, const sf::Shader *shader = nullptr);
    
    bool contains(const sf::Vector2i &pos);
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
    void init(float w, float h, sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr);

    // Modifiers
    void setColors(sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr);
    void setAlpha(u8 value) override;
    
    // Delegation methods
    void update() override;
    
    // Wrappers
    void draw(sf::RenderWindow &window, const sf::Shader *shader = nullptr);
};


//
// Text view
//

class TextView : public View {

public:
    
    sf::Text text;
    
    TextView() : View(Align::UpperLeft) { };
    TextView(usize flags) : View(flags) { };

    // Modifiers
    void setAlpha(u8 value) override;

    // Delegation methods
    void update() override;
    
    // Wrappers
    void setString(const string &str);
    void setFontSize(unsigned size);
    void setStyle(const sf::Font &font, unsigned int size, const sf::Color &color);
    void draw(sf::RenderWindow &window, const sf::Shader *shader = nullptr);
};


//
// Progress view
//

class ProgressView : public ImageView {

public:
       
    using ImageView::ImageView;

    // Initializers
    void init(class Assets &assets);
    
    // Modifiers
    // void setAlpha(u8 value);

    // Rotates the progress wheel
    void step();
    
    // Delegation methods
    void update() override;
};
