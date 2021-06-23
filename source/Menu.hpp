#include "renderd7.hpp"

inline std::vector<RenderD7::DirContent> dircontentz;
inline int dirselz = 0;
inline bool changeddirz = false;
class MMM : public RenderD7::Scene
{
    public:
    void Draw(void) const override;
    void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) override;
    MMM();
    private:
    std::vector<RenderD7::TObject> buttons = 
    {
        {20, 35, 100, 35, "Browse", -11, 10},
        {20, 85, 100, 35, "Titles", 0, 9},
        {20, 135, 100, 35, "Credits", -8, 10},
        {20, 285, 100, 35, "Exit", 8, 10},
        {200, 35, 100, 35, "Settings", -30, 10}
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
