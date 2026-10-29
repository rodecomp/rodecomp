#include "Contents/Effects/EffectPrimitiveMotion.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Actors/Pc.h"
#include "Contents/World/World.h"
#include "Resource/Sprites/ActionResource.h"
#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/EffectActor.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/RagEffectState.h"
#include "Contents/Sound/SoundMgr.h"
#include "Resource/World/TerrainResource.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"
#include "Render/Renderer.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Projection/RenderProjection.h"

#include <bit>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
  // INLINED
  __forceinline int Random(int range) {
    if (range == 0) {
      return 0;
    }

    return std::abs(rand() % range);
  }

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
    const CView* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
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
  particle.orgPos = {
    context.parentRender.position.x + particle.deltaPos2.x, context.parentRender.position.y + particle.deltaPos2.y,
    context.parentRender.position.z + particle.deltaPos2.z
  };

  if (particle.changePoint == particle.stateCnt) {
    particle.unknown_0138 = 1;
    constexpr float sentinel = 0.0001f;

    if (particle.changeWidthSpeed != sentinel) {
      particle.widthSpeed = particle.changeWidthSpeed;
    }

    if (particle.changeHeightSpeed != sentinel) {
      particle.heightSpeed = particle.changeHeightSpeed;
    }

    if (particle.changeSpeed != sentinel) {
      particle.speed = particle.changeSpeed;
    }

    if (particle.changeWidthAccel != sentinel) {
      particle.widthAccel = particle.changeWidthAccel;
    }

    if (particle.changeHeightAccel != sentinel) {
      particle.heightAccel = particle.changeHeightAccel;
    }

    if (particle.changeAccel != sentinel) {
      particle.accel = particle.changeAccel;
    }

    if (particle.changeSpeed3d.x != sentinel) {
      particle.speed3d.x = particle.changeSpeed3d.x;
    }

    if (particle.changeSpeed3d.y != sentinel) {
      particle.speed3d.y = particle.changeSpeed3d.y;
    }

    if (particle.changeSpeed3d.z != sentinel) {
      particle.speed3d.z = particle.changeSpeed3d.z;
    }

    particle.changeWidthSpeed = sentinel;
    particle.changeHeightSpeed = sentinel;
    particle.changeSpeed = sentinel;
    particle.changeWidthAccel = sentinel;
    particle.changeHeightAccel = sentinel;
    particle.changeAccel = sentinel;
    particle.changeSpeed3d = {sentinel, sentinel, sentinel};
    particle.changeAccel3d = {sentinel, sentinel, sentinel};
  }

  particle.speed3d.x = particle.speed3d.x + particle.deltaPosAccel.x;
  particle.speed3d.y = particle.deltaPosAccel.y + particle.speed3d.y;
  particle.speed3d.z = particle.deltaPosAccel.z + particle.speed3d.z;
  particle.widthSpeed = particle.widthAccel + particle.widthSpeed;
  particle.speed = particle.accel + particle.speed;
  particle.heightSpeed = particle.heightAccel + particle.heightSpeed;
  particle.longSpeed = particle.longAccel + particle.longSpeed;
  particle.latiSpeed = particle.latiAccel + particle.latiSpeed;

  if ((particle.pattern & 0x400u) != 0) {
    particle.speed3d = {};
    particle.speed3d = Forward(particle.speed, particle.transform);
    particle.deltaPos.x = particle.speed3d.x + particle.deltaPos.x;
    particle.deltaPos.z = particle.speed3d.z + particle.deltaPos.z;
    particle.deltaPos.y = particle.speed3d.y + particle.deltaPos.y;
  } else if ((particle.pattern & 0x100u) != 0) {
    particle.deltaPos.x = particle.speed3d.x + particle.deltaPos.x;
    particle.deltaPos.y = particle.speed3d.y + particle.deltaPos.y;
    particle.deltaPos.z = particle.speed3d.z + particle.deltaPos.z;
  } else {
    particle.speed3d = Forward(particle.speed, particle.transform);
    particle.deltaPos.x = particle.speed3d.x + particle.deltaPos.x;
    particle.deltaPos.z = particle.speed3d.z + particle.deltaPos.z;
    particle.deltaPos.y = particle.speed3d.y + particle.deltaPos.y;
  }

  if ((particle.pattern & 0x400u) != 0) {
    const float x = particle.master->state.deltaPos[0] - render.position.x;
    const float z = particle.master->state.deltaPos[2] - render.position.z;
    const float distance = std::sqrt(z * z + x * x);

    if (3.0f >= distance) {
      particle.unknown_014C.x = std::bit_cast<float>(1u);
      return false;
    }

    particle.longitude = render.angle;
  }

  render.position.x = particle.orgPos.x + particle.deltaPos.x;
  render.position.y = particle.deltaPos.y + particle.orgPos.y;
  render.position.z = particle.deltaPos.z + particle.orgPos.z;
  particle.widthSize = particle.widthSpeed + particle.widthSize;
  particle.heightSize = particle.heightSpeed + particle.heightSize;
  particle.longitude = particle.longitude + particle.longSpeed;
  particle.latitude = particle.latitude + particle.latiSpeed;

  if (!(particle.heightSize <= particle.maxHeightSize)) {
    particle.heightAccel = 0;
    particle.heightSpeed = 0;
  }

  struct matrix matrix;

  if ((particle.pattern & 0x200u) != 0) {
    matrix.MakeZRotation(context.cameraPitch);
  } else if ((particle.pattern & 0x40u) != 0) {
    particle.longitude = context.cameraYaw + 90.0f;
    particle.roll = context.cameraPitch;
    matrix.MakeXRotation(GetRadian(TruncateToInt(particle.latitude)));
    matrix.AppendZRotation(GetRadian(TruncateToInt(particle.roll)));
  } else {
    matrix.MakeZRotation(GetRadian(TruncateToInt(particle.roll)));
    matrix.AppendXRotation(GetRadian(TruncateToInt(particle.latitude)));
  }

  const float angle = (particle.pattern & 0x200u) != 0 ? render.angle : particle.longitude;
  matrix.AppendYRotation(GetRadian(TruncateToInt(angle)));
  std::memcpy(particle.rotTransform, matrix.values, sizeof(particle.rotTransform));

  if ((particle.pattern & 2u) != 0) {
    const auto index = particle.chCnt;
    particle.chCnt = index + 1u == 10u ? 0u : index + 1u;
    const float offset = static_cast<float>(static_cast<int>(index)) / 10.0f;
    particle.v1 = offset;
    particle.v2 = offset + 1.0f;
  }

  if ((particle.pattern & 0x10u) != 0) {
    particle.master->state.deltaPos[0] = particle.deltaPos2.x + particle.deltaPos.x;
    particle.master->state.deltaPos[1] = particle.deltaPos.y + particle.deltaPos2.y;
    particle.master->state.deltaPos[2] = particle.deltaPos.z + particle.deltaPos2.z;
  }

  if ((particle.pattern & 4u) != 0 && render.position.y > context.ground.Height(render.position.x, render.position.z)) {
    particle.pattern &= ~4u;
    particle.stateCnt = particle.duration - 25;
    particle.speed = 0;
    particle.master->state.flag = 1;
  }

  const int frame = particle.stateCnt;

  if ((particle.pattern & 0x20u) != 0 && frame % static_cast<int>(particle.type) == 0) {
    particle.heightSpeed *= -1.0f;
  }

  const bool wrap254 = (particle.pattern & 0x800u) != 0;
  const bool gated = wrap254 && (particle.red == 254 || particle.green == 254 || particle.blue == 254);
  bool advance = true;

  if (gated) {
    if (particle.RGBCycleDelay != 0) {
      ++particle.RGBCycleCnt;
    }

    advance = particle.RGBCycleDelay == particle.RGBCycleCnt;
  }

  if (advance) {
    particle.red += particle.redSpeed;
    particle.green += particle.greenSpeed;
    particle.blue += particle.blueSpeed;

    if (gated) {
      particle.RGBCycleCnt = 0;
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

  particle.ProcessAlpha();

  if (particle.stateCnt % particle.animSpeed == 0) {
    const auto next = render.curMotion + 1u;
    render.curMotion = next;

    if (particle.repeatAnim != 0) {
      render.curMotion = static_cast<int>(next) % static_cast<int>(particle.totalTexture);
    } else if (next == particle.totalTexture) {
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
  ++particle.stateCnt;
  return particle.stateCnt <= particle.duration;
}

// OFFSET: 0x00BB6180
// FUNCTION: fn_19773
bool Prim3DCasting4(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
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

    if (slot.process >= particle.duration - 40) {
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
bool Prim3DCasting5(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
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
bool Prim3DCylinder(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  particle.orgPos = {
    context.parentRender.position.x + particle.deltaPos2.x, context.parentRender.position.y + particle.deltaPos2.y,
    context.parentRender.position.z + particle.deltaPos2.z
  };

  particle.speed = particle.accel + particle.speed;
  particle.latiSpeed = particle.latiAccel + particle.latiSpeed;
  particle.heightSpeed = particle.heightAccel + particle.heightSpeed;
  particle.latitude = particle.latiSpeed + particle.latitude;
  particle.outerSpeed = particle.outerAccel + particle.outerSpeed;
  particle.innerSpeed = particle.innerAccel + particle.innerSpeed;
  particle.longSpeed = particle.longAccel + particle.longSpeed;
  particle.longitude = particle.longSpeed + particle.longitude;

  struct matrix& matrix = *reinterpret_cast<struct matrix*>(particle.transform);
  matrix.MakeXRotation(GetRadian(static_cast<int>(particle.latitude)));
  matrix.AppendYRotation(GetRadian(static_cast<int>(particle.longitude)));

  particle.speed3d = Upward(particle.speed, particle.transform);
  particle.deltaPos.y = particle.speed3d.y + particle.deltaPos.y;
  particle.deltaPos.x = particle.speed3d.x + particle.deltaPos.x;
  particle.deltaPos.z = particle.speed3d.z + particle.deltaPos.z;
  particle.longitude = particle.longSpeed + particle.longitude;
  particle.innerSize += particle.innerSpeed;
  render.position.x = particle.deltaPos.x + particle.orgPos.x;
  render.position.y = particle.deltaPos.y + particle.orgPos.y;
  render.position.z = particle.deltaPos.z + particle.orgPos.z;
  particle.outerSize += particle.outerSpeed;
  particle.heightSize = particle.heightSpeed + particle.heightSize;

  if (particle.heightSize > particle.maxHeightSize) {
    particle.heightSize = particle.maxHeightSize;
  }

  if ((particle.pattern & 1u) != 0 && particle.stateCnt == particle.fadeOutCnt) {
    particle.outerSpeed = -(particle.outerSize / static_cast<float>(particle.duration - particle.stateCnt + 1));
  }

  particle.ProcessAlpha();

  if (particle.stateCnt % particle.animSpeed == 0) {
    const auto next = render.curMotion + 1u;
    render.curMotion = next;

    if (particle.repeatAnim != 0) {
      render.curMotion = static_cast<int>(next) % static_cast<int>(particle.totalTexture);
    } else if (next == particle.totalTexture) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00BB6B30
// FUNCTION: fn_19778
bool Prim3DCylinderHalf(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  particle.orgPos = {
    context.parentRender.position.x + particle.deltaPos2.x, context.parentRender.position.y + particle.deltaPos2.y,
    context.parentRender.position.z + particle.deltaPos2.z
  };

  particle.speed = particle.accel + particle.speed;
  particle.outerSpeed = particle.outerAccel + particle.outerSpeed;
  particle.innerSpeed = particle.innerAccel + particle.innerSpeed;
  particle.heightSpeed = particle.heightAccel + particle.heightSpeed;

  struct matrix& matrix = *reinterpret_cast<struct matrix*>(particle.transform);
  matrix.MakeXRotation(GetRadian(static_cast<int>(particle.latitude)));
  matrix.AppendYRotation(GetRadian(static_cast<int>(particle.longitude)));

  particle.speed3d = Upward(particle.speed, particle.transform);
  particle.deltaPos.x = particle.speed3d.x + particle.deltaPos.x;
  particle.deltaPos.y = particle.speed3d.y + particle.deltaPos.y;
  particle.deltaPos.z = particle.speed3d.z + particle.deltaPos.z;
  render.position.x = particle.deltaPos.x + particle.orgPos.x;
  render.position.y = particle.deltaPos.y + particle.orgPos.y;
  render.position.z = particle.deltaPos.z + particle.orgPos.z;
  particle.longitude = particle.longSpeed + particle.longitude;
  particle.outerSize = particle.outerSpeed + particle.outerSize;
  particle.innerSize = particle.innerSpeed + particle.innerSize;
  particle.heightSize += particle.heightSpeed;

  if (particle.heightSize > particle.maxHeightSize) {
    particle.heightSize = particle.maxHeightSize;
  }

  if ((particle.pattern & 1u) != 0 && particle.stateCnt == particle.fadeOutCnt) {
    particle.outerSpeed = -(particle.outerSize / static_cast<float>(particle.duration - particle.stateCnt + 1));
  }

  particle.ProcessAlpha();

  if (particle.stateCnt % particle.animSpeed == 0) {
    const auto next = render.curMotion + 1u;
    render.curMotion = next;

    if (particle.repeatAnim != 0) {
      render.curMotion = static_cast<int>(next) % static_cast<int>(particle.totalTexture);
    } else if (next == particle.totalTexture) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00BB8610
// FUNCTION: fn_19784
void AdvanceEffectPrimitive(EffectPrimitiveState& particle, RenderObjectState& render, EffectMotionContext& context) {
  particle.orgPos = {
    context.parentRender.position.x + particle.deltaPos2.x, context.parentRender.position.y + particle.deltaPos2.y,
    context.parentRender.position.z + particle.deltaPos2.z
  };

  if (particle.stateCnt == particle.changePoint) {
    particle.speed = particle.changeSpeed;
    particle.accel = particle.changeAccel;
  }

  particle.speed = particle.accel + particle.speed;
  particle.speed3d = Forward(particle.speed, particle.transform);
  particle.deltaPos.x = particle.speed3d.x + particle.deltaPos.x;
  particle.deltaPos.y = particle.speed3d.y + particle.deltaPos.y;
  particle.deltaPos.z = particle.speed3d.z + particle.deltaPos.z;
  render.position.x = particle.deltaPos.x + particle.orgPos.x;
  render.position.y = particle.deltaPos.y + particle.orgPos.y;
  render.position.z = particle.deltaPos.z + particle.orgPos.z;

  if ((particle.pattern & 1u) != 0 && particle.stateCnt == static_cast<int>(particle.chCnt)) {
    particle.accel = 0;
    particle.speed = 0;
  }

  if ((particle.pattern & 2u) != 0) {
    if (particle.stateCnt == particle.duration - 30) {
      particle.speed = particle.changeSpeed3d.y;
      particle.accel = particle.changeAccel3d.y;
    }
  } else {
    particle.ProcessAlpha();
  }
}

// OFFSET: 0x00BAE3A0
// FUNCTION: fn_19764
bool CEffectPrim::AnimMotion() {
  CActRes* action = this->render.action;

  if (this->state.stateCnt % this->state.animSpeed != 0) {
    return true;
  }

  const unsigned animation = static_cast<unsigned>(this->render.previousAnimation);
  ++this->render.curMotion;

  if (this->state.repeatAnim != 0) {
    this->render.curMotion =
      static_cast<std::uint32_t>(static_cast<int>(this->render.curMotion) % static_cast<int>(action->FrameCount(animation)));
    return true;
  }

  if ((this->state.pattern & 0x800u) != 0) {
    if (this->render.curMotion == action->FrameCount(animation)) {
      --this->render.curMotion;
    }

    return true;
  }

  return this->render.curMotion != action->FrameCount(animation);
}

// OFFSET: 0x00BB5E80
// FUNCTION: fn_19772
bool CEffectPrim::UpdateFireWall() {
  const CRagEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  for (int index = 0; index < 4; ++index) {
    if (this->state.size == 3.0f && index < 3) {
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

    const int limit = this->state.duration - 40;
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
bool CEffectPrim::UpdateStoneCurse() {
  const CRagEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  for (int index = 0; index < 4; ++index) {
    if (this->state.size == 3.0f && index < 3) {
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

    if (slot.process < this->state.duration - 40) {
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
void CEffectPrim::Prim3DCircle() {
  const CRagEffect* master = this->state.master;
  this->state.orgPos.x = master->render.position.x + this->state.deltaPos2.x;
  this->state.orgPos.y = master->render.position.y + this->state.deltaPos2.y;
  this->state.orgPos.z = master->render.position.z + this->state.deltaPos2.z;

  float base;

  if (this->state.changePoint == this->state.stateCnt) {
    this->state.radiusAccel = this->state.changeAccel;
    base = this->state.changeSpeed;
  } else {
    base = this->state.radiusSpeed;
  }

  const float horizontal = base + this->state.radiusAccel;
  const float forwardX = this->state.longAccel + this->state.longSpeed;
  this->state.speed = this->state.accel + this->state.speed;
  const float forwardZ = this->state.innerAccel + this->state.innerSpeed;
  this->state.longSpeed = forwardX;
  const float angle = forwardX + this->state.longitude;
  this->state.radiusSpeed = horizontal;
  const float side = horizontal + this->state.radius;
  this->state.innerSpeed = forwardZ;
  const float vertical = forwardZ + this->state.innerSize;
  this->state.longitude = angle;
  this->state.radius = side;
  this->state.innerSize = vertical;

  reinterpret_cast<struct matrix*>(this->state.transform)->MakeXRotation(GetRadian(static_cast<int>(this->state.latitude)));
  reinterpret_cast<struct matrix*>(this->state.transform)->AppendYRotation(GetRadian(static_cast<int>(this->state.longitude)));

  const float distance = this->state.speed;
  this->state.speed3d = Forward(distance, this->state.transform);
  const float offsetX = this->state.speed3d.x + this->state.deltaPos.x;
  const float offsetY = this->state.speed3d.y + this->state.deltaPos.y;
  const float offsetZ = this->state.speed3d.z + this->state.deltaPos.z;
  this->state.deltaPos.x = offsetX;
  this->state.deltaPos.y = offsetY;
  this->state.deltaPos.z = offsetZ;
  this->render.position.x = offsetX + this->state.orgPos.x;
  this->render.position.y = offsetY + this->state.orgPos.y;
  this->render.position.z = offsetZ + this->state.orgPos.z;

  float scale = this->state.innerSize;

  if ((this->state.pattern & 1u) != 0 || scale > this->state.radius) {
    scale = this->state.radius;
  }

  this->state.size = scale;
  this->state.ProcessAlpha();
  this->ShiftSegment();
}

// OFFSET: 0x00BBAB10
// FUNCTION: fn_19790
void CEffectPrim::PrimBottom2() {
  const CRagEffect* master = this->state.master;
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
void CEffectPrim::PrimBottom() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  const CRagEffect* master = this->state.master;
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
void CEffectPrim::PrimBottomOut() {
  FaceCamera(this->state.slots);

  const CRagEffect* master = this->state.master;
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
void CEffectPrim::PrimBubbleDrop() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  const CRagEffect* master = this->state.master;
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
void CEffectPrim::PrimChemical2() {
  FaceCamera(this->state.slots);

  const CRagEffect* master = this->state.master;
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
void CEffectPrim::PrimChemical3() {
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
void CEffectPrim::PrimChemical() {
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
void CEffectPrim::PrimEffectTexture2() {
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
void CEffectPrim::PrimEffectTexture_Ani() {
  FaceCamera(this->state.slots);

  const CRagEffect* master = this->state.master;
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

    if (slot.flag1[0] >= static_cast<int>(this->state.totalTexture)) {
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
void CEffectPrim::PrimFirstAid() {
  FaceCamera(this->state.slots);

  const CRagEffect* master = this->state.master;
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
void CEffectPrim::PrimIntimidate() {
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
void CEffectPrim::PrimPoison() {
  FaceCamera(this->state.slots);

  const CRagEffect* master = this->state.master;
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
void CEffectPrim::PrimGuard() {
  const CRagEffect* master = this->state.master;
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
bool CEffectPrim::PrimLineLink() {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const CRenderObject* target = nullptr;

  const CRagEffect* master = this->state.master;
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
void CEffectPrim::PrimSMA() {
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
      const float x = this->state.deltaPos.x - this->render.position.x;
      const float z = this->state.deltaPos.z - this->render.position.z;
      Get_Angle(x, z, &slot.rotStart);
    }

    slot.vecBNow.x += slot.height[1] * g_cosTable[slot.rotStart];
    slot.vecBNow.z += slot.height[1] * g_sinTable[slot.rotStart];
  }
}

// OFFSET: 0x00BC2A60
// FUNCTION: fn_19835
void CEffectPrim::PrimShieldBoomerang3() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  const CRagEffect* master = this->state.master;
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

// OFFSET: 0x00BC2D60
// FUNCTION: fn_19836
void CEffectPrim::PrimShieldBoomerang() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);
  this->render.position = this->state.master->render.position;

  EffectPrimitiveSlot& slot = slots[0];

  if (slot.life != 1) {
    return;
  }

  ++slot.process;

  if (slot.process <= 0) {
    return;
  }

  const char interval = slot.flag1[2] == 1 ? 1 : 3;

  if (slot.flag1[2] == 1) {
    if (slot.process <= 10) {
      slot.alphaB += 25;
    }

    if (slot.alphaB >= 255) {
      slot.alphaB = 255;
    }
  }

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

  if (slot.process == 1) {
    const float rx = this->state.deltaPos.x - slot.vecBNow.x;
    const float rz = this->state.deltaPos.z - slot.vecBNow.z;
    Get_Angle(rx, rz, &slot.rotStart);
  }

  if (slot.flag1[1] == 0) {
    if (slot.maxHeight < slot.height[1] * slot.process) {
      if (slot.flag1[2] == 1) {
        slot.flag1[1] = 2;
        slot.height[1] *= 3;

        if (Random(2) == 0) {
          slot.rotStart += Random(45);

          if (slot.rotStart >= 360) {
            slot.rotStart -= 360;
          }
        } else {
          slot.rotStart -= Random(45);

          if (slot.rotStart < 0) {
            slot.rotStart += 360;
          }
        }
      } else {
        slot.flag1[1] = 1;
      }
    }

    slot.vecBNow.y += slot.height[2];
  } else if (slot.flag1[1] == 1) {
    const float rx = this->render.position.x - slot.vecBNow.x;
    const float rz = this->render.position.z - slot.vecBNow.z;
    std::int16_t angle;
    Get_Angle(rx, rz, &angle);

    if (slot.rotStart < angle - 4 || slot.rotStart > angle + 4) {
      slot.rotStart += 5;

      if (slot.rotStart >= 360) {
        slot.rotStart -= 360;
      }
    } else {
      slot.flag1[1] = 2;
    }
  } else {
    slot.alphaB -= 12;

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }

    slot.vecBNow.y -= slot.height[2] * 1.0f;
  }

  slot.vecBNow.x += slot.height[1] * g_cosTable[slot.rotStart];
  slot.vecBNow.z += slot.height[1] * g_sinTable[slot.rotStart];
}

// OFFSET: 0x00BBF520
// FUNCTION: fn_19817
bool CEffectPrim::PrimKouenka() {
  this->render.position = this->state.master->render.position;

  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life == 1) {
    for (int i = 0; i < 4; i += 2) {
      if (slot.height[i + 1] > slot.height[i]) {
        slot.height[i] += 3;

        if (slot.height[i] > 359) {
          slot.height[i] = 359;
        }

        if (slot.height[i] > slot.height[i + 1]) {
          slot.height[i + 1] = static_cast<float>(Random(static_cast<int>(slot.height[i])));
        }
      } else {
        slot.height[i] -= 2;

        if (slot.height[i] < 0) {
          slot.height[i] = 0;
        }

        if (slot.height[i] <= slot.height[i + 1]) {
          slot.height[i + 1] = static_cast<float>(359 - Random(static_cast<int>(slot.height[i])));
        }
      }
    }

    slot.vecBPre.y -= slot.maxHeight;
    this->state.alpha -= 3;

    if (this->state.alpha < 3) {
      this->state.alpha = 0;
    }

    slot.vecBPre.x += 0.15f * g_sinTable[static_cast<int>(slot.height[0])];
    slot.vecBPre.z += 0.15f * g_sinTable[static_cast<int>(slot.height[2])];
    this->state.deltaPos = slot.vecBPre;
  }

  this->render.position = this->state.deltaPos;

  if ((this->state.pattern & 0x20) != 0) {
    this->state.roll = this->render.angle - static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;
  }

  this->state.ProcessAlpha();
  this->ShiftSegment();
  return this->AnimMotion();
}

// OFFSET: 0x00BC27B0
// FUNCTION: fn_19834
bool CEffectPrim::PrimSakura() {
  this->render.position = this->state.master->render.position;

  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life == 1) {
    for (int i = 0; i < 4; i += 2) {
      if (slot.height[i + 1] > slot.height[i]) {
        slot.height[i] += 3;

        if (slot.height[i] > 359) {
          slot.height[i] = 359;
        }

        if (slot.height[i] > slot.height[i + 1]) {
          slot.height[i + 1] = static_cast<float>(Random(static_cast<int>(slot.height[i])));
        }
      } else {
        slot.height[i] -= 2;

        if (slot.height[i] < 0) {
          slot.height[i] = 0;
        }

        if (slot.height[i] <= slot.height[i + 1]) {
          slot.height[i + 1] = static_cast<float>(359 - Random(static_cast<int>(slot.height[i])));
        }
      }
    }

    slot.vecBPre.y += slot.maxHeight;

    if (slot.flag1[0] == 0) {
      slot.vecBPre.x += 0.24f * g_sinTable[static_cast<int>(slot.height[0])];
      slot.vecBPre.z += 0.30f * g_sinTable[static_cast<int>(slot.height[2])];
    } else {
      slot.vecBPre.x += 0.12f * g_sinTable[static_cast<int>(slot.height[0])];
      slot.vecBPre.z += 0.15f * g_sinTable[static_cast<int>(slot.height[2])];
    }

    if (slot.vecBPre.y > this->render.position.y) {
      slot.vecBPre.x = this->render.position.x + Random(300) - 150;
      slot.vecBPre.y = this->render.position.y - 200;
      slot.vecBPre.z = this->render.position.z + Random(300) - 150;
    }

    this->state.deltaPos = slot.vecBPre;
  }

  this->render.position = this->state.deltaPos;

  if ((this->state.pattern & 0x20) != 0) {
    this->state.roll = this->render.angle - static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;
  }

  this->state.ProcessAlpha();
  this->ShiftSegment();
  return this->AnimMotion();
}

// OFFSET: 0x00BC4410
// FUNCTION: fn_19843
void CEffectPrim::PrimTeiHit2() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);
  this->render.position = this->state.master->render.position;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = slots[ec];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process <= 0) {
      continue;
    }

    slot.fullDisplayAngle -= 5;

    if (slot.fullDisplayAngle < 0) {
      slot.fullDisplayAngle += 360;
    }

    if (slot.process == 1) {
      const float rx = this->state.deltaPos.x - this->render.position.x;
      const float rz = this->state.deltaPos.z - this->render.position.z;
      Get_Angle(rx, rz, &slot.rotStart);
      slot.rotStart += Random(60) - 30;

      if (slot.rotStart >= 360) {
        slot.rotStart -= 360;
      }

      if (slot.rotStart < 0) {
        slot.rotStart += 360;
      }
    } else if (slot.process <= 10) {
      slot.alphaB += 25;
    } else {
      slot.alphaB -= 3;

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
        slot.life = 0;
      }
    }

    slot.vecBNow.y += slot.height[2];
    slot.vecBNow.x += slot.height[1] * g_cosTable[slot.rotStart];
    slot.vecBNow.z += slot.height[1] * g_sinTable[slot.rotStart];
  }
}

// OFFSET: 0x00BC3C70
// FUNCTION: fn_19840
bool CEffectPrim::PrimSuperAngel() {
  const CRagEffect* master = this->state.master;
  this->render.position.x = master->render.position.x;
  this->render.position.y = master->render.position.y;
  this->render.position.z = master->render.position.z;

  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life == 1) {
    ++slot.process;
    slot.vecBNow.y -= 0.1f;
    slot.vecBPre = slot.vecBNow;
    this->state.deltaPos = slot.vecBPre;

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

  this->render.position.x = this->state.deltaPos.x;
  this->render.position.y = this->state.deltaPos.y;
  this->render.position.z = this->state.deltaPos.z;
  this->ShiftSegment();
  return true;
}

// OFFSET: 0x00BC3FA0
// FUNCTION: fn_19841
void CEffectPrim::PrimTanji2() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  const CRagEffect* master = this->state.master;
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

  if (slot.flag1[2] == 1) {
    if (slot.process <= 10) {
      slot.alphaB += 25;
    }

    if (slot.alphaB >= 255) {
      slot.alphaB = 255;
    }
  }

  const int rate = slot.flag1[2] == 1 ? 1 : 3;

  if (slot.process % rate == 0) {
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

  if (slot.process == 1) {
    const float rx = this->state.deltaPos.x - slot.vecBNow.x;
    const float rz = this->state.deltaPos.z - slot.vecBNow.z;
    Get_Angle(rx, rz, &slot.rotStart);
  }

  slot.vecBNow.x += slot.height[1] * g_cosTable[slot.rotStart];
  slot.vecBNow.z += slot.height[1] * g_sinTable[slot.rotStart];

  if (slot.process % 4 == 0) {
    CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
    CRagEffect* effect = gameMode->scene->player->LaunchEffect(416, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);

    if (effect) {
      vector3d position = slot.vecBNow;
      effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00BBE350
// FUNCTION: fn_19811
bool CEffectPrim::PrimGhost() {
  this->render.position = this->state.master->render.position;

  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life == 1) {
    ++slot.process;

    if (slot.process <= 250) {
      if (slot.height[1] > this->state.alpha) {
        this->state.alpha += 1.0f;
      }
    }

    if (slot.process >= 2600) {
      int a = 360;
      a /= slot.flag1[0];

      if (slot.flag1[2] <= 1) {
        if (slot.process % a == 0) {
          ++slot.flag1[0];
        }

        slot.distance *= 1.002f;
      }

      if (slot.process % 15 == 0) {
        if (slot.flag1[2] == 2) {
          this->state.alpha -= 5.0f;
        } else {
          this->state.alpha -= 1.0f;
        }

        if (this->state.alpha < 0.0f) {
          this->state.alpha = 0.0f;
        }
      }
    }

    slot.riseAngle += 3;

    if (slot.riseAngle >= 360) {
      slot.riseAngle -= 360;
    }

    slot.rotStart += slot.flag1[0];

    if (slot.rotStart >= 360) {
      slot.rotStart -= 360;
    }

    slot.vecBPre = slot.vecBNow;

    if (slot.flag1[2] == 2) {
      int angle = slot.rotStart * 2;

      if (angle >= 360) {
        angle -= 360;
      }

      slot.vecBPre.x += slot.distance * g_cosTable[slot.rotStart];
      slot.vecBPre.z += slot.distance * g_sinTable[angle];
    } else {
      slot.vecBPre.x += slot.distance * g_cosTable[slot.rotStart];
      slot.vecBPre.z += slot.distance * g_sinTable[slot.rotStart];
    }

    slot.vecBPre.y += slot.maxHeight * g_sinTable[slot.riseAngle];
    this->state.deltaPos = slot.vecBPre;

    const CView* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
    int angle = slot.rotStart + static_cast<int>(view->camera.yaw);

    if (angle < 0) {
      angle += 360;
    }

    if (angle >= 360) {
      angle -= 360;
    }

    if (slot.flag1[2] == 0) {
      if (angle >= 180 && angle < 270) {
        this->render.previousAnimation = 6;
      } else if (angle >= 90 && angle < 180) {
        this->render.previousAnimation = 0;
      } else if (angle >= 0 && angle < 90) {
        this->render.previousAnimation = 0;
      } else {
        this->render.previousAnimation = 6;
      }
    } else {
      if (Random(999) == 1) {
        this->render.previousAnimation = 1;
      }

      if (Random(1001) == 0) {
        this->render.previousAnimation = 0;
      }
    }
  }

  this->render.position = this->state.deltaPos;
  this->state.ProcessAlpha();
  this->ShiftSegment();
  return this->AnimMotion();
}

// OFFSET: 0x00BC4730
// FUNCTION: fn_19844
void CEffectPrim::PrimThrowItem() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  const CRagEffect* master = this->state.master;
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
    const float x = this->state.deltaPos.x - slot.vecBNow.x;
    const float z = this->state.deltaPos.z - slot.vecBNow.z;
    Get_Angle(x, z, &slot.rotStart);
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
bool CEffectPrim::PrimPeong() {
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
void CEffectPrim::PrimGI3() {
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
void CEffectPrim::PrimWaterFall() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life > 0) {
      ++slot.process;
    }
  }
}

// OFFSET: 0x00BB6F30
// FUNCTION: fn_19779
bool CEffectPrim::Prim3DParticle() {
  const CRagEffect* master = this->state.master;
  this->state.orgPos.x = master->render.position.x + this->state.deltaPos2.x;
  this->state.orgPos.y = master->render.position.y + this->state.deltaPos2.y;
  this->state.orgPos.z = master->render.position.z + this->state.deltaPos2.z;

  const int frame = this->state.stateCnt;

  if (frame == this->state.changePoint) {
    this->state.speed3d = this->state.changeSpeed3d;
    this->state.deltaPosAccel = this->state.changeAccel3d;
  }

  float vertical = this->state.accel + this->state.speed;
  float lateral = this->state.sizeAccel + this->state.sizeSpeed;
  std::uint32_t flags = this->state.pattern;
  this->state.speed = vertical;
  this->state.sizeSpeed = lateral;

  if ((flags & 0x400u) != 0) {
    const float z = this->state.deltaPosAccel.z + this->state.speed3d.z;
    const float x = this->state.deltaPosAccel.x + this->state.speed3d.x;
    const float y = this->state.deltaPosAccel.y + this->state.speed3d.y;
    this->state.speed3d.z = z;
    this->state.deltaPos.z = z + this->state.deltaPos.z;
    this->state.speed3d.x = x;
    this->state.deltaPos.x = x + this->state.deltaPos.x;
    this->state.speed3d.y = y;
    this->state.deltaPos.y = y + this->state.deltaPos.y;
    this->state.roll = this->state.rollSpeed + this->state.roll;
    this->state.size = lateral + this->state.size;
  } else if ((flags & 1u) != 0 || frame > this->state.fadeInCnt) {
    const float side = lateral + this->state.size;
    this->state.speed3d = Forward(vertical, this->state.transform);
    this->state.deltaPos.x = this->state.speed3d.x + this->state.deltaPos.x;
    this->state.deltaPos.y = this->state.speed3d.y + this->state.deltaPos.y;
    this->state.deltaPos.z = this->state.speed3d.z + this->state.deltaPos.z;
    this->state.size = side;
    this->state.roll = this->state.rollSpeed + this->state.roll;
  }

  flags = this->state.pattern;

  if ((flags & 2u) != 0) {
    if (this->state.stateCnt % this->state.chCnt == 0) {
      this->state.chCnt = rand() % 8 + 12;
      this->state.deltaPos3.x = static_cast<float>(rand() % 5 + 5) / 100.0f;
      this->state.deltaPos3.z = static_cast<float>(rand() % 5 + 5) / 100.0f;

      if (rand() % 2 == 0) {
        this->state.deltaPos3.x = -this->state.deltaPos3.x;
      }

      if (rand() % 2 == 0) {
        this->state.deltaPos3.z = -this->state.deltaPos3.z;
      }
    }

    flags = this->state.pattern;
    this->state.deltaPos.x = this->state.deltaPos3.x + this->state.deltaPos.x;
  }

  if ((flags & 4u) != 0 && this->state.stateCnt % this->state.chCnt == 0) {
    this->state.speed = static_cast<float>(rand() % 35 + 15) / 100.0f;
    this->state.longitude = static_cast<float>(rand() % 80) + (this->state.longitude - 40.0f);
    this->state.latitude = static_cast<float>(rand() % 80) + (this->state.latitude - 40.0f);
    reinterpret_cast<struct matrix*>(this->state.transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.longitude)));
    reinterpret_cast<struct matrix*>(this->state.transform)->AppendXRotation(GetRadian(static_cast<int>(this->state.latitude)));
    flags = this->state.pattern;
    this->state.chCnt = rand() % 10 + 5;
  }

  if ((flags & 8u) != 0 && this->state.stateCnt % this->state.chCnt == 0) {
    this->state.longitude = static_cast<float>(rand() % 10) + (this->state.longitude - 5.0f);
    this->state.latitude = static_cast<float>(rand() % 10) + (this->state.latitude - 5.0f);
    reinterpret_cast<struct matrix*>(this->state.transform)->MakeXRotation(GetRadian(static_cast<int>(this->state.latitude)));
    reinterpret_cast<struct matrix*>(this->state.transform)->AppendYRotation(GetRadian(static_cast<int>(this->state.longitude)));
    this->state.chCnt = rand() % 15 + 5;
  }

  this->render.position = {
    this->state.deltaPos.x + this->state.orgPos.x, this->state.deltaPos.y + this->state.orgPos.y,
    this->state.deltaPos.z + this->state.orgPos.z
  };

  if ((this->state.pattern & 0x100u) != 0) {
    if (3.0f >= this->CalcDist(this->state.master->state.deltaPos[0], this->state.master->state.deltaPos[2])) {
      return false;
    }
  }

  if ((this->state.pattern & 0x20u) != 0) {
    this->state.roll = this->render.angle - static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;
  }

  this->state.ProcessAlpha();
  this->ShiftSegment();
  return this->AnimMotion();
}

// OFFSET: 0x00BB75B0
// FUNCTION: fn_19780
bool CEffectPrim::Prim3DParticleGravity() {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  const CRagEffect* master = this->state.master;
  this->state.orgPos.z = master->render.position.z + this->state.deltaPos2.z;
  this->state.orgPos.y = master->render.position.y + this->state.deltaPos2.y;
  this->state.orgPos.x = master->render.position.x + this->state.deltaPos2.x;
  this->state.gravSpeed = this->state.gravAccel + this->state.gravSpeed;

  const float speed = this->state.accel + this->state.speed;
  this->state.speed = speed < 0.0f ? 0.0f : speed;
  this->state.speed3d = Forward(this->state.speed, this->state.transform);

  this->state.deltaPos.x = this->state.speed3d.x + this->state.deltaPos.x;
  this->state.deltaPos.y = this->state.deltaPos.y + this->state.gravSpeed;
  this->state.deltaPos.z = this->state.speed3d.z + this->state.deltaPos.z;

  this->render.position.x = this->state.deltaPos.x + this->state.orgPos.x;
  this->render.position.y = this->state.deltaPos.y + this->state.orgPos.y;
  this->render.position.z = this->state.deltaPos.z + this->state.orgPos.z;
  this->state.size = this->state.sizeSpeed + this->state.size;

  if (this->state.gravSpeed > 0.0f && this->render.position.y > mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z)) {
    this->state.gravSpeed = this->state.gravSpeed / -1.5f;

    if (this->state.playWaveType == 1) {
      char name[52];
      std::sprintf(name, "effect\\EF_Coin%d.wav", rand() % 3 + 1);
      const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
      g_soundMgr->PlayWave(
        name, this->render.position.x - player->render.position.x, 0.0f, this->render.position.z - player->render.position.z, 250, 40, 1.0f, 0
      );
    }
  }

  this->state.ProcessAlpha();
  this->ShiftSegment();
  return this->AnimMotion();
}

// OFFSET: 0x00BB9610
// FUNCTION: fn_19786
bool CEffectPrim::Prim3DTextureNoMaster() {
  EffectPrimitiveState& particle = this->state;
  const vector3d origin{
    particle.orgPos.x + particle.deltaPos2.x, particle.orgPos.y + particle.deltaPos2.y, particle.orgPos.z + particle.deltaPos2.z
  };

  if (particle.changePoint == particle.stateCnt) {
    constexpr float sentinel = 0.0001f;

    if (particle.changeWidthSpeed != sentinel) {
      particle.widthSpeed = particle.changeWidthSpeed;
    }

    if (particle.changeHeightSpeed != sentinel) {
      particle.heightSpeed = particle.changeHeightSpeed;
    }

    if (particle.changeSpeed != sentinel) {
      particle.speed = particle.changeSpeed;
    }

    if (particle.changeWidthAccel != sentinel) {
      particle.widthAccel = particle.changeWidthAccel;
    }

    if (particle.changeHeightAccel != sentinel) {
      particle.heightAccel = particle.changeHeightAccel;
    }

    if (particle.changeAccel != sentinel) {
      particle.accel = particle.changeAccel;
    }

    if (particle.changeSpeed3d.x != sentinel) {
      particle.speed3d.x = particle.changeSpeed3d.x;
    }

    if (particle.changeSpeed3d.y != sentinel) {
      particle.speed3d.y = particle.changeSpeed3d.y;
    }

    if (particle.changeSpeed3d.z != sentinel) {
      particle.speed3d.z = particle.changeSpeed3d.z;
    }

    if (particle.changeAccel3d.x != sentinel) {
      particle.deltaPosAccel.x = particle.changeAccel3d.x;
    }

    if (particle.changeAccel3d.y != sentinel) {
      particle.deltaPosAccel.y = particle.changeAccel3d.y;
    }

    if (particle.changeAccel3d.z != sentinel) {
      particle.deltaPosAccel.z = particle.changeAccel3d.z;
    }
  }

  particle.speed3d.x = particle.deltaPosAccel.x + particle.speed3d.x;
  particle.speed3d.y = particle.deltaPosAccel.y + particle.speed3d.y;
  particle.speed3d.z = particle.deltaPosAccel.z + particle.speed3d.z;
  particle.speed = particle.accel + particle.speed;
  particle.widthSpeed = particle.widthAccel + particle.widthSpeed;
  particle.heightSpeed = particle.heightSpeed + particle.heightAccel;
  particle.longSpeed = particle.longAccel + particle.longSpeed;
  particle.latiSpeed = particle.latiAccel + particle.latiSpeed;

  if ((particle.pattern & 0x400u) != 0) {
    particle.speed3d = {};
    particle.speed3d = Forward(particle.speed, particle.transform);
    particle.deltaPos.x = particle.speed3d.x + particle.deltaPos.x;
    particle.deltaPos.y = particle.speed3d.y + particle.deltaPos.y;
    particle.deltaPos.z = particle.speed3d.z + particle.deltaPos.z;
  } else if ((particle.pattern & 0x100u) != 0) {
    particle.deltaPos.x = particle.speed3d.x + particle.deltaPos.x;
    particle.deltaPos.y = particle.speed3d.y + particle.deltaPos.y;
    particle.deltaPos.z = particle.speed3d.z + particle.deltaPos.z;
  } else {
    particle.speed3d = Forward(particle.speed, particle.transform);
    particle.deltaPos.x = particle.speed3d.x + particle.deltaPos.x;
    particle.deltaPos.y = particle.speed3d.y + particle.deltaPos.y;
    particle.deltaPos.z = particle.speed3d.z + particle.deltaPos.z;
  }

  if ((particle.pattern & 0x400u) != 0) {
    if (3.0f >= this->CalcDist(particle.master->state.deltaPos[0], particle.master->state.deltaPos[2])) {
      particle.unknown_014C.x = std::bit_cast<float>(1u);
      return false;
    }

    particle.longitude = this->render.angle;
  }

  this->render.position.x = origin.x + particle.deltaPos.x;
  this->render.position.y = origin.y + particle.deltaPos.y;
  this->render.position.z = origin.z + particle.deltaPos.z;
  particle.widthSize = particle.widthSize + particle.widthSpeed;
  particle.heightSize = particle.heightSize + particle.heightSpeed;
  particle.longitude = particle.longSpeed + particle.longitude;
  particle.latitude = particle.latiSpeed + particle.latitude;

  if (!(particle.heightSize <= particle.maxHeightSize)) {
    particle.heightAccel = 0;
    particle.heightSpeed = 0;
  }

  auto& rotation = *reinterpret_cast<struct matrix*>(particle.rotTransform);
  float angle;

  if ((particle.pattern & 0x200u) != 0) {
    const CView* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
    rotation.MakeZRotation(view->camera.pitch);
    angle = this->render.angle;
  } else {
    if ((particle.pattern & 0x40u) != 0) {
      const CView* view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera;
      particle.longitude = view->camera.yaw + 90.0f;
      particle.roll = view->camera.pitch;
      rotation.MakeXRotation(GetRadian(static_cast<int>(particle.latitude)));
      rotation.AppendZRotation(GetRadian(static_cast<int>(particle.roll)));
    } else {
      rotation.MakeZRotation(GetRadian(static_cast<int>(particle.roll)));
      rotation.AppendXRotation(GetRadian(static_cast<int>(particle.latitude)));
    }

    angle = particle.longitude;
  }

  rotation.AppendYRotation(GetRadian(static_cast<int>(angle)));

  if ((particle.pattern & 2u) != 0) {
    const auto index = particle.chCnt;
    particle.chCnt = index + 1u == 10u ? 0u : index + 1u;
    const float offset = static_cast<float>(static_cast<int>(index)) / 10.0f;
    particle.v1 = offset;
    particle.v2 = offset + 1.0f;
  }

  if ((particle.pattern & 0x10u) != 0) {
    particle.master->state.deltaPos[0] = particle.deltaPos2.x + particle.deltaPos.x;
    particle.master->state.deltaPos[1] = particle.deltaPos.y + particle.deltaPos2.y;
    particle.master->state.deltaPos[2] = particle.deltaPos.z + particle.deltaPos2.z;
  }

  if ((particle.pattern & 4u) != 0) {
    CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());

    if (this->render.position.y > mode->scene->terrain->Grid().Height(this->render.position.x, this->render.position.z)) {
      particle.pattern &= ~4u;
      particle.stateCnt = particle.duration - 25;
      particle.speed = 0;
      particle.master->state.flag = 1;
    }
  }

  if ((particle.pattern & 0x20u) != 0 && particle.stateCnt % static_cast<int>(particle.type) == 0) {
    particle.heightSpeed *= -1.0f;
  }

  const bool wrap254 = (particle.pattern & 0x800u) != 0;
  const bool gated = wrap254 && (particle.red == 254 || particle.blue == 254 || particle.green == 254);
  bool advance = true;

  if (gated) {
    if (particle.RGBCycleDelay != 0) {
      ++particle.RGBCycleCnt;
    }

    advance = particle.RGBCycleDelay == particle.RGBCycleCnt;
  }

  if (advance) {
    particle.red += particle.redSpeed;
    particle.green += particle.greenSpeed;
    particle.blue += particle.blueSpeed;

    if (gated) {
      particle.RGBCycleCnt = 0;
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

  particle.ProcessAlpha();

  if (particle.stateCnt % particle.animSpeed == 0) {
    const auto next = this->render.curMotion + 1u;
    this->render.curMotion = next;

    if (particle.repeatAnim != 0) {
      this->render.curMotion = static_cast<int>(next) % static_cast<int>(particle.totalTexture);
    } else if (next == particle.totalTexture) {
      return false;
    }
  }

  return true;
}

// OFFSET: 0x00BB78A0
// FUNCTION: fn_19781
bool CEffectPrim::Prim3DParticleNoMaster() {
  float positionX = this->state.orgPos.x + this->state.deltaPos2.x;
  const float positionY = this->state.orgPos.y + this->state.deltaPos2.y;
  const float positionZ = this->state.orgPos.z + this->state.deltaPos2.z;
  const int frame = this->state.stateCnt;

  if (frame == this->state.changePoint) {
    this->state.speed3d = this->state.changeSpeed3d;
    this->state.deltaPosAccel = this->state.changeAccel3d;
  }

  float vertical = this->state.accel + this->state.speed;
  float lateral = this->state.sizeAccel + this->state.sizeSpeed;
  std::uint32_t flags = this->state.pattern;
  this->state.speed = vertical;
  this->state.sizeSpeed = lateral;

  if ((flags & 0x400u) != 0) {
    const float z = this->state.deltaPosAccel.z + this->state.speed3d.z;
    const float x = this->state.deltaPosAccel.x + this->state.speed3d.x;
    const float y = this->state.deltaPosAccel.y + this->state.speed3d.y;
    this->state.speed3d.z = z;
    this->state.deltaPos.z = z + this->state.deltaPos.z;
    this->state.speed3d.x = x;
    this->state.deltaPos.x = x + this->state.deltaPos.x;
    this->state.speed3d.y = y;
    this->state.deltaPos.y = y + this->state.deltaPos.y;
    this->state.roll = this->state.rollSpeed + this->state.roll;
    this->state.size = lateral + this->state.size;
  } else if ((flags & 1u) != 0 || frame > this->state.fadeInCnt) {
    const float side = lateral + this->state.size;
    this->state.speed3d = Forward(vertical, this->state.transform);
    this->state.deltaPos.x = this->state.speed3d.x + this->state.deltaPos.x;
    this->state.deltaPos.y = this->state.speed3d.y + this->state.deltaPos.y;
    this->state.deltaPos.z = this->state.speed3d.z + this->state.deltaPos.z;
    this->state.size = side;
    this->state.roll = this->state.rollSpeed + this->state.roll;
  }

  flags = this->state.pattern;

  if ((flags & 2u) != 0) {
    if (this->state.stateCnt % this->state.chCnt == 0) {
      this->state.chCnt = rand() % 8 + 12;
      this->state.deltaPos3.x = static_cast<float>(rand() % 5 + 5) / 100.0f;
      this->state.deltaPos3.z = static_cast<float>(rand() % 5 + 5) / 100.0f;

      if (rand() % 2 == 0) {
        this->state.deltaPos3.x = -this->state.deltaPos3.x;
      }

      if (rand() % 2 == 0) {
        this->state.deltaPos3.z = -this->state.deltaPos3.z;
      }
    }

    flags = this->state.pattern;
    this->state.deltaPos.x = this->state.deltaPos3.x + this->state.deltaPos.x;
  }

  if ((flags & 4u) != 0 && this->state.stateCnt % this->state.chCnt == 0) {
    this->state.speed = static_cast<float>(rand() % 35 + 15) / 100.0f;
    this->state.longitude = static_cast<float>(rand() % 80) + (this->state.longitude - 40.0f);
    this->state.latitude = static_cast<float>(rand() % 80) + (this->state.latitude - 40.0f);
    reinterpret_cast<struct matrix*>(this->state.transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.longitude)));
    reinterpret_cast<struct matrix*>(this->state.transform)->AppendXRotation(GetRadian(static_cast<int>(this->state.latitude)));
    this->state.chCnt = rand() % 10 + 5;
    flags = this->state.pattern;
  }

  if ((flags & 8u) != 0 && this->state.stateCnt % this->state.chCnt == 0) {
    this->state.longitude = static_cast<float>(rand() % 10) + (this->state.longitude - 5.0f);
    this->state.latitude = static_cast<float>(rand() % 10) + (this->state.latitude - 5.0f);
    reinterpret_cast<struct matrix*>(this->state.transform)->MakeXRotation(GetRadian(static_cast<int>(this->state.latitude)));
    reinterpret_cast<struct matrix*>(this->state.transform)->AppendYRotation(GetRadian(static_cast<int>(this->state.longitude)));
    this->state.chCnt = rand() % 15 + 5;
    flags = this->state.pattern;
  }

  this->render.position.x = positionX + this->state.deltaPos.x;
  this->render.position.y = positionY + this->state.deltaPos.y;
  this->render.position.z = positionZ + this->state.deltaPos.z;

  if ((flags & 0x20u) != 0) {
    this->state.roll = this->render.angle - static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;
  }

  this->state.ProcessAlpha();
  this->ShiftSegment();
  return this->AnimMotion();
}

// OFFSET: 0x00BB7EC0
// FUNCTION: fn_19782
bool CEffectPrim::Prim3DParticleOrbit() {
  const CRagEffect* master = this->state.master;
  this->state.orgPos = {master->render.position.x, master->render.position.y, master->render.position.z};

  const float lateral = this->state.longAccel + this->state.longSpeed;
  float vertical = this->state.gravSpeed + this->state.gravAccel;
  const float spread = this->state.radiusSpeed + this->state.radiusAccel;
  this->state.longSpeed = lateral;
  const float angle = lateral + this->state.longitude;
  this->state.gravSpeed = vertical;
  vertical = vertical + this->state.deltaPos2.y;
  const float side = spread + this->state.radius;
  this->state.longitude = angle;
  this->state.deltaPos2.y = vertical;
  this->state.radius = side;

  const float moveX = this->state.orgPos.x + this->state.deltaPos2.x;
  this->state.orgPos.y = vertical + this->state.orgPos.y;
  this->state.orgPos.x = moveX;
  this->state.orgPos.z = this->state.deltaPos2.z + this->state.orgPos.z;
  const int frame = this->state.stateCnt;
  this->state.roll = this->state.rollSpeed + this->state.roll;

  if (frame == this->state.changePoint) {
    this->state.gravSpeed = this->state.changeSpeed;
    this->state.gravAccel = this->state.changeAccel;
  }

  if (this->state.chRadiSpeed != 0) {
    int index = this->state.unknown_01D8;
    const std::uint32_t count = static_cast<std::uint32_t>(this->state.chPoint.size());

    if (index < 0 || static_cast<std::uint32_t>(index) >= count) {
      index = 0;
      this->state.unknown_01D8 = 0;
    }

    if (static_cast<float>(frame) == this->state.chPoint[index]) {
      this->state.radiusSpeed = this->state.chVal1[index];
      this->state.radiusAccel = this->state.chVal2[index];
      this->state.unknown_01D8 = index + 1;
    }
  }

  reinterpret_cast<struct matrix*>(this->state.transform)->MakeYRotation(GetRadian(static_cast<int>(this->state.longitude)));

  const float distance = this->state.radius;
  this->state.deltaPos = Forward(distance, this->state.transform);
  this->render.position.x = this->state.deltaPos.x + this->state.orgPos.x;
  this->render.position.y = this->state.deltaPos.y + this->state.orgPos.y;
  this->render.position.z = this->state.deltaPos.z + this->state.orgPos.z;

  this->state.size = this->state.sizeSpeed + this->state.size;
  const float rotation = this->state.latiSpeed + this->state.latitude;
  this->state.latitude = rotation;
  reinterpret_cast<struct matrix*>(this->state.transform)->AppendZRotation(rotation);

  this->state.ProcessAlpha();
  this->ShiftSegment();
  return this->AnimMotion();
}

// OFFSET: 0x00BB8180
// FUNCTION: fn_19783
bool CEffectPrim::Prim3DParticleSpline() {
  const CRagEffect* master = this->state.master;
  this->state.orgPos = {
    master->render.position.x + this->state.deltaPos2.x, master->render.position.y + this->state.deltaPos2.y,
    master->render.position.z + this->state.deltaPos2.z
  };

  const int angle = static_cast<int>(this->state.roll);
  const float lateral = this->state.latiSpeed;
  this->state.latiSpeed = lateral + this->state.latiAccel;
  this->state.latitude = lateral + this->state.latitude;
  this->state.speed = this->state.accel + this->state.speed;
  this->state.gravSpeed = this->state.gravAccel + this->state.gravSpeed;

  const float distance = this->state.speed - this->state.gravSpeed;
  this->state.speed3d = Forward(distance, this->state.transform);
  reinterpret_cast<struct matrix*>(this->state.rotTransform)->MakeTransform(this->state.rotAxis, GetRadian(angle));

  const float move = this->state.latitude;
  this->state.deltaPos3 = Lift(move, this->state.rotTransform);
  this->state.size = this->state.sizeSpeed + this->state.size;

  this->state.deltaPos.x = this->state.deltaPos.x - this->state.speed3d.x;
  this->state.deltaPos.y = this->state.deltaPos.y - this->state.speed3d.y;
  this->state.deltaPos.z = this->state.deltaPos.z - this->state.speed3d.z;

  this->render.position.x = this->state.deltaPos.x + this->state.orgPos.x + this->state.deltaPos3.x;
  this->render.position.y = this->state.deltaPos.y + this->state.orgPos.y + this->state.deltaPos3.y;
  this->render.position.z = this->state.deltaPos.z + this->state.orgPos.z + this->state.deltaPos3.z;

  this->state.ProcessAlpha();
  this->ShiftSegment();
  return this->AnimMotion();
}

// OFFSET: 0x00BE6FE0
// FUNCTION: fn_19931
void CEffectPrim::ShiftSegment() {
  const int count = this->state.numSegment;

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
    record.scale = this->state.size - this->state.size / static_cast<float>(count) * static_cast<float>(index) * 0.5f;
  }

  EffectPrimitiveRecord& first = this->state.records[0];
  first.position.x = this->render.position.x;
  first.position.y = this->render.position.y;
  first.position.z = this->render.position.z;

  for (int corner = 0; corner < 4; ++corner) {
    first.corners[corner] = this->state.segPos[corner];
  }

  first.unknown_003C = this->state.radius;
  first.scale = this->state.size;
  first.angle = this->state.longitude;
  std::memcpy(first.matrix, this->state.transform, sizeof(first.matrix));
  first.alpha = this->state.alpha;

  const std::uint32_t red = static_cast<unsigned char>(this->state.red);
  const std::uint32_t green = static_cast<unsigned char>(this->state.green);
  const std::uint32_t blue = static_cast<unsigned char>(this->state.blue);
  first.color = (static_cast<std::uint32_t>(static_cast<int>(this->state.alpha)) << 24) | (red << 16) | (green << 8) | blue;
}

// INLINED
static void FollowMaster(CEffectPrim& particle) {
  const CRagEffect* master = particle.state.master;
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
bool CEffectPrim::FollowMasterAndAdvance() {
  FollowMaster(*this);
  return this->AnimMotion();
}

// OFFSET: 0x00BE9560
// FUNCTION: fn_19952
void CEffectPrim::FollowMasterAndTickSlots() {
  FollowMaster(*this);
  TickLiveSlots(this->state.slots);
}

// OFFSET: 0x00BE8770
// FUNCTION: fn_19944
void CEffectPrim::FollowMasterAndTickSlots2() {
  FollowMaster(*this);
  TickLiveSlots(this->state.slots);
}

// INLINED
static __forceinline void ShrinkLast(EffectPrimitiveSlot& slot, int value, float growth) {
  const float height = slot.height[value] - 5.0f;
  slot.height[value] = height;

  if (height > 0.0f) {
    slot.distance *= growth;
  }

  if (height < 0.0f) {
    slot.height[value] = 0.0f;
  }
}

// INLINED
static __forceinline void WanderAngle(float& angle, float& target) {
  if (target > angle) {
    angle += 3.0f;

    if (angle > 359.0f) {
      angle = 359.0f;
    }

    if (angle > target) {
      const int range = static_cast<int>(angle);
      int pick = 0;

      if (range != 0) {
        const int roll = std::rand() % range;
        pick = roll < 0 ? 0 : roll;
      }

      target = static_cast<float>(pick);
    }
  } else {
    angle -= 2.0f;

    if (angle < 0.0f) {
      angle = 0.0f;
    }

    if (angle <= target) {
      const int range = static_cast<int>(angle);
      int pick = 0;

      if (range != 0) {
        const int roll = std::rand() % range;
        pick = roll < 0 ? 0 : roll;
      }

      target = static_cast<float>(359 - pick);
    }
  }
}

// INLINED
static __forceinline void WanderAngleSlow(float& angle, float& target) {
  if (target > angle) {
    angle += 1.0f;

    if (angle > 359.0f) {
      angle = 359.0f;
    }

    if (angle > target) {
      const int range = static_cast<int>(angle);
      int pick = 0;

      if (range != 0) {
        const int roll = std::rand() % range;
        pick = roll < 0 ? 0 : roll;
      }

      target = static_cast<float>(pick);
    }
  } else {
    angle -= 1.0f;

    if (angle < 0.0f) {
      angle = 0.0f;
    }

    if (angle <= target) {
      const int range = static_cast<int>(angle);
      int pick = 0;

      if (range != 0) {
        const int roll = std::rand() % range;
        pick = roll < 0 ? 0 : roll;
      }

      target = static_cast<float>(359 - pick);
    }
  }
}

// INLINED
static __forceinline void SpinDown(float& angle) {
  angle -= 1.0f;

  if (angle < 0.0f) {
    angle += 360.0f;
  }
}

// INLINED
static __forceinline void LaunchVector(vector3d& point, float speed, float pitch, float yaw) {
  point.y -= speed * g_sinTable[static_cast<int>(pitch)];
  const float flat = speed * g_cosTable[static_cast<int>(pitch)];
  point.x += flat * g_sinTable[static_cast<int>(yaw)];
  point.z += flat * g_cosTable[static_cast<int>(yaw)];
}

// INLINED
static __forceinline void WaveSlotHeights(EffectPrimitiveSlot& slot, int index, float amplitude) {
  for (int value = 0; value < 21; ++value) {
    const int phase = index * 45 < 90 ? slot.process + index * 90 : (slot.process + index * 45) * 2;
    slot.height[value] = slot.maxHeight;
    slot.height[value] = g_sinTable[value * 9] * slot.maxHeight * amplitude * g_sinTable[phase % 360] + slot.maxHeight;
  }
}

// OFFSET: 0x00BB9FA0
// FUNCTION: fn_19787
bool CEffectPrim::PrimAcidDemon() {
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;
    const int roll = std::rand();

    if (index == 3) {
      const int step = roll % 2;
      slot.rotStart = static_cast<std::int16_t>((step < 0 ? 0 : step) + 2 + slot.rotStart);
    } else {
      const int step = roll % 3;
      slot.rotStart = static_cast<std::int16_t>((step < 0 ? 0 : step) + 4 + slot.rotStart);
    }

    if (slot.rotStart >= 360) {
      slot.rotStart -= 360;
    }

    if (slot.process >= 40) {
      slot.alphaB -= 15;

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    } else if (slot.process < 20) {
      slot.alphaB += 5;

      if (slot.alphaB > 80) {
        slot.alphaB = 80;
      }
    }

    slot.maxHeight *= index == 3 ? 1.025f : 1.005f;

    for (int value = 0; value < 21; ++value) {
      if (slot.process <= 90) {
        slot.height[value] = g_sinTable[value * 9] * slot.maxHeight * g_sinTable[slot.process];
      } else {
        slot.flag1[value] = 1;
      }

      if (slot.height[value] > g_sinTable[value * 9] * slot.maxHeight) {
        slot.flag1[value] = 1;
      }

      if (slot.height[value] < 0.0f) {
        slot.height[value] = 0.0f;
      }
    }
  }

  return true;
}

// OFFSET: 0x00BBA1D0
// FUNCTION: fn_19788
bool CEffectPrim::PrimAsuraCasting() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  if (slots[0].flag1[4] == 1) {
    FollowMaster(*this);
  }

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = slots[index];

    if (slot.life != 1) {
      continue;
    }

    if (++slot.process <= 0) {
      continue;
    }

    if (slot.flag1[4] == 2) {
      if (slot.process < 20) {
        slot.height[1] += 13.0f;

        if (slot.height[1] >= 33.0f) {
          slot.height[1] = 33.0f;
        }

        slot.height[2] += 13.0f;

        if (slot.height[2] >= 56.0f) {
          slot.height[2] = 56.0f;
        }

        slot.height[3] += 13.0f;

        if (slot.height[3] >= 79.0f) {
          slot.height[3] = 79.0f;
        }

        slot.height[4] += 13.0f;

        if (slot.height[4] >= 102.0f) {
          slot.height[4] = 102.0f;
        }

        slot.height[5] += 13.0f;

        if (slot.height[5] >= 125.0f) {
          slot.height[5] = 125.0f;
        }

        slot.height[6] += 13.0f;

        if (slot.height[6] >= 148.0f) {
          slot.height[6] = 148.0f;
        }

        slot.height[7] += 13.0f;

        if (slot.height[7] >= 171.0f) {
          slot.height[7] = 171.0f;
        }

        slot.height[8] += 13.0f;

        if (slot.height[8] >= 194.0f) {
          slot.height[8] = 194.0f;
        }

        slot.height[9] += 13.0f;

        if (slot.height[9] >= 217.0f) {
          slot.height[9] = 217.0f;
        }

        slot.height[10] += 13.0f;

        if (slot.height[10] >= 240.0f) {
          slot.height[10] = 240.0f;
        }
      } else {
        for (int value = 1; value < 9; ++value) {
          slot.height[value] -= 2.0f;

          if (slot.height[value] < 0.0f) {
            slot.height[value] = 0.0f;
          }
        }

        if (slot.process > 100) {
          ShrinkLast(slot, 9, 1.005f);
          ShrinkLast(slot, 10, 1.005f);
        }
      }

      continue;
    }

    if (slot.process < 20) {
        slot.height[1] += 13.0f;

        if (slot.height[1] >= 70.0f) {
          slot.height[1] = 70.0f;
        }

        slot.height[2] += 13.0f;

        if (slot.height[2] >= 90.0f) {
          slot.height[2] = 90.0f;
        }

        slot.height[3] += 13.0f;

        if (slot.height[3] >= 110.0f) {
          slot.height[3] = 110.0f;
        }

        slot.height[4] += 13.0f;

        if (slot.height[4] >= 130.0f) {
          slot.height[4] = 130.0f;
        }

        slot.height[5] += 13.0f;

        if (slot.height[5] >= 150.0f) {
          slot.height[5] = 150.0f;
        }

        slot.height[6] += 13.0f;

        if (slot.height[6] >= 170.0f) {
          slot.height[6] = 170.0f;
        }

        slot.height[7] += 13.0f;

        if (slot.height[7] >= 190.0f) {
          slot.height[7] = 190.0f;
        }

        slot.height[8] += 13.0f;

        if (slot.height[8] >= 210.0f) {
          slot.height[8] = 210.0f;
        }

        slot.height[9] += 13.0f;

        if (slot.height[9] >= 230.0f) {
          slot.height[9] = 230.0f;
        }

        slot.height[10] += 13.0f;

        if (slot.height[10] >= 250.0f) {
          slot.height[10] = 250.0f;
        }
    } else {
      for (int value = 1; value < 10; ++value) {
        slot.height[value] -= 5.0f;

        if (slot.height[value] < 0.0f) {
          slot.height[value] = 0.0f;
        }
      }

      if (slot.process > (slot.flag1[4] == 4 ? 25 : 140)) {
        ShrinkLast(slot, 10, 1.01f);
      }
    }

    if (slot.process < 50 && slot.flag1[4] != 4) {
      slot.distance -= 0.1f;
    }
  }

  return true;
}

// OFFSET: 0x00BBD020
// FUNCTION: fn_19803
void CEffectPrim::PrimElectric2() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    if (++slot.process <= 0) {
      continue;
    }

    slot.distance += 0.6f;

    if (slot.process <= 10) {
      const int boost = std::rand() % 6;
      slot.alphaB += static_cast<std::int16_t>((boost < 0 ? 0 : boost) + 15);
    }

    if (slot.process > 20) {
      slot.alphaB -= 5;
    }

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }

    if (slot.process == 1) {
      for (int step = 21; step != 0; --step) {
        slot.height[0] += 18.0f;

        if (slot.height[0] >= 360.0f) {
          slot.height[0] -= 360.0f;
          continue;
        }

        for (int value = static_cast<int>(slot.vecTNow.y); value >= 1; --value) {
          slot.height[value] = slot.height[value - 1];
        }
      }
    } else if (slot.vecTNow.y >= 0.0f) {
      for (int value = 0; static_cast<float>(value) <= slot.vecTNow.y; ++value) {
        slot.height[value] += 8.0f;

        if (slot.height[value] >= 360.0f) {
          slot.height[value] -= 360.0f;
        }

        if (slot.height[value] < 0.0f) {
          slot.height[value] += 360.0f;
        }
      }
    }
  }
}

// OFFSET: 0x00BBD560
// FUNCTION: fn_19806
bool CEffectPrim::PrimFlowerCast() {
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    const int process = ++slot.process;

    if (slot.rotStart >= 360) {
      slot.rotStart -= 360;
    }

    slot.maxHeight = g_sinTable[(process % 90) * 4] * 2.5f + slot.vecTNow.y;

    if (slot.riseAngle >= 360) {
      slot.riseAngle -= 360;
    }

    if (slot.riseAngle < 0) {
      slot.riseAngle += 360;
    }

    if (process > 110) {
      slot.alphaB -= 1;

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    } else if (process < 20) {
      slot.alphaB += 5;

      if (slot.alphaB > 180) {
        slot.alphaB = 180;
      }
    }

    for (int value = 0; value < 21; ++value) {
      if (slot.flag1[value] != 0) {
        continue;
      }

      float height = slot.maxHeight * g_sinTable[value * 9];

      if (slot.process <= 90) {
        height *= g_sinTable[slot.process];
      }

      slot.height[value] = height;

      const float limit = slot.maxHeight * g_sinTable[value * 9];

      if (height > limit) {
        slot.height[value] = limit;
        slot.flag1[value] = 1;
      }

      if (slot.height[value] < 0.0f) {
        slot.height[value] = 0.0f;
      }
    }
  }

  return true;
}

// OFFSET: 0x00BBD740
// FUNCTION: fn_19807
void CEffectPrim::PrimForest() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life <= 0) {
      continue;
    }

    const int process = ++slot.process;

    if (slot.rotStart >= 360) {
      slot.rotStart -= 360;
    }

    if (slot.flag1[0] == 13) {
      if (process <= 40) {
        ++slot.alphaB;
      }

      if (slot.alphaB <= 0) {
        slot.alphaB = 0;
      }
    } else if (slot.flag1[0] >= 11) {
      if (process <= 40) {
        ++slot.alphaB;
      } else if (process > 140) {
        --slot.alphaB;
      }

      if (slot.alphaB <= 0) {
        slot.alphaB = 0;
      }
    }

    if (index == 1 || index == 3) {
      slot.maxHeight = slot.height[0];
      slot.maxHeight = g_sinTable[(process % 720) / 2] * 0.5f + slot.height[0];
    }
  }
}

// OFFSET: 0x00BBFDD0
// FUNCTION: fn_19819
bool CEffectPrim::PrimMapParticle() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = slots[index];

    if (slot.life != 1) {
      continue;
    }

    for (int pair = 0; pair < 2; ++pair) {
      WanderAngleSlow(slot.height[pair * 2], slot.height[pair * 2 + 1]);
    }

    SpinDown(slot.height[4]);
    SpinDown(slot.height[5]);
    SpinDown(slot.height[6]);
    SpinDown(slot.height[7]);

    const int first = static_cast<int>(slot.height[4]);
    const float firstSin = g_sinTable[first] * 20.0f;
    const float firstCos = g_cosTable[first] * 20.0f;
    slot.vecBPre.x = firstSin;
    slot.vecBPre.z = firstCos;
    const int second = static_cast<int>(slot.height[5]);
    float secondSin = g_sinTable[second] * 20.0f;
    float secondCos = g_cosTable[second] * 20.0f;
    slot.vecBNow.x = secondSin;
    slot.vecBNow.z = secondCos;
    const int third = static_cast<int>(slot.height[6]);
    const float thirdSin = g_sinTable[third] * 20.0f;
    float thirdCos = g_cosTable[third] * 20.0f;
    slot.vecTPre.x = thirdSin;
    slot.vecTPre.z = thirdCos;
    const int fourth = static_cast<int>(slot.height[7]);
    float fourthSin = g_sinTable[fourth] * 20.0f;
    const float fourthCos = g_cosTable[fourth] * 20.0f;
    slot.vecTNow.x = fourthSin;
    slot.vecTNow.z = fourthCos;

    if (slot.vecBPre.y < 0.0f) {
      slot.vecBPre.x = g_sinTable[static_cast<int>(slot.height[0])] * 2.5f + firstSin;
      slot.vecBPre.z = g_sinTable[static_cast<int>(slot.height[2])] * 2.5f + firstCos;
    }

    if (slot.vecBNow.y < 0.0f) {
      secondSin -= g_sinTable[static_cast<int>(slot.height[0])] * 2.5f;
      slot.vecBNow.x = secondSin;
      secondCos -= g_sinTable[static_cast<int>(slot.height[2])] * 2.5f;
      slot.vecBNow.z = secondCos;
    }

    if (slot.vecTPre.y < 0.0f) {
      slot.vecTPre.x = g_sinTable[static_cast<int>(slot.height[0])] * 2.5f + thirdSin;
      thirdCos -= g_sinTable[static_cast<int>(slot.height[2])] * 2.5f;
      slot.vecTPre.z = thirdCos;
    }

    if (slot.vecTNow.y < 0.0f) {
      fourthSin -= g_sinTable[static_cast<int>(slot.height[0])] * 2.5f;
      slot.vecTNow.x = fourthSin;
      slot.vecTNow.z = g_sinTable[static_cast<int>(slot.height[2])] * 2.5f + fourthCos;
    }
  }

  return true;
}

// OFFSET: 0x00BC02F0
// FUNCTION: fn_19820
void CEffectPrim::PrimParticleSpread() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;
    const float speed = slot.maxHeight;
    LaunchVector(slot.vecBPre, speed, slot.height[2], slot.height[0]);
    LaunchVector(slot.vecBNow, speed, slot.height[6], slot.height[4]);
    LaunchVector(slot.vecTPre, speed, slot.height[10], slot.height[8]);

    if (slot.process <= 25) {
      continue;
    }

    slot.maxHeight = speed * 0.9f;

    for (int value = 12; value < 15; ++value) {
      slot.height[value] -= 6.0f;

      if (slot.height[value] < 0.0f) {
        slot.height[value] = 0.0f;
      }
    }
  }
}

// OFFSET: 0x00BC0540
// FUNCTION: fn_19821
void CEffectPrim::PrimParticleUp() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = slots[index];

    if (slot.life != 1) {
      continue;
    }

    const int process = ++slot.process;

    if (process <= 0) {
      continue;
    }

    slot.fullDisplayAngle -= 5;

    if (slot.fullDisplayAngle < 0) {
      slot.fullDisplayAngle += 360;
    }

    if (process == 1) {
      const int angle = std::rand() % 360;
      slot.rotStart = static_cast<std::int16_t>(angle < 0 ? 0 : angle);
    } else if (process <= 10) {
      slot.alphaB += 15;
    } else {
      if (slot.flag1[1] == 1) {
        const int flash = 36 - index * 7;
        slot.flag1[0] = process == flash - 1 || process == flash ? 1 : 0;
      }

      slot.alphaB -= 3;

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
        slot.life = 0;
      }
    }

    slot.vecBNow.y -= slot.height[2];
    slot.vecBNow.x += slot.height[1] * g_cosTable[slot.rotStart];
    slot.vecBNow.z += slot.height[1] * g_sinTable[slot.rotStart];
  }
}

