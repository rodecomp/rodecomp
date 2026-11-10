#include "Resource/Bitmap/BitmapImage.h"
#include "Resource/Bitmap/IJL15Runtime.h"

#include <cstdlib>

// OFFSET: 0x0074BCC0
// FUNCTION: fn_07871
bool BitmapImage::DecodeJpeg(const void* data, int bytes) {
  IJL15Properties properties = {};
  if (ijlInit(&properties) != 0) {
    return false;
  }

  properties.jpegSize = bytes;
  properties.jpegBytes = const_cast<unsigned char*>(static_cast<const unsigned char*>(data));
  unsigned char* source;

  if (ijlRead(&properties, 1) == 0) {
    this->width = properties.jpegWidth;
    this->height = properties.jpegHeight;
    source = new unsigned char[properties.jpegHeight * properties.jpegWidth * 3];
    this->pixels = new std::uint32_t[this->height * this->width];
    properties.dibWidth = this->width;
    properties.dibHeight = this->height;
    properties.dibBytes = source;
    properties.dibColor = 2;
    properties.dibChannels = 3;
    properties.dibPadding = 0;

    if (ijlRead(&properties, 3) != 0) {
      delete[] source;
      delete[] this->pixels;
      this->pixels = nullptr;

      return false;
    }
  } else {
    source = reinterpret_cast<unsigned char*>(this);
  }

  ijlFree(&properties);

  for (int y = 0; y < this->height; ++y) {
    std::uint32_t* out = &this->pixels[this->width * y];
    const unsigned char* input = &source[3 * this->width * y];
    if (!input) {
      return false;
    }

    for (int x = 0; x < this->width; ++x) {
      const unsigned red = input[2];
      const unsigned blue = input[0];
      const unsigned green = input[1];
      *out++ = red >= 254 && green <= 3 && blue >= 254 ? 0x000A0A0Au : (((((0xFFu << 8) + red) << 8) + green) << 8) + blue;
      input += 3;
    }
  }

  delete[] source;
  this->alpha = 0;
  return true;
}
