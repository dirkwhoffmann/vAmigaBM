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
console(*this),
splashScreen(*this),
canvas(*this),
controller(*this),
musicStream(*this)
{
    for (int i = 0; i < argc; i++) {
        this->argv.push_back(string(argv[i]));
    }
}

Application::~Application()
{
}

sf::VideoMode
Application::proposedVideoMode()
{
    auto print = [](sf::VideoMode &mode)
    {
        std::cout << mode.width << "x" << mode.height
        << " - " << mode.bitsPerPixel << " bpp" << std::endl;
    };
    
    // List all video modes
    std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
    for (auto &mode : modes) {
        std::cout << "Fullscreen: "; print(mode);
    }
    auto mode = sf::VideoMode::getDesktopMode();
    std::cout << "Desktop: "; print(mode);

    return mode;
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
    
    
    // Select a suitable video mode
    videoMode = proposedVideoMode();

    // Create render window
    // curw = videoMode.width / 2;
    // curh = curw * aspectRatio;
    curw = 752 * 2;
    curh = curw * aspectRatio;
    window.create(sf::VideoMode(curw, curh), "vAmiga Bare Metal");

    window.setFramerateLimit(60);

    if (!window.isOpen()) {
        throw Exception("Unable to create window");
    }
        
    controller.init();
    splashScreen.init();
    canvas.init();
    console.init();
    musicStream.init();
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
            {
                window.close();
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (event.key.code == sf::Keyboard::F11) console.toggle();
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
            }
            case sf::Event::Resized:
            {
                float w = event.size.width;
                float h = event.size.height;
                
                if (w < 752 || h < 752 * aspectRatio) {
                    w = 752;
                    h = w * aspectRatio;
                    window.setSize(sf::Vector2u{(unsigned)w,(unsigned)h});
                }
                
                window.setView(sf::View(sf::FloatRect(0, 0, w, h)));
                
                /*
                sf::FloatRect rect;
                if (h / w < aspectRatio) {
                    
                    w = h / aspectRatio / w;
                    rect = sf::FloatRect((1.0 - w) * 0.5, 0.0, w, 1.0);

                } else {
                    
                    h = w * aspectRatio / h;
                    rect = sf::FloatRect(0.0, (1.0 - h) * 0.5, 1.0, h);
                }
                
                auto view = window.getDefaultView();
                view.setViewport(rect);
                window.setView(view);
                */
                
                splashScreen.resize(w, h);
                canvas.resize(w, h);
                console.resize(w, h);
                break;
            }
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
