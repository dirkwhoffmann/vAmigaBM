// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

class Layer {
    
    // The render texture
    sf::RenderTexture texture;
    
    // Alpha channel parameters
    isize alpha = 0, targetAlpha = 0;
    
    // Time until alpha is supposed to reach targetAlpha in seconds
    float delay;
    
    // Triggers an alpha animation
    void setTargetAlpha(isize target, float seconds);
    
    // Event loop handlers
    void draw() { };
    void update(sf::Time dt);
    void handle(const sf::Event &event) { };
};

