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
    
    class Application &app;
    
public:

    OS(Application &ref) : app(ref) { };
    
    bool gotMouse;
    int mouseDX;
    int mouseDY;
    
    int mouseCenterX;
    int mouseCenterY;
    
    // Translates a logical coordinate or size to a physical coordinate or size
    static float scale(float value);
    
    // Retains or releases the mouse
    void retainMouse();
    void releaseMouse();
    
    // Checks for a mouse movement and saves the delta steps
    bool mouseMoved();
};
