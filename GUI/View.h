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

struct ImageView : sf::RectangleShape {
        
    // Initializers
    void init(float x, float y, float w, float h, const sf::Texture &tex);
    void init(float x, float y, float w, const sf::Texture &tex);
    void init(float w, float h, const sf::Texture &tex);
    void init(float w, const sf::Texture &tex);
    void init(const sf::Vector2f &origin, const sf::Vector2f &size, const sf::Texture &tex);
    void init(const sf::Vector2f &size, const sf::Texture &tex);

    // Positions the view using normalized coordiates
    void move(const sf::RenderWindow &window, float x, float y);
    void center(const sf::RenderWindow &window, float x = 0.5, float y = 0.5);

    void draw(sf::RenderWindow &window);
};
