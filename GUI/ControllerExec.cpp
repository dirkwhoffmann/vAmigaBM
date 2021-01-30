// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

template <> void
Controller::exec <Command::help> (Arguments& argv)
{
    console.println("Syntax: <component> <command> [ <arguments> ]");
    console.println("        <component> ::= amiga | agnus | ");
    console.println("                        cia | ");
    console.println("                        df0 | df1 | df2 | df3 | ");
    console.println("                        rtc");
    console.println("");
    console.println("Type '<component> help' for more details.");
}

template <> void
Controller::exec <Component::amiga, Command::help> (Arguments& argv)
{
    console.println("Syntax: amiga <command>");
    console.println("              <command> ::= power | run | pause | reset");
    console.println();
    console.println("power [on | off] : Turns the virtual Amiga on or off");
    console.println("run              : Starts the emulator");
    console.println("pause            : Pauses the emulator");
    console.println("reset            : Performs a hard reset");
}

template <> void
Controller::exec <Component::agnus, Command::help> (Arguments& argv)
{
    console.println("Syntax: agnus <command>");
    console.println("              <command> ::= config | inspect | set");
    console.println();
    console.println("config            : Prints the current configuration");
    console.println("inspect           : Informs about the internal state");
    console.println("set <key> <value> : Configures the component");
}

template <> void
Controller::exec <Component::amiga, Command::power> (Arguments &argv)
{
    if (argv.empty()) throw TooFewArgumentsError();

    string &cmd = argv.front();
    argv.pop_front();
    
    if (cmd == "on") {
        console.println("PowerOn");
        app.controller.amiga.powerOn();
        return;
    }
    if (cmd == "off") {
        console.println("PowerOff");
        app.controller.amiga.powerOn();
        return;
    }
    
    throw UnknownCommandError(cmd);
}

template <> void
Controller::exec <Component::amiga, Command::run> (Arguments &argv)
{
    if (!argv.empty()) throw TooManyArgumentsError();
    
    app.controller.amiga.run();
}

template <> void
Controller::exec <Component::amiga, Command::pause> (Arguments &argv)
{
    if (!argv.empty()) throw TooManyArgumentsError();
    
    app.controller.amiga.pause();
}

template <> void
Controller::exec <Component::amiga, Command::reset> (Arguments &argv)
{
    if (!argv.empty()) throw TooManyArgumentsError();
    
    app.controller.amiga.reset(true);
}

    
/*
void
Controller::execDfnInsert(Arguments &argv, int n)
{
    if (!argv.empty()) {
        ADFFile *file = AmigaFile::make<ADFFile>(argv.front().c_str());
        if (file == nullptr) {
            console.println("Can't find ADF\n");
            return;
        }
        Disk *disk = Disk::makeWithFile(file);
        if (disk == nullptr) {
            console.println("Can't create disk\n");
            return;
        }
        app.controller.amiga.df[n]->insertDisk(disk);
    }
}

void
Controller::execRtc(Arguments &argv)
{
    app.console.println("RTC");
    app.controller.amiga.configure(OPT_RTC_MODEL, 99);
}
*/
