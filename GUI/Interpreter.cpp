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

Interpreter::Interpreter(Application &ref) : app(ref), controller(ref.controller)
{
};

bool
Interpreter::matches(const std::string& s1, const std::string& s2)
{
    if (s1.length() != s2.length()) return false;
    
    for(size_t i = 0; i < s1.length(); i++) {
        if (tolower(s1[i]) != tolower(s2[i])) return false;
    }
    
    return true;
}

bool
Interpreter::matches(const std::list<string>& argv, const std::string& s)
{
    return argv.empty() ? false : matches(argv.front(), s);
}

string
Interpreter::lowercased(const std::string& s)
{
    string result;
    for (auto c : s) { result += tolower(c); } // .push_back(c); }
    return result;
}

string
Interpreter::pop(Arguments& argv)
{
    if (argv.empty()) return "";
    
    string result = argv.front();
    argv.pop_front();
    return result;
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

template <> void
Interpreter::exec <Component::amiga> (Arguments& argv)
{
    if (argv.empty()) throw TooFewArgumentsError();
    
    string &cmd = argv.front();
    argv.pop_front();

    if (cmd == "help") {
        controller.exec <Component::amiga, Command::help> (argv);
        return;
    }
    if (cmd == "pause") {
        controller.exec <Component::amiga, Command::pause> (argv);
        return;
    }
    if (cmd == "power") {
        controller.exec <Component::amiga, Command::power> (argv);
        return;
    }
    if (cmd == "reset") {
        controller.exec <Component::amiga, Command::reset> (argv);
        return;
    }
    if (cmd == "run") {
        controller.exec <Component::amiga, Command::run> (argv);
        return;
    }

    throw UnknownCommandError(cmd);
}

template <> void
Interpreter::exec <Component::agnus> (Arguments& argv)
{
    if (argv.empty()) throw TooFewArgumentsError();
    
    string &cmd = argv.front();
    argv.pop_front();
    
    if (cmd == "help") {
        controller.exec <Component::amiga, Command::help> (argv);
        return;
    }

    throw UnknownCommandError(cmd);
}

template <> void
Interpreter::exec <Component::dfn> (Arguments& argv, int n)
{
    if (argv.empty()) throw TooFewArgumentsError();
}

template <> void
Interpreter::exec <Component::easterEgg> (Arguments& argv)
{
    app.console.println("GREETINGS PROFESSOR HOFFMANN");
    app.console.println();
    app.console.println("THE ONLY WINNING MOVE IS NOT TO PLAY.");
    app.console.println();
    app.console.println("HOW ABOUT A NICE GAME OF CHESS?");
}

template <> void
Interpreter::exec <Component::rtc> (Arguments& argv)
{
    if (argv.empty()) throw TooFewArgumentsError();
}


/*
template <> void
Interpreter::exec <Command::help> (Arguments& argv)
{
    if (argv.empty()) {
        controller.exec <Command::help> (argv);
        return;
    }
}
*/

void
Interpreter::exec(const string& command)
{
    std::list<string> tokens;
    
    // Split the command string
    std::stringstream ss(command);
    std::string token;
    while (std::getline(ss, token, ' ')) tokens.push_back(lowercased(token));
 
    // Only proceed if there is somethig to evaluate
    if (tokens.empty()) return;

    // Call the appropriate command handler
    try {
        
        string &cmd = tokens.front();
        tokens.pop_front();
        
        if (cmd == "amiga") {
            exec <Component::amiga> (tokens);
        } else if (cmd == "agnus") {
            exec <Component::agnus> (tokens);
        } else if (cmd == "df0") {
            exec <Component::dfn> (tokens, 0);
        } else if (cmd == "df1") {
            exec <Component::dfn> (tokens, 1);
        } else if (cmd == "df2") {
            exec <Component::dfn> (tokens, 2);
        } else if (cmd == "df3") {
            exec <Component::dfn> (tokens, 3);
        } else if (cmd == "help") {
           controller.exec <Command::help> (tokens);
        } else if (cmd == "rtc") {
            exec <Component::rtc> (tokens);
        } else if (cmd == "joshua") {
            exec <Component::easterEgg> (tokens);
        } else {
            // app.console << "Syntax error: " << cmd << '\n';
            exec("help");
        }
        
    } catch (UnknownCommandError &err) {
        app.console << "Unknown command: " << err.what() << '\n';
        exec(tokens.front() + " help");
    } catch (TooFewArgumentsError &err) {
        app.console << "Too few arguments" << '\n';
        exec(tokens.front() + " help");
    } catch (ConfigError &err) {
        app.console << "Invalid argument. Expected: " << err.what() << '\n';
    }
}
