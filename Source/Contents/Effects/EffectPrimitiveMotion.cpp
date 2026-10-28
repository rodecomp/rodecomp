#include "Contents/Effects/EffectPrimitiveMotion.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Contents/Camera/Camera.h"
#include "Resource/World/TerrainResource.h"
#include "Contents/World/WorldScene.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Actors/RenderObject.h"
#include "Resource/Sprites/ActionResource.h"
#include "Contents/Effects/EffectParticle.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/RagEffectState.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <bit>
#include <cmath>
#include <cstdlib>
#include <cstdio>
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

  // INLINED
  EffectVector3 Lift(float distance, const float* matrix) {
    const EffectVector3 point{0.0f, distance, 0.0f};
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

// OFFSET: 0x00BAE3A0
// FUNCTION: fn_19764
bool EffectParticle::AdvanceFrame() {
  CActRes* action = this->render.action;

  if (this->state.unknown_01E0 % this->state.unknown_01E8 != 0) {
    return true;
  }

  const unsigned animation = static_cast<unsigned>(this->render.previousAnimation);
  ++this->render.unknown_003C;

  if (this->state.unknown_0254 != 0) {
    this->render.unknown_003C =
      static_cast<std::uint32_t>(static_cast<int>(this->render.unknown_003C) % static_cast<int>(action->FrameCount(animation)));
    return true;
  }

  if ((this->state.flags & 0x800u) != 0) {
    if (this->render.unknown_003C == action->FrameCount(animation)) {
      --this->render.unknown_003C;
    }

    return true;
  }

  return this->render.unknown_003C != action->FrameCount(animation);
}

// OFFSET: 0x00BB5E80
// FUNCTION: fn_19772
bool EffectParticle::UpdateFireWall() {
  const RagEffectState* parent = this->state.parent;
  this->render.position[0] = std::bit_cast<float>(parent->unknown_013C[3]);
  this->render.position[1] = std::bit_cast<float>(parent->unknown_013C[4]);
  this->render.position[2] = std::bit_cast<float>(parent->unknown_013C[5]);

  for (int index = 0; index < 4; ++index) {
    if (this->state.scale == 3.0f && index < 3) {
      index = 3;
    }

    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.active != 1) {
      continue;
    }

    ++slot.frame;

    if (index < 3) {
      slot.angle = static_cast<std::int16_t>(slot.angle + index + 3);
    }

    if (slot.angle >= 360) {
      slot.angle = static_cast<std::int16_t>(slot.angle - 360);
    }

    const int limit = this->state.lifetime - 40;
    bool grown = false;

    if (slot.frame < limit) {
      if (slot.frame < 20) {
        if (index == 3) {
          this->state.slots[3].unknown_0002 = static_cast<std::int16_t>(this->state.slots[3].unknown_0002 + 8);

          if (this->state.slots[3].unknown_0002 > 100) {
            this->state.slots[3].unknown_0002 = 100;
          }

          grown = true;
        } else {
          slot.unknown_0002 = static_cast<std::int16_t>(slot.unknown_0002 + 10);

          if (slot.unknown_0002 > 180) {
            slot.unknown_0002 = 180;
          }
        }
      }

      if (index == 3) {
        grown = true;
      }
    } else {
      if (index < 3) {
        slot.unknown_0002 = static_cast<std::int16_t>(slot.unknown_0002 - 5);

        if (slot.unknown_0002 < 0) {
          slot.unknown_0002 = 0;
        }
      } else {
        slot.unknown_0002 = static_cast<std::int16_t>(slot.unknown_0002 - 3);

        if (slot.unknown_0002 < 0) {
          slot.unknown_0002 = 0;
        }

        grown = true;
      }
    }

    if (!grown) {
      for (int sample = 0; sample < 21; ++sample) {
        if (slot.finished[sample] == 0) {
          if (slot.frame <= 90) {
            slot.values[sample] = slot.unknown_0008 * ClientSine(sample * 9) * ClientSine(slot.frame);
          }

          const float maximum = slot.unknown_0008 * ClientSine(sample * 9);

          if (slot.values[sample] > maximum) {
            slot.values[sample] = maximum;
            slot.finished[sample] = 1;
          }

          if (slot.values[sample] < 0.0f) {
            slot.values[sample] = 0.0f;
          }
        }
      }
    } else {
      for (int sample = 0; sample < 21; ++sample) {
        if (slot.finished[sample] != 0) {
          const float base = slot.unknown_0008 * ClientSine(sample * 9) * 0.55f;
          slot.values[sample] = base;
          slot.values[sample] = slot.unknown_0008 * ClientSine(sample * 9) * 0.45f * ClientSine(slot.frame % 360) + base;
        } else {
          float value;

          if (slot.frame > 90) {
            slot.finished[sample] = 1;
            value = slot.values[sample];
          } else {
            value = slot.unknown_0008 * ClientSine(sample * 9) * ClientSine(slot.frame);
            slot.values[sample] = value;
          }

          if (value > slot.unknown_0008 * ClientSine(sample * 9)) {
            slot.finished[sample] = 1;
          }

          if (slot.values[sample] < 0.0f) {
            slot.values[sample] = 0.0f;
          }
        }
      }
    }
  }

  return true;
}

