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
    
    /*
    ImageView(float x, float y, float w, float h, sf::Texture &tex);
    ImageView(sf::Vector2f &origin, sf::Vector2f &size, sf::Texture &tex);
    ImageView(sf::Vector2f &size, sf::Texture &tex);
    */
    
    void init(float x, float y, float w, float h, sf::Texture &tex);
    void init(float x, float y, float w, sf::Texture &tex);
    void init(const sf::Vector2f &origin, const sf::Vector2f &size, const sf::Texture &tex);
    void init(const sf::Vector2f &size, const sf::Texture &tex);

    void draw(sf::RenderWindow &window);
};
