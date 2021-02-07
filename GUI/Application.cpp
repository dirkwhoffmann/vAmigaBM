// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"
#include "Amiga.h"

Application::Application() :
console(*this), splashScreen(*this), canvas(*this), controller(*this), interpreter(*this)
{
}

Application::~Application()
{
}

void
Application::run()
{
    printf("vAmigaSFML\n");

    if (!sf::Shader::isAvailable()) {
        throw std::runtime_error("Sorry. No shader support\n");
    }
     
    window.create(sf::VideoMode(W,H), "My Window");
    window.setFramerateLimit(60);
    
    splashScreen.init();
    canvas.init();
    
    if (!console.init()) {
        printf("Can't initialize Console\n");
        exit(1);
    }

    controller.init();
    
    console.exec("source startup.ini", true);
    
    while (window.isOpen()) {
        
        sf::Time dt = clock.restart();
        
        processEvents();
        update(dt);
        render();
    }
    
    controller.deinit();
}

void
Application::processEvents()
{
    sf::Event event;
    
    while (window.pollEvent(event)) {
        
        switch (event.type) {
                
            case sf::Event::Closed:
                
                window.close();
                break;
                
            case sf::Event::KeyPressed:
                
                if (event.key.code == sf::Keyboard::F12) console.toggle();
                break;
                
            default:
                break;
        }
        
        // Distribute the event to the uppermost visible layer
        if (console.isVisible())     { console.handle(event); }
        else if (canvas.isVisible()) { canvas.handle(event); }
        else                         { splashScreen.handle(event); }
    }
}

void
Application::update(sf::Time dt)
{
    splashScreen.update(dt);
    canvas.update(dt);
    console.update(dt);
}

void
Application::render()
{
    // Splash screen layer
    if (splashScreen.isVisible()) splashScreen.render();
    
    // Emulator layer
    if (canvas.isVisible()) canvas.render();
    
    // Console layer
    if (console.isVisible()) console.render();

    window.display();
}
