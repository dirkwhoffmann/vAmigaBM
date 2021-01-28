// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

Application::Application() : console(*this), interpreter(*this)
{
}

Application::~Application()
{
}

void
Application::run()
{
    std::cout << "vAmigaSFML\n";

    if (!sf::Shader::isAvailable()) {
        throw std::runtime_error("Sorry. No shader support\n");
    }
     
    window.create(sf::VideoMode(W,H), "My Window");
    
    // sf::RenderWindow window(sf::VideoMode(W,H), "My Window");
    window.setFramerateLimit(60);
    
    logoTex = Assets::get(TextureID::logo);
    
    info1 = sf::Text("Press F12 to enter the debug console",
                     Assets::get(FontID::logo));
    
    info1.setCharacterSize(48);
    info1.setFillColor(sf::Color(0x50,0x50,0x50,0xFF));
    sf::FloatRect textRect = info1.getLocalBounds();
    info1.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    info1.setPosition(sf::Vector2f(W * 0.5, H * 0.8));
        
    sf::Vector2u size = logoTex.getSize();
    unsigned logoWidth = 0.66 * W;
    unsigned logoHeight = logoWidth * ((float)size.y / (float)size.x);
    
    background.setSize(sf::Vector2f(logoWidth, logoHeight));
    background.setTexture(&logoTex);
    background.setPosition(0.17 * W, 0.125 * H);

    if (!console.init()) {
        printf("Can't initialize Console\n");
        exit(1);
    }
        
    while (window.isOpen()) {
        
        processEvents();
        update();
        render();
    }
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
                processKeyEvents(event);
                break;
                
            case sf::Event::KeyReleased:
                console.keyReleased(event.key.code);
                break;
                
            case sf::Event::TextEntered:
                if (event.text.unicode < 128) {
                    console.type(static_cast<char>(event.text.unicode));
                }
                break;
                
            default:
                break;
        }
    }
}

void
Application::processKeyEvents(const sf::Event& event)
{
    switch (event.key.code) {
            
        case sf::Keyboard::F12:
            
            printf("F12\n");
            console.toggle();
            break;
            
        default:
            
            if (console.isVisible()) {
                console.keyPressed(event.key.code);
            }
    }
}

void
Application::update()
{
    
}

void
Application::render()
{
    window.clear(sf::Color::Blue);
    
    window.draw(rectangle, 4, sf::Quads);
    window.draw(background);
    window.draw(info1);
    
    console.render(window);
    
    window.display();
}
