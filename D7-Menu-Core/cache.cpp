#include "cache.hpp"
#include "renderd7.hpp"
#include "log.hpp"
#include "ini.hpp"
#include <iostream>
#include <sstream>
#include <cstdint>
#include "utils.hpp"
#include "SheetMaker.hpp"

Log cachelog;

void Cache::Create(std::vector<std::shared_ptr<Title>> t, const std::string& path)
{
	SheetMaker sheet;
    remove(path.c_str());
    INI::INIFile cache(path);
    INI::INIStructure cachedata;
    for(unsigned i = 0; i < t.size(); i++)
    {
        RenderD7::Msg::DisplayWithProgress("D7-Menu-Core", "Writing Cache: " + t[i]->name(), i, (int)t.size(), RenderD7::Color::Hex("#00DD11"));
        cachedata[t[i]->name()]["name"] = t[i]->name();
        //cachedata[t[i]->name()]["author"] = t[i]->author();
        cachedata[t[i]->name()]["prod"] = t[i]->prodcode();
        cachedata[t[i]->name()]["id"] = std::to_string(t[i]->ID());
        sheet.AddInage(48, 48, (u8)UTF16toUTF8((char16_t*)t[i]->IconBuffer));
    }
    sheet.Write(path + ".png");
    cache.write(cachedata);
}

bool Cache::Read(std::vector<std::shared_ptr<Title>> t, const std::string& path, bool nand)
{
    int zz = 0;
    RenderD7::Msg::Display("D7-Menu-Core", "Look for exisring cache...", Top);
    if (!RenderD7::FS::FileExist(path))
    {
        return false;
    }
    std::vector<std::string> secs;
    RenderD7::Msg::Display("D7-Menu-Core",  "Loading Titles from cache...", Top);
    INI::INIFile cache(path);
    INI::INIStructure cachedata;
    cache.read(cachedata);
    for (auto const& it : cachedata)
    {
	    auto const& section = it.first;
        secs.push_back(section);
        RenderD7::Msg::DisplayWithProgress("D7-Menu-Core",  "Loading Data: " + section, zz, cachedata.size(), RenderD7::Color::Hex("#00DD11"));
        zz++;
    }
    for(unsigned i = 1; i < secs.size(); i++)
    {
        auto newData = std::make_shared<Title>();
        
        std::string title = cachedata[secs[i]]["name"];

        std::string prodCode = cachedata[secs[i]]["prod"];
        

        uint64_t newID = 0;
        std::istringstream iss(cachedata[secs[i]]["id"]);
        iss >> newID;
        RenderD7::Msg::DisplayWithProgress("D7-Menu-Core",  "Loading Titles from cache: ", i, secs.size(), RenderD7::Color::Hex("#00DD11"));
        newData->LoadFromCache(newID, title, prodCode, nand ? MEDIATYPE_NAND : MEDIATYPE_SD);
        t.push_back(newData);
    }
    return true;
    cachelog.Write("return");
}
