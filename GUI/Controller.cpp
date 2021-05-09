// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Application.h"
#include "ADFFile.h"

extern "C" {
void process(const void *ref, long id, long data)
{
    ((Controller *)ref)->processMessage(id, data);
}
}

Controller::Controller(Application &ref) : GUIComponent(ref)
{
    printf("Creating controller\n");
}

Controller::~Controller()
{
    
}

void
Controller::init()
{
}

void
Controller::awake()
{
    // Register to the message queue
    amiga.msgQueue.setListener(this, process);
}

void
Controller::deinit()
{
    amiga.powerOff();
}

void
Controller::processMessage(long id, long data)
{
    MsgType msg = (MsgType)id;
    isize pan, vol;
    
    switch (msg) {
            
        case MSG_POWER_ON:
            canvas.powerOn();
            return;
            
        case MSG_POWER_OFF:
            canvas.powerOff();
            statusBar.close();
            return;

        case MSG_RUN:
            return;

        case MSG_PAUSE:
            return;

        case MSG_RESET:
            updateWarp();
            return;
                  
        case MSG_MUTE_ON:
        case MSG_MUTE_OFF:
            statusBar.setNeedsUpdate(StatusBarItem::MUTE);
            return;

        case MSG_WARP_ON:
        case MSG_WARP_OFF:
            statusBar.setNeedsUpdate(StatusBarItem::STATE);
            return;
            
        case MSG_POWER_LED_ON:
        case MSG_POWER_LED_DIM:
        case MSG_POWER_LED_OFF:
            statusBar.setNeedsUpdate(StatusBarItem::POWER_LED);
            return;
            
        case MSG_DISK_INSERT:
            vol = (i8)((data >> 16) & 0xFF);
            pan = (i8)((data >> 24) & 0xFF);
            app.playInsert(vol, pan);
            return;

        case MSG_DISK_EJECT:
            vol = (i8)((data >> 16) & 0xFF);
            pan = (i8)((data >> 24) & 0xFF);
            app.playEject(vol, pan);
            return;

        case MSG_DRIVE_STEP:
            vol = (i8)((data >> 16) & 0xFF);
            pan = (i8)((data >> 24) & 0xFF);
            app.playClick(vol, pan);
            app.statusBar.setNeedsUpdate(StatusBarItem::DRIVE_CYL, data);
            return;
            
        case MSG_DRIVE_POLL:
            vol = (i8)((data >> 16) & 0xFF);
            pan = (i8)((data >> 24) & 0xFF);
            app.playClick(vol, pan);
            app.statusBar.setNeedsUpdate(StatusBarItem::DRIVE_CYL, data);
            return;

        case MSG_DRIVE_MOTOR_ON:
        case MSG_DRIVE_MOTOR_OFF:
            updateWarp();
            app.statusBar.setNeedsUpdate(StatusBarItem::DISK_SPIN, data);
            return;
            
        case MSG_SHAKING:
            app.inputManager.releaseMouse();
            return;
            
        case MSG_SNAPSHOT_RESTORED:
            updateWarp();
            return;
            
        default:
            break;
            
    }
    // printf("%s\n", MsgTypeEnum::key(msg));
}

void
Controller::updateWarp()
{    
    bool warp;
    
    // Determin the new warp status based on the selected warp activation mode
    switch (warpActivation) {
            
        case WarpActivation::never:
            warp = false;
            break;
            
        case WarpActivation::always:
            warp = true;
            break;
            
        case WarpActivation::automatic:
            warp = amiga.paula.diskController.spinning();
            break;
            
        default:
            assert(false);
    }
        
    // Change warp status if neccessary
    warp ? amiga.signalWarpOn() : amiga.signalWarpOff();
}

void
Controller::flipWarpMode()
{
    switch (warpActivation) {
            
        case WarpActivation::automatic:
            warpActivation = WarpActivation::never;
            break;
            
        case WarpActivation::never:
            warpActivation = WarpActivation::always;
            break;
            
        case WarpActivation::always:
            warpActivation = WarpActivation::automatic;
            break;
            
        default:
            assert(false);
    }
    
    updateWarp();
    statusBar.setNeedsUpdate(StatusBarItem::STATE);
}
