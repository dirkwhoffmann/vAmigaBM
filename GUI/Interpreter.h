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
#include <list>

class Interpreter
{
    // Reference to the associated application
     class Application &app;
    
    //
    // Initializing
    //

public:
    
    Interpreter(Application &ref) : app(ref) { };

    
    //
    // Execting commands
    //
    
    void execute(const std::string& command);    
    void executeAgnus(const std::list<std::string> &argv);
    void executeAmiga(const std::list<std::string> &argv);
    void executeHelp(const std::list<std::string> &argv);
};
