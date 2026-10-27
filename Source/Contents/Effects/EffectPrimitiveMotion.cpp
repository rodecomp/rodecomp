#include "Contents/Effects/EffectPrimitiveMotion.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/RagEffectState.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <bit>
#include <cmath>
#include <cstring>

namespace {
  // INLINED
  EffectVector3 Forward(float distance, const float* matrix) {
    const EffectVector3 point{0.0f, 0.0f, distance};
    return {
      ((matrix[3] * point.y + matrix[0] * point.x) + point.z * matrix[6]) + matrix[9],
      ((matrix[4] * point.y + matrix[1] * point.x) + point.z * matrix[7]) + matrix[10],
      ((matrix[5] * point.y + matrix[2] * point.x) + point.z * matrix[8]) + matrix[11]
    };
  }

  // INLINED
  EffectVector3 Upward(float distance, const float* matrix) {
    const EffectVector3 point{0.0f, -distance, 0.0f};
    return {
      ((matrix[3] * point.y + matrix[0] * point.x) + point.z * matrix[6]) + matrix[9],
      ((matrix[4] * point.y + matrix[1] * point.x) + point.z * matrix[7]) + matrix[10],
      ((matrix[5] * point.y + matrix[2] * point.x) + point.z * matrix[8]) + matrix[11]
    };
  }
}

