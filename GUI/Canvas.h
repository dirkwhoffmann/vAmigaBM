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

    // Dimensions of the noise texture
    static const int TEX_NOISE_W = HPIXELS;
    static const int TEX_NOISE_H = VPIXELS;

    // Dimensions of a long frame or short frame textures
    static const int TEX_FRAME_W = HPIXELS;
    static const int TEX_FRAME_H = VPIXELS;
    
    // Dimensions of the merge texture
    static const int TEX_MERGE_W = HPIXELS;
    static const int TEX_MERGE_H = VPIXELS * 2;

    // The displayed part of the final texture
    sf::IntRect textureRect = {
        HBLANK_CNT * 4,
        VBLANK_CNT * 2,
        (HPOS_CNT - HBLANK_CNT) * 4,
        (VPOS_CNT - VBLANK_CNT - 2) * 2
    };

private:
    
    // The current screen buffer
    ScreenBuffer screenBuffer = { nullptr, false };
    ScreenBuffer prevBuffer = { nullptr, false };

    // Indicates whether the recently drawn frames were long or short frames
    bool currLOF = true;
    bool prevLOF = true;

    // The noise texture
    sf::Texture noiseTexture;
    sf::RectangleShape noiseTextureRect;
    
    // The emulator textures
    sf::Texture longFrameTexture;
    sf::Texture shortFrameTexture;
    sf::RectangleShape longFrameRect;
    sf::RectangleShape shortFrameRect;

    // The merge texture
    sf::RenderTexture mergeTexture;
    sf::RectangleShape mergeTextureRect;
        
    // The merge shaders
    sf::Shader *mergeShader = nullptr;
    sf::Shader *mergeBypassShader = nullptr;
    
    // Indicates if the aspect ratio should be kept constant
    bool letterbox = true;
    
    // Initial mouse position after releasing
    int baseX, baseY;
    
        
    //
    // Initializing
    //
    
public:
    
    Canvas(class Application &ref);
    ~Canvas();
    
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
    void mouseMoved(int dx, int dy);
    void powerOn();
    void powerOff();
};
