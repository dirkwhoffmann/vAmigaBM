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
    //
    // Commands
    //
    
    root.add("clear",
             "command", "Clears the console window",
             &Controller::exec <Token::clear>, 0, 0);

    root.add("hide",
             "command", "Hides the debug console",
             &Controller::exec <Token::hide>, 0, 0);

    root.add("joshua",
             "command", "",
             &Controller::exec <Token::easteregg>, 0, 0);
    root.seek("joshua")->hidden = true;
    
    //
    // Agnus
    //

    root.add("agnus",
             "component", "Custom chip");

    root.add("agnus", "dump",
             "command", "Displays the internal state");

    root.add("agnus", "dump", "registers",
             "category", "Displays register values",
             &Controller::exec <Token::agnus, Token::dump, Token::registers>, 0, 0);

    root.add("agnus", "dump", "events",
             "category", "Displays scheduled events",
             &Controller::exec <Token::agnus, Token::dump, Token::events>, 0, 0);

    root.add("agnus", "set",
             "command", "Configures the component");
    
    root.add("agnus", "set", "",
             "key", "Displays the current configuration",
             &Controller::exec <Token::agnus, Token::set>, 0, 0);
    
    root.add("agnus", "set", "revision" ,
             "key", "Selects the emulated chip model",
             &Controller::exec <Token::agnus, Token::set, Token::revision>, 1, 0);
    
    //
    // Amiga
    //
    
    root.add("amiga",
             "component", "The virtual Amiga");
    
    root.add("amiga", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::amiga, Token::dump>, 0, 0);
    
    root.add("amiga", "power",
             "command", "Switches the Amiga on or off");
    
    root.add("amiga", "power", "on",
             "state", "Switches the Amiga on",
             &Controller::exec <Token::amiga, Token::on>, 0, 0);

    root.add("amiga", "power", "off",
             "state", "Switches the Amiga off",
             &Controller::exec <Token::amiga, Token::off>, 0, 0);

    root.add("amiga", "pause",
             "command", "Halts the emulator thread",
             &Controller::exec <Token::amiga, Token::pause>, 0, 0);
    
    root.add("amiga", "reset",
             "command", "Performs a hard reset",
             &Controller::exec <Token::amiga, Token::reset>, 0, 0);
    
    root.add("amiga", "run",
             "command", "Starts the emulator thread",
             &Controller::exec <Token::amiga, Token::run>, 0, 0);

    //
    // Copper
    //
    
    root.add("copper",
             "component", "Custom Chip (Agnus)");
    
    root.add("copper", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::copper, Token::dump>, 0, 0);

    
    //
    // CPU
    //
    
    root.add("cpu",
             "component", "Motorola 68k CPU");
    
    root.add("cpu", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::cpu, Token::dump>, 0, 0);

    
    //
    // Denise
    //
    
    root.add("denise",
             "component", "Custom chip");
    
    root.add("denise", "dump",
             "command", "Displays the internal state");

    root.add("denise", "dump", "registers",
             "category", "Displays the register contents",
             &Controller::exec <Token::denise, Token::dump, Token::registers>, 0, 0);

    root.add("denise", "set",
             "command", "Configures the component");

    root.add("denise", "set", "",
             "key", "Displays the current configuration",
             &Controller::exec <Token::denise, Token::set>, 0, 0);

    
    //
    // Df0, Df1, Df2, Df3
    //
    
    root.add("dfn",
             "component", "Floppy drive");
    
    root.add("dfn", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::dfn, Token::dump>, 0, 0);

    root.add("dfn", "eject",
             "command", "Ejects a floppy disk",
             &Controller::exec <Token::dfn, Token::eject>, 0, 0);

    root.add("dfn", "insert",
             "command", "Inserts a floppy disk",
             &Controller::exec <Token::dfn, Token::insert>, 1, 0);

    //
    // Memory
    //
    
    root.add("memory",
             "component", "Ram and Rom");
    
    root.add("memory", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::memory, Token::dump>, 0, 0);
}
