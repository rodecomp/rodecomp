#include "Contents/Effects/Skills/ArrowEffect.h"
#include "Contents/Jobs/JobId.h"

#include "Client/Mode/ModeMgr.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00E4A1A0
// FUNCTION: fn_24250
CArrowEffect::CArrowEffect() : startPos({0.0f, 0.0f, 0.0f}), velocity({0.0f, 0.0f, 0.0f}), targetPos({0.0f, 0.0f, 0.0f}) {
  this->Reset();
}

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
__declspec(safebuffers) bool CArrowEffect::OnProcess() {
  float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.motionStartTime) / 24.0);
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

  this->render.spriteAngle = this->GetDir(this->render.angle);
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
  if (message == 14 && a == 0) {
    const char* sprite;
    const char* action;
    switch (static_cast<std::int64_t>(static_cast<std::uint64_t>(static_cast<std::uint32_t>(i)) << 32 | static_cast<std::uint32_t>(h))) {
      case JT_PARASITE:
      case JT_G_PARASITE:
        action = "몬스터\\PARASITE_BULLET.act";
        sprite = "몬스터\\PARASITE_BULLET.spr";
        break;

      case JT_LIVE_PEACH_TREE:
      case JT_G_LIVE_PEACH_TREE:
        action = "몬스터\\PEACH_TREE_BULLET.act";
        sprite = "몬스터\\PEACH_TREE_BULLET.spr";
        break;

      case JT_EVIL_CLOUD_HERMIT:
      case JT_G_EVIL_CLOUD_HERMIT:
        action = "몬스터\\GINSENG_BULLET.act";
        sprite = "몬스터\\GINSENG_BULLET.spr";
        break;

      case JT_STONE_SHOOTER:
      case JT_ILL_STONE_SHOOTER:
        action = "몬스터\\STONE_SHOOTER_BULLET.act";
        sprite = "몬스터\\STONE_SHOOTER_BULLET.spr";
        break;

      case JT_WOOTAN_SHOOTER:
      case JT_ILL_WOOTAN_SHOOTER:
      case JT_G_ILL_WOOTAN_SHOOTER:
        action = "몬스터\\WOOTAN_SHOOTER_BULLET.act";
        sprite = "몬스터\\WOOTAN_SHOOTER_BULLET.spr";
        break;

      case JT_POTON_CANON:
        action = "몬스터\\CANON_BULLET.act";
        sprite = "몬스터\\CANON_BULLET.spr";
        break;

      case JT_POTON_CANON_1:
        action = "몬스터\\CANON_BULLET_1.act";
        sprite = "몬스터\\CANON_BULLET_1.spr";
        break;

      case JT_POTON_CANON_2:
        action = "몬스터\\CANON_BULLET_2.act";
        sprite = "몬스터\\CANON_BULLET_2.spr";
        break;

      case JT_POTON_CANON_3:
        action = "몬스터\\CANON_BULLET_3.act";
        sprite = "몬스터\\CANON_BULLET_3.spr";
        break;

      case JT_LADY_TANEE:
        action = "몬스터\\LADY_TANEE_BULLET.act";
        sprite = "몬스터\\LADY_TANEE_BULLET.spr";
        break;

      case JT_RAGGED_ZOMBIE:
        action = "몬스터\\RAGGED_ZOMBIE_BULLET.act";
        sprite = "몬스터\\RAGGED_ZOMBIE_BULLET.spr";
        break;

      case JT_PHYLLA:
        action = "몬스터\\LADY_TANEE_BULLET.act";
        sprite = "몬스터\\LADY_TANEE_BULLET.spr";
        break;

      case JT_PETAL:
        action = "몬스터\\PETAL_BULLET.act";
        sprite = "몬스터\\PETAL_BULLET.spr";
        break;

      case JT_MG_CORRUPTION_ROOT:
        action = "몬스터\\MG_CORRUPTION_ROOT_BULLET.act";
        sprite = "몬스터\\MG_CORRUPTION_ROOT_BULLET.spr";
        break;

      case JT_E_MD_NYAILO:
        action = "몬스터\\e_nyailo_ammo.act";
        sprite = "몬스터\\e_nyailo_ammo.spr";
        break;

      case JT_TRIBE_DORAM:
      case JT_DO_SUMMONER:
      case JT_DO_CART_SUMMONER:
      case JT_DO_SUMMONER_B:
      case JT_DO_CART_SUMMONER_B:
      case JT_SPIRIT_HANDLER:
      case JT_SPIRIT_HANDLER_RIDING:
        action = "몬스터\\su_soulattack.act";
        sprite = "몬스터\\su_soulattack.spr";
        break;

      case JT_MD_C_CORRUPTION_ROOT:
        action = "몬스터\\mg_corruption_root_bullet.act";
        sprite = "몬스터\\mg_corruption_root_bullet.spr";
        break;

      case JT_SUMMON_HELLTREE:
        action = "몬스터\\SUMMON_HELLTREE_FRUIT.act";
        sprite = "몬스터\\SUMMON_HELLTREE_FRUIT.spr";
        break;

      case JT_EP19_LIMACINA:
      case JT_G_EP19_LIMACINA:
        action = "몬스터\\EP19_LIMACINA_BULLET.act";
        sprite = "몬스터\\EP19_LIMACINA_BULLET.spr";
        break;

      case JT_EP20_MELIBE_ICESLUG:
        action = "몬스터\\phylla_bullet.act";
        sprite = "몬스터\\phylla_bullet.spr";
        break;

      case JT_EP20_RGAN_WARLOCK:
      case JT_G_EP20_RGAN_WARLOCK:
        action = "몬스터\\canon_bullet_3.act";
        sprite = "몬스터\\canon_bullet_3.spr";
        break;

      case JT_MD_PRI_RIGEL_AC:
      case JT_MD_PRI_RIGEL_ACS:
        action = "몬스터\\RIGEL_BULLET.act";
        sprite = "몬스터\\RIGEL_BULLET.spr";
        break;

      case JT_RAYDRIC_ARCHER_H:
      case JT_MD_C_RAYDRIC_ARCHER:
      case JT_MD_C_RAY_ARCHER_G:
        action = "몬스터\\skel_archer_arrow.act";
        sprite = "몬스터\\skel_archer_arrow.spr";
        break;

      default:
        action = "몬스터\\skel_archer_arrow.act";
        sprite = "몬스터\\skel_archer_arrow.spr";
        break;
    }

    this->SetSpriteName(sprite);
    this->SetActionName(action);
    this->speedScale = 1.0f;
    this->render.position = *reinterpret_cast<const vector3d*>(b);
    this->render.position.y -= 10.0f;
    this->startPos.x = this->render.position.x;
    this->startPos.y = this->render.position.y;
    this->startPos.z = this->render.position.z;
    this->targetAid = d;
    this->render.visible = 0;
    this->GetTargetPos(d, this->targetPos);
    this->render.motionStartTime = timeGetTime();
  } else if (message == 64 && a == 0) {
    this->SetSpriteName("몬스터\\skel_archer_arrow.spr");
    this->SetActionName("몬스터\\skel_archer_arrow.act");
    this->speedScale = 1.0f;
    this->render.position = *reinterpret_cast<const vector3d*>(b);
    this->render.position.y -= 10.0f;
    this->startPos.x = this->render.position.x;
    this->startPos.y = this->render.position.y;
    this->startPos.z = this->render.position.z;
    this->targetPos = *reinterpret_cast<const vector3d*>(d);
    this->targetPos.y -= 10.0f;
    this->render.visible = 0;
    this->targetAid = 0;
    this->render.motionStartTime = timeGetTime();
  } else {
    this->CRenderObject::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  if ((f | g) != 0) {
    this->startFrame = *reinterpret_cast<const float*>(f);
  }
}
