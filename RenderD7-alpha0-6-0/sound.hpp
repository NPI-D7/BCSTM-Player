

#include <3ds.h>
#include <string>

class sound {
public:
	sound(const std::string& path, int channel = 1, bool toloop = false);
	~sound();
	void play();
	void stop();

private:
	u32 dataSize;
	ndspWaveBuf waveBuf;
	u8* data = NULL;
	int chnl;
};

