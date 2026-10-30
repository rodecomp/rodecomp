#include "Contents/Effects/ArrowEffect.h"

#include "Client/Mode/ModeMgr.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00E4BA20
// FUNCTION: fn_24277
void CArrowEffect::Reset() {
  this->render.animation = 0;
  this->startPos = this->render.position;
  this->startFrame = 0.0f;
  this->velocity.x = 0.0f;
  this->velocity.y = 0.0f;
  this->velocity.z = 0.0f;
  this->render.curMotion = 0;
  this->frameCount = 0;
}

// OFFSET: 0x00E4BE60
// FUNCTION: fn_24280
bool CArrowEffect::OnProcess() {
  float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.unknown_008C) / 24.0);
  if (this->startFrame > frame) {
    return true;
  }

  if (this->render.visible == 0) {
    this->render.visible = 1;

    vector3d target;
    target.x = 0.0f;
    target.y = 0.0f;
    target.z = 0.0f;
    if (this->targetAid == 0 || !this->GetTargetPos(this->targetAid, target)) {
      target.z = this->targetPos.z;
      target.x = this->targetPos.x;
      target.y = this->targetPos.y;
    }

    target.y -= 10.0f;
    this->velocity.z = target.z - this->render.position.z;
    this->velocity.y = target.y - this->render.position.y;
    this->velocity.x = (target.x - this->render.position.x) * 0.125f;
    this->velocity.y *= 0.125f;
    this->velocity.z *= 0.125f;

    float distance = this->CalcDist(target.x, target.y, target.z);
    if (distance < 40.0f) {
      this->speedScale = 40.0f / distance;
      this->velocity.x = this->speedScale * this->velocity.x;
      this->velocity.y = this->speedScale * this->velocity.y;
      this->velocity.z = this->speedScale * this->velocity.z;
    }

    this->Face(target.x, target.z);

    float dy = target.y - this->render.position.y;
    float dx = target.x - this->render.position.x;
    float dz = target.z - this->render.position.z;
    float inverse = 1.0f / SquareRoot(dy * dy + dx * dx + dz * dz);
    this->render.position.x += inverse * dx * 2.5f;
    this->render.position.y += inverse * dy * 2.5f;
    this->render.position.z += inverse * dz * 2.5f;
  }

  frame -= this->startFrame;
  ++this->frameCount;
  this->render.previousAnimation = this->render.animation;
  if (this->frameCount > 8) {
    ++this->render.curMotion;
    if (this->render.curMotion >= static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation))) {
      this->render.curMotion = static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation)) - 1;
    }
  }

  this->render.unknown_007C = this->GetDir(this->render.angle);
  g_modeMgr.GetGameMode();
  this->render.position.x = frame * this->velocity.x + this->startPos.x;
  this->render.position.y = frame * this->velocity.y + this->startPos.y;
  this->render.position.z = frame * this->velocity.z + this->startPos.z;
  if (frame > 8.0f / this->speedScale) {
    return false;
  }

  return true;
}