// OFFSET: 0x00BB63C0
// FUNCTION: fn_19775
bool EffectParticle::UpdateStoneCurse() {
  const RagEffectState* parent = this->state.parent;
  this->render.position[0] = std::bit_cast<float>(parent->unknown_013C[3]);
  this->render.position[1] = std::bit_cast<float>(parent->unknown_013C[4]);
  this->render.position[2] = std::bit_cast<float>(parent->unknown_013C[5]);

  for (int index = 0; index < 4; ++index) {
    if (this->state.scale == 3.0f && index < 3) {
      index = 3;
    }

    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.active != 1) {
      continue;
    }

    ++slot.frame;

    if (index < 3) {
      slot.angle = static_cast<std::int16_t>(slot.angle + index + 3);
    }

    if (slot.angle >= 360) {
      slot.angle = static_cast<std::int16_t>(slot.angle - 360);
    }

    if (slot.frame < this->state.lifetime - 40) {
      if (slot.frame < 20 && slot.unknown_0004 != 1) {
        if (index == 3) {
          this->state.slots[3].unknown_0002 = static_cast<std::int16_t>(this->state.slots[3].unknown_0002 + 8);

          if (this->state.slots[3].unknown_0002 > 60) {
            this->state.slots[3].unknown_0002 = 60;
          }
        } else {
          slot.unknown_0002 = static_cast<std::int16_t>(slot.unknown_0002 + 10);

          if (slot.unknown_0002 > 180) {
            slot.unknown_0002 = 180;
          }
        }
      }
    } else {
      slot.unknown_0002 = static_cast<std::int16_t>(slot.unknown_0002 - (index >= 3 ? 2 : 5));

      if (slot.unknown_0002 < 0) {
        slot.unknown_0002 = 0;
      }
    }

    for (int sample = 0; sample < 21; ++sample) {
      if (slot.finished[sample] == 0) {
        if (slot.frame <= 90) {
          slot.values[sample] = slot.unknown_0008 * ClientSine(sample * 9) * ClientSine(slot.frame);
        }

        const float maximum = slot.unknown_0008 * ClientSine(sample * 9);

        if (slot.values[sample] > maximum) {
          slot.values[sample] = maximum;
          slot.finished[sample] = 1;
        }

        if (slot.values[sample] < 0.0f) {
          slot.values[sample] = 0.0f;
        }
      }
    }
  }

  return true;
}

