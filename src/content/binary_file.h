#pragma once

#include "util/string.h"

namespace blowbox
{
    /**
    * BinaryFile allows you to load a binary file from disk based on a given file path.
    *
    * @brief Wraps the loading and accessing of text files from disk.
    */
    class BinaryFile
    {
    public:
        /**
        * @brief Constructs a BinaryFile.
        * @param[in] file_path The filepath to the file to be loaded.
        */
        BinaryFile(const String& file_path);
        ~BinaryFile();

        /** @brief Reloads the BinaryFile. */
        void Reload();
    };
}