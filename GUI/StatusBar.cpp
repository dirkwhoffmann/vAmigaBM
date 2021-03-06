// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

StatusBar::StatusBar(Application &ref) : Layer(ref)
{
    
}

StatusBar::~StatusBar()
{
    
}

void StatusBar::init()
{
    Layer::init();

    // Configure the animation speed
    delay = 0.5;
}

void
StatusBar::awake()
{
    // auto size = app.window.getSize();
}

void
StatusBar::update(u64 frames, sf::Time dt)
{
    Layer::update(frames, dt);
}

void
StatusBar::render()
{
    
}

void
StatusBar::respond(const sf::Event &event)
{
    
}

void
StatusBar::resize(float width, float height)
{
    
}
