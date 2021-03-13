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
    amiga.queue.setListener(this, process);
}

void
Controller::deinit()
{
    amiga.powerOff();
    amiga.queue.removeListener();
}

void
Controller::processMessage(long id, long data)
{
    MsgType msg = (MsgType)id;

    switch (msg) {
            
        case MSG_POWER_ON:
            canvas.powerOn();
            break;
            
        case MSG_POWER_OFF:
            canvas.powerOff();
            statusBar.close();
            break;

        case MSG_RUN:
            break;

        case MSG_PAUSE:
            break;

        case MSG_RESET:
            updateWarp();
            break;
                  
        case MSG_MUTE_ON:
        case MSG_MUTE_OFF:
            statusBar.setNeedsUpdate(StatusBarItem::MUTE);
            break;

        case MSG_WARP_ON:
        case MSG_WARP_OFF:
            statusBar.setNeedsUpdate(StatusBarItem::STATE);
            break;
            
        case MSG_POWER_LED_ON:
        case MSG_POWER_LED_DIM:
        case MSG_POWER_LED_OFF:
            statusBar.setNeedsUpdate(StatusBarItem::POWER_LED);
            break;
            
        case MSG_DISK_INSERT:
            app.play(SoundID::insert);
            break;

        case MSG_DISK_EJECT:
            app.play(SoundID::eject);
            break;

        case MSG_DRIVE_STEP:
            app.playClick();
            app.statusBar.setNeedsUpdate(StatusBarItem::DRIVE_CYL, data);
            break;
            
        case MSG_DRIVE_POLL:
            app.playClick();
            app.statusBar.setNeedsUpdate(StatusBarItem::DRIVE_CYL, data);
            return;

        case MSG_DRIVE_MOTOR_ON:
        case MSG_DRIVE_MOTOR_OFF:
            updateWarp();
            app.statusBar.setNeedsUpdate(StatusBarItem::DISK_SPIN, data);
            break;
            
        case MSG_SHAKING:
            printf("SHAKING\n");
            app.inputManager.releaseMouse();
            break;
            
        case MSG_SNAPSHOT_RESTORED:
            updateWarp();
            break;
            
        default:
            return;
            
    }
    printf("%s\n", MsgTypeEnum::key(msg));
}

void
Controller::updateWarp()
{
    printf("updateWarp()\n");
    
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

void
Controller::insertDisk(const string &name, isize n)
{
    auto path = "/tmp/" + name;
    
    try {
        ADFFile *adf = AmigaFile::make <ADFFile> (path.c_str());
        Disk *disk = Disk::makeWithFile(adf);
        amiga.paula.diskController.insertDisk(disk, n);
        
    } catch (VAError &err) {
        console << "Failed to insert disk: " << err.what() << '\n';
    }
}

bool
Controller::parseBool(string& token)
{
    if (token == "1" || token == "true" || token == "yes") return true;
    if (token == "0" || token == "false" || token == "no") return false;

    throw ParseBoolError("");
}

long
Controller::parseNum(string& token)
{
    long result;
    
    try { result = stol(token, nullptr, 0); }
    catch (std::exception& err) { throw ParseNumError(token); }

    return result;
}

void
Controller::dump(HardwareComponent &component, Dump::Category category)
{
    std::stringstream ss; string line;
    
    amiga.suspend();
    component.dump(category, ss);
    amiga.resume();
    
    while(std::getline(ss, line)) console << line << '\n';
}
