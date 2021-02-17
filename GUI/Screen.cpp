// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

sf::Vector2u
Screen::maxDimensions(bool verbose)
{
    auto result = sf::Vector2u(0,0);
    
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();

    for (auto &mode : modes) {
        
        if (verbose) {
            printf("%d x %d: %d\n", mode.width, mode.height, mode.bitsPerPixel);
        }
        
        if (mode.width > result.x) result.x = mode.width;
        if (mode.height > result.y) result.y = mode.height;
    }

    return result;
}
