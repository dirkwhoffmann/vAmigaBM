/// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Command.h"

enum class Token
{
    none,
    
    // Components
    agnus, amiga, blitter, cia, copper, cpu, denise, dfn, diskcontroller,
    memory, paula, serial, rtc,

    // Commands
    about, autosync, clear, config, connect, disconnect, dsksync, easteregg,
    eject, help, hide, insert, dump, list, load, lock, on, off, pause,
    registers, reset, run, set,
    
    // Keys
    bankmap, borderblank, checksum, chip, device, esync, events, extrom,
    extstart, fast, raminitpattern, revision, rom, slow, slowramdelay,
    slowrammirror, speed, state, tod, todbug, unmappingtype, wom
};

struct ParseError : public std::exception {
    std::string description;
    ParseError() : description("") { }
    ParseError(const std::string &s) : description(s) { }
    const char *what() const throw() override { return description.c_str(); }
};

struct ParseBoolError : ParseError {
    const char *what() const throw() override { return "true, false"; }
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

class Interpreter
{
    // Reference to other components
    class Application &app;
    class Controller &controller;
    
    // The registered instruction set
    Command root;
    
    
    //
    // Initializing
    //

public:
    
    Interpreter(Application &ref);

private:
    
    // Registers the instruction set
    void registerInstructions();
    
    
    //
    // Parsing input
    //
    
private:
        
    
    //
    // Executing commands
    //
    
public:
    
    // Executes a command
    void exec(const std::string& userInput);
    bool exec(Arguments &argv);
        
    // Prints a syntax summary
    void usage(Command& command, const string& prefix);
    void syntax(Command& command, const string& prefix);
    void syntax() { syntax(root, ""); }
};
