#include "SheetMaker.hpp"

SheetMaker::SheetMaker()
{
    height = 1024;
    width = 1024;
    ImageBuffer.resize(width * height * 4);
    bitmap->height = height;
    bitmap->width = width;
}

SheetMaker::~SheetMaker()
{

}

void SheetMaker::AddInage(int zwidth, int zheight, u16 *buf)
{
    if ((count2 * zwidth) > width) count++;

    /*for(unsigned y = count*zheight; y < height; y++){
    for(unsigned x = count2*zwidth; x < width; x++) {
    const u32 srcPos = ((((y >> 3) * (width >> 3) + (x >> 3)) << 6) +
	((x & 1) | ((y & 1) << 1) | ((x & 2) << 1) | ((y & 2) << 2) |
	((x & 4) << 2) | ((y & 4) << 3))) * 4;
    const u32 dstPos = (y * zwidth + x) * 4;
    ImageBuffer[dstPos + 0] = tex.data[srcPos + 3];
    ImageBuffer[dstPos + 1] = tex.data[srcPos + 2];
    ImageBuffer[dstPos + 2] = tex.data[srcPos + 1];
    ImageBuffer[dstPos + 3] = tex.data[srcPos + 0];
    
    }}
    count2++;*/

    // Convert RGB565 icon to a RGB24 one
	int x=(count2*zwidth);
	int y=(count*zheight);
	int tile_size = 16;
	int tile_number = 1;
	int extra_x = 0;
	int extra_y = 0;
	int i=0;
	int tile_x[16] = {0,1,0,1,2,3,2,3,0,1,0,1,2,3,2,3};
	int tile_y[16] = {0,0,1,1,0,0,1,1,2,2,3,3,2,2,3,3};
	while (tile_number < 37){
		while (i < (tile_size)){
			BMP::PutPixel565(bitmap->pixels, tile_x[i-((tile_number-1)<<6)] + extra_x, tile_y[i-((tile_number-1)<<6)] + extra_y, buf[i]);
			BMP::PutPixel565(bitmap->pixels, 4+tile_x[i-((tile_number-1)<<6)] + extra_x, tile_y[i-((tile_number-1)<<6)] + extra_y, buf[i+16]);
			BMP::PutPixel565(bitmap->pixels, tile_x[i-((tile_number-1)<<6)] + extra_x, 4+tile_y[i-((tile_number-1)<<6)] + extra_y, buf[i+32]);
			BMP::PutPixel565(bitmap->pixels, 4+tile_x[i-((tile_number-1)<<6)] + extra_x, 4+tile_y[i-((tile_number-1)<<6)] + extra_y, buf[i+48]);
			i++;
		}
		if (tile_number % 6 == 0){
			extra_x = 0;
			extra_y = extra_y + 8;
		}else extra_x = extra_x + 8;
		tile_number++;
		tile_size = tile_size + 64;
		i = i + 48;
	}
	bitmap->magic = 0x4C494D47;
    count2++;
}

void SheetMaker::Write(std::string path)
{
    lodepng::encode(path.c_str(), ImageBuffer, width, height);
    //std::string path2 = path + ".bmp";
    BMP::Save("/BCSTM-Player/cache/bmp.bmp", bitmap);
}