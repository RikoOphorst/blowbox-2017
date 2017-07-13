#pragma once

#include "util/shared_ptr.h"
#include "util/vector.h"
#include "util/queue.h"
#include "core/scene/entity.h"

namespace blowbox
{
    class SceneManager
    {
        friend class BlowboxCore;
        friend class EntityFactory;
    public:
        SceneManager();
        ~SceneManager();

	protected:
        void Update();
        void PostUpdate();

    public:
        void AddEntity(SharedPtr<Entity> entity);
        void RemoveEntity(SharedPtr<Entity> entity);

        SharedPtr<Entity>& GetRootEntity();
        const SharedPtr<Entity>& GetRootEntity() const;
        Vector<SharedPtr<Entity>>& GetEntities();
        const Vector<SharedPtr<Entity>>& GetEntities() const;

    private:
        SharedPtr<Entity> root_entity_;
        Vector<SharedPtr<Entity>> all_entities_;

        Queue<SharedPtr<Entity>> entities_to_be_added_;
        Queue<SharedPtr<Entity>> entities_to_be_removed_;
    };
}