// OFFSET: 0x00BB65C0
// FUNCTION: fn_19776
void EffectParticle::UpdateBeginSpell() {
  const RagEffectState* parent = this->state.parent;
  this->state.origin.x = std::bit_cast<float>(parent->unknown_013C[3]) + this->state.velocity.x;
  this->state.origin.y = std::bit_cast<float>(parent->unknown_013C[4]) + this->state.velocity.y;
  this->state.origin.z = std::bit_cast<float>(parent->unknown_013C[5]) + this->state.velocity.z;

  float base;

  if (this->state.unknown_0200 == this->state.unknown_01E0) {
    this->state.unknown_0350 = this->state.unknown_02A0;
    base = this->state.unknown_029C;
  } else {
    base = this->state.unknown_034C;
  }

  const float horizontal = base + this->state.unknown_0350;
  const float forwardX = this->state.unknown_033C + this->state.unknown_0330;
  this->state.unknown_02BC = this->state.unknown_02C0 + this->state.unknown_02BC;
  const float forwardZ = this->state.unknown_0304 + this->state.unknown_02FC;
  this->state.unknown_0330 = forwardX;
  const float angle = forwardX + this->state.unknown_030C;
  this->state.unknown_034C = horizontal;
  const float side = horizontal + this->state.unknown_0348;
  this->state.unknown_02FC = forwardZ;
  const float vertical = forwardZ + this->state.unknown_02F4;
  this->state.unknown_030C = angle;
  this->state.unknown_0348 = side;
  this->state.unknown_02F4 = vertical;

  reinterpret_cast<AffineMatrix*>(this->state.matrix1)->SetXRotation(ClientRadians(static_cast<int>(this->state.unknown_0310)));
  reinterpret_cast<AffineMatrix*>(this->state.matrix1)->RotateY(ClientRadians(static_cast<int>(this->state.unknown_030C)));

  const float distance = this->state.unknown_02BC;
  this->state.unknown_0370 = Forward(distance, this->state.matrix1);
  const float offsetX = this->state.unknown_0370.x + this->state.offset.x;
  const float offsetY = this->state.unknown_0370.y + this->state.offset.y;
  const float offsetZ = this->state.unknown_0370.z + this->state.offset.z;
  this->state.offset.x = offsetX;
  this->state.offset.y = offsetY;
  this->state.offset.z = offsetZ;
  this->render.position[0] = offsetX + this->state.origin.x;
  this->render.position[1] = offsetY + this->state.origin.y;
  this->render.position[2] = offsetZ + this->state.origin.z;

  float scale = this->state.unknown_02F4;

  if ((this->state.flags & 1u) != 0 || scale > this->state.unknown_0348) {
    scale = this->state.unknown_0348;
  }

  this->state.scale = scale;
  this->state.UpdateColor();
  this->UpdateRecords();
}

