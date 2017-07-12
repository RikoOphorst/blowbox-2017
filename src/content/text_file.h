#pragma once

#include "core/eastl.h"
#include "EASTL/string.h"

namespace blowbox
{
    class TextFile
    {
    public:
        TextFile(const eastl::string& file_path);
        ~TextFile();
        
        const eastl::string& GetFilePath() const;
        const eastl::string& GetFileContent() const;

    private:
        eastl::string file_path_;
        eastl::string file_content_;
    };
}