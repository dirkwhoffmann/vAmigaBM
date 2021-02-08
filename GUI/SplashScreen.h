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
    
    // Text
    TextView info[4] = {
        TextView(view::center), TextView(view::center),
        TextView(view::center), TextView(view::center)
    };

    // Name of the config file
    string configPath;
    string configFile;

    // Action assigned to the space key
    enum { Quit, Aros, Launch} action = Quit;
    
    // Bookkeeping
    /*
    bool configFileFound     = false;
    bool configFileProcessed = false;
    bool kickstartFound      = false;
    bool emulatorReady       = false;
    */
    
    //
    // Initializing
    //

public:

    SplashScreen(class Application &ref);
    ~SplashScreen();
    
    // Delegation methods
    void init();
    void awake() { execLaunchPhase(1); }
    
    // Runs a certain launch phase
    void execLaunchPhase(isize phase);
    
    // void launchEmulator();
    
    
    //
    // Methods from Layer class
    //

public:
    
    virtual bool isVisible() override;
    void handle(const sf::Event &event) override;
    void render() override;
    void resize(float width, float height);
}; 
