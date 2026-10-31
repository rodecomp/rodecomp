#include "Contents/Effects/MsgEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/GroundScope.h"
#include "Contents/Actors/Skill.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Effects/Particle.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/MapSkyMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <cstdint>
#include <cstdlib>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_25145
int g_damageMillionUnit;

// GLOBAL: gl_24976
std::string g_damageResourceName;

// GLOBAL: gl_24975
bool g_msgEffectShowSpecial = true;

// OFFSET: 0x00E4A550
// FUNCTION: fn_24255
CMsgEffect::CMsgEffect() {
  this->m_drift.x = 0.0f;
  this->m_drift.y = 0.0f;
  this->m_drift.z = 0.0f;
  this->unknown_016C.x = 0.0f;
  this->unknown_016C.y = 0.0f;
  this->unknown_016C.z = 0.0f;
  this->m_velocity.x = 0.0f;
  this->m_owner = nullptr;
  this->render.animation = 0;
  this->render.curMotion = 0;
  this->render.unknown_00A2 = 1;
  this->m_value = 0;
  this->m_type = 0;
  this->m_finished = 0;
  this->unknown_018C = 0.0f;
  this->m_baseY = 1.0f;
  this->m_baseX = 1.0f;
  this->m_scale = 1.0f;
  this->unknown_01A0 = 0;
  this->unknown_01A4 = 0;
  this->render.unknown_0080 = 0xFFFFFFFF;
  this->m_digit = 0;
  this->unknown_01B4 = 0;
  this->unknown_01B0 = 1;
  this->m_drift.x = 0.0f;
  this->m_drift.y = 0.0f;
  this->m_drift.z = 0.0f;
  this->m_velocity.x = 0.0f;
  this->m_velocity.y = 0.0f;
  this->m_velocity.z = 0.0f;
  this->m_visible = true;
}

