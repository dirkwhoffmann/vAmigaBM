// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Utils.h"

#include "GUIComponent.h"
#include "OS.h"
#include "Exception.h"
#include "Controller.h"
#include "Screen.h"
#include "Layer.h"
#include "SplashScreen.h"
#include "Canvas.h"
#include "Console.h"
#include "Interpreter.h"
#include "AssetManager.h"
#include "View.h"
#include "AmigaMusicStream.h"

class Application : public GUIComponent
{

public:
    
    //
    // Constants
    //
    
    int winXmin = 0;
    int winYmin = 0;
    int winXinit = 0;
    int winYinit = 0;
        
    // Aspect ration of the render window
    static constexpr float aspectRatio = 0.7525;
    
    
    //
    // Members
    //

public:
    
    // The command line parameters
    std::vector<string> argv;
        
    // The application window
    sf::RenderWindow window;

    // The emulator instance
    Amiga amiga;
    
    // Gateway to the emulator
    Controller controller;
    
    // Gateway to the host operating system
    OS os;
        
    // The splash screen layer
    SplashScreen splashScreen;

    // The emulator layer
    Canvas canvas;

    // The debug console layer
    Console console;

    // The music stream
    AmigaMusicStream musicStream;
                
    // Asset managers
    Assets assets;
    
    // The sound object pool
    sf::Sound sound[16];
    

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
     * 1. check     : Several system checks are performed to see if the
     *                application can run in the given environment. E.g., it is
     *                checked the underlying graphics system offers shader
     *                support.
     *
     * 2. init      : All sub components enter their initial state. At this
     *                stage, those initializations are performed that do not
     *                rely on the state of other components as these components
     *                might still be uninitialized.
     *
     * 3. awake     : This is the second initialization phase. At this point,
     *                a component can assume that all components have completed
     *                the first initialization phase.
     *
     * 4. run       : The application enters the main event loop. It remains
     *                in this loop until the user quits the application.
     */
    void check();
    void init();
    void awake();
    void run();

    
    //
    // Performing continuous tasks
    //
    
    void update(u64 frames, sf::Time dt);
    void render();
    void fps(float fps);
    
    //
    // Responding to events
    //

    void respond(sf::Event &event);
    void resize(float w, float h);
    
    
    //
    // Working with the audio backend
    //

    /* This functions searches the object pool for a free sound object,
     * attaches a sound buffer from the asset storage and plays the sound. The
     * min and max parameters can be used to limit the search range. This
     * feature is used for the clicking sounds to limit the number of sounds
     * that are played simultaneously.
     */
    void play(SoundID, float volume = 25.0, isize min = 0, isize max = 15);

    void playInsert() { play(SoundID::insert, 25.0); }
    void playEject()  { play(SoundID::eject, 25.0); }
    void playClick()  { play(SoundID::click, 25.0, 13, 15); }
};
