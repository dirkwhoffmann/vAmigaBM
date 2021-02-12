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

#include "OS.h"
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
    
    // Initial window size
    static const int W = 1536;
    static const int H = W * 0.7525;
    
    
    //
    // Members
    //

public:
        
    // The emulator instance
    Amiga amiga;
    
    // Gateway to the emulator
    Controller controller;
    
    // Splash screen layer
    SplashScreen splashScreen;

    // The emulator layer
    Canvas canvas;

    // The debug console layer
    Console console;

    // The render window
    sf::RenderWindow window;

    // The event loop timer
    sf::Clock clock;

    // The command line parameters
    std::vector<string> argv;
    
    // The current window size
    int curw = W, curh = H;
        
    
    //
    // Initializing
    //
    
public:
    
    Application(int argc, const char *argv[]);
    ~Application();
    
    
    //
    // Launching the app
    //
    
    /* On start, the application executes the following launch sequence:
     *
     * 1. check     : The applications performs several system checks to see if
     *                it can run in the given environment. E.g., it is checked
     *                the underlying graphics system offers shader support.
     *
     * 2. init      : All sub components enter their initial state.
     *
     * 3. configure : All components configure themself. Beside other things,
     *                a configuration file is read which configures the
     *                virtual Amiga.
     *
     * 4. run       : The application enters the main event loop. It remains
     *                in this loop until the user quits the application.
     */
    void check();
    void init();
    void configure();
    void run();

    
    //
    // Running the app
    //
    
    void processEvents();
    void update(sf::Time dt);
    void render();
};
