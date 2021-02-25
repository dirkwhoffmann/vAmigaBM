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
GUIComponent(*this),
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
    
    // Setup window dimensions
    winXmin = OS::scale(canvas.textureRect.width);
    winYmin = OS::scale(canvas.textureRect.height);
    int winXinit = winXmin;
    int winYinit = winYmin;
    
    // Create render window
    auto videoMode = sf::VideoMode(winXinit, winYinit);
    window.create(videoMode, "vAmiga Bare Metal");

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
Application::awake()
{
    controller.awake();
    splashScreen.awake();
    canvas.awake();
    console.awake();
}

void
Application::run()
{
    sf::Clock clock;
    sf::Clock fpsClock;
    sf::Event event;
    u64 frames = 0;
    
    // Render at 60 Hz
    // window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    
    // Start music stream
    musicStream.setVolume(50.0); 
    musicStream.play();
    
    while (window.isOpen()) {
        
        sf::Time dt = clock.restart();

        while (window.pollEvent(event)) respond(event);
        update(frames, dt);
        render();
        
        // Compute the frames per second once in a while
        if (++frames % 128 == 0) fps(128.0 / fpsClock.restart().asSeconds());
    }
    
    controller.deinit();
}

void
Application::respond(sf::Event &event)
{
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
    if (console.isVisible()) console.respond(event);
    else if (canvas.isVisible()) canvas.respond(event);
    else if (splashScreen.isVisible()) splashScreen.respond(event);
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
Application::update(u64 frames, sf::Time dt)
{
    splashScreen.update(frames, dt);
    canvas.update(frames, dt);
    console.update(frames, dt);
}

void
Application::render()
{
    window.clear();
    
    if (!canvas.isOpaque()) splashScreen.render();
    if (canvas.isVisible()) canvas.render();
    if (console.isVisible()) console.render();

    window.display();
}

void
Application::fps(float fps)
{
    printf("fps: %.2f\n", fps);
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
