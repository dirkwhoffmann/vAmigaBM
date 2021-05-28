// -----------------------------------------------------------------------------
// This file is part of vAmiga
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include "MsgQueueTypes.h"
#include "AmigaComponent.h"
#include "RingBuffer.h"

class MsgQueue : public AmigaComponent {
        
    // Ring buffer storing all pending messages
    util::RingBuffer<Message, 64> queue;
                
    // The registered listener
    const void *listener = nullptr;
    
    // The registered callback function
    Callback *callback = nullptr;
    
    
    //
    // Constructing
    //
    
public:
    
    using AmigaComponent::AmigaComponent;
    // MsgQueue(Amiga& ref) : AmigaComponent(ref) { }

    
    //
    // Methods from HardwareComponent
    //
    
public:
        
    const char *getDescription() const override { return "MsgQueue"; }

private:
    
    void _initialize() override;
    void _reset(bool hard) override { };
    
    isize _size() override { return 0; }
    isize _load(const u8 *buffer) override { return 0; }
    isize _save(u8 *buffer) override { return 0; }
    
    
    //
    // Managing the queue
    //
    
public:
    
    // Registers a listener together with it's callback function
    void setListener(const void *listener, Callback *func);
            
    // Sends a message
    void put(MsgType type, long data = 0);
};
