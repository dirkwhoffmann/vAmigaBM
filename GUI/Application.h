// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Console.h"
#include "Interpreter.h"
#include "AssetManager.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

using std::string;

class Application
{

public:

    // The debug console
    Console console;
 
    // Interpreter for commands typed into the debug console
    Interpreter interpreter;
    
    
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
