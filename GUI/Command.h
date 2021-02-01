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

class Controller;

typedef std::list<std::string> Arguments;

struct CmdDescriptor {
    
    string name;
    string arg1;
    string info;
    std::vector<CmdDescriptor> args;
    void (Controller::*func)(Arguments&, long) = nullptr;
    isize numArgs = 0;
    long param = 0;
    bool hidden = false;
        
    // Appends a new descriptor to the args vector
    CmdDescriptor *add(const std::string &token,
                       const std::string &a1,
                       const std::string &help,
                       void (Controller::*func)(Arguments&, long) = nullptr,
                       isize numArgs = 0, long param = 0);
    
    CmdDescriptor *add(const std::string &t1, const std::string &t2,
                       const std::string &a1,
                       const std::string &help,
                       void (Controller::*func)(Arguments&, long) = nullptr,
                       isize numArgs = 0, long param = 0);
    
    CmdDescriptor *add(const std::string &t1, const std::string &t2, const std::string &t3,
                       const std::string &a1,
                       const std::string &help,
                       void (Controller::*func)(Arguments&, long) = nullptr,
                       isize numArgs = 0, long param = 0);
    
    // Returns a matching descriptor from the args vector
    CmdDescriptor *seek(const string& token);

    // Collects the type descriptions in the args vector
    std::vector<std::string> types();
    
    // Filters out the args of a given type
    std::vector<CmdDescriptor *> filter(std::string& type);
    
    // Returns a syntax string
    string syntax();
};



