#include "scene_manager.h"

#include "core/debug/performance_profiler.h"
#include "core/scene/entity_factory.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    SceneManager::SceneManager()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    SceneManager::~SceneManager()
    {

    }
    
    //------------------------------------------------------------------------------------------------------
    void SceneManager::Startup()
    {
        root_entity_ = EntityFactory::CreateEntity("RootEntity");
        root_entity_->SetInScene(true);
        all_entities_.push_back(root_entity_);
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::Update()
    {
        PerformanceProfiler::ProfilerBlock block("SceneManager::Update", ProfilerBlockType_CORE);
        for (int i = 0; i < all_entities_.size(); i++)
        {
            all_entities_[i]->Update();
        }
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::PostUpdate()
    {
        while (!entities_to_be_removed_.empty())
        {
            SharedPtr<Entity> entity = entities_to_be_removed_.front();

            for (int i = 0; i < all_entities_.size(); i++) 
            {
                if (all_entities_[i] == entity)
                {
                    all_entities_[i]->SetInScene(false);
                    all_entities_.erase(all_entities_.begin() + i);
                    break;
                }
            }

            entities_to_be_removed_.pop();
        }

        while (!entities_to_be_added_.empty())
        {
            SharedPtr<Entity> entity = entities_to_be_added_.front();
            entity->SetInScene(true);
            all_entities_.push_back(entity);
            entities_to_be_added_.pop();
        }
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::Shutdown()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::AddEntity(SharedPtr<Entity> entity)
    {
        entities_to_be_added_.push(entity);

        for (int i = 0; i < entity->children_.size(); i++)
        {
            AddEntity(entity->children_[i]);
        }
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::RemoveEntity(SharedPtr<Entity> entity)
    {
        entities_to_be_removed_.push(entity);

        for (int i = 0; i < entity->children_.size(); i++)
        {
            RemoveEntity(entity->children_[i]);
        }
    }
    
    //------------------------------------------------------------------------------------------------------
    void SceneManager::SetMainCamera(SharedPtr<Camera> camera)
    {
        main_camera_ = camera;
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<Camera> SceneManager::GetMainCamera()
    {
        return main_camera_;
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::AddDirectionalLight(SharedPtr<DirectionalLight> light)
    {
        directional_lights_.push_back(light);
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::AddPointLight(SharedPtr<PointLight> light)
    {
        point_lights_.push_back(light);
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::AddSpotLight(SharedPtr<SpotLight> light)
    {
        spot_lights_.push_back(light);
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::RemoveDirectionalLight(SharedPtr<DirectionalLight> light)
    {
        for (int i = 0; i < directional_lights_.size(); i++)
        {
            if (directional_lights_[i] == light)
            {
                directional_lights_.erase(directional_lights_.begin() + i);
                break;
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::RemovePointLight(SharedPtr<PointLight> light)
    {
        for (int i = 0; i < point_lights_.size(); i++)
        {
            if (point_lights_[i] == light)
            {
                point_lights_.erase(point_lights_.begin() + i);
                break;
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::RemoveSpotLight(SharedPtr<SpotLight> light)
    {
        for (int i = 0; i < spot_lights_.size(); i++)
        {
            if (spot_lights_[i] == light)
            {
                spot_lights_.erase(spot_lights_.begin() + i);
                break;
            }
        }
    }

    //------------------------------------------------------------------------------------------------------
    Vector<SharedPtr<DirectionalLight>>& SceneManager::GetDirectionalLights()
    {
        return directional_lights_;
    }

    //------------------------------------------------------------------------------------------------------
    Vector<SharedPtr<PointLight>>& SceneManager::GetPointLights()
    {
        return point_lights_;
    }

    //------------------------------------------------------------------------------------------------------
    Vector<SharedPtr<SpotLight>>& SceneManager::GetSpotLights()
    {
        return spot_lights_;
    }

    //------------------------------------------------------------------------------------------------------
    SharedPtr<Entity> SceneManager::GetRootEntity()
    {
        return root_entity_;
    }

    //------------------------------------------------------------------------------------------------------
    Vector<SharedPtr<Entity>>& SceneManager::GetEntities()
    {
        return all_entities_;
    }
    
    //------------------------------------------------------------------------------------------------------
    const Vector<SharedPtr<Entity>>& SceneManager::GetEntities() const
    {
        return all_entities_;
    }
}