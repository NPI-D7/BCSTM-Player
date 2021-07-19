#include "renderd7.hpp"

class MMM : public RenderD7::Scene
{
    public:
    void Draw(void) const override;
    void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) override;
    MMM();
    private:
    std::vector<RenderD7::TObject> buttons = 
    {
        {20, 35, 120, 35, "Browse", -11, 10},
        {20, 85, 120, 35, "Titles(X)", 0, 9},
        {20, 135, 120, 35, "Credits", -8, 10},
        {20, 185, 120, 35, "Exit", 8, 10},
        {180, 35, 120, 35, "Settings(X)", -15, 10},
        {180, 85, 120, 35, "Update(X)", -15, 10},
        {180, 135, 120, 35, "Nothing(X)", -15, 10},
        {180, 185, 120, 35, "Nothing(X)", -15, 10}
    };
    int Selection = 0;
};

class Browse : public RenderD7::Scene
{
    public:
    void Draw(void) const override;
    void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) override;
    Browse();
   // static void FS_Thread(RenderD7::Parameter param);
    private:
    std::vector<RenderD7::DirContent> dircontent;
    int dirsel = 0;
    bool changeddir = false;
};

class Credits : public RenderD7::Scene
{
    public:
    void Draw(void) const override;
    void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) override;
    Credits();
    private:
    int n = 0;
};

class Titles : public RenderD7::Scene
{
    public:
    void Draw(void) const override;
    void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) override;
    Titles();
    
    private:
    int selection = 0;  
    int maxtitles = 0;  
};

class RomfsBrowse : public RenderD7::Scene
{
    public:
    void Draw(void) const override;
    void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) override;
    RomfsBrowse();
    private:
    std::vector<RenderD7::DirContent> dircontent;
    int dirsel = 0;
    bool changeddir = false;
};
