// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "config.h"
#include "Application.h"
#include "IO.h"

#include <fstream>

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
    configFile = util::extractName(configPath);
    
    // Messages
    title1.setStyle(app.assets.get(FontID::sans_b), 40, sf::Color(0x40,0x40,0x40,0xFF));
    title2.setStyle(app.assets.get(FontID::sans_l), 40, sf::Color(0x40,0x40,0x40,0xFF));
    errMsg.setStyle(app.assets.get(FontID::sans_r), 40, sf::Color(0xE0,0x50,0x50,0xFF));
    runMsg.setStyle(app.assets.get(FontID::sans_l), 46, sf::Color(0x50,0x50,0x50,0xFF));
    info1.setStyle(app.assets.get(FontID::sans_l), 36, sf::Color(0x50,0x50,0x50,0xFF));
    info2.setStyle(app.assets.get(FontID::sans_l), 36, sf::Color(0x50,0x50,0x50,0xFF));
    info3.setStyle(app.assets.get(FontID::sans_l), 36, sf::Color(0x50,0x50,0x50,0xFF));
    title1.setString("vAmiga");
    title2.setString("Bare Metal");
    info2.setString("Press F11 to open the debug console");
    // info3.setString("Press F12 to open the menu");
    info3.setString("");

    // Background
    background.init(0, 0,
                    sf::Color(0x89,0x89,0x89), sf::Color(0x89,0x89,0x89),
                    sf::Color(0xF0,0xF0,0xF0), sf::Color(0xF0,0xF0,0xF0));
    
    // Logo
    logo.init(app.assets.get(TextureID::logo));
    title.init(app.assets.get(TextureID::title));
}

void
SplashScreen::launchPhase(isize phase)
{
    std::ifstream stream;
    
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
            
            try {
                amiga.retroShell.execScript(stream);
                amiga.retroShell.printPrompt();
            }
            catch (Exception &e) {
                errMsg.setString(configFile + ": Error in line " + std::to_string(e.data));
                // runMsg.setString("Press SPACE to quit");
                runMsg.setString("");
                info1.setString("Press SPACE to quit");
                spcAction = Quit;
                f10Action = None;
                return;
            }
            
            [[fallthrough]];
            
        case 2: // Check the Kickstart Rom
            
            if (!app.amiga.mem.hasKickRom()) {
                errMsg.setString("No Kickstart Rom");
                runMsg.setString("");
                info1.setString("Press F10 to install the Aros Kickstart replacement");
                spcAction = None;
                f10Action = Aros;
                return;
            }
            
            [[fallthrough]];
            
        case 3: // Check if the emulator can run with the current config
            
            try { amiga.isReady(); } catch (const VAError &e) {
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
            info1.setString("");
            spcAction = Launch;
            f10Action = None;
            return;
    }
}

bool
SplashScreen::loadAros()
{
    try {
        app.amiga.mem.loadRom("aros-amiga-m68k-rom.bin");
        app.amiga.mem.loadExt("aros-amiga-m68k-ext.bin");
        app.amiga.configure(OPT_EXT_START, 0xE0);
        return true;
        
    } catch (VAError &err) {
        printf("Failed to load Aros: %s\n", ErrorCodeEnum::key(err.data));
        return false;
    }
}

void
SplashScreen::respond(const sf::Event &event)
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
                    statusBar.open();
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
SplashScreen::update(u64 frames, sf::Time dt)
{
    Layer::update(frames, dt);
}

void
SplashScreen::render()
{
    background.draw(app.window);
    logo.draw(app.window);
    title1.draw(app.window);
    title2.draw(app.window);
    errMsg.draw(app.window);
    runMsg.draw(app.window);
    info1.draw(app.window);
    info2.draw(app.window);
    info3.draw(app.window);
}

void
SplashScreen::resize(float width, float height)
{
    printf("(%f,%f)\n", width, height);
    
    background.setSize(width, height);
        
    logo.setPosition(width * 0.5, height * 0.08);
    logo.setH(height * 0.46);
    
    title1.setFontSize(height/13);
    title2.setFontSize(height/13);
    errMsg.setFontSize(height/28);
    runMsg.setFontSize(height/20);
    info1.setFontSize(height/28);
    info2.setFontSize(height/28);
    info3.setFontSize(height/28);
    
    // Determine centers for composed title text
    int text1w = title1.text.getLocalBounds().width;
    int text2w = title2.text.getLocalBounds().width;
    int offset = (text1w - text2w) / 2;

    title1.setPosition(width * 0.49 + offset, height * 0.57);
    title2.setPosition(width * 0.51 + offset, height * 0.57);
    errMsg.setPosition(width * 0.5, height * 0.685);
    runMsg.setPosition(width * 0.5, height * 0.74);
    info1.setPosition(width * 0.5, height * 0.80);
    info2.setPosition(width * 0.5, height * 0.85);
    info3.setPosition(width * 0.5, height * 0.90);
}
