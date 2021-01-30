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

struct ParseError : public std::exception
{
    std::string description;
    ParseError() : description("") { }
    ParseError(const std::string &s) : description(s) { }
    const char *what() const throw() override { return description.c_str(); }
};

struct UnknownComponentError : public ParseError {
    UnknownComponentError(const std::string &s) : ParseError(s) { }
};

struct UnknownCommandError : public ParseError {
    UnknownCommandError(const std::string &s) : ParseError(s) { }
};

struct TooFewArgumentsError : public ParseError {
    TooFewArgumentsError() : ParseError() { }
};

struct TooManyArgumentsError : public ParseError {
    TooManyArgumentsError() : ParseError() { }
};

struct CmdDescriptor {
    
    string name;
    string arg1;
    string arg2;
    string info; 
    std::vector<CmdDescriptor> args;
    void (Controller::*func)(Arguments&);
    
    CmdDescriptor(const string &_name,
                  const string &_arg1,
                  const string &_arg2,
                  const string &_info,
                  void (Controller::*_func)(Arguments&)) :
    name(_name), arg1(_arg1), arg2(_arg2), info(_info), func(_func) { };
    
    CmdDescriptor *searchToken(const string& token);
};

struct CommandDescriptor {

    string token1, token2, args, help;
    void (Controller::*func)(Arguments&);
};

enum class Component
{
    agnus,
    amiga,
    dfn,
    rtc,
    easterEgg
};

enum class Command
{
    about,
    eject,
    help,
    insert,
    inspect,
    list,
    on,
    off,
    pause,
    reset,
    run,
    set
};

class Interpreter
{
    // Reference to other components
    class Application &app;
    class Controller &controller;
    
    // Descriptor lookup table for all supported instructions
    std::vector<CommandDescriptor> descriptors;

    // std::vector<CmdDescriptor> cmdDescriptors;
    CmdDescriptor root = CmdDescriptor("", "<component>", "<command> [<arguments>]",
                                       "", nullptr);
    
    //
    // Initializing
    //

public:
    
    Interpreter(Application &ref);

    
    //
    // Managing the instruction descriptor list
    //
    
    void registerInstr(const std::string &token1, const std::string &token2,
                       const std::string &args, const std::string &help,
                       void (Controller::*func)(Arguments&));
    
    
    //
    // Parsing input
    //
    
    // [[deprecated]] bool matches(const std::string& s1, std::string const& s2);
    // [[deprecated]] bool matches(const std::list<string>& argv, const std::string& s);
    string lowercased(const std::string& s);
    // [[deprecated]] string pop(Arguments& argv);
    
    
    //
    // Printing output
    //
    
    void print(const string& s);
    void println(const string& s);

    
    //
    // Executing commands
    //
    
    void exec(const std::string& userInput);
    
    void help();
    void help(const string& component);
    
    
    template <Component cmp> void exec(Arguments& argv);
    template <Component cmp> void exec(Arguments& argv, int n);
};
