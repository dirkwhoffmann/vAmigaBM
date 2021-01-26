//
//  main.cpp
//  vAmigaSFML
//
//  Created by Dirk Hoffmann on 26.01.21.
//

#include "Console.h"
#include <unistd.h>

Console console;

int main(int argc, const char * argv[]) {

    /*
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
    */
    
    unsigned desktopW = sf::VideoMode::getDesktopMode().width;
    // unsigned desktopH = sf::VideoMode::getDesktopMode().height;
    unsigned w = desktopW / 2;
    unsigned h = w * 0.7525;

    std::cout << "vAmigaSFML\n";
    
    sf::Texture texture;
    // if (!texture.loadFromFile("terminator.jpeg")) {
    if (!texture.loadFromFile("logo.png")) {
        printf("Error loading logo\n");
        exit(1);
    }
    sf::Vector2u size = texture.getSize();
    
    unsigned logoWidth = w / 1.5;
    unsigned logoHeight = logoWidth * ((float)size.y / (float)size.x);
    
    sf::RectangleShape background(sf::Vector2f(logoWidth, logoHeight));
    background.setTexture(&texture);
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
    
    return 0;
}
