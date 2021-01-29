// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"
#include "Interpreter.h"

void
Interpreter::execute(const string& command)
{
    std::list<string> tokens;
    
    // Split command string
    std::stringstream ss(command);
    std::string token;
    while (std::getline(ss, token, ' ')) tokens.push_back(token);
 
    for (auto &it : tokens) printf("%s ", it.c_str());
    printf("\n");

    // Call the appropriate command handler
    if (!tokens.empty()) {
        
        string &cmd = tokens.front();
        tokens.pop_front();
        
        if (cmd == "amiga") {
            executeAmiga(tokens);
        } else if (cmd == "agnus") {
            executeAgnus(tokens);
        } else if (cmd == "help") {
           executeHelp(tokens);
        } else if (cmd == "joshua") {
            app.console.print("GREETINGS PROFESSOR FALKEN");
        } else {
            app.console.print("Unknown command");
        }
    }
}

void
Interpreter::executeAgnus(const std::list<std::string> &argv)
{
    app.console.print("Agnus");
}

void
Interpreter::executeAmiga(const std::list<std::string> &argv)
{
    app.console.print("Amiga");
}

void
Interpreter::executeHelp(const std::list<std::string> &argv)
{
    app.console.print("Help me");
}
