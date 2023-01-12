#pragma once
#include <nlc.hpp>
#include <nuseful/stringtools.hpp>

#define getcol(col) nlc::color_storage::Get(col)

struct TObject
{
    int x;
    int y;
    int w;
    int h;
    std::string name;
    int ftx;
    int fty;
};

class MMM : public nlc::scene
{
    public:
    void Draw(void) const override;
    void Logic() override;
    MMM();
    private:
    std::vector<TObject> buttons = 
    {
        {100, 35, 120, 35, "Browse", 0, 0},
        {100, 85, 120, 35, "Titles", 0, 0},
        {100, 135, 120, 35, "Credits", 0, 0},
        {100, 185, 120, 35, "Exit", 0, 0},
    };
    int Selection = 0;
};

class Browse : public nlc::scene
{
    public:
    void Draw(void) const override;
    void Logic() override;
    Browse();
   // static void FS_Thread(RenderD7::Parameter param);
    private:
    std::string dir;
    std::vector<nlc::fsys::DirEntry> dircontent;
    int dirsel = 0;
    bool changeddir = false;
};

class Credits : public nlc::scene
{
    public:
    void Draw(void) const override;
    void Logic() override;
    Credits();
    private:
    int n = 0;
};

//class Titles : public nlc::scene
//{
//    public:
//    void Draw(void) const override;
//    void Logic() override;
//    Titles();
//    
//    private:
//    int selection = 0;  
//    int maxtitles = 0;  
//};

//class RomfsBrowse : public nlc::scene
//{
//    public:
//    void Draw(void) const override;
//    void Logic() override;
//    RomfsBrowse();
//    private:
//    std::vector<RenderD7::DirContent> dircontent;
//    int dirsel = 0;
//    bool changeddir = false;
//};
