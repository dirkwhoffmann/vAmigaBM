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
    
 
        
    // sf::Text info1;

    // Background
    GradientView background;
  
    // vAmiga logo
    ImageView logo = ImageView(view::center);

    // Help text
    TextView text1 = TextView(view::center);
    
    
    //
    // Initializing
    //

public:

    SplashScreen(class Application &ref);
    ~SplashScreen();
    
    void init();
    
    
    //
    // Methods from Layer class
    //

    virtual bool isVisible() override;
    void handle(const sf::Event &event) override;
    void render() override;
    void resize(float width, float height);
}; 
