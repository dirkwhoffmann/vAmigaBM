// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Application.h"

class Canvas : public Layer {
    
public:
    
    // Texture constants
    static const int texX1 = HBLANK_CNT * 4;
    static const int texX2 = HPOS_CNT * 4;
    static const int texY1 = VBLANK_CNT * 2;
    static const int texY2 = (VPOS_CNT - 1) * 2;
    static const int texW  = texX2 - texX1;
    static const int texH  = texY2 - texY1;

private:
    
    // The current screen buffer
    ScreenBuffer screenBuffer = { nullptr, false };
    ScreenBuffer prevBuffer = { nullptr, false };

    // Indicates whether the recently drawn frames were long or short frames
    bool currLOF = true;
    bool prevLOF = true;

    // The emulator textures
    sf::Texture longFrameTexture;
    sf::Texture shortFrameTexture;

    // The merge texture
    sf::Texture mergeTexture;
    
    // The merge shaders
    sf::Shader *mergeShader = nullptr;
    sf::Shader *mergeBypassShader = nullptr;

    // Render object
    ImageView view = ImageView(Align::UpperLeft);
    
    // Indicates if the aspect ratio should be kept constant
    bool letterbox = true;
    
    // Initial mouse position after releasing
    int baseX, baseY;
    
public:
        
    //
    // Initializing
    //
    
public:
    
    Canvas(class Application &ref);
    ~Canvas();
    
    // Delegation methods
    void init();
    void configure();
    

    //
    // Methods from Layer class
    //
    
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
    
    
    //
    // Working with the mouse
    //
    
    void mouseMoved(int dx, int dy);
};
