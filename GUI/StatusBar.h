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

namespace StatusBarItem {

static const u32 DRIVE_LED  = 0x00000001;
static const u32 DRIVE_CYL  = 0x00000010;
static const u32 DISK_ICON  = 0x00000100;
static const u32 DISK_SPIN  = 0x00001000;
static const u32 POWER_LED  = 0x00010000;
static const u32 MUTE       = 0x00020000;
static const u32 MHZ        = 0x00040000;
static const u32 STATE      = 0x00080000;
static const u32 PORTS      = 0x00100000;
}

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
    
public:
    
    u32 needsUpdate = 0;
    
    
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
    void alphaDidChange() override;
    
    
    //
    // Refreshs dirty elements
    //

public:
    
    void setNeedsUpdate(u32 flags, isize i = 0) { needsUpdate |= flags << i; }

    void refresh();
    void refreshMhz(float value);
    void refreshDrive(isize nr);
    void refreshPort(PortNr nr);
};
