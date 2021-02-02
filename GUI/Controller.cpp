// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

extern "C" {
void process(const void *ref, long id, long data)
{
    ((Controller *)ref)->processMessage(id, data);
}
}

Controller::Controller(Application &ref) :
app(ref), amiga(ref.amiga), console(ref.console)
{
    printf("Creating controller\n");
}

Controller::~Controller()
{
    
}

void
Controller::init()
{
    ErrorCode ec;
    
    amiga.queue.addListener(this, process);
    
    try {
        RomFile *rom = AmigaFile::make <RomFile> ("/tmp/kick13.rom");
        amiga.mem.loadRom(rom);
        amiga.configure(OPT_CHIP_RAM, 512);
        amiga.denise.pixelEngine.setPalette(PALETTE_COLOR);
        
    } catch (VAError &exception) {
        printf("Can't find kickstart rom\n");
    }
    
    if (amiga.isReady(&ec)) {
        amiga.run();
    } else {
        printf("Amiga can't run: %s\n", ErrorCodeEnum::key(ec));
    }    
}

void
Controller::deinit()
{
    amiga.powerOff();
    amiga.queue.removeListener(this);
}

void
Controller::processMessage(long id, long data)
{
    MsgType msg = (MsgType)id;

    switch (msg) {
            
        case MSG_POWER_ON:
            break;
            
        case MSG_POWER_OFF:
            break;

        case MSG_RUN:
            break;

        case MSG_PAUSE:
            break;

        case MSG_RESET:
            break;
            
        default:
            return;
            
    }
    printf("%s\n", MsgTypeEnum::key(msg));
}
