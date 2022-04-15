#include "SheetMaker.hpp"

#define RGBA8(r, g, b, a) ((((r) & 0xFF) << 0) | (((g) & 0xFF) << 8) | (((b) & 0xFF) << 16) | (((a) & 0xFF) << 24))

SheetMaker::SheetMaker()
{
    height = 1024;
    width = 1024;
    ImageBuffer.resize(width * height * 4);
}

SheetMaker::~SheetMaker()
{

}

uint32_t* SheetMaker::ToRGBA888(uint16_t* bufdata, int ywidth, int yheight)
{
    uint8_t r = (((((int)bufdata >> 11) & 0x1F) * 527) + 23) >> 6;
    uint8_t g = (((((int)bufdata >> 5) & 0x1F) * 259) + 33) >> 6;
    uint8_t b = ((((int)bufdata & 0x1F) & 0x1F) + 23) >> 6;
    return RGBA8((int)r,(int)g,(int)b,255);    
}

void SheetMaker::AddInage(int zwidth, int zheight, u16* buf)
{
    if (((count2 * zwidth) + zwidth) > (int)width)
    {
        count++;
        count2 = 0;
    }
    u32* imagesrc = this->ToRGBA888(buf, zwidth, zheight);
    for(unsigned y = count*zheight; y < height; y++){
    for(unsigned x = count2*zwidth; x < width; x++) {
    const u32 srcPos = ((((y >> 3) * (width >> 3) + (x >> 3)) << 6) +
	((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) |
	((x & 4) << 2) | ((y & 4) << 3))) * 4;
    const u32 dstPos = (y * zwidth + x) * 4;
    ImageBuffer[dstPos + 0] = ((uint8_t *)imagesrc)[srcPos + 0];
    ImageBuffer[dstPos + 1] = ((uint8_t *)imagesrc)[srcPos + 1];
    ImageBuffer[dstPos + 2] = ((uint8_t *)imagesrc)[srcPos + 2];
    ImageBuffer[dstPos + 3] = ((uint8_t *)imagesrc)[srcPos + 3];
    }}
    count2++;
}

void SheetMaker::Write(std::string path)
{
    //lodepng::encode(path.c_str(), ImageBuffer, width, height);
}