// OFFSET: 0x00BC07F0
// FUNCTION: fn_19823
bool CEffectPrim::PrimPeongUp() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = slots[index];

    if (slot.life != 1) {
      continue;
    }

    if (++slot.process <= 0) {
      continue;
    }

    slot.alphaB += slot.process >= 30 ? -2 : 5;

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }

    for (int pair = 0; pair < 2; ++pair) {
      WanderAngle(slot.height[pair * 2], slot.height[pair * 2 + 1]);
    }

    float height = slot.vecBPre.y;

    if (height < 0.0f) {
      slot.vecBPre.x += g_sinTable[static_cast<int>(slot.height[0])] * 0.03f;
      slot.vecBPre.z += g_sinTable[static_cast<int>(slot.height[2])] * 0.03f;
    }

    slot.vecBPre.y = height - slot.maxHeight;
  }

  return true;
}

// OFFSET: 0x00BC0AE0
// FUNCTION: fn_19825
void CEffectPrim::PrimPokJuk() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = slots[index];

    if (slot.life != 1) {
      continue;
    }

    const int process = ++slot.process;

    if (process <= 0) {
      continue;
    }

    if (process % 3 == 0) {
      slot.vecTPre = slot.vecTNow;
      slot.vecTNow = slot.vecBPre;
      slot.vecBPre = slot.vecBNow;
    }

    const int delay = slot.flag1[2];

    if (process <= delay + 180 && process >= delay + 160) {
      const char beat = static_cast<char>(process % 20);

      if (beat == 15 || beat == 19) {
        slot.flag1[3] = 1;
      } else {
        slot.flag1[3] = beat == 17 ? 2 : 0;
      }
    } else {
      slot.flag1[3] = 0;
    }

    slot.rotStart += 5;

    if (slot.rotStart >= 360) {
      slot.rotStart -= 360;
    }

    if (slot.flag1[1] == 0) {
      slot.vecBNow.y -= 0.25f;

      if (process > 154) {
        slot.flag1[1] = 1;
        slot.alphaB = 250;
      }
    } else if (slot.flag1[1] == 1) {
      const float speed = slot.distance;
      const float flat = speed * g_cosTable[slot.fullDisplayAngle];
      slot.vecBNow.y = speed * g_sinTable[slot.fullDisplayAngle] + slot.vecBNow.y + 0.02f;
      slot.distance = speed * 0.98f;
      --slot.alphaB;
      slot.vecBNow.x += flat * g_cosTable[slot.riseAngle];
      slot.vecBNow.z += flat * g_sinTable[slot.riseAngle];

      const int coin = std::rand() % 2;

      if ((coin < 0 ? 0 : coin) == 0) {
        --slot.alphaB;
      }

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    }

    if (slot.process > 2000) {
      slot.process = 0;
      slot.flag1[1] = 0;
      const int heading = std::rand() % 360;
      slot.riseAngle = static_cast<std::int16_t>(heading < 0 ? 0 : heading);
      slot.vecBNow.x = this->render.position.x;
      slot.vecBNow.y = this->render.position.y;
      slot.vecBNow.z = this->render.position.z;
      slot.vecBNow.x = this->render.position.x - 15.0f;
      const int spread = std::rand() % 100;
      slot.distance = static_cast<float>(spread < 0 ? 0 : spread) * 0.004f + 0.4f;
    }
  }
}

