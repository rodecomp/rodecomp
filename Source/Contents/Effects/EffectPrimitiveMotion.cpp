#include "Contents/Effects/EffectPrimitiveMotion.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ActiveGameMode.h"
#include "Client/Session.h"
#include "Contents/Camera/Camera.h"
#include "Contents/World/WorldScene.h"
#include "Resource/Sprites/ActionResource.h"
#include "Contents/Effects/EffectParticle.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/AttachedEffect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Effects/RagEffectState.h"
#include "Contents/Sound/SoundMgr.h"
#include "Resource/World/TerrainResource.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
  // INLINED
  vector3d Forward(float distance, const float* matrix) {
    const vector3d point{0.0f, 0.0f, distance};
    return {
      ((matrix[3] * point.y + matrix[0] * point.x) + point.z * matrix[6]) + matrix[9],
      ((matrix[4] * point.y + matrix[1] * point.x) + point.z * matrix[7]) + matrix[10],
      ((matrix[5] * point.y + matrix[2] * point.x) + point.z * matrix[8]) + matrix[11]
    };
  }

  // INLINED
  vector3d Upward(float distance, const float* matrix) {
    const vector3d point{0.0f, -distance, 0.0f};
    return {
      ((matrix[3] * point.y + matrix[0] * point.x) + point.z * matrix[6]) + matrix[9],
      ((matrix[4] * point.y + matrix[1] * point.x) + point.z * matrix[7]) + matrix[10],
      ((matrix[5] * point.y + matrix[2] * point.x) + point.z * matrix[8]) + matrix[11]
    };
  }

  // INLINED
  vector3d Lift(float distance, const float* matrix) {
    const vector3d point{0.0f, distance, 0.0f};
    return {
      ((matrix[3] * point.y + matrix[0] * point.x) + point.z * matrix[6]) + matrix[9],
      ((matrix[4] * point.y + matrix[1] * point.x) + point.z * matrix[7]) + matrix[10],
      ((matrix[5] * point.y + matrix[2] * point.x) + point.z * matrix[8]) + matrix[11]
    };
  }

  // INLINED
  void FaceCamera(EffectPrimitiveSlot* slots) {
    const CView* view = static_cast<CGameMode*>(g_world.ActiveGameMode())->camera;
    slots[0].alphaT = static_cast<std::int16_t>(360 - static_cast<int>(view->camera.yaw));
    slots[1].alphaT = static_cast<std::int16_t>(360 - static_cast<int>(view->camera.pitch) + 90);

    if (slots[1].alphaT >= 360) {
      slots[1].alphaT -= 360;
    }
  }

  // INLINED
  __forceinline void FollowTrail(EffectPrimitiveSlot& trail, const EffectPrimitiveSlot& previous, int fade) {
    trail.fullDisplayAngle = previous.fullDisplayAngle;
    trail.distance = previous.distance;
    trail.height[8] = previous.height[8];
    trail.alphaB = previous.alphaB - fade;

    if (trail.alphaB < 0) {
      trail.alphaB = 0;
    }

    trail.vecBNow = previous.vecBNow;
    trail.fullDisplayAngle = previous.fullDisplayAngle;
    trail.flag1[0] = previous.flag1[0];
  }
}

