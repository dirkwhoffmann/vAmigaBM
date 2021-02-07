// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include <SFML/Graphics.hpp>

class Layer {
    
public:
    
    // The render texture
    sf::RenderTexture texture;
    
    // Alpha channel parameters
    isize alpha = 0, targetAlpha = 0;
    
    // Time until alpha is supposed to reach targetAlpha in seconds
    float delay;
    
    // Triggers an alpha animation
    void setTargetAlpha(isize target, float seconds);
    
    // Informs about the visual state
    bool isVisible() { return alpha > 0; }
    bool isAnimating() { return alpha != targetAlpha; }
    bool isFadingIn() { return targetAlpha > alpha; }
    bool isFadingOut() { return targetAlpha < alpha; }
    
    // Event loop handlers
    void draw() { };
    void update(sf::Time dt);
    void handle(const sf::Event &event) { };
};

