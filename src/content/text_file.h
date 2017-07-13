#pragma once

#include "util/string.h"

namespace blowbox
{
    class TextFile
    {
    public:
        TextFile(const String& file_path);
        ~TextFile();
        
        const String& GetFilePath() const;
        const String& GetFileContent() const;

    private:
        String file_path_;
        String file_content_;
    };
}