// OFFSET: 0x00BB8C50
// FUNCTION: fn_19785
bool MoveEffectPrimitive(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  particle.origin = {
    context.parentRender.position.x + particle.velocity.x, context.parentRender.position.y + particle.velocity.y,
    context.parentRender.position.z + particle.velocity.z
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
    const float x = particle.master->state.offset[0] - render.position.x;
    const float z = particle.master->state.offset[2] - render.position.z;
    const float distance = std::sqrt(z * z + x * x);

    if (3.0f >= distance) {
      particle.unknown_014C.x = std::bit_cast<float>(1u);
      return false;
    }

    particle.unknown_030C = render.angle;
  }

  render.position.x = particle.origin.x + particle.offset.x;
  render.position.y = particle.offset.y + particle.origin.y;
  render.position.z = particle.offset.z + particle.origin.z;
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
    particle.master->state.offset[0] = particle.velocity.x + particle.offset.x;
    particle.master->state.offset[1] = particle.offset.y + particle.velocity.y;
    particle.master->state.offset[2] = particle.offset.z + particle.velocity.z;
  }

  if ((particle.flags & 4u) != 0 && render.position.y > context.ground.Height(render.position.x, render.position.z)) {
    particle.flags &= ~4u;
    particle.unknown_01E0 = particle.lifetime - 25;
    particle.unknown_02BC = 0;
    particle.master->state.unknown_0170 = 1;
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
    const auto next = render.curMotion + 1u;
    render.curMotion = next;

    if (particle.unknown_0254 != 0) {
      render.curMotion = static_cast<int>(next) % static_cast<int>(particle.textureCount);
    } else if (next == particle.textureCount) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00BAE7D0
// FUNCTION: fn_19769
bool UpdateEffectPrimitive15(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context, int updateTick) {
  if (!particle.master) {
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
  const vector3d& parent = context.parentRender.position;
  render.position.x = parent.x;
  render.position.y = parent.y;
  render.position.z = parent.z;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = particle.slots[index];

    if (slot.life != 1) {
      continue;
    }

    float height = slot.distance - 0.05f;
    ++slot.process;
    slot.distance = height;

    if (height <= 0.0f) {
      slot.distance = 10.0f;
      slot.alphaB = 0;
      height = 10.0f;
    }

    slot.maxHeight = height;
    slot.riseAngle = static_cast<std::int16_t>(static_cast<int>(90.0f - height * 9.0f));

    if (slot.process >= particle.lifetime - 40) {
      slot.alphaB -= 3;

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    } else if (slot.alphaB < 70) {
      ++slot.alphaB;
    }

    for (int value = 0; value < 21; ++value) {
      if (slot.flag1[value] == 0) {
        slot.height[value] = slot.maxHeight;

        if (slot.height[value] < 0.0f) {
          slot.height[value] = 0;
        }
      }
    }
  }

  return true;
}

// OFFSET: 0x00BB62A0
// FUNCTION: fn_19774
bool PulseEffectPrimitiveSlots(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  const vector3d& parent = context.parentRender.position;
  render.position.x = parent.x;
  render.position.y = parent.y;
  render.position.z = parent.z;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = particle.slots[index];

    if (slot.life != 1) {
      continue;
    }

    float phase = slot.distance + 0.1f;
    const int frame = ++slot.process;
    slot.distance = phase;

    if (phase >= 13.0f) {
      slot.distance = 3.0f;
      slot.alphaB = 0;
      phase = 3.0f;

      if (frame > 50) {
        slot.life = 0;
      }
    }

    phase -= 3.0f;
    float height;

    if (phase > 5.0f) {
      height = 10.0f - phase;

      if (slot.alphaB > 0) {
        slot.alphaB -= 2;
      }
    } else {
      slot.alphaB += 2;
      height = phase;
    }

    slot.maxHeight = height * 2.0f;
    slot.riseAngle = static_cast<std::int16_t>(static_cast<int>(90.0f - phase * 5.0f));

    for (int value = 0; value < 21; ++value) {
      if (slot.flag1[value] == 0) {
        slot.height[value] = slot.maxHeight;

        if (slot.height[value] < 0.0f) {
          slot.height[value] = 0;
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
    context.parentRender.position.x + particle.velocity.x, context.parentRender.position.y + particle.velocity.y,
    context.parentRender.position.z + particle.velocity.z
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
  render.position.x = particle.offset.x + particle.origin.x;
  render.position.y = particle.offset.y + particle.origin.y;
  render.position.z = particle.offset.z + particle.origin.z;
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
    const auto next = render.curMotion + 1u;
    render.curMotion = next;

    if (particle.unknown_0254 != 0) {
      render.curMotion = static_cast<int>(next) % static_cast<int>(particle.textureCount);
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
    context.parentRender.position.x + particle.velocity.x, context.parentRender.position.y + particle.velocity.y,
    context.parentRender.position.z + particle.velocity.z
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
  render.position.x = particle.offset.x + particle.origin.x;
  render.position.y = particle.offset.y + particle.origin.y;
  render.position.z = particle.offset.z + particle.origin.z;
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
    const auto next = render.curMotion + 1u;
    render.curMotion = next;

    if (particle.unknown_0254 != 0) {
      render.curMotion = static_cast<int>(next) % static_cast<int>(particle.textureCount);
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
    context.parentRender.position.x + particle.velocity.x, context.parentRender.position.y + particle.velocity.y,
    context.parentRender.position.z + particle.velocity.z
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
  render.position.x = particle.offset.x + particle.origin.x;
  render.position.y = particle.offset.y + particle.origin.y;
  render.position.z = particle.offset.z + particle.origin.z;

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
  ++this->render.curMotion;

  if (this->state.unknown_0254 != 0) {
    this->render.curMotion =
      static_cast<std::uint32_t>(static_cast<int>(this->render.curMotion) % static_cast<int>(action->FrameCount(animation)));
    return true;
  }

  if ((this->state.flags & 0x800u) != 0) {
    if (this->render.curMotion == action->FrameCount(animation)) {
      --this->render.curMotion;
    }

    return true;
  }

  return this->render.curMotion != action->FrameCount(animation);
}

// OFFSET: 0x00BB5E80
// FUNCTION: fn_19772
bool EffectParticle::UpdateFireWall() {
  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  for (int index = 0; index < 4; ++index) {
    if (this->state.scale == 3.0f && index < 3) {
      index = 3;
    }

    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (index < 3) {
      slot.rotStart = static_cast<std::int16_t>(slot.rotStart + index + 3);
    }

    if (slot.rotStart >= 360) {
      slot.rotStart = static_cast<std::int16_t>(slot.rotStart - 360);
    }

    const int limit = this->state.lifetime - 40;
    bool grown = false;

    if (slot.process < limit) {
      if (slot.process < 20) {
        if (index == 3) {
          this->state.slots[3].alphaB = static_cast<std::int16_t>(this->state.slots[3].alphaB + 8);

          if (this->state.slots[3].alphaB > 100) {
            this->state.slots[3].alphaB = 100;
          }

          grown = true;
        } else {
          slot.alphaB = static_cast<std::int16_t>(slot.alphaB + 10);

          if (slot.alphaB > 180) {
            slot.alphaB = 180;
          }
        }
      }

      if (index == 3) {
        grown = true;
      }
    } else {
      if (index < 3) {
        slot.alphaB = static_cast<std::int16_t>(slot.alphaB - 5);

        if (slot.alphaB < 0) {
          slot.alphaB = 0;
        }
      } else {
        slot.alphaB = static_cast<std::int16_t>(slot.alphaB - 3);

        if (slot.alphaB < 0) {
          slot.alphaB = 0;
        }

        grown = true;
      }
    }

    if (!grown) {
      for (int sample = 0; sample < 21; ++sample) {
        if (slot.flag1[sample] == 0) {
          if (slot.process <= 90) {
            slot.height[sample] = slot.maxHeight * ClientSine(sample * 9) * ClientSine(slot.process);
          }

          const float maximum = slot.maxHeight * ClientSine(sample * 9);

          if (slot.height[sample] > maximum) {
            slot.height[sample] = maximum;
            slot.flag1[sample] = 1;
          }

          if (slot.height[sample] < 0.0f) {
            slot.height[sample] = 0.0f;
          }
        }
      }
    } else {
      for (int sample = 0; sample < 21; ++sample) {
        if (slot.flag1[sample] != 0) {
          const float base = slot.maxHeight * ClientSine(sample * 9) * 0.55f;
          slot.height[sample] = base;
          slot.height[sample] = slot.maxHeight * ClientSine(sample * 9) * 0.45f * ClientSine(slot.process % 360) + base;
        } else {
          float value;

          if (slot.process > 90) {
            slot.flag1[sample] = 1;
            value = slot.height[sample];
          } else {
            value = slot.maxHeight * ClientSine(sample * 9) * ClientSine(slot.process);
            slot.height[sample] = value;
          }

          if (value > slot.maxHeight * ClientSine(sample * 9)) {
            slot.flag1[sample] = 1;
          }

          if (slot.height[sample] < 0.0f) {
            slot.height[sample] = 0.0f;
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
  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  for (int index = 0; index < 4; ++index) {
    if (this->state.scale == 3.0f && index < 3) {
      index = 3;
    }

    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (index < 3) {
      slot.rotStart = static_cast<std::int16_t>(slot.rotStart + index + 3);
    }

    if (slot.rotStart >= 360) {
      slot.rotStart = static_cast<std::int16_t>(slot.rotStart - 360);
    }

    if (slot.process < this->state.lifetime - 40) {
      if (slot.process < 20 && slot.alphaT != 1) {
        if (index == 3) {
          this->state.slots[3].alphaB = static_cast<std::int16_t>(this->state.slots[3].alphaB + 8);

          if (this->state.slots[3].alphaB > 60) {
            this->state.slots[3].alphaB = 60;
          }
        } else {
          slot.alphaB = static_cast<std::int16_t>(slot.alphaB + 10);

          if (slot.alphaB > 180) {
            slot.alphaB = 180;
          }
        }
      }
    } else {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB - (index >= 3 ? 2 : 5));

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    }

    for (int sample = 0; sample < 21; ++sample) {
      if (slot.flag1[sample] == 0) {
        if (slot.process <= 90) {
          slot.height[sample] = slot.maxHeight * ClientSine(sample * 9) * ClientSine(slot.process);
        }

        const float maximum = slot.maxHeight * ClientSine(sample * 9);

        if (slot.height[sample] > maximum) {
          slot.height[sample] = maximum;
          slot.flag1[sample] = 1;
        }

        if (slot.height[sample] < 0.0f) {
          slot.height[sample] = 0.0f;
        }
      }
    }
  }

  return true;
}

// OFFSET: 0x00BB65C0
// FUNCTION: fn_19776
void EffectParticle::UpdateBeginSpell() {
  const AttachedEffect* master = this->state.master;
  this->state.origin.x = master->render.position.x + this->state.velocity.x;
  this->state.origin.y = master->render.position.y + this->state.velocity.y;
  this->state.origin.z = master->render.position.z + this->state.velocity.z;

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
  this->render.position.x = offsetX + this->state.origin.x;
  this->render.position.y = offsetY + this->state.origin.y;
  this->render.position.z = offsetZ + this->state.origin.z;

  float scale = this->state.unknown_02F4;

  if ((this->state.flags & 1u) != 0 || scale > this->state.unknown_0348) {
    scale = this->state.unknown_0348;
  }

  this->state.scale = scale;
  this->state.UpdateColor();
  this->UpdateRecords();
}

// OFFSET: 0x00BBAB10
// FUNCTION: fn_19790
void EffectParticle::PrimBottom2() {
  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.height[0] == 1.0f) {
      slot.fullDisplayAngle += 4;
    } else {
      slot.fullDisplayAngle += 2;
    }

    if (slot.fullDisplayAngle >= 360) {
      slot.fullDisplayAngle -= 360;
    }

    slot.maxHeight = 20.0f + 10.0f * g_sinTable[slot.fullDisplayAngle];
  }
}

// OFFSET: 0x00BBABC0
// FUNCTION: fn_19791
void EffectParticle::PrimBottom() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  EffectPrimitiveSlot& slot = slots[0];

  if (slot.life != 1) {
    return;
  }

  ++slot.process;

  if (slot.process % 6 == 0) {
    for (int index = 3; index > 0; --index) {
      EffectPrimitiveSlot& trail = slots[index];
      const EffectPrimitiveSlot& previous = slots[index - 1];

      if (trail.life > 0) {
        trail.fullDisplayAngle = previous.fullDisplayAngle;
        trail.distance = previous.distance + 0.5f;
        trail.height[8] = previous.height[8];
        trail.alphaB = previous.alphaB - 50;

        if (trail.alphaB < 0) {
          trail.alphaB = 0;
        }

        trail.vecBNow = previous.vecBNow;
        trail.fullDisplayAngle = previous.fullDisplayAngle;
        trail.flag1[0] = previous.flag1[0];
        trail.flag1[2] = 2;
        trail.height[7] = 0;
      }
    }
  }

  slot.height[8] += 5.0f;

  if (slot.height[8] >= 360.0f) {
    slot.height[8] -= 360.0f;
  }

  ++slot.rotStart;

  if (slot.rotStart >= 360) {
    slot.rotStart -= 360;
  }

  slot.vecBNow.x = this->render.position.x;
  slot.vecBNow.z = this->render.position.z;
  slot.vecBPre.y = this->render.position.y - 6.0f;
  slot.vecBNow.y = slot.vecBPre.y + 3.0f * g_sinTable[slot.rotStart];

  if (slot.flag1[2] == 7) {
    slot.fullDisplayAngle += 10;

    if (slot.fullDisplayAngle >= 360) {
      slot.fullDisplayAngle -= 360;
    }
  }
}

// OFFSET: 0x00BBAF30
// FUNCTION: fn_19792
void EffectParticle::PrimBottomOut() {
  FaceCamera(this->state.slots);

  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process <= 10) {
      slot.alphaB += 25;
    } else {
      slot.alphaB -= 5;

      if (slot.alphaB <= 0) {
        slot.alphaB = 0;
        slot.process = 0;
        slot.vecBNow.y = slot.flag1[1] == 0 ? this->render.position.y : this->render.position.y - 5.0f;
      }

      slot.vecBNow.y -= 0.2f;
    }

    slot.height[8] += 5.0f;

    if (slot.height[8] >= 360.0f) {
      slot.height[8] -= 360.0f;
    }

    if (slot.flag1[1] == 1) {
      slot.fullDisplayAngle += 5;

      if (slot.fullDisplayAngle >= 360) {
        slot.fullDisplayAngle -= 360;
      }
    }
  }
}

// OFFSET: 0x00BBB120
// FUNCTION: fn_19793
void EffectParticle::PrimBubbleDrop() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  EffectPrimitiveSlot& slot = slots[0];

  if (slot.life != 1) {
    return;
  }

  ++slot.process;

  if (slot.process <= 0) {
    return;
  }

  if (slot.process > 3) {
    FollowTrail(slots[3], slots[2], 25);
  }

  if (slot.process > 2) {
    FollowTrail(slots[2], slots[1], 25);
  }

  if (slot.process > 1) {
    FollowTrail(slots[1], slot, 100);
  }

  if (slot.process <= 10) {
    slot.alphaB += 20;
  }

  if (slot.process < slot.flag1[4]) {
    slot.fullDisplayAngle -= 15;

    if (slot.fullDisplayAngle < 0) {
      slot.fullDisplayAngle += 360;
    }

    slot.vecBPre.y += slot.height[1];
    slot.vecBNow = {
      this->render.position.x + slot.vecBPre.x, this->render.position.y + slot.vecBPre.y, this->render.position.z + slot.vecBPre.z
    };
  } else {
    slot.alphaB -= 10;

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }
  }
}

// OFFSET: 0x00BBB9D0
// FUNCTION: fn_19795
void EffectParticle::PrimChemical2() {
  FaceCamera(this->state.slots);

  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process <= 20) {
      slot.alphaB += 4;
    } else if (slot.process > 50) {
      slot.alphaB -= 2;

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    }
  }
}

// OFFSET: 0x00BBBB40
// FUNCTION: fn_19796
void EffectParticle::PrimChemical3() {
  FaceCamera(this->state.slots);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process <= 10) {
      if (slot.flag1[0] < 10) {
        slot.alphaB += 12;
      } else {
        slot.alphaB += 20;
      }
    }

    if (slot.flag1[0] >= 10) {
      slot.height[3] += 0.05f;
    }

    slot.height[1] += slot.height[3];

    if (slot.height[1] >= 200.0f) {
      slot.height[1] -= 200.0f;
    }

    if (slot.process > 20) {
      if (slot.flag1[0] < 10) {
        slot.alphaB -= 4;
      } else {
        slot.alphaB -= 2;
      }

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    }
  }
}

