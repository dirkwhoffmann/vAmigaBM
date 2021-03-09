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
    amiga.queue.addListener(this, process);
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
            canvas.powerOn();
            statusBar.open();
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
            break;
                  
        case MSG_MUTE_ON:
        case MSG_MUTE_OFF:
        case MSG_WARP_ON:
        case MSG_WARP_OFF:
            statusBar.needsUpdate |= StatusBarItem::MUTE;
            break;
            
        case MSG_POWER_LED_ON:
        case MSG_POWER_LED_DIM:
        case MSG_POWER_LED_OFF:
            statusBar.needsUpdate |= StatusBarItem::POWER_LED;
            break;
            
        case MSG_DISK_INSERT:
            app.play(SoundID::insert);
            break;

        case MSG_DISK_EJECT:
            app.play(SoundID::eject);
            break;

        case MSG_DRIVE_STEP:
            app.playClick();
            app.statusBar.needsUpdate |= (StatusBarItem::DRIVE_CYL << data);
            break;
            
        case MSG_DRIVE_POLL:
            app.playClick();
            app.statusBar.needsUpdate |= (StatusBarItem::DRIVE_CYL << data);
            return;

        case MSG_DRIVE_MOTOR_ON:
        case MSG_DRIVE_MOTOR_OFF:
            printf("Drive: %ld\n", data);
            app.statusBar.needsUpdate |= (StatusBarItem::DISK_SPIN << data);
            break;
            
        case MSG_SHAKING:
            printf("SHAKING\n");
            app.inputManager.releaseMouse();
            break;
            
        default:
            return;
            
    }
    printf("%s\n", MsgTypeEnum::key(msg));
}

bool
Controller::parseBool(string& token)
{
    if (token == "1" || token == "true" || token == "yes") return true;
    if (token == "0" || token == "false" || token == "no") return false;

    throw ParseBoolError("");
}

long
Controller::parseNum(isize base, string& token)
{
    assert(base == 10 || base == 16);
    
    long result;
    
    std::stringstream ss;
    try { ss << (base == 10 ? std::dec : std::hex) << token; ss >> result; }
    catch (std::exception& err) { result = 1; }

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
