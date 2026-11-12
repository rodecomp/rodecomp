#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/Bitmap/IJL15Runtime.h"

#include <cstdlib>

// OFFSET: 0x0074BCC0
// FUNCTION: fn_07871
bool CBitmapRes::DecodeJpeg(const void* data, int bytes) {
  IJL15Properties properties = {};
  if (ijlInit(&properties) != 0) {
    return false;
  }

  properties.jpegSize = bytes;
  properties.jpegBytes = const_cast<unsigned char*>(static_cast<const unsigned char*>(data));
  unsigned char* source;

  if (ijlRead(&properties, 1) == 0) {
    this->image.width = properties.jpegWidth;
    this->image.height = properties.jpegHeight;
    source = new unsigned char[properties.jpegHeight * properties.jpegWidth * 3];
    this->image.pixels = new std::uint32_t[this->image.height * this->image.width];
    properties.dibWidth = this->image.width;
    properties.dibHeight = this->image.height;
    properties.dibBytes = source;
    properties.dibColor = 2;
    properties.dibChannels = 3;
    properties.dibPadding = 0;

    if (ijlRead(&properties, 3) != 0) {
      delete[] source;
      delete[] this->image.pixels;
      this->image.pixels = nullptr;

      return false;
    }
  } else {
    source = reinterpret_cast<unsigned char*>(this);
  }

  ijlFree(&properties);

  for (int y = 0; y < this->image.height; ++y) {
    std::uint32_t* out = &this->image.pixels[this->image.width * y];
    const unsigned char* input = &source[3 * this->image.width * y];
    if (!input) {
      return false;
    }

    for (int x = 0; x < this->image.width; ++x) {
      const unsigned red = input[2];
      const unsigned blue = input[0];
      const unsigned green = input[1];
      *out++ = red >= 254 && green <= 3 && blue >= 254 ? 0x000A0A0Au : (((((0xFFu << 8) + red) << 8) + green) << 8) + blue;
      input += 3;
    }
  }

  delete[] source;
  this->image.alpha = 0;
  return true;
}
