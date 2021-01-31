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

CmdDescriptor *
CmdDescriptor::add(const std::string &token,
                   const std::string &a1, const std::string &a2,
                   const std::string &help,
                   void (Controller::*func)(Arguments&, long),
                   isize num, long param)
{
    // Make sure the key does not yet exist
    assert(seek(token) == nullptr);

    // Expand template tokens
    if (token == "dfn") {
        add("df0", a1, a2, help, func, num, 0);
        add("df1", a1, a2, help, func, num, 1);
        add("df2", a1, a2, help, func, num, 2);
        add("df3", a1, a2, help, func, num, 3);
        return nullptr;
    }
    
    // Register instruction
    CmdDescriptor d { token, a1, a2, help, std::vector<CmdDescriptor>(), func, num, param };
    args.push_back(d);
    
    return seek(token);
}

CmdDescriptor *
CmdDescriptor::add(const std::string &t1, const std::string &t2,
                   const std::string &a1, const std::string &a2,
                   const std::string &help,
                   void (Controller::*func)(Arguments&, long),
                   isize num, long param)
{
    // Expand template tokens
    if (t1 == "dfn") {
        add("df0", t2, a1, a2, help, func, num, 0);
        add("df1", t2, a1, a2, help, func, num, 1);
        add("df2", t2, a1, a2, help, func, num, 2);
        add("df3", t2, a1, a2, help, func, num, 3);
        return nullptr;
    }
    
    return seek(t1)->add(t2, a1, a2, help, func, num, param);
}

CmdDescriptor *
CmdDescriptor::add(const std::string &t1, const std::string &t2, const std::string &t3,
                   const std::string &a1, const std::string &a2,
                   const std::string &help,
                   void (Controller::*func)(Arguments&, long),
                   isize num, long param)
{
    // Expand template tokens
    if (t1 == "dfn") {
        add("df0", t2, t3, a1, a2, help, func, num, 0);
        add("df1", t2, t3, a1, a2, help, func, num, 1);
        add("df2", t2, t3, a1, a2, help, func, num, 2);
        add("df3", t2, t3, a1, a2, help, func, num, 3);
        return nullptr;
    }
    
    return seek(t1)->add(t2, t3, a1, a2, help, func, num, param);
}

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

CmdDescriptor *
CmdDescriptor::seek(const string& token)
{
    for (auto& it : args) {
        if (it.name == token) return &it;
    }
    return nullptr;
}

void
Interpreter::exec(const string& userInput)
{
    std::list<string> tokens;
    std::string token;

    // Split the command string
    std::stringstream ss(userInput);
    while (std::getline(ss, token, ' ')) tokens.push_back(lowercased(token));

    // Only proceed if some input is given
    if (tokens.empty()) return;

    // If a single word is typed in, check the list of single-word commands
    if (execSingle(tokens)) return;
    
    // Call the standard execution handler
    execMultiple(tokens);
}

bool
Interpreter::execSingle(Arguments &argv)
{
    if (argv.front() == "clear") {
        assert(false);
        // app.console.clear(); // TODO
        return true;
    }

    if (argv.front() == "clear") {
        app.console.clearLine();
        return true;
    }

    if (argv.front() == "joshua") {
        controller.exec <Token::easteregg> (argv, 0);
        return true;
    }
    
    return false;
}

bool
Interpreter::execMultiple(Arguments &argv)
{
    CmdDescriptor *current = &root;
    std::string prefix;
    
    while (current) {
                
        // Extract token
        std::string token = argv.empty() ? "" : argv.front();

        // Search token
        CmdDescriptor *next = current->seek(token);
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
    
    //
    // Syntax error
    //
    
    // Determine horizontal tabular positions to align the output
    int tab = (int)current->arg1.length();
    for (auto &it : current->args) {
        tab = std::max(tab, (int)it.name.length());
    }
    tab += 7;
    
    app.console << "usage: ";
    app.console << prefix << current->arg1 << ' ' << current->arg2 << '\n';
    
    if (int size = (int)current->args.size()) {
        
        app.console << '\n';
        app.console.tab(tab - (int)current->arg1.length());
        app.console << current->arg1 << " : ";
        app.console << size << (size == 1 ? " option" : " options") << '\n' << '\n';
        
        for (auto &it : current->args) {
            string name = it.name == "" ? "''" : it.name;
            app.console.tab(tab - (int)name.length());
            app.console << name;
            app.console << " : ";
            app.console << it.info;
            app.console << '\n';
        }
        app.console << '\n';
    }
    
    return false;
}
