#include "scene_manager.h"

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
    void SceneManager::Update()
    {
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
            eastl::shared_ptr<Entity> entity = entities_to_be_removed_.front();

            for (int i = 0; i < all_entities_.size(); i++) 
            {
                if (all_entities_[i] == entity)
                {
                    all_entities_.erase(all_entities_.begin() + i);
                    break;
                }
            }

            entities_to_be_removed_.pop();
        }

        while (!entities_to_be_added_.empty())
        {
            eastl::shared_ptr<Entity> entity = entities_to_be_removed_.front();
            all_entities_.push_back(entity);
            entities_to_be_added_.pop();
        }
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::AddEntity(eastl::shared_ptr<Entity> entity)
    {
        entities_to_be_added_.push(entity);
    }

    //------------------------------------------------------------------------------------------------------
    void SceneManager::RemoveEntity(eastl::shared_ptr<Entity> entity)
    {
        entities_to_be_removed_.push(entity);
    }
    
    //------------------------------------------------------------------------------------------------------
    eastl::shared_ptr<Entity>& SceneManager::GetRootEntity()
    {
        return root_entity_;
    }
    
    //------------------------------------------------------------------------------------------------------
    const eastl::shared_ptr<Entity>& SceneManager::GetRootEntity() const
    {
        return root_entity_;
    }

    //------------------------------------------------------------------------------------------------------
    eastl::vector<eastl::shared_ptr<Entity>>& SceneManager::GetEntities()
    {
        return all_entities_;
    }
    
    //------------------------------------------------------------------------------------------------------
    const eastl::vector<eastl::shared_ptr<Entity>>& SceneManager::GetEntities() const
    {
        return all_entities_;
    }
}