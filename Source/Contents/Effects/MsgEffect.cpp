#include "Contents/Effects/MsgEffect.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/World/World.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/World/TerrainResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"

#include <cstdint>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24976
std::string g_damageResourceName;

// GLOBAL: gl_24975
bool g_msgEffectShowSpecial = true;

// OFFSET: 0x00E4A9B0
// FUNCTION: fn_24258
CMsgEffect::~CMsgEffect() {
  if (this->m_owner != nullptr) {
    this->m_owner->SendMsg(this, 48, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
  }
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
