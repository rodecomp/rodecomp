#include "Contents/Models/ModelAnimation.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Render/Projection/RenderProjection.h"

#include <cmath>
#include <cstring>

// OFFSET: 0x00554340
// FUNCTION: fn_02022
__declspec(noinline) ModelRotationKey matrix::GetRotation() const {
  ModelRotationKey result{0, {0, 0, 0, 1}};
  const float trace = ((this->values[0] + 1.0f) + this->values[4]) + this->values[8];
  const float root = std::sqrt(trace);
  const float w = root * 0.5f;
  result.rotation[3] = w;
  const float divisor = w * 4.0f;
  result.rotation[0] = (this->values[7] - this->values[5]) / divisor;
  result.rotation[1] = (this->values[2] - this->values[6]) / divisor;
  result.rotation[2] = (this->values[3] - this->values[1]) / divisor;

  return result;
}

// OFFSET: 0x00540370
// FUNCTION: fn_01721
void LegacyPositionTrack::Sample(int frame, matrix& output) const {
  const std::vector<ModelPositionKey>& keys = *this;
  const unsigned count = keys.size();

  if (count == 0) {
    output.Identity();
    return;
  }

  if (count == 1) {
    output.MakeTranslate(keys[0].position[0], keys[0].position[1], keys[0].position[2]);
    return;
  }

  bool found = false;
  unsigned next = 1;

  for (; next < count; ++next) {
    if (keys[next].frame > frame) {
      found = true;
      break;
    }
  }

  if (!found) {
    return;
  }

  const auto& a = keys[next - 1];
  const auto& b = keys[next];
  const float amount = static_cast<float>(frame - a.frame) / static_cast<float>(b.frame - a.frame);
  const float previous = static_cast<float>(1.0 - static_cast<double>(amount));
  const vector3d position{previous * a.position[0] + amount * b.position[0], previous * a.position[1] + amount * b.position[1], previous * a.position[2] + amount * b.position[2]};
  output.MakeTranslate(position.x, position.y, position.z);
}

// OFFSET: 0x00540670
// FUNCTION: fn_01723
__declspec(safebuffers) void LegacyRotationTrack::Sample(int frame, matrix& output) const {
  const std::vector<ModelRotationKey>& keys = *this;
  const unsigned count = keys.size();

  if (count == 0) {
    std::memcpy(output.values, g_identityMatrix, sizeof(g_identityMatrix));
    return;
  }

  if (count == 1) {
    output.MakeRotation(keys[0]);
    return;
  }

  bool found = false;
  unsigned next = 1;

  for (; next < count; ++next) {
    if (keys[next].frame > frame) {
      found = true;
      break;
    }
  }

  if (!found) {
    return;
  }

  const auto& a = keys[next - 1];
  const auto& b = keys[next];

  if (a.rotation[3] == 1.0f && b.rotation[3] == 1.0f) {
    std::memcpy(output.values, g_identityMatrix, sizeof(g_identityMatrix));
    return;
  }

  ModelRotationKey result;

  if (next == 0) {
    result.Interpolate(0.0f, keys[0], keys[1], 0);
    output.MakeRotation(result);
  } else {
    const float amount = static_cast<float>((frame - a.frame)) / static_cast<float>((b.frame - a.frame));
    result.Interpolate(amount, a, b, 0);
    output.MakeRotation(result);
  }
}

// OFFSET: 0x00540BF0
// FUNCTION: fn_01725
void LegacyScaleTrack::Sample(int, matrix& output) const {
  const std::vector<ModelScaleKey>& keys = *this;
  const unsigned count = keys.size();

  if (count == 0) {
    std::memcpy(output.values, g_identityMatrix, sizeof(g_identityMatrix));
  } else if (count == 1) {
    output.SetScale(keys[0].scale[0], keys[0].scale[1], keys[0].scale[2]);
  }
}

// OFFSET: 0x00540C60
// FUNCTION: fn_01726
void ModelScaleTrack::Sample(float frame, matrix& output) const {
  const std::vector<ModelRotationKey>& keys = *this;
  const unsigned count = keys.size();

  if (count == 0) {
    std::memcpy(output.values, g_identityMatrix, sizeof(g_identityMatrix));
    return;
  }

  if (count == 1) {
    output.SetScale(keys[0].rotation[0], keys[0].rotation[1], keys[0].rotation[2]);
    return;
  }

  const auto& last = keys[count - 1];

  if (frame >= static_cast<float>(last.frame)) {
    output.SetScale(last.rotation[0], last.rotation[1], last.rotation[2]);
    return;
  }

  matrix scale;
  std::memcpy(scale.values, g_identityMatrix, sizeof(g_identityMatrix));
  ModelRotationKey first{0, {1, 1, 1, 1}};
  ModelRotationKey second{0, {1, 1, 1, 1}};

  for (unsigned next = 0; next < count; ++next) {
    if (static_cast<float>(keys[next].frame) > frame) {
      if (next == 0) {
        second = keys[0];
      } else {
        first = keys[next - 1];
        second = keys[next];
      }

      break;
    }
  }

  float amount = 0.0f;

  if (second.frame != first.frame) {
    amount = (frame - static_cast<float>(first.frame)) / static_cast<float>((second.frame - first.frame));
  }

  scale.SetScale(
    (1.0f - amount) * first.rotation[0] + second.rotation[0] * amount, (1.0f - amount) * first.rotation[1] + second.rotation[1] * amount,
    first.rotation[2] * (1.0f - amount) + second.rotation[2] * amount
  );
  output = scale;
}