// OFFSET: 0x00BBBC40
// FUNCTION: fn_19797
void EffectParticle::PrimChemical() {
  FaceCamera(this->state.slots);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process <= 10) {
      slot.alphaB += 18;
    } else if (slot.process > 20) {
      slot.alphaB -= 15;

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    }
  }
}

// OFFSET: 0x00BBCB30
// FUNCTION: fn_19800
void EffectParticle::PrimEffectTexture2() {
  FaceCamera(this->state.slots);

  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life != 1) {
    return;
  }

  ++slot.process;

  if (slot.height[8] >= 360.0f) {
    slot.height[8] -= 360.0f;
  }

  if (slot.process <= 30) {
    slot.alphaB += 10;

    if (slot.alphaB > 255) {
      slot.alphaB = 255;
    }
  } else {
    slot.alphaB -= 3;

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }
  }

  slot.vecBNow.y = slot.vecBPre.y;
}

// OFFSET: 0x00BBCF10
// FUNCTION: fn_19802
void EffectParticle::PrimEffectTexture_Ani() {
  FaceCamera(this->state.slots);

  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life != 1) {
    return;
  }

  ++slot.process;

  const int frameTicks = slot.flag1[4] == 0 ? 8 : 6;

  if (slot.process % frameTicks == 0) {
    ++slot.flag1[0];

    if (slot.flag1[0] >= static_cast<int>(this->state.textureCount)) {
      slot.flag1[0] = 0;
    }
  }

  if (slot.alphaB >= 255) {
    slot.alphaB = 255;
  }

  if (slot.alphaB < 0) {
    slot.alphaB = 0;
  }

  slot.vecBNow.y = slot.vecBPre.y;
  slot.vecBNow.x = this->render.position.x;
  slot.vecBNow.z = this->render.position.z;
}

