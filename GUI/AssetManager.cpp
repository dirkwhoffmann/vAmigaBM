// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "AssetManager.h"

TextureManager Assets::textures;
ShaderManager Assets::shaders;

void
TextureManager::load(TextureID id)
{
    std::string path = "";
    
    switch (id) {
            
        case TextureID::logo:     path = "logo.png"; break;
        case TextureID::concrete: path = "concrete.png"; break;
            
        default:
            assert(false);
    }
    
    std::unique_ptr<sf::Texture> asset(new sf::Texture());
    
    if (asset->loadFromFile(path)) {
        cache.insert(std::make_pair(id, std::move(asset)));
    } else {
        throw std::runtime_error("AssetManager::loadTexture: " + path);
    }
}

void
ShaderManager::load(ShaderID id)
{
    assert(false);
}

