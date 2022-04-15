#include "SheetMaker.hpp"

struct px
{
    u8 r;
    u8 g;
    u8 b;
};

SheetMaker::SheetMaker()
{
    height = 1024;
    width = 1024;
    ImageBuffer.resize(width * height * 4);
}

SheetMaker::~SheetMaker()
{

}

u8* SheetMaker::ToRGB888(u8* bufdata, int ywidth, int yheight)
{
    px pxmap[12][12];
    for(unsigned y = 0; y < yheight; y++){
    for(unsigned x = 0; x < ywidth; x++) {
        
    }}
}

void SheetMaker::AddInage(int zwidth, int zheight, C3D_Tex tex)
{
    if (((count2 * zwidth) + zwidth) > (int)width)
    {
        count++;
        count2 = 0;
    }

    /*for(unsigned y = count*zheight; y < height; y++){
    for(unsigned x = count2*zwidth; x < width; x++) {
    const u32 srcPos = ((((y >> 3) * (width >> 3) + (x >> 3)) << 6) +
	((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) |
	((x & 4) << 2) | ((y & 4) << 3))) * 4;
    const u32 dstPos = (y * zwidth + x) * 4;
    ImageBuffer[dstPos + 0] = ((uint8_t *)tex.data)[srcPos + 3];
    ImageBuffer[dstPos + 1] = ((uint8_t *)tex.data)[srcPos + 2];
    ImageBuffer[dstPos + 2] = ((uint8_t *)tex.data)[srcPos + 1];
    ImageBuffer[dstPos + 3] = ((uint8_t *)tex.data)[srcPos + 0];
    }}*/
    
}

void SheetMaker::Write(std::string path)
{
    //lodepng::encode(path.c_str(), ImageBuffer, width, height);
}