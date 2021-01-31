// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

void
Interpreter::registerInstructions()
{
    root.arg1 = "<component>";
    root.arg2 = "<command> [<arguments>]";
    
    //
    // Agnus
    //
    
    init("agnus", "<command>", "[<arguments>]",
         "Amiga custom chip");
    
    init("agnus", "inspect", "", "",
         "Display the internal state",
         &Controller::exec <Token::agnus, Token::inspect>);
    
    init("agnus", "set", "<key>", "<value>",
         "Configure the component");
    
    init("agnus", "set", "" ,"", "",
         "Display the current configuration",
         &Controller::exec <Token::agnus, Token::set>);

    init("agnus", "set", "revision" ,"<revision>", "",
         "Select the emulated chip model",
         &Controller::exec <Token::agnus, Token::set, Token::revision>);
    
    //
    // Amiga
    //
    
    init("amiga", "<command>", "[<arguments>]",
         "The virtual Amiga");
    
    init("amiga", "inspect", "", "",
         "Display the internal state",
         &Controller::exec <Token::amiga, Token::inspect>);
    
    init("amiga", "off", "", "",
         "Switch the Amiga off",
         &Controller::exec <Token::amiga, Token::off>);
    
    init("amiga", "on", "", "",
         "Switch the Amiga on",
         &Controller::exec <Token::amiga, Token::on>);
    
    init("amiga", "pause", "", "",
         "Halt the emulator thread",
         &Controller::exec <Token::amiga, Token::pause>);

    init("amiga", "reset", "", "",
         "Perform a hard reset",
         &Controller::exec <Token::amiga, Token::reset>);

    init("amiga", "run", "", "",
         "Start the emulator thread",
         &Controller::exec <Token::amiga, Token::run>);
    
    //
    // CPU
    //

    init("cpu", "<command>", "[<arguments>]",
         "Motorola 68k CPU");
    
    init("cpu", "inspect", "", "",
         "Display the internal state",
         &Controller::exec <Token::cpu, Token::inspect>);
}
