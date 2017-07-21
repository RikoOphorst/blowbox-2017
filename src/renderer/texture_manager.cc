#include "texture_manager.h"

#include "core/debug/console.h"
#include "core/get.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    TextureManager::TextureManager()
    {

    }

    //------------------------------------------------------------------------------------------------------
    TextureManager::~TextureManager()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void TextureManager::Startup()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void TextureManager::NewFrame()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void TextureManager::Shutdown()
    {

    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Texture> TextureManager::AddTexture(const String& name, SharedPtr<Texture> texture)
    {
        auto it = textures_.find(name);

        if (it == textures_.end())
        {
            textures_[name] = texture;
            return textures_[name];
        }
        else
        {
            char buf[512];
            sprintf(buf, "Tried adding a Texture (%s) to the TextureManager, but a Texture was already present under that name. No action was taken, but this might result in unexpected behaviour.");
            Get::Console()->LogWarning(buf);
        }
    }

    //------------------------------------------------------------------------------------------------------
    void TextureManager::RemoveTexture(const String& name)
    {
        auto it = textures_.find(name);

        if (it != textures_.end())
        {
            BLOWBOX_ASSERT(it->second.use_count() == 0);
            textures_.erase(it);
        }
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<Texture> TextureManager::GetTexture(const String& name)
    {
        auto it = textures_.find(name);

        if (it != textures_.end())
        {
            return it->second;
        }
        else
        {
            return AddTexture(name, eastl::make_shared<Texture>());
        }
    }
}