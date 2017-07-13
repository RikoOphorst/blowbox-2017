#include "text_file.h"

#include <string>
#include <fstream>
#include <sstream>

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    TextFile::TextFile(const String& file_path) :
        file_path_(file_path)
    {
        std::ifstream file(file_path.c_str());
        std::stringstream buffer;
        buffer << file.rdbuf();

        file_content_ = buffer.str().c_str();
    }
    
    //------------------------------------------------------------------------------------------------------
    TextFile::~TextFile()
    {

    }

    //------------------------------------------------------------------------------------------------------
    const String& TextFile::GetFilePath() const
    {
        return file_path_;
    }

    //------------------------------------------------------------------------------------------------------
    const String& TextFile::GetFileContent() const
    {
        return file_content_;
    }
}