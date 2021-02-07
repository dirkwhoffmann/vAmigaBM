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

ImageView::ImageView(usize flags)
{
    this->flags = flags;
}

void
ImageView::init(const sf::Vector2f &origin, const sf::Vector2f &size, const sf::Texture &tex)
{
    setTexture(&tex);
    setSize(size);
    setPosition(origin);
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
ImageView::setPosition(const sf::Vector2f &position)
{
    setPosition(position.x, position.y);
}

void
ImageView::setPosition(float x, float y)
{
    if (flags & view::center) {
        
        x -= getSize().x / 2;
        y -= getSize().y / 2;
    }
    RectangleShape::setPosition(x, y);
}

void
ImageView::draw(sf::RenderWindow &window)
{
    window.draw(*this);
}

//
// GradientView
//

GradientView::GradientView(usize flags)
{
    this->flags = flags;
}

void
GradientView::init(float x, float y, float w, float h,
                   sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr)
{
    this->w = w;
    this->h = h;
    
    setPosition(x, y);
    rectangle[0].color = ul;
    rectangle[1].color = ur;
    rectangle[2].color = ll;
    rectangle[3].color = lr;
}

void
GradientView::init(float w, float h,
                   sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr)
{
    init(0, 0, w, h, ul, ur, ll, lr);
}

void
GradientView::setPosition(const sf::Vector2f &position)
{
    setPosition(position.x, position.y);
}

void
GradientView::setPosition(float x, float y)
{
    float xx = x + (flags & view::center) ? w / 2 : 0;
    float yy = y + (flags & view::center) ? h / 2 : 0;

    rectangle[0].position = sf::Vector2f(xx + 0, yy + 0);
    rectangle[1].position = sf::Vector2f(xx + w, yy + 0);
    rectangle[2].position = sf::Vector2f(xx + w, yy + h);
    rectangle[3].position = sf::Vector2f(xx + 0, yy + h);
}

void
GradientView::draw(sf::RenderWindow &window)
{
    window.draw(rectangle, 4, sf::Quads);
}