// OFFSET: 0x005404A0
// FUNCTION: fn_01722
void ModelPositionTrack::Sample(float frame, const matrix& base, matrix& output) const {
  const std::vector<ModelRotationKey>& keys = *this;
  const unsigned count = keys.size();

  if (count == 0) {
    output.values[9] = base.values[9];
    output.values[10] = base.values[10];
    output.values[11] = base.values[11];
    return;
  }

  if (count == 1) {
    output.values[9] = keys[0].rotation[0];
    output.values[10] = keys[0].rotation[1];
    output.values[11] = keys[0].rotation[2];
    return;
  }

  if (frame >= static_cast<float>(keys[count - 1].frame)) {
    output.values[9] = keys[count - 1].rotation[0];
    output.values[10] = keys[keys.size() - 1].rotation[1];
    output.values[11] = keys[keys.size() - 1].rotation[2];
    return;
  }

  ModelRotationKey first;
  ModelRotationKey second;

  for (unsigned next = 0; next < count; ++next) {
    if (static_cast<float>(keys[next].frame) > frame) {
      if (next == 0) {
        ModelRotationKey start;
        start.frame = 0;
        start.rotation[0] = base.values[9];
        start.rotation[1] = base.values[10];
        start.rotation[2] = base.values[11];
        first = start;
        second = keys[0];
      } else {
        first = keys[next - 1];
        second = keys[next];
      }

      break;
    }
  }

  const float amount = (frame - static_cast<float>(first.frame)) / static_cast<float>((second.frame - first.frame));
  const float previous = 1.0f - amount;

  const vector3d position{
    first.rotation[0] * previous + second.rotation[0] * amount, first.rotation[1] * previous + second.rotation[1] * amount,
    first.rotation[2] * previous + second.rotation[2] * amount
  };

  output.values[9] = position.x;
  output.values[10] = position.y;
  output.values[11] = position.z;
}

// OFFSET: 0x005407A0
// FUNCTION: fn_01724
void ModelRotationTrack::Sample(float frame, const matrix& base, matrix& output) const {
  const std::vector<ModelRotationKey>& keys = *this;
  matrix rotation;
  std::memcpy(rotation.values, g_identityMatrix, sizeof(g_identityMatrix));
  const unsigned count = keys.size();

  if (count == 0) {
    rotation = base;
  } else if (count == 1) {
    rotation.MakeRotation(keys[0]);
  } else if (frame >= static_cast<float>(keys[count - 1].frame)) {
    rotation.MakeRotation(keys[count - 1]);
  } else {
    ModelRotationKey a{0, {0, 0, 0, 1}};
    ModelRotationKey b{0, {0, 0, 0, 1}};

    for (unsigned next = 0; next < count; ++next) {
      if (static_cast<float>(keys[next].frame) > frame) {
        if (next == 0) {
          a = base.GetRotation();
          a.frame = 0;
          b = keys[0];
        } else {
          a = keys[next - 1];
          b = keys[next];
        }

        break;
      }
    }

    float amount = 0.0f;

    if (b.frame != a.frame) {
      amount = (frame - static_cast<float>(a.frame)) / static_cast<float>((b.frame - a.frame));
    }

    float ax = a.rotation[0];
    float ay = a.rotation[1];
    float az = a.rotation[2];
    float aw = a.rotation[3];
    float bx = b.rotation[0];
    float by = b.rotation[1];
    float bz = b.rotation[2];
    float bw = b.rotation[3];
    float dot = ((bx * ax + by * ay) + bz * az) + bw * aw;

    if (dot < 0.0f) {
      ax = -ax;
      ay = -ay;
      az = -az;
      aw = -aw;
      dot = -dot;
    }

    ModelRotationKey result{0, {0, 0, 0, 1}};

    float firstWeight;
    float secondWeight;

    if (dot > -0.95f) {
      firstWeight = g_projectionDepthNumerator - amount;

      if (dot > 0.95f) {
        secondWeight = amount;
      } else {
        const float angle = static_cast<float>(std::acos(static_cast<double>(dot)));
        const float denominator = ClientSin(angle);
        firstWeight = ClientSin(firstWeight * angle) / denominator;
        secondWeight = ClientSin(angle * amount) / denominator;
      }
    } else {
      bx = -ay;
      by = ax;
      bz = -aw;
      bw = az;
      firstWeight = ClientSin((g_depthAverageHalf - amount) * 3.141592f);
      secondWeight = ClientSin(amount * 3.141592f);
    }

    result.rotation[0] = bx * secondWeight + firstWeight * ax;
    result.rotation[1] = by * secondWeight + firstWeight * ay;
    result.rotation[2] = bz * secondWeight + az * firstWeight;
    result.rotation[3] = bw * secondWeight + aw * firstWeight;
    rotation.MakeRotation(result);
  }

  matrix previous = output;
  output.Compose(previous, rotation);
}
