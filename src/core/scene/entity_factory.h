#pragma once

#include "util/shared_ptr.h"
#include "util/string.h"

namespace blowbox
{
    class Entity;

    /**
    * A Factory pattern for creating Entity instances and creating 
    * parental links between entities.
    * 
    * @brief Factory for creating Entity instances and managing Entity children.
    */
    class EntityFactory
    {
    public:
        /**
        * @brief Creates a parentless Entity instance
        * @param[in] name The name of the new Entity.
        */
        static SharedPtr<Entity> CreateEntity(const String& name);

        /**
        * @brief Creates an Entity instance with a parent.
        * @param[in] name The name of the new Entity.
        * @param[in] parent The parent of the new Entity.
        */
        static SharedPtr<Entity> CreateEntity(const String& name, SharedPtr<Entity> parent);

        /**
        * @brief Adds a child to an Entity.
        * @param[in] entity The entity to which you want to add a child
        * @param[in] child The child you want to add to the entity
        */
        static void AddChildToEntity(SharedPtr<Entity> entity, SharedPtr<Entity> child);

        /**
        * @brief Remove a child from an Entity.
        * @param[in] entity The entity from which you want to remove a child
        * @param[in] child The child you want to remove from the entity
        */
        static void RemoveChildFromEntity(SharedPtr<Entity> entity, SharedPtr<Entity> child);
    };
}