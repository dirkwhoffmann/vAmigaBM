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

#ifdef __MACH__
#include <ApplicationServices/ApplicationServices.h>
#endif

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
    gotMouse = true;
    
    app.window.setMouseCursorGrabbed(true);
    app.window.setMouseCursorVisible(false);
    
    mouseCenterX = app.window.getSize().x / 2;
    mouseCenterY = app.window.getSize().y / 2;
    auto center = sf::Vector2i(app.os.mouseCenterX,app.os.mouseCenterY);
    sf::Mouse::setPosition(center, app.window);
    
/*
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
*/
}

void
OS::releaseMouse()
{
    if (gotMouse) {

        app.window.setMouseCursorGrabbed(false);
        app.window.setMouseCursorVisible(true);
        gotMouse = false;
    }
    /*
#ifdef __MACH__
    
    if (gotMouse) {
        
        CGDisplayShowCursor(kCGNullDirectDisplay);
        CGAssociateMouseAndMouseCursorPosition(true);
        gotMouse = false;
    }
#endif
     */
}

bool
OS::mouseMoved()
{
    sf::Vector2i current = sf::Mouse::getPosition(app.window);
    
    mouseDX = current.x - mouseCenterX;
    mouseDY = current.y - mouseCenterY;
    
    if (mouseDX != 0 || mouseDY != 0) {

        // printf("lockX: %d lockY: %d current: (%d,%d)\n",
        //        mouseLockX, mouseLockY, current.x, current.y);
        printf("dx: %d dy: %d\n", mouseDX, mouseDY);
        
        sf::Mouse::setPosition(sf::Vector2i(mouseCenterX, mouseCenterY), app.window);
        return true;
    }
    return false;
            
        /*
#ifdef __MACH__
    CGAssociateMouseAndMouseCursorPosition(false);
    CGGetLastMouseDelta(&mouseDX, &mouseDY);
    return mouseDX != 0 || mouseDY != 0;
#else
    return false;
#endif
         */
}
