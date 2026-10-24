#include "Contents/GroundWaterState.h"
#include "Render/TextureCache.h"
#include "Resource/GroundResource.h"

#include <algorithm>
#include <bit>
#include <cstdio>
#include <cstring>
#include <emmintrin.h>

// OFFSET: 0x00ABD640
// FUNCTION: fn_17546
GroundWaterState::GroundWaterState()
    : level(0.0f), animation(3), pitch(50), phase(2), type(0), height(1.0f), columns(1), rows(1), partitioned(false) {
}

// OFFSET: 0x00AC3320
// FUNCTION: fn_17600
void GroundWaterState::Set(float newLevel, int newType, int newAnimation, int newPitch, int newPhase, float newHeight) {
  this->animation = newAnimation;
  this->pitch = newPitch;
  this->level = newLevel;
  this->height = newHeight;
  this->phase = newPhase % 360;
  this->type = newType;
}

// OFFSET: 0x00AC30D0
// FUNCTION: fn_17599
bool GroundWaterState::Import(const CGndRes& resource, TextureCache& textures) {
  if (resource.major == 1 && resource.minor >= 8) {
    this->columns = resource.waterWidth;
    this->rows = resource.waterHeight;
    this->levels = resource.waterHeights;
    this->surfaces.resize(resource.water.size());
    static_assert(sizeof(GroundWaterSurface) == sizeof(GroundWater));

    if (!resource.water.empty()) {
      std::memcpy(this->surfaces.data(), resource.water.data(), resource.water.size() * sizeof(GroundWater));
    }

    this->animation = std::bit_cast<int>(resource.waterDefault[5]);
    this->pitch = _mm_cvttss_si32(_mm_set_ss(std::bit_cast<float>(resource.waterDefault[4])));
    this->phase = _mm_cvttss_si32(_mm_set_ss(std::bit_cast<float>(resource.waterDefault[3]))) % 360;
    this->type = std::bit_cast<int>(resource.waterDefault[1]);
    this->height = std::bit_cast<float>(resource.waterDefault[2]);
  } else {
    this->surfaces.push_back(
      {this->level, this->type, this->height, static_cast<float>(this->phase), static_cast<float>(this->pitch),
       this->animation, 0, 0.0f, nullptr}
    );
    this->levels.push_back(std::bit_cast<std::uint32_t>(this->level));
  }

  this->Update(textures);
  this->partitioned = this->levels.size() > 1;
  return this->partitioned;
}

// OFFSET: 0x00AC1390
// FUNCTION: fn_17580
float GroundWaterState::Level(int width, int height, int column, int row) const {
  const int cellWidth = width / this->columns;
  const int cellHeight = height / this->rows;
  const int x = std::min(column / cellWidth, this->columns - 1);
  const int z = std::min(row / cellHeight, this->rows - 1);
  const unsigned index = static_cast<unsigned>(x) + static_cast<unsigned>(z) * static_cast<unsigned>(this->columns);
  return index < this->levels.size() ? std::bit_cast<float>(this->levels[index]) : 0.0f;
}

// OFFSET: 0x00AC1430
// FUNCTION: fn_17581
void GroundWaterState::Update(TextureCache& textures) {
  // STATIC: 0x01159248; original client-code-page bytes, not a UTF-8 path.
  constexpr char format[] = "\xBF\xF6\xC5\xCD\\water%d%02d.jpg";
  char name[32]{};

  for (GroundWaterSurface& surface : this->surfaces) {
    surface.frame = std::bit_cast<int>(static_cast<unsigned>(surface.frame) + 1u);
    const int end = std::bit_cast<int>(static_cast<unsigned>(surface.animation) << 5);

    if (end <= surface.frame) {
      surface.frame = 0;
    }

    const int image = (surface.frame / surface.animation) % 32;
    _snprintf_s(name, sizeof(name), _TRUNCATE, format, surface.type, image);
    surface.texture = textures.Load(name, 0, false, true, false);
    const float phase = surface.speed + surface.phase;
    surface.phase = phase;

    if (phase > 180.0f) {
      surface.phase = phase - 360.0f;
    }
  }
}
