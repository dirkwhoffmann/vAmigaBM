// -----------------------------------------------------------------------------
// This file is part of vAmiga Bare Metal
//
// Copyright (C) Dirk W. Hoffmann. www.dirkwhoffmann.de
// Licensed under the GNU General Public License v3
//
// See https://www.gnu.org for license information
// -----------------------------------------------------------------------------

#pragma once

#include <SFML/Graphics.hpp>
#include <map>

enum class TextureID
{
    logo
};

class AssetManager
{
    // Singleton object
    static AssetManager *manager;
    
    // Asset storages
    std::map<TextureID, std::unique_ptr<sf::Texture>> textureCache;
    
public:
    AssetManager();
    
    // Returns an asset of a specific type
    static sf::Texture& texture(TextureID id);

    // Caches an asset of a specific type
    static void loadTexture(TextureID id);
};
