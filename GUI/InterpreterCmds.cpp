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

    root.add("agnus", "dump", "[<category>]", "",
             "Display the internal state");

    root.add("agnus", "dump", "", "", "",
             "Display registers",
             &Controller::exec <Token::agnus, Token::dump>, 0, 0);

    root.add("agnus", "dump", "events", "", "",
             "Display scheduled events",
             &Controller::exec <Token::agnus, Token::dump, Token::events>, 0, 0);

    root.add("agnus", "set", "<key>", "<value>",
             "Configure the component");
    
    root.add("agnus", "set", "" ,"", "",
             "Display the current configuration",
             &Controller::exec <Token::agnus, Token::set>, 0, 0);
    
    root.add("agnus", "set", "revision" ,"<revision>", "",
             "Select the emulated chip model",
             &Controller::exec <Token::agnus, Token::set, Token::revision>, 1, 0);
    
    //
    // Amiga
    //
    
    root.add("amiga", "<command>", "[<arguments>]",
             "The virtual Amiga");
    
    root.add("amiga", "dump", "", "",
             "Display the internal state",
             &Controller::exec <Token::amiga, Token::dump>, 0, 0);
    
    root.add("amiga", "power", "[ on | off ]", "",
             "Switch the Amiga on or off");
    
    root.add("amiga", "power", "on", "", "",
             "Switch the Amiga on",
             &Controller::exec <Token::amiga, Token::on>, 0, 0);

    root.add("amiga", "power", "off", "", "",
             "Switch the Amiga off",
             &Controller::exec <Token::amiga, Token::off>, 0, 0);

    root.add("amiga", "pause", "", "",
             "Halt the emulator thread",
             &Controller::exec <Token::amiga, Token::pause>, 0, 0);
    
    root.add("amiga", "reset", "", "",
             "Perform a hard reset",
             &Controller::exec <Token::amiga, Token::reset>, 0, 0);
    
    root.add("amiga", "run", "", "",
             "Start the emulator thread",
             &Controller::exec <Token::amiga, Token::run>, 0, 0);

    //
    // Copper
    //
    
    root.add("copper", "<command>", "[<arguments>]",
             "Access the Copper");
    
    root.add("copper", "dump", "", "",
             "Display the internal state",
             &Controller::exec <Token::copper, Token::dump>, 0, 0);

    
    //
    // CPU
    //
    
    root.add("cpu", "<command>", "[<arguments>]",
             "Motorola 68k CPU");
    
    root.add("cpu", "dump", "", "",
             "Display the internal state",
             &Controller::exec <Token::cpu, Token::dump>, 0, 0);

    
    //
    // Denise
    //
    
    root.add("denise", "<command>", "[<arguments>]",
             "Graphics");
    
    root.add("denise", "dump", "", "",
             "Displays the internal state");

    root.add("denise", "dump", "config", "", "",
             "Displays the current configuration",
             &Controller::exec <Token::denise, Token::dump, Token::config>, 0, 0);

    root.add("denise", "dump", "registers", "", "",
             "Displays the register contents",
             &Controller::exec <Token::denise, Token::dump, Token::registers>, 0, 0);

    
    //
    // Df0, Df1, Df2, Df3
    //
    
    root.add("dfn", "<command>", "[<arguments>]",
             "Floppy drive");
    
    root.add("dfn", "dump", "", "",
             "Display the internal state",
             &Controller::exec <Token::dfn, Token::dump>, 0, 0);

    root.add("dfn", "eject", "", "",
             "Eject floppy disk",
             &Controller::exec <Token::dfn, Token::eject>, 0, 0);

    root.add("dfn", "insert", "<path>", "",
             "Insert floppy disk",
             &Controller::exec <Token::dfn, Token::insert>, 1, 0);

    //
    // Memory
    //
    
    root.add("memory", "<command>", "[<arguments>]",
             "Ram and Rom");
    
    root.add("memory", "dump", "", "",
             "Display the internal state",
             &Controller::exec <Token::memory, Token::dump>, 0, 0);
}