// OFFSET: 0x00BB8C50
// FUNCTION: fn_19785
bool MoveEffectPrimitive(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  particle.origin = {
    context.parentRender.position[0] + particle.velocity.x, context.parentRender.position[1] + particle.velocity.y,
    context.parentRender.position[2] + particle.velocity.z
  };

  if (particle.unknown_0200 == particle.unknown_01E0) {
    particle.unknown_0138 = 1;
    constexpr float sentinel = 0.0001f;

    if (particle.unknown_02C8 != sentinel) {
      particle.unknown_0358 = particle.unknown_02C8;
    }

    if (particle.unknown_02CC != sentinel) {
      particle.unknown_0364 = particle.unknown_02CC;
    }

    if (particle.unknown_029C != sentinel) {
      particle.unknown_02BC = particle.unknown_029C;
    }

    if (particle.unknown_02C4 != sentinel) {
      particle.unknown_035C = particle.unknown_02C4;
    }

    if (particle.unknown_02D0 != sentinel) {
      particle.unknown_0368 = particle.unknown_02D0;
    }

    if (particle.unknown_02A0 != sentinel) {
      particle.unknown_02C0 = particle.unknown_02A0;
    }

    if (particle.unknown_02A4.x != sentinel) {
      particle.unknown_0370.x = particle.unknown_02A4.x;
    }

    if (particle.unknown_02A4.y != sentinel) {
      particle.unknown_0370.y = particle.unknown_02A4.y;
    }

    if (particle.unknown_02A4.z != sentinel) {
      particle.unknown_0370.z = particle.unknown_02A4.z;
    }

    particle.unknown_02C8 = sentinel;
    particle.unknown_02CC = sentinel;
    particle.unknown_029C = sentinel;
    particle.unknown_02C4 = sentinel;
    particle.unknown_02D0 = sentinel;
    particle.unknown_02A0 = sentinel;
    particle.unknown_02A4 = {sentinel, sentinel, sentinel};
    particle.unknown_02B0 = {sentinel, sentinel, sentinel};
  }

  particle.unknown_0370.x = particle.unknown_0370.x + particle.unknown_03AC.x;
  particle.unknown_0370.y = particle.unknown_03AC.y + particle.unknown_0370.y;
  particle.unknown_0370.z = particle.unknown_03AC.z + particle.unknown_0370.z;
  particle.unknown_0358 = particle.unknown_035C + particle.unknown_0358;
  particle.unknown_02BC = particle.unknown_02C0 + particle.unknown_02BC;
  particle.unknown_0364 = particle.unknown_0368 + particle.unknown_0364;
  particle.unknown_0330 = particle.unknown_033C + particle.unknown_0330;
  particle.unknown_0334 = particle.unknown_0340 + particle.unknown_0334;

  if ((particle.flags & 0x400u) != 0) {
    particle.unknown_0370 = {};
    particle.unknown_0370 = Forward(particle.unknown_02BC, particle.matrix1);
    particle.offset.x = particle.unknown_0370.x + particle.offset.x;
    particle.offset.z = particle.unknown_0370.z + particle.offset.z;
    particle.offset.y = particle.unknown_0370.y + particle.offset.y;
  } else if ((particle.flags & 0x100u) != 0) {
    particle.offset.x = particle.unknown_0370.x + particle.offset.x;
    particle.offset.y = particle.unknown_0370.y + particle.offset.y;
    particle.offset.z = particle.unknown_0370.z + particle.offset.z;
  } else {
    particle.unknown_0370 = Forward(particle.unknown_02BC, particle.matrix1);
    particle.offset.x = particle.unknown_0370.x + particle.offset.x;
    particle.offset.z = particle.unknown_0370.z + particle.offset.z;
    particle.offset.y = particle.unknown_0370.y + particle.offset.y;
  }

  if ((particle.flags & 0x400u) != 0) {
    const float x = particle.parent->offset[0] - render.position[0];
    const float z = particle.parent->offset[2] - render.position[2];
    const float distance = std::sqrt(z * z + x * x);

    if (3.0f >= distance) {
      particle.unknown_014C.x = std::bit_cast<float>(1u);
      return false;
    }

    particle.unknown_030C = render.angle;
  }

  render.position[0] = particle.origin.x + particle.offset.x;
  render.position[1] = particle.offset.y + particle.origin.y;
  render.position[2] = particle.offset.z + particle.origin.z;
  particle.unknown_0354 = particle.unknown_0358 + particle.unknown_0354;
  particle.unknown_0360 = particle.unknown_0364 + particle.unknown_0360;
  particle.unknown_030C = particle.unknown_030C + particle.unknown_0330;
  particle.unknown_0310 = particle.unknown_0310 + particle.unknown_0334;

  if (!(particle.unknown_0360 <= particle.unknown_036C)) {
    particle.unknown_0368 = 0;
    particle.unknown_0364 = 0;
  }

  AffineMatrix matrix;

  if ((particle.flags & 0x200u) != 0) {
    matrix.SetZRotation(context.cameraPitch);
  } else if ((particle.flags & 0x40u) != 0) {
    particle.unknown_030C = context.cameraYaw + 90.0f;
    particle.unknown_0314 = context.cameraPitch;
    matrix.SetXRotation(ClientRadians(TruncateToInt(particle.unknown_0310)));
    matrix.RotateZ(ClientRadians(TruncateToInt(particle.unknown_0314)));
  } else {
    matrix.SetZRotation(ClientRadians(TruncateToInt(particle.unknown_0314)));
    matrix.RotateX(ClientRadians(TruncateToInt(particle.unknown_0310)));
  }

  const float angle = (particle.flags & 0x200u) != 0 ? render.angle : particle.unknown_030C;
  matrix.RotateY(ClientRadians(TruncateToInt(angle)));
  std::memcpy(particle.matrix2, matrix.values, sizeof(particle.matrix2));

  if ((particle.flags & 2u) != 0) {
    const auto index = particle.unknown_01D4;
    particle.unknown_01D4 = index + 1u == 10u ? 0u : index + 1u;
    const float offset = static_cast<float>(static_cast<int>(index)) / 10.0f;
    particle.unknown_02E4 = offset;
    particle.unknown_02E8 = offset + 1.0f;
  }

  if ((particle.flags & 0x10u) != 0) {
    particle.parent->offset[0] = particle.velocity.x + particle.offset.x;
    particle.parent->offset[1] = particle.offset.y + particle.velocity.y;
    particle.parent->offset[2] = particle.offset.z + particle.velocity.z;
  }

  if ((particle.flags & 4u) != 0 && render.position[1] > context.ground.Height(render.position[0], render.position[2])) {
    particle.flags &= ~4u;
    particle.unknown_01E0 = particle.lifetime - 25;
    particle.unknown_02BC = 0;
    particle.parent->unknown_0170 = 1;
  }

  const int frame = particle.unknown_01E0;

  if ((particle.flags & 0x20u) != 0 && frame % static_cast<int>(particle.unknown_01CC) == 0) {
    particle.unknown_0364 *= -1.0f;
  }

  const bool wrap254 = (particle.flags & 0x800u) != 0;
  const bool gated = wrap254 && (particle.red == 254 || particle.green == 254 || particle.blue == 254);
  bool advance = true;

  if (gated) {
    if (particle.unknown_0234 != 0) {
      ++particle.unknown_01FC;
    }

    advance = particle.unknown_0234 == particle.unknown_01FC;
  }

  if (advance) {
    particle.red += particle.unknown_0248;
    particle.green += particle.unknown_024C;
    particle.blue += particle.unknown_0250;

    if (gated) {
      particle.unknown_01FC = 0;
    }
  }

  if (wrap254) {
    particle.red = particle.red > 254 ? 0 : particle.red < 0 ? 254 : particle.red;
    particle.green = particle.green > 254 ? 0 : particle.green < 0 ? 254 : particle.green;
    particle.blue = particle.blue > 254 ? 0 : particle.blue < 0 ? 254 : particle.blue;
  } else {
    particle.red = particle.red > 255 ? 0 : particle.red < 0 ? 255 : particle.red;
    particle.green = particle.green > 255 ? 255 : particle.green < 0 ? 0 : particle.green;
    particle.blue = particle.blue > 255 ? 255 : particle.blue < 0 ? 0 : particle.blue;
  }

  particle.UpdateColor();

  if (particle.unknown_01E0 % particle.unknown_01E8 == 0) {
    const auto next = render.unknown_003C + 1u;
    render.unknown_003C = next;

    if (particle.unknown_0254 != 0) {
      render.unknown_003C = static_cast<int>(next) % static_cast<int>(particle.textureCount);
    } else if (next == particle.textureCount) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00BAE7D0
// FUNCTION: fn_19769
bool UpdateEffectPrimitive15(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context, int updateTick) {
  if (!particle.parent) {
    return false;
  }

  if (updateTick == 0) {
    return true;
  }

  MoveEffectPrimitive(particle, render, context);
  ++particle.unknown_01E0;
  return particle.unknown_01E0 <= particle.lifetime;
}

// OFFSET: 0x00BB6180
// FUNCTION: fn_19773
bool ShrinkEffectPrimitiveSlots(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  render.position[0] = context.parentRender.position[0];
  render.position[1] = context.parentRender.position[1];
  render.position[2] = context.parentRender.position[2];

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = particle.slots[index];

    if (slot.active != 1) {
      continue;
    }

    float height = slot.unknown_0080 - 0.05f;
    ++slot.frame;
    slot.unknown_0080 = height;

    if (height <= 0.0f) {
      slot.unknown_0080 = 10.0f;
      slot.unknown_0002 = 0;
      height = 10.0f;
    }

    slot.unknown_0008 = height;
    slot.unknown_007E = static_cast<std::int16_t>(static_cast<int>(90.0f - height * 9.0f));

    if (slot.frame >= particle.lifetime - 40) {
      slot.unknown_0002 -= 3;

      if (slot.unknown_0002 < 0) {
        slot.unknown_0002 = 0;
      }
    } else if (slot.unknown_0002 < 70) {
      ++slot.unknown_0002;
    }

    for (int value = 0; value < 21; ++value) {
      if (slot.finished[value] == 0) {
        slot.values[value] = slot.unknown_0008;

        if (slot.values[value] < 0.0f) {
          slot.values[value] = 0;
        }
      }
    }
  }

  return true;
}

// OFFSET: 0x00BB62A0
// FUNCTION: fn_19774
bool PulseEffectPrimitiveSlots(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  render.position[0] = context.parentRender.position[0];
  render.position[1] = context.parentRender.position[1];
  render.position[2] = context.parentRender.position[2];

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = particle.slots[index];

    if (slot.active != 1) {
      continue;
    }

    float phase = slot.unknown_0080 + 0.1f;
    const int frame = ++slot.frame;
    slot.unknown_0080 = phase;

    if (phase >= 13.0f) {
      slot.unknown_0080 = 3.0f;
      slot.unknown_0002 = 0;
      phase = 3.0f;

      if (frame > 50) {
        slot.active = 0;
      }
    }

    phase -= 3.0f;
    float height;

    if (phase > 5.0f) {
      height = 10.0f - phase;

      if (slot.unknown_0002 > 0) {
        slot.unknown_0002 -= 2;
      }
    } else {
      slot.unknown_0002 += 2;
      height = phase;
    }

    slot.unknown_0008 = height * 2.0f;
    slot.unknown_007E = static_cast<std::int16_t>(static_cast<int>(90.0f - phase * 5.0f));

    for (int value = 0; value < 21; ++value) {
      if (slot.finished[value] == 0) {
        slot.values[value] = slot.unknown_0008;

        if (slot.values[value] < 0.0f) {
          slot.values[value] = 0;
        }
      }
    }
  }

  return true;
}

