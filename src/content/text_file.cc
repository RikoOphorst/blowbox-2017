#include "text_file.h"

#include <string>
#include <fstream>
#include <sstream>

#include "core/debug/performance_profiler.h"

namespace blowbox
{
    //------------------------------------------------------------------------------------------------------
    TextFile::TextFile(const String& file_path) :
        file_path_(file_path)
    {
        Reload();
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
    
    //------------------------------------------------------------------------------------------------------
    void TextFile::Reload()
    {
        char buf[512];
        sprintf(buf, "TextFile: %s", file_path_.c_str());
        PerformanceProfiler::ProfilerBlock block(buf, ProfilerBlockType_CONTENT);

        std::ifstream file(file_path_.c_str());
        std::stringstream buffer;
        buffer << file.rdbuf();

        file_content_ = buffer.str().c_str();
    }
}