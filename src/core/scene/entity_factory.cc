#include "entity_factory.h"

#include "core/get.h"
#include "core/scene/entity.h"
#include "core/scene/scene_manager.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    SharedPtr<Entity> EntityFactory::CreateEntity(const String& name)
    {
        return eastl::make_shared<Entity>(name);
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<Entity> EntityFactory::CreateEntity(const String& name, SharedPtr<Entity> parent)
    {
        SharedPtr<Entity> new_entity = eastl::make_shared<Entity>(name);
        AddChildToEntity(parent, new_entity);

        return new_entity;
    }
    
    //------------------------------------------------------------------------------------------------------
    void EntityFactory::AddChildToEntity(SharedPtr<Entity> entity, SharedPtr<Entity> child)
    {
        bool add_succeeded = entity->AddChild(child);

        if (add_succeeded)
        {
            child->parent_ = entity;
            MakeEntityGraphDirty(child.get());
        }

        if (entity->GetInScene() == true && child->GetInScene() == false)
        {
            Get::SceneManager()->AddEntity(child);
        }
    }
    
    //------------------------------------------------------------------------------------------------------
    void EntityFactory::RemoveChildFromEntity(SharedPtr<Entity> entity, SharedPtr<Entity> child)
    {
        bool remove_succeeded = entity->RemoveChild(child);

        if (remove_succeeded == true)
        {
            MakeEntityGraphDirty(child.get());

            if (child->GetInScene() == true)
            {
                Get::SceneManager()->RemoveEntity(child);
            }
        }
    }
    
    //------------------------------------------------------------------------------------------------------
    void EntityFactory::MakeEntityGraphDirty(Entity* entity)
    {
        entity->transform_dirty_ = true;

        for (int i = 0; i < entity->children_.size(); i++)
        {
            MakeEntityGraphDirty(entity->children_[i].get());
        }
    }
}