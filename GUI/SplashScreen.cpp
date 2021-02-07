// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

SplashScreen::SplashScreen(Application &ref) : Layer(ref)
{
    alpha = targetAlpha = 0xFF;
}

SplashScreen::~SplashScreen()
{
    
}

bool SplashScreen::init()
{
    info1 = sf::Text("Press F12 to enter the debug console",
                     Assets::get(FontID::logo));
    
    info1.setCharacterSize(48);
    info1.setFillColor(sf::Color(0x50,0x50,0x50,0xFF));
    sf::FloatRect textRect = info1.getLocalBounds();
    info1.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    info1.setPosition(sf::Vector2f(W * 0.5, H * 0.8));
    
    logo = Assets::get(TextureID::logo);
    
    sf::Vector2u size = logo.getSize();
    unsigned logoWidth = 0.66 * W;
    unsigned logoHeight = logoWidth * ((float)size.y / (float)size.x);
    
    background.setSize(sf::Vector2f(logoWidth, logoHeight));
    background.setTexture(&logo);
    background.setPosition(0.17 * W, 0.125 * H);

    return true;
}

bool SplashScreen::isVisible()
{
    return app.amiga.isPoweredOff() || app.canvas.isAnimating();
}

void SplashScreen::handle(const sf::Event &event)
{
    
}

void SplashScreen::render()
{
    app.window.draw(rectangle, 4, sf::Quads);
    app.window.draw(background);
    app.window.draw(info1);
}
