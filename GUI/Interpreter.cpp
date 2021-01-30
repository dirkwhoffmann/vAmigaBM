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

bool
Interpreter::matches(std::string const& s1, std::string const& s2)
{
    if (s1.length() != s2.length()) return false;
    
    for(size_t i = 0; i < s1.length(); i++) {
        if (tolower(s1[i]) != tolower(s2[i])) return false;
    }
    
    return true;
}

void
Interpreter::print(const string& s)
{
    app.console.print(s);
}

void
Interpreter::println(const string& s)
{
    app.console.println(s);
}

void
Interpreter::execute(const string& command)
{
    std::list<string> tokens;
    
    // Split the command string
    std::stringstream ss(command);
    std::string token;
    while (std::getline(ss, token, ' ')) tokens.push_back(token);
 
    for (auto &it : tokens) printf("%s ", it.c_str());
    printf("\n");

    // Only proceed if there is somethig to evaluate
    if (tokens.empty()) return;

    // Call the appropriate command handler
    try {
        
        string &cmd = tokens.front();
        tokens.pop_front();
        
        if (matches(cmd, "amiga")) {
            executeAmiga(tokens);
        } else if (matches(cmd, "agnus")) {
            executeAgnus(tokens);
        } else if (matches(cmd, "help")) {
           executeHelp(tokens);
        } else if (matches(cmd, "rtc")) {
           executeRtc(tokens);
        } else if (matches(cmd, "joshua")) {
            
            app.console.println("GREETINGS PROFESSOR HOFFMANN");
            app.console.println();
            app.console.println("THE ONLY WINNING MOVE IS NOT TO PLAY.");
            app.console.println();
            app.console.println("HOW ABOUT A NICE GAME OF CHESS?");

        } else {
            app.console << "Unknown command: " << cmd << '\n';
        }

    } catch (ConfigError &err) {
        app.console << "Invalid argument. Expected: " << err.what() << '\n';
    }
}

void
Interpreter::executeAgnus(const std::list<std::string> &argv)
{
    app.console << "Agnus" << '\n';
}

void
Interpreter::executeAmiga(const std::list<std::string> &argv)
{
    app.console << "Amiga" << '\n';
}

void
Interpreter::executeHelp(const std::list<std::string> &argv)
{
    if (argv.empty()) {
        println("Syntax: <component> <command> [ <arguments> ]");
        // println("");
        println("        <component> ::= Amiga | Agnus | CIA | RTC");
        println("");
        println("Type help <component> for a detailed list of commands.");
        return;
    }
}

void
Interpreter::executeRtc(const std::list<std::string> &argv)
{
    app.console.println("RTC");
    app.controller.amiga.configure(OPT_RTC_MODEL, 99);
}
