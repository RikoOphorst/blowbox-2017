#include "material_manager.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    MaterialManager::MaterialManager()
    {

    }

    //------------------------------------------------------------------------------------------------------
    MaterialManager::~MaterialManager()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    void MaterialManager::Startup()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void MaterialManager::NewFrame()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void MaterialManager::Shutdown()
    {

    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Material> MaterialManager::AddMaterial(const String& name, SharedPtr<Material> material)
    {
        auto it = materials_.find(name);

        if (it == materials_.end())
        {
            materials_[name] = material;
            return materials_[name];
        }
        else
        {
            SharedPtr<Material>& stored_material = it->second;
            *stored_material = *material;
            stored_material->SetName(name);

            return stored_material;
        }
    }

    //------------------------------------------------------------------------------------------------------
    void MaterialManager::RemoveMaterial(const String& name)
    {
        auto it = materials_.find(name);

        if (it != materials_.end())
        {
            BLOWBOX_ASSERT(it->second.use_count() == 0);
            materials_.erase(it);
        }
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Material> MaterialManager::GetMaterial(const String& name)
    {
        auto it = materials_.find(name);

        if (it != materials_.end())
        {
            return it->second;
        }
        else
        {
            return AddMaterial(name, eastl::make_shared<Material>());
        }
    }
}