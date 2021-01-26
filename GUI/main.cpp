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
    unsigned h = w / 1.78;

    std::cout << "vAmigaSFML\n";
    
    sf::Texture texture;
    if (!texture.loadFromFile("terminator.jpeg")) {
        printf("Error loading font\n");
        exit(1);
    }
    sf::RectangleShape background(sf::Vector2f(w, h));
    background.setTexture(&texture);
    
    if (!console.init()) {
        printf("Can't initialize Console\n");
        exit(1);
    }
        
    sf::RenderWindow window(sf::VideoMode(w,h), "My Window");
    window.setFramerateLimit(60);
    // setVerticalSyncEnabled(true);
        
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
        
        window.draw(background);
        
        console.render(window);
        
        window.display();
    }
    
    return 0;
}