// OFFSET: 0x00BBD480
// FUNCTION: fn_19805
void EffectParticle::PrimFirstAid() {
  FaceCamera(this->state.slots);

  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life != 1) {
    return;
  }

  ++slot.process;
  slot.height[8] += 3.0f;

  if (slot.height[8] >= 360.0f) {
    slot.height[8] -= 360.0f;
  }

  if (slot.process <= 20) {
    slot.alphaB += 4;
  }

  if (slot.process > 100) {
    --slot.alphaB;

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }
  }
}

// OFFSET: 0x00BBF3F0
// FUNCTION: fn_19816
void EffectParticle::PrimIntimidate() {
  FaceCamera(this->state.slots);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process > 0) {
      slot.distance += slot.height[0];
    }

    if (slot.process <= 10 && slot.process > 0) {
      if (slot.flag1[0] == 0) {
        slot.alphaB += 15;
      } else {
        slot.alphaB += 25;
      }
    } else if (slot.process > 25) {
      slot.alphaB -= 7;

      if (slot.alphaB <= 0) {
        slot.alphaB = 0;
      }
    }

    if (slot.flag1[0] == 2) {
      slot.rotStart += 3;
    }

    if (slot.rotStart >= 360) {
      slot.rotStart -= 360;
    }

    if (slot.flag1[0] == 2) {
      slot.height[2] += 3.0f;
    } else {
      slot.height[2] += 5.0f;
    }

    if (slot.height[2] >= 360.0f) {
      slot.height[2] -= 360.0f;
    }
  }
}

