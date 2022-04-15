#include "renderd7.hpp"

class SheetMaker
{
    public:
    SheetMaker();
    ~SheetMaker();
    void AddInage(int zwidth, int zheight, u16* buf);
    uint32_t ToRGBA888(uint16_t* bufdata, int ywidth, int yheight);
    void Write(std::string path);
    private:
    int count;
    int count2;
    std::vector<u8> ImageBuffer;
    unsigned width, height;
};