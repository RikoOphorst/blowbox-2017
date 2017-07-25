#pragma once

#include "util/unordered_map.h"
#include "util/string.h"
#include "util/shared_ptr.h"
#include "util/weak_ptr.h"
#include "content/image.h"

namespace blowbox
{
    /**
    * By identifying every file by its file path, this class allows you to
    * efficiently handle your image resource loading of files from the host
    * machine's hard drive.
    *
    * @brief Manages any images that should be loaded from disk.
    */
    class ImageManager
    {
    public:
        ImageManager();
        ~ImageManager();

        /** @brief Starts up the FileManager. */
        void Startup();
        /** @brief Starts a new frame in the FileManager. */
        void NewFrame();
        /** @brief Shuts down the FileManager. */
        void Shutdown();

        /**
        * @brief Loads an Image from disk. If the Image had already been loaded, it gets reloaded.
        * @param[in] file_path Path to the image to be loaded.
        * @returns A WeakPtr to the loaded Image.
        */
        WeakPtr<Image> LoadImage(const String& file_path);

        /**
        * @brief Unloads an Image from memory. If there are still any dependants on this Image, this function will assert.
        * @param[in] file_path Path to the image to be unloaded.
        */
        void UnloadImage(const String& file_path);

        /**
        * @brief Access a Image by name. If it hasn't been loaded yet, it will automatically be loaded.
        * @param[in] file_path Path to the image to be accessed.
        * @returns A WeakPtr to the accessed Image.
        */
        WeakPtr<Image> GetImage(const String& file_path);

    private:
        UnorderedMap<String, SharedPtr<Image>> images_; //!< All images that are in the ImageManager.
    };
}