// OFFSET: 0x00BC09F0
// FUNCTION: fn_19824
void EffectParticle::PrimPoison() {
  FaceCamera(this->state.slots);

  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.maxHeight >= 30.0f) {
      slot.maxHeight -= 30.0f;
    }

    if (slot.flag1[10] == 1) {
      if (slot.process < 10) {
        if (slot.alphaB <= 45) {
          slot.alphaB += 10;
        }
      } else if (slot.process > 65) {
        if (slot.alphaB >= 10) {
          slot.alphaB -= 10;
        }
      }
    } else if (slot.alphaB <= 45) {
      slot.alphaB += 5;
    }
  }
}

// OFFSET: 0x00BBEA00
// FUNCTION: fn_19813
void EffectParticle::PrimGuard() {
  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process > 1 && slot.flag1[0] == 1) {
      slot.rotStart += 10;
    } else {
      slot.rotStart = static_cast<std::int16_t>(static_cast<int>(this->state.master->state.parent->render.angle + 270.0f));
    }

    if (slot.rotStart >= 360) {
      slot.rotStart -= 360;
    }

    if (slot.process < 10) {
      if (slot.alphaB < 100) {
        slot.alphaB += 20;
      }
    } else if (slot.process > 26) {
      slot.height[0] += 0.1f;

      if (slot.alphaB > 0) {
        slot.alphaB -= 6;
      }

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    }
  }
}

// OFFSET: 0x00BBFAA0
// FUNCTION: fn_19818
bool EffectParticle::PrimLineLink() {
  CGameMode* gameMode = static_cast<CGameMode*>(g_world.ActiveGameMode());
  const CRenderObject* target = nullptr;

  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  EffectPrimitiveSlot& slot = this->state.slots[0];
  ++slot.process;

  slot.vecTNow = {this->render.position.x, this->render.position.y, this->render.position.z};
  slot.vecTNow.y -= 8.0f;

  if (slot.height[4] == 2.0f) {
    if (slot.process > 40) {
      slot.alphaB -= 3;

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    } else if (slot.process < 20) {
      slot.alphaB += 6;
    }
  } else if (slot.height[4] == 3.0f) {
    const int phase = slot.process % 360;

    if (slot.process <= 80) {
      ++slot.alphaB;
    }

    slot.maxHeight += g_sinTable[phase] * 0.005f;
  } else if (slot.process % 10 < 5) {
    slot.alphaB += 2;
  } else {
    slot.alphaB -= 2;
  }

  if (slot.height[0] > 0.0f) {
    if (slot.height[0] == static_cast<float>(g_session.m_aid)) {
      target = reinterpret_cast<const CRenderObject*>(gameMode->scene->player);
    } else {
      target = gameMode->scene->GetGameActorByAID(static_cast<std::uint32_t>(slot.height[0]));
    }
  }

  if (target == nullptr) {
    slot.life = 0;
    slot.alphaB = 0;
  } else {
    slot.vecTPre = {target->render.position.x, target->render.position.y, target->render.position.z};
    slot.vecTPre.y -= 8.0f;
  }

  return true;
}

