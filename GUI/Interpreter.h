/// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include <string>

class Interpreter
{
    // Reference to the associated application
     class Application &application;
    
    //
    // Initializing
    //

public:
    
    Interpreter(Application &ref) : application(ref) { };

    
    //
    // Execting commands
    //
    
    void execute(const std::string& command);    
};
