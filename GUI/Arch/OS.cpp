// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "OS.h"

#include <ApplicationServices/ApplicationServices.h>

bool OS::gotMouse = false;
int OS::mouseDX = 0;
int OS::mouseDY = 0;

void
OS::retainMouse()
{
    if (!gotMouse) {
    
        CGDisplayHideCursor(kCGNullDirectDisplay);
        CGEventErr err = CGAssociateMouseAndMouseCursorPosition(false);
        if (err != CGEventNoErr) {
            printf("CGAssociateMouseAndMouseCursorPosition returned %d\n", err);
        }
        gotMouse = true;
    }
}

void
OS::releaseMouse()
{
    if (gotMouse) {
        
        CGDisplayShowCursor(kCGNullDirectDisplay);
        CGAssociateMouseAndMouseCursorPosition(true);
        gotMouse = false;
    }
}

bool
OS::mouseMoved()
{
    CGAssociateMouseAndMouseCursorPosition(false);
    CGGetLastMouseDelta(&mouseDX, &mouseDY);
    return mouseDX != 0 || mouseDY != 0;
}
