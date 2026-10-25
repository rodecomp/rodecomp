#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/World/GroundResource.h"
#include "Resource/ResourceByteReader.h"

#include <bit>
#include <cstring>

namespace {
  const unsigned char* Slice(ResourceByteReader& input, std::uint32_t bytes) {
    const auto address = reinterpret_cast<std::uintptr_t>(input.data) + input.position;
    input.position += bytes;
    return reinterpret_cast<const unsigned char*>(address);
  }
}

// OFFSET: 0x0074E1C0
// FUNCTION: fn_07906
CGndRes::CGndRes(GroundResourceContext& context) : loaded(0), width(0), height(0), cellSize(10.0f), m_context(&context) {
}

// OFFSET: 0x0074E390
// OFFSET: 0x0074DD10
// FUNCTION: fn_07908
CGndRes::~CGndRes() = default;

// OFFSET: 0x0074DB10
// FUNCTION: fn_07899
CGndRes* CGndRes::Create() {
  return new CGndRes(*this->m_context);
}

// OFFSET: 0x0074DC80
// FUNCTION: fn_07900
bool CGndRes::Load(const char* name) {
  auto& file = this->m_context->file;

  if (!file.Open(name)) {
    return false;
  }

  const auto* bytes = static_cast<const unsigned char*>(file.Data());
  this->m_bytes.assign(bytes, bytes + file.Size());
  file.Close();
  ResourceByteReader input{this->m_bytes.data(), static_cast<unsigned>(this->m_bytes.size())};
  std::uint32_t magic;
  input.Read(&magic, sizeof(magic));

  if (magic != 0x4E475247u) {
    return false;
  }

  this->loaded = 1;
  return this->Parse(input);
}

// OFFSET: 0x0074E650
// FUNCTION: fn_07911
bool CGndRes::Parse(ResourceByteReader& input) {
  input.Read(&this->major, 1);
  input.Read(&this->minor, 1);

  if (this->major != 1 || this->minor <= 6) {
    return false;
  }

  input.Read(&this->width, sizeof(this->width));
  input.Read(&this->height, sizeof(this->height));
  input.Read(&this->cellSize, sizeof(this->cellSize));
  input.Read(&this->textureCount, sizeof(this->textureCount));
  std::uint32_t nameBytes;
  input.Read(&nameBytes, sizeof(nameBytes));
  this->textureNames.resize(static_cast<std::uint32_t>(this->textureCount));

  for (int index = 0; index < this->textureCount; ++index) {
    this->textureNames[index] = reinterpret_cast<const char*>(Slice(input, nameBytes));
  }

  input.Read(&this->lightmapCount, sizeof(this->lightmapCount));
  std::uint32_t unused[3];
  input.Read(&unused[0], sizeof(unused[0]));
  input.Read(&unused[1], sizeof(unused[1]));
  input.Read(&unused[2], sizeof(unused[2]));
  this->lightmaps = Slice(input, static_cast<std::uint32_t>(this->lightmapCount) << 8);
  input.Read(&this->tileCount, sizeof(this->tileCount));
  this->tiles = Slice(input, 40u * static_cast<std::uint32_t>(this->tileCount));
  this->cells = Slice(input, 28u * static_cast<std::uint32_t>(this->width) * static_cast<std::uint32_t>(this->height));

  if (this->minor >= 8) {
    this->ReadWater(input);
  }

  return true;
}

// OFFSET: 0x0074E870
// FUNCTION: fn_07912
void CGndRes::ReadWater(ResourceByteReader& input) {
  GroundWater record;
  input.Read(&record.parameters, sizeof(record.parameters));
  input.Read(&this->waterWidth, sizeof(this->waterWidth));
  input.Read(&this->waterHeight, sizeof(this->waterHeight));
  const float count = static_cast<float>(this->waterWidth * this->waterHeight);
  const auto reserveCount = count < 0.0f ? static_cast<std::uint32_t>(-1) : static_cast<std::uint32_t>(count);
  this->waterHeights.reserve(reserveCount);
  this->water.reserve(reserveCount);

  if (this->major != 1) {
    return;
  }

  if (this->minor == 8) {
    this->waterDefault = record.parameters;
    float level = 0.0f;

    for (int index = 0; static_cast<float>(index) < count; ++index) {
      input.Read(&level, sizeof(level));
      this->waterHeights.push_back(level);
      record.parameters.level = level;
      std::memset(record.unknown_0018, 0, sizeof(record.unknown_0018));
      this->water.push_back(record);
    }
  } else if (this->minor == 9) {
    for (int index = 0; static_cast<float>(index) < count; ++index) {
      input.Read(&record.parameters, sizeof(record.parameters));
      std::memset(record.unknown_0018, 0, sizeof(record.unknown_0018));
      this->water.push_back(record);
      this->waterHeights.push_back(record.parameters.level);
    }
  }
}

// OFFSET: 0x0074DCE0
// FUNCTION: fn_07901
void CGndRes::Unload() {
  this->textureNames.clear();
  this->lightmaps = nullptr;
  this->tiles = nullptr;
  this->cells = nullptr;
}

// OFFSET: 0x0074E540
// FUNCTION: fn_07910
void CGndRes::Lightmap(int index, unsigned char* output) const {
  if (this->minor >= 7) {
    std::memcpy(output, this->lightmaps + (static_cast<std::uint32_t>(index) << 8), 256);
    return;
  }

  std::uint32_t indices[4];
  std::memcpy(indices, this->legacyLightmapIndices + 16u * static_cast<std::uint32_t>(index), sizeof(indices));
  unsigned char decoded[4][64];

  for (unsigned channel = 0; channel < 4; ++channel) {
    const unsigned char* packed = this->legacyLightmapBytes + 40u * indices[channel];

    for (unsigned bit = 0, pixel = 0; bit < 320; bit += 5, ++pixel) {
      const unsigned shift = bit % 8;
      unsigned value = ((248u >> shift) & packed[bit / 8]) << shift;

      if (shift >= 4) {
        value += ((248u << (8 - shift)) & packed[bit / 8 + 1]) >> (8 - shift);
      }

      decoded[channel][pixel] = static_cast<unsigned char>(value);
    }
  }

  for (unsigned pixel = 0; pixel < 64; ++pixel) {
    const unsigned transposed = pixel / 8 + 8 * (pixel & 7);
    output[transposed] = decoded[0][pixel];
    output[64 + 3 * transposed] = decoded[1][pixel];
    output[65 + 3 * transposed] = decoded[2][pixel];
    output[66 + 3 * transposed] = decoded[3][pixel];
  }
}
