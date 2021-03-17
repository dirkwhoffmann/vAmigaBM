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

void
TextureManager::load(TextureID id)
{
    string path = "";
    
    switch (id) {
            
        case TextureID::logo: path = "logo.png"; break;
        case TextureID::title: path = "title.png"; break;
        case TextureID::disk: path = "disk.png"; break;
        case TextureID::diskProtected: path = "disk_protected.png"; break;
        case TextureID::diskLarge: path = "disk_large.png"; break;
        case TextureID::spin: path = "spin.png"; break;
        case TextureID::ledBlack: path = "led_black.png"; break;
        case TextureID::ledGrey: path = "led_grey.png"; break;
        case TextureID::ledGreen: path = "led_green.png"; break;
        case TextureID::ledRed: path = "led_red.png"; break;
        case TextureID::mute: path = "mute.png"; break;
        case TextureID::halt: path = "halt.png"; break;
        case TextureID::sync: path = "sync.png"; break;
        case TextureID::warp: path = "warp.png"; break;
        case TextureID::syncLock: path = "sync_lock.png"; break;
        case TextureID::warpLock: path = "warp_lock.png"; break;
        case TextureID::none: path = "none.png"; break;
        case TextureID::mouse: path = "mouse36.png"; break;
        case TextureID::joystick: path = "joystick.png"; break;
        case TextureID::keyset: path = "keyset.png"; break;
            
        default:
            assert(false);
    }
    
    std::unique_ptr<sf::Texture> asset(new sf::Texture());
    
    if (asset->loadFromFile(path)) {
        cache.insert(std::make_pair(id, std::move(asset)));
    } else {
        throw std::runtime_error("AssetManager: Can't load " + path);
    }
}

void
FontManager::load(FontID id)
{
    string path = "";
    
    switch (id) {
            
        // case FontID::console: path = "DroidSansMono.ttf"; break;
        case FontID::console: path = "IBMPlexMono-Medium.ttf"; break;
        case FontID::sans_l:  path = "SourceSansPro-Light.ttf"; break;
        case FontID::sans_r:  path = "SourceSansPro-Regular.ttf"; break;
        case FontID::sans_sb: path = "SourceSansPro-SemiBold.ttf"; break;
        case FontID::sans_b:  path = "SourceSansPro-Bold.ttf"; break;
        case FontID::sans_bk: path = "SourceSansPro-Black.ttf"; break;

        default:
            assert(false);
    }
    
    std::unique_ptr<sf::Font> asset(new sf::Font());
    
    if (asset->loadFromFile(path)) {
        cache.insert(std::make_pair(id, std::move(asset)));
    } else {
        throw std::runtime_error("AssetManager: Can't load " + path);
    }
}

void
SoundManager::load(SoundID id)
{
    string path = "";
    
    switch (id) {
            
        case SoundID::insert: path = "insert.wav"; break;
        case SoundID::eject:  path = "eject.wav"; break;
        case SoundID::click:  path = "click.wav"; break;

        default:
            assert(false);
    }
    
    std::unique_ptr<sf::SoundBuffer> asset(new sf::SoundBuffer());
    
    if (asset->loadFromFile(path)) {
        cache.insert(std::make_pair(id, std::move(asset)));
    } else {
        throw std::runtime_error("AssetManager: Can't load " + path);
    }
}

void
ShaderManager::load(ShaderID id)
{
    string vert = "";
    string frag = "";
    
    switch (id) {
            
        case ShaderID::merge: frag = "merge.frag"; break;
        case ShaderID::mergeBypass: frag = "mergeBypass.frag"; break;
            
        default:
            assert(false);
    }
    
    std::unique_ptr<sf::Shader> asset(new sf::Shader());
    
    /*
    if (vert != "") {
        if (asset->loadFromFile(vert, sf::Shader::Vertex)) {
        } else {
            throw std::runtime_error("AssetManager: Can't load " + vert);
        }
    }
    */
    if (asset->loadFromFile(frag, sf::Shader::Fragment)) {
        cache.insert(std::make_pair(id, std::move(asset)));
    } else {
        throw std::runtime_error("AssetManager: Can't load " + frag);
    }
}
