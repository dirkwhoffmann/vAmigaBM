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
    registerInstructions();
};

string
Interpreter::lowercased(const std::string& s)
{
    string result;
    for (auto c : s) { result += tolower(c); }
    return result;
}

void
Interpreter::exec(const string& userInput)
{
    Arguments tokens;
    std::string token;

    // Split the command string
    std::stringstream ss(userInput);
    while (std::getline(ss, token, ' ')) tokens.push_back(lowercased(token));

    // Only proceed if some input is given
    if (tokens.empty()) return;

    // Hand over the token vector
    exec(tokens);
}

bool
Interpreter::exec(Arguments &argv)
{
    Command *current = &root;
    std::string prefix, token;
    
    while (current) {
                
        // Extract token
        token = argv.empty() ? "" : argv.front();

        // Search token
        Command *next = current->seek(token);
        if (next == nullptr) break;
        
        prefix += token + " ";
        current = next;
        if (!argv.empty()) argv.pop_front();
    }

    // Check if a command handler is present
    if (current->func) {
        
        try {
            // Check the remaining arguments
            if (argv.size() < current->numArgs) {
                throw TooFewArgumentsError();
            }
            if (argv.size() > current->numArgs) {
                throw TooFewArgumentsError();
            }
            
            // Call the command handler
            (controller.*(current->func))(argv, current->param);
            return true;
            
        } catch (TooFewArgumentsError &err) {
            // app.console << "Too few arguments" << '\n';
        } catch (TooManyArgumentsError &err) {
            // app.console << "Too many arguments" << '\n';
        }
    }
    
    // Syntax error
    syntax(*current, prefix);
    
    return false;
}

void
Interpreter::syntax(Command& current, const string& prefix)
{
    // Print the usage string
    app.console << "Usage: " << prefix << current.syntax() << '\n' << '\n';
    
    // Collect all argument types
    auto types = current.types();

    // Determine tabular positions to align the output
    int tab = 0;
    for (auto &it : types) tab = std::max(tab, (int)it.length());
    tab += 7;
    
    for (auto &it : types) {
        
        auto opts = current.filter(it);
        int size = (int)it.length();

        app.console.tab(tab - size);
        app.console << "<" << it << "> : ";
        app.console << (int)opts.size() << (opts.size() == 1 ? " choice" : " choices");
        app.console << '\n' << '\n';
        
        for (auto &opt : opts) {

            string name = opt->token == "" ? "<>" : opt->token;
            app.console.tab(tab + 2 - (int)name.length());
            app.console << name;
            app.console << " : ";
            app.console << opt->info;
            app.console << '\n';
        }
        app.console << '\n';
    }
}
