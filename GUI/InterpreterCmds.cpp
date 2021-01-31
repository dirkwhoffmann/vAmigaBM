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
    root.add("agnus", "<command>", "[<arguments>]",
             "Amiga custom chip");
    
    root.add("agnus", "inspect", "", "",
         "Display the internal state",
         0, &Controller::exec <Token::agnus, Token::inspect>);
    
    root.add("agnus", "set", "<key>", "<value>",
         "Configure the component");
    
    root.add("agnus", "set", "" ,"", "",
         "Display the current configuration",
         0, &Controller::exec <Token::agnus, Token::set>);

    root.add("agnus", "set", "revision" ,"<revision>", "",
         "Select the emulated chip model",
         0, &Controller::exec <Token::agnus, Token::set, Token::revision>);
    
    //
    // Amiga
    //
    
    root.add("amiga", "<command>", "[<arguments>]",
         "The virtual Amiga");
    
    root.add("amiga", "inspect", "", "",
         "Display the internal state",
         0, &Controller::exec <Token::amiga, Token::inspect>);
    
    root.add("amiga", "off", "", "",
         "Switch the Amiga off",
         0, &Controller::exec <Token::amiga, Token::off>);
    
    root.add("amiga", "on", "", "",
         "Switch the Amiga on",
         0, &Controller::exec <Token::amiga, Token::on>);
    
    root.add("amiga", "pause", "", "",
         "Halt the emulator thread",
         0, &Controller::exec <Token::amiga, Token::pause>);

    root.add("amiga", "reset", "", "",
         "Perform a hard reset",
         0, &Controller::exec <Token::amiga, Token::reset>);

    root.add("amiga", "run", "", "",
         "Start the emulator thread",
         0, &Controller::exec <Token::amiga, Token::run>);
    
    //
    // CPU
    //

    root.add("cpu", "<command>", "[<arguments>]",
         "Motorola 68k CPU");
    
    root.add("cpu", "inspect", "", "",
         "Display the internal state",
         0, &Controller::exec <Token::cpu, Token::inspect>);
    
    //
    // Df0, Df1, Df2, Df3
    //
    
    root.add("dfn", "<command>", "[<arguments>]",
         "Floppy drive");
    
    root.add("dfn", "inspect", "", "",
         "Display the internal state",
         0, &Controller::exec <Token::dfn, Token::inspect>);
}
