// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Application.h"

void
Layer::init()
{
    assert(app.window.isOpen());
}

void
Layer::open()
{
    targetAlpha = 0xFF;
}

void
Layer::close()
{
    targetAlpha = 0x00;
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
