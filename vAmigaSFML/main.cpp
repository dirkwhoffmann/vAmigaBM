//
//  main.cpp
//  vAmigaSFML
//
//  Created by Dirk Hoffmann on 26.01.21.
//

#include "Console.h"

Console console;

int main(int argc, const char * argv[]) {

    std::cout << "vAmigaSFML\n";
    
    if (!console.init()) {
        printf("Can't initialize Console\n");
        exit(1);
    }
    
    unsigned w = sf::VideoMode::getDesktopMode().width;
    // unsigned h = sf::VideoMode::getDesktopMode().height;
    
    sf::RenderWindow window(sf::VideoMode(w/2,(w/2) / 1.4), "My Window");
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
                
        console.render(window);
        
        window.display();
    }
    
    return 0;
}
