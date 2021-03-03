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
    const isize numSamples = 1024;
    
    data.samples = (i16 *)app.amiga.paula.muxer.nocopy(numSamples);
    data.sampleCount = sizeof(SampleType) / 2 * numSamples;
    
    return true;
}

void
AmigaMusicStream::onSeek(sf::Time timeOffset)
{
    printf("onSeek: %lld\n", timeOffset.asMicroseconds());
}
