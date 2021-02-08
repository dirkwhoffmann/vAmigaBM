// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Utils.h"

#include "Exception.h"
#include "Controller.h"
#include "Layer.h"
#include "SplashScreen.h"
#include "Canvas.h"
#include "Console.h"
#include "Interpreter.h"
#include "AssetManager.h"
#include "View.h"

#include <SFML/Graphics.hpp>

class Application
{

public:
    
    //
    // Constants
    //
    
    // Window dimensions
    static const int W = 1536;
    static const int H = W * 0.7525;
    
    
    //
    // Members
    //

public:
    
    // The emulator instance
    Amiga amiga;

    // The event loop timer
    sf::Clock clock;
    
    // Gateway to the emulator
    Controller controller;
    
    // Splash screen layer
    SplashScreen splashScreen;

    // The emulator layer
    Canvas canvas;

    // The debug console layer
    Console console;
 
    // Interpreter for commands typed into the debug console
    Interpreter interpreter;

    // The render window
    sf::RenderWindow window;

    
    //
    // Initializing
    //
    
public:
    
    Application();
    ~Application();
    
    
    //
    // Launching the app
    //
    
    /* On start, the application processes the following launching sequence:
     *
     * 1. check     : The applications performs some system checks to see if
     *                it can run in the given environment.
     *
     * 2. init      : All sub components are initialized.
     *
     * 3. configure : The emulator is configured by reading and processing a
     *                configuration file. If a file name was provided as a
     *                command line parameter, this file is used. Otherwise, the
     *                default init file is used.
     *
     * 4. run        : The application enters the main event loop. It remains
     *                 in this loop until the user quits the application.
     */
    void check();
    void init();
    void configure(const string& file);
    void run();

    
    //
    // Running the app
    //
    
    // Game loop handlers
    void processEvents();
    void update(sf::Time dt);
    void render();
};
