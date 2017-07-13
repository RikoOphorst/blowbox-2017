#pragma once

#include "util/shared_ptr.h"
#include "util/vector.h"
#include <DirectXMath.h>

namespace blowbox
{
    class Entity
    {
        friend class SceneManager;
    public:
        Entity();
        Entity(SharedPtr<Entity> parent);
        ~Entity();

        void SetLocalPosition(const DirectX::XMFLOAT3& position);
        void SetLocalRotation(const DirectX::XMFLOAT3& rotation);
        void SetLocalScaling(const DirectX::XMFLOAT3& scaling);
        const DirectX::XMFLOAT3& GetLocalPosition() const;
        const DirectX::XMFLOAT3& GetLocalRotation() const;
        const DirectX::XMFLOAT3& GetLocalScaling() const;
        const DirectX::XMMATRIX& GetWorldTransform();

    protected:
        void Init();
        void Update();
        void Shutdown();

	protected:
		bool IsTransformDirty();
		void UpdateWorldTransform();

    private:
        SharedPtr<Entity> parent_;
        Vector<SharedPtr<Entity>> children_;

        // Transform stuff
        DirectX::XMFLOAT3 position_;
        DirectX::XMFLOAT3 rotation_;
        DirectX::XMFLOAT3 scaling_;
        DirectX::XMMATRIX world_transform_;
        bool transform_dirty_;
    };
}