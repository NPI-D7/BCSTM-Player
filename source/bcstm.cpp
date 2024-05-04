#include <arpa/inet.h>

#include <bcstm.hpp>

bool D7::BCSTM::LoadFile(const std::string &path) {
  Stop();
  file.open(path, std::ios::in | std::ios::binary);
  if (!file) {
    err_msg = "Unable to load File!";
    return false;
  }
  is_little_endian = true;  // default to true

  auto magic = read32();
  if (!(is_little_endian = (read16() == 0xFEFF))) {
    magic = htonl(magic);
  }
  if (magic != 0x4D545343) {  // CSTM
    file.close();
    err_msg = "File is invalid!";
    return false;
  }
  file.seekg(0x10);
  auto sbc = read16();
  read16();

  for (unsigned short i = 0; i < sbc; i++) {
    auto sec = read16();
    read16();  // padding
    auto off = read32();
    read32();  // size
    if (sec == InfoBlock)
      info_offset = off;
    else if (sec == DataBlock)
      data_offset = off;
  }
  if (!info_offset || !data_offset) {
    err_msg = "Data/Info Section not found!";
    return false;
  }

  file.seekg((info_offset + 0x20));
  if (read8() != 2) {
    err_msg = "Unknown Error!";
    return false;
  }
  is_looping = read8();
  channel_count = read8();
  if (channel_count > 2) {
    err_msg = "File has not 2 channels!";
    return false;
  }
  file.seekg((info_offset + 0x24));
  sample_rate = read32();
  auto _loop_pos = read32();
  auto _loop_end = read32();
  num_blocks = read32();
  block_size = read32();
  block_samples = read32();
  read32();  // last block used bytes
  last_block_samples = read32();
  last_block_size = read32();

  loop_start = _loop_pos / block_samples;
  loop_end =
      (_loop_end % block_samples ? num_blocks : _loop_end / block_samples);

  while (read32() != ChannelInfo) {
    // Find Channel Info
  }
  file_advance(read32() + channel_count * 8 - 12);

  // get adpcm data
  for (unsigned int i = 0; i < channel_count; i++) {
    file.read(reinterpret_cast<char *>(adpcm_coefs[i]),
              sizeof(unsigned short) * 16);
    // beginning context
    file.read(reinterpret_cast<char *>(&adpcm_data[i][0]),
              sizeof(ndspAdpcmData));
    // loop context
    file.read(reinterpret_cast<char *>(&adpcm_data[i][1]),
              sizeof(ndspAdpcmData));
    // skip padding
    read16();
  }

  file.seekg((data_offset + 0x20));
  is_loaded = true;
  return true;
}

void D7::BCSTM::Update() { this->stream(); }

void D7::BCSTM::Play() {
  if (is_paused) {
    for (unsigned int i = 0; i < channel_count; i++) {
      ndspChnSetPaused(channel[i], false);
    }
    is_paused = false;
    return;
  }
  if (is_streaming) return;
  for (unsigned int i = 0; i < channel_count; i++) {
    {
      channel[i] = 0;
      while (channel[i] < 24 && ((active_channels >> channel[i]) & 1)) {
        channel[i]++;
      }
      if (channel[i] == 24) {
        err_msg = "Current chennel equals 24!";
        return;
      }
      active_channels |= 1 << channel[i];
      ndspChnWaveBufClear(channel[i]);
    }
    static float mix[16];
    ndspChnSetFormat(channel[i],
                     NDSP_FORMAT_ADPCM | NDSP_3D_SURROUND_PREPROCESSED);
    ndspChnSetRate(channel[i], sample_rate);

    if (channel_count == 1) {
      mix[0] = mix[1] = 0.5f;
    } else if (channel_count == 2) {
      if (i == 0) {
        mix[0] = 0.8f;
        mix[1] = 0.0f;
        mix[2] = 0.2f;
        mix[3] = 0.0f;
      } else {
        mix[0] = 0.0f;
        mix[1] = 0.8f;
        mix[2] = 0.0f;
        mix[3] = 0.2f;
      }
    }
    ndspChnSetMix(channel[i], mix);
    ndspChnSetAdpcmCoefs(channel[i], adpcm_coefs[i]);

    for (unsigned int j = 0; j < buffer_count; j++) {
      memset(&wave_buf[i][j], 0, sizeof(ndspWaveBuf));
      wave_buf[i][j].status = NDSP_WBUF_DONE;
      buffer_data[i][j].resize(block_size);
    }
  }
  is_streaming = true;
}