// OFFSET: 0x00BC1AA0
// FUNCTION: fn_19830
void EffectParticle::PrimSMA() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process <= 0) {
      continue;
    }

    if (slot.process == 1) {
      const float x = this->state.offset.x - this->render.position.x;
      const float z = this->state.offset.z - this->render.position.z;
      CalculateVectorAngle(x, z, &slot.rotStart);
    }

    slot.vecBNow.x += slot.height[1] * g_cosTable[slot.rotStart];
    slot.vecBNow.z += slot.height[1] * g_sinTable[slot.rotStart];
  }
}

// OFFSET: 0x00BC2A60
// FUNCTION: fn_19835
void EffectParticle::PrimShieldBoomerang3() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  EffectPrimitiveSlot& slot = slots[0];

  if (slot.life != 1) {
    return;
  }

  ++slot.process;

  if (slot.process <= 0) {
    return;
  }

  const char interval = 3;

  if (slot.process % interval == 0) {
    if (slot.process > 3) {
      FollowTrail(slots[3], slots[2], 25);
    }

    if (slot.process > 2) {
      FollowTrail(slots[2], slots[1], 25);
    }

    if (slot.process > 1) {
      FollowTrail(slots[1], slot, 150);
    }
  }

  slot.fullDisplayAngle -= 15;

  if (slot.fullDisplayAngle < 0) {
    slot.fullDisplayAngle += 360;
  }

  if (slot.process <= 5) {
    slot.alphaB += 45;

    if (slot.alphaB > 255) {
      slot.alphaB = 255;
    }
  }

  if (slot.process >= 15) {
    slot.alphaB -= 10;

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }
  }

  slot.vecBNow.x += slot.height[1] * g_cosTable[slot.rotStart];
  slot.vecBNow.z += slot.height[1] * g_sinTable[slot.rotStart];
}

// OFFSET: 0x00BC3C70
// FUNCTION: fn_19840
bool EffectParticle::PrimSuperAngel() {
  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life == 1) {
    ++slot.process;
    slot.vecBNow.y -= 0.1f;
    slot.vecBPre = slot.vecBNow;
    this->state.offset = slot.vecBPre;

    float& alpha = this->state.alpha;
    const char kind = slot.flag1[0];

    if (slot.process < 50) {
      if (slot.flag1[1] == 0) {
        if (kind == 1) {
          if (alpha < 200.0f) {
            alpha += 5.0f;
          }
        } else if (kind == 10) {
          if (alpha < 255.0f) {
            alpha += 10.0f;
          }

          if (alpha > 255.0f) {
            alpha = 255.0f;
          }
        } else if (kind == 11) {
          alpha = 0.0f;
        } else if (kind >= 2) {
          if (slot.process < 30 && alpha < 150.0f) {
            alpha += 5.0f;
          }
        } else if (alpha < 245.0f) {
          alpha += 5.0f;
        }
      } else if (alpha < static_cast<float>(40 - slot.flag1[1])) {
        alpha += 1.0f;
      }
    }

    if (slot.process > 30) {
      int& motion = this->render.curMotion;

      if (kind == 0) {
        if (slot.process % 3 == 0) {
          ++motion;
        }

        if (motion > 29) {
          motion = 29;
        }
      } else if (kind == 1) {
        if (slot.process % 3 == 0) {
          ++motion;
        }

        if (motion > 29) {
          motion = 29;
        }
      } else if (kind == 10 || kind == 11) {
        if (slot.process % 4 == 0) {
          ++motion;
        }

        if (motion > 15) {
          motion = 15;
        }
      } else if (kind >= 2) {
        if (slot.process % 3 == 0) {
          ++motion;
        }

        if (motion > 29) {
          motion = 29;
        }
      }
    }

    if (kind == 0) {
      if (slot.process > 100) {
        alpha -= 4.0f;

        if (alpha < 0.0f) {
          alpha = 0.0f;
        }
      }
    } else if (kind == 2) {
      if (slot.process > 30) {
        alpha -= 3.0f;

        if (alpha < 0.0f) {
          alpha = 0.0f;
        }
      }
    } else if (kind == 3) {
      if (slot.process > 60) {
        alpha -= 3.0f;

        if (alpha < 0.0f) {
          alpha = 0.0f;
        }
      }
    } else if (kind == 4) {
      if (slot.process > 70) {
        alpha -= 3.0f;

        if (alpha < 0.0f) {
          alpha = 0.0f;
        }
      }
    } else if (kind == 10) {
      if (slot.process > 100) {
        alpha -= 6.0f;

        if (alpha < 0.0f) {
          alpha = 0.0f;
        }
      }
    } else if (slot.flag1[1] != 0) {
      if (slot.process > 75) {
        alpha -= 3.0f;

        if (alpha < 0.0f) {
          alpha = 0.0f;
        }
      }
    } else if (slot.process > 75) {
      alpha -= 6.0f;

      if (alpha < 0.0f) {
        alpha = 0.0f;
      }
    }
  }

  this->render.position.x = this->state.offset.x;
  this->render.position.y = this->state.offset.y;
  this->render.position.z = this->state.offset.z;
  this->UpdateRecords();
  return true;
}

