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
    
    // Background
    GradientView background;
  
    // vAmiga logo
    ImageView logo = ImageView(view::center);
    
    // Text messages
    TextView errMsg = TextView(view::center);
    TextView runMsg = TextView(view::center);
    TextView info[3] = {
        TextView(view::center), TextView(view::center), TextView(view::center)
    };

    // Name of the config file
    string configPath;
    string configFile;

    // Key actions
    typedef enum { None, Launch, Quit, Aros, Menu, Console } KeyAction;
    KeyAction spcAction = None;
    KeyAction f10Action = None;

    
    //
    // Initializing
    //

public:

    SplashScreen(class Application &ref);
    ~SplashScreen();
    
    // Delegation methods
    void init();
    void configure() { launchPhase(1); }
    
    // Runs a certain launch phase
    void launchPhase(isize phase);
        
    // Loads the Aros replacement Roms
    bool loadAros();
    
    
    //
    // Methods from Layer class
    //

public:
    
    virtual bool isVisible() override;
    void handle(const sf::Event &event) override;
    void render() override;
    void resize(float width, float height);
}; 