// OFFSET: 0x00BB6F30
// FUNCTION: fn_19779
bool EffectParticle::UpdateHit6() {
  const RagEffectState* parent = this->state.parent;
  this->state.origin.x = std::bit_cast<float>(parent->unknown_013C[3]) + this->state.velocity.x;
  this->state.origin.y = std::bit_cast<float>(parent->unknown_013C[4]) + this->state.velocity.y;
  this->state.origin.z = std::bit_cast<float>(parent->unknown_013C[5]) + this->state.velocity.z;

  const int frame = this->state.unknown_01E0;

  if (frame == this->state.unknown_0200) {
    this->state.unknown_0370 = this->state.unknown_02A4;
    this->state.unknown_03AC = this->state.unknown_02B0;
  }

  float vertical = this->state.unknown_02C0 + this->state.unknown_02BC;
  float lateral = this->state.unknown_0320 + this->state.unknown_031C;
  std::uint32_t flags = this->state.flags;
  this->state.unknown_02BC = vertical;
  this->state.unknown_031C = lateral;

  if ((flags & 0x400u) != 0) {
    const float z = this->state.unknown_03AC.z + this->state.unknown_0370.z;
    const float x = this->state.unknown_03AC.x + this->state.unknown_0370.x;
    const float y = this->state.unknown_03AC.y + this->state.unknown_0370.y;
    this->state.unknown_0370.z = z;
    this->state.offset.z = z + this->state.offset.z;
    this->state.unknown_0370.x = x;
    this->state.offset.x = x + this->state.offset.x;
    this->state.unknown_0370.y = y;
    this->state.offset.y = y + this->state.offset.y;
    this->state.unknown_0314 = this->state.unknown_0338 + this->state.unknown_0314;
    this->state.scale = lateral + this->state.scale;
  } else if ((flags & 1u) != 0 || frame > this->state.unknown_01EC) {
    const float side = lateral + this->state.scale;
    this->state.unknown_0370 = Forward(vertical, this->state.matrix1);
    this->state.offset.x = this->state.unknown_0370.x + this->state.offset.x;
    this->state.offset.y = this->state.unknown_0370.y + this->state.offset.y;
    this->state.offset.z = this->state.unknown_0370.z + this->state.offset.z;
    this->state.scale = side;
    this->state.unknown_0314 = this->state.unknown_0338 + this->state.unknown_0314;
  }

  flags = this->state.flags;

  if ((flags & 2u) != 0) {
    if (this->state.unknown_01E0 % this->state.unknown_01D4 == 0) {
      this->state.unknown_01D4 = rand() % 8 + 12;
      this->state.unknown_03A0.x = static_cast<float>(rand() % 5 + 5) / 100.0f;
      this->state.unknown_03A0.z = static_cast<float>(rand() % 5 + 5) / 100.0f;

      if (rand() % 2 == 0) {
        this->state.unknown_03A0.x = -this->state.unknown_03A0.x;
      }

      if (rand() % 2 == 0) {
        this->state.unknown_03A0.z = -this->state.unknown_03A0.z;
      }
    }

    flags = this->state.flags;
    this->state.offset.x = this->state.unknown_03A0.x + this->state.offset.x;
  }

  if ((flags & 4u) != 0 && this->state.unknown_01E0 % this->state.unknown_01D4 == 0) {
    this->state.unknown_02BC = static_cast<float>(rand() % 35 + 15) / 100.0f;
    this->state.unknown_030C = static_cast<float>(rand() % 80) + (this->state.unknown_030C - 40.0f);
    this->state.unknown_0310 = static_cast<float>(rand() % 80) + (this->state.unknown_0310 - 40.0f);
    reinterpret_cast<AffineMatrix*>(this->state.matrix1)->SetYRotation(ClientRadians(static_cast<int>(this->state.unknown_030C)));
    reinterpret_cast<AffineMatrix*>(this->state.matrix1)->RotateX(ClientRadians(static_cast<int>(this->state.unknown_0310)));
    flags = this->state.flags;
    this->state.unknown_01D4 = rand() % 10 + 5;
  }

  if ((flags & 8u) != 0 && this->state.unknown_01E0 % this->state.unknown_01D4 == 0) {
    this->state.unknown_030C = static_cast<float>(rand() % 10) + (this->state.unknown_030C - 5.0f);
    this->state.unknown_0310 = static_cast<float>(rand() % 10) + (this->state.unknown_0310 - 5.0f);
    reinterpret_cast<AffineMatrix*>(this->state.matrix1)->SetXRotation(ClientRadians(static_cast<int>(this->state.unknown_0310)));
    reinterpret_cast<AffineMatrix*>(this->state.matrix1)->RotateY(ClientRadians(static_cast<int>(this->state.unknown_030C)));
    this->state.unknown_01D4 = rand() % 15 + 5;
  }

  this->render.position[0] = this->state.offset.x + this->state.origin.x;
  this->render.position[1] = this->state.offset.y + this->state.origin.y;
  this->render.position[2] = this->state.offset.z + this->state.origin.z;

  if ((this->state.flags & 0x100u) != 0) {
    if (3.0f >= this->HorizontalDistance(this->state.parent->offset[0], this->state.parent->offset[2])) {
      return false;
    }
  }

  if ((this->state.flags & 0x20u) != 0) {
    this->state.unknown_0314 = this->render.angle - static_cast<CGameMode*>(g_world.ActiveGameMode())->camera->camera.yaw;
  }

  this->state.UpdateColor();
  this->UpdateRecords();
  return this->AdvanceFrame();
}

