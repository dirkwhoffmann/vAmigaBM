// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include <SFML/Audio.hpp>

class AmigaMusicStream : public sf::SoundStream
{
    class Application &app;
    
    short buffer[2048];
    
public :

    AmigaMusicStream(Application &ref) : sf::SoundStream(), app(ref) { }
    void init();
    
    virtual bool onGetData(sf::SoundStream::Chunk &data) override;
    virtual void onSeek(sf::Time timeOffset) override;
};
