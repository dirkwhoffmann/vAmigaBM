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

Application::Application(int argc, const char *argv[]) :
controller(*this),
os(*this),
console(*this),
splashScreen(*this),
canvas(*this),
musicStream(*this)
{
    for (int i = 0; i < argc; i++) {
        this->argv.push_back(string(argv[i]));
    }
}

Application::~Application()
{
}

void
Application::check()
{
    // Check for shader support
    if (!sf::Shader::isAvailable()) {
        throw Exception("No shader support");
    }
}

void
Application::init()
{
    // REMOVE ASAP
    int x1 = HBLANK_CNT * 4;
    int x2 = HPOS_CNT * 4;
    int y1 = VBLANK_CNT;
    int y2 = VPOS_CNT - 1;
    printf("Emulator dimensions: %d x %d\n", x2 - x1, y2 - y1);
    
    // Create render window
    auto videoMode = sf::VideoMode(winXinit, winYinit);
    window.create(videoMode, "vAmiga Bare Metal");

    window.setFramerateLimit(60);

    if (!window.isOpen()) {
        throw Exception("Unable to create window");
    }
        
    controller.init();
    splashScreen.init();
    canvas.init();
    console.init();
    musicStream.init();
    
    // Fire a resize event
    resize(window.getSize().x, window.getSize().y);
}

void
Application::configure()
{
    controller.configure();
    splashScreen.configure();
    canvas.configure();
    console.configure();
}

void
Application::run()
{
    musicStream.setVolume(50.0); 
    musicStream.play();
    
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
                if (event.key.code == sf::Keyboard::F11) console.toggle();
                break;

            case sf::Event::MouseButtonPressed:
                break;
                
            case sf::Event::Resized:
                resize(event.size.width, event.size.height);
                break;

            default:
                break;
        }
        
        // Distribute the event to the uppermost visible layer
        if (console.isVisible()) console.handle(event);
        else if (canvas.isVisible()) canvas.handle(event);
        else if (splashScreen.isVisible()) splashScreen.handle(event);
    }
}

void
Application::resize(float w, float h)
{
    // Restore the minimal window size if the proposed size is smaller
    if (w < winXmin || h < winYmin) {
        
        w = winXmin;
        h = winYmin;
        
        window.setSize(sf::Vector2u{(unsigned)w,(unsigned)h});
    }
    
    // Adjust the view to the new size
    window.setView(sf::View(sf::FloatRect(0, 0, w, h)));

    // Inform all layers
    splashScreen.resize(w, h);
    canvas.resize(w, h);
    console.resize(w, h);
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
    if (splashScreen.isVisible()) splashScreen.render();
    if (canvas.isVisible()) canvas.render();
    if (console.isVisible()) console.render();

    window.display();
}

void
Application::play(SoundID id, float volume, isize min, isize max)
{
    // Search the pool for a free sound object
    for (isize i = min; i <= max; i++) {

        if (sound[i].getStatus() == sf::SoundSource::Status::Stopped) {
            
            sound[i].setBuffer(assets.get(id));
            sound[i].setVolume(volume);
            sound[i].play();
            break;
        }
    }
}
