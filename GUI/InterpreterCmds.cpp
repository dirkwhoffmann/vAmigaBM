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
    // Amiga
    //
    
    root.add("amiga",
             "component", "The virtual Amiga");
        
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

    root.add("amiga", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::amiga, Token::dump>, 0, 0);

    
    //
    // Memory
    //
    
    root.add("memory",
             "component", "Ram and Rom");
    
    root.add("memory", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::memory, Token::config>, 0, 0);

    root.add("memory", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::memory, Token::dump>, 0, 0);
    
    
    //
    // CPU
    //
    
    root.add("cpu",
             "component", "Motorola 68k CPU");
    
    root.add("cpu", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::cpu, Token::dump>, 0, 0);

    
    //
    // CIA
    //
    
    root.add("cia",
             "component", "Complex Interface Adapters");
    
    root.add("cia", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::cia, Token::config>, 0, 0);

    root.add("cia", "set",
             "command", "Configures the component");
        
    root.add("cia", "set", "revision" ,
             "key", "Selects the emulated chip model",
             &Controller::exec <Token::cia, Token::set, Token::revision>, 1, 0);

    root.add("cia", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::cia, Token::dump>, 0, 0);

    
    //
    // Agnus
    //

    root.add("agnus",
             "component", "Custom chip");

    root.add("agnus", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::agnus, Token::config>, 0, 0);
    
    root.add("agnus", "set",
             "command", "Configures the component");
        
    root.add("agnus", "set", "revision" ,
             "key", "Selects the emulated chip model",
             &Controller::exec <Token::agnus, Token::set, Token::revision>, 1, 0);

    root.add("agnus", "set", "slowrammirror",
             "key", "Enables or disables ECS Slow Ram mirroring",
             &Controller::exec <Token::agnus, Token::set, Token::slowrammirror>, 1, 0);

    root.add("agnus", "dump",
             "command", "Displays the internal state");

    root.add("agnus", "dump", "registers",
             "category", "Displays register values",
             &Controller::exec <Token::agnus, Token::dump, Token::registers>, 0, 0);

    root.add("agnus", "dump", "events",
             "category", "Displays scheduled events",
             &Controller::exec <Token::agnus, Token::dump, Token::events>, 0, 0);
    
    
    //
    // Blitter
    //
    
    root.add("blitter",
             "component", "Custom Chip (Agnus)");
    
    root.add("blitter", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::blitter, Token::dump>, 0, 0);

    
    //
    // Copper
    //
    
    root.add("copper",
             "component", "Custom Chip (Agnus)");
    
    root.add("copper", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::copper, Token::dump>, 0, 0);

    
    //
    // Denise
    //
    
    root.add("denise",
             "component", "Custom chip");
    
    root.add("denise", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::denise, Token::config>, 0, 0);

    root.add("denise", "set",
             "command", "Configures the component");

    root.add("denise", "set", "revision" ,
             "key", "Selects the emulated chip model",
             &Controller::exec <Token::denise, Token::set, Token::revision>, 1, 0);

    root.add("denise", "set", "borderblnk" ,
             "key", "Enables the ECS borderblank feature",
             &Controller::exec <Token::denise, Token::set, Token::borderblank>, 1, 0);

    root.add("denise", "dump",
             "command", "Displays the internal state");

    root.add("denise", "dump", "registers",
             "category", "Displays the register contents",
             &Controller::exec <Token::denise, Token::dump, Token::registers>, 0, 0);

    
    //
    // Paula
    //
    
    root.add("paula",
             "component", "Custom chip");

    root.add("paula", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::paula, Token::config>, 0, 0);

    root.add("paula", "dump",
             "command", "Displays the internal state");

    root.add("paula", "dump", "registers",
             "command", "Displays the register contents",
             &Controller::exec <Token::paula, Token::dump, Token::registers>, 0, 0);

    
    //
    // RTC
    //

    root.add("rtc",
             "component", "Real-time clock");

    root.add("rtc", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::rtc, Token::config>, 0, 0);

    root.add("rtc", "set",
             "command", "Configures the component");
        
    root.add("rtc", "set", "revision" ,
             "key", "Selects the emulated chip model",
             &Controller::exec <Token::rtc, Token::set, Token::revision>, 1, 0);

    root.add("rtc", "dump",
             "command", "Displays the internal state");

    root.add("rtc", "dump", "registers",
             "command", "Displays the register contents",
             &Controller::exec <Token::rtc, Token::dump, Token::registers>, 0, 0);

    
    //
    // Serial port
    //
    
    root.add("serial",
             "component", "Serial port");

    root.add("serial", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::serial, Token::config>, 0, 0);

    root.add("serial", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::serial, Token::dump>, 0, 0);

    
    //
    // Disk controller
    //
    
    root.add("dc",
             "component", "Disk Controller");

    root.add("dc", "set",
             "command", "Configures the component");
        
    root.add("dc", "set", "revision" ,
             "key", "Configures the drive speed",
             &Controller::exec <Token::diskcontroller, Token::speed>, 1, 0);

    root.add("dc", "disksync",
             "command", "Controls modifications of the DSKSYNC register");

    root.add("dc", "disksync", "auto" ,
             "key", "Always receive a SYNC signal",
             &Controller::exec <Token::diskcontroller, Token::dsksync, Token::autosync>, 1, 0);

    root.add("dc", "disksync", "lock",
             "command", "Prevents writes to DSKSYNC",
             &Controller::exec <Token::diskcontroller, Token::dsksync, Token::lock>, 1, 0);
        
    root.add("dc", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::diskcontroller, Token::dump>, 0, 0);


    //
    // Df0, Df1, Df2, Df3
    //
    
    root.add("dfn",
             "component", "Floppy drive");

    root.add("dfn", "connect",
             "command", "Connects the drive",
             &Controller::exec <Token::dfn, Token::connect>, 0, 0);
    root.seek("df0")->remove("connect");

    root.add("dfn", "disconnect",
             "command", "Disconnects the drive",
             &Controller::exec <Token::dfn, Token::disconnect>, 0, 0);
    root.seek("df0")->remove("disconnect");

    root.add("dfn", "eject",
             "command", "Ejects a floppy disk",
             &Controller::exec <Token::dfn, Token::eject>, 0, 0);

    root.add("dfn", "insert",
             "command", "Inserts a floppy disk",
             &Controller::exec <Token::dfn, Token::insert>, 1, 0);

    root.add("dfn", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::dfn, Token::dump>, 0, 0);
}
