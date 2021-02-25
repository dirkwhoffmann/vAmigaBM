// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "View.h"

class SplashScreen : public Layer {
    
    // Background
    GradientView background;
  
    // vAmiga logo
    ImageView logo = ImageView(Align::Top | Align::Proportional);
    ImageView title = ImageView(Align::Centered);
    
    // Text messages
    TextView title1 = TextView(Align::Right | Align::Top);
    TextView title2 = TextView(Align::Left | Align::Top);
    TextView errMsg = TextView(Align::Centered);
    TextView runMsg = TextView(Align::Centered);
    TextView info[3] = {
        TextView(Align::Centered),
        TextView(Align::Centered),
        TextView(Align::Centered)
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
    
    // Experimental
    float myfloat = .05f;
    
    
    //
    // Methods from Layer class
    //

public:
    
    void handle(const sf::Event &event) override;
    void update(sf::Time dt) override;
    void render() override;
    void resize(float width, float height) override;
    
    
    //
    // Opening and closing
    //
    
public:
    
    // Shows or hides the canvas window
    void open() { setTargetAlpha(0xFF, 0.5); }
    void close() { setTargetAlpha(0x00, 0.5); }
    void toggle() { isVisible() ? close() : open(); }
}; 
