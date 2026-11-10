#include "Contents/Models/ModelAnimation.h"

#include <bit>
#include <cmath>

// OFFSET: 0x00554CD0
// FUNCTION: fn_02030
// OFFSET: 0x00554B90
// FUNCTION: fn_02029
void QuaternionMatrix(const ModelRotationKey& key, matrix& output) {
  const float x2 = key.rotation[0] + key.rotation[0];
  const float y2 = key.rotation[1] + key.rotation[1];
  const float z2 = key.rotation[2] + key.rotation[2];
  const float wx = key.rotation[3] * x2;
  const float wy = key.rotation[3] * y2;
  const float wz = key.rotation[3] * z2;
  const float xx = key.rotation[0] * x2;
  const float xy = key.rotation[0] * y2;
  const float xz = key.rotation[0] * z2;
  const float yy = key.rotation[1] * y2;
  const float yz = key.rotation[1] * z2;
  const float zz = key.rotation[2] * z2;

  output.values[0] = 1.0f - (yy + zz);
  output.values[1] = xy + wz;
  output.values[2] = xz - wy;
  output.values[3] = xy - wz;
  output.values[4] = 1.0f - (xx + zz);
  output.values[5] = yz + wx;
  output.values[6] = xz + wy;
  output.values[7] = yz - wx;
  output.values[8] = 1.0f - (xx + yy);
  output.values[9] = 0.0f;
  output.values[10] = 0.0f;
  output.values[11] = 0.0f;
}

// OFFSET: 0x00541B30
// FUNCTION: fn_01736
void InterpolateModelRotation(float amount, const ModelRotationKey& first, const ModelRotationKey& second, int spins, ModelRotationKey& output) {
  const float dot = ((second.rotation[1] * first.rotation[1] + second.rotation[0] * first.rotation[0]) + first.rotation[2] * second.rotation[2]) +
                    first.rotation[3] * second.rotation[3];
  const bool flip = dot < 0.0f;
  const float magnitude = flip ? -dot : dot;
  constexpr double epsilon = 0.000001;
  float firstWeight;
  float secondWeight;

  if (!(epsilon > 1.0 - static_cast<double>(magnitude))) {
    const float angle = static_cast<float>(std::acos(static_cast<double>(magnitude)));
    const float denominator = ClientSin(angle);
    const float phase = (static_cast<float>(spins) * 3.141592f + angle) * amount;

    firstWeight = ClientSin(angle - phase) / denominator;
    secondWeight = ClientSin(phase) / denominator;
  } else {
    firstWeight = static_cast<float>(1.0 - static_cast<double>(amount));
    secondWeight = amount;
  }

  if (flip) {
    secondWeight = -secondWeight;
  }

  output.rotation[0] = first.rotation[0] * firstWeight + second.rotation[0] * secondWeight;
  output.rotation[1] = secondWeight * second.rotation[1] + firstWeight * first.rotation[1];
  output.rotation[2] = firstWeight * first.rotation[2] + secondWeight * second.rotation[2];
  output.rotation[3] = firstWeight * first.rotation[3] + secondWeight * second.rotation[3];
}
