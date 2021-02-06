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
    root.seek("clear")->hidden = true;

    root.add("close",
             "command", "Hides the debug console",
             &Controller::exec <Token::close>, 0, 0);
    root.seek("close")->hidden = true;

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

    root.add("amiga", "run",
             "command", "Starts the emulator thread",
             &Controller::exec <Token::amiga, Token::run>, 0, 0);

    root.add("amiga", "pause",
             "command", "Halts the emulator thread",
             &Controller::exec <Token::amiga, Token::pause>, 0, 0);
    
    root.add("amiga", "reset",
             "command", "Performs a hard reset",
             &Controller::exec <Token::amiga, Token::reset>, 0, 0);
    
    root.add("amiga", "dump",
             "command", "Displays the component state",
             &Controller::exec <Token::amiga, Token::dump>, 0, 0);

    
    //
    // Memory
    //
    
    root.add("memory",
             "component", "Ram and Rom");
    
    root.add("memory", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::memory, Token::config>, 0, 0);

    root.add("memory", "set",
             "command", "Configures the component");
        
    root.add("memory", "set", "chip" ,
             "key", "Configures the amouts of chip memory",
             &Controller::exec <Token::memory, Token::set, Token::chip>, 1, 0);

    root.add("memory", "set", "slow" ,
             "key", "Configures the amouts of slow memory",
             &Controller::exec <Token::memory, Token::set, Token::slow>, 1, 0);

    root.add("memory", "set", "fast" ,
             "key", "Configures the amouts of flow memory",
             &Controller::exec <Token::memory, Token::set, Token::fast>, 1, 0);

    root.add("memory", "set", "extstart" ,
             "key", "Sets the start address for Rom extensions",
             &Controller::exec <Token::memory, Token::set, Token::extstart>, 1, 0);

    root.add("memory", "set", "slowramdelay" ,
             "key", "Enables or disables slow Ram bus delays",
             &Controller::exec <Token::memory, Token::set, Token::slowramdelay>, 1, 0);

    root.add("memory", "set", "bankmap" ,
             "key", "Selects the bank mapping scheme",
             &Controller::exec <Token::memory, Token::set, Token::bankmap>, 1, 0);

    root.add("memory", "set", "unmapped" ,
             "key", "Determines the behaviour of unmapped memory",
             &Controller::exec <Token::memory, Token::set, Token::unmappingtype>, 1, 0);

    root.add("memory", "set", "raminit" ,
             "key", "Determines how Ram is initialized on startup",
             &Controller::exec <Token::memory, Token::set, Token::raminitpattern>, 1, 0);
    
    root.add("memory", "load",
             "command", "Installs a Rom image");
            
    root.add("memory", "load", "rom",
             "command", "Installs a Kickstart Rom",
             &Controller::exec <Token::memory, Token::load, Token::rom>, 1, 0);

    root.add("memory", "load", "extrom",
             "command", "Installs a Rom extension",
             &Controller::exec <Token::memory, Token::load, Token::extrom>, 1, 0);

    root.add("memory", "dump",
             "command", "Displays the component state");

    root.add("memory", "dump", "state",
             "command", "Displays the current state",
             &Controller::exec <Token::memory, Token::dump, Token::state>, 0, 0);

    root.add("memory", "dump", "bankmap",
             "command", "Displays the bank map",
             &Controller::exec <Token::memory, Token::dump, Token::bankmap>, 0, 0);

    root.add("memory", "dump", "checksum",
             "command", "Computes memory checksums",
             &Controller::exec <Token::memory, Token::dump, Token::checksums>, 0, 0);

    
    //
    // CPU
    //
    
    root.add("cpu",
             "component", "Motorola 68k CPU");
    
    root.add("cpu", "dump",
             "command", "Displays the component state");

    root.add("cpu", "dump", "state",
             "command", "Displays the current state",
             &Controller::exec <Token::cpu, Token::dump, Token::state>, 0, 0);

    root.add("cpu", "dump", "registers",
             "command", "Displays the current register values",
             &Controller::exec <Token::cpu, Token::dump, Token::registers>, 0, 0);

    
    //
    // CIA
    //
    
    root.add("ciaa",
             "component", "Complex Interface Adapter A");

    root.add("ciab",
             "component", "Complex Interface Adapter B");

    root.add("cia", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::cia, Token::config>, 0, 0);

    root.add("cia", "set",
             "command", "Configures the component");
        
    root.add("cia", "set", "revision" ,
             "key", "Selects the emulated chip model",
             &Controller::exec <Token::cia, Token::set, Token::revision>, 1, 0);

    root.add("cia", "set", "todbug" ,
             "key", "Enables or disables the TOD hardware bug",
             &Controller::exec <Token::cia, Token::set, Token::todbug>, 1, 0);

    root.add("cia", "set", "esync" ,
             "key", "Turns E-clock syncing on or off",
             &Controller::exec <Token::cia, Token::set, Token::esync>, 1, 0);

    root.add("cia", "dump",
             "command", "Displays the component state");

    root.add("cia", "dump", "state",
             "category", "Displays the current state",
             &Controller::exec <Token::cia, Token::dump, Token::state>, 0, 0);

    root.add("cia", "dump", "registers",
             "category", "Displays the current register values",
             &Controller::exec <Token::cia, Token::dump, Token::registers>, 0, 0);

    root.add("cia", "dump", "tod",
             "category", "Displays the state of the 24-bit counter",
             &Controller::exec <Token::cia, Token::dump, Token::tod>, 0, 0);

    
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

    root.add("agnus", "dump", "state",
             "category", "Displays the current state",
             &Controller::exec <Token::agnus, Token::dump, Token::state>, 0, 0);

    root.add("agnus", "dump", "registers",
             "category", "Displays the current register value",
             &Controller::exec <Token::agnus, Token::dump, Token::registers>, 0, 0);

    root.add("agnus", "dump", "events",
             "category", "Displays scheduled events",
             &Controller::exec <Token::agnus, Token::dump, Token::events>, 0, 0);
    
    
    //
    // Blitter
    //
    
    root.add("blitter",
             "component", "Custom Chip (Agnus)");
    
    root.add("blitter", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::blitter, Token::config>, 0, 0);
    
    root.add("blitter", "set",
             "command", "Configures the component");
        
    root.add("blitter", "set", "accuracy",
             "level", "Selects the emulation accuracy level",
             &Controller::exec <Token::blitter, Token::set, Token::accuracy>, 1, 0);

    root.add("blitter", "dump",
             "command", "Displays the internal state");

    root.add("blitter", "dump", "state",
             "category", "Displays the internal state",
             &Controller::exec <Token::blitter, Token::dump, Token::state>, 0, 0);

    root.add("blitter", "dump", "registers",
             "category", "Displays the current register value",
             &Controller::exec <Token::blitter, Token::dump, Token::registers>, 0, 0);

    
    //
    // Copper
    //
    
    root.add("copper",
             "component", "Custom Chip (Agnus)");
    
    root.add("copper", "dump",
             "command", "Displays the internal state");

    root.add("copper", "dump", "state",
             "category", "Displays the current state",
             &Controller::exec <Token::copper, Token::dump, Token::state>, 0, 0);

    root.add("copper", "dump", "registers",
             "category", "Displays the current register value",
             &Controller::exec <Token::copper, Token::dump, Token::registers>, 0, 0);

    
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

    root.add("denise", "dump", "state",
             "category", "Displays the current state",
             &Controller::exec <Token::denise, Token::dump, Token::state>, 0, 0);

    root.add("denise", "dump", "registers",
             "category", "Displays the current register value",
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
             "command", "Displays the current register value",
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
             "command", "Displays the current register value",
             &Controller::exec <Token::rtc, Token::dump, Token::registers>, 0, 0);

    
    //
    // Keyboard
    //

    root.add("keyboard",
             "component", "Keyboard");

    root.add("keyboard", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::keyboard, Token::config>, 0, 0);
    
    root.add("keyboard", "set",
             "command", "Configures the component");
        
    root.add("keyboard", "set", "accuracy",
             "key", "Determines the emulation accuracy level",
             &Controller::exec <Token::keyboard, Token::set, Token::accuracy>, 1, 0);

    root.add("keyboard", "dump",
             "command", "Displays the internal state",
             &Controller::exec <Token::keyboard, Token::dump>, 0, 0);

    
    //
    // Serial port
    //
    
    root.add("serial",
             "component", "Serial port");

    root.add("serial", "config",
             "command", "Displays the current configuration",
             &Controller::exec <Token::serial, Token::config>, 0, 0);

    root.add("serial", "set",
             "command", "Configures the component");
        
    root.add("serial", "set", "device",
             "key", "",
             &Controller::exec <Token::serial, Token::set, Token::device>, 1, 0);

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
