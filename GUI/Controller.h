// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Amiga.h"
#include "Interpreter.h"

class Controller
{
    // Reference to other components
    class Application &app;
    class Console &console;

public:
    
    // The emulator instance
    Amiga amiga;
        
    
    //
    // Initializing
    //
    
public:
    
    Controller(Application &ref);
    ~Controller();
    
    // Creates the emulator instance
    void init();
    
    // Destroys the emulator instance
    void deinit();
    
    
    //
    // Processing messages
    //
    
    void processMessage(long id, long data);
    
    
    //
    // Executing commands
    //
    
    void exec(const std::string& command);
    
    template <Token t1> void exec(Arguments& argv);
    template <Token t1, Token t2> void exec(Arguments& argv);
    template <Token t1, Token t2, Token t3> void exec(Arguments& argv);

    template <Token t1> void exec(Arguments& argv, int n);
    template <Token t1, Token t2> void exec(Arguments& argv, int n);
    template <Token t1, Token t2, Token t3> void exec(Arguments& argv, int n);
};