// OFFSET: 0x00BB78A0
// FUNCTION: fn_19781
bool EffectParticle::UpdateDetoxication() {
  float positionX = this->state.origin.x + this->state.velocity.x;
  const float positionY = this->state.origin.y + this->state.velocity.y;
  const float positionZ = this->state.origin.z + this->state.velocity.z;
  const int frame = this->state.unknown_01E0;

  if (frame == this->state.unknown_0200) {
    this->state.unknown_0370 = this->state.unknown_02A4;
    this->state.unknown_03AC = this->state.unknown_02B0;
  }

  float vertical = this->state.unknown_02C0 + this->state.unknown_02BC;
  float lateral = this->state.unknown_0320 + this->state.unknown_031C;
  std::uint32_t flags = this->state.flags;
  this->state.unknown_02BC = vertical;
  this->state.unknown_031C = lateral;

  if ((flags & 0x400u) != 0) {
    const float z = this->state.unknown_03AC.z + this->state.unknown_0370.z;
    const float x = this->state.unknown_03AC.x + this->state.unknown_0370.x;
    const float y = this->state.unknown_03AC.y + this->state.unknown_0370.y;
    this->state.unknown_0370.z = z;
    this->state.offset.z = z + this->state.offset.z;
    this->state.unknown_0370.x = x;
    this->state.offset.x = x + this->state.offset.x;
    this->state.unknown_0370.y = y;
    this->state.offset.y = y + this->state.offset.y;
    this->state.unknown_0314 = this->state.unknown_0338 + this->state.unknown_0314;
    this->state.scale = lateral + this->state.scale;
  } else if ((flags & 1u) != 0 || frame > this->state.unknown_01EC) {
    const float side = lateral + this->state.scale;
    this->state.unknown_0370 = Forward(vertical, this->state.matrix1);
    this->state.offset.x = this->state.unknown_0370.x + this->state.offset.x;
    this->state.offset.y = this->state.unknown_0370.y + this->state.offset.y;
    this->state.offset.z = this->state.unknown_0370.z + this->state.offset.z;
    this->state.scale = side;
    this->state.unknown_0314 = this->state.unknown_0338 + this->state.unknown_0314;
  }

  flags = this->state.flags;

  if ((flags & 2u) != 0) {
    if (this->state.unknown_01E0 % this->state.unknown_01D4 == 0) {
      this->state.unknown_01D4 = rand() % 8 + 12;
      this->state.unknown_03A0.x = static_cast<float>(rand() % 5 + 5) / 100.0f;
      this->state.unknown_03A0.z = static_cast<float>(rand() % 5 + 5) / 100.0f;

      if (rand() % 2 == 0) {
        this->state.unknown_03A0.x = -this->state.unknown_03A0.x;
      }

      if (rand() % 2 == 0) {
        this->state.unknown_03A0.z = -this->state.unknown_03A0.z;
      }
    }

    flags = this->state.flags;
    this->state.offset.x = this->state.unknown_03A0.x + this->state.offset.x;
  }

  if ((flags & 4u) != 0 && this->state.unknown_01E0 % this->state.unknown_01D4 == 0) {
    this->state.unknown_02BC = static_cast<float>(rand() % 35 + 15) / 100.0f;
    this->state.unknown_030C = static_cast<float>(rand() % 80) + (this->state.unknown_030C - 40.0f);
    this->state.unknown_0310 = static_cast<float>(rand() % 80) + (this->state.unknown_0310 - 40.0f);
    reinterpret_cast<AffineMatrix*>(this->state.matrix1)->SetYRotation(ClientRadians(static_cast<int>(this->state.unknown_030C)));
    reinterpret_cast<AffineMatrix*>(this->state.matrix1)->RotateX(ClientRadians(static_cast<int>(this->state.unknown_0310)));
    this->state.unknown_01D4 = rand() % 10 + 5;
    flags = this->state.flags;
  }

  if ((flags & 8u) != 0 && this->state.unknown_01E0 % this->state.unknown_01D4 == 0) {
    this->state.unknown_030C = static_cast<float>(rand() % 10) + (this->state.unknown_030C - 5.0f);
    this->state.unknown_0310 = static_cast<float>(rand() % 10) + (this->state.unknown_0310 - 5.0f);
    reinterpret_cast<AffineMatrix*>(this->state.matrix1)->SetXRotation(ClientRadians(static_cast<int>(this->state.unknown_0310)));
    reinterpret_cast<AffineMatrix*>(this->state.matrix1)->RotateY(ClientRadians(static_cast<int>(this->state.unknown_030C)));
    this->state.unknown_01D4 = rand() % 15 + 5;
    flags = this->state.flags;
  }

  this->render.position[0] = positionX + this->state.offset.x;
  this->render.position[1] = positionY + this->state.offset.y;
  this->render.position[2] = positionZ + this->state.offset.z;

  if ((flags & 0x20u) != 0) {
    this->state.unknown_0314 = this->render.angle - static_cast<CGameMode*>(g_world.ActiveGameMode())->camera->camera.yaw;
  }

  this->state.UpdateColor();
  this->UpdateRecords();
  return this->AdvanceFrame();
}

