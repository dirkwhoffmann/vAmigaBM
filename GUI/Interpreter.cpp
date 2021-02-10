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


Interpreter::Interpreter(Application &ref) :
app(ref), controller(ref.controller), console(ref.console)
{
    registerInstructions();
};

Arguments
Interpreter::split(const string& userInput)
{
    std::stringstream ss(userInput);
    std::string token;
    Arguments result;

    while (std::getline(ss, token, ' ')) result.push_back(lowercased(token));
    return result;
}
    
isize
Interpreter::autoComplete(Arguments &argv)
{
    isize result = 0;

    Command *current = &root;
    std::string prefix, token;

    for (auto it = argv.begin(); current && it != argv.end(); it++) {
        
        result += (isize)current->autoComplete(*it);
        current = current->seek(*it);
    }
    return result;
}

isize
Interpreter::autoComplete(string& userInput)
{
    Arguments tokens = split(userInput);
    isize result = autoComplete(tokens);
    
    if (result > 0) {
        userInput = "";
        for (const auto &it : tokens) {
            // userInput += (userInput == "" ? "" : " ") + it;
            userInput += it + " ";
        }
    }
    return result;
}

/*
void
Interpreter::exec(std::istream &stream)
{
    isize line = 0;
    string command;
        
    while(std::getline(stream, command)) {

        line++;
        printf("Line %zd: %s\n", line, command.c_str());

        // Skip empty lines
        if (command == "") continue;

        // Skip comments
        if (command.substr(0,1) == "#") continue;
        
        // Execute the command
        if (!exec(command, true)) {
            throw Exception(command, line);
        }
    }
}
*/

bool
Interpreter::exec(const string& userInput, bool verbose)
{
    // Split the command string
    Arguments tokens = split(userInput);
        
    // Auto complete the token list
    autoComplete(tokens);
            
    // Process the command
    return exec(tokens, verbose);
}

bool
Interpreter::exec(Arguments &argv, bool verbose)
{
    Command *current = &root;
    std::string prefix, token;

    // In 'verbose' mode, print the token list
    if (verbose) {
        for (const auto &it : argv) console << it << ' ';
        console << '\n';
    }

    // Skip empty lines
    if (argv.empty()) return true;
    
    try {
        
        // Seek the command in the command tree
        while (current) {
            
            // Extract token
            token = argv.empty() ? "" : argv.front();
            
            // Break the loop if this token is unknown
            Command *next = current->seek(token);
            if (next == nullptr) break;
            
            // Go one level down
            prefix += next->token + " ";
            current = next;
            if (!argv.empty()) argv.pop_front();
        }
        
        // Error out if no token has been recognized
        if (current == &root) throw SyntaxError();

        // Error out if no command handler is present
        if (current->func == nullptr) throw TooFewArgumentsError();
                
        // Check the argument count
        if (argv.size() < current->numArgs) throw TooFewArgumentsError();
        if (argv.size() > current->numArgs) throw TooFewArgumentsError();
        
        // Call the command handler
        (controller.*(current->func))(argv, current->param);
        return true;

    } catch (SyntaxError &err) {
        console << token << ": Syntax error";
        console << '\n';
        
    } catch (TooFewArgumentsError &err) {
        console << current->tokens() << ": Too few arguments";
        console << '\n';
        
    } catch (TooManyArgumentsError &err) {
        console << current->tokens() << ": Too many arguments";
        console << '\n';
        
    } catch (EnumParseError &err) {
        console << current->tokens() << ": Invalid key. ";
        console << "Expected: " << err.what() << '\n';

    } catch (ParseError &err) {
        console << "Invalid argument. ";
        console << "Expected: " << err.what() << '\n';

    } catch (ConfigUnsupportedError) {
        console << "This option is not yet supported.";
        console << '\n';
        
    } catch (ConfigLockedError &err) {
        console << "This option is locked because the Amiga is powered on.";
        console << '\n';

    } catch (ConfigArgError &err) {
        console << "Error: Invalid argument. Expected: " << err.what();
        console << '\n';

    } catch (ConfigFileReadError &err) {
        console << "Error: Unable to read file " << err.what();
        console << '\n';

    } catch (VAError &err) {
        console << err.what();
        console << '\n';
    }
    
    return false;
}

void
Interpreter::usage(Command& current)
{
    console << "Usage: " << current.usage() << '\n' << '\n';
}

void
Interpreter::help(const string& userInput)
{
    // Split the command string
    Arguments tokens = split(userInput);
        
    // Auto complete the token list
    autoComplete(tokens);
            
    // Process the command
    help(tokens);
}

void
Interpreter::help(Arguments &argv)
{
    Command *current = &root;
    std::string prefix, token;
    
    while (1) {
                
        // Extract token
        token = argv.empty() ? "" : argv.front();
        
        // Check if this token matches a known command
        Command *next = current->seek(token);
        if (next == nullptr) break;
        
        prefix += next->token + " ";
        current = next;
        if (!argv.empty()) argv.pop_front();
    }

    help(*current);
}

void
Interpreter::help(Command& current)
{
    // Print the usage string
    usage(current);
    
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

        console.tab(tab - size);
        console << "<" << it << "> : ";
        console << (int)opts.size() << (opts.size() == 1 ? " choice" : " choices");
        console << '\n' << '\n';
        
        for (auto &opt : opts) {

            string name = opt->token == "" ? "<>" : opt->token;
            console.tab(tab + 2 - (int)name.length());
            console << name;
            console << " : ";
            console << opt->info;
            console << '\n';
        }
        console << '\n';
    }
}
