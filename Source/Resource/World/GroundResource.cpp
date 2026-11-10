#include "Resource/World/GroundResource.h"
#include "Resource/ResourceMgr.h"

#include <cstring>

// OFFSET: 0x0074E1C0
// FUNCTION: fn_07906
CGndRes::CGndRes() {
  this->width = 0;
  this->height = 0;
  this->cellSize = 10.0f;
  this->loaded = 0;
}

// OFFSET: 0x0074E390
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

  char magic[4];
  this->m_file.Read(magic, sizeof(magic));

  if (std::memcmp(magic, "GRGN", 4) != 0) {
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
  std::uint32_t lightmapWidth;
  std::uint32_t lightmapHeight;
  std::uint32_t lightmapCells;
  this->m_file.Read(&lightmapWidth, sizeof(lightmapWidth));
  this->m_file.Read(&lightmapHeight, sizeof(lightmapHeight));
  this->m_file.Read(&lightmapCells, sizeof(lightmapCells));
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
  GroundWaterParameters parameters;
  GroundWater entry;
  this->m_file.Read(&parameters, sizeof(parameters));
  this->m_file.Read(&this->waterWidth, sizeof(this->waterWidth));
  this->m_file.Read(&this->waterHeight, sizeof(this->waterHeight));
  const float count = static_cast<float>(this->waterWidth * this->waterHeight);
  this->waterHeights.reserve(static_cast<std::uint32_t>(count));
  this->water.reserve(static_cast<std::uint32_t>(count));

  if (this->major != 1) {
    return;
  }

  if (this->minor == 8) {
    this->waterDefault = parameters;
    float level = 0.0f;

    for (int index = 0; static_cast<float>(index) < count; ++index) {
      this->m_file.Read(&level, sizeof(level));
      this->waterHeights.push_back(level);
      parameters.level = level;
      entry.parameters = parameters;
      entry.reserved[0] = 0;
      entry.reserved[1] = 0;
      entry.reserved[2] = 0;
      this->water.push_back(entry);
    }
  } else if (this->minor == 9) {
    for (int index = 0; static_cast<float>(index) < count; ++index) {
      this->m_file.Read(&parameters, sizeof(parameters));
      entry.parameters = parameters;
      entry.reserved[0] = 0;
      entry.reserved[1] = 0;
      entry.reserved[2] = 0;
      this->water.push_back(entry);
      this->waterHeights.push_back(parameters.level);
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
// FUNCTION: fn_07909
void GroundLegacyLightmap::Decode(unsigned char* output) const {
  for (int index = 0; index < 64; ++index) {
    const int bit = index * 5;
    const int shift = bit % 8;
    const unsigned char* packed = this->bits + bit / 8;
    const int mask = 248 >> shift;
    unsigned char value;

    if (shift < 4) {
      value = static_cast<unsigned char>((packed[0] & mask) << shift);
    } else {
      value = static_cast<unsigned char>(((packed[0] & mask) << shift) + (((248 << (8 - shift)) & packed[1]) >> (8 - shift)));
    }

    output[index] = value;
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

// INLINED
CGndRes::CGndRes(const char* extension, const char* prefix) {
  ResourceMgr::Instance().Register(extension, prefix, this);
}

// GLOBAL: gl_25556
CGndRes g_gndPrototype("gnd", "");
