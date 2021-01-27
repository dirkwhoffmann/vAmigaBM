// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#include "AssetManager.h"

AssetManager *AssetManager::manager = nullptr;
AssetManager assetManager;

AssetManager::AssetManager()
{
    // This class is a singleton
    assert(manager == nullptr);
    manager = this;
}

sf::Texture&
AssetManager::texture(TextureID id)
{
    auto& map = manager->textureCache;
    
    // Lookup the requested element
    auto it = map.find(id);

    // Load the element if it is not cached
    if (it == map.end()) {
        loadTexture(id);
        it = map.find(id);
        assert(it != map.end());
    }
        
    return *it->second;
}

void
AssetManager::loadTexture(TextureID id)
{
    auto& map = manager->textureCache;
    
    std::string path = "";
    
    switch (id) {
            
        case TextureID::logo: path = "logo.png"; break;

        default:
            assert(false);
    }
        
    std::unique_ptr<sf::Texture> asset(new sf::Texture());
    if (!asset->loadFromFile(path)) {
        throw std::runtime_error("AssetManager::loadTexture: " + path);
    }
    
    auto inserted = map.insert(std::make_pair(id, std::move(asset)));
    assert(inserted.second);
}
