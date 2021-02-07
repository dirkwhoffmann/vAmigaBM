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
    
    // Informs about the current state
    virtual bool isActive() = 0;
    virtual bool isResponsive() = 0;
    virtual bool isVisible() { return alpha > 0; }
    virtual bool isAnimating() { return alpha != targetAlpha; }
    virtual bool isFadingIn() { return targetAlpha > alpha; }
    virtual bool isFadingOut() { return targetAlpha < alpha; }
    
    // Event loop handlers
    virtual void handle(const sf::Event &event) = 0;
    virtual void update(sf::Time dt);
    virtual void render() { };
};

