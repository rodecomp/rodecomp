#include "Resource/Bitmap/BitmapImage.h"
#include "Resource/Bitmap/IJL15Runtime.h"

#include <cstdlib>

// OFFSET: 0x0074BCC0
// FUNCTION: fn_07871
bool BitmapImage::DecodeJpeg(const void* data, int bytes, IJL15Runtime& runtime) {
  IJL15Properties properties = {};
  if (runtime.initialize(&properties) != 0) {
    return false;
  }

  properties.jpegBytes = const_cast<unsigned char*>(static_cast<const unsigned char*>(data));
  properties.jpegSize = bytes;
  unsigned char* source;

  if (runtime.read(&properties, 1) == 0) {
    this->width = properties.jpegWidth;
    this->height = properties.jpegHeight;
    const std::uint32_t count = static_cast<std::uint32_t>(this->width) * static_cast<std::uint32_t>(this->height);
    source = static_cast<unsigned char*>(std::malloc(count * 3u));
    this->pixels = static_cast<std::uint32_t*>(std::malloc(count > 0x3FFFFFFFu ? 0xFFFFFFFFu : count * 4u));
    properties.dibWidth = this->width;
    properties.dibHeight = this->height;
    properties.dibBytes = source;
    properties.dibColor = 2;
    properties.dibChannels = 3;
    properties.dibPadding = 0;

    if (runtime.read(&properties, 3) != 0) {
      std::free(source);
      std::free(this->pixels);
      this->pixels = nullptr;

      return false;
    }
  } else {
    source = reinterpret_cast<unsigned char*>(this);
  }

  runtime.release(&properties);

  for (int y = 0; y < this->height; ++y) {
    const unsigned char* input = source + 3u * (static_cast<std::uint32_t>(y) * this->width);
    if (!input) {
      return false;
    }

    for (int x = 0; x < this->width; ++x, input += 3) {
      const unsigned blue = input[0], green = input[1], red = input[2];
      this->pixels[static_cast<std::uint32_t>(y) * this->width + x] =
        red >= 254 && green <= 3 && blue >= 254 ? 0x000A0A0Au : blue | (green << 8) | (red << 16) | 0xFF000000u;
    }
  }

  std::free(source);
  this->alpha = 0;
  return true;
}