// OFFSET: 0x00BB7EC0
// FUNCTION: fn_19782
bool EffectParticle::UpdateExit() {
  const RagEffectState* parent = this->state.parent;
  this->state.origin = {std::bit_cast<float>(parent->unknown_013C[3]), std::bit_cast<float>(parent->unknown_013C[4]), std::bit_cast<float>(parent->unknown_013C[5])};

  const float lateral = this->state.unknown_033C + this->state.unknown_0330;
  float vertical = this->state.unknown_02EC + this->state.unknown_02F0;
  const float spread = this->state.unknown_034C + this->state.unknown_0350;
  this->state.unknown_0330 = lateral;
  const float angle = lateral + this->state.unknown_030C;
  this->state.unknown_02EC = vertical;
  vertical = vertical + this->state.velocity.y;
  const float side = spread + this->state.unknown_0348;
  this->state.unknown_030C = angle;
  this->state.velocity.y = vertical;
  this->state.unknown_0348 = side;

  const float moveX = this->state.origin.x + this->state.velocity.x;
  this->state.origin.y = vertical + this->state.origin.y;
  this->state.origin.x = moveX;
  this->state.origin.z = this->state.velocity.z + this->state.origin.z;
  const int frame = this->state.unknown_01E0;
  this->state.unknown_0314 = this->state.unknown_0338 + this->state.unknown_0314;

  if (frame == this->state.unknown_0200) {
    this->state.unknown_02EC = this->state.unknown_029C;
    this->state.unknown_02F0 = this->state.unknown_02A0;
  }

  if (this->state.unknown_0158 != 0) {
    int index = this->state.unknown_01D8;
    const std::uint32_t count = static_cast<std::uint32_t>(this->state.unknown_0258[0].size());

    if (index < 0 || static_cast<std::uint32_t>(index) >= count) {
      index = 0;
      this->state.unknown_01D8 = 0;
    }

    if (static_cast<float>(frame) == this->state.unknown_0258[0][index]) {
      this->state.unknown_034C = this->state.unknown_0258[1][index];
      this->state.unknown_0350 = this->state.unknown_0258[2][index];
      this->state.unknown_01D8 = index + 1;
    }
  }

  reinterpret_cast<AffineMatrix*>(this->state.matrix1)->SetYRotation(ClientRadians(static_cast<int>(this->state.unknown_030C)));

  const float distance = this->state.unknown_0348;
  this->state.offset = Forward(distance, this->state.matrix1);
  this->render.position[0] = this->state.offset.x + this->state.origin.x;
  this->render.position[1] = this->state.offset.y + this->state.origin.y;
  this->render.position[2] = this->state.offset.z + this->state.origin.z;

  this->state.scale = this->state.unknown_031C + this->state.scale;
  const float rotation = this->state.unknown_0334 + this->state.unknown_0310;
  this->state.unknown_0310 = rotation;
  reinterpret_cast<AffineMatrix*>(this->state.matrix1)->RotateZ(rotation);

  this->state.UpdateColor();
  this->UpdateRecords();
  return this->AdvanceFrame();
}

