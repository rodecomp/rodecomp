#include "Contents/Models/ModelAnimation.h"

#include <cmath>

namespace {
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
void SampleLegacyPosition(std::span<const ModelPositionKey> keys, int frame, matrix& output) {
  if (keys.empty()) {
    output.Identity();
  } else if (keys.size() == 1) {
    output.MakeTranslate(keys[0].position[0], keys[0].position[1], keys[0].position[2]);
  } else {
    unsigned next = 1;

    while (keys[next].frame <= frame) {
      if (++next >= keys.size()) {
        return;
      }
    }

    const auto& a = keys[next - 1];
    const auto& b = keys[next];
    const float amount = static_cast<float>((frame - a.frame)) / static_cast<float>((b.frame - a.frame));
    const float previous = static_cast<float>(1.0 - static_cast<double>(amount));
    output.MakeTranslate(
      previous * a.position[0] + amount * b.position[0], previous * a.position[1] + amount * b.position[1], previous * a.position[2] + amount * b.position[2]
    );
  }
}

// OFFSET: 0x00540670
// FUNCTION: fn_01723
void SampleLegacyRotation(std::span<const ModelRotationKey> keys, int frame, matrix& output) {
  if (keys.empty()) {
    output.Identity();
    return;
  }

  if (keys.size() == 1) {
    QuaternionMatrix(keys[0], output);
    return;
  }

  unsigned next = 1;

  while (keys[next].frame <= frame) {
    if (++next >= keys.size()) {
      return;
    }
  }

  const auto& a = keys[next - 1];
  const auto& b = keys[next];

  if (a.rotation[3] == 1.0f && b.rotation[3] == 1.0f) {
    output.Identity();
    return;
  }

  const float amount = static_cast<float>((frame - a.frame)) / static_cast<float>((b.frame - a.frame));
  ModelRotationKey result;
  InterpolateModelRotation(amount, a, b, 0, result);
  QuaternionMatrix(result, output);
}

// OFFSET: 0x00540BF0
// FUNCTION: fn_01725
void SampleLegacyScale(std::span<const ModelScaleKey> keys, int, matrix& output) {
  if (keys.empty()) {
    output.Identity();
  } else if (keys.size() == 1) {
    output.SetScale(keys[0].scale[0], keys[0].scale[1], keys[0].scale[2]);
  }
}

// OFFSET: 0x00540C60
// FUNCTION: fn_01726
void SampleModelScale(std::span<const ModelRotationKey> keys, float frame, matrix& output) {
  if (keys.empty()) {
    output.Identity();
    return;
  }

  if (keys.size() == 1 || frame >= static_cast<float>(keys.back().frame)) {
    const auto& key = keys.size() == 1 ? keys[0] : keys.back();
    output.SetScale(key.rotation[0], key.rotation[1], key.rotation[2]);
    return;
  }

  unsigned next = 0;

  while (!(static_cast<float>(keys[next].frame) > frame)) {
    if (++next >= keys.size()) {
      output.SetScale(1, 1, 1);
      return;
    }
  }

  const ModelRotationKey first = next ? keys[next - 1] : ModelRotationKey{0, {1, 1, 1, 1}};
  const auto& second = keys[next];
  const float amount = second.frame != first.frame ? (frame - static_cast<float>(first.frame)) / static_cast<float>((second.frame - first.frame)) : 0.0f;
  output.SetScale(
    (1.0f - amount) * first.rotation[0] + second.rotation[0] * amount, (1.0f - amount) * first.rotation[1] + second.rotation[1] * amount,
    first.rotation[2] * (1.0f - amount) + second.rotation[2] * amount
  );
}

// OFFSET: 0x005404A0
// FUNCTION: fn_01722
void SampleModelPosition(std::span<const ModelRotationKey> keys, float frame, const matrix& base, matrix& output) {
  if (keys.empty()) {
    output.values[9] = base.values[9];
    output.values[10] = base.values[10];
    output.values[11] = base.values[11];

    return;
  }

  if (keys.size() == 1 || !(frame < static_cast<float>(keys.back().frame))) {
    const auto& key = keys.size() == 1 ? keys[0] : keys.back();
    output.values[9] = key.rotation[0];
    output.values[10] = key.rotation[1];
    output.values[11] = key.rotation[2];

    return;
  }

  unsigned next = 0;

  while (!(static_cast<float>(keys[next].frame) > frame)) {
    ++next;
  }

  const ModelRotationKey first = next ? keys[next - 1] : ModelRotationKey{0, {base.values[9], base.values[10], base.values[11], 0}};
  const auto& second = keys[next];
  const float amount = (frame - static_cast<float>(first.frame)) / static_cast<float>((second.frame - first.frame));
  const float previous = 1.0f - amount;

  output.values[9] = first.rotation[0] * previous + second.rotation[0] * amount;
  output.values[10] = first.rotation[1] * previous + second.rotation[1] * amount;
  output.values[11] = first.rotation[2] * previous + second.rotation[2] * amount;
}

// OFFSET: 0x005407A0
// FUNCTION: fn_01724
void SampleModelRotation(std::span<const ModelRotationKey> keys, float frame, const matrix& base, matrix& output) {
  matrix rotation;

  if (keys.empty()) {
    rotation = base;
  } else if (keys.size() == 1 || frame >= static_cast<float>(keys.back().frame)) {
    QuaternionMatrix(keys.size() == 1 ? keys[0] : keys.back(), rotation);
  } else {
    unsigned next = 0;

    while (!(static_cast<float>(keys[next].frame) > frame)) {
      if (++next >= keys.size()) {
        break;
      }
    }

    ModelRotationKey a = next < keys.size() ? (next ? keys[next - 1] : MatrixQuaternion(base)) : ModelRotationKey{0, {0, 0, 0, 1}};
    ModelRotationKey b = next < keys.size() ? keys[next] : ModelRotationKey{0, {0, 0, 0, 1}};
    const float amount = b.frame != a.frame ? (frame - static_cast<float>(a.frame)) / static_cast<float>((b.frame - a.frame)) : 0.0f;
    float dot = ((b.rotation[0] * a.rotation[0] + b.rotation[1] * a.rotation[1]) + b.rotation[2] * a.rotation[2]) + b.rotation[3] * a.rotation[3];

    if (dot < 0.0f) {
      for (float& value : a.rotation) {
        value = -value;
      }

      dot = -dot;
    }

    float firstWeight;
    float secondWeight;

    if (!(dot > -0.95f)) {
      b.rotation[0] = -a.rotation[1];
      b.rotation[1] = a.rotation[0];
      b.rotation[2] = -a.rotation[3];
      b.rotation[3] = a.rotation[2];
      firstWeight = ClientSin((0.5f - amount) * 3.141592f);
      secondWeight = ClientSin(amount * 3.141592f);
    } else if (dot > 0.95f) {
      firstWeight = 1.0f - amount;
      secondWeight = amount;
    } else {
      const float angle = static_cast<float>(std::acos(static_cast<double>(dot)));
      const float denominator = ClientSin(angle);
      firstWeight = ClientSin((1.0f - amount) * angle) / denominator;
      secondWeight = ClientSin(angle * amount) / denominator;
    }

    ModelRotationKey result{0, {}};
    result.rotation[0] = b.rotation[0] * secondWeight + firstWeight * a.rotation[0];
    result.rotation[1] = b.rotation[1] * secondWeight + firstWeight * a.rotation[1];
    result.rotation[2] = b.rotation[2] * secondWeight + a.rotation[2] * firstWeight;
    result.rotation[3] = b.rotation[3] * secondWeight + a.rotation[3] * firstWeight;
    QuaternionMatrix(result, rotation);
  }

  matrix previous = output;
  output.Compose(previous, rotation);
}
