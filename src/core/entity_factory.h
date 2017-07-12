#pragma once

#include "core/eastl.h"
#include "EASTL/shared_ptr.h"

namespace blowbox
{
    class Entity;

    class EntityFactory
    {
    public:
        static eastl::shared_ptr<Entity> CreateEntity();
        static eastl::shared_ptr<Entity> CreateEntity(eastl::shared_ptr<Entity> parent);
    };
}