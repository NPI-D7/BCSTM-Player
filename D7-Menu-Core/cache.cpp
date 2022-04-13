#include "cache.hpp"
#include "renderd7.hpp"
#include "log.hpp"
#include "ini.hpp"

Log cachelog;

void Cache::Create(std::vector<std::shared_ptr<Title>> t, const std::string& path)
{
    remove(path.c_str());
    INI::INIFile cache(path);
    INI::INIStructure cachedata;
    for(unsigned i = 0; i < t.size(); i++)
    {
        RenderD7::Msg::DisplayWithProgress("D7-Menu-Core", "Writing Cache: " + t[i]->name(), i, (int)t.size(), RenderD7::Color::Hex("#00DD11"));
        cachedata[t[i]->name()]["name"] = t[i]->name();
        cachedata[t[i]->name()]["author"] = t[i]->author();
        cachedata[t[i]->name()]["prod"] = t[i]->prodcode();
        cachedata[t[i]->name()]["id"] = t[i]->ID();
    }
    cache.write(cachedata);
}

bool Cache::Read(std::vector<std::shared_ptr<Title>> t, const std::string& path, bool nand)
{
    RenderD7::Msg::Display("D7-Menu-Core", "Look for exisring cache...", Top);
    if (!RenderD7::FS::FileExist(path))
    {
        return false;
    }

    RenderD7::Msg::Display("D7-Menu-Core",  "Loading Titles from cache...", Top);

    FILE *cache = fopen(path.c_str(), "rb");
    //Check revision
    uint8_t rev = 0;
    fseek(cache, 2, SEEK_SET);
    rev = fgetc(cache);
    fseek(cache, 0, SEEK_SET);
    cachelog.Write("Read File...");
    if(rev != 2)
    {
        fclose(cache);
        return false;
    }

    uint16_t count = 0;
    fread(&count, sizeof(uint16_t), 1, cache);
    fgetc(cache);
    cachelog.Write("Read count...");
    for(unsigned i = 0; i < count; i++)
    {
        auto newData = std::make_shared<Title>();
        
        char title[64];
        fread(title, sizeof(uint16_t), 0x40, cache);
        fgetc(cache);

        /*char author[64];
        fread(author, sizeof(uint16_t), 0x20, cache);
        fgetc(cache);*/

        char prodCode[16];
        fread(prodCode, 1, 16, cache);
        fgetc(cache);

        uint64_t newID = 0;
        fread(&newID, sizeof(uint64_t), 1, cache);
        fgetc(cache);
        RenderD7::Msg::DisplayWithProgress("D7-Menu-Core",  "Loading Titles from cache: ", i, count, RenderD7::Color::Hex("#00DD11"));
        newData->LoadFromCache(newID, title, prodCode, nand ? MEDIATYPE_NAND : MEDIATYPE_SD);
        t.push_back(newData);
    }
    cachelog.Write("Read Data...");
    fclose(cache);
    cachelog.Write("Close File...");

    return true;
    cachelog.Write("return");
}
