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


Assets::Assets(class Application &ref) :
textures(ref),
fonts(ref),
sounds(ref),
shaders(ref)
{

}

void
TextureManager::load(TextureID id)
{
    bool highdpi = scale(1) != 1;
    string path = "";
    
    switch (id) {
            
        case TextureID::diskLarge:
            path = "disk128.png"; break;
        case TextureID::ledBlack:
            path = "led_black.png"; break;
        case TextureID::ledGrey:
            path = "led_grey.png"; break;
        case TextureID::ledGreen:
            path = "led_green.png"; break;
        case TextureID::ledRed:
            path = "led_red.png"; break;
        case TextureID::logo:
            path = "logo.png"; break;
        case TextureID::title:
            path = "title.png"; break;
        case TextureID::disk:
            path = highdpi ? "disk36.png" : "disk18.png"; break;
        case TextureID::diskProtected:
            path = highdpi ? "disk_protected36.png" : "disk_protected18.png"; break;
        case TextureID::spin:
            path = highdpi ? "spin36.png" : "spin18.png"; break;
        case TextureID::mute:
            path = highdpi ? "mute36.png" : "mute18.png"; break;
        case TextureID::halt:
            path = highdpi ? "halt36.png" : "halt18.png"; break;
        case TextureID::sync:
            path = highdpi ? "sync36.png" : "sync18.png"; break;
        case TextureID::warp:
            path = highdpi ? "warp36.png" : "warp18.png"; break;
        case TextureID::syncLock:
            path = highdpi ? "sync_lock36.png" : "sync_lock18.png"; break;
        case TextureID::warpLock:
            path = highdpi ? "warp_lock36.png" : "warp_lock18.png"; break;
        case TextureID::none:
            path = highdpi ? "none36.png" : "none18.png"; break;
        case TextureID::mouse:
            path = highdpi ? "mouse36.png" : "mouse18.png"; break;
        case TextureID::joystick:
            path = highdpi ? "joystick36.png" : "joystick18.png"; break;
        case TextureID::keyset:
            path = highdpi ? "keyset36.png": "keyset18.png"; break;

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
