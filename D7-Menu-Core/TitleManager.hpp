#include "title.hpp"

#include <3ds.h>
#include <memory>
#include <vector>

namespace TitleManager
{
    void ScanSD(void);
    
    inline std::vector<std::shared_ptr<Title>> sdtitles;
}