// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

Application::Application()
{
}

Application::~Application()
{
}

void
Application::run()
{
    if (!sf::Shader::isAvailable()) {
        throw std::runtime_error("Sorry. No shader support\n");
    }
    
    // sf::Shader &shader = AssetManager::shader(ShaderID::blur);
    
    unsigned desktopW = sf::VideoMode::getDesktopMode().width;
    // unsigned desktopH = sf::VideoMode::getDesktopMode().height;
    unsigned w = desktopW / 2;
    unsigned h = w * 0.7525;
    
    std::cout << "vAmigaSFML\n";

    sf::Texture logoTex = Assets::get(TextureID::logo);
    
    // if (!texture.loadFromFile("terminator.jpeg")) {
    /*
    if (!logoTex.loadFromFile("logo.png")) {
        printf("Error loading logo texture\n");
        exit(1);
    }
    */
    
    sf::Vector2u size = logoTex.getSize();
    unsigned logoWidth = w / 1.5;
    unsigned logoHeight = logoWidth * ((float)size.y / (float)size.x);
    
    sf::RectangleShape background(sf::Vector2f(logoWidth, logoHeight));
    background.setTexture(&logoTex);
    background.setPosition((w - logoWidth) / 2, h/8);
    
    if (!console.init()) {
        printf("Can't initialize Console\n");
        exit(1);
    }
        
    sf::RenderWindow window(sf::VideoMode(w,h), "My Window");
    window.setFramerateLimit(60);
    // setVerticalSyncEnabled(true);
        
    sf::Vertex rectangle[] =
    {
        sf::Vertex(sf::Vector2f(0,0), sf::Color(0x89,0x89,0x89)),
        sf::Vertex(sf::Vector2f(w,0), sf::Color(0x89,0x89,0x89)),
        sf::Vertex(sf::Vector2f(w,h), sf::Color(0xF0,0xF0,0xF0)),
        sf::Vertex(sf::Vector2f(0,h), sf::Color(0xF0,0xF0,0xF0))
    };
    
    while (window.isOpen()) {
        
        sf::Event event;
        
        while (window.pollEvent(event)) {
            
            switch (event.type) {
                    
                case sf::Event::Closed:
                    window.close();
                    break;
                    
                case sf::Event::TextEntered:
                    if (event.text.unicode < 128) {
                        std::cout << "Character: " << static_cast<char>(event.text.unicode) << std::endl;
                        console.add(static_cast<char>(event.text.unicode));
                    }
                    break;
                    
                default:
                    break;
                    
            }
        }

        window.clear(sf::Color::Blue);
        
        window.draw(rectangle, 4, sf::Quads);
        window.draw(background);

        console.render(window);
        
        window.display();
    }    
}
