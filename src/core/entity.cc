#include "entity.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Entity::Entity() :
        position_(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
        rotation_(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
        scaling_(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)),
        world_transform_(DirectX::XMMatrixIdentity()),
        transform_dirty_(true)
    {

    }

    //------------------------------------------------------------------------------------------------------
    Entity::Entity(eastl::shared_ptr<Entity> parent) :
        position_(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
        rotation_(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
        scaling_(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)),
        world_transform_(DirectX::XMMatrixIdentity()),
        transform_dirty_(true),
        parent_(parent)
    {
        parent_ = parent;
    }
    
    //------------------------------------------------------------------------------------------------------
    Entity::~Entity()
    {

    }

    void Entity::SetPosition(const DirectX::XMFLOAT3 & position)
    {
    }

    void Entity::SetRotation(const DirectX::XMFLOAT3 & rotation)
    {
    }

    void Entity::SetScaling(const DirectX::XMFLOAT3 & scaling)
    {
    }

    const DirectX::XMFLOAT3 & Entity::GetPosition() const
    {
        // TODO: insert return statement here
    }

    const DirectX::XMFLOAT3 & Entity::GetRotation() const
    {
        // TODO: insert return statement here
    }

    const DirectX::XMFLOAT3 & Entity::GetScaling() const
    {
        // TODO: insert return statement here
    }

    const DirectX::XMMATRIX & Entity::GetWorldTransform() const
    {
        // TODO: insert return statement here
    }

    //------------------------------------------------------------------------------------------------------
    void Entity::Init()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void Entity::Update()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void Entity::Shutdown()
    {

    }
}