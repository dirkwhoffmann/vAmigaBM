// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "AssetManager.h"
#include "Console.h"

class Application
{
    // The debug console
    Console console;
 
    //
    // Initializing
    //
    
public:
    
    Application();
    ~Application();
    
    //
    // Running the app
    //
    
    // Main entry point of the application
    void run();
};
