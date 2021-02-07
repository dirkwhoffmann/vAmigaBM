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

#include "Controller.h"
#include "Layer.h"
#include "SplashScreen.h"
#include "Console.h"
#include "Interpreter.h"
#include "AssetManager.h"

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
    
    // The current screen buffer
    ScreenBuffer screenBuffer = { nullptr, false };
    
    // The emulator texture
    sf::Texture emuTex;
 
    // Gateway to the emulator
    Controller controller;
    
    // The intro screen
    SplashScreen splashScreen;
    
    // The debug console
    Console console;
 
    // Interpreter for commands typed into the debug console
    Interpreter interpreter;

    // The render window
    sf::RenderWindow window;

    // Render objects
    sf::RectangleShape foreground;

    
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
    
    // Game loop handlers
    void processEvents();
    void update(sf::Time dt);
    void render();
};
