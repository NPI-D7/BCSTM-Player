#include "RomFS_Manager.hpp"
#include <3ds.h>

Result RomFS::Mount(id, mediatype, name)
{
    romfsMountFromTitle(id, mediatype, name);
}