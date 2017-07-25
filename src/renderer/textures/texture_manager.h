#pragma once

#include "util/unordered_map.h"
#include "util/shared_ptr.h"
#include "util/weak_ptr.h"
#include "util/string.h"
#include "renderer/textures/texture.h"

namespace blowbox
{
    /**
    * The TextureManager allows you to have re-usable instances of Textures
    * globally accessible by name throughout the application. You should
    * probably use it.
    *
    * @brief Manages global Texture instances.
    */
    class TextureManager
    {
    public:
        TextureManager();
        ~TextureManager();

        /** @brief Starts up the TextureManager. */
        void Startup();

        /** @brief Starts a new frame in the TextureManager. */
        void NewFrame();

        /** @brief Shuts down the TextureManager. */
        void Shutdown();

        /**
        * @brief Adds a Texture to the TextureManager.
        * @param[in] name The name of the Texture to be added.
        * @remarks If an entry with that name already exists, all settings from the to-be-added Texture will be copied into the Texture instance that was already stored in the TextureManager.
        */
        WeakPtr<Texture> AddTexture(const String& name, SharedPtr<Texture> texture);

        /**
        * @brief Removes a Texture from the TextureManager. Asserts if there are still dependants on the Texture.
        * @param[in] name The name of the Texture to be removed.
        */
        void RemoveTexture(const String& name);

        /**
        * @brief Access a Texture from the TextureManager. If the Texture doesn't exist in the TextureManager, a default one with the searched-for name will be added and returned.
        * @param[in] name The name of the Texture to be added.
        */
        WeakPtr<Texture> GetTexture(const String& name);

        /** 
        * @param[in] name The name of the Texture to check.
        * @returns Whether the Texture has been loaded.
        */
        bool HasBeenLoaded(const String& name);

    private:
        UnorderedMap<String, SharedPtr<Texture>> textures_;   //!< All Textures stored in the TextureManager.
    };
}