// OFFSET: 0x00E4A9B0
// FUNCTION: fn_24258
CMsgEffect::~CMsgEffect() {
  if (this->m_owner != nullptr) {
    this->m_owner->SendMsg(this, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
}

// OFFSET: 0x00E4B8B0
// FUNCTION: fn_24272
vector2d& vector2d::Normalize() {
  const float inverse = 1.0f / SquareRoot(this->x * this->x + this->y * this->y);
  this->y = inverse * this->y;
  this->x = inverse * this->x;
  return *this;
}

// OFFSET: 0x00E4B630
// FUNCTION: fn_24270
bool CMsgEffect::UsesMsgEffect(int type) {
  switch (type) {
    case 3:
    case 18:
    case 27:
    case 31:
      return true;
    default:
      return false;
  }
}

// INLINED
static float MsgEffectElapsedFrames(CMsgEffect* effect) {
  return static_cast<CGameActor*>(static_cast<CRenderObject*>(effect))->GetElapsedFrames();
}

// INLINED
__forceinline static void MsgEffectDrift(CMsgEffect* effect, float elapsed) {
  float dirX = effect->m_drift.x;
  if (effect->m_drift.x == 0.0f && effect->m_owner != nullptr) {
    if (g_session.m_damageSkinMode == 0) {
      const int angle = static_cast<int>(static_cast<CRenderObject*>(effect->m_owner)->render.angle);
      if (angle < 0) {
        dirX = 0.8f;
        effect->m_drift.x = 0.8f;
        effect->m_drift.z = 0.8f;
      } else if (angle < 90) {
        dirX = -0.8f;
        effect->m_drift.x = -0.8f;
        effect->m_drift.z = 0.8f;
      } else if (angle < 180) {
        dirX = -0.8f;
        effect->m_drift.x = -0.8f;
        effect->m_drift.z = -0.8f;
      } else if (angle < 270) {
        dirX = 0.8f;
        effect->m_drift.x = 0.8f;
        effect->m_drift.z = -0.8f;
      } else {
        dirX = 0.8f;
        effect->m_drift.x = 0.8f;
        effect->m_drift.z = 0.8f;
      }
    } else {
      if (g_session.m_damageSkinMode == 1) {
        effect->m_drift.x = 0.8f;
        effect->m_drift.z = -0.8f;
      } else if (g_session.m_damageSkinMode == 2) {
        effect->m_drift.x = 0.01f;
        effect->m_drift.z = 0.01f;
      } else if (g_session.m_damageSkinMode == 3) {
        effect->m_drift.x = -0.8f;
        effect->m_drift.z = -0.8f;
      }

      CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      float yaw = 0.0f;
      if (mode != nullptr) {
        yaw = mode->camera->camera.desiredYaw;
      }

      matrix rotation;
      rotation.MakeYRotation(yaw / 180.0f * 3.141592f);
      const float x = effect->m_drift.x;
      const float y = effect->m_drift.y;
      const float z = effect->m_drift.z;
      dirX = rotation.values[0] * x + rotation.values[3] * y + rotation.values[6] * z + rotation.values[9];
      effect->m_drift.x = dirX;
      effect->m_drift.z = rotation.values[2] * x + rotation.values[5] * y + rotation.values[8] * z + rotation.values[11];
    }
  }

  const float step = elapsed / 3.0f;
  effect->render.position.x = dirX * step + effect->m_velocity.x + dirX * 3.0f;
  effect->render.position.z = effect->m_drift.z * step + effect->m_velocity.z + effect->m_drift.z * 3.0f;
  float height = effect->m_baseY + 8.0f - (2.0f - elapsed / 30.0f) * elapsed + effect->m_drift.y;
  effect->render.position.y = height;
  if (height > 0.0f) {
    effect->m_drift.y = height / (height * 3.0f * 0.25f);
  } else if (height < 0.0f) {
    height = -height;
    effect->m_drift.y = height / (height * 3.0f * 0.25f);
  }
}

// OFFSET: 0x00E4C3E0
// FUNCTION: fn_24286
bool CMsgEffect::OnProcess() {
  if (g_updateTick == 0) {
    return true;
  }

  if (this->m_finished != 0) {
    if (this->m_type != 0x12 || !this->m_visible) {
      return false;
    }
    if (this->unknown_01A0 > 35) {
      if (this->unknown_01B4 == 0) {
        return false;
      }
      this->unknown_01B4 = 0;
      this->m_finished = 0;
      return true;
    }
    if (this->unknown_01B4 == 0) {
      return false;
    }
  }

  this->FollowOwner();
  switch (this->m_type) {
    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x15:
    case 0x16:
    case 0x19:
    case 0x72:
      break;
    default:
      this->PlayFrameSounds();
      break;
  }

  float elapsed;
  switch (this->m_type) {
    case 0:
    case 1:
    case 2:
    case 0x11: {
      CActRes* action = this->GetActRes();
      elapsed = MsgEffectElapsedFrames(this);
      const int frames = static_cast<int>(action->FrameCount(this->render.previousAnimation));
      if (frames == 0) {
        return true;
      }
      const float motion = elapsed / this->render.speed;
      this->render.curMotion = static_cast<int>(motion) % frames;
      if (this->m_type == 1 && this->render.unknown_0070 == 10000) {
        this->render.curMotion = 15;
        const ActionFrame& frame = action->Frame(this->render.previousAnimation, 15);
        const_cast<SpriteFrameCell&>(frame.cells[0]).mirrored = 0;
        this->SetColor(255, 250, 80, 80);
        return true;
      }
      if (motion / static_cast<float>(frames) < 1.0f) {
        return true;
      }
      if (this->m_type != 1) {
        return false;
      }
      if (this->m_value == static_cast<int>(g_session.m_aid)) {
        g_modeMgr.GetGameMode()->SendMsg(0x21, 0, 0, 0, 0);
      }
      return false;
    }

    case 4:
    case 5:
    case 7:
    case 8:
    case 9:
    case 10:
    case 0x1A:
    case 0x25:
    case 0x26: {
      elapsed = MsgEffectElapsedFrames(this);
      CActRes* action = this->GetActRes();
      const int frames = static_cast<int>(action->FrameCount(this->render.previousAnimation));
      this->render.curMotion = static_cast<int>(elapsed / this->render.speed) % frames;
      return true;
    }

    case 6: {
      elapsed = MsgEffectElapsedFrames(this);
      CActRes* action = this->GetActRes();
      const int frames = static_cast<int>(action->FrameCount(this->render.previousAnimation));
      const float motion = elapsed * 1.2f;
      this->render.curMotion = static_cast<int>(motion) % frames;
      if (motion / static_cast<float>(frames) < 1.0f) {
        return true;
      }
      return false;
    }

    case 0xB:
    case 0xC:
      if (this->m_owner == nullptr) {
        return true;
      }
      elapsed = MsgEffectElapsedFrames(this);
      if (static_cast<int>(elapsed) % 34 == 0) {
        this->m_owner->SendMsg(nullptr, 0x4D, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
      }
      return true;

    case 0xD:
      elapsed = MsgEffectElapsedFrames(this);
      this->render.previousAnimation = this->render.animation;
      this->render.position.y = this->m_baseY - elapsed * 0.54f;
      this->SetColor(static_cast<int>(250.0f - elapsed * 3.0f), -1, -1, -1);
      this->render.scale = 1.0f;
      this->GetActRes();
      return !(elapsed > 80.0f);

    case 0xE:
    case 0x15:
      elapsed = MsgEffectElapsedFrames(this);
      this->render.previousAnimation = this->render.animation;
      MsgEffectDrift(this, elapsed);
      if (this->render.scale > 1.2f) {
        this->render.scale = this->m_scale - elapsed * 0.24f;
      } else if (this->render.scale < 1.2f) {
        this->render.scale = 1.2f;
      }
      this->SetColor(static_cast<int>(250.0f - elapsed * 3.4f), -1, -1, -1);
      return !(elapsed > 70.0f);

    case 0xF: {
      elapsed = MsgEffectElapsedFrames(this);
      this->render.previousAnimation = this->render.animation;
      MsgEffectDrift(this, elapsed);
      if (this->render.scale > 1.0f) {
        this->render.scale = this->m_scale - elapsed * 0.24f * 0.6f;
      } else if (this->render.scale < 1.0f) {
        this->render.scale = 1.0f;
      }
      CActRes* action = this->GetActRes();
      if (this->render.curMotion < static_cast<int>(action->FrameCount(this->render.previousAnimation)) - 1) {
        const int frames = static_cast<int>(action->FrameCount(this->render.previousAnimation));
        this->render.curMotion = static_cast<int>(elapsed * g_depthAverageHalf) % frames;
      }
      this->SetColor(static_cast<int>(255.0f - elapsed * 3.45f), -1, -1, -1);
      return !(elapsed > 70.0f);
    }

    case 0x10:
      elapsed = MsgEffectElapsedFrames(this);
      this->render.previousAnimation = this->render.animation;
      MsgEffectDrift(this, elapsed);
      if (this->render.scale > 1.3f) {
        this->render.scale = this->m_scale - elapsed * 0.24f;
      } else if (this->render.scale < 1.3f) {
        this->render.scale = 1.3f;
      }
      this->SetColor(static_cast<int>(250.0f - elapsed * 3.4f), -1, -1, -1);
      return !(elapsed > 70.0f);

    case 0x12:
      if (this->unknown_01B4 != 0 && this->m_owner != nullptr) {
        CRenderObject* owner = static_cast<CRenderObject*>(this->m_owner);
        this->Face(owner->render.position.x, owner->render.position.z);
        this->render.angle -= 180.0f;
        if (this->render.angle < 0.0f) {
          this->render.angle += 360.0f;
        }
        this->render.previousAnimation = this->Get8Dir2(this->render.angle, 1) + 8;
        this->render.position.x = this->m_velocity.x + this->render.position.x;
        const int tick = this->unknown_01A0;
        if (tick > 10 && tick < 25) {
          this->unknown_019C = this->m_velocity.x;
          this->render.position.z = this->m_velocity.z + this->render.position.z;
        } else {
          this->unknown_019C = this->m_velocity.x;
          this->render.position.y = this->m_velocity.y + this->render.position.y;
          this->render.position.z = this->m_velocity.z + this->render.position.z;
          if (tick >= 25) {
            this->m_finished = 1;
            this->m_velocity.y = this->m_velocity.y * -1.9f;
          }
        }
        this->GetActRes();
        ++this->unknown_01A0;
        this->unknown_01A4 = this->render.previousAnimation;
        this->render.curMotion = 0;
        return true;
      }
      if (this->m_owner == nullptr) {
        return false;
      }
      {
        CActRes* action = this->GetActRes();
        const int frames = static_cast<int>(action->FrameCount(this->render.previousAnimation));
        int tick = this->unknown_01A0;
        if (tick % static_cast<int>(this->render.speed) == 0) {
          if (this->unknown_01B0 != 0) {
            ++this->render.curMotion;
          }
          tick = 0;
        }
        if (this->render.curMotion >= frames) {
          this->render.curMotion = 0;
        }
        this->unknown_01A4 = this->render.previousAnimation;
        this->unknown_01A0 = tick + 1;
      }
      return true;

    case 0x16: {
      elapsed = MsgEffectElapsedFrames(this);
      this->render.previousAnimation = this->render.animation;
      this->render.position.y = this->m_baseY - elapsed * 0.1f;
      int fade = 0;
      if (elapsed > 90.0f) {
        fade = static_cast<int>((elapsed - 90.0f) * 8.0f);
      }
      this->SetColor(250 - fade, -1, -1, -1);
      if (this->render.scale < 3.0f) {
        this->render.scale = elapsed * 0.18f + this->render.scale;
      } else if (this->render.scale > 3.0f) {
        this->render.scale = 3.0f;
      }
      return !(elapsed > 120.0f);
    }

    case 0x18: {
      elapsed = MsgEffectElapsedFrames(this);
      this->render.previousAnimation = this->render.animation;
      if (this->render.curMotion == 0) {
        CPlayer* player = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player;
        g_soundMgr->PlayWave("effect\\kyrie_guard.wav", this->render.position.x - player->render.position.x, 0.0f,
                             this->render.position.z - player->render.position.z, 250, 40, 1.0f, 0);
      }
      ++this->render.curMotion;
      if (this->render.curMotion >= static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation))) {
        this->render.curMotion = static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation)) - 1;
      }
      this->SetColor(250, -1, -1, -1);
      this->render.scale = 1.2f;
      this->m_scale = 1.2f;
      return !(elapsed > 10.0f);
    }

    case 0x19: {
      elapsed = MsgEffectElapsedFrames(this);
      this->render.previousAnimation = this->render.animation;
      this->render.curMotion = 0;
      this->render.position.y = this->m_baseY - elapsed * 0.1f - 15.0f;
      int fade = 0;
      if (elapsed > 90.0f) {
        fade = static_cast<int>((elapsed - 90.0f) * 8.0f);
      }
      int blue = -1;
      int tint = -1;
      if (fade <= 210) {
        tint = 150;
        blue = 255;
      }
      int alpha = 0;
      if (fade <= 210) {
        alpha = 210 - fade;
      }
      this->SetColor(alpha, tint, tint, blue);
      if (this->render.scale < 2.0f) {
        this->render.scale = elapsed * 0.15f + this->render.scale;
      } else if (this->render.scale > 2.0f) {
        this->render.scale = 2.0f;
      }
      return !(elapsed > 120.0f);
    }

    case 0x1B:
    case 0x1F: {
      CActRes* action = this->GetActRes();
      const int frames = static_cast<int>(action->FrameCount(this->render.previousAnimation));
      if (frames == 0) {
        return true;
      }
      const int tick = this->unknown_01A0;
      this->render.curMotion = static_cast<int>(static_cast<float>(tick) / this->render.speed) % frames;
      if (this->unknown_01B4 == 1) {
        if (this->render.curMotion >= frames - 1) {
          this->unknown_01B4 = 0;
        }
        if (tick < 8) {
          this->render.position.x = this->m_velocity.x + this->render.position.x;
          this->render.position.y = this->m_velocity.y + this->render.position.y;
          this->render.position.z = this->m_velocity.z + this->render.position.z;
        }
      }
      this->unknown_01A0 = tick + 1;
      return true;
    }

    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x20:
    case 0x21:
    case 0x22:
    case 0x23:
    case 0x24: {
      CActRes* action = this->GetActRes();
      elapsed = MsgEffectElapsedFrames(this);
      const int frames = static_cast<int>(action->FrameCount(this->render.previousAnimation));
      if (frames == 0) {
        return true;
      }
      this->render.curMotion = static_cast<int>(elapsed / this->render.speed) % frames;
      return true;
    }

    case 0x72:
      elapsed = MsgEffectElapsedFrames(this);
      this->render.previousAnimation = this->render.animation;
      this->render.position.y = this->m_baseY - elapsed * 0.18f;
      this->SetColor(static_cast<int>(250.0f - elapsed * 2.0f), -1, -1, -1);
      if (this->render.scale > 1.0f) {
        this->render.scale = this->m_scale - elapsed * 0.24f;
      } else if (this->render.scale < 1.0f) {
        this->render.scale = 1.0f;
      }
      return !(elapsed > 120.0f);

    default:
      return true;
  }
}

