#pragma once

#include <rd7.hpp>

namespace D7 {
class BCSTM {
 public:
  BCSTM() {}
  ~BCSTM() { Stop(); }

  bool LoadFile(const std::string& path);
  void Update();
  void Play();
  void Pause();
  void Stop();

  inline bool IsLooping() { return this->is_looping; }
  inline bool IsLoaded() { return this->is_loaded; }
  inline unsigned int GetLoopStart() { return this->loop_start; }
  inline unsigned int GetLoopEnd() { return this->loop_end; }
  inline unsigned int GetChannelCount() { return this->channel_count; }
  inline unsigned int GetTotal() { return this->num_blocks; }
  inline unsigned int GetCurrent() { return this->current_block; }
  inline unsigned int GetSampleRate() { return this->sample_rate; }
  inline std::string GetErrorMessage() { return this->err_msg; }

 private:
  unsigned int read32();
  unsigned short read16();
  unsigned char read8();
  bool file_advance(unsigned long long bytes);
  void stream();
  void fill_buffers();

  enum RefType : uint16_t {
    ByteTable = 0x0100,
    ReferenceTable = 0x0101,
    SampleData = 0x1F00,
    DSPADPCMInfo = 0x0300,
    InfoBlock = 0x4000,
    SeekBlock = 0x4001,
    DataBlock = 0x4002,
    StreamInfo = 0x4100,
    TrackInfo = 0x4101,
    ChannelInfo = 0x4102,
  };

  static const int buffer_count = 20;

  std::string err_msg = "";

  std::fstream file;
  unsigned int current_time = 0;
  unsigned int last_time = 0;

  bool is_loaded = false;
  bool is_paused = false;
  bool is_looping = false;
  bool is_streaming = false;
  bool is_little_endian;

  unsigned int channel_count = 0;
  unsigned int sample_rate = 0;

  unsigned int loop_start = 0;
  unsigned int loop_end = 0;
  unsigned int num_blocks = 0;
  unsigned int block_size = 0;
  unsigned int block_samples = 0;
  unsigned int last_block_size = 0;
  unsigned int last_block_samples = 0;
  unsigned short adpcm_coefs[2][16];

  unsigned int current_block = 0;
  unsigned int info_offset = 0;
  unsigned int data_offset = 0;

  unsigned int active_channels = 0;
  unsigned short channel[2];
  ndspWaveBuf wave_buf[2][buffer_count];
  ndspAdpcmData adpcm_data[2][2];

  std::vector<unsigned char, RD7::LinearAllocator<unsigned char>>
      buffer_data[2][buffer_count];
};
}  // namespace D7