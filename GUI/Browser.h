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
#include "View.h"

class Browser : public Layer {
    
    // Views
    GradientView background;
    ImageView icon;
    TextBoxView name;
    TextBoxView path;
    sf::RectangleShape line;
    TextBoxView item[16];
    
    // State
    std::vector<string> files;
    std::vector<string> filtered;

    isize selectedItem = 0;
    string input;
    
    
public:
    
    bool needsUpdate = false;
    
    
    //
    // Initializing
    //
    
public:
    
    Browser(class Application &ref);
    ~Browser();
    
    // Delegation methods
    void init();
    void awake();
    
    virtual void open() override;
    
    
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
    
    void setNeedsUpdate() { needsUpdate = true; }

    void refresh();
};
