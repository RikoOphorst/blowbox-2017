#pragma once

#include "util/string.h"

namespace blowbox
{
    /**
    * TextFile allows you to load a text file from disk based on a given file path.
    * It's content can be requested through TextFile::GetFileContent() which returns
    * a String containing the full contents of the file. Under the hood, we use STL
    * fstream to actually load the file.
    *
    * @brief Wraps the loading and accessing of text files from disk.
    */
    class TextFile
    {
    public:
        /**
        * Constructs a TextFile by loading a file from disk based on the supplied file_path argument.
        * @param[in] file_path The path to the file that you wish to load.
        */
        TextFile(const String& file_path);

        /**
        * Destructs the TextFile.
        */
        ~TextFile();
        
        /**
        * @brief Returns the file path leading to the file that is encapsulated by the TextFile.
        * @returns The file path leading to the file that is encapsulated by the TextFile.
        */
        const String& GetFilePath() const;

        /**
        * @brief Returns the contents of the file encapsulated by the TextFile.
        * @returns The contents of the file encapsulated by the TextFile.
        */
        const String& GetFileContent() const;

    private:
        String file_path_; //!< File path to the file that is encapsulated by the TextFile.
        String file_content_; //!< Contents of the file that is encapsulated by the TextFile.
    };
}