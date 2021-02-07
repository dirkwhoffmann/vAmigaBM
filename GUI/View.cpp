// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

//
// ImageView
//

void
ImageView::init(const sf::Vector2f &origin, const sf::Vector2f &size, const sf::Texture &tex)
{
    setPosition(origin);
    setSize(size);
    setTexture(&tex);
}

void
ImageView::init(const sf::Vector2f &size, const sf::Texture &tex)
{
    init(sf::Vector2f{0,0}, size, tex);
}

void
ImageView::init(float x, float y, float w, float h, const sf::Texture &tex)
{
    init(sf::Vector2f{x,y}, sf::Vector2f{w,h}, tex);
}

void
ImageView::init(float x, float y, float w, const sf::Texture &tex)
{
    init(x, y, w, w * (float)tex.getSize().y / (float)tex.getSize().x, tex);
}

void
ImageView::init(float w, float h, const sf::Texture &tex)
{
    init(0, 0, w, h, tex);
}

void
ImageView::init(float w, const sf::Texture &tex)
{
    init(0, 0, w, w * (float)tex.getSize().y / (float)tex.getSize().x, tex);
}

void
ImageView::move(const sf::RenderWindow &window, float x, float y)
{
    auto size = window.getView().getSize();
    
    sf::Vector2f origin = sf::Vector2f{x * size.x, y * size.y };
    setPosition(origin);
    
    auto pos = getPosition();
    printf("x %f y %f\n", x, y);
    printf("%f %f\n", pos.x, pos.y);
}

void
ImageView::center(const sf::RenderWindow &window, float x, float y)
{
    auto size = window.getView().getSize();
    auto dx = getSize().x / 2;
    auto dy = getSize().y / 2;
    
    sf::Vector2f origin = sf::Vector2f{x * size.x - dx, y * size.y - dy};
    setPosition(origin);
}

void
ImageView::draw(sf::RenderWindow &window)
{
    window.draw(*this);
}
