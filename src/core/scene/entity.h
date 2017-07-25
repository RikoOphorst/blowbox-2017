#pragma once

#include "util/shared_ptr.h"
#include "util/weak_ptr.h"
#include "util/vector.h"
#include "util/queue.h"
#include "util/string.h"
#include "renderer/meshes/mesh.h"
#include "renderer/buffers/upload_buffer.h"
#include <DirectXMath.h>

namespace blowbox
{
    class Material;

    /**
    * An Entity is the Blowbox equivalent of a game object. It has
    * everything in it that you need. There are no components. Every
    * Entity has the same layout and same functionality: it allows you
    * to render something into the scene. It contains a transform, and
    * it is part of a scene graph. It is recommended that you construct
    * an Entity through the EntityFactory instead of doing it through
    * the Entity::Entity() constructor. However, it is very much possible
    * to create an Entity through its constructor.
    *
    * @brief The Blowbox equivalent of a game object.
    */
    class Entity
    {
        friend class SceneManager;
        friend class EntityFactory;
    public:
        /** 
        * @brief Constructs an Entity. 
        * @param[in] name The name of this Entity.
        */
        Entity(const String& name);

        /** @brief Destructs the Entity. */
        ~Entity();

        /**
        * @brief Sets the name of this Entity.
        * @param[in] name The new name of this Entity.
        */
        void SetName(const String& name);

        /**
        * @brief Returns the name of this Entity.
        * @returns The name of this Entity.
        */
        const String& GetName() const;

        /**
        * @brief Sets the local position of this Entity.
        * @param[in] position The new local position for this Entity.
        */
        void SetLocalPosition(const DirectX::XMFLOAT3& position);

        /**
        * @brief Sets the local rotation of this Entity.
        * @param[in] rotation The new local rotation for this Entity.
        */
        void SetLocalRotation(const DirectX::XMFLOAT3& rotation);

        /**
        * @brief Sets the local scaling of this Entity.
        * @param[in] scaling The new local scaling for this Entity.
        */
        void SetLocalScaling(const DirectX::XMFLOAT3& scaling);

        /**
        * @brief Sets the Mesh of this Entity.
        * @param[in] mesh The mesh that you want to bind to this Entity.
        */
        void SetMesh(SharedPtr<Mesh> mesh);

        /**
        * @brief Sets the visibility of this Entity.
        * @param[in] visibility The visibility of this Entity.
        */
        void SetVisible(bool visibility);

        /**
        * @brief Sets the Material of this Entity.
        * @param[in] material The material that you want to bind to this Entity.
        */
        void SetMaterial(WeakPtr<Material> material);

        /** @returns The local position of this Entity. */
        const DirectX::XMFLOAT3& GetLocalPosition() const;

        /** @returns The local rotation of this Entity. */
        const DirectX::XMFLOAT3& GetLocalRotation() const;

        /** @returns The local scaling of this Entity. */
        const DirectX::XMFLOAT3& GetLocalScaling() const;

        /** @returns The Mesh that is bound to this Entity. */
        SharedPtr<Mesh> GetMesh() const;
        
        /** @returns Whether this Entity is visible. */
        bool GetVisible() const;

        /** @returns The Material that is bound to this Entity. */
        WeakPtr<Material> GetMaterial() const;

        /**
        * @brief Returns the world transform of this Entity.
        * @returns The world transform of this Entity.
        * @remarks This function is not marked const because it might calculate the world transform upon call of this function, depending on whether the current transform is dirty.
        */
        const DirectX::XMMATRIX& GetWorldTransform();

        /** @returns The children of this Entity. */
        const Vector<SharedPtr<Entity>>& GetChildren() const;

        /** @returns This Entity's constant buffer. */
        UploadBuffer& GetConstantBuffer() { return constant_buffer_; };

    protected:
        /** @brief Initialises the Entity. */
        void Init();

        /** @brief Updates the Entity. */
        void Update();

        /** @brief Shuts down the Entity. */
        void Shutdown();

        /**
        * @brief Adds a child Entity to this Entity.
        * @param[in] entity The child Entity that should be added to this Entity
        * @returns Whether the child Entity was successfully added
        */
        bool AddChild(SharedPtr<Entity> entity);

        /**
        * @brief Removes a child Entity from this Entity.
        * @param[in] entity The child Entity that should be removed from this Entity
        * @returns Whether the child Entity was successfully removed
        */
        bool RemoveChild(SharedPtr<Entity> entity);

        /**
        * @brief Sets whether the Entity is present in the SceneManager.
        * @param[in] in_scene Whether the Entity exists in the SceneManager.
        */
        void SetInScene(bool in_scene);

        /**
        * @brief Returns whether the Entity is present in the SceneManager.
        * @returns Whether the Entity exists in the SceneManager.
        */
        bool GetInScene() const;

	protected:
        /** 
        * @brief Returns whether the transform is dirty. 
        * @returns Whether the transform is dirty. 
        */
		bool IsTransformDirty();

        /** @brief Updates the world transform based on current position, rotation and scaling. */
		void UpdateWorldTransform();

    private:
        String name_;                           //!< The name of this Entity.
        WeakPtr<Entity> parent_;                //!< The parent of this Entity.
        Vector<SharedPtr<Entity>> children_;    //!< All the children of this Entity.

        // Transform stuff
        DirectX::XMFLOAT3 position_;            //!< The local position of this Entity.
        DirectX::XMFLOAT3 rotation_;            //!< The local rotation of this Entity.
        DirectX::XMFLOAT3 scaling_;             //!< The local scaling of this Entity.
        DirectX::XMMATRIX world_transform_;     //!< The world transform of this Entity.
        bool transform_dirty_;                  //!< Whether the current world_transform_ is dirty (i.e. position/rotation/scaling changed).

        bool in_scene_;                         //!< Flag that determines whether this Entity exists in the SceneManager.
        bool is_visible_;                       //!< Whether this Entity is visible in the scene (i.e. being rendered).

        SharedPtr<Mesh> mesh_;                  //!< The Mesh object that is attached to this Entity.
        UploadBuffer constant_buffer_;          //!< This Entity's constant buffer.

        WeakPtr<Material> material_;            //<! The Material this Entity's Mesh should be rendered with.
    };
}