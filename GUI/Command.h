/// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Utils.h"

class Controller;

typedef std::list<string> Arguments;

struct Command {
    
    // The token string (e.g., "agnus" or "set")
    string token;
    
    // A string describing the token type (e.g., "component or "command")
    string type;
    
    // The help string for this command
    string info;
    
    // The sub commands of this command
    std::vector<Command> args;
    
    // Command handler
    void (Controller::*func)(Arguments&, long) = nullptr;
    
    // Number of additional arguments expected by the command handler
    isize numArgs = 0;
    
    // An additional paramter passed to the command handler
    long param = 0;
    
    // Indicates if this command appears in the help descriptions
    bool hidden = false;
        
    // Appends a new descriptor to the args vector
    Command *add(const string &token,
                       const string &a1,
                       const string &help,
                       void (Controller::*func)(Arguments&, long) = nullptr,
                       isize numArgs = 0, long param = 0);
    
    Command *add(const string &t1, const string &t2,
                       const string &a1,
                       const string &help,
                       void (Controller::*func)(Arguments&, long) = nullptr,
                       isize numArgs = 0, long param = 0);
    
    Command *add(const string &t1, const string &t2, const string &t3,
                       const string &a1,
                       const string &help,
                       void (Controller::*func)(Arguments&, long) = nullptr,
                       isize numArgs = 0, long param = 0);
    
    // Returns a matching descriptor from the args vector
    Command *seek(const string& token);

    // Collects the type descriptions in the args vector
    std::vector<std::string> types();
    
    // Filters out the args of a given type
    std::vector<Command *> filter(string& type);
    
    // Returns a syntax string for this command
    string syntax();
};
