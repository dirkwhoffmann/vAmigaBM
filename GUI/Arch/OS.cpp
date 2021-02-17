// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "OS.h"

#ifdef __MACH__
#include <ApplicationServices/ApplicationServices.h>
#endif

bool OS::gotMouse = false;
int OS::mouseDX = 0;
int OS::mouseDY = 0;

float
OS::scale(float value)
{
#ifdef __MACH__
    return value * 2.0;
#else
    return value;
#endif
}

void
OS::retainMouse()
{
#ifdef __MACH__
    
    if (!gotMouse) {
    
        CGDisplayHideCursor(kCGNullDirectDisplay);
        CGEventErr err = CGAssociateMouseAndMouseCursorPosition(false);
        if (err != CGEventNoErr) {
            printf("CGAssociateMouseAndMouseCursorPosition returned %d\n", err);
        }
        gotMouse = true;
    }
#endif
}

void
OS::releaseMouse()
{
#ifdef __MACH__
    
    if (gotMouse) {
        
        CGDisplayShowCursor(kCGNullDirectDisplay);
        CGAssociateMouseAndMouseCursorPosition(true);
        gotMouse = false;
    }
#endif
}

bool
OS::mouseMoved()
{
#ifdef __MACH__
    CGAssociateMouseAndMouseCursorPosition(false);
    CGGetLastMouseDelta(&mouseDX, &mouseDY);
    return mouseDX != 0 || mouseDY != 0;
#endif
}