// OFFSET: 0x00BC4730
// FUNCTION: fn_19844
void EffectParticle::PrimThrowItem() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  const AttachedEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  EffectPrimitiveSlot& slot = slots[0];

  if (slot.life != 1) {
    return;
  }

  ++slot.process;

  if (slot.process % 2 == 1) {
    if (slot.process > 3) {
      FollowTrail(slots[3], slots[2], 25);
    }

    if (slot.process > 2) {
      FollowTrail(slots[2], slots[1], 25);
    }

    if (slot.process > 1) {
      FollowTrail(slots[1], slot, 150);
    }
  }

  if (slot.flag1[2] == 15) {
    slot.fullDisplayAngle += 45;
  } else {
    slot.fullDisplayAngle += 10;
  }

  if (slot.fullDisplayAngle >= 360) {
    slot.fullDisplayAngle -= 360;
  }

  if (slot.process == 1) {
    const float x = this->state.offset.x - slot.vecBNow.x;
    const float z = this->state.offset.z - slot.vecBNow.z;
    CalculateVectorAngle(x, z, &slot.rotStart);
  }

  float progress = static_cast<float>(slot.process);
  progress /= static_cast<float>(slot.flag1[1]);

  if (progress > 1.05f) {
    slot.alphaB -= 20;

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }

    return;
  }

  if (progress > 1.0f) {
    progress = 1.0f;
  }

  const int arc = static_cast<int>(180.0f * progress);
  slot.vecBPre.y += slot.height[2];

  if (slot.flag1[2] == 0) {
    slot.vecBNow.y = slot.vecBPre.y - g_sinTable[arc] * 10.0f;
  } else if (slot.flag1[2] == 15) {
    slot.vecBNow.y = slot.vecBPre.y - g_sinTable[arc];
  } else {
    slot.vecBNow.y = slot.vecBPre.y - g_sinTable[arc] * 30.0f;
  }

  slot.vecBNow.x += slot.height[1] * g_cosTable[slot.rotStart];
  slot.vecBNow.z += slot.height[1] * g_sinTable[slot.rotStart];
}

// OFFSET: 0x00BC06B0
// FUNCTION: fn_19822
bool EffectParticle::PrimPeong() {
  FaceCamera(this->state.slots);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process <= 0) {
      continue;
    }

    if (slot.process <= 90) {
      const float cosine = g_cosTable[slot.riseAngle];
      float sine = g_sinTable[slot.riseAngle];
      slot.vecBPre.x += cosine * slot.maxHeight;
      slot.vecBPre.z += sine * slot.maxHeight;

      int angle = slot.process * 2;

      if (angle >= 360) {
        angle -= 360;
      }

      sine = g_sinTable[angle];
      slot.vecBPre.y = slot.vecBNow.y - sine * slot.maxHeight * 50.0f;
    }

    if (slot.process <= 25) {
      slot.alphaB += 2;
    }

    if (slot.process > 70) {
      --slot.alphaB;
    }

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }
  }

  return true;
}

// OFFSET: 0x00BBDE10
// FUNCTION: fn_19810
void EffectParticle::PrimGI3() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    slot.process += 1;
    slot.distance -= 0.01f;

    if (slot.process >= 10) {
      slot.alphaB -= 1;

      if (slot.alphaB <= 0) {
        slot.alphaB = 0;

        if (slot.process > 30) {
          slot.life = 0;
        }

        slot.distance = 20.0f;
        slot.riseAngle = 81 + 9;
      }
    } else {
      slot.alphaB += 10;
    }

    for (int sample = 0; sample < 21; ++sample) {
      slot.height[sample] = slot.maxHeight * g_sinTable[slot.process];
    }
  }
}

// OFFSET: 0x00BEBC40
// FUNCTION: fn_19971
void EffectParticle::PrimWaterFall() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life > 0) {
      ++slot.process;
    }
  }
}

