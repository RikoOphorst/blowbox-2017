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

	//------------------------------------------------------------------------------------------------------
	void Entity::SetLocalPosition(const DirectX::XMFLOAT3& position)
    {
		position_ = position;
		transform_dirty_ = true;
    }

	//------------------------------------------------------------------------------------------------------
	void Entity::SetLocalRotation(const DirectX::XMFLOAT3& rotation)
    {
		rotation_ = rotation;
		transform_dirty_ = true;
    }

	//------------------------------------------------------------------------------------------------------
	void Entity::SetLocalScaling(const DirectX::XMFLOAT3& scaling)
    {
		scaling_ = scaling;
		transform_dirty_ = true;
    }

	//------------------------------------------------------------------------------------------------------
	const DirectX::XMFLOAT3& Entity::GetLocalPosition() const
    {
		return position_;
    }

	//------------------------------------------------------------------------------------------------------
	const DirectX::XMFLOAT3& Entity::GetLocalRotation() const
    {
		return rotation_;
    }

	//------------------------------------------------------------------------------------------------------
	const DirectX::XMFLOAT3& Entity::GetLocalScaling() const
    {
		return scaling_;
    }

	//------------------------------------------------------------------------------------------------------
	const DirectX::XMMATRIX& Entity::GetWorldTransform()
    {
		if (transform_dirty_)
		{
			UpdateWorldTransform();
		}

		return world_transform_;
    }

    //------------------------------------------------------------------------------------------------------
    void Entity::Init()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void Entity::Update()
    {
		if (transform_dirty_)
		{
			UpdateWorldTransform();
		}
    }

    //------------------------------------------------------------------------------------------------------
    void Entity::Shutdown()
    {

    }
	
	//------------------------------------------------------------------------------------------------------
	bool Entity::IsTransformDirty()
	{
		return transform_dirty_;
	}
	
	//------------------------------------------------------------------------------------------------------
	void Entity::UpdateWorldTransform()
	{
		world_transform_ =
			(parent_ != nullptr ? parent_->GetWorldTransform() : DirectX::XMMatrixIdentity()) *
			DirectX::XMMatrixScaling(scaling_.x, scaling_.y, scaling_.z) *
			DirectX::XMMatrixRotationRollPitchYaw(rotation_.x, rotation_.y, rotation_.z) *
			DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);
	}
}