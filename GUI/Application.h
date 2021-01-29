// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

// #include <iostream>

#include"Amiga.h"

#include "Console.h"
#include "Interpreter.h"
#include "AssetManager.h"

#include <SFML/Graphics.hpp>

using std::string;

class Application
{

    //
    // Constants
    //
    
    static const int W = 1536;
    static const int H = W * 0.7525;
    
    
    //
    // Members
    //
    
public:
    
    // The emulator instance
    class Amiga amiga;
    
    // The current screen buffer
    ScreenBuffer screenBuffer = { nullptr, false };
    
    // The emulator texture
    sf::Texture emuTex;
    
    // The debug console
    Console console;
 
    // Interpreter for commands typed into the debug console
    Interpreter interpreter;

    // The render window
    sf::RenderWindow window;

    // Render objects
    sf::RectangleShape background;
    sf::RectangleShape foreground;
    sf::Texture logoTex;
    sf::Text info1;
    sf::Vertex rectangle[4] =
    {
        sf::Vertex(sf::Vector2f(0,0), sf::Color(0x89,0x89,0x89)),
        sf::Vertex(sf::Vector2f(W,0), sf::Color(0x89,0x89,0x89)),
        sf::Vertex(sf::Vector2f(W,H), sf::Color(0xF0,0xF0,0xF0)),
        sf::Vertex(sf::Vector2f(0,H), sf::Color(0xF0,0xF0,0xF0))
    };
    
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
    void processKeyEvents(const sf::Event& event);
    void update();
    void render();
};
