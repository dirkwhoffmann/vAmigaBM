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

class Controller
{
    
public:
    
    // The emulator instance
    Amiga amiga;
        
    
    //
    // Initializing
    //
    
public:
    
    Controller();
    ~Controller();
    
    // Creates the emulator instance
    void init();
    
    // Destroys the emulator instance
    void deinit();
    
    
    //
    // Processing messages
    //
    
    void processMessage(long id, long data);
};
