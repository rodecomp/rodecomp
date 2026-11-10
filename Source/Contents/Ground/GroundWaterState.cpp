#include "Contents/Ground/GroundGeometry.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/World/GroundResource.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00AC3320
// FUNCTION: fn_17600
void GroundBase::SetWater(float newLevel, int newType, int newAnimation, int newPitch, int newPhase, float newHeight) {
  this->waterAnimation = newAnimation;
  this->waterPitch = newPitch;
  this->waterLevel = newLevel;
  this->waterHeight = newHeight;
  this->waterPhase = newPhase % 360;
  this->waterType = newType;
}

// OFFSET: 0x00AC30D0
// FUNCTION: fn_17599
void GroundGeometry::ImportWater(const CGndRes& resource) {
  if (resource.major == 1 && resource.minor >= 8) {
    this->waterColumns = resource.waterWidth;
    this->waterRows = resource.waterHeight;
    this->waterLevels = resource.waterHeights;
    this->waterSurfaces.resize(resource.water.size());

    static_assert(sizeof(GroundWaterSurface) == sizeof(GroundWater));
    if (!resource.water.empty()) {
      std::memcpy(this->waterSurfaces.data(), resource.water.data(), resource.water.size() * sizeof(GroundWater));
    }

    this->waterAnimation = resource.waterDefault.animation;
    this->waterPitch = static_cast<int>(resource.waterDefault.pitch);
    this->waterPhase = static_cast<int>(resource.waterDefault.speed) % 360;
    this->waterType = resource.waterDefault.type;
    this->waterHeight = resource.waterDefault.height;
  } else {
    this->waterSurfaces.push_back(
      {this->waterLevel, this->waterType, this->waterHeight, static_cast<float>(this->waterPhase), static_cast<float>(this->waterPitch),
       this->waterAnimation, 0, 0.0f, nullptr}
    );
    this->waterLevels.push_back(this->waterLevel);
  }

  this->UpdateWater();
  this->waterPartitioned = this->waterLevels.size() > 1;
}

// OFFSET: 0x00AC1390
// FUNCTION: fn_17580
float GroundGeometry::WaterLevel(int column, int row) const {
  const int cellWidth = this->width / this->waterColumns;
  const int cellHeight = this->height / this->waterRows;
  const int x = std::min(column / cellWidth, this->waterColumns - 1);
  const int z = std::min(row / cellHeight, this->waterRows - 1);
  const unsigned index = static_cast<unsigned>(x) + static_cast<unsigned>(z) * static_cast<unsigned>(this->waterColumns);

  if (this->waterLevels.size() > index) {
    return this->waterLevels[index];
  }

  return 0.0f;
}

// OFFSET: 0x00AC1430
// FUNCTION: fn_17581
void GroundGeometry::UpdateWater() {
  // STATIC: 0x01159248
  // STRING: st_16830
  static constexpr char format[] = "워터\\water%d%02d.jpg";
  char name[32]{};

  for (auto it = this->waterSurfaces.begin(); it != this->waterSurfaces.end(); ++it) {
    GroundWaterSurface& surface = *it;
    ++surface.frame;
    const int end = surface.animation * 32;

    if (end <= surface.frame) {
      surface.frame = 0;
    }

    const int image = (surface.frame / surface.animation) % 32;
    StringFormat(name, sizeof(name), _TRUNCATE, format, surface.type, image);
    surface.texture = g_texMgr.Load(name, 0, false, true, false);
    const float phase = surface.speed + surface.phase;
    surface.phase = phase;

    if (phase > 180.0f) {
      surface.phase = phase - 360.0f;
    }
  }
}
