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
// View
//

View::View(usize flags)
{
    this->flags = flags;
}

void
View::init(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
    update();
}

float
View::x1()
{
    return (flags & Align::Left) ? x : (flags & Align::Right) ? x - w : x - w / 2;
}

float
View::x2()
{
    return (flags & Align::Left) ? x + w : (flags & Align::Right) ? x : x + w / 2;
}

float
View::y1()
{
    return (flags & Align::Top) ? y : (flags & Align::Bottom) ? y - h : y - h / 2;
}

float
View::y2()
{
    return (flags & Align::Top) ? y + h : (flags & Align::Bottom) ? y : y + h / 2;
}

void
View::setW(float w)
{
    if (flags & Align::Proportional) { this->h = w * this->h / this->w; }
    this->w = w;
    update();
}

void
View::setH(float h)
{
    if (flags & Align::Proportional) { this->w = h * this->w / this->h; }
    this->h = h;
    update();
}


//
// ImageView
//

void
ImageView::init(float x, float y, const sf::Texture &tex)
{
    View::init(x, y, tex.getSize().x, tex.getSize().y);
    rectangle.setTexture(&tex);
    
    if (flags & Align::FlippedX) {
        
        auto rect = rectangle.getTextureRect();
        auto size = tex.getSize();
        rectangle.setTextureRect(sf::IntRect(size.x, rect.top, -size.x, rect.height));
    }
    if (flags & Align::FlippedY) {
        
        auto rect = rectangle.getTextureRect();
        auto size = tex.getSize();
        rectangle.setTextureRect(sf::IntRect(rect.left, size.y, rect.width, -size.y));
    }
}

void
ImageView::init(const sf::Texture &tex)
{
    init(0, 0, tex);
}

void
ImageView::update()
{
    rectangle.setPosition(sf::Vector2f{x1(),y1()});
    rectangle.setSize(sf::Vector2f{w,h});
}

void
ImageView::draw(sf::RenderWindow &window)
{
    window.draw(rectangle);
}


//
// GradientView
//

void
GradientView::init(float x, float y, float w, float h,
                   sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr)
{
    View::init(x, y, w, h);
    setColors(ul, ur, ll, lr);
}

void
GradientView::init(float w, float h,
                   sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr)
{
    init(0, 0, w, h, ul, ur, ll, lr);
}

void
GradientView::setColors(sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr)
{
    rectangle[0].color = ul;
    rectangle[1].color = ur;
    rectangle[2].color = ll;
    rectangle[3].color = lr;
}

void
GradientView::update()
{
    rectangle[0].position = sf::Vector2f(x1(), y1());
    rectangle[1].position = sf::Vector2f(x2(), y1());
    rectangle[2].position = sf::Vector2f(x2(), y2());
    rectangle[3].position = sf::Vector2f(x1(), y2());
}

void
GradientView::draw(sf::RenderWindow &window)
{
    window.draw(rectangle, 4, sf::Quads);
}


//
// TextView
//

void
TextView::update()
{
    text.setPosition(x1(), y1());
}

void
TextView::setString(const string &str)
{
    text.setString(str);
    setW(text.getLocalBounds().width);
    setH(text.getLocalBounds().height);
}

void
TextView::setFontSize(unsigned size)
{
    text.setCharacterSize(size);
    setW(text.getLocalBounds().width);
    setH(text.getLocalBounds().height);
}

void
TextView::setStyle(const sf::Font &font, unsigned int size, const sf::Color &color)
{
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(color);
}

void
TextView::draw(sf::RenderWindow &window)
{
    window.draw(text);
}
