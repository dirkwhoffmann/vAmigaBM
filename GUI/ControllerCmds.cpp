// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

//
// Top-level commands
//

template <> void
Controller::exec <Token::clear> (Arguments &argv, long param)
{
    app.console.clear();
}

template <> void
Controller::exec <Token::hide> (Arguments &argv, long param)
{
    app.console.close();
}

template <> void
Controller::exec <Token::easteregg> (Arguments& argv, long param)
{
    console << "GREETINGS PROFESSOR HOFFMANN." << '\n' << '\n';
    console << "THE ONLY WINNING MOVE IS NOT TO PLAY." << '\n' << '\n';
    console << "HOW ABOUT A NICE GAME OF CHESS?" << '\n';
}


//
// Agnus
//

template <> void
Controller::exec <Token::agnus, Token::config> (Arguments &argv, long param)
{
    dump(amiga.agnus, Dump::Config);
}

template <> void
Controller::exec <Token::agnus, Token::dump, Token::registers> (Arguments &argv, long param)
{
    dump(amiga.agnus, Dump::Registers);
}

template <> void
Controller::exec <Token::agnus, Token::dump, Token::events> (Arguments &argv, long param)
{
    dump(amiga.agnus, Dump::Events);
}

template <> void
Controller::exec <Token::agnus, Token::set, Token::revision> (Arguments &argv, long param)
{
    amiga.configure(OPT_AGNUS_REVISION, AgnusRevisionEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::agnus, Token::set, Token::slowRamMirror> (Arguments &argv, long param)
{
    amiga.configure(OPT_SLOW_RAM_MIRROR, parseBool(argv.front()));
}


//
// Amiga
//

template <> void
Controller::exec <Token::amiga, Token::help> (Arguments &argv, long param)
{
    printf("amiga help\n");
}

template <> void
Controller::exec <Token::amiga, Token::on> (Arguments &argv, long param)
{
    printf("amiga on\n");
    amiga.powerOn();
}

template <> void
Controller::exec <Token::amiga, Token::off> (Arguments &argv, long param)
{
    printf("amiga off\n");
    amiga.powerOff();
}

template <> void
Controller::exec <Token::amiga, Token::run> (Arguments &argv, long param)
{
    printf("amiga run\n");
    amiga.run();
}

template <> void
Controller::exec <Token::amiga, Token::pause> (Arguments &argv, long param)
{
    printf("amiga pause\n");
    amiga.pause();
}

template <> void
Controller::exec <Token::amiga, Token::reset> (Arguments &argv, long param)
{
    printf("amiga reset\n");
    amiga.reset(true);
}

template <> void
Controller::exec <Token::amiga, Token::dump> (Arguments &argv, long param)
{
    std::stringstream ss; string line;

    amiga.dump(ss);
    while(std::getline(ss, line)) console << line << '\n';
}


//
// Copper
//

template <> void
Controller::exec <Token::copper, Token::dump> (Arguments& argv, long param)
{
    dump(amiga.agnus.copper, Dump::State);
}


//
// CPU
//

template <> void
Controller::exec <Token::cpu, Token::dump> (Arguments& argv, long param)
{
    dump(amiga.cpu, Dump::State);
}


//
// Denise
//

template <> void
Controller::exec <Token::denise, Token::config> (Arguments& argv, long param)
{
    dump(amiga.denise, Dump::Config);
}

template <> void
Controller::exec <Token::denise, Token::dump, Token::registers> (Arguments& argv, long param)
{
    dump(amiga.denise, Dump::Registers);
}


//
// Df0, Df1, Df2, Df3
//

template <> void
Controller::exec <Token::dfn, Token::eject> (Arguments& argv, long param)
{
    printf("Df%ld::eject\n", param);
    amiga.df[param]->ejectDisk();
}

template <> void
Controller::exec <Token::dfn, Token::dump> (Arguments& argv, long param)
{
    dump(*amiga.df[param], Dump::State);
}

template <> void
Controller::exec <Token::dfn, Token::insert> (Arguments& argv, long param)
{
    string path = argv.front();
    
    try {
        ADFFile *adf = AmigaFile::make <ADFFile> (path.c_str());
        Disk *disk = Disk::makeWithFile(adf);
        amiga.df[param]->insertDisk(disk);
        
    } catch (VAError &err) {
        console << "Failed to insert disk: " << err.what() << '\n';
    }
}


//
// Memory
//

template <> void
Controller::exec <Token::memory, Token::dump> (Arguments& argv, long param)
{
    dump(amiga.mem, Dump::State);
}
