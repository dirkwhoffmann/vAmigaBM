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
// Agnus
//

template <> void
Controller::exec <Token::agnus, Token::dump> (Arguments &argv, long param)
{
    std::stringstream ss; string line;

    amiga.agnus.dump(ss);
    while(std::getline(ss, line)) console << line << '\n';
}

template <> void
Controller::exec <Token::agnus, Token::dump, Token::events> (Arguments &argv, long param)
{
    std::stringstream ss; string line;

    amiga.agnus.dumpEvents(ss);
    while(std::getline(ss, line)) console << line << '\n';
}

template <> void
Controller::exec <Token::agnus, Token::set> (Arguments &argv, long param)
{
    printf("agnus set\n");
}

template <> void
Controller::exec <Token::agnus, Token::set, Token::revision> (Arguments &argv, long param)
{
    printf("agnus set revision\n");
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
// CPU
//

template <> void
Controller::exec <Token::cpu, Token::dump> (Arguments& argv, long param)
{
    std::stringstream ss; string line;

    amiga.cpu.dump(ss);
    while(std::getline(ss, line)) console << line << '\n';
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
    std::stringstream ss;
    string line;
    
    amiga.df[param]->dump(ss);
    while(std::getline(ss, line)) console << line << '\n';
}

template <> void
Controller::exec <Token::dfn, Token::insert> (Arguments& argv, long param)
{
    printf("Df%ld::insert\n", param);

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
// Misc
//

template <> void
Controller::exec <Token::easteregg> (Arguments& argv, long param)
{
    console << "GREETINGS PROFESSOR HOFFMANN" << '\n' << '\n';
    console << "THE ONLY WINNING MOVE IS NOT TO PLAY." << '\n' << '\n';
    console << "HOW ABOUT A NICE GAME OF CHESS?" << '\n';
}
