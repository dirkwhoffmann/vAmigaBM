// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "GUIComponent.h"

#include "Application.h"
#include "Canvas.h"
#include "Controller.h"
#include "SplashScreen.h"
#include "Console.h"

GUIComponent::GUIComponent(Application& ref) :
amiga(ref.amiga),
app(ref),
controller(ref.controller),
splash(ref.splashScreen),
canvas(ref.canvas),
console(ref.console)
{
    
}
