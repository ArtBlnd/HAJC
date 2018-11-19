#ifndef __HAJC_TOOLS_FILE_CONTEXT_H__
#define __HAJC_TOOLS_FILE_CONTEXT_H__

#include <string>
#include <fstream>

namespace HAJC
{
    class FileContext
    {
        std::fstream fcStream;

    public:
        FileContext(std::string filename);
    };
}

#endif