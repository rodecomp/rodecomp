#include "Resource/Sprites/SpriteResource.h"

#include <cstddef>

// OFFSET: 0x00761070
// FUNCTION: fn_08063
std::uint8_t* CSprRes::EncodeSpriteRuns(const std::uint8_t* pixels, int width, int height, std::uint16_t& bytes) {
  std::uint16_t zeroCount = 0;
  std::uint16_t index = 0;
  int zeroRun = 0;
  std::vector<std::uint8_t> encoded;

  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x, ++index) {
      if (pixels[index] == 0) {
        if (!zeroRun) {
          zeroRun = 1;
          encoded.push_back(0);
          ++zeroCount;
        } else if (++zeroCount >= 255) {
          encoded.push_back(static_cast<std::uint8_t>(zeroCount));
          zeroRun = 0;
          zeroCount = 0;
        }
      } else if (zeroRun) {
        encoded.push_back(static_cast<std::uint8_t>(zeroCount));
        encoded.push_back(pixels[index]);
        zeroRun = 0;
        zeroCount = 0;
      } else {
        zeroCount = 0;
        encoded.push_back(pixels[index]);
        zeroRun = 0;
      }
    }
  }

  if (zeroCount != 0) {
    encoded.push_back(static_cast<std::uint8_t>(zeroCount));
  }

  bytes = static_cast<std::uint16_t>(encoded.size());
  auto* result = new std::uint8_t[bytes];

  for (std::uint16_t index = 0; index < bytes; ++index) {
    result[index] = encoded[index];
  }

  return result;
}

// OFFSET: 0x00760FE0
// FUNCTION: fn_08062
std::uint8_t* CSprRes::ReduceSprite(const std::uint8_t* pixels, int width, int height, int halfWidth, int halfHeight) {
  const int outputWidth = halfWidth != 0 ? width / 2 : width;
  const int outputHeight = halfHeight != 0 ? height / 2 : height;
  auto* output = new std::uint8_t[outputWidth * outputHeight];

  for (int y = 0; y < outputHeight; ++y) {
    std::uint8_t* line = &output[y * outputWidth];
    for (int x = 0; x < outputWidth; ++x) {
      line[x] = pixels[(halfHeight + 1) * y * width + (halfWidth + 1) * x];
    }
  }

  return output;
}

// OFFSET: 0x0056F7B0
// FUNCTION: fn_02405
void ConvertSpritePalette(std::uint32_t* output, const std::uint8_t* input, int count) {
  for (int index = 0; index < count; ++index) {
    const std::uint8_t blue = input[index * 4 + 2];
    const std::uint8_t red = input[index * 4];
    const std::uint8_t green = input[index * 4 + 1];
    const std::uint32_t g = green;
    const std::uint32_t b = blue;
    const std::uint32_t r = red;
    if (red >= 254 && green < 4 && blue >= 254) {
      output[index] = 1057;
    } else {
      output[index] = ((b | 0x40000u) >> 3) | (4 * ((g & 0xF8u) | (32 * (r & 0xF8u))));
    }
  }
}

// OFFSET: 0x007612C0
// FUNCTION: fn_45828
std::uint8_t* CSprRes::DecodeSpriteRuns(const std::uint8_t* runs, int width, int height) {
  auto* pixels = new std::uint8_t[height * width];
  std::uint8_t* line;
  std::uint16_t zeroCount = 0;
  int value;
  const std::uint8_t* source = runs;

  for (int y = 0; y < height; ++y) {
    line = pixels + width * y;

    if (line) {
      for (int x = 0; x < width; ++x) {
        if (zeroCount == 0) {
          value = *source++;
          if (value == 0) {
            zeroCount = *source++;
          }
        }

        if (zeroCount) {
          *line++ = 0;
          --zeroCount;
        } else {
          *line++ = static_cast<std::uint8_t>(value);
        }
      }
    }
  }

  return pixels;
}
