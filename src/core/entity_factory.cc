#include "entity_factory.h"

#include "core/get.h"
#include "core/entity.h"
#include "core/scene_manager.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    eastl::shared_ptr<Entity> EntityFactory::CreateEntity()
    {
        eastl::shared_ptr<Entity> entity(new Entity());
        Get::SceneManager()->AddEntity(entity);

        return entity;
    }

    //------------------------------------------------------------------------------------------------------
    eastl::shared_ptr<Entity> EntityFactory::CreateEntity(eastl::shared_ptr<Entity> parent)
    {
        eastl::shared_ptr<Entity> entity(new Entity(parent));
        Get::SceneManager()->AddEntity(entity);

        return entity;
    }
}