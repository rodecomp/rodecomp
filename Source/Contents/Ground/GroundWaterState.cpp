#include "Contents/Ground/GroundGeometry.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/World/GroundResource.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstring>

// OFFSET: 0x00AC3320
// FUNCTION: fn_17600
void GroundBase::SetWater(float newLevel, int newType, int newAnimation, int newPitch, int newPhase, float newHeight) {
  this->waterLevel = newLevel;
  this->waterAnimation = newAnimation;
  this->waterPitch = newPitch;
  this->waterPhase = newPhase % 360;
  this->waterHeight = newHeight;
  this->waterType = newType;
}

// OFFSET: 0x00AC30D0
// FUNCTION: fn_17599
void GroundGeometry::ImportWater(const CGndRes& resource) {
  if (resource.major == 1 && resource.minor >= 8) {
    this->waterColumns = resource.waterWidth;
    this->waterRows = resource.waterHeight;
    this->waterLevels.assign(resource.waterHeights.begin(), resource.waterHeights.end());

    this->waterSurfaces.assign(resource.water.begin(), resource.water.end());

    this->waterAnimation = resource.waterDefault.animation;
    this->waterPitch = static_cast<int>(resource.waterDefault.pitch);
    this->waterPhase = static_cast<int>(resource.waterDefault.speed) % 360;
    this->waterType = resource.waterDefault.type;
    this->waterHeight = resource.waterDefault.height;
  } else {
    this->waterSurfaces.push_back(
      {{this->waterLevel, this->waterType, this->waterHeight, static_cast<float>(this->waterPhase), static_cast<float>(this->waterPitch),
        this->waterAnimation},
       0,
       0.0f,
       nullptr}
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
    ++it->frame;
    const int end = it->parameters.animation * 32;

    if (end <= it->frame) {
      it->frame = 0;
    }

    const int image = (it->frame / it->parameters.animation) % 32;
    StringFormat(name, sizeof(name), _TRUNCATE, format, it->parameters.type, image);
    it->texture = g_texMgr.Load(name, 0, false, true, false);
    const float phase = it->parameters.speed + it->phase;
    it->phase = phase;

    if (phase > 180.0f) {
      it->phase = phase - 360.0f;
    }
  }
}
