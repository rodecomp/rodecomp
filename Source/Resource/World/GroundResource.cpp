#include "Resource/World/GroundResource.h"

#include <cstring>

// OFFSET: 0x0074E1C0
// FUNCTION: fn_07906
CGndRes::CGndRes() : loaded(0), width(0), height(0), cellSize(10.0f) {
}

// OFFSET: 0x0074E390
// OFFSET: 0x0074DD10
// FUNCTION: fn_07908
CGndRes::~CGndRes() = default;

// OFFSET: 0x0074DB10
// FUNCTION: fn_07899
CGndRes* CGndRes::Create() {
  return new CGndRes;
}

// OFFSET: 0x0074DC80
// FUNCTION: fn_07900
bool CGndRes::Load(const char* name) {
  if (!this->m_file.Open(name, 0, true, false)) {
    return false;
  }

  std::uint32_t magic;
  this->m_file.Read(&magic, sizeof(magic));

  if (magic != 0x4E475247u) {
    return false;
  }

  this->loaded = 1;
  return this->Parse();
}

// OFFSET: 0x0074E650
// FUNCTION: fn_07911
bool CGndRes::Parse() {
  this->m_file.Read(&this->major, 1);
  this->m_file.Read(&this->minor, 1);

  if (this->major != 1 || this->minor <= 6) {
    return false;
  }

  this->m_file.Read(&this->width, sizeof(this->width));
  this->m_file.Read(&this->height, sizeof(this->height));
  this->m_file.Read(&this->cellSize, sizeof(this->cellSize));
  this->m_file.Read(&this->textureCount, sizeof(this->textureCount));
  std::uint32_t nameBytes;
  this->m_file.Read(&nameBytes, sizeof(nameBytes));
  this->textureNames.resize(static_cast<std::uint32_t>(this->textureCount));

  for (int index = 0; index < this->textureCount; ++index) {
    this->textureNames[index] = static_cast<const char*>(this->m_file.ReadPtr(nameBytes));
  }

  this->m_file.Read(&this->lightmapCount, sizeof(this->lightmapCount));
  std::uint32_t unused[3];
  this->m_file.Read(&unused[0], sizeof(unused[0]));
  this->m_file.Read(&unused[1], sizeof(unused[1]));
  this->m_file.Read(&unused[2], sizeof(unused[2]));
  if (this->minor >= 7) {
    this->lightmaps = static_cast<const unsigned char*>(this->m_file.ReadPtr(static_cast<std::uint32_t>(this->lightmapCount) << 8));
  } else {
    this->legacyLightmapIndices = static_cast<const std::uint32_t*>(this->m_file.ReadPtr(static_cast<std::uint32_t>(this->lightmapCount) << 4));
    std::uint32_t count;
    this->m_file.Read(&count, sizeof(count));
    this->legacyLightmaps = static_cast<const GroundLegacyLightmap*>(this->m_file.ReadPtr(40u * count));
  }
  this->m_file.Read(&this->tileCount, sizeof(this->tileCount));
  this->tiles = static_cast<const unsigned char*>(this->m_file.ReadPtr(40u * static_cast<std::uint32_t>(this->tileCount)));
  this->cells = static_cast<const unsigned char*>(this->m_file.ReadPtr(28u * static_cast<std::uint32_t>(this->width) * static_cast<std::uint32_t>(this->height)));

  if (this->minor >= 8) {
    this->ReadWater();
  }

  return true;
}

// OFFSET: 0x0074E870
// FUNCTION: fn_07912
void CGndRes::ReadWater() {
  GroundWater record;
  this->m_file.Read(&record.parameters, sizeof(record.parameters));
  this->m_file.Read(&this->waterWidth, sizeof(this->waterWidth));
  this->m_file.Read(&this->waterHeight, sizeof(this->waterHeight));
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
      this->m_file.Read(&level, sizeof(level));
      this->waterHeights.push_back(level);
      record.parameters.level = level;
      std::memset(record.unknown_0018, 0, sizeof(record.unknown_0018));
      this->water.push_back(record);
    }
  } else if (this->minor == 9) {
    for (int index = 0; static_cast<float>(index) < count; ++index) {
      this->m_file.Read(&record.parameters, sizeof(record.parameters));
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

// OFFSET: 0x0074E4B0
void GroundLegacyLightmap::Decode(unsigned char* output) const {
  int pixel = 0;

  for (int bit = 0; bit < 320; bit += 5) {
    const int shift = bit % 8;
    const unsigned char* packed = this->bits + bit / 8;
    unsigned char value;

    if (shift >= 4) {
      value = static_cast<unsigned char>(((packed[0] & (248 >> shift)) << shift) + (((248 << (8 - shift)) & packed[1]) >> (8 - shift)));
    } else {
      value = static_cast<unsigned char>((packed[0] & (248 >> shift)) << shift);
    }

    output[pixel++] = value;
  }
}

// OFFSET: 0x0074E540
// FUNCTION: fn_07910
void CGndRes::Lightmap(int index, unsigned char* output) const {
  if (this->minor >= 7) {
    std::memcpy(output, this->lightmaps + (static_cast<std::uint32_t>(index) << 8), 256);
    return;
  }

  const std::uint32_t* indices = this->legacyLightmapIndices + 4 * index;
  unsigned char brightness[64];
  unsigned char red[64];
  unsigned char green[64];
  unsigned char blue[64];
  this->legacyLightmaps[indices[0]].Decode(brightness);
  this->legacyLightmaps[indices[1]].Decode(red);
  this->legacyLightmaps[indices[2]].Decode(green);
  this->legacyLightmaps[indices[3]].Decode(blue);

  for (int pixel = 0; pixel < 64; ++pixel) {
    const unsigned transposed = static_cast<unsigned>(pixel) / 8 + 8 * (pixel & 7);
    output[transposed] = brightness[pixel];
    output[64 + 3 * transposed] = red[pixel];
    output[65 + 3 * transposed] = green[pixel];
    output[66 + 3 * transposed] = blue[pixel];
  }
}
