// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "Amiga.h"
#include "Interpreter.h"

class Controller : public GUIComponent
{
    
public:
    
    // Available warp modes
    enum class WarpActivation { never, always, automatic };
    
    // The currently selected warp activation mode
    WarpActivation warpActivation = WarpActivation::automatic;
    
    
    //
    // Initializing
    //
    
public:
    
    Controller(Application &ref);
    ~Controller();

    // Delegation methods
    void init();
    void awake();

    void deinit();

    
    //
    // Processing messages
    //
    
    void processMessage(long id, long data);
    
    
    //
    // Managing warp mode
    //
    
    // Updates the warp status according to the current emulator state
    void updateWarp();
    
    // Flips through the available warp modes
    void flipWarpMode();
    
    
    //
    // Executing commands
    //
    
    void exec(const string& command) throws;
    
    template <Token t1> void exec(Arguments& argv, long param) throws;
    template <Token t1, Token t2> void exec(Arguments& argv, long param) throws;
    template <Token t1, Token t2, Token t3> void exec(Arguments& argv, long param) throws;

private:
    
    bool parseBool(string& token);
    long parseNum(isize base, string& token);
    long parseDec(string& token) { return parseNum(10, token); }
    long parseHex(string& token) { return parseNum(16, token); }

    void dump(HardwareComponent &component, Dump::Category category);
};
