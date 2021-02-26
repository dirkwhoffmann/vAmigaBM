// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

void
Layer::init()
{
    assert(app.window.isOpen());
    
    // w = app.window.getView().getSize().x;
    // h = app.window.getView().getSize().y;
    
    // printf("w = %f h = %f\n", w, h);
}

void
Layer::setTargetAlpha(isize target, float seconds)
{
    targetAlpha = target;
    delay = seconds;
}

void
Layer::update(u64 frames, sf::Time dt)
{
    if (alpha != targetAlpha) {

        float delta = dt.asSeconds();
        
        if (alpha < targetAlpha) {
            alpha += 0xFF * delta / delay;
            if (alpha > targetAlpha) alpha = targetAlpha;
        }
        if (alpha > targetAlpha) {
            alpha -= 0xFF * delta / delay;
            if (alpha < targetAlpha) alpha = targetAlpha;
        }
        
        alphaDidChange();
    }
}
