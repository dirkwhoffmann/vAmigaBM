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
    TextView info1  = TextView(Align::Centered);
    TextView info2  = TextView(Align::Centered);
    TextView info3  = TextView(Align::Centered);

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
    void awake() { launchPhase(1); }
    
    // Runs a certain launch phase
    void launchPhase(isize phase);
        
    // Loads the Aros replacement Roms
    bool loadAros();
    
    // Experimental
    // float myfloat = .05f;
    
    
    //
    // Performing continuous tasks
    //
    
public:
    
    void update(u64 frames, sf::Time dt) override;
    void render() override;
    
    
    //
    // Responding to events
    //
    
public:
        
    void respond(const sf::Event &event) override;
    void resize(float width, float height) override;
}; 
