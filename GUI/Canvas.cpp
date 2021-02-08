// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

Canvas::Canvas(Application &ref) : Layer(ref)
{
    
}

Canvas::~Canvas()
{
    
}

void Canvas::init()
{
    Layer::init();
    
    if (!emuTex.create(HPIXELS, VPIXELS)) {
        throw Exception("Can't create emulator texture");
    }
    
    int x1 = HBLANK_CNT * 4;
    int x2 = HPOS_CNT * 4;
    int y1 = VBLANK_CNT;
    int y2 = VPOS_CNT - 1;
    
    foreground.init(w, h, emuTex);
    foreground.setTextureRect(sf::IntRect(x1, y1, x2 - x1, y2 - y1));
}

void
Canvas::awake()
{
    
}

void
Canvas::handle(const sf::Event &event)
{
    switch (event.type) {
                    
        default:
            break;
    }
}

void
Canvas::update(sf::Time dt)
{
    Layer::update(dt);
    
    if (app.amiga.isPaused()) {
        
        emuTex.update((u8 *)app.amiga.denise.pixelEngine.getNoise());
        
    } else if (app.amiga.isRunning()) {
            
        ScreenBuffer current = app.amiga.denise.pixelEngine.getStableBuffer();
        if (screenBuffer.data != current.data) {
            screenBuffer = current;
            emuTex.update((u8 *)(screenBuffer.data + 4 * HBLANK_MIN));
        }
    }
}

void
Canvas::render()
{
    foreground.setFillColor(sf::Color(0xFF,0xFF,0xFF,alpha));
    
    app.window.draw(foreground);
}
