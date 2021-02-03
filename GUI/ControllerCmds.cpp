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
    amiga.powerOn();
}

template <> void
Controller::exec <Token::amiga, Token::off> (Arguments &argv, long param)
{
    amiga.powerOff();
}

template <> void
Controller::exec <Token::amiga, Token::run> (Arguments &argv, long param)
{
    amiga.run();
}

template <> void
Controller::exec <Token::amiga, Token::pause> (Arguments &argv, long param)
{
    amiga.pause();
}

template <> void
Controller::exec <Token::amiga, Token::reset> (Arguments &argv, long param)
{
    amiga.reset(true);
}

template <> void
Controller::exec <Token::amiga, Token::dump> (Arguments &argv, long param)
{
    dump(amiga, Dump::State);
}


//
// Memory
//

template <> void
Controller::exec <Token::memory, Token::config> (Arguments& argv, long param)
{
    dump(amiga.mem, Dump::Config);
}

template <> void
Controller::exec <Token::memory, Token::load, Token::rom> (Arguments& argv, long param)
{
    amiga.mem.loadRomFromFile(argv.front().c_str());
}

template <> void
Controller::exec <Token::memory, Token::load, Token::extrom> (Arguments& argv, long param)
{
    amiga.mem.loadExtFromFile(argv.front().c_str());
}

template <> void
Controller::exec <Token::memory, Token::set, Token::chip> (Arguments& argv, long param)
{
    amiga.configure(OPT_CHIP_RAM, parseNumeric(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::slow> (Arguments& argv, long param)
{
    amiga.configure(OPT_SLOW_RAM, parseNumeric(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::fast> (Arguments& argv, long param)
{
    amiga.configure(OPT_FAST_RAM, parseNumeric(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::extrom> (Arguments& argv, long param)
{
    amiga.configure(OPT_EXT_START, parseNumeric(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::extstart> (Arguments& argv, long param)
{
    amiga.configure(OPT_EXT_START, parseNumeric(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::slowramdelay> (Arguments& argv, long param)
{
    amiga.configure(OPT_SLOW_RAM_DELAY, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::bankmap> (Arguments& argv, long param)
{
    amiga.configure(OPT_BANKMAP, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::unmappingtype> (Arguments& argv, long param)
{
    amiga.configure(OPT_UNMAPPING_TYPE, BankMapEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::set, Token::raminitpattern> (Arguments& argv, long param)
{
    amiga.configure(OPT_RAM_INIT_PATTERN, BankMapEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::memory, Token::dump, Token::state> (Arguments& argv, long param)
{
    dump(amiga.mem, Dump::State);
}

template <> void
Controller::exec <Token::memory, Token::dump, Token::bankmap> (Arguments& argv, long param)
{
    dump(amiga.mem, Dump::BankMap);
}

template <> void
Controller::exec <Token::memory, Token::dump, Token::checksum> (Arguments& argv, long param)
{
    dump(amiga.mem, Dump::Checksums);
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
// CIA
//

template <> void
Controller::exec <Token::cia, Token::config> (Arguments &argv, long param)
{
    dump(amiga.ciaA, Dump::Config);
}

template <> void
Controller::exec <Token::cia, Token::dump> (Arguments& argv, long param)
{
    dump(amiga.ciaA, Dump::State);
}

template <> void
Controller::exec <Token::cia, Token::set, Token::revision> (Arguments &argv, long param)
{
    amiga.configure(OPT_CIA_REVISION, CIARevisionEnum::parse(argv.front()));
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
Controller::exec <Token::agnus, Token::set, Token::slowrammirror> (Arguments &argv, long param)
{
    amiga.configure(OPT_SLOW_RAM_MIRROR, parseBool(argv.front()));
}


//
// Blitter
//

template <> void
Controller::exec <Token::blitter, Token::dump> (Arguments& argv, long param)
{
    dump(amiga.agnus.blitter, Dump::State);
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

template <> void
Controller::exec <Token::denise, Token::set, Token::revision> (Arguments &argv, long param)
{
    amiga.configure(OPT_DENISE_REVISION, DeniseRevisionEnum::parse(argv.front()));
}

template <> void
Controller::exec <Token::denise, Token::set, Token::borderblank> (Arguments &argv, long param)
{
    amiga.configure(OPT_BRDRBLNK, parseBool(argv.front()));
}


//
// Paula
//

template <> void
Controller::exec <Token::paula, Token::config> (Arguments& argv, long param)
{
    dump(amiga.paula.muxer, Dump::Config);
}

template <> void
Controller::exec <Token::paula, Token::dump, Token::registers> (Arguments& argv, long param)
{
    dump(amiga.paula.muxer, Dump::Registers);
}


//
// RTC
//

template <> void
Controller::exec <Token::rtc, Token::config> (Arguments& argv, long param)
{
    dump(amiga.rtc, Dump::Config);
}

template <> void
Controller::exec <Token::rtc, Token::dump, Token::registers> (Arguments& argv, long param)
{
    dump(amiga.rtc, Dump::Registers);
}

template <> void
Controller::exec <Token::rtc, Token::set, Token::revision> (Arguments &argv, long param)
{
    amiga.configure(OPT_RTC_MODEL, RTCRevisionEnum::parse(argv.front()));
}


//
// Serial port
//

template <> void
Controller::exec <Token::serial, Token::config> (Arguments& argv, long param)
{
    dump(amiga.serialPort, Dump::Config);
}

template <> void
Controller::exec <Token::serial, Token::dump> (Arguments& argv, long param)
{
    dump(amiga.serialPort, Dump::State);
}

template <> void
Controller::exec <Token::serial, Token::set, Token::device> (Arguments &argv, long param)
{
    amiga.configure(OPT_SERIAL_DEVICE, SerialPortDeviceEnum::parse(argv.front()));
}


//
// Disk controller
//

template <> void
Controller::exec <Token::diskcontroller, Token::config> (Arguments& argv, long param)
{
    dump(amiga.paula.diskController, Dump::Config);
}

template <> void
Controller::exec <Token::diskcontroller, Token::dump> (Arguments& argv, long param)
{
    dump(amiga.paula.diskController, Dump::Registers);
}

template <> void
Controller::exec <Token::diskcontroller, Token::speed> (Arguments& argv, long param)
{
    amiga.configure(OPT_DRIVE_SPEED, parseNumeric(argv.front()));
}

template <> void
Controller::exec <Token::diskcontroller, Token::dsksync, Token::autosync> (Arguments& argv, long param)
{
    amiga.configure(OPT_AUTO_DSKSYNC, parseBool(argv.front()));
}

template <> void
Controller::exec <Token::diskcontroller, Token::dsksync, Token::lock> (Arguments& argv, long param)
{
    amiga.configure(OPT_LOCK_DSKSYNC, parseBool(argv.front()));
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
Controller::exec <Token::dfn, Token::connect> (Arguments& argv, long param)
{
    printf("Df%ld::connect\n", param);
    amiga.configure(OPT_DRIVE_CONNECT, param, true);
}

template <> void
Controller::exec <Token::dfn, Token::disconnect> (Arguments& argv, long param)
{
    printf("Df%ld::disconnect\n", param);
    amiga.configure(OPT_DRIVE_CONNECT, param, false);
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