// OFFSET: 0x00BC1F80
// FUNCTION: fn_19832
void CEffectPrim::PrimSTIN4() {
  CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  EffectPrimitiveSlot* slots = this->state.slots;

  for (int index = 0; index < 1; ++index) {
    EffectPrimitiveSlot& slot = slots[index];

    if (slot.life != 1 || ++slot.process <= 0) {
      continue;
    }

    const CRenderObject* target;

    if (this->state.deltaPos.y == static_cast<float>(g_session.m_aid)) {
      target = reinterpret_cast<const CRenderObject*>(gameMode->scene->player);
    } else {
      target = gameMode->scene->GetGameActorByAID(static_cast<std::uint32_t>(this->state.deltaPos.y));
    }

    if (target == nullptr) {
      slot.flag1[1] = 10;

      if (slot.process == 1) {
        return;
      }
    }

    slot.riseAngle += 30;

    if (slot.riseAngle >= 360) {
      slot.riseAngle -= 360;
    }

    if (slot.process == 1) {
      Get_Angle(target->render.position.x - this->render.position.x, target->render.position.z - this->render.position.z, &slot.rotStart);
      const int swing = std::rand() % 21;
      const std::int16_t turn = static_cast<std::int16_t>((swing < 0 ? 0 : swing) + 25);

      if (slot.flag1[2] == 0) {
        slot.rotStart += turn;

        if (slot.rotStart >= 360) {
          slot.rotStart -= 360;
        }
      } else {
        slot.rotStart -= turn;

        if (slot.rotStart < 0) {
          slot.rotStart += 360;
        }
      }
    } else if (slot.process <= 20) {
      slot.alphaB += 8;

      if (slot.alphaB > 120) {
        slot.alphaB = 120;
      }
    } else {
      slot.flag1[1] = 10;
    }

    if (slot.process > 5) {
      if (slot.flag1[1] == 0) {
        std::int16_t heading;
        Get_Angle(target->render.position.x - slot.vecBNow.x, target->render.position.z - slot.vecBNow.z, &heading);

        if (slot.rotStart >= heading - 3 && slot.rotStart <= heading + 3) {
          slot.flag1[1] = 10;
        } else if (slot.flag1[2] == 0) {
          slot.rotStart -= 5;

          if (slot.rotStart < 0) {
            slot.rotStart += 360;
          }
        } else {
          slot.rotStart += 5;

          if (slot.rotStart >= 360) {
            slot.rotStart -= 360;
          }
        }
      } else if (slot.flag1[1] == 10) {
        for (int fade = 0; fade < 4; ++fade) {
          slots[fade].alphaB -= 5;

          if (slots[fade].alphaB < 0) {
            slots[fade].alphaB = 0;
            slots[fade].life = 0;
          }
        }

        if (slot.flag1[2] == 0) {
          --slot.rotStart;

          if (slot.rotStart < 0) {
            slot.rotStart += 360;
          }
        } else {
          ++slot.rotStart;

          if (slot.rotStart >= 360) {
            slot.rotStart -= 360;
          }
        }
      }
    }

    slot.vecBNow.x += slot.height[1] * g_cosTable[slot.rotStart];
    slot.vecBNow.z += slot.height[1] * g_sinTable[slot.rotStart];

    if (slot.process % 3 == 0) {
      for (int trail = 2; trail >= 0; --trail) {
        slots[trail + 1].vecBNow = slots[trail].vecBNow;
        slots[trail + 1].alphaB = static_cast<std::int16_t>(static_cast<int>(static_cast<float>(slots[trail].alphaB) * 0.8f));
        slots[trail + 1].riseAngle = slots[trail].riseAngle;
      }
    }
  }
}

