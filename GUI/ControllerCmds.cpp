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
Controller::exec <Token::agnus, Token::inspect> (Arguments &argv)
{
    printf("agnus inspect\n");
}

template <> void
Controller::exec <Token::agnus, Token::set> (Arguments &argv)
{
    printf("agnus set\n");
}

template <> void
Controller::exec <Token::agnus, Token::set, Token::revision> (Arguments &argv)
{
    printf("agnus set revision\n");
}

//
// Amiga
//

template <> void
Controller::exec <Token::amiga, Token::help> (Arguments &argv)
{
    printf("amiga help\n");
}

template <> void
Controller::exec <Token::amiga, Token::on> (Arguments &argv)
{
    printf("amiga on\n");
    amiga.powerOn();
}

template <> void
Controller::exec <Token::amiga, Token::off> (Arguments &argv)
{
    printf("amiga off\n");
    amiga.powerOff();
}

template <> void
Controller::exec <Token::amiga, Token::run> (Arguments &argv)
{
    printf("amiga run\n");
    amiga.run();
}

template <> void
Controller::exec <Token::amiga, Token::pause> (Arguments &argv)
{
    printf("amiga pause\n");
    amiga.pause();
}

template <> void
Controller::exec <Token::amiga, Token::reset> (Arguments &argv)
{
    printf("amiga reset\n");
    amiga.reset(true);
}

template <> void
Controller::exec <Token::amiga, Token::inspect> (Arguments &argv)
{
    printf("amiga inspect\n");
}

//
// CPU
//

template <> void
Controller::exec <Token::cpu, Token::inspect> (Arguments& argv)
{
    std::stringstream ss;
    string line;

    amiga.cpu._dump(ss);
    while(std::getline(ss, line)) console << line << '\n';
}

template <> void
Controller::exec <Token::easteregg> (Arguments& argv)
{
    console.println("GREETINGS PROFESSOR HOFFMANN");
    console.println();
    console.println("THE ONLY WINNING MOVE IS NOT TO PLAY.");
    console.println();
    console.println("HOW ABOUT A NICE GAME OF CHESS?");
}
