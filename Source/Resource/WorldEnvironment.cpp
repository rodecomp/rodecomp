#include "Resource/ResourceByteReader.h"
#include "Resource/WorldResource.h"
#include "Utility/AffineMatrix.h"

#include <bit>

// OFFSET: 0x00761CC0
// FUNCTION: fn_08073
void C3dWorldRes::ReadEnvironment(ResourceByteReader& input, unsigned version) {
  if (version < 0x206u) {
    if (version >= 0x103u) {
      input.Read(&this->waterLevel, sizeof(this->waterLevel));
    }

    if (version >= 0x108u) {
      input.Read(&this->waterType, sizeof(this->waterType));
      input.Read(&this->waveHeight, sizeof(this->waveHeight));
      input.Read(&this->waveSpeed, sizeof(this->waveSpeed));
      input.Read(&this->wavePitch, sizeof(this->wavePitch));
    } else {
      this->waterType = 0;
      this->waveHeight = 1.0f;
      this->waveSpeed = 2.0f;
      this->wavePitch = 50.0f;
    }

    if (version >= 0x109u) {
      input.Read(&this->waterAnimation, sizeof(this->waterAnimation));
    } else {
      this->waterAnimation = 3;
    }
  }

  if (version >= 0x105u) {
    input.Read(&this->longitude, sizeof(this->longitude));
    input.Read(&this->latitude, sizeof(this->latitude));
    input.Read(this->diffuse, sizeof(this->diffuse));
    input.Read(this->ambient, sizeof(this->ambient));
  } else {
    this->latitude = 45;
    this->longitude = 45;

    for (float& value : this->diffuse) {
      value = 1.0f;
    }

    for (float& value : this->ambient) {
      value = 0.3f;
    }
  }

  constexpr float pi = 3.141592f;
  AffineMatrix matrix;
  matrix.SetXRotation((static_cast<float>(this->latitude) / 180.0f) * pi);
  matrix.RotateY((static_cast<float>(this->longitude) / 180.0f) * pi);

  for (unsigned component = 0; component < 3; ++component) {
    this->lightDirection[component] =
      ((matrix.values[component] * 0.0f + matrix.values[component + 3]) + matrix.values[component + 6] * 0.0f) +
      matrix.values[component + 9];
  }

  if (version >= 0x107u) {
    unsigned unused;
    input.Read(&unused, sizeof(unused));
  }

  if (version >= 0x106u) {
    for (int& bound : this->bounds) {
      input.Read(&bound, sizeof(bound));
    }
  } else {
    this->bounds[0] = -500;
    this->bounds[1] = 500;
    this->bounds[2] = -500;
    this->bounds[3] = 500;
  }
}
