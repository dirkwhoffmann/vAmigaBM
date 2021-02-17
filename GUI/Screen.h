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

class Screen {
  
public:
    
    // Analyzes all video modes and returns the maximum dimensions
    static sf::Vector2u maxDimensions(bool verbose = false);    
};
