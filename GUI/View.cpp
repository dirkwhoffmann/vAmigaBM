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
View::init(float w, float h)
{
    this->x = 0;
    this->y = 0;
    this->w = w;
    this->h = h;
    update();
}

void
View::setX(float x)
{
    if (this->x == x) return;
    
    this->x = x;
    update();
}

void
View::setY(float y)
{
    if (this->y == y) return;
    
    this->y = y;
    update();
}

void
View::setPosition(float x, float y)
{
    if (this->x != x || this->y != y) {
        
        this->x = x;
        this->y = y;
        update();
    }
}

void
View::setW(float w)
{
    if (flags & Align::Proportional) { this->h = w * this->h / this->w; }

    if (this->w != w) {
        this->w = w;
        update();
    }
}

void
View::setH(float h)
{
    if (flags & Align::Proportional) { this->w = h * this->w / this->h; }

    if (this->h != h) {
        this->h = h;
        update();
    }
}

void
View::setSize(float w, float h)
{
    assert((flags & Align::Proportional) == 0);
    
    if (this->w != w || this->h != h) {
        
        this->w = w;
        this->h = h;
        update();
    }
}

void
View::update()
{
    rx = x;
    ry = y;
    
    rx -= (flags & Align::Left) ? 0 : (flags & Align::Right) ? w : w / 2;
    ry -= (flags & Align::Top) ? 0 : (flags & Align::Bottom) ? h : h / 2;
}


//
// ImageView
//

void
ImageView::init(const sf::Texture &tex)
{
    View::init(tex.getSize().x, tex.getSize().y);
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
ImageView::update()
{
    View::update();
    
    rectangle.setPosition(sf::Vector2f { rx,ry } );
    rectangle.setSize(sf::Vector2f{w,h});
}

void
ImageView::draw(sf::RenderWindow &window, const sf::Shader *shader)
{
    window.draw(rectangle, shader);
}


//
// GradientView
//

void
GradientView::init(float w, float h,
                   sf::Color ul, sf::Color ur, sf::Color ll, sf::Color lr)
{
    View::init(w, h);
    setColors(ul, ur, ll, lr);
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
    View::update();

    rectangle[0].position = sf::Vector2f( rx, ry );
    rectangle[1].position = sf::Vector2f( rx + w, ry );
    rectangle[2].position = sf::Vector2f( rx + w, ry + h );
    rectangle[3].position = sf::Vector2f( rx, ry + h );
}

void
GradientView::draw(sf::RenderWindow &window, const sf::Shader *shader)
{
    window.draw(rectangle, 4, sf::Quads, shader);
}


//
// TextView
//

void
TextView::update()
{
    View::update();

    text.setPosition(rx, ry);
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
TextView::draw(sf::RenderWindow &window, const sf::Shader *shader)
{
    window.draw(text, shader);
}
