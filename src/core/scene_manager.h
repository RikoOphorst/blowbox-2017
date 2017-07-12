#pragma once

#include "core/eastl.h"
#include <EASTL/shared_ptr.h>
#include <EASTL/vector.h>
#include <EASTL/queue.h>
#include "core/entity.h"

namespace blowbox
{
    class SceneManager
    {
        friend class BlowboxCore;
        friend class EntityFactory;
    protected:
        SceneManager();
        ~SceneManager();

        void Update();
        void PostUpdate();

    public:
        void AddEntity(eastl::shared_ptr<Entity> entity);
        void RemoveEntity(eastl::shared_ptr<Entity> entity);

        eastl::shared_ptr<Entity>& GetRootEntity();
        const eastl::shared_ptr<Entity>& GetRootEntity() const;
        eastl::vector<eastl::shared_ptr<Entity>>& GetEntities();
        const eastl::vector<eastl::shared_ptr<Entity>>& GetEntities() const;

    private:
        eastl::shared_ptr<Entity> root_entity_;
        eastl::vector<eastl::shared_ptr<Entity>> all_entities_;

        eastl::queue<eastl::shared_ptr<Entity>> entities_to_be_added_;
        eastl::queue<eastl::shared_ptr<Entity>> entities_to_be_removed_;
    };
}