// OFFSET: 0x00BC2690
// FUNCTION: fn_19833
void CEffectPrim::PrimSaintWing() {
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;
    ++slot.riseAngle;

    if (slot.riseAngle >= 360) {
      slot.riseAngle -= 360;
    }

    slot.height[0] = -9.0f - static_cast<float>(this->state.slots[1].alphaT - 115) * 0.17f;
    slot.fullDisplayAngle = static_cast<std::int16_t>(static_cast<int>(g_sinTable[slot.riseAngle] * 20.0f + 20.0f));

    if (index == 0 || index == 2) {
      const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
      slot.rotStart = static_cast<std::int16_t>(static_cast<int>(player->render.angle + 135.0f));
    } else if (index == 1 || index == 3) {
      const auto* player = reinterpret_cast<const CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
      slot.rotStart = static_cast<std::int16_t>(static_cast<int>(player->render.angle + 45.0f));
    }
  }
}

// OFFSET: 0x00BC3720
// FUNCTION: fn_19837
bool CEffectPrim::PrimSoulParticle() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    for (int pair = 0; pair < 6; ++pair) {
      WanderAngle(slot.height[pair * 2], slot.height[pair * 2 + 1]);
    }

    if (slot.process < 10) {
      slot.height[12] += 20.0f;
      slot.height[13] += 20.0f;
      slot.height[14] += 20.0f;
    } else if (slot.process > 30) {
      for (int value = 12; value < 15; ++value) {
        slot.height[value] -= 3.0f;

        if (slot.height[value] < 0.0f) {
          slot.height[value] = 0.0f;
        }
      }
    }

    const float speed = slot.maxHeight;
    const float first = slot.vecBPre.y;

    if (first < 0.0f) {
      slot.vecBPre.x += speed * g_sinTable[static_cast<int>(slot.height[0])];
      slot.vecBPre.z += speed * g_sinTable[static_cast<int>(slot.height[2])];
    }

    slot.vecBPre.y = first - speed;
    const float second = slot.vecBNow.y;

    if (second < 0.0f) {
      slot.vecBNow.x -= speed * g_sinTable[static_cast<int>(slot.height[4])];
      slot.vecBNow.z -= speed * g_sinTable[static_cast<int>(slot.height[6])];
    }

    slot.vecBNow.y = speed + second;

    if (slot.vecTPre.y < 0.0f) {
      const float fast = speed * 1.2f;
      slot.vecTPre.x += fast * g_sinTable[static_cast<int>(slot.height[8])];
      slot.vecTPre.z -= fast * g_sinTable[static_cast<int>(slot.height[10])];
    }
  }

  return true;
}

