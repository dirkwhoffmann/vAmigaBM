// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "GUIComponent.h"
#include <SFML/Graphics.hpp>

class Layer : public GUIComponent
{
    
public:
        
    // The render texture
    sf::RenderTexture texture;
        
    // Alpha channel parameters
    isize alpha = 0, targetAlpha = 0;
    
    // Time until alpha is supposed to reach targetAlpha in seconds
    float delay;
    
    //
    // Initializing
    //
    
    Layer(Application &ref) : GUIComponent(ref) { }
    void init();

    
    //
    // Working with the layer
    //
    
    // Triggers an alpha animation
    void setTargetAlpha(isize target, float seconds);
    
    // Informs about the visual state of this layer
    virtual bool isVisible() { return alpha > 0; }
    virtual bool isOpaque() { return alpha == 0xFF; }
    virtual bool isAnimating() { return alpha != targetAlpha; }
    virtual bool isFadingIn() { return targetAlpha > alpha; }
    virtual bool isFadingOut() { return targetAlpha < alpha; }
    
    // Event loop handlers
    virtual void handle(const sf::Event &event) = 0;
    virtual void update(sf::Time dt);
    virtual void render() { };
    
    // Delegation methods
    virtual void resize(float width, float height) = 0;
};