void D7::BCSTM::Pause() {
  if (!is_streaming) return;
  is_paused = true;
  for (unsigned int i = 0; i < channel_count; i++) {
    ndspChnSetPaused(channel[i], true);
  }
}

void D7::BCSTM::Stop() {
  if (file) file.close();
  channel_count = 0;
  sample_rate = 0;
  loop_start = 0;
  loop_end = 0;
  num_blocks = 0;
  block_size = 0;
  block_samples = 0;
  last_block_size = 0;
  last_block_samples = 0;
  current_block = 0;
  info_offset = 0;
  data_offset = 0;
  active_channels = 0;
  is_paused = false;
  is_looping = false;
  err_msg = "None";
  is_loaded = false;
  if (!is_streaming) return;
  is_streaming = false;
  for (unsigned int i = 0; i < channel_count; i++) {
    ndspChnWaveBufClear(channel[i]);
    active_channels &= ~(1 << channel[i]);
  }
}

void D7::BCSTM::stream() {
  current_time = svcGetSystemTick();
  if (current_time - last_time >= 100000000 && is_loaded) {
    if (!is_streaming) return;
    if (!is_paused) fill_buffers();
    last_time = current_time;
  }
}

void D7::BCSTM::fill_buffers() {
  for (unsigned int bufIndex = 0; bufIndex < buffer_count; ++bufIndex) {
    if (wave_buf[0][bufIndex].status != NDSP_WBUF_DONE) continue;
    if (channel_count == 2 && wave_buf[1][bufIndex].status != NDSP_WBUF_DONE)
      continue;

    if (is_looping && current_block == loop_end) {
      current_block = loop_start;
      file.seekg(
          (data_offset + 0x20 + block_size * channel_count * loop_start));
    }
    if (!is_looping && current_block == loop_end) {
      this->Stop();
    }

    for (unsigned int channelIndex = 0; channelIndex < channel_count;
         ++channelIndex) {
      ndspWaveBuf *buf = &wave_buf[channelIndex][bufIndex];

      memset(buf, 0, sizeof(ndspWaveBuf));
      buf->data_adpcm = buffer_data[channelIndex][bufIndex].data();
      file.read(
          reinterpret_cast<char *>(buf->data_adpcm),
          (current_block == num_blocks - 1) ? last_block_size : block_size);
      DSP_FlushDataCache(buf->data_adpcm, block_size);

      if (current_block == 0)
        buf->adpcm_data = &adpcm_data[channelIndex][0];
      else if (current_block == loop_start)
        buf->adpcm_data = &adpcm_data[channelIndex][1];

      if (current_block == num_blocks - 1)
        buf->nsamples = last_block_samples;
      else
        buf->nsamples = block_samples;

      ndspChnWaveBufAdd(channel[channelIndex], buf);
    }

    current_block++;
  }
}

unsigned int D7::BCSTM::read32() {
  unsigned int ret;
  file.read(reinterpret_cast<char *>(&ret), sizeof(ret));
  return (is_little_endian ? ret : htonl(ret));
}

unsigned short D7::BCSTM::read16() {
  unsigned short ret;
  file.read(reinterpret_cast<char *>(&ret), sizeof(ret));
  return (is_little_endian ? ret : htons(ret));
}

unsigned char D7::BCSTM::read8() {
  unsigned char ret;
  file.read(reinterpret_cast<char *>(&ret), sizeof(ret));
  return ret;
}

bool D7::BCSTM::file_advance(unsigned long long bytes) {
  size_t seek_pos = (size_t)file.tellg() + bytes;
  file.seekg(seek_pos);
  return (!file.fail() && file.tellg() == seek_pos);
}