#include "cache.hpp"
#include "renderd7.hpp"
#include "log.hpp"

Log cachelog;

void Cache::Create(std::vector<std::shared_ptr<Title>> t, const std::string& path)
{
    
    cachelog.Write("Made Dirs...");
    remove(path.c_str());
    cachelog.Write("romove path");
    FILE *cache = fopen(path.c_str(), "wb");
    cachelog.Write("Make file");
    u16 countOut = t.size();
    cachelog.Write("set count");
    fwrite(&countOut, sizeof(u16), 1, cache);
    cachelog.Write("write count1");
    fputc(0x02, cache);
    cachelog.Write("fpputc blabla");
    cachelog.Write("Write count");
    for(unsigned i = 0; i < t.size(); i++)
    {
        RenderD7::Msg::DisplayWithProgress("D7-Menu-Core", "Writing Cache: " + t[i]->name(), i, (int)t.size(), RenderD7::Color::Hex("#00DD11"));
        char16_t titleOut[0x40];
        memset(titleOut, 0, 0x40 * 2);
        memcpy(titleOut, t[i]->name().data(), t[i]->name().length() * 2);
        fwrite(titleOut, sizeof(char16_t), 0x40, cache);
        fputc(0, cache);

        /*char16_t Author[0x20];
        memset(Author, 0, 0x20 * 2);
        memcpy(Author, t[i]->author().data(), t[i]->author().length() * 2);
        fwrite(Author, sizeof(char16_t), 0x20, cache);
        fputc(0, cache);*/
        
        char prodOut[16];
        memset(prodOut, 0, 16);
        memcpy(prodOut, t[i]->prodcode().data(), 16);
        fwrite(prodOut, 1, 16, cache);
        fputc(0, cache);

        uint64_t idOut = t[i]->ID();
        fwrite(&idOut, sizeof(uint64_t), 1, cache);
        fputc(0, cache);
    }
    cachelog.Write("Write Titles");
    fclose(cache);
    cachelog.Write("close File");
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
