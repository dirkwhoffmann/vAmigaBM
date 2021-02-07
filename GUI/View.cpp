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
ImageView::init(float x, float y, float w, float h, sf::Texture &tex)
{
    init(sf::Vector2f{x,y}, sf::Vector2f{w,h}, tex);
}

void
ImageView::init(float x, float y, float w, sf::Texture &tex)
{
    init(x, y, w, w * (float)tex.getSize().y / (float)tex.getSize().x, tex);
}

void
ImageView::draw(sf::RenderWindow &window)
{
    window.draw(*this);
}
