#include "Contents/Ground/GroundGeometry.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/World/GroundResource.h"

#include <algorithm>
#include <array>

// OFFSET: 0x00AC3040
// FUNCTION: fn_17597
void GroundLightmapSet::Clear() {
  this->lightmaps.clear();

  while (this->lightmapTextures.size() != 0) {
    delete this->lightmapTextures.back();
    this->lightmapTextures.pop_back();
  }
}

// OFFSET: 0x00AC0360
// FUNCTION: fn_17565
void GroundLightmapSet::Build(CGndRes& resource) {
  this->Clear();
  this->lightmapCount = resource.lightmapCount;
  this->lightmaps.resize(static_cast<unsigned>(this->lightmapCount));
  this->atlasCount = (this->lightmapCount + 1023) / 1024;
  this->lightmapTextures.reserve(this->atlasCount);
  std::array<std::uint32_t, 256 * 256> pixels;

  for (int first = 0; first < this->lightmapCount; first += 1024) {
    const int atlas = first / 1024;
    for (int index = 0; index < 1024 && first + index < this->lightmapCount; ++index) {
      unsigned char decoded[256];
      resource.Lightmap(first + index, decoded);
      GroundLightmap& map = this->lightmaps[first + index];
      const int tile = first + index - atlas * 1024;
      const unsigned column = tile % 32;
      const unsigned row = tile / 32;

      for (unsigned y = 0; y < 8; ++y) {
        for (unsigned x = 0; x < 8; ++x) {
          const unsigned pixel = y * 8 + x;
          pixels[(row * 8 + y) * 256 + column * 8 + x] =
            (((((decoded[pixel] << 8) + decoded[64 + pixel * 3]) << 8) + decoded[65 + pixel * 3]) << 8) + decoded[66 + pixel * 3];
        }
      }

      const float left = static_cast<float>(8 * column + 1) / 256.0f;
      const float top = static_cast<float>(8 * row + 1) / 256.0f;
      map.uv[0][0] = std::clamp(left, 0.0f, 1.0f);
      map.uv[0][1] = std::clamp(top, 0.0f, 1.0f);
      const float right = static_cast<float>(8 * column + 7) / 256.0f;
      map.uv[1][0] = std::clamp(right, 0.0f, 1.0f);
      map.uv[1][1] = std::clamp(top, 0.0f, 1.0f);
      const float bottom = static_cast<float>(8 * row + 7) / 256.0f;
      map.uv[2][0] = std::clamp(left, 0.0f, 1.0f);
      map.uv[2][1] = std::clamp(bottom, 0.0f, 1.0f);
      map.uv[3][0] = std::clamp(right, 0.0f, 1.0f);
      map.uv[3][1] = std::clamp(bottom, 0.0f, 1.0f);
      map.color[0] = decoded[172];
      map.color[1] = decoded[173];
      map.color[2] = decoded[174];
      const unsigned samples[4] = {9, 13, 41, 45};

      for (unsigned corner = 0; corner < 4; ++corner) {
        const unsigned pixel = samples[corner];
        map.cornerColors[corner].a = decoded[pixel];
        map.cornerColors[corner].r = decoded[64 + pixel * 3];
        map.cornerColors[corner].g = decoded[65 + pixel * 3];
        map.cornerColors[corner].b = decoded[66 + pixel * 3];
      }
    }

    this->lightmapTextures.push_back(g_texMgr.Upload(256, 256, pixels.data(), 1, 0, false, false, false));
  }

  for (int index = 0; index < this->lightmapCount; ++index) {
    this->lightmaps[index].texture = this->lightmapTextures[index / 1024];
  }
}
