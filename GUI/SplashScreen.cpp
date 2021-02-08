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

void
SplashScreen::init()
{
    Layer::init();
    
    // Help text
    text1.setStyle(Assets::get(FontID::logo), 48, sf::Color(0x50,0x50,0x50,0xFF));
    text1.setPosition(sf::Vector2f(w * 0.5, h * 0.8));
    text1.setString("Press F12 to enter the debug console");

    /*
    info1 = sf::Text("Press F12 to enter the debug console",
                     Assets::get(FontID::logo));
    
    info1.setCharacterSize(48);
    info1.setFillColor(sf::Color(0x50,0x50,0x50,0xFF));
    sf::FloatRect textRect = info1.getLocalBounds();
    info1.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top + textRect.height/2.0f);
    info1.setPosition(sf::Vector2f(w * 0.5, h * 0.8));
    */
    
    // Setup background
    background.init(w, h,
                    sf::Color(0x89,0x89,0x89), sf::Color(0x89,0x89,0x89),
                    sf::Color(0xF0,0xF0,0xF0), sf::Color(0xF0,0xF0,0xF0));
    
    // Setup vAmiga logo
    logo.init(0.5 * w, 0.4 * h, 0.66 * w, Assets::get(TextureID::logo));
}

bool
SplashScreen::isVisible()
{
    return app.amiga.isPoweredOff() || app.canvas.isAnimating();
}

void
SplashScreen::handle(const sf::Event &event)
{
    
}

void
SplashScreen::render()
{
    background.draw(app.window);
    logo.draw(app.window);
    text1.draw(app.window);
}

void
SplashScreen::resize(float width, float height)
{
    printf("(%f,%f)\n", width, height); 
}
