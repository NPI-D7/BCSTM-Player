#include "renderd7.hpp"

class MMM : public RenderD7::Scene
{
    public:
    void Draw(void) const override;
    void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) override;
    private:
    std::vector<RenderD7::TObject> buttons = 
    {
        {90, 50, 140, 35, "Browse", -8, 10},
        {90, 100, 140, 35, "Credits", -10, 10},
        {90, 150, 140, 35, "Exit", 5, 10}
    };
    int Selection = 0;
};

class Browse : public RenderD7::Scene
{
    public:
    void Draw(void) const override;
    void Logic(u32 hDown, u32 hHeld, u32 hUp, touchPosition touch) override;
    Browse();
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
    private:
};
