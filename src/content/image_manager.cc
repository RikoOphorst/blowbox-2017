#include "image_manager.h"

#include "util/assert.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    ImageManager::ImageManager()
    {

    }

    //------------------------------------------------------------------------------------------------------
    ImageManager::~ImageManager()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void ImageManager::Startup()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void ImageManager::NewFrame()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void ImageManager::Shutdown()
    {
        for (auto it = images_.begin(); it != images_.end(); it++)
        {
            BLOWBOX_ASSERT(it->second.use_count() == 1);
            it->second.reset();
        }
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Image> ImageManager::LoadImage(const String& file_path)
    {
        auto it = images_.find(file_path);

        if (it == images_.end())
        {
            images_[file_path] = eastl::make_shared<Image>(file_path);
        }
        else
        {
            images_[file_path]->Reload();
        }

        return images_[file_path];
    }

    //------------------------------------------------------------------------------------------------------
    void ImageManager::UnloadImage(const String& file_path)
    {
        auto it = images_.find(file_path);

        if (it != images_.end())
        {
            BLOWBOX_ASSERT(images_[file_path].use_count() == 1);
            images_.erase(it);
        }
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Image> ImageManager::GetImage(const String& file_path)
    {
        auto it = images_.find(file_path);

        if (it == images_.end())
        {
            return LoadImage(file_path);
        }
        else
        {
            return images_[file_path];
        }
    }
}