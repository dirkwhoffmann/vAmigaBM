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
    if (!sf::Shader::isAvailable()) {
        throw std::runtime_error("Sorry. No shader support\n");
    }
    
    // sf::Shader &shader = AssetManager::shader(ShaderID::blur);
    
    unsigned desktopW = sf::VideoMode::getDesktopMode().width;
    // unsigned desktopH = sf::VideoMode::getDesktopMode().height;
    unsigned w = desktopW / 2;
    unsigned h = w * 0.7525;
    
    std::cout << "vAmigaSFML\n";

    sf::RenderWindow window(sf::VideoMode(w,h), "My Window");
    window.setFramerateLimit(60);
    // setVerticalSyncEnabled(true);

    
    
    sf::Texture logoTex = Assets::get(TextureID::logo);
    
    auto info1 = sf::Text("Press F12 to enter the debug console",
                          Assets::get(FontID::logo));
    info1.setCharacterSize(48);
    info1.setFillColor(sf::Color(0x50,0x50,0x50,0xFF));
    sf::FloatRect textRect = info1.getLocalBounds();
    info1.setOrigin(textRect.left + textRect.width/2.0f,
                   textRect.top  + textRect.height/2.0f);
    info1.setPosition(sf::Vector2f(w * 0.5,h * 0.8));
    
    
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
                    
                case sf::Event::KeyPressed:
                    
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
                    break; 

                case sf::Event::KeyReleased:
                    
                    console.keyReleased(event.key.code);
                    break;

                case sf::Event::TextEntered:
                    if (event.text.unicode < 128) {
                        std::cout << "Character: " << static_cast<char>(event.text.unicode) << std::endl;
                        console.type(static_cast<char>(event.text.unicode));
                    }
                    break;
                    
                default:
                    break;
                    
            }
        }

        window.clear(sf::Color::Blue);
        
        window.draw(rectangle, 4, sf::Quads);
        window.draw(background);
        window.draw(info1);
        
        console.render(window);
        
        window.display();
    }    
}