// OFFSET: 0x00BC3A60
// FUNCTION: fn_19838
void CEffectPrim::PrimSphereWind2() {
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    const int process = ++slot.process;

    if (process <= 0) {
      continue;
    }

    if (process <= 25) {
      slot.alphaB += 2;
    } else if (process > 70) {
      slot.alphaB -= 2;
    }

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }

    slot.distance *= 1.01f;
    ++slot.riseAngle;

    const int coin = std::rand() % 2;

    if ((coin < 0 ? 0 : coin) == 0) {
      ++slot.riseAngle;
    }

    if (slot.riseAngle >= 360) {
      slot.riseAngle -= 360;
    }

    slot.rotStart = static_cast<std::int16_t>(static_cast<int>(static_cast<float>(slot.rotStart) + slot.height[2]));

    if (slot.rotStart >= 360) {
      slot.rotStart -= 360;
    }
  }
}

// OFFSET: 0x00BE7820
// FUNCTION: fn_19938
void CEffectPrim::PrimGI4() {
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1 || ++slot.process <= 0) {
      continue;
    }

    slot.distance += 0.07f;
    --slot.riseAngle;

    if (slot.riseAngle < 10) {
      slot.riseAngle = 10;
    }

    if (slot.distance >= 3.5f) {
      slot.alphaB -= 3;

      if (slot.alphaB <= 0) {
        slot.distance = 2.87f;
        slot.riseAngle = 74;
        slot.alphaB = 0;
      }
    } else {
      slot.alphaB += 10;
    }

    WaveSlotHeights(slot, index, 0.15f);
  }
}

