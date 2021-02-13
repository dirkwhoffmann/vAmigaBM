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
        
    unsigned int channelCnt = 2;
    unsigned int sampleRate = 44100;
    
    app.amiga.paula.muxer.setSampleRate(sampleRate);

    initialize(channelCnt, sampleRate);
}

bool
AmigaMusicStream::onGetData(sf::SoundStream::Chunk &data)
{    
    data.samples = (i16 *)app.amiga.paula.muxer.nocopy(512);
    data.sampleCount = 1024;
    
    return true;
}

void
AmigaMusicStream::onSeek(sf::Time timeOffset)
{
    printf("onSeek: %lld\n", timeOffset.asMicroseconds());
}
