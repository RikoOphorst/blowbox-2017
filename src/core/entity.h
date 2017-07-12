#pragma once

#include "core/eastl.h"
#include <EASTL/shared_ptr.h>
#include <EASTL/vector.h>
#include <DirectXMath.h>

namespace blowbox
{
    class Entity
    {
        friend class SceneManager;
    public:
        Entity();
        Entity(eastl::shared_ptr<Entity> parent);
        ~Entity();

        void SetPosition(const DirectX::XMFLOAT3& position);
        void SetRotation(const DirectX::XMFLOAT3& rotation);
        void SetScaling(const DirectX::XMFLOAT3& scaling);
        const DirectX::XMFLOAT3& GetPosition() const;
        const DirectX::XMFLOAT3& GetRotation() const;
        const DirectX::XMFLOAT3& GetScaling() const;
        const DirectX::XMMATRIX& GetWorldTransform() const;

    protected:
        void Init();
        void Update();
        void Shutdown();

    private:
        eastl::shared_ptr<Entity> parent_;
        eastl::vector<eastl::shared_ptr<Entity>> children_;

        // Transform stuff
        DirectX::XMFLOAT3 position_;
        DirectX::XMFLOAT3 rotation_;
        DirectX::XMFLOAT3 scaling_;
        DirectX::XMMATRIX world_transform_;
        bool transform_dirty_;
    };
}