#pragma once

#include "util/shared_ptr.h"

namespace blowbox
{
    class Entity;

    class EntityFactory
    {
    public:
        static SharedPtr<Entity> CreateEntity();
        static SharedPtr<Entity> CreateEntity(SharedPtr<Entity> parent);
    };
}