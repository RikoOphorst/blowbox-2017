#pragma once

#include "util/unordered_map.h"
#include "util/string.h"
#include "util/shared_ptr.h"
#include "util/weak_ptr.h"
#include "content/text_file.h"
#include "content/binary_file.h"

namespace blowbox
{
    /**
    * By identifying every file by its file path, this class allows you to
    * efficiently handle your file resource loading of files from the host
    * machine's hard drive.
    *
    * @brief Manages any files that should be loaded from disk.
    */
    class FileManager
    {
    public:
        FileManager();
        ~FileManager();

        /** @brief Starts up the FileManager. */
        void Startup();
        /** @brief Starts a new frame in the FileManager. */
        void NewFrame();
        /** @brief Shuts down the FileManager. */
        void Shutdown();

        /**
        * @brief Loads a TextFile from disk. If the TextFile had already been loaded, it gets reloaded.
        * @param[in] file_path Path to the file to be loaded.
        * @returns A WeakPtr to the loaded TextFile.
        */
        WeakPtr<TextFile> LoadTextFile(const String& file_path);

        /**
        * @brief Unloads a TextFile from memory. If there are still any dependants on this TextFile, this function will assert.
        * @param[in] file_path Path to the file to be unloaded.
        */
        void UnloadTextFile(const String& file_path);

        /**
        * @brief Access a TextFile by name. If it hasn't been loaded yet, it will automatically be loaded.
        * @param[in] file_path Path to the file to be accessed.
        * @returns A WeakPtr to the accessed TextFile.
        */
        WeakPtr<TextFile> GetTextFile(const String& file_path);


        /**
        * @brief Loads a BinaryFile from disk. If the BinaryFile had already been loaded, it gets reloaded.
        * @param[in] file_path Path to the file to be loaded.
        * @returns A WeakPtr to the loaded BinaryFile.
        */
        WeakPtr<BinaryFile> LoadBinaryFile(const String& file_path);

        /**
        * @brief Unloads a BinaryFile from memory. If there are still any dependants on this BinaryFile, this function will assert.
        * @param[in] file_path Path to the file to be unloaded.
        */
        void UnloadBinaryFile(const String& file_path);

        /**
        * @brief Access a BinaryFile by name. If it hasn't been loaded yet, it will automatically be loaded.
        * @param[in] file_path Path to the file to be accessed.
        * @returns A WeakPtr to the accessed BinaryFile.
        */
        WeakPtr<BinaryFile> GetBinaryFile(const String& file_path);

    private:
        UnorderedMap<String, SharedPtr<BinaryFile>> binary_files_;  //!< All BinaryFiles that have been loaded.
        UnorderedMap<String, SharedPtr<TextFile>> text_files_;      //!< All TextFiles that have been loaded.
    };
}