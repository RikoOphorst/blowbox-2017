#pragma once

#include "util/shared_ptr.h"
#include "util/vector.h"
#include "util/queue.h"
#include "core/scene/entity.h"
#include "renderer/cameras/camera.h"

namespace blowbox
{
    class DirectionalLight;
    class PointLight;
    class SpotLight;

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

        /**
        * @brief Sets the main Camera of this scene, i.e. the camera from where the scene is rendered.
        * @param[in] camera The camera that should act as the main Camera for this scene.
        */
        void SetMainCamera(SharedPtr<Camera> camera);

        /** @returns The main Camera of this scene. */
        SharedPtr<Camera> GetMainCamera();

        void AddDirectionalLight(SharedPtr<DirectionalLight> light);
        void AddPointLight(SharedPtr<PointLight> light);
        void AddSpotLight(SharedPtr<SpotLight> light);

        void RemoveDirectionalLight(SharedPtr<DirectionalLight> light);
        void RemovePointLight(SharedPtr<PointLight> light);
        void RemoveSpotLight(SharedPtr<SpotLight> light);

        Vector<SharedPtr<DirectionalLight>>& GetDirectionalLights();
        Vector<SharedPtr<PointLight>>& GetPointLights();
        Vector<SharedPtr<SpotLight>>& GetSpotLights();

    private:
        SharedPtr<Entity> root_entity_;                             //!< The root Entity in the scene.
        Vector<SharedPtr<Entity>> all_entities_;                    //!< All Entity instances in the scene.

        Queue<SharedPtr<Entity>> entities_to_be_added_;             //!< Queue for Entity instances that need to be added to the scene.
        Queue<SharedPtr<Entity>> entities_to_be_removed_;           //!< Queue for Entity instances that need to be removed from the scene.

        SharedPtr<Camera> main_camera_;                             //!< Stores the main camera for the scene.

        Vector<SharedPtr<DirectionalLight>> directional_lights_;    //!< All DirectionalLight instances in the scene.
        Vector<SharedPtr<PointLight>> point_lights_;                //!< All PointLight instances in the scene.
        Vector<SharedPtr<SpotLight>> spot_lights_;                  //!< All SpotLight instances in the scene.
    };
}