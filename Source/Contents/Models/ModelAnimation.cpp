#include "Contents/Models/ModelAnimation.h"

#include <cmath>
#include <cstring>

namespace {
  __declspec(noinline) ModelRotationKey MatrixQuaternion(const matrix& matrix);

  // OFFSET: 0x00554340
  // FUNCTION: fn_02022
  ModelRotationKey MatrixQuaternion(const matrix& matrix) {
    ModelRotationKey result{0, {0, 0, 0, 1}};
    const float trace = ((matrix.values[0] + 1.0f) + matrix.values[4]) + matrix.values[8];
    const float root = std::sqrt(trace);
    const float w = root * 0.5f;
    result.rotation[3] = w;
    const float divisor = w * 4.0f;
    result.rotation[0] = (matrix.values[7] - matrix.values[5]) / divisor;
    result.rotation[1] = (matrix.values[2] - matrix.values[6]) / divisor;
    result.rotation[2] = (matrix.values[3] - matrix.values[1]) / divisor;

    return result;
  }
}

// OFFSET: 0x00540370
// FUNCTION: fn_01721
void LegacyPositionTrack::Sample(int frame, matrix& output) const {
  const std::vector<ModelPositionKey>& keys = *this;
  const unsigned count = keys.size();

  if (count == 0) {
    std::memcpy(output.values, g_identityMatrix, sizeof(g_identityMatrix));
    return;
  }

  if (count == 1) {
    output.MakeTranslate(keys[0].position[0], keys[0].position[1], keys[0].position[2]);
    return;
  }

  for (unsigned next = 1; next < count; ++next) {
    if (keys[next].frame > frame) {
      const auto& a = keys[next - 1];
      const auto& b = keys[next];
      const float amount = static_cast<float>((frame - a.frame)) / static_cast<float>((b.frame - a.frame));
      const float previous = static_cast<float>(1.0 - static_cast<double>(amount));
      output.MakeTranslate(
        previous * a.position[0] + amount * b.position[0], previous * a.position[1] + amount * b.position[1], previous * a.position[2] + amount * b.position[2]
      );
      return;
    }
  }
}

// OFFSET: 0x00540670
// FUNCTION: fn_01723
void LegacyRotationTrack::Sample(int frame, matrix& output) const {
  const std::vector<ModelRotationKey>& keys = *this;
  const unsigned count = keys.size();

  if (count == 0) {
    std::memcpy(output.values, g_identityMatrix, sizeof(g_identityMatrix));
    return;
  }

  if (count == 1) {
    QuaternionMatrix(keys[0], output);
    return;
  }

  unsigned next = 1;

  for (; next < count; ++next) {
    if (keys[next].frame > frame) {
      break;
    }
  }

  if (next >= count) {
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
  } else {
    const float amount = static_cast<float>((frame - a.frame)) / static_cast<float>((b.frame - a.frame));
    result.Interpolate(amount, a, b, 0);
  }

  QuaternionMatrix(result, output);
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
      if (next != 0) {
        first = keys[next - 1];
      }

      second = keys[next];
      break;
    }
  }

  const float amount = second.frame != first.frame ? (frame - static_cast<float>(first.frame)) / static_cast<float>((second.frame - first.frame)) : 0.0f;
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
    output.values[9] = keys[keys.size() - 1].rotation[0];
    output.values[10] = keys[keys.size() - 1].rotation[1];
    output.values[11] = keys[keys.size() - 1].rotation[2];
    return;
  }

  ModelPositionKey first;
  ModelPositionKey second;

  for (unsigned next = 0; next < count; ++next) {
    if (static_cast<float>(keys[next].frame) > frame) {
      if (next == 0) {
        first = ModelPositionKey{0, {base.values[9], base.values[10], base.values[11]}};
      } else {
        std::memcpy(&first, &keys[next - 1], sizeof(first));
      }

      std::memcpy(&second, &keys[next], sizeof(second));
      break;
    }
  }

  const float amount = (frame - static_cast<float>(first.frame)) / static_cast<float>((second.frame - first.frame));
  const float previous = 1.0f - amount;

  output.values[9] = first.position[0] * previous + second.position[0] * amount;
  output.values[10] = first.position[1] * previous + second.position[1] * amount;
  output.values[11] = first.position[2] * previous + second.position[2] * amount;
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
    QuaternionMatrix(keys[0], rotation);
  } else if (frame >= static_cast<float>(keys[count - 1].frame)) {
    QuaternionMatrix(keys[count - 1], rotation);
  } else {
    ModelRotationKey a{0, {0, 0, 0, 1}};
    ModelRotationKey b{0, {0, 0, 0, 1}};

    for (unsigned next = 0; next < count; ++next) {
      if (static_cast<float>(keys[next].frame) > frame) {
        if (next == 0) {
          a = MatrixQuaternion(base);
          a.frame = 0;
        } else {
          a = keys[next - 1];
        }

        b = keys[next];
        break;
      }
    }

    float amount = 0.0f;

    if (b.frame != a.frame) {
      amount = (frame - static_cast<float>(a.frame)) / static_cast<float>((b.frame - a.frame));
    }

    ModelRotationKey result{0, {0, 0, 0, 1}};
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

    float firstWeight;
    float secondWeight;

    if (dot > -0.95f) {
      if (dot > 0.95f) {
        firstWeight = 1.0f - amount;
        secondWeight = amount;
      } else {
        const float angle = static_cast<float>(std::acos(static_cast<double>(dot)));
        const float denominator = ClientSin(angle);
        firstWeight = ClientSin((1.0f - amount) * angle) / denominator;
        secondWeight = ClientSin(angle * amount) / denominator;
      }
    } else {
      bx = -ay;
      by = ax;
      bz = -aw;
      bw = az;
      firstWeight = ClientSin((0.5f - amount) * 3.141592f);
      secondWeight = ClientSin(amount * 3.141592f);
    }

    result.rotation[0] = bx * secondWeight + firstWeight * ax;
    result.rotation[1] = by * secondWeight + firstWeight * ay;
    result.rotation[2] = bz * secondWeight + az * firstWeight;
    result.rotation[3] = bw * secondWeight + aw * firstWeight;
    QuaternionMatrix(result, rotation);
  }

  matrix previous = output;
  output.Compose(previous, rotation);
}
