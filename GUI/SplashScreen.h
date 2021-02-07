// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

// #include "Application.h"
#include "View.h"

class SplashScreen : public Layer {
    
    // Drawing dimensions
    float w, h;
        
    sf::RectangleShape logo;
    sf::Text info1;
    sf::Vertex rectangle[4];
    // sf::Texture logo;
    
    ImageView logoView;
    
    
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

    virtual bool isVisible() override;
    void handle(const sf::Event &event) override;
    void render() override;
    void resize(float width, float height);
}; 
