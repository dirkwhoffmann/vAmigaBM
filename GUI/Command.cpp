/// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

Command *
Command::add(const std::string &token,
                   const std::string &a1,
                   const std::string &help,
                   void (Controller::*func)(Arguments&, long),
                   isize num, long param)
{
    // Make sure the key does not yet exist
    assert(seek(token) == nullptr);

    // Expand template tokens
    if (token == "dfn") {
        add("df0", a1, help, func, num, 0);
        add("df1", a1, help, func, num, 1);
        add("df2", a1, help, func, num, 2);
        add("df3", a1, help, func, num, 3);
        return nullptr;
    }
    
    // Register instruction
    Command d { token, a1, help, std::vector<Command>(), func, num, param };
    args.push_back(d);
    
    return seek(token);
}

Command *
Command::add(const std::string &t1, const std::string &t2,
                   const std::string &a1,
                   const std::string &help,
                   void (Controller::*func)(Arguments&, long),
                   isize num, long param)
{
    // Expand template tokens
    if (t1 == "dfn") {
        add("df0", t2, a1, help, func, num, 0);
        add("df1", t2, a1, help, func, num, 1);
        add("df2", t2, a1, help, func, num, 2);
        add("df3", t2, a1, help, func, num, 3);
        return nullptr;
    }
    
    return seek(t1)->add(t2, a1, help, func, num, param);
}

Command *
Command::add(const std::string &t1, const std::string &t2, const std::string &t3,
                   const std::string &a1,
                   const std::string &help,
                   void (Controller::*func)(Arguments&, long),
                   isize num, long param)
{
    // Expand template tokens
    if (t1 == "dfn") {
        add("df0", t2, t3, a1, help, func, num, 0);
        add("df1", t2, t3, a1, help, func, num, 1);
        add("df2", t2, t3, a1, help, func, num, 2);
        add("df3", t2, t3, a1, help, func, num, 3);
        return nullptr;
    }
    
    return seek(t1)->add(t2, t3, a1, help, func, num, param);
}

Command *
Command::seek(const string& token)
{
    for (auto& it : args) {
        if (it.token == token) return &it;
    }
    return nullptr;
}

std::vector<std::string>
Command::types()
{
    std::vector<std::string> result;
    
    for (auto &it : args) {
        
        if (it.hidden) continue;
        
        if (std::find(result.begin(), result.end(), it.type) == result.end()) {
            result.push_back(it.type);
        }
    }
    
    for (auto &it : result) {
        printf("key: %s\n", it.c_str());
    }
    
    return result;
}

std::vector<Command *>
Command::filter(std::string& type)
{
    std::vector<Command *> result;
    
    for (auto &it : args) {
        
        if (it.hidden) continue;
        if (it.type == type) result.push_back(&it);
    }
    
    return result;
}

string
Command::syntax()
{
    string firstArg, otherArgs;
    
    // The simple case: All command tokens have been parsed successfully
    if (args.empty()) {
        return numArgs == 0 ? "" : numArgs == 1 ? "<argument>" : "<arguments>";
    }
    
    // Collect all argument types
    auto t = types();
    
    // Describe the first argument
    for (usize i = 0; i < t.size(); i++) {
        firstArg += (i == 0 ? "" : "|") + t[i];
    }
    firstArg = "<" + firstArg + ">";
    
    // Describe the remaining arguments (if any)
    bool printArg = false, printOpt = false;
    for (auto &it : args) {
        if (it.func != nullptr && it.numArgs == 0) printOpt = true;
        if (it.numArgs > 0 || !it.args.empty()) printArg = true;
    }
    if (printArg) {
        otherArgs = printOpt ? " [<arguments>]" : " <arguments>";
    }
    
    return firstArg + otherArgs;
}
