// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

SplashScreen::SplashScreen(Application &ref) : Layer(ref)
{
    alpha = targetAlpha = 0xFF;
}

SplashScreen::~SplashScreen()
{
    
}

void
SplashScreen::init()
{
    Layer::init();
    
    // Config file
    configPath = app.argv.size() <= 1 ? "startup.ini" : app.argv[1];
    configFile = extractName(configPath);
    
    // Help text
    info[0].setStyle(Assets::get(FontID::sans_r), 40, sf::Color(0xE0,0x50,0x50,0xFF));
    info[1].setStyle(Assets::get(FontID::sans_l), 46, sf::Color(0x50,0x50,0x50,0xFF));
    info[2].setStyle(Assets::get(FontID::sans_l), 36, sf::Color(0x50,0x50,0x50,0xFF));
    info[3].setStyle(Assets::get(FontID::sans_l), 36, sf::Color(0x50,0x50,0x50,0xFF));

    info[0].setPosition(sf::Vector2f(w * 0.5, h * 0.685));
    info[1].setPosition(sf::Vector2f(w * 0.5, h * 0.74));
    info[2].setPosition(sf::Vector2f(w * 0.5, h * 0.85));
    info[3].setPosition(sf::Vector2f(w * 0.5, h * 0.90));

    info[2].setString("Press F11 to open the menu");
    info[3].setString("Press F12 to open the debug console");

    // Setup background
    background.init(w, h,
                    sf::Color(0x89,0x89,0x89), sf::Color(0x89,0x89,0x89),
                    sf::Color(0xF0,0xF0,0xF0), sf::Color(0xF0,0xF0,0xF0));
    
    // Setup vAmiga logo
    logo.init(0.5 * w, 0.37 * h, 0.62 * w, Assets::get(TextureID::logo));
    
    // Launch the emulator
}

void
SplashScreen::launchPhase(isize phase)
{
    std::ifstream stream;
    // auto warn = sf::Color(0xE0,0x50,0x50,0xFF);
    
    switch (phase) {
            
        case 1: // Open and process the config file
            
            stream.open(configPath);
            if (!stream.is_open()) {
                info[0].setString("Failed to open " + configFile);
                info[1].setString("Press SPACE to quit");
                action = Quit;
                return;
            }
            
            try { app.console.exec(stream); }
            catch (Exception &e) {
                info[0].setString(configFile + ": Error in line " + std::to_string(e.data));
                info[1].setString("Press SPACE to quit");
                action = Quit;
                return;
            }
            
            [[fallthrough]];
            
        case 2: // Check the Kickstart Rom
            
            if (!app.amiga.mem.hasKickRom()) {
                info[0].setString("No Kickstart Rom");
                info[1].setString("Press SPACE to install the Aros Kickstart replacement");
                action = Aros;
                return;
            }
            
            [[fallthrough]];
            
        case 3: // Check if the emulator can run with the current config
            
            if (!app.amiga.isReady()) {
                info[0].setString("Failed to launch the emulator");
                info[1].setString("Press SPACE to quit");
                action = Quit;
                return;
            }
        
            [[fallthrough]];
            
        case 4: // All clear. Ready to lift off
            
            info[0].setString("");
            info[1].setString("Press SPACE to start");
            return;
    }
}

bool
SplashScreen::isVisible()
{
    return app.amiga.isPoweredOff() || app.canvas.isAnimating();
}

void
SplashScreen::handle(const sf::Event &event)
{
    
}

void
SplashScreen::render()
{
    background.draw(app.window);
    logo.draw(app.window);
    info[0].draw(app.window);
    info[1].draw(app.window);
    info[2].draw(app.window);
    info[3].draw(app.window);
}

void
SplashScreen::resize(float width, float height)
{
    printf("(%f,%f)\n", width, height); 
}
