// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Application.h"

class SplashScreen : public Layer {
    
    static const int W = 1536;
    static const int H = W * 0.7525;
    
    sf::RectangleShape background;
    sf::Text info1;
    sf::Vertex rectangle[4] =
    {
        sf::Vertex(sf::Vector2f(0,0), sf::Color(0x89,0x89,0x89)),
        sf::Vertex(sf::Vector2f(W,0), sf::Color(0x89,0x89,0x89)),
        sf::Vertex(sf::Vector2f(W,H), sf::Color(0xF0,0xF0,0xF0)),
        sf::Vertex(sf::Vector2f(0,H), sf::Color(0xF0,0xF0,0xF0))
    };

    sf::Texture logo;
    
    
    //
    // Initializing
    //

public:

    SplashScreen(class Application &ref);
    ~SplashScreen();
    
    bool init();
    
    
    //
    // Methods from Layer class
    //

    virtual bool isActive() override { return true; }
    virtual bool isResponsive() override;
    virtual bool isVisible() override;
    virtual bool isAnimating() override { return false; }
    void handle(const sf::Event &event) override;
    void render() override;
}; 
