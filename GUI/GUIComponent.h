// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Aliases.h"

class Amiga;
class Application;
class Canvas;
class Console;
class Controller;
class InputManager;
class SplashScreen;
class StatusBar;

// Base class for all GUI components
class GUIComponent {

protected:

    Amiga &amiga;
    Application &app;
    Canvas &canvas;
    Console &console;
    Controller &controller;
    InputManager &inputManager;
    SplashScreen &splash;
    StatusBar &statusBar;

public:

    GUIComponent(Application& ref);
};
