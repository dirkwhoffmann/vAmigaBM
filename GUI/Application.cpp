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

Application::Application() : console(*this), controller(*this), interpreter(*this)
{
}

Application::~Application()
{
}

void
Application::run()
{
    printf("vAmigaSFML\n");

    if (!sf::Shader::isAvailable()) {
        throw std::runtime_error("Sorry. No shader support\n");
    }
     
    window.create(sf::VideoMode(W,H), "My Window");
    window.setFramerateLimit(60);
    
    emuTex.create(HPIXELS, VPIXELS);
    
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
    
    int x1 = HBLANK_CNT * 4;
    int x2 = HPOS_CNT * 4;
    int y1 = VBLANK_CNT;
    int y2 = VPOS_CNT - 1;
    foreground.setSize(sf::Vector2f(W,H));
    foreground.setTexture(&emuTex);
    foreground.setTextureRect(sf::IntRect(x1, y1, x2 - x1, y2 - y1));

    if (!console.init()) {
        printf("Can't initialize Console\n");
        exit(1);
    }

    controller.init();
    
    console.exec("source startup.ini", true);
    
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
                
                if (event.key.code == sf::Keyboard::F12) console.toggle();
                break;
                
            default:
                break;
        }
        
        if (console.isResponsive()) console.handle(event);
    }
}

void
Application::update(sf::Time dt)
{
    if (amiga.isPoweredOff()) {
        emuTex.update((u8 *)amiga.denise.pixelEngine.getNoise());
        return;
    }
    
    ScreenBuffer current = amiga.denise.pixelEngine.getStableBuffer();
    if (screenBuffer.data != current.data) {
        screenBuffer = current;
        emuTex.update((u8 *)(screenBuffer.data + 4 * HBLANK_MIN));
    }
    
    console.update(dt); 
}

void
Application::render()
{
    window.clear(sf::Color::Blue);
    
    window.draw(rectangle, 4, sf::Quads);
    window.draw(background);
    window.draw(info1);
    
    window.draw(foreground);
    
    console.render();
    
    window.display();
}