// OFFSET: 0x00BE7AD0
// FUNCTION: fn_19940
void CEffectPrim::PrimGI1() {
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    const int process = ++slot.process;

    if (process <= 0) {
      continue;
    }

    slot.distance += 0.07f;
    --slot.riseAngle;

    if (slot.riseAngle < 10) {
      slot.riseAngle = 10;
    }

    if (slot.distance >= 4.0f) {
      slot.alphaB -= 3;

      if (slot.alphaB <= 0) {
        slot.alphaB = 0;

        if (process < this->state.duration - 30) {
          slot.distance = 3.37f;
          slot.riseAngle = 74;
        }
      }
    } else {
      slot.alphaB += 10;
    }

    WaveSlotHeights(slot, index, 0.3f);
  }
}

// OFFSET: 0x00BE8900
// FUNCTION: fn_19946
bool CEffectPrim::PrimCHOOKGI() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);
  FollowMaster(*this);

  EffectPrimitiveSlot& slot = slots[0];

  if (slot.life != 1) {
    return true;
  }

  if (++slot.process <= 100) {
    slot.alphaB += 2;

    if (slot.alphaB > 255) {
      slot.alphaB = 255;
    }
  }

  for (int pair = 0; pair < 3; ++pair) {
    WanderAngleSlow(slot.height[pair * 2], slot.height[pair * 2 + 1]);
  }

  const char rate = slot.flag1[0];
  bool spin = true;

  if (rate != 0) {
    spin = rate >= 10 ? slot.process % (rate - 9) == 0 : (slot.process & 1) == 0;
  }

  if (spin) {
    slot.height[6] += 1.0f;

    if (slot.height[6] >= 360.0f) {
      slot.height[6] -= 360.0f;
    }
  }

  if (slot.height[10] == 1.0f) {
    const float x = g_sinTable[static_cast<int>(slot.height[0])] * 0.7f;
    slot.vecBNow.x = x;
    const float z = g_sinTable[static_cast<int>(slot.height[2])] * 0.7f;
    slot.vecBNow.z = z;
    slot.vecBNow.y = g_sinTable[static_cast<int>(slot.height[4])] * 0.7f;
    slot.vecBNow.x = g_sinTable[static_cast<int>(slot.height[6])] * 5.5f + x;
    slot.vecBNow.z = g_cosTable[static_cast<int>(slot.height[6])] * 5.5f + z;
  } else {
    float x = g_sinTable[static_cast<int>(slot.height[0])];
    float z;
    float y;

    if (rate == 0) {
      slot.vecBNow.x = x;
      z = g_sinTable[static_cast<int>(slot.height[2])];
      slot.vecBNow.z = z;
      y = g_sinTable[static_cast<int>(slot.height[4])];
    } else {
      x *= 0.5f;
      slot.vecBNow.x = x;
      z = g_sinTable[static_cast<int>(slot.height[2])] * 0.5f;
      slot.vecBNow.z = z;
      y = g_sinTable[static_cast<int>(slot.height[4])] * 0.5f;
    }

    slot.vecBNow.y = y;
    slot.vecBNow.x = g_sinTable[static_cast<int>(slot.height[6])] * 7.0f + x;
    slot.vecBNow.z = g_cosTable[static_cast<int>(slot.height[6])] * 7.0f + z;
  }

  slot.vecBNow.y += slot.height[7];
  const int drift = std::rand() % 5;
  slot.height[8] += static_cast<float>((drift < 0 ? 0 : drift) + 5);

  if (slot.height[8] >= 360.0f) {
    slot.height[8] -= 360.0f;
  }

  return true;
}

