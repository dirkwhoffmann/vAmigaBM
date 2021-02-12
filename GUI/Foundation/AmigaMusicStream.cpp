// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

void
AmigaMusicStream::init() {
        
    unsigned int channelCnt = 1;
    unsigned int sampleRate = 44100;
    
    initialize(channelCnt, sampleRate);
}

bool
AmigaMusicStream::onGetData(sf::SoundStream::Chunk &data)
{
    printf("onGetData: %zu\n", data.sampleCount);
        
    app.amiga.paula.muxer.copyMono(fbuffer, 512);
    
    for (isize i = 0; i < 512; i++) {
        buffer[i] = fbuffer[i] * 100000;
    }
    
    data.sampleCount = 512;
    data.samples = buffer;
    
    return true;
}

void
AmigaMusicStream::onSeek(sf::Time timeOffset)
{
    printf("onSeek: %lld\n", timeOffset.asMicroseconds());
}
