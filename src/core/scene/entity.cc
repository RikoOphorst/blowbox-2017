#include "entity.h"

#include "core/scene/scene_manager.h"
#include "core/get.h"
#include "core/debug/performance_profiler.h"
#include "renderer/materials/material.h"
#include "core/scene/entity_factory.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    Entity::Entity(const String& name) :
        position_(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
        rotation_(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)),
        scaling_(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f)),
        world_transform_(DirectX::XMMatrixIdentity()),
        transform_dirty_(true),
        is_visible_(true),
        in_scene_(false),
        name_(name)
    {
        constant_buffer_.Create(L"ConstantBuffer", 3, sizeof(DirectX::XMMATRIX));
    }
    
    //------------------------------------------------------------------------------------------------------
    Entity::~Entity()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void Entity::SetName(const String& name)
    {
        name_ = name;
    }

    //------------------------------------------------------------------------------------------------------
    const String& Entity::GetName() const
    {
        return name_;
    }

	//------------------------------------------------------------------------------------------------------
	void Entity::SetLocalPosition(const DirectX::XMFLOAT3& position)
    {
		position_ = position;
        EntityFactory::MakeEntityGraphDirty(this);
    }

	//------------------------------------------------------------------------------------------------------
	void Entity::SetLocalRotation(const DirectX::XMFLOAT3& rotation)
    {
		rotation_ = rotation;
        EntityFactory::MakeEntityGraphDirty(this);
    }

	//------------------------------------------------------------------------------------------------------
	void Entity::SetLocalScaling(const DirectX::XMFLOAT3& scaling)
    {
		scaling_ = scaling;
        EntityFactory::MakeEntityGraphDirty(this);
    }

    //------------------------------------------------------------------------------------------------------
    void Entity::SetMesh(SharedPtr<Mesh> mesh)
    {
        mesh_ = mesh;
    }

    //------------------------------------------------------------------------------------------------------
    void Entity::SetVisible(bool visibility)
    {
        is_visible_ = visibility;
    }

    //------------------------------------------------------------------------------------------------------
    void Entity::SetMaterial(WeakPtr<Material> material)
    {
        material_ = material;
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
    SharedPtr<Mesh> Entity::GetMesh() const
    {
        return mesh_;
    }

    //------------------------------------------------------------------------------------------------------
    bool Entity::GetVisible() const
    {
        return is_visible_;
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Material> Entity::GetMaterial() const
    {
        return material_;
    }

	//------------------------------------------------------------------------------------------------------
	const DirectX::XMMATRIX& Entity::GetWorldTransform()
    {
		if (IsTransformDirty())
		{
			UpdateWorldTransform();
		}

		return world_transform_;
    }

    //------------------------------------------------------------------------------------------------------
    const Vector<SharedPtr<Entity>>& Entity::GetChildren() const
    {
        return children_;
    }

    //------------------------------------------------------------------------------------------------------
    void Entity::Init()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void Entity::Update()
    {
		if (IsTransformDirty())
		{
			UpdateWorldTransform();
		}
    }

    //------------------------------------------------------------------------------------------------------
    void Entity::Shutdown()
    {

    }

    //------------------------------------------------------------------------------------------------------
    bool Entity::AddChild(SharedPtr<Entity> entity)
    {
#ifdef _DEBUG
        for (int i = 0; i < children_.size(); i++)
        {
            if (children_[i] == entity)
            {
                return false;
            }
        }
#endif

        children_.push_back(entity);
        return true;
    }

    //------------------------------------------------------------------------------------------------------
    bool Entity::RemoveChild(SharedPtr<Entity> entity)
    {
        for (int i = 0; i < children_.size(); i++)
        {
            if (children_[i] == entity)
            {
                children_.erase(children_.begin() + i);
                return true;
            }
        }

        return false;
    }

    //------------------------------------------------------------------------------------------------------
    void Entity::SetInScene(bool in_scene)
    {
        in_scene_ = in_scene;

        for (int i = 0; i < children_.size(); i++)
        {
            children_[i]->SetInScene(in_scene);
        }
    }

    //------------------------------------------------------------------------------------------------------
    bool Entity::GetInScene() const
    {
        return in_scene_;
    }
	
	//------------------------------------------------------------------------------------------------------
	bool Entity::IsTransformDirty()
	{
		return transform_dirty_ || (parent_.expired() ? false : parent_.lock()->IsTransformDirty());
	}
	
	//------------------------------------------------------------------------------------------------------
	void Entity::UpdateWorldTransform()
	{
        PerformanceProfiler::ProfilerBlock block("Entity::UpdateWorldTransform", ProfilerBlockType_CORE);

		world_transform_ =
			(parent_.lock() != nullptr ? parent_.lock()->GetWorldTransform() : DirectX::XMMatrixIdentity()) *
			DirectX::XMMatrixScaling(scaling_.x, scaling_.y, scaling_.z) *
			DirectX::XMMatrixRotationRollPitchYaw(rotation_.x, rotation_.y, rotation_.z) *
			DirectX::XMMatrixTranslation(position_.x, position_.y, position_.z);

        transform_dirty_ = false;
	}
}