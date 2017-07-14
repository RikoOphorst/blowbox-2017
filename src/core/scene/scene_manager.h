#pragma once

#include "util/shared_ptr.h"
#include "util/vector.h"
#include "util/queue.h"
#include "core/scene/entity.h"

namespace blowbox
{
    /**
    * Manages the entire scene. It keeps a list of all Entity instances 
    * and updates them accordingly. It also keeps track of the root of
    * the scene graph. It also stores stuff like all the lights in the
    * scene and it also keeps track of the main Camera.
    *
    * @brief Manages the entire scene.
    */
    class SceneManager
    {
        friend class BlowboxCore;
        friend class EntityFactory;
        friend class Entity;
    public:
        /**
        * @brief Constructs the SceneManager.
        * @remarks Should only be constructed by the BlowboxCore. Do not construct yourself.
        */
        SceneManager();

        /** @brief Destructs the SceneManager. */
        ~SceneManager();

	protected:
        /** @brief Starts up the entire scene. */
        void Startup();

        /** @brief Updates the entire scene. */
        void Update();

        /** @brief PostUpdates the entire scene. */
        void PostUpdate();

        /** @brief Shuts down the entire scene. */
        void Shutdown();

        /**
        * @brief Adds an Entity and all of its children to the scene.
        * @param[in] entity The entity to add to the scene.
        */
        void AddEntity(SharedPtr<Entity> entity);

        /**
        * @brief Removes an Entity and all of its children from the scene.
        * @param[in] entity The entity to remove from the scene.
        */
        void RemoveEntity(SharedPtr<Entity> entity);

    public:
        /**
        * @brief Returns the root Entity.
        * @returns The root Entity in the scene.
        */
        SharedPtr<Entity> GetRootEntity();

        /**
        * @brief Returns all the Entity instances in the scene.
        * @returns All the Entity instances in the scene.
        */
        Vector<SharedPtr<Entity>>& GetEntities();

        /**
        * @brief Const version of SceneManager::GetEntities().
        * @returns All the Entity instances in the scene.
        */
        const Vector<SharedPtr<Entity>>& GetEntities() const;

    private:
        SharedPtr<Entity> root_entity_;                     //!< The root Entity in the scene.
        Vector<SharedPtr<Entity>> all_entities_;            //!< All Entity instances in the scene.

        Queue<SharedPtr<Entity>> entities_to_be_added_;     //!< Queue for Entity instances that need to be added to the scene.
        Queue<SharedPtr<Entity>> entities_to_be_removed_;   //!< Queue for Entity instances that need to be removed from the scene.
    };
}