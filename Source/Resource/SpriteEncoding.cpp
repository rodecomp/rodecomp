#include "Resource/SpriteResource.h"

#include <cstddef>
#include <new>

// OFFSET: 0x00761070
// FUNCTION: fn_08063
std::uint8_t* EncodeSpriteRuns(const std::uint8_t* pixels, int width, int height, std::uint16_t& bytes) {
  std::vector<std::uint8_t> encoded;
  unsigned zeroCount = 0;
  bool zeroRun = false;
  std::uint32_t index = 0;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x, ++index) {
      const std::uint8_t pixel = pixels[static_cast<std::uint16_t>(index)];

      if (pixel == 0) {
        if (!zeroRun) {
          encoded.push_back(0);
          zeroRun = true;
        }

        if (++zeroCount >= 255) {
          encoded.push_back(static_cast<std::uint8_t>(zeroCount));
          zeroRun = false;
          zeroCount = 0;
        }
      } else {
        if (zeroRun) {
          encoded.push_back(static_cast<std::uint8_t>(zeroCount));
        }

        encoded.push_back(pixel);
        zeroRun = false;
        zeroCount = 0;
      }
    }
  }

  if (zeroCount != 0) {
    encoded.push_back(static_cast<std::uint8_t>(zeroCount));
  }

  bytes = static_cast<std::uint16_t>(encoded.size());
  auto* result = static_cast<std::uint8_t*>(::operator new(bytes));

  for (unsigned index = 0; index < bytes; ++index) {
    result[index] = encoded[index];
  }

  return result;
}

// OFFSET: 0x00760FE0
// FUNCTION: fn_08062
std::uint8_t* ReduceSprite(const std::uint8_t* pixels, int width, int height, int halfWidth, int halfHeight) {
  const int outputWidth = halfWidth != 0 ? width / 2 : width;
  const int outputHeight = halfHeight != 0 ? height / 2 : height;
  auto* output = static_cast<std::uint8_t*>(::operator new(static_cast<std::size_t>(outputWidth) * outputHeight));

  for (int y = 0; y < outputHeight; ++y) {
    const auto* source = pixels + width * y * (halfHeight + 1);

    for (int x = 0; x < outputWidth; ++x) {
      output[y * outputWidth + x] = source[x * (halfWidth + 1)];
    }
  }

  return output;
}

// OFFSET: 0x0056F7B0
// FUNCTION: fn_02405
void ConvertSpritePalette(std::uint32_t* output, const std::uint8_t* input, int count) {
  for (int index = 0; index < count; ++index) {
    const unsigned red = input[index * 4];
    const unsigned green = input[index * 4 + 1];
    const unsigned blue = input[index * 4 + 2];
    output[index] = red >= 254 && green < 4 && blue >= 254
                      ? 1057u
                      : ((blue | 0x40000u) >> 3) | (4 * ((green & 0xF8u) | (32 * (red & 0xF8u))));
  }
}
