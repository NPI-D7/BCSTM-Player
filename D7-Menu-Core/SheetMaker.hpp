#include "renderd7.hpp"
#include "bmp.hpp"
class SheetMaker
{
    public:
    SheetMaker();
    ~SheetMaker();
    void AddInage(int zwidth, int zheight, C3D_Tex tex);
    void Write(std::string path);
    private:
    int count;
    int count2;
    std::vector<u8> ImageBuffer;
    unsigned width, height;
    //BMP bmp(1024, 1024);
};