#include "Contents/GroundGeometry.h"
#include "Render/Texture.h"
#include "Render/TextureCache.h"
#include "Resource/GroundResource.h"

#include <algorithm>
#include <array>

// OFFSET: 0x00AC3040
// FUNCTION: fn_17597
void GroundGeometry::ClearLightmaps() {
  this->lightmaps.clear();

  while (!this->lightmapTextures.empty()) {
    delete this->lightmapTextures.back();
    this->lightmapTextures.pop_back();
  }
}

// OFFSET: 0x00AC0360
// FUNCTION: fn_17565
void GroundGeometry::BuildLightmaps(CGndRes& resource, TextureCache& textures) {
  this->ClearLightmaps();
  this->lightmaps.resize(static_cast<unsigned>(resource.lightmapCount));
  const unsigned atlasCount = (static_cast<unsigned>(resource.lightmapCount) + 1023u) / 1024u;
  this->lightmapTextures.reserve(atlasCount);
  std::array<std::uint32_t, 256 * 256> pixels;

  for (int first = 0; first < resource.lightmapCount; first += 1024) {
    for (int index = 0; index < 1024 && first + index < resource.lightmapCount; ++index) {
      unsigned char decoded[256];
      resource.Lightmap(first + index, decoded);
      const unsigned column = static_cast<unsigned>(index) % 32;
      const unsigned row = static_cast<unsigned>(index) / 32;

      for (unsigned y = 0; y < 8; ++y) {
        for (unsigned x = 0; x < 8; ++x) {
          const unsigned pixel = y * 8 + x;
          pixels[(row * 8 + y) * 256 + column * 8 + x] =
            (static_cast<unsigned>(decoded[pixel]) << 24) | (static_cast<unsigned>(decoded[64 + pixel * 3]) << 16) |
            (static_cast<unsigned>(decoded[65 + pixel * 3]) << 8) | decoded[66 + pixel * 3];
        }
      }

      GroundLightmap& map = this->lightmaps[first + index];
      const float left = std::clamp(static_cast<float>(8 * column + 1) / 256.0f, 0.0f, 1.0f);
      const float right = std::clamp(static_cast<float>(8 * column + 7) / 256.0f, 0.0f, 1.0f);
      const float top = std::clamp(static_cast<float>(8 * row + 1) / 256.0f, 0.0f, 1.0f);
      const float bottom = std::clamp(static_cast<float>(8 * row + 7) / 256.0f, 0.0f, 1.0f);
      map.uv[0][0] = left;
      map.uv[0][1] = top;
      map.uv[1][0] = right;
      map.uv[1][1] = top;
      map.uv[2][0] = left;
      map.uv[2][1] = bottom;
      map.uv[3][0] = right;
      map.uv[3][1] = bottom;
      map.color[0] = decoded[172];
      map.color[1] = decoded[173];
      map.color[2] = decoded[174];
      const unsigned samples[4] = {9, 13, 41, 45};

      for (unsigned corner = 0; corner < 4; ++corner) {
        const unsigned pixel = samples[corner];
        map.unknown_0028[corner] = (static_cast<unsigned>(decoded[pixel]) << 24) |
                                   (static_cast<unsigned>(decoded[64 + pixel * 3]) << 16) |
                                   (static_cast<unsigned>(decoded[65 + pixel * 3]) << 8) | decoded[66 + pixel * 3];
      }
    }

    this->lightmapTextures.push_back(textures.Upload(256, 256, pixels.data(), 4, 0, false, false, false));
  }

  for (unsigned index = 0; index < this->lightmaps.size(); ++index) {
    this->lightmaps[index].texture = this->lightmapTextures[index / 1024];
  }
}
