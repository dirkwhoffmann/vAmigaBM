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
 
void
Interpreter::exec(const string& userInput, bool verbose)
{
    Arguments tokens;
    std::string token;

    // Split the command string
    std::stringstream ss(userInput);
    while (std::getline(ss, token, ' ')) tokens.push_back(lowercased(token));
        
    // Auto complete the token list
    autoComplete(tokens);
            
    // Process the token list
    if (!tokens.empty()) exec(tokens, verbose);
}

bool
Interpreter::exec(Arguments &argv, bool verbose)
{
    Command *current = &root;
    std::string prefix, token;
    
    // Print the token list (if requested)
    if (verbose) {
        for (const auto &it : argv) app.console << it << ' ';
        app.console << '\n';
    }

    while (current) {
                
        // Extract token
        token = argv.empty() ? "" : argv.front();

        printf("auto completion(%s): %s\n", token.c_str(), current->autoComplete(token).c_str());
        
        // Search token
        Command *next = current->seek(token);
        if (next == nullptr) break;
        
        prefix += next->token + " ";
        current = next;
        if (!argv.empty()) argv.pop_front();
    }

    // Error out if no command handler is present
    if (current->func == nullptr) {
        if (token != "") app.console << "Parse error: " << token << '\n';
        prefix != "" ? syntax(*current, prefix) : help();
        return false;
    }
    
    try {
        // Check the remaining arguments
        if (argv.size() < current->numArgs) throw TooFewArgumentsError();
        if (argv.size() > current->numArgs) throw TooFewArgumentsError();
        
        // Call the command handler
        (controller.*(current->func))(argv, current->param);
        
    } catch (TooFewArgumentsError &err) {
        app.console << "Error: Too few arguments." << '\n';
        syntax(*current, prefix);
        
    } catch (TooManyArgumentsError &err) {
        app.console << "Error: Too many arguments." << '\n';
        syntax(*current, prefix);
        
    } catch (EnumParseError &err) {
        app.console << "Error: Invalid key. Expected: " << err.what() << '\n';

    } catch (ParseError &err) {
        app.console << "Error: Invalid argument. Expected: " << err.what() << '\n';

    } catch (ConfigLockedError &err) {
        app.console << "Error: Power off the Amiga before changing this option." << '\n';
        
    } catch (ConfigArgError &err) {
        app.console << "Error: Invalid argument. Expected: " << err.what() << '\n';
    
    } catch (VAError &err) {
        app.console << err.what() << '\n';
    }
    
    return false;
}

/*
string
Interpreter::autoComplete(const string& userInput)
{
    Arguments tokens;
    std::string token;

    // Split the command string
    std::stringstream ss(userInput);
    while (std::getline(ss, token, ' ')) tokens.push_back(lowercased(token));

    // Only proceed if some input is given
    if (tokens.empty()) return "";

    // Hand over the token vector
    return autoComplete(tokens);
}
*/

void
Interpreter::autoComplete(Arguments &argv)
{
    Command *current = &root;
    std::string prefix, token;
    
    for (auto it = argv.begin(); it != argv.end(); it++) {
        
        *it += current->autoComplete(*it);
        current = current->seek(*it);
    }
}

void
Interpreter::help()
{
    app.console << "Type 'help' for a list of available commands." << '\n' << '\n';
}

void
Interpreter::usage(Command& current, const string& prefix)
{
    app.console << "Usage: " << prefix << current.syntax() << '\n' << '\n';
}

void
Interpreter::syntax(Command& current, const string& prefix)
{
    // Print the usage string
    usage(current, prefix);
    
    // Collect all argument types
    auto types = current.types();

    // Determine tabular positions to align the output
    int tab = 0;
    // for (auto &it : types) tab = std::max(tab, (int)it.length());
    for (auto &it : current.args) {
        tab = std::max(tab, (int)it.token.length());
        tab = std::max(tab, 2 + (int)it.type.length());
    }
    tab += 5;
    
    for (auto &it : types) {
        
        auto opts = current.filterType(it);
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
