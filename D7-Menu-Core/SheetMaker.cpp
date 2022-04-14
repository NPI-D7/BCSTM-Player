#include "SheetMaker.hpp"

SheetMaker::SheetMaker()
{
    height = 1024;
    width = 1024;
    ImageBuffer.resize(width * height * 4);
}

SheetMaker::~SheetMaker()
{

}

void SheetMaker::AddInage(int zwidth, int zheight, C2D_Image image)
{
    if ((count2 * zwidth) > width) count++;

    for(unsigned y = count*zheight; y < height; y++){
    for(unsigned x = count2*zwidth; x < width; x++) {
    const u32 dstPos = ((((y >> 3) * (count2*zwidth >> 3) + (x >> 3)) << 6) +
	((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) |
	((x & 4) << 2) | ((y & 4) << 3))) * 4;
    const u32 srcPos = (y * width + x) * 4;
    ImageBuffer[srcPos + 0] = ((uint8_t *)image.tex->data)[dstPos + 3];
    ImageBuffer[srcPos + 1] = ((uint8_t *)image.tex->data)[dstPos + 2];
    ImageBuffer[srcPos + 2] = ((uint8_t *)image.tex->data)[dstPos + 1];
    ImageBuffer[srcPos + 3] = ((uint8_t *)image.tex->data)[dstPos + 0];
    }}
    count2++;

}

void SheetMaker::Write(std::string path)
{
    lodepng::encode(path.c_str(), ImageBuffer, width, height);
}