// OFFSET: 0x00BB8180
// FUNCTION: fn_19783
bool EffectParticle::UpdateWarp() {
  const RagEffectState* parent = this->state.parent;
  this->state.origin = {
    std::bit_cast<float>(parent->unknown_013C[3]) + this->state.velocity.x, std::bit_cast<float>(parent->unknown_013C[4]) + this->state.velocity.y,
    std::bit_cast<float>(parent->unknown_013C[5]) + this->state.velocity.z
  };

  const int angle = static_cast<int>(this->state.unknown_0314);
  const float lateral = this->state.unknown_0334;
  this->state.unknown_0334 = lateral + this->state.unknown_0340;
  this->state.unknown_0310 = lateral + this->state.unknown_0310;
  this->state.unknown_02BC = this->state.unknown_02C0 + this->state.unknown_02BC;
  this->state.unknown_02EC = this->state.unknown_02F0 + this->state.unknown_02EC;

  const float distance = this->state.unknown_02BC - this->state.unknown_02EC;
  this->state.unknown_0370 = Forward(distance, this->state.matrix1);
  reinterpret_cast<AffineMatrix*>(this->state.matrix2)->SetAxisRotation(this->state.unknown_0290, ClientRadians(angle));

  const float move = this->state.unknown_0310;
  this->state.unknown_03A0 = Lift(move, this->state.matrix2);
  this->state.scale = this->state.unknown_031C + this->state.scale;

  this->state.offset.x = this->state.offset.x - this->state.unknown_0370.x;
  this->state.offset.y = this->state.offset.y - this->state.unknown_0370.y;
  this->state.offset.z = this->state.offset.z - this->state.unknown_0370.z;

  this->render.position[0] = this->state.offset.x + this->state.origin.x + this->state.unknown_03A0.x;
  this->render.position[1] = this->state.offset.y + this->state.origin.y + this->state.unknown_03A0.y;
  this->render.position[2] = this->state.offset.z + this->state.origin.z + this->state.unknown_03A0.z;

  this->state.UpdateColor();
  this->UpdateRecords();
  return this->AdvanceFrame();
}

// OFFSET: 0x00BE6FE0
// FUNCTION: fn_19931
void EffectParticle::UpdateRecords() {
  const int count = this->state.unknown_01F4;

  for (int index = count - 1; index > 0; --index) {
    EffectPrimitiveRecord& record = this->state.records[index];
    const EffectPrimitiveRecord& previous = this->state.records[index - 1];
    record.position = previous.position;

    for (int corner = 0; corner < 4; ++corner) {
      record.corners[corner] = previous.corners[corner];
    }

    record.unknown_003C = previous.unknown_003C;
    record.angle = previous.angle;
    std::memcpy(record.matrix, previous.matrix, sizeof(record.matrix));
    record.alpha = this->state.unknown_0324 - this->state.unknown_0324 / static_cast<float>(count) * static_cast<float>(index);

    const std::uint32_t red = static_cast<unsigned char>(this->state.red);
    const std::uint32_t green = static_cast<unsigned char>(this->state.green);
    const std::uint32_t blue = static_cast<unsigned char>(this->state.blue);
    record.color = (static_cast<std::uint32_t>(static_cast<int>(record.alpha)) << 24) | (red << 16) | (green << 8) | blue;
    record.scale = this->state.scale - this->state.scale / static_cast<float>(count) * static_cast<float>(index) * 0.5f;
  }

  EffectPrimitiveRecord& first = this->state.records[0];
  first.position.x = this->render.position[0];
  first.position.y = this->render.position[1];
  first.position.z = this->render.position[2];

  for (int corner = 0; corner < 4; ++corner) {
    first.corners[corner] = this->state.corners[corner];
  }

  first.unknown_003C = this->state.unknown_0348;
  first.scale = this->state.scale;
  first.angle = this->state.unknown_030C;
  std::memcpy(first.matrix, this->state.matrix1, sizeof(first.matrix));
  first.alpha = this->state.unknown_0324;

  const std::uint32_t red = static_cast<unsigned char>(this->state.red);
  const std::uint32_t green = static_cast<unsigned char>(this->state.green);
  const std::uint32_t blue = static_cast<unsigned char>(this->state.blue);
  first.color = (static_cast<std::uint32_t>(static_cast<int>(this->state.unknown_0324)) << 24) | (red << 16) | (green << 8) | blue;
}
