#include "renderd7.hpp"

class SheetMaker
{
    public:
    SheetMaker();
    ~SheetMaker();
    void AddInage(int zwidth, int zheight, C2D_Image image);
    void Write(std::string path);
    private:
    int count;
    int count2;
    std::vector<u8> ImageBuffer;
    unsigned width, height;
};