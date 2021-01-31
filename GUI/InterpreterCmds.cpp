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
    
    init1("agnus", "<command>", "[<arguments>]",
         "Amiga custom chip");
    
    init2("agnus", "inspect", "", "",
         "Display the internal state",
         &Controller::exec <Token::agnus, Token::inspect>);
    
    init2("agnus", "set", "<key>", "<value>",
         "Configure the component");
    
    init3("agnus", "set", "" ,"", "",
         "Display the current configuration",
         &Controller::exec <Token::agnus, Token::set>);

    init3("agnus", "set", "revision" ,"<revision>", "",
         "Select the emulated chip model",
         &Controller::exec <Token::agnus, Token::set, Token::revision>);
    
    //
    // Amiga
    //
    
    init1("amiga", "<command>", "[<arguments>]",
         "The virtual Amiga");
    
    init2("amiga", "inspect", "", "",
         "Display the internal state",
         &Controller::exec <Token::amiga, Token::inspect>);
    
    init2("amiga", "off", "", "",
         "Switch the Amiga off",
         &Controller::exec <Token::amiga, Token::off>);
    
    init2("amiga", "on", "", "",
         "Switch the Amiga on",
         &Controller::exec <Token::amiga, Token::on>);
    
    init2("amiga", "pause", "", "",
         "Halt the emulator thread",
         &Controller::exec <Token::amiga, Token::pause>);

    init2("amiga", "reset", "", "",
         "Perform a hard reset",
         &Controller::exec <Token::amiga, Token::reset>);

    init2("amiga", "run", "", "",
         "Start the emulator thread",
         &Controller::exec <Token::amiga, Token::run>);
    
    //
    // CPU
    //

    init1("cpu", "<command>", "[<arguments>]",
         "Motorola 68k CPU");
    
    init2("cpu", "inspect", "", "",
         "Display the internal state",
         &Controller::exec <Token::cpu, Token::inspect>);
    
    //
    // Dfn
    //
    
    init1("df0", "<command>", "[<arguments>]",
         "Internal floppy drive");
    
    init2("df0", "inspect", "", "",
         "Display the internal state",
         &Controller::exec <Token::dfn, Token::inspect>, 0);

    init1("df1", "<command>", "[<arguments>]",
         "First external floppy drive");

    init2("df1", "inspect", "", "",
         "Display the internal state",
         &Controller::exec <Token::dfn, Token::inspect>, 1);

    init1("df2", "<command>", "[<arguments>]",
         "Second external floppy drive");
    
    init2("df2", "inspect", "", "",
         "Display the internal state",
         &Controller::exec <Token::dfn, Token::inspect>, 2);

    init1("df3", "<command>", "[<arguments>]",
         "Third external floppy drive");

    init2("df3", "inspect", "", "",
         "Display the internal state",
         &Controller::exec <Token::dfn, Token::inspect>, 3);

 
    
}
