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
    CmdDescriptor agnusSet =
    CmdDescriptor("set", "<key>", "<value>",
                  "Configure the component",
                  nullptr);
    agnusSet.args.push_back(CmdDescriptor("model", "<model>", "",
                                          "Set the chip revision",
                                          &Controller::exec <Component::amiga, Command::help>));
    agnusSet.args.push_back(CmdDescriptor("test", "<foo>", "",
                                          "Set something",
                                          &Controller::exec <Component::amiga, Command::help>));
    agnusSet.args.push_back(CmdDescriptor("other", "<foo>", "",
                                          "Set another thing",
                                          &Controller::exec <Component::amiga, Command::help>));
    
    CmdDescriptor agnus = CmdDescriptor("agnus", "<command>", "[<arguments>]",
                                        "Bus scheduling, Frame control",
                                        nullptr);
    agnus.args.push_back(agnusSet);
    
    root = CmdDescriptor("", "<component>", "<command> [<arguments>]",
                         "",
                         nullptr);
    root.args.push_back(agnus);
    
    
    registerInstr("amiga", "help", "",
                  "Print command instructions",
                  &Controller::exec <Component::amiga, Command::help>);
    
    registerInstr("amiga", "inspect", "",
                  "Display the internal state",
                  &Controller::exec <Component::amiga, Command::inspect>);

    registerInstr("amiga", "on", "",
                  "Turn the Amiga on",
                  &Controller::exec <Component::amiga, Command::on>);

    registerInstr("amiga", "off", "",
                  "Turn the Amiga off",
                  &Controller::exec <Component::amiga, Command::off>);

    registerInstr("amiga", "reset", "",
                  "Reset the emulator thread",
                  &Controller::exec <Component::amiga, Command::reset>);

    registerInstr("amiga", "run", "",
                  "Start the emulator thread",
                  &Controller::exec <Component::amiga, Command::run>);

    registerInstr("amiga", "pause", "",
                  "Halt the emulator thread",
                  &Controller::exec <Component::amiga, Command::pause>);
};

void
Interpreter::registerInstr(const std::string &token1, const std::string &token2,
                           const std::string &args, const std::string &help,
                           void (Controller::*func)(Arguments&))
{
    descriptors.push_back(CommandDescriptor { token1, token2, args, help, func });
}

/*
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
*/


string
Interpreter::lowercased(const std::string& s)
{
    string result;
    for (auto c : s) { result += tolower(c); } // .push_back(c); }
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
    if (cmd == "on") {
        controller.exec <Component::amiga, Command::on> (argv);
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

CmdDescriptor *
CmdDescriptor::searchToken(const string& token)
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
    
    // Split the command string
    std::stringstream ss(userInput);
    std::string prefix;
    std::string token;
    while (std::getline(ss, token, ' ')) tokens.push_back(lowercased(token));
    if (tokens.empty()) return;
    
    CmdDescriptor *current = &root;
    
    while (!tokens.empty() && current) {
        
        printf("current->name = %s\n", current->name.c_str());
        
        // Extract token
        token = tokens.front();

        // Search token
        CmdDescriptor *next = current->searchToken(token);
        if (next == nullptr) break;
        
        prefix += token + " ";
        current = next;
        tokens.pop_front();
    }

    // Check if a command handler is present
    if (current->func) {
        printf("Command handler found. Call it...\n");
        (controller.*(current->func))(tokens);
        return;
    }
    
    //
    // Syntax error
    //
    
    // Determine horizontal tabular positions to align the output
    int tab = (int)current->arg1.length();
    for (auto &it : current->args) {
        tab = std::max(tab, (int)it.name.length());
    }
    tab += 8;
    
    app.console << "Syntax: ";
    app.console << prefix << current->arg1 << ' ' << current->arg2 << '\n' << '\n';
    
    app.console.tab(tab - (int)current->arg1.length());
    app.console << current->arg1 << " : ";
    app.console << (int)current->args.size() << " options" << '\n' << '\n';
    
    for (auto &it : current->args) {
        app.console.tab(tab - (int)it.name.length());
        app.console << it.name;
        app.console << " : ";
        app.console << it.info;
        app.console << '\n';
    }
    app.console << '\n';
}

void
Interpreter::help()
{
    std::vector <const std::string> items;
    
    // Extract all available components
    for (auto& it: descriptors) {
        if(std::find(items.begin(), items.end(), it.token1) == items.end()) {
            items.push_back(it.token1);
        }
    }
    
    app.console << "Syntax: <component> <command> [ <arguments> ]" << '\n';
    app.console << "        <component> ::= ";

    int size = (int)items.size();
    for (int i = 0; i < size; i++) {
        app.console.tab(24);
        app.console << items[i].c_str();
        if (i < size - 1) app.console << " |";
        app.console << '\n';
    }

    app.console << '\n';
    app.console << "Type '<component> help' for more details." << '\n';
}

void
Interpreter::help(const string& component)
{
    std::vector <string> items;
    std::vector <string> args;
    std::vector <string> help;

    // Extract all available commands for this component
    for (auto& it: descriptors) {
        if(std::find(items.begin(), items.end(), it.token2) == items.end()) {
            items.push_back(it.token2);
            args.push_back(it.args);
            help.push_back(it.help);
        }
    }
    int size = (int)items.size();
    
    // Determine horizontal tabular positions to align the output
    int tab1 = 12;
    int tab2 = 0;
    for (int i = 0; i < items.size(); i++) {
        tab2 = std::max(tab2, (int)(items[i].length() + args[i].length()));
    }
    tab2 += tab1;
    
    app.console << "Syntax: " << component << " <command> [ <arguments> ]" << '\n';
    app.console << '\n';
    app.console.tab(tab2 - 9);
    // app.console << "<command> : One out of the following " << size << " commands" << '\n' << '\n';
    app.console << "<command> :" << " Description" << '\n' << '\n';

    for (int i = 0; i < size; i++) {
        // app.console.tab(tab1);
        app.console.tab(tab2 - (int)(items[i].length() + args[i].length()));
        app.console << items[i].c_str();
        app.console << " " << args[i].c_str();
        app.console.tab(tab2);
        app.console << ": ";
        app.console << help[i].c_str();
        app.console << '\n';
    }

    app.console << '\n';
    app.console << "Type '<component> help' for more details." << '\n';
}