// OFFSET: 0x00BC4BF0
// FUNCTION: fn_19845
bool CEffectPrim::PrimTwilight() {
  EffectPrimitiveSlot* slots = this->state.slots;
  FaceCamera(slots);
  FollowMaster(*this);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process < 50) {
      slot.alphaB += 5;
    }

    if (slot.process > slot.rotStart) {
      slot.alphaB -= 5;
    }

    for (int pair = 0; pair < 2; ++pair) {
      WanderAngleSlow(slot.height[pair * 2], slot.height[pair * 2 + 1]);
    }

    slot.vecBNow.x += g_sinTable[static_cast<int>(slot.height[0])] * 0.1f;
    slot.vecBNow.z += g_sinTable[static_cast<int>(slot.height[2])] * 0.1f;
    slot.height[8] += 1.0f;

    if (slot.height[8] >= 360.0f) {
      slot.height[8] -= 360.0f;
    }

    ++slot.fullDisplayAngle;

    if (slot.fullDisplayAngle >= 360) {
      slot.fullDisplayAngle -= 360;
    }
  }

  return true;
}

// OFFSET: 0x00BC5440
// FUNCTION: fn_19847
bool CEffectPrim::PrimWind() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process <= 0) {
      continue;
    }

    slot.rotStart += 5;

    if (slot.rotStart >= 360) {
      slot.rotStart -= 360;
    }

    slot.fullDisplayAngle += 3;

    if (slot.alphaT == 2) {
      if (slot.fullDisplayAngle > 180) {
        slot.fullDisplayAngle = 180;
      }

      if (slot.process > 50) {
        --slot.alphaB;

        if (slot.alphaB < 0) {
          slot.alphaB = 0;
        }
      }

      if (slot.process > 12) {
        slot.distance += 0.1f;
      }
    } else {
      if (slot.fullDisplayAngle > 120) {
        slot.fullDisplayAngle = 120;
      }

      if (slot.alphaT == 1 && slot.process > 20) {
        slot.alphaB -= 2;

        if (slot.alphaB < 0) {
          slot.alphaB = 0;
        }

        slot.distance += 0.1f;
      }
    }

    if (slot.process > 1400) {
      if (slot.alphaT != 3) {
        slot.alphaB -= 3;
      }

      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    }

    if (slot.alphaT == 2) {
      if (slot.process < 12) {
        ++slot.alphaB;
      }
    } else if (slot.process < 12) {
      slot.alphaB += 10;

      if (slot.alphaB > 250) {
        slot.alphaB = 250;
      }
    }
  }

  return true;
}

// INLINED
static __forceinline void RotatePairX(vector3d& point, int angle) {
  const float y = point.y;
  const float z = point.z;
  point.z = z * g_cosTable[angle] - y * g_sinTable[angle];
  point.y = y * g_cosTable[angle] + z * g_sinTable[angle];
}

// INLINED
static __forceinline void RotatePairY(vector3d& point, int angle) {
  const float x = point.x;
  const float z = point.z;
  point.x = x * g_cosTable[angle] - z * g_sinTable[angle];
  point.z = z * g_cosTable[angle] + x * g_sinTable[angle];
}

// OFFSET: 0x00BE7320
// FUNCTION: fn_19932
void CEffectPrim::Tei_TowardScreen(vector3d& vec1, vector3d& vec2, vector3d& vec3, vector3d& vec4, short rot0, short rot1) {
  RotatePairX(vec1, rot1);
  RotatePairX(vec2, rot1);
  RotatePairX(vec3, rot1);
  RotatePairX(vec4, rot1);
  RotatePairY(vec1, rot0);
  RotatePairY(vec2, rot0);
  RotatePairY(vec3, rot0);
  RotatePairY(vec4, rot0);
}

// OFFSET: 0x00BE7570
// FUNCTION: fn_19933
RenderVertex TransformVert(const matrix& view, const vector3d& position) {
  RenderVertex vertex;
  vertex.x = 0.0f;
  vertex.y = 0.0f;
  vertex.z = 0.0f;
  vertex.rhw = 0.0f;
  vertex.diffuse = 0;
  vertex.specular = 0xFF000000;
  vertex.u = 0.0f;
  vertex.v = 0.0f;
  g_renderer->viewport.Projection().Project(&position.x, view.values, vertex);
  return vertex;
}

// OFFSET: 0x00BE8680
// FUNCTION: fn_19943
bool CEffectPrim::PrimTextureSpin() {
  this->render.position = this->state.master->render.position;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    const int process = ++slot.process;

    if (slot.height[2] == 0.0f) {
      slot.rotStart = static_cast<std::int16_t>(slot.rotStart + 6);
      if (slot.rotStart >= 360) {
        slot.rotStart = static_cast<std::int16_t>(slot.rotStart - 360);
      }
      slot.riseAngle = static_cast<std::int16_t>(slot.riseAngle + 3);
      if (slot.riseAngle >= 360) {
        slot.riseAngle = static_cast<std::int16_t>(slot.riseAngle - 360);
      }
    }

    if (process % 4 == 0) {
      slot.height[1] += 1.0f;
    }

    const float frames = static_cast<float>(this->state.totalTexture);
    if (slot.height[1] >= frames) {
      slot.height[1] -= frames;
    }
  }

  return true;
}

