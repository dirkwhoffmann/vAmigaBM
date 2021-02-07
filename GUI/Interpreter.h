// -----------------------------------------------------------------------------
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
    agnus, amiga, audio, blitter, cia, copper, cpu, denise, dfn, dc, keyboard,
    memory, paula, serial, rtc,

    // Commands
    about, autosync, clear, config, connect, disconnect, dsksync, easteregg,
    eject, close, insert, inspect, list, load, lock, on, off, pause, reset,
    run, set, source,
    
    // Categories
    state, registers, events, checksums,
    
    // Keys
    accuracy, bankmap, borderblank, chip, device, esync, extrom, extstart,
    fast, filter, mechanics, model, pan, raminitpattern, revision, rom,
    sampling, slow, slowramdelay, slowrammirror, speed, tod, todbug,
    unmappingtype, volume, wom
};

struct ParseError : public std::exception {
    
    std::string description;
    
    ParseError(const std::string &s) : description(s) { }
    const char *what() const throw() override { return description.c_str(); }
};

struct ParseBoolError : ParseError {
    ParseBoolError() : ParseError("true, false") { }
};

struct SyntaxError : public ParseError {
    SyntaxError() : ParseError("Unknown command") { }
};

struct TooFewArgumentsError : public ParseError {
    TooFewArgumentsError() : ParseError("") { }
};

struct TooManyArgumentsError : public ParseError {
    TooManyArgumentsError() : ParseError("") { }
};

class Interpreter
{
    // Reference to other components
    class Application &app;
    class Controller &controller;
    class Console &console;
    
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
    
public:
    
    // Splits an input string into an argument list
    Arguments split(const string& userInput);

    // Auto-completes a command. Returns the number of auto-completed tokens
    isize autoComplete(Arguments &argv);
    isize autoComplete(string& userInput);

    
    //
    // Executing commands
    //
    
public:
    
    // Executes a script file
    bool exec(std::istream &stream);

    // Executes a single command
    bool exec(const string& userInput, bool verbose = false);
    bool exec(Arguments &argv, bool verbose = false);
            
    // Prints a usage string for a command
    void usage(Command &command);
    
    // Displays a help text for a (partially typed in) command
    void help(const string &userInput);
    void help(Arguments &argv);
    void help(Command &command);
};