// OFFSET: 0x00BB6820
// FUNCTION: fn_19777
bool SpinEffectPrimitive(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  particle.origin = {
    context.parentRender.position[0] + particle.velocity.x, context.parentRender.position[1] + particle.velocity.y,
    context.parentRender.position[2] + particle.velocity.z
  };

  particle.unknown_02BC = particle.unknown_02C0 + particle.unknown_02BC;
  particle.unknown_0334 = particle.unknown_0340 + particle.unknown_0334;
  particle.unknown_0364 = particle.unknown_0368 + particle.unknown_0364;
  particle.unknown_0310 = particle.unknown_0334 + particle.unknown_0310;
  particle.unknown_0300 = particle.unknown_0308 + particle.unknown_0300;
  particle.unknown_02FC = particle.unknown_0304 + particle.unknown_02FC;
  particle.unknown_0330 = particle.unknown_033C + particle.unknown_0330;
  particle.unknown_030C = particle.unknown_0330 + particle.unknown_030C;

  AffineMatrix& matrix = *reinterpret_cast<AffineMatrix*>(particle.matrix1);
  matrix.SetXRotation(ClientRadians(static_cast<int>(particle.unknown_0310)));
  matrix.RotateY(ClientRadians(static_cast<int>(particle.unknown_030C)));

  particle.unknown_0370 = Upward(particle.unknown_02BC, particle.matrix1);
  particle.offset.y = particle.unknown_0370.y + particle.offset.y;
  particle.offset.x = particle.unknown_0370.x + particle.offset.x;
  particle.offset.z = particle.unknown_0370.z + particle.offset.z;
  particle.unknown_030C = particle.unknown_0330 + particle.unknown_030C;
  particle.unknown_02F4 += particle.unknown_02FC;
  render.position[0] = particle.offset.x + particle.origin.x;
  render.position[1] = particle.offset.y + particle.origin.y;
  render.position[2] = particle.offset.z + particle.origin.z;
  particle.unknown_02F8 += particle.unknown_0300;
  particle.unknown_0360 = particle.unknown_0364 + particle.unknown_0360;

  if (particle.unknown_0360 > particle.unknown_036C) {
    particle.unknown_0360 = particle.unknown_036C;
  }

  if ((particle.flags & 1u) != 0 && particle.unknown_01E0 == particle.unknown_01F0) {
    particle.unknown_0300 = -(particle.unknown_02F8 / static_cast<float>(particle.lifetime - particle.unknown_01E0 + 1));
  }

  particle.UpdateColor();

  if (particle.unknown_01E0 % particle.unknown_01E8 == 0) {
    const auto next = render.unknown_003C + 1u;
    render.unknown_003C = next;

    if (particle.unknown_0254 != 0) {
      render.unknown_003C = static_cast<int>(next) % static_cast<int>(particle.textureCount);
    } else if (next == particle.textureCount) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00BB6B30
// FUNCTION: fn_19778
bool RollEffectPrimitive(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  particle.origin = {
    context.parentRender.position[0] + particle.velocity.x, context.parentRender.position[1] + particle.velocity.y,
    context.parentRender.position[2] + particle.velocity.z
  };

  particle.unknown_02BC = particle.unknown_02C0 + particle.unknown_02BC;
  particle.unknown_0300 = particle.unknown_0308 + particle.unknown_0300;
  particle.unknown_02FC = particle.unknown_0304 + particle.unknown_02FC;
  particle.unknown_0364 = particle.unknown_0368 + particle.unknown_0364;

  AffineMatrix& matrix = *reinterpret_cast<AffineMatrix*>(particle.matrix1);
  matrix.SetXRotation(ClientRadians(static_cast<int>(particle.unknown_0310)));
  matrix.RotateY(ClientRadians(static_cast<int>(particle.unknown_030C)));

  particle.unknown_0370 = Upward(particle.unknown_02BC, particle.matrix1);
  particle.offset.x = particle.unknown_0370.x + particle.offset.x;
  particle.offset.y = particle.unknown_0370.y + particle.offset.y;
  particle.offset.z = particle.unknown_0370.z + particle.offset.z;
  render.position[0] = particle.offset.x + particle.origin.x;
  render.position[1] = particle.offset.y + particle.origin.y;
  render.position[2] = particle.offset.z + particle.origin.z;
  particle.unknown_030C = particle.unknown_0330 + particle.unknown_030C;
  particle.unknown_02F8 = particle.unknown_0300 + particle.unknown_02F8;
  particle.unknown_02F4 = particle.unknown_02FC + particle.unknown_02F4;
  particle.unknown_0360 += particle.unknown_0364;

  if (particle.unknown_0360 > particle.unknown_036C) {
    particle.unknown_0360 = particle.unknown_036C;
  }

  if ((particle.flags & 1u) != 0 && particle.unknown_01E0 == particle.unknown_01F0) {
    particle.unknown_0300 = -(particle.unknown_02F8 / static_cast<float>(particle.lifetime - particle.unknown_01E0 + 1));
  }

  particle.UpdateColor();

  if (particle.unknown_01E0 % particle.unknown_01E8 == 0) {
    const auto next = render.unknown_003C + 1u;
    render.unknown_003C = next;

    if (particle.unknown_0254 != 0) {
      render.unknown_003C = static_cast<int>(next) % static_cast<int>(particle.textureCount);
    } else if (next == particle.textureCount) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00BB8610
// FUNCTION: fn_19784
void AdvanceEffectPrimitive(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  particle.origin = {
    context.parentRender.position[0] + particle.velocity.x, context.parentRender.position[1] + particle.velocity.y,
    context.parentRender.position[2] + particle.velocity.z
  };

  if (particle.unknown_01E0 == particle.unknown_0200) {
    particle.unknown_02BC = particle.unknown_029C;
    particle.unknown_02C0 = particle.unknown_02A0;
  }

  particle.unknown_02BC = particle.unknown_02C0 + particle.unknown_02BC;
  particle.unknown_0370 = Forward(particle.unknown_02BC, particle.matrix1);
  particle.offset.x = particle.unknown_0370.x + particle.offset.x;
  particle.offset.y = particle.unknown_0370.y + particle.offset.y;
  particle.offset.z = particle.unknown_0370.z + particle.offset.z;
  render.position[0] = particle.offset.x + particle.origin.x;
  render.position[1] = particle.offset.y + particle.origin.y;
  render.position[2] = particle.offset.z + particle.origin.z;

  if ((particle.flags & 1u) != 0 && particle.unknown_01E0 == static_cast<int>(particle.unknown_01D4)) {
    particle.unknown_02C0 = 0;
    particle.unknown_02BC = 0;
  }

  if ((particle.flags & 2u) != 0) {
    if (particle.unknown_01E0 == particle.lifetime - 30) {
      particle.unknown_02BC = particle.unknown_02A4.y;
      particle.unknown_02C0 = particle.unknown_02B0.y;
    }
  } else {
    particle.UpdateColor();
  }
}