// OFFSET: 0x00BE9490
// FUNCTION: fn_19951
void CEffectPrim::PrimScrollTexture() {
  this->render.position = this->state.master->render.position;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1 || ++slot.process <= 0) {
      continue;
    }

    slot.alphaB = static_cast<std::int16_t>(slot.alphaB + 2);
    if (slot.alphaB > 255) {
      slot.alphaB = 255;
    }

    slot.height[1] += 1.0f;
    if (slot.height[1] >= static_cast<float>(this->state.totalTexture)) {
      slot.height[1] = 0.0f;
    }

    slot.distance -= 0.66f;
    slot.height[10] -= 1.0f;
    if (slot.distance < 0.0f) {
      slot.distance += slot.maxHeight;
    }
    if (slot.height[10] < 0.0f) {
      slot.height[10] += slot.maxHeight;
    }
  }
}

// OFFSET: 0x00BE95C0
// FUNCTION: fn_19953
void CEffectPrim::PrimFrameFade() {
  FaceCamera(this->state.slots);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    if (++slot.process % 3 == 0) {
      ++slot.flag1[0];
      const int frames = static_cast<int>(this->state.totalTexture);
      if (slot.flag1[0] >= frames) {
        slot.flag1[0] = static_cast<char>(frames - 1);
        slot.alphaB = static_cast<std::int16_t>(slot.alphaB - 15);
      }
    }

    if (slot.alphaB >= 255) {
      slot.alphaB = 255;
    } else if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }
  }
}

// OFFSET: 0x00BEB3E0
// FUNCTION: fn_19969
void CEffectPrim::PrimRiseFollow() {
  FaceCamera(this->state.slots);
  this->render.position = this->state.master->render.position;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1 || ++slot.process <= 0) {
      continue;
    }

    slot.rotStart = static_cast<std::int16_t>(slot.rotStart + 5);
    if (slot.rotStart > 100) {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB - 2);
      slot.rotStart = 100;
    }

    if (slot.alphaB >= 255) {
      slot.alphaB = 255;
    } else if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }

    slot.vecBNow.y = slot.vecBPre.y;
    slot.vecBNow.x = this->render.position.x;
    slot.vecBNow.z = this->render.position.z;
  }
}

// OFFSET: 0x00BBAA10
// FUNCTION: fn_19789
void CEffectPrim::PrimFadeInFollow() {
  FaceCamera(this->state.slots);
  this->render.position = this->state.master->render.position;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life == 1) {
      ++slot.process;
      if (slot.alphaB < 255) {
        ++slot.alphaB;
      }
    }
  }
}

// OFFSET: 0x00BBD390
// FUNCTION: fn_19804
void CEffectPrim::PrimAnimatedFlash() {
  FaceCamera(this->state.slots);

  EffectPrimitiveSlot& slot = this->state.slots[0];
  if (slot.life != 1) {
    return;
  }

  const int process = ++slot.process;
  if (process % 4 == 0) {
    char frame = static_cast<char>(slot.flag1[0] + 1);
    if (frame >= static_cast<int>(this->state.totalTexture)) {
      frame = 0;
    }
    slot.flag1[0] = frame;
  }

  if (process < 30) {
    slot.alphaB = static_cast<std::int16_t>(slot.alphaB + 10);
  } else if (process > 80) {
    slot.alphaB = static_cast<std::int16_t>(slot.alphaB - 3);
  }

  if (slot.alphaB >= 255) {
    slot.alphaB = 255;
  } else if (slot.alphaB < 0) {
    slot.alphaB = 0;
  }
}

// OFFSET: 0x00BE99A0
// FUNCTION: fn_19956
void CEffectPrim::PrimRandomSpin() {
  FaceCamera(this->state.slots);
  this->render.position = this->state.master->render.position;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    const int process = ++slot.process;
    if (process <= 0) {
      continue;
    }

    slot.fullDisplayAngle = static_cast<std::int16_t>(slot.fullDisplayAngle - 10);
    if (slot.fullDisplayAngle < 0) {
      slot.fullDisplayAngle = static_cast<std::int16_t>(slot.fullDisplayAngle + 360);
    }

    if (process == 1) {
      const int angle = std::rand() % 360;
      slot.rotStart = static_cast<std::int16_t>(angle >= 0 ? angle : 0);
    } else if (process <= 10) {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB + 10);
    } else if (process > 100 && --slot.alphaB < 0) {
      slot.alphaB = 0;
      slot.life = 0;
    }
  }
}

// OFFSET: 0x00BEA220
// FUNCTION: fn_19961
void CEffectPrim::PrimHeightRamp() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    const int process = ++slot.process;
    slot.rotStart = static_cast<std::int16_t>(slot.rotStart + 1);
    if (slot.rotStart >= 360) {
      slot.rotStart = static_cast<std::int16_t>(slot.rotStart - 360);
    }

    if (process >= 125) {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB - 15);
      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
      slot.maxHeight -= 5.0f;
      if (slot.maxHeight < 0.0f) {
        slot.maxHeight = 0.0f;
      }
    }

    for (float& height : slot.height) {
      float value;
      if (slot.process < 100) {
        value = 0.0f;
      } else if (slot.process > 115) {
        value = slot.maxHeight;
      } else {
        value = g_sinTable[6 * slot.process - 600] * slot.maxHeight;
      }

      height = value;
      if (value < 0.0f) {
        height = 0.0f;
      }
    }
  }
}

// OFFSET: 0x00BBBE40
// FUNCTION: fn_19798
void CEffectPrim::PrimBobbing() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life <= 0) {
      continue;
    }

    const int process = ++slot.process;

    if (slot.riseAngle >= 360) {
      slot.riseAngle = static_cast<std::int16_t>(slot.riseAngle - 360);
    }

    if (slot.flag1[0] == 11) {
      if (process <= 40) {
        ++slot.alphaB;
      } else if (process > 140) {
        --slot.alphaB;
      }

      if (slot.alphaB <= 0) {
        slot.alphaB = 0;
      }
    }

    slot.height[0] = slot.height[2];
    slot.height[0] = g_sinTable[process % 360] * 0.1f + slot.height[2];
  }
}

// OFFSET: 0x00BE7620
// FUNCTION: fn_19936
bool CEffectPrim::PrimTwinSpin() {
  this->render.position = this->state.master->render.position;

  EffectPrimitiveSlot& first = this->state.slots[0];
  first.riseAngle = static_cast<std::int16_t>(first.riseAngle + 3);
  const int firstProcess = ++first.process;
  if (first.riseAngle >= 360) {
    first.riseAngle = static_cast<std::int16_t>(first.riseAngle - 360);
  }

  const int fadeStart = this->state.duration - 25;
  if (firstProcess > fadeStart) {
    first.alphaB = static_cast<std::int16_t>(first.alphaB - 8);
    if (first.alphaB < 0) {
      first.alphaB = 0;
    }
  }

  EffectPrimitiveSlot& second = this->state.slots[1];
  second.riseAngle = static_cast<std::int16_t>(second.riseAngle + 3);
  const int secondProcess = ++second.process;
  if (second.riseAngle >= 360) {
    second.riseAngle = static_cast<std::int16_t>(second.riseAngle - 360);
  }

  if (secondProcess > fadeStart) {
    second.alphaB = static_cast<std::int16_t>(second.alphaB - 8);
    if (second.alphaB < 0) {
      second.alphaB = 0;
    }
  }

  return true;
}

// OFFSET: 0x00BE90C0
// FUNCTION: fn_19948
void CEffectPrim::PrimFlashFade() {
  FaceCamera(this->state.slots);

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process <= 15) {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB + 13);
      if (slot.alphaB > 255) {
        slot.alphaB = 255;
      }
    } else if (slot.process > 20) {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB - 3);
      if (slot.alphaB < 0) {
        slot.life = 0;
        slot.alphaB = 0;
      }
    }
  }
}

// OFFSET: 0x00BEB330
// FUNCTION: fn_19968
bool CEffectPrim::PrimFadeAlpha() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process < 50) {
      this->state.alpha += 5.0f;
      if (this->state.alpha > 150.0f) {
        this->state.alpha = 150.0f;
      }
    } else if (slot.process > this->state.duration - 50) {
      this->state.alpha -= 5.0f;
      if (this->state.alpha < 0.0f) {
        this->state.alpha = 0.0f;
      }
    }
  }

  this->state.ProcessAlpha();
  this->ShiftSegment();
  return this->AnimMotion();
}

// OFFSET: 0x00BE92B0
// FUNCTION: fn_19950
void CEffectPrim::PrimFrameScroll() {
  this->render.position = this->state.master->render.position;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1 || ++slot.process <= 0) {
      continue;
    }

    slot.height[1] += 1.0f;
    if (slot.height[1] >= static_cast<float>(this->state.totalTexture)) {
      slot.height[1] = 0.0f;
    }

    slot.distance -= 1.0f;
    if (slot.distance < 0.0f) {
      slot.distance += slot.maxHeight;
    }
  }
}

// OFFSET: 0x00BEA310
// FUNCTION: fn_19962
void CEffectPrim::PrimFramePhase() {
  this->render.position = this->state.master->render.position;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1 || ++slot.process <= 0) {
      continue;
    }

    slot.height[1] += 1.0f;
    if (slot.height[1] >= static_cast<float>(this->state.totalTexture)) {
      slot.height[1] = 0.0f;
    }

    slot.height[9] = slot.height[2] + slot.height[9];
    if (slot.height[9] >= 1.0f) {
      slot.height[9] -= 1.0f;
    }
  }
}

// OFFSET: 0x00BE9CE0
// FUNCTION: fn_19958
void CEffectPrim::PrimTrailShift() {
  this->render.position = this->state.master->render.position;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    const int process = ++slot.process;
    slot.vecBNow = this->render.position;
    slot.vecBNow.y = this->render.position.y - 10.0f;

    if (process <= 0) {
      continue;
    }

    const int delay = index * 10;
    if (slot.alphaT == 1) {
      if (process > delay) {
        slot.riseAngle = static_cast<std::int16_t>(slot.riseAngle + 2);
      }
      if (slot.riseAngle >= 360) {
        slot.riseAngle = static_cast<std::int16_t>(slot.riseAngle - 360);
      }
    } else {
      if (process > delay) {
        slot.riseAngle = static_cast<std::int16_t>(slot.riseAngle - 2);
      }
      if (slot.riseAngle < 0) {
        slot.riseAngle = static_cast<std::int16_t>(slot.riseAngle + 360);
      }
    }

    if (process > 60) {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB - 6);
      if (slot.alphaB < 0) {
        slot.alphaB = 0;
      }
    }
    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }

    if (process == 1) {
      for (int step = 0; step < 21; ++step) {
        slot.height[0] += 9.0f;
        if (slot.height[0] > 180.0f) {
          slot.height[0] = 180.0f;
          continue;
        }

        for (int point = static_cast<int>(slot.vecTNow.y); point >= 1; --point) {
          slot.height[point] = slot.height[point - 1];
          slot.flag1[point] = slot.flag1[point - 1];
        }
      }
    }
  }
}

// OFFSET: 0x00BBE960
// FUNCTION: fn_19812
void CEffectPrim::PrimGrowFollow() {
  this->render.position = this->state.master->render.position;

  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life <= 0) {
      continue;
    }

    ++slot.process;

    if (slot.process <= 0) {
      continue;
    }

    if (slot.process <= 10) {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB + (this->state.size != 113.0f ? 2 : 6));
    }

    slot.distance *= 1.07f;

    if (slot.process > 10 && --slot.alphaB < 0) {
      slot.alphaB = 0;
    }
  }
}

// OFFSET: 0x00BC3BD0
// FUNCTION: fn_19839
void CEffectPrim::PrimSpinFade() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1 || ++slot.process <= 0) {
      continue;
    }

    const int turn = std::rand() % 3;
    slot.rotStart = static_cast<std::int16_t>(slot.rotStart + (turn >= 0 ? turn : 0) + 3);
    if (slot.rotStart >= 360) {
      slot.rotStart = static_cast<std::int16_t>(slot.rotStart - 360);
    }

    if (slot.flag1[5] == 1) {
      if (slot.process > 120) {
        --slot.alphaB;
      }
    } else if (slot.flag1[5] != 10 && slot.process > 60) {
      --slot.alphaB;
    }

    if (slot.process <= 40) {
      ++slot.alphaB;
    }

    if (slot.alphaB < 0) {
      slot.alphaB = 0;
    }
  }
}

// OFFSET: 0x00BC4380
// FUNCTION: fn_19842
void CEffectPrim::PrimExpandPulse() {
  for (int index = 0; index < 4; ++index) {
    EffectPrimitiveSlot& slot = this->state.slots[index];

    if (slot.life != 1) {
      continue;
    }

    ++slot.process;

    if (slot.process > 0) {
      slot.distance += slot.height[0];
    }

    if (static_cast<unsigned>(slot.process - 11) > 9) {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB - 5);
      if (slot.alphaB <= 0) {
        slot.alphaB = 0;
      }
    } else if (slot.flag1[1] != 24) {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB + 15);
    } else {
      slot.alphaB = static_cast<std::int16_t>(slot.alphaB + (g_serviceOptions.Get(229) != 0 ? 8 : 15));
    }
  }
}

// OFFSET: 0x00BE75D0
// FUNCTION: fn_19934
void CEffectPrim::ScaleByMaster(float& scale) const {
  if (this->state.master && this->state.master->state.parent && this->state.count == 5) {
    this->state.master->state.parent->ScaleByFadePulse(scale);
  }
}
