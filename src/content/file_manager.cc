#include "file_manager.h"

#include "util/assert.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    FileManager::FileManager()
    {

    }

    //------------------------------------------------------------------------------------------------------
    FileManager::~FileManager()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void FileManager::Startup()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void FileManager::NewFrame()
    {

    }

    //------------------------------------------------------------------------------------------------------
    void FileManager::Shutdown()
    {
        for (auto it = binary_files_.begin(); it != binary_files_.end(); it++)
        {
            BLOWBOX_ASSERT(it->second.use_count() == 1);
            it->second.reset();
        }

        for (auto it = text_files_.begin(); it != text_files_.end(); it++)
        {
            BLOWBOX_ASSERT(it->second.use_count() == 1);
            it->second.reset();
        }
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<TextFile> FileManager::LoadTextFile(const String& file_path)
    {
        auto it = text_files_.find(file_path);

        if (it == text_files_.end())
        {
            text_files_[file_path] = eastl::make_shared<TextFile>(file_path);
        }
        else
        {
            text_files_[file_path]->Reload();
        }

        return text_files_[file_path];
    }

    //------------------------------------------------------------------------------------------------------
    void FileManager::UnloadTextFile(const String& file_path)
    {
        auto it = text_files_.find(file_path);

        if (it != text_files_.end())
        {
            BLOWBOX_ASSERT(text_files_[file_path].use_count() == 1);
            text_files_.erase(it);
        }
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<TextFile> FileManager::GetTextFile(const String& file_path)
    {
        auto it = text_files_.find(file_path);

        if (it == text_files_.end())
        {
            return LoadTextFile(file_path);
        }
        else
        {
            return text_files_[file_path];
        }
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<BinaryFile> FileManager::LoadBinaryFile(const String& file_path)
    {
        auto it = binary_files_.find(file_path);

        if (it == binary_files_.end())
        {
            binary_files_[file_path] = eastl::make_shared<BinaryFile>(file_path);
        }
        else
        {
            binary_files_[file_path]->Reload();
        }

        return binary_files_[file_path];
    }

    //------------------------------------------------------------------------------------------------------
    void FileManager::UnloadBinaryFile(const String& file_path)
    {
        auto it = binary_files_.find(file_path);

        if (it != binary_files_.end())
        {
            BLOWBOX_ASSERT(binary_files_[file_path].use_count() == 1);
            binary_files_.erase(it);
        }
    }

    //------------------------------------------------------------------------------------------------------
    WeakPtr<BinaryFile> FileManager::GetBinaryFile(const String& file_path)
    {
        auto it = binary_files_.find(file_path);

        if (it == binary_files_.end())
        {
            return LoadBinaryFile(file_path);
        }
        else
        {
            return binary_files_[file_path];
        }
    }
}