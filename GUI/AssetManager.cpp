// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "Application.h"

void
TextureManager::load(TextureID id)
{
    std::string path = "";
    
    switch (id) {
            
        case TextureID::krunch: path = "krunch.png"; break;
        case TextureID::logo: path = "logo.png"; break;
        case TextureID::title: path = "title.png"; break;

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
    std::string path = "";
    
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
    std::string path = "";
    
    switch (id) {
            
        case SoundID::insert: path = "insert.aiff"; break;
        case SoundID::eject:  path = "eject.aiff"; break;
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
    std::string vert = "";
    std::string frag = "";
    
    switch (id) {
            
        case ShaderID::blur: frag = "blur.frag"; break;
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