// OFFSET: 0x00E4D6A0
// FUNCTION: fn_24287
bool CSkill::OnProcess() {
  this->UpdateGaugePositions();
  this->ProcessDamageMessages();

  switch (this->GetEffectiveJob(-1)) {
  case 0xCC:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x336, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xCB:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x8E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
      g_modeMgr.GetGameMode()->SendMsg(0x7A, 0, 0, 0, 0);
    }
    break;

  case 0xCA:
    if (!this->m_hasEffect) {
      this->m_hasEffect = true;
      this->LaunchEffect(0x677, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_effectMgr->Spawn(this->render.accountId, 0x676, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;

  case 0x82:
    if (this->m_effectTimer >= EffectLifetime(0x5B) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x5B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0x83:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x13F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xB1:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x12E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0x9A:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0xEF, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xE1:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x304, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
      int random = rand() % 100;
      random = random < 0 ? 0 : random;
      if (random == 1) {
        g_effectMgr->Spawn(this->skillAid, 0x60A, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      } else if (random == 2) {
        g_effectMgr->Spawn(this->skillAid, 0x60B, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
      }
    }
    break;

  case 0xB6:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x195, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0x9B:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0xF0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xBB:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x26C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0x9C:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0xF1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0x9D:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0xF2, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xC8:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x29F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xC9:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x2A0, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xB4:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x176, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xB7:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x194, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xB9:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x205, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xB8:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x20A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xE2:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x30F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xC7:
    if (!this->m_hasEffect && g_effectMgr->Spawn(this->skillAid, 0x613, this->render.position.x, this->render.position.y, this->render.position.z, 0, this->range * 2, 0, 0)) {
      this->m_hasEffect = true;
    }
    break;

  case 0xB3:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x172, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xE3:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x310, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0x92:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0xAB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xE0:
    if (!this->m_hasEffect && g_effectMgr->Spawn(this->skillAid, 0x438, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0)) {
      this->m_hasEffect = true;
      this->m_effectId = 0x438;
    }
    break;

  case 0x84:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x13E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0x88:
    if (this->m_effectTimer >= EffectLifetime(0x61) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x61, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0x87:
  case 0xC5:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x8A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0x8D:
  case 0xC3:
  case 0xC4:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x4A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0x8E:
    if (this->m_effectTimer >= EffectLifetime(0x5F) - 20u) {
      this->m_effectTimer = 0;
      if (g_serviceOptions.Get(0xE5)) {
        g_effectMgr->Spawn(0, 0x489, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      } else {
        this->LaunchEffect(0x5F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }
    }
    break;

  case 0x81:
    if (this->m_effectTimer >= EffectLifetime(0x13C) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x13C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0x80:
    if (this->m_effectTimer >= EffectLifetime(0x13D) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x13D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xCD:
    if (this->m_effectTimer >= EffectLifetime(0x339) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x339, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xCE:
    if (this->m_effectTimer >= EffectLifetime(0x33B) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x33B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xCF:
    if (this->m_effectTimer >= EffectLifetime(0x33C) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x33C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xDA:
    if (this->m_effectTimer >= EffectLifetime(0x358) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x358, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\REVERBERATION.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;

  case 0xDE:
    if (this->m_effectTimer >= EffectLifetime(0x35C) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x35C, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\POEMOFNETHERWORLD.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;

  case 0xDB:
    if (this->m_effectTimer >= EffectLifetime(0x359) - 8u) {
      this->m_effectTimer = 0;
      g_effectMgr->Spawn(0, 0x4FC, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
      g_soundMgr->PlayWave("effect\\rainstorm.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;

  case 0xD0:
    if (this->m_effectTimer >= EffectLifetime(0x33D) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x33D, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xB2:
    if (this->m_effectTimer >= EffectLifetime(0x155) - 8u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x155, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0x7E:
    if (this->m_effectTimer >= EffectLifetime(0x13B) - 7u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x13B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0x7F:
  case 0xED:
    if (this->m_effectTimer >= EffectLifetime(0x19) - 7u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x19, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xDC:
    if (this->m_effectTimer >= EffectLifetime(0x398) - 7u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x398, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xDD:
    if (this->m_effectTimer >= EffectLifetime(0x39E) - 7u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x39E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xEB:
    if (this->m_effectTimer >= EffectLifetime(0x399) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x399, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\S스펠피스트.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;

  case 0xEA:
    if (this->m_effectTimer >= EffectLifetime(0x397) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x397, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\G가시나무덫.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;

  case 0xBC:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x277, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xBD:
    if (this->m_effectTimer >= EffectLifetime(0x27A) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x27A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xD2:
    if (this->m_effectTimer >= EffectLifetime(0x2E3) - 1u) {
      this->m_effectTimer = 0;
    }
    break;

  case 0xD3:
    if (this->m_effectTimer >= EffectLifetime(0x2E4) - 1u) {
      this->m_effectTimer = 0;
    }
    break;

  case 0xD4:
    if (this->m_effectTimer >= EffectLifetime(0x2E5) - 1u) {
      this->m_effectTimer = 0;
    }
    break;

  case 0xD5:
    if (this->m_effectTimer >= EffectLifetime(0x2E6) - 1u) {
      this->m_effectTimer = 0;
    }
    break;

  case 0xD6:
  case 0xD7:
  case 0xD8:
  case 0xD9:
    if (this->m_effectTimer >= EffectLifetime(0x2E7) - 1u) {
      this->m_effectTimer = 0;
    }
    break;

  case 0xBE:
    if (this->m_effectTimer >= EffectLifetime(0x27E) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x27E, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xBF:
    if (this->m_effectTimer >= EffectLifetime(0x27F) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x27F, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xC0:
    if (this->m_effectTimer >= EffectLifetime(0x280) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x280, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xC1:
    if (this->m_effectTimer >= EffectLifetime(0x281) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x281, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xC2:
    if (this->m_effectTimer >= EffectLifetime(0x282) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x282, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0x108:
    if (this->m_effectTimer >= EffectLifetime(0x460) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x460, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xE6:
    if (this->m_effectTimer >= EffectLifetime(0x390) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x390, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xEE:
    if (!this->m_effect) {
      this->m_effect = this->LaunchEffect(0x3AB, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    if (this->m_effect) {
      this->m_effect->SetStateDuration(this->m_effect->GetDuration() + 21);
    }
    break;

  case 0xEF:
    if (this->m_effectTimer >= EffectLifetime(0x1D1) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x1D1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xF0:
    if (this->m_effectTimer >= EffectLifetime(0x440) - 1u) {
      this->m_effectTimer = 0;
      g_effectMgr->Spawn(this->render.accountId, 0x440, 0.0f, 0.0f, 0.0f, 0, -1, 0, 0);
    }
    break;

  case 0xF5:
    this->LaunchGroundEffects(0x3BF, 3, 0);
    break;

  case 0xF6:
    this->LaunchGroundEffects(0x3C4, 0, 0);
    break;

  case 0xF7:
    this->LaunchGroundEffects(0x3CF, 0, 0);
    break;

  case 0xE7:
  case 0xF1:
    if (this->m_effectTimer >= EffectLifetime(0x394) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x394, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\EF_FireWall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;

  case 0xF2:
    if (this->m_effectTimer >= EffectLifetime(0x394) - 1u) {
      this->m_effectTimer = 0;
      CRagEffect* effect = this->LaunchEffect(0x394, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\EF_FireWall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (effect) {
        effect->state.unknown_11C88[0] = 20;
        effect->state.unknown_11C88[1] = 20;
        effect->state.unknown_11C88[2] = 180;
      }
    }
    break;

  case 0xF3:
    if (this->m_effectTimer >= EffectLifetime(0x394) - 1u) {
      this->m_effectTimer = 0;
      CRagEffect* effect = this->LaunchEffect(0x394, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\EF_FireWall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (effect) {
        effect->state.unknown_11C88[0] = 20;
        effect->state.unknown_11C88[1] = 180;
        effect->state.unknown_11C88[2] = 20;
      }
    }
    break;

  case 0xF4:
    if (this->m_effectTimer >= EffectLifetime(0x394) - 1u) {
      this->m_effectTimer = 0;
      CRagEffect* effect = this->LaunchEffect(0x394, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\EF_FireWall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
      if (effect) {
        effect->state.unknown_11C88[0] = 40;
        effect->state.unknown_11C88[1] = 40;
        effect->state.unknown_11C88[2] = 40;
      }
    }
    break;

  case 0xE8:
    if (this->m_effectTimer >= EffectLifetime(0x395) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x395, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\EF_FireWall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;

  case 0xE9:
    if (this->m_effectTimer >= EffectLifetime(0x396) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x396, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\EF_FireWall.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;

  case 0xE4:
    if (this->m_effectTimer >= EffectLifetime(0x3A1) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x3A1, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      g_soundMgr->PlayWave("effect\\S워머.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    }
    break;

  case 0xF8:
    this->LaunchGroundEffects(0x3E0, 0, 0);
    break;

  case 0xF9:
    this->LaunchGroundEffects(0x3E1, 0, 0);
    break;

  case 0xFA:
    this->LaunchGroundEffects(0x3E2, 0, 0);
    break;

  case 0xFB:
    this->LaunchGroundEffects(0x3E3, 0, 0);
    break;

  case 0xFC:
    this->LaunchGroundEffects(0x3EF, 0, 0);
    break;

  case 0xFD:
    this->LaunchGroundEffects(0x3FC, 0, 0);
    break;

  case 0xFE:
    if (this->m_effectTimer >= EffectLifetime(0x402) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x402, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0xFF:
  case 0x100:
    if (this->m_effectTimer >= EffectLifetime(0x19) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x19, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0x102:
    if (this->m_effectTimer >= EffectLifetime(0x41B) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x41B, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0x101:
    if (this->m_effectTimer >= EffectLifetime(0x41A) - 1u) {
      this->m_effectTimer = 0;
      this->LaunchEffect(0x41A, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
    }
    break;

  case 0x105:
    if (this->m_effectTimer >= EffectLifetime(0x428) - 1u) {
      this->m_effectTimer = 0;
      g_effectMgr->Spawn(0, 0x428, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
    }
    break;

  case 0x103:
    if (!this->m_hasEffect) {
      this->m_hasEffect = true;
      g_effectMgr->Spawn(0, 0x4FE, this->render.position.x, this->render.position.y, this->render.position.z, 0, -1, 0, 0);
    }
    break;

  case 0x10C:
    if (!this->m_hasEffect) {
      this->m_hasEffect = true;
      this->LaunchEffect(this->m_effectId, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      this->m_effectId = 0x520;
    }
    break;
  default:
    break;
  }

  ++this->m_effectTimer;
  return true;
}

// OFFSET: 0x00E4F370
// FUNCTION: fn_24289
void CMsgEffect::FollowOwner() {
  CRenderObject* owner = static_cast<CRenderObject*>(this->m_owner);
  switch (this->m_type) {
    case 1:
    case 2:
    case 8:
    case 9:
    case 10:
    case 0x25:
    case 0x26:
      if (owner != nullptr) {
        this->render.position = owner->render.position;
        if (this->render.unknown_0070 == 10000) {
          this->render.position.y -= 10.0f;
        } else {
          this->render.position.y -= 20.0f;
        }
      }
      return;

    case 3:
      if (owner != nullptr) {
        CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
        const float dist = owner->CalcDist(this->render.position.x, this->render.position.z);
        const int dir = this->Get8Dir2(this->render.angle, 1);
        this->render.previousAnimation = this->render.animation + dir;
        if (dist > 5.0f) {
          const float oldX = this->render.position.x;
          const float oldZ = this->render.position.z;
          float dx = this->render.position.x - owner->render.position.x;
          float dz = this->render.position.z - owner->render.position.z;
          const float inv = 1.0f / SquareRoot(dx * dx + dz * dz);
          dx *= inv;
          dz *= inv;
          this->render.position.x = owner->render.position.x + dx * 5.0f;
          this->render.position.z = owner->render.position.z + dz * 5.0f;
          const float movedX = this->render.position.x - oldX;
          const float movedZ = this->render.position.z - oldZ;
          this->unknown_018C += SquareRoot(movedX * movedX + movedZ * movedZ);
          CActRes* action = this->GetActRes();
          const unsigned frames = action->FrameCount(this->render.previousAnimation);
          this->render.curMotion = static_cast<int>(this->unknown_018C * 0.23f * 4.0f / this->render.speed) % static_cast<int>(frames);
        }
        this->Face(owner->render.position.x, owner->render.position.z);
        this->render.position.y = scene->terrain->Grid().Height(this->render.position.x, this->render.position.z);
      }
      return;

    case 4:
    case 0xB:
      if (owner != nullptr) {
        this->render.position = owner->render.position;
        this->render.position.y -= 15.0f;
      }
      return;

    case 5:
      if (owner != nullptr) {
        this->render.position = owner->render.position;
      }
      return;

    case 7:
      if (owner != nullptr) {
        this->render.position = owner->render.position;
        this->render.position.y -= 15.0f;
        this->render.angle = owner->render.angle;
        const int dir = this->Get8Dir(this->render.angle);
        this->render.previousAnimation = this->render.animation + dir;
        if (static_cast<unsigned>(dir - 2) <= 3u) {
          this->render.unknown_0080 = 3;
        } else {
          this->render.unknown_0080 = static_cast<std::uint32_t>(-3);
        }
      }
      return;

    case 0xC:
      if (owner != nullptr) {
        this->render.position = owner->render.position;
        this->render.position.y -= 20.0f;
      }
      return;

    case 0xD:
    case 0xE:
    case 0xF:
    case 0x10:
    case 0x11:
    case 0x15:
    case 0x16:
    case 0x18:
    case 0x19:
      if (owner != nullptr) {
        this->render.position.x = owner->render.position.x;
        this->m_velocity.x = owner->render.position.x;
        this->render.position.z = owner->render.position.z;
        this->m_velocity.z = owner->render.position.z;
      }
      this->render.unknown_0124 = 0x200;
      return;

    case 0x12:
      if (owner == nullptr) {
        return;
      }
      if (this->unknown_01B4 != 0) {
        return;
      }
      {
        CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
        const float dist = owner->CalcDist(this->render.position.x, this->render.position.z);
        const int dir = this->Get8Dir2(this->render.angle, 1);
        this->render.previousAnimation = this->render.animation + dir;
        if (this->unknown_01B0 == 0) {
          if (dist <= 25.0f) {
            this->unknown_01B0 = 1;
          }
          const float oldX = this->render.position.x;
          const float oldZ = this->render.position.z;
          float dx = this->render.position.x - owner->render.position.x;
          float dz = this->render.position.z - owner->render.position.z;
          const float inv = 1.0f / SquareRoot(dx * dx + dz * dz);
          dx *= inv;
          dz *= inv;
          this->render.position.x += (owner->render.position.x - this->render.position.x) / 25.0f;
          this->render.curMotion = 8;
          this->render.position.z += (owner->render.position.z - this->render.position.z) / 25.0f;
          const float movedX = this->render.position.x - oldX;
          const float movedZ = this->render.position.z - oldZ;
          this->unknown_018C += SquareRoot(movedX * movedX + movedZ * movedZ);
        } else if (dist > 12.0f && this->unknown_01B4 == 0) {
          const float oldX = this->render.position.x;
          const float oldZ = this->render.position.z;
          float dx = this->render.position.x - owner->render.position.x;
          float dz = this->render.position.z - owner->render.position.z;
          const float inv = 1.0f / SquareRoot(dx * dx + dz * dz);
          dx *= inv;
          dz *= inv;
          this->render.position.x += (owner->render.position.x - this->render.position.x) / dist;
          this->render.position.z += (owner->render.position.z - this->render.position.z) / dist;
          const float movedX = this->render.position.x - oldX;
          const float movedZ = this->render.position.z - oldZ;
          this->unknown_018C += SquareRoot(movedX * movedX + movedZ * movedZ);
        } else if (dist > 3.0f) {
          const float oldX = this->render.position.x;
          const float oldZ = this->render.position.z;
          float dx = this->render.position.x - owner->render.position.x;
          float dz = this->render.position.z - owner->render.position.z;
          const float inv = 1.0f / SquareRoot(dx * dx + dz * dz);
          dx *= inv;
          dz *= inv;
          this->render.position.x += (owner->render.position.x - this->render.position.x) / 20.0f;
          this->render.position.z += (owner->render.position.z - this->render.position.z) / 20.0f;
          const float movedX = this->render.position.x - oldX;
          const float movedZ = this->render.position.z - oldZ;
          this->unknown_018C += SquareRoot(movedX * movedX + movedZ * movedZ);
        }
        this->Face(owner->render.position.x, owner->render.position.z);
        this->render.position.y = scene->terrain->Grid().Height(this->render.position.x, this->render.position.z) - 25.0f;
      }
      return;

    case 0x1A:
      if (owner == nullptr) {
        return;
      }
      this->render.position = owner->render.position;
      this->render.position.y -= 20.0f;
      if (this->m_type == 0x72) {
        this->render.position.x = static_cast<CRenderObject*>(this->m_owner)->render.position.x;
        this->render.position.z = static_cast<CRenderObject*>(this->m_owner)->render.position.z;
      }
      return;

    case 0x1B:
    case 0x1F:
      if (owner == nullptr) {
        return;
      }
      {
        CWorld* scene = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene;
        const float dist = owner->CalcDist(this->render.position.x, this->render.position.z);
        const int dir = this->Get8Dir2(this->render.angle, 1);
        if (this->unknown_01B4 == 1) {
          this->render.previousAnimation = this->render.animation + dir + 12 + ((dir + 4 < 8) ? 8 : 0);
        } else if (dist > 15.0f) {
          const float oldX = this->render.position.x;
          const float oldZ = this->render.position.z;
          this->render.previousAnimation = this->render.animation + 8 + dir;
          float dx = this->render.position.x - owner->render.position.x;
          float dz = this->render.position.z - owner->render.position.z;
          const float inv = 1.0f / SquareRoot(dx * dx + dz * dz);
          dx *= inv;
          dz *= inv;
          float scale = dist - 0.7f;
          if (scale <= 15.0f) {
            scale = 14.9f;
          }
          this->render.position.x = owner->render.position.x + dx * scale;
          this->render.position.z = owner->render.position.z + dz * scale;
          const float movedX = this->render.position.x - oldX;
          const float movedZ = this->render.position.z - oldZ;
          this->unknown_018C += SquareRoot(movedX * movedX + movedZ * movedZ);
          if (owner->render.unknown_0070 == 0 && dist < 16.0f) {
            this->render.previousAnimation = this->render.animation + dir;
          }
        } else if (owner->render.unknown_0070 == 4) {
          this->render.previousAnimation = this->render.animation + 0x18 + dir;
        } else if (owner->render.unknown_0070 == 3) {
          this->render.previousAnimation = this->render.animation + 0x20 + dir;
        } else if (owner->render.unknown_0070 == 0 || dist <= 14.0f) {
          this->render.previousAnimation = this->render.animation + dir;
        }
        this->Face(owner->render.position.x, owner->render.position.z);
        this->render.position.y = scene->terrain->Grid().Height(this->render.position.x, this->render.position.z);
      }
      return;

    case 0x1C:
    case 0x1D:
    case 0x1E:
    case 0x22:
    case 0x23:
    case 0x24:
      if (owner != nullptr) {
        this->render.position = owner->render.position;
        float yOff = 20.0f;
        if (this->render.unknown_0070 == 10000) {
          yOff = 10.0f;
        }
        this->render.position.y -= yOff;
        this->render.position.x -= 3.0f;
      }
      return;

    case 0x20:
      if (owner != nullptr) {
        this->render.position = owner->render.position;
        this->render.position.y -= 25.0f;
      }
      return;

    case 0x21:
      if (owner != nullptr) {
        this->render.position = owner->render.position;
        this->render.position.y -= static_cast<float>(static_cast<CGameActor*>(this->m_owner)->unknown_03C8) + 10.0f;
      }
      return;

    default:
      if (this->m_type == 0x72) {
        this->render.position.x = static_cast<CRenderObject*>(this->m_owner)->render.position.x;
        this->render.position.z = static_cast<CRenderObject*>(this->m_owner)->render.position.z;
      }
      return;
  }
}

// OFFSET: 0x00E4F2D0
// FUNCTION: fn_24288
bool CSummonActorEffect::OnProcess() {
  const bool result = this->CMsgEffect::OnProcess();
  const unsigned char* color = reinterpret_cast<const unsigned char*>(&this->render.color);
  this->SetColor(g_serviceOptions.Get(0x10A) != 0 ? 0x32 : 0xFF, color[2], color[1], color[0]);
  return result;
}

// OFFSET: 0x00E50340
// FUNCTION: fn_24293
void CMsgEffect::Render(int argument) {
  switch (this->m_type) {
    case 13:
    case 14:
    case 15:
    case 16:
    case 21:
    case 22:
    case 114:
      if (!g_msgEffectShowSpecial) {
        return;
      }
      break;
  }

  if (this->m_visible) {
    this->CRenderObject::Render(argument);
  }
}

// OFFSET: 0x00E512B0
// FUNCTION: fn_24301
void CMsgEffect::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int) {
  const vector3d* position = reinterpret_cast<const vector3d*>(b);
  switch (static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) {
    case 14: {
      this->render.visible = 1;
      this->render.unknown_008C = timeGetTime();
      if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
        const std::string* names = reinterpret_cast<const std::string*>(d);
        this->SetSpriteName(ResourceMgr::Instance(), names[0].c_str());
        this->SetActionName(ResourceMgr::Instance(), names[1].c_str());
      }

      if ((static_cast<std::int64_t>(g) << 32 | static_cast<std::uint32_t>(f)) == 1000) {
        this->SetAction(10, 4, 0);
        this->render.unknown_0070 = 10000;
        this->render.color = 0xFFFF0000;
      } else {
        this->SetAction(f, 4, 1);
      }

      switch (this->m_type) {
        case 3:
        case 27:
        case 31:
          break;

        case 6:
          this->render.position = *position;
          break;

        case 11:
        case 12:
          this->render.curMotion = 1;
          this->render.visible = 0;
          break;

        case 13:
          this->render.position = *position;
          this->m_baseX = this->render.position.x;
          this->render.position.y -= 12.0f;
          this->render.curMotion = 0;
          this->render.scale = 5.0f;
          this->m_scale = 5.0f;
          this->m_baseY = this->render.position.y;
          this->render.unknown_0080 = static_cast<std::uint32_t>(-5);
          break;

        case 15:
        case 25:
          this->render.position = *position;
          this->render.position.y -= 12.0f;
          this->render.curMotion = 1;
          this->render.scale = 3.0f;
          this->m_scale = 3.0f;
          this->render.unknown_0080 = static_cast<std::uint32_t>(-5);
          this->m_baseY = this->render.position.y;
          this->render.color = 0xFFAAAAAA;
          break;

        case 19:
          this->render.position = *position;
          this->render.position.y -= 12.0f;
          this->render.curMotion = 0;
          this->render.unknown_0080 = static_cast<std::uint32_t>(-5);
          this->m_baseY = this->render.position.y;
          break;

        case 24:
          this->render.position = *position;
          this->render.position.y -= 2.0f;
          this->render.curMotion = 0;
          this->render.scale = 1.0f;
          this->m_scale = 1.0f;
          this->render.unknown_0080 = static_cast<std::uint32_t>(-5);
          this->m_baseY = this->render.position.y;
          break;

        default:
          this->render.position = *position;
          this->render.position.y -= 20.0f;
          break;
      }
      return;
    }

    case 22:
      this->m_type = d;
      this->m_value = b;
      switch (d) {
        case 3:
        case 31:
          this->render.unknown_00A2 = 0;
          this->render.unknown_0080 = 0;
          this->render.unknown_00A4Bytes[0] = 1;
          this->render.unknown_0054 = 1.0f;
          break;

        case 4:
          this->SetColor(0x7F, 0xFF, 0, 0);
          break;

        case 7:
          this->SetColor(0x7F, -1, -1, -1);
          this->render.unknown_00A2 = 0;
          this->render.unknown_0080 = 5;
          break;

        case 5:
        case 6:
          this->render.unknown_00A2 = 0;
          this->render.unknown_0080 = 5;
          break;

        case 18:
          this->render.unknown_00A2 = 0;
          this->render.unknown_0080 = 0;
          break;

        case 27:
          this->render.unknown_00A2 = 0;
          this->render.unknown_0080 = 0;
          this->render.unknown_00A4Bytes[0] = 1;
          this->render.unknown_0054 = 2.0f;
          break;
      }
      return;

    case 29:
      if (this->m_type == 18) {
        this->m_velocity.x = (position->x - this->render.position.x) / 10.0f;
        this->m_velocity.y = (position->y - this->render.position.y) / 10.0f;
        this->unknown_01B4 = 1;
        this->unknown_01B0 = 0;
        this->m_velocity.z = (position->z - this->render.position.z) / 10.0f;
        return;
      }

      if (this->m_type == 27 || this->m_type == 31) {
        this->m_velocity.x = (position->x - this->render.position.x) / 10.0f;
        this->m_velocity.y = (position->y - this->render.position.y) / 10.0f;
        this->unknown_01B4 = 1;
        this->unknown_01A0 = 0;
        this->m_velocity.z = (position->z - this->render.position.z) / 10.0f;
        return;
      }
      break;

    case 49:
      this->m_owner = nullptr;
      switch (this->m_type) {
        case 6:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 21:
        case 22:
        case 25:
          break;

        default:
          this->m_finished = 1;
          break;
      }
      return;

    case 50:
      this->m_owner = sender;
      switch (this->m_type) {
        case 3:
        case 18:
        case 27:
        case 31:
          if (sender != nullptr) {
            const CRenderObject* owner = static_cast<const CRenderObject*>(sender);
            this->render.position.x = owner->render.position.x - ClientSine(static_cast<int>(owner->render.angle)) * 5.0f;
            owner = static_cast<const CRenderObject*>(this->m_owner);
            this->render.position.z = ClientCosine(static_cast<int>(owner->render.angle)) * 5.0f + owner->render.position.z;
          }
          break;

        case 6:
          if (sender != nullptr) {
            this->render.position = static_cast<const CRenderObject*>(sender)->render.position;
          }
          break;
      }
      return;

    case 53:
      this->m_finished = 1;
      return;

    case 64: {
      int offset = 0;
      switch (this->m_type) {
        case 13:
          this->m_baseX = this->render.position.x;
          return;

        case 22:
          offset = 15;
        case 14:
        case 16:
        case 21:
          this->SetSpriteName(ResourceMgr::Instance(), FormatOwnedString("%s.spr", g_damageResourceName.c_str()).c_str());
          this->SetActionName(ResourceMgr::Instance(), FormatOwnedString("%s.act", g_damageResourceName.c_str()).c_str());
          this->render.animation = 0;
          this->render.previousAnimation = 0;
          this->render.position = *position;
          this->render.position.y -= static_cast<float>(offset + 12);
          this->m_baseY = this->render.position.y;
          break;

        case 15:
        case 17:
        case 18:
        case 19:
        case 20:
          return;

        default:
          if (this->m_type != 114) {
            return;
          }

          this->SetSpriteName(ResourceMgr::Instance(), FormatOwnedString("%s.spr", g_damageResourceName.c_str()).c_str());
          this->SetActionName(ResourceMgr::Instance(), FormatOwnedString("%s.act", g_damageResourceName.c_str()).c_str());
          this->render.animation = 0;
          this->render.previousAnimation = 0;
          this->render.position = *position;
          this->m_baseX = this->render.position.x;
          this->render.position.y -= 12.0f;
          this->m_baseY = this->render.position.y;
          break;
      }

      this->render.curMotion = d;
      this->render.visible = 1;
      if (d < 0 || d > 11) {
        this->render.curMotion = 0;
      }

      this->render.unknown_0078 = f;
      this->m_digit = f;
      this->render.scale = 5.0f;
      this->m_scale = 5.0f;
      this->render.unknown_0080 = static_cast<std::uint32_t>(-2);
      this->render.unknown_008C = timeGetTime();
      return;
    }
  }

  this->EffectActor::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
}