// OFFSET: 0x00E50500
// FUNCTION: fn_24295
void CArrowEffect::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int) {
  const std::int64_t id = static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message);
  if (id == 14) {
    const char* sprite;
    const char* action;
    switch (static_cast<std::int64_t>(i) << 32 | static_cast<std::uint32_t>(h)) {
      case 0x582:
      case 0x60E:
        sprite = "몬스터\\PEACH_TREE_BULLET.spr";
        action = "몬스터\\PEACH_TREE_BULLET.act";
        break;

      case 0x584:
      case 0x5FB:
        sprite = "몬스터\\GINSENG_BULLET.spr";
        action = "몬스터\\GINSENG_BULLET.act";
        break;

      case 0x5D7:
      case 0x4F32:
        sprite = "몬스터\\STONE_SHOOTER_BULLET.spr";
        action = "몬스터\\STONE_SHOOTER_BULLET.act";
        break;

      case 0x5DA:
      case 0x4F33:
      case 0x4F37:
        sprite = "몬스터\\WOOTAN_SHOOTER_BULLET.spr";
        action = "몬스터\\WOOTAN_SHOOTER_BULLET.act";
        break;

      case 0x5DC:
      case 0x613:
        sprite = "몬스터\\PARASITE_BULLET.spr";
        action = "몬스터\\PARASITE_BULLET.act";
        break;

      case 0x680:
        sprite = "몬스터\\CANON_BULLET.spr";
        action = "몬스터\\CANON_BULLET.act";
        break;

      case 0x681:
        sprite = "몬스터\\CANON_BULLET_1.spr";
        action = "몬스터\\CANON_BULLET_1.act";
        break;

      case 0x682:
        sprite = "몬스터\\CANON_BULLET_2.spr";
        action = "몬스터\\CANON_BULLET_2.act";
        break;

      case 0x683:
        sprite = "몬스터\\CANON_BULLET_3.spr";
        action = "몬스터\\CANON_BULLET_3.act";
        break;

      case 0x698:
      case 0x7E5:
        sprite = "몬스터\\LADY_TANEE_BULLET.spr";
        action = "몬스터\\LADY_TANEE_BULLET.act";
        break;

      case 0x749:
        sprite = "몬스터\\RAGGED_ZOMBIE_BULLET.spr";
        action = "몬스터\\RAGGED_ZOMBIE_BULLET.act";
        break;

      case 0x93C:
        sprite = "몬스터\\PETAL_BULLET.spr";
        action = "몬스터\\PETAL_BULLET.act";
        break;

      case 0x9AB:
        sprite = "몬스터\\MG_CORRUPTION_ROOT_BULLET.spr";
        action = "몬스터\\MG_CORRUPTION_ROOT_BULLET.act";
        break;

      case 0xF2A:
        sprite = "몬스터\\e_nyailo_ammo.spr";
        action = "몬스터\\e_nyailo_ammo.act";
        break;

      case 0x1079:
      case 0x107A:
      case 0x107B:
      case 0x107C:
      case 0x107D:
      case 0x10D4:
      case 0x10DB:
        sprite = "몬스터\\su_soulattack.spr";
        action = "몬스터\\su_soulattack.act";
        break;

      case 0x505F:
        sprite = "몬스터\\mg_corruption_root_bullet.spr";
        action = "몬스터\\mg_corruption_root_bullet.act";
        break;

      case 0x5173:
        sprite = "몬스터\\SUMMON_HELLTREE_FRUIT.spr";
        action = "몬스터\\SUMMON_HELLTREE_FRUIT.act";
        break;

      case 0x5410:
      case 0x5455:
        sprite = "몬스터\\EP19_LIMACINA_BULLET.spr";
        action = "몬스터\\EP19_LIMACINA_BULLET.act";
        break;

      case 0x55D2:
        sprite = "몬스터\\phylla_bullet.spr";
        action = "몬스터\\phylla_bullet.act";
        break;

      case 0x55E1:
      case 0x55F1:
        sprite = "몬스터\\canon_bullet_3.spr";
        action = "몬스터\\canon_bullet_3.act";
        break;

      case 0x569F:
      case 0x56A0:
        sprite = "몬스터\\RIGEL_BULLET.spr";
        action = "몬스터\\RIGEL_BULLET.act";
        break;

      default:
        sprite = "몬스터\\skel_archer_arrow.spr";
        action = "몬스터\\skel_archer_arrow.act";
        break;
    }

    this->SetSpriteName(ResourceMgr::Instance(), sprite);
    this->SetActionName(ResourceMgr::Instance(), action);
    this->speedScale = 1.0f;
    this->render.position = *reinterpret_cast<const vector3d*>(b);
    this->render.position.y -= 10.0f;
    this->startPos = this->render.position;
    this->targetAid = d;
    this->render.visible = 0;
    this->GetTargetPos(d, this->targetPos);
  } else if (id == 64) {
    this->SetSpriteName(ResourceMgr::Instance(), "몬스터\\skel_archer_arrow.spr");
    this->SetActionName(ResourceMgr::Instance(), "몬스터\\skel_archer_arrow.act");
    this->speedScale = 1.0f;
    this->render.position = *reinterpret_cast<const vector3d*>(b);
    this->render.position.y -= 10.0f;
    this->startPos = this->render.position;
    this->targetPos = *reinterpret_cast<const vector3d*>(d);
    this->targetPos.y -= 10.0f;
    this->render.visible = 0;
    this->targetAid = 0;
  } else {
    this->CRenderObject::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  this->render.unknown_008C = timeGetTime();
  if ((static_cast<std::int64_t>(g) << 32 | static_cast<std::uint32_t>(f)) != 0) {
    this->startFrame = *reinterpret_cast<const float*>(f);
  }
}
