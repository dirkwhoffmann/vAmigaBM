// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Aliases.h"

class OS {
    
public:

    static bool gotMouse;
    static int mouseDX;
    static int mouseDY;
    
    // Retains or releases the mouse
    static void retainMouse();
    static void releaseMouse();
    
    // Checks for a mouse movement and saves the delta steps
    static bool mouseMoved();
};
