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
        {90, 20, 140, 35, "Browse", -8, 10},
        {90, 70, 140, 35, "Titles", 0, 9},
        {90, 120, 140, 35, "Credits", -4, 10},
        {90, 170, 140, 35, "Exit", 4, 10}
    };
    int Selection = 0;
};

class Browse : public RenderD7::Scene
{
    public:
    void Draw(void) const override;
    void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) override;
    Browse();
    static void FS_Thread(RenderD7::Parameter param);
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
