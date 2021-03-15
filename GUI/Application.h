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

#include "manymouse.h"
#include "Aliases.h"

#include "GUIComponent.h"
#include "Exception.h"
#include "Controller.h"
#include "Screen.h"
#include "InputManager.h"
#include "Layer.h"
#include "SplashScreen.h"
#include "Browser.h"
#include "Canvas.h"
#include "StatusBar.h"
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
    
    // The input device manager
    InputManager inputManager;
            
    // The splash screen layer
    SplashScreen splashScreen;

    // The emulator layer
    Canvas canvas;

    // The status bar
    StatusBar statusBar;
    
    // The file browser layer
    Browser fileBrowser;
    
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
    void fps(float fps, float hz);
    
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
    void play(SoundID, float vol, float pan, isize min, isize max);

    void playInsert(float vol, float pan) { play(SoundID::insert, vol, pan, 0, 15); }
    void playEject(float vol, float pan)  { play(SoundID::eject, vol, pan, 0, 15); }
    void playClick(float vol, float pan) { play(SoundID::click, vol, pan, 13, 15); }

    void setPan(isize pan);
};