// OFFSET: 0x00BB6F30
// FUNCTION: fn_19779
bool EffectParticle::UpdateHit6() {
  const AttachedEffect* master = this->state.master;
  this->state.origin.x = master->render.position.x + this->state.velocity.x;
  this->state.origin.y = master->render.position.y + this->state.velocity.y;
  this->state.origin.z = master->render.position.z + this->state.velocity.z;

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

  this->render.position = {
    this->state.offset.x + this->state.origin.x, this->state.offset.y + this->state.origin.y,
    this->state.offset.z + this->state.origin.z
  };

  if ((this->state.flags & 0x100u) != 0) {
    if (3.0f >= this->HorizontalDistance(this->state.master->state.offset[0], this->state.master->state.offset[2])) {
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

// OFFSET: 0x00BB75B0
// FUNCTION: fn_19780
bool EffectParticle::UpdateCoin() {
  CGameMode* mode = static_cast<CGameMode*>(g_world.ActiveGameMode());
  const AttachedEffect* master = this->state.master;
  this->state.origin.z = master->render.position.z + this->state.velocity.z;
  this->state.origin.y = master->render.position.y + this->state.velocity.y;
  this->state.origin.x = master->render.position.x + this->state.velocity.x;
  this->state.unknown_02EC = this->state.unknown_02F0 + this->state.unknown_02EC;

  const float speed = this->state.unknown_02C0 + this->state.unknown_02BC;
  this->state.unknown_02BC = speed < 0.0f ? 0.0f : speed;
  this->state.unknown_0370 = Forward(this->state.unknown_02BC, this->state.matrix1);

  this->state.offset.x = this->state.unknown_0370.x + this->state.offset.x;
  this->state.offset.y = this->state.offset.y + this->state.unknown_02EC;
  this->state.offset.z = this->state.unknown_0370.z + this->state.offset.z;

  this->render.position.x = this->state.offset.x + this->state.origin.x;
  this->render.position.y = this->state.offset.y + this->state.origin.y;
  this->render.position.z = this->state.offset.z + this->state.origin.z;
  this->state.scale = this->state.unknown_031C + this->state.scale;

  if (this->state.unknown_02EC > 0.0f && this->render.position.y > mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z)) {
    this->state.unknown_02EC = this->state.unknown_02EC / -1.5f;

    if (this->state.unknown_0208 == 1) {
      char name[52];
      std::sprintf(name, "effect\\EF_Coin%d.wav", rand() % 3 + 1);
      const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_world.ActiveGameMode())->scene->player);
      g_soundMgr->PlayWave(
        name, this->render.position.x - player->render.position.x, 0.0f, this->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
      );
    }
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

  this->render.position.x = positionX + this->state.offset.x;
  this->render.position.y = positionY + this->state.offset.y;
  this->render.position.z = positionZ + this->state.offset.z;

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
  const AttachedEffect* master = this->state.master;
  this->state.origin = {master->render.position.x, master->render.position.y, master->render.position.z};

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
  this->render.position.x = this->state.offset.x + this->state.origin.x;
  this->render.position.y = this->state.offset.y + this->state.origin.y;
  this->render.position.z = this->state.offset.z + this->state.origin.z;

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
  const AttachedEffect* master = this->state.master;
  this->state.origin = {
    master->render.position.x + this->state.velocity.x, master->render.position.y + this->state.velocity.y,
    master->render.position.z + this->state.velocity.z
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

  this->render.position.x = this->state.offset.x + this->state.origin.x + this->state.unknown_03A0.x;
  this->render.position.y = this->state.offset.y + this->state.origin.y + this->state.unknown_03A0.y;
  this->render.position.z = this->state.offset.z + this->state.origin.z + this->state.unknown_03A0.z;

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
    record.alpha = this->state.alpha - this->state.alpha / static_cast<float>(count) * static_cast<float>(index);

    const std::uint32_t red = static_cast<unsigned char>(this->state.red);
    const std::uint32_t green = static_cast<unsigned char>(this->state.green);
    const std::uint32_t blue = static_cast<unsigned char>(this->state.blue);
    record.color = (static_cast<std::uint32_t>(static_cast<int>(record.alpha)) << 24) | (red << 16) | (green << 8) | blue;
    record.scale = this->state.scale - this->state.scale / static_cast<float>(count) * static_cast<float>(index) * 0.5f;
  }

  EffectPrimitiveRecord& first = this->state.records[0];
  first.position.x = this->render.position.x;
  first.position.y = this->render.position.y;
  first.position.z = this->render.position.z;

  for (int corner = 0; corner < 4; ++corner) {
    first.corners[corner] = this->state.corners[corner];
  }

  first.unknown_003C = this->state.unknown_0348;
  first.scale = this->state.scale;
  first.angle = this->state.unknown_030C;
  std::memcpy(first.matrix, this->state.matrix1, sizeof(first.matrix));
  first.alpha = this->state.alpha;

  const std::uint32_t red = static_cast<unsigned char>(this->state.red);
  const std::uint32_t green = static_cast<unsigned char>(this->state.green);
  const std::uint32_t blue = static_cast<unsigned char>(this->state.blue);
  first.color = (static_cast<std::uint32_t>(static_cast<int>(this->state.alpha)) << 24) | (red << 16) | (green << 8) | blue;
}

// INLINED
static void FollowMaster(EffectParticle& particle) {
  const AttachedEffect* master = particle.state.master;
  particle.render.position.x = master->render.position.x;
  particle.render.position.y = master->render.position.y;
  particle.render.position.z = master->render.position.z;
}

// INLINED
static void TickLiveSlots(EffectPrimitiveSlot* slots) {
  for (int index = 0; index < 4; ++index) {
    if (slots[index].life == 1) {
      ++slots[index].process;
    }
  }
}

// OFFSET: 0x00BEB020
// FUNCTION: fn_19966
bool EffectParticle::FollowMasterAndAdvance() {
  FollowMaster(*this);
  return this->AdvanceFrame();
}

// OFFSET: 0x00BE9560
// FUNCTION: fn_19952
void EffectParticle::FollowMasterAndTickSlots() {
  FollowMaster(*this);
  TickLiveSlots(this->state.slots);
}

// OFFSET: 0x00BE8770
