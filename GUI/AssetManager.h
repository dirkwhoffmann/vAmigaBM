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

enum class FontID
{
    console,
    sans_l,
    sans_r,
    sans_sb
};

enum class ShaderID
{
    blur
};

template <typename Asset, typename ID>
class AssetManager
{
protected:
    
    // Asset storage
    std::map<ID, std::unique_ptr<Asset>> cache;

public:
        
    // Requests an asset
    Asset& get(ID id)
    {
        // Lookup the requested element
        auto it = cache.find(id);
        
        // Load the element if it is not cached
        if (it == cache.end()) {
            load(id);
            it = cache.find(id);
            assert(it != cache.end());
        }
        
        return *it->second;
    }
    
    // Caches an asset
    virtual void load(ID id) = 0;
};

class TextureManager : public AssetManager<sf::Texture, TextureID> {
    void load(TextureID id) override;
};

class FontManager : public AssetManager<sf::Font, FontID> {
    void load(FontID id) override;
};

class ShaderManager : public AssetManager<sf::Shader, ShaderID> {
    void load(ShaderID id) override;
};

class Assets {
    
    static TextureManager textures;
    static FontManager fonts;
    static ShaderManager shaders;
    
public:
    
    static sf::Texture& get(TextureID id) { return textures.get(id); }
    static sf::Font& get(FontID id) { return fonts.get(id); }
    static sf::Shader& get(ShaderID id) { return shaders.get(id); }
};
