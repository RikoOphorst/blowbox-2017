#pragma once

#include "util/unordered_map.h"
#include "util/shared_ptr.h"
#include "util/weak_ptr.h"
#include "util/string.h"
#include "renderer/materials/material.h"

namespace blowbox
{
    /**
    * The MaterialManager allows you to have re-usable instances of Materials 
    * globally accessible by name throughout the application. You should 
    * probably use it.
    *
    * @brief Manages global Material instances.
    */
    class MaterialManager
    {
        friend class MaterialList;
    public:
        MaterialManager();
        ~MaterialManager();

        /** @brief Starts up the MaterialManager. */
        void Startup();
        
        /** @brief Starts a new frame in the MaterialManager. */
        void NewFrame();
        
        /** @brief Shuts down the MaterialManager. */
        void Shutdown();

        /**
        * @brief Adds a Material to the MaterialManager. 
        * @param[in] name The name of the Material to be added.
        * @param[in] material The Material to be added.
        * @remarks If an entry with that name already exists, all settings from the to-be-added Material will be copied into the Material instance that was already stored in the MaterialManager.
        */
        WeakPtr<Material> AddMaterial(const String& name, SharedPtr<Material> material);

        /**
        * @brief Removes a Material from the MaterialManager. Asserts if there are still dependants on the Material.
        * @param[in] name The name of the Material to be removed.
        */
        void RemoveMaterial(const String& name);

        /**
        * @brief Access a Material from the MaterialManager. If the Material doesn't exist in the MaterialManager, a default one with the searched-for name will be added and returned.
        * @param[in] name The name of the Material to be added.
        */
        WeakPtr<Material> GetMaterial(const String& name);

    private:
        UnorderedMap<String, SharedPtr<Material>> materials_;   //!< All Materials stored in the MaterialManager.
    };
}