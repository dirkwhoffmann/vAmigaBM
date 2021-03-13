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
    
    // Position
    static const isize numRows = 12;
    static const isize w = 1016;
    static const isize h = 832;
    static const isize pad = 32;
    static const isize icn = 128;
    isize dx = 0;
    isize dy = 0;
    
    // Views
    GradientView background;
    ImageView icon;
    TextView name;
    TextView path;
    SolidView selector;
    sf::RectangleShape line;
    TextView item[numRows];
    
    // State
    std::vector<string> files;
    std::vector<string> filtered;

    isize selectedItem = 0;
    string input;
    
    
public:
    
    bool needsUpdate = false;

    // Action function
    std::function<void(const string &s)> action; 
    
    
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
    virtual void close() override;

    
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

private:
    
    void refresh();
    
    isize highlightedRow() const;
    isize indexForRow(isize row) const;
};
