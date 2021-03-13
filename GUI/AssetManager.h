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
#include <SFML/Audio.hpp>
#include <map>

enum class TextureID
{
    logo,
    title,

    disk,
    diskProtected,
    diskLarge,
    spin,
    ledBlack,
    ledGrey,
    ledGreen,
    ledRed,
    mute,
    halt,
    sync,
    warp,
    syncLock,
    warpLock,
    none,
    mouse,
    joystick,
    keyset
};

enum class FontID
{
    console,
    sans_l,
    sans_r,
    sans_sb,
    sans_b,
    sans_bk,
};

enum class SoundID
{
    insert,
    eject,
    click
};

enum class ShaderID
{
    merge,
    mergeBypass
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

class SoundManager : public AssetManager<sf::SoundBuffer, SoundID> {
    void load(SoundID id) override;
};

class ShaderManager : public AssetManager<sf::Shader, ShaderID> {
    void load(ShaderID id) override;
};

class Assets {
    
    TextureManager textures;
    FontManager fonts;
    SoundManager sounds;
    ShaderManager shaders;
    
public:
    
    sf::Texture &get(TextureID id) { return textures.get(id); }
    sf::Font &get(FontID id) { return fonts.get(id); }
    sf::SoundBuffer &get(SoundID id) { return sounds.get(id); }
    sf::Shader &get(ShaderID id) { return shaders.get(id); }
};
