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
console(*this), splashScreen(*this), canvas(*this), controller(*this)
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
    // Create window
    window.create(sf::VideoMode(W,H), "vAmiga Bare Metal");
    window.setFramerateLimit(60);

    if (!window.isOpen()) {
        throw Exception("Unable to create window");
    }
    
    controller.init();
    splashScreen.init();
    canvas.init();
    console.init();
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
Application::configure()
{
    
}

void
Application::configure(const string& path)
{
    std::cout << "Reading config file " << path << std::endl;
        
    // Open file
    std::ifstream stream(path);
    if (!stream.is_open()) {
        std::cout << "Can't open file " << path << std::endl;
    }
    
    // Process script
    try {
        console.exec(stream);
    } catch (Exception &e) {
        std::cout << "Error in line " << DEC << (isize)e.data << '\n';
        console << e.what() << '\n';
    }
}

void
Application::run()
{    
    while (window.isOpen()) {
        
        sf::Time dt = clock.restart();
        // printf("dt = %f\n", dt.asSeconds());
        
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
                if (event.key.code == sf::Keyboard::F12) console.toggle();
                break;
            }
            case sf::Event::Resized:
            {
                float w = static_cast<float>(event.size.width);
                float h = static_cast<float>(event.size.height);
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
