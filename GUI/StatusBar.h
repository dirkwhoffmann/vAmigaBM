// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Layer.h"

class StatusBar : public Layer {
        
    GradientView bar;
    
    ImageView powerLed;
    ImageView driveLed[4];
    TextView cylinder[4];
    ImageView disk[4];
    ProgressView spin[4];
    
    ImageView port[2];
    ImageView mute;
    TextView mhz;
    ImageView state;
    
    
    //
    // Initializing
    //
    
public:
    
    StatusBar(class Application &ref);
    ~StatusBar();
    
    // Delegation methods
    void init();
    void awake();
    

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
    
    // Refreshs dirty elements
    void refresh();
};
