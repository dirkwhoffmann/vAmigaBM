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
#include "Exception.h"
#include "Errors.h"

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

struct ParseBoolError : ParseError {
    using ParseError::ParseError;
    // ParseBoolError(const string &s) : ParseError(s) { }
};

struct SyntaxError : public ParseError {
    using ParseError::ParseError;
    // SyntaxError(const string &s) : ParseError(s) { }
};

struct TooFewArgumentsError : public ParseError {
    using ParseError::ParseError;
    // TooFewArgumentsError(const string &s) : ParseError(s) { }
};

struct TooManyArgumentsError : public ParseError {
    using ParseError::ParseError;
    // TooManyArgumentsError(const string &s) : ParseError(s) { }
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
    // void exec(std::istream &stream) throws;

    // Executes a single command
    void exec(const string& userInput, bool verbose = false) throws;
    void exec(Arguments &argv, bool verbose = false) throws;
            
    // Prints a usage string for a command
    void usage(Command &command);
    
    // Displays a help text for a (partially typed in) command
    void help(const string &userInput);
    void help(Arguments &argv);
    void help(Command &command);
};
