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

namespace view {
static const usize center = 0b0001;
static const usize flipx  = 0b0010;
static const usize flipy  = 0b0100;
}

struct View {

    usize flags = 0;
};


//
// Image view
//

struct ImageView : View, sf::RectangleShape {
        
    ImageView(usize _flags = 0);
    
    // Initializers
    void init(float x, float y, float w, float h, const sf::Texture &tex);
    void init(float x, float y, float w, const sf::Texture &tex);
    void init(float w, float h, const sf::Texture &tex);
    void init(float w, const sf::Texture &tex);
    void init(const sf::Vector2f &origin, const sf::Vector2f &size, const sf::Texture &tex);
    void init(const sf::Vector2f &size, const sf::Texture &tex);

    // Wrappers
    void setPosition(const sf::Vector2f &position);
    void setPosition(float x, float y);
    void draw(sf::RenderWindow &window);
};


//
// Gradient view
//

struct GradientView : View {
    
    float w, h;
    sf::Vertex rectangle[4];
    
    GradientView(usize flags = 0);

    void init(float x, float y, float w, float h,
              sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr);
    void init(float w, float h,
              sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr);

    // Wrappers
    void setPosition(const sf::Vector2f &position);
    void setPosition(float x, float y);
    void setColors(sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr);
    void draw(sf::RenderWindow &window);
};


//
// Text view
//

struct TextView : View, sf::Text {

    TextView(usize flags = 0);

    // Wrappers
    void setString(const string &str);
    void setStyle(const sf::Font &font, unsigned int size, const sf::Color &color);
    void draw(sf::RenderWindow &window);
};
