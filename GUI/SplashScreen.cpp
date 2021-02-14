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
    
    // Messages
    errMsg.setStyle(app.assets.get(FontID::sans_r), 40, sf::Color(0xE0,0x50,0x50,0xFF));
    runMsg.setStyle(app.assets.get(FontID::sans_l), 46, sf::Color(0x50,0x50,0x50,0xFF));
    info[0].setStyle(app.assets.get(FontID::sans_l), 36, sf::Color(0x50,0x50,0x50,0xFF));
    info[1].setStyle(app.assets.get(FontID::sans_l), 36, sf::Color(0x50,0x50,0x50,0xFF));
    info[2].setStyle(app.assets.get(FontID::sans_l), 36, sf::Color(0x50,0x50,0x50,0xFF));

    errMsg.setPosition(sf::Vector2f(w * 0.5, h * 0.685));
    runMsg.setPosition(sf::Vector2f(w * 0.5, h * 0.74));
    info[0].setPosition(sf::Vector2f(w * 0.5, h * 0.80));
    info[1].setPosition(sf::Vector2f(w * 0.5, h * 0.85));
    info[2].setPosition(sf::Vector2f(w * 0.5, h * 0.90));

    info[1].setString("Press F11 to open the debug console");
    info[2].setString("Press F12 to open the menu");

    // Setup background
    background.init(w, h,
                    sf::Color(0x89,0x89,0x89), sf::Color(0x89,0x89,0x89),
                    sf::Color(0xF0,0xF0,0xF0), sf::Color(0xF0,0xF0,0xF0));
    
    // Setup vAmiga logo
    logo.init(0.5 * w, 0.37 * h, 0.62 * w, app.assets.get(TextureID::logo));
    
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
                errMsg.setString("Failed to open " + configFile);
                runMsg.setString("Press SPACE to quit");
                spcAction = Quit;
                f10Action = None;
                return;
            }
            
            try { app.console.exec(stream); }
            catch (Exception &e) {
                errMsg.setString(configFile + ": Error in line " + std::to_string(e.data));
                runMsg.setString("Press SPACE to quit");
                spcAction = Quit;
                f10Action = None;
                return;
            }
            
            [[fallthrough]];
            
        case 2: // Check the Kickstart Rom
            
            if (!app.amiga.mem.hasKickRom()) {
                errMsg.setString("No Kickstart Rom");
                runMsg.setString("");
                info[0].setString("Press F10 to install the Aros Kickstart replacement");
                spcAction = None;
                f10Action = Aros;
                return;
            }
            
            [[fallthrough]];
            
        case 3: // Check if the emulator can run with the current config
            
            if (!app.amiga.isReady()) {
                errMsg.setString("Failed to launch the emulator");
                runMsg.setString("Press SPACE to quit");
                spcAction = Quit;
                f10Action = None;
                return;
            }
        
            [[fallthrough]];
            
        case 4: // All clear. Ready to lift off
            
            errMsg.setString("");
            runMsg.setString("Press SPACE to start");
            info[0].setString("");
            spcAction = Launch;
            f10Action = None;
            return;
    }
}

bool
SplashScreen::loadAros()
{
    try {
        app.amiga.mem.loadRomFromFile("aros-amiga-m68k-rom.bin");
        app.amiga.mem.loadExtFromFile("aros-amiga-m68k-ext.bin");
        app.amiga.configure(OPT_EXT_START, 0xE0);
        return true;
        
    } catch (VAError &err) {
        printf("Failed to load Aros: %s\n", ErrorCodeEnum::key(err.errorCode));
        return false;
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
    switch (event.type) {
            
        case sf::Event::KeyPressed:
        {
            KeyAction action =
            (event.key.code == sf::Keyboard::F10) ? f10Action :
            (event.key.code == sf::Keyboard::Space) ? spcAction : None;
            
            switch (action) {
                    
                case Launch:
                    spcAction = None;
                    app.amiga.run();
                    break;
                    
                case Quit:
                    app.window.close();
                    break;
                    
                case Aros:
                    loadAros();
                    launchPhase(3);
                    break;
                    
                default:
                    break;
            }
        }
        default:
            break;
    }
}

void
SplashScreen::render()
{
    background.draw(app.window);
    logo.draw(app.window);
    errMsg.draw(app.window);
    runMsg.draw(app.window);
    info[0].draw(app.window);
    info[1].draw(app.window);
    info[2].draw(app.window);
}

void
SplashScreen::resize(float width, float height)
{
    printf("(%f,%f)\n", width, height); 
}
