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

void SheetMaker::AddInage(int zwidth, int zheight, C2D_Image *image)
{
    for(unsigned y = count*zheight; y < height; y++){
    for(unsigned x = count*zwidth; x < width; x++) {
    ImageBuffer[4 * width * y + 4 * x + 0] = ((uint8_t *)this->img.tex->data)[dstPos + 3];
    ImageBuffer[4 * width * y + 4 * x + 1] = ((uint8_t *)this->img.tex->data)[dstPos + 2];
    ImageBuffer[4 * width * y + 4 * x + 2] = ((uint8_t *)this->img.tex->data)[dstPos + 1];
    ImageBuffer[4 * width * y + 4 * x + 3] = ((uint8_t *)this->img.tex->data)[dstPos + 0];
    }}
    count++;
}

void SheetMaker::Write(std::string path)
{
    lodepng::encode(path.c_str(), ImageBuffer, width, height);
}