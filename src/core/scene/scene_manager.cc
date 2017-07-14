#include "scene_manager.h"

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