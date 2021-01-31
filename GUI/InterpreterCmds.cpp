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
             &Controller::exec <Token::agnus, Token::inspect>, 0);
    
    root.add("agnus", "set", "<key>", "<value>",
             "Configure the component");
    
    root.add("agnus", "set", "" ,"", "",
             "Display the current configuration",
             &Controller::exec <Token::agnus, Token::set>, 0);
    
    root.add("agnus", "set", "revision" ,"<revision>", "",
             "Select the emulated chip model",
             &Controller::exec <Token::agnus, Token::set, Token::revision>, 0);
    
    //
    // Amiga
    //
    
    root.add("amiga", "<command>", "[<arguments>]",
             "The virtual Amiga");
    
    root.add("amiga", "inspect", "", "",
             "Display the internal state",
             &Controller::exec <Token::amiga, Token::inspect>, 0);
    
    root.add("amiga", "off", "", "",
             "Switch the Amiga off",
             &Controller::exec <Token::amiga, Token::off>, 0);
    
    root.add("amiga", "on", "", "",
             "Switch the Amiga on",
             &Controller::exec <Token::amiga, Token::on>, 0);
    
    root.add("amiga", "pause", "", "",
             "Halt the emulator thread",
             &Controller::exec <Token::amiga, Token::pause>, 0);
    
    root.add("amiga", "reset", "", "",
             "Perform a hard reset",
             &Controller::exec <Token::amiga, Token::reset>, 0);
    
    root.add("amiga", "run", "", "",
             "Start the emulator thread",
             &Controller::exec <Token::amiga, Token::run>, 0);
    
    //
    // CPU
    //
    
    root.add("cpu", "<command>", "[<arguments>]",
             "Motorola 68k CPU");
    
    root.add("cpu", "inspect", "", "",
             "Display the internal state",
             &Controller::exec <Token::cpu, Token::inspect>, 0);
    
    //
    // Df0, Df1, Df2, Df3
    //
    
    root.add("dfn", "<command>", "[<arguments>]",
             "Floppy drive");
    
    root.add("dfn", "eject", "", "",
             "Eject floppy disk",
             &Controller::exec <Token::dfn, Token::eject>, 0);

    root.add("dfn", "insert", "<path>", "",
             "Insert floppy disk",
             &Controller::exec <Token::dfn, Token::insert>, 0);

    root.add("dfn", "inspect", "", "",
             "Display the internal state",
             &Controller::exec <Token::dfn, Token::inspect>, 0);
}
