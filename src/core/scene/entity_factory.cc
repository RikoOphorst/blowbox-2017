#include "entity_factory.h"

#include "core/get.h"
#include "core/scene/entity.h"
#include "core/scene/scene_manager.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    SharedPtr<Entity> EntityFactory::CreateEntity()
    {
        SharedPtr<Entity> entity(new Entity());
        Get::SceneManager()->AddEntity(entity);

        return entity;
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<Entity> EntityFactory::CreateEntity(SharedPtr<Entity> parent)
    {
        SharedPtr<Entity> entity(new Entity(parent));
        Get::SceneManager()->AddEntity(entity);

        return entity;
    }
}