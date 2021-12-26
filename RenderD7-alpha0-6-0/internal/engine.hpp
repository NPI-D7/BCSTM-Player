#pragma once
#include <3ds.h>
#include <citro2d.h>
#include <citro3d.h>
#include <memory>
#include <stack>
#include <string>
#include <functional>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <cstring>
#include <sys/stat.h>
#include <algorithm>
#include <iostream>
#include <codecvt>
#include <map>
#include "lang.hpp"
#include "parameter.hpp"
#include "thread.hpp"
#include "ini.hpp"
#include "stringtool.hpp"

enum EngineType
{
    _2D,
    _3D
};

namespace Npi
class Game {
    public:
    Game(EngineType, std::string name);
    ~Game(){}
    private:
    std::string g_name;
    EngineType g_etype;
};
}
