#include "Contents/Effects/MandraAttackEffect.h"

#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00E4C310
// FUNCTION: fn_24285
bool CMandraAttackEffect::OnProcess() {
  float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.unknown_008C) / 24.0);
  float startFrame = this->startFrame;
  if (startFrame > frame) {
    this->render.visible = 0;
    return true;
  }

  this->render.visible = 1;
  if (!this->render.isMotionFinished) {
    int frameCount = static_cast<int>(this->GetActRes()->FrameCount(this->render.previousAnimation));
    float elapsed = (frame - startFrame) / this->render.speed;
    this->render.curMotion = static_cast<int>(elapsed) % frameCount;
    if (elapsed / frameCount >= 1.0f) {
      this->render.isMotionFinished = true;
      this->render.curMotion = frameCount - 1;
    }
  }

  return !this->render.isMotionFinished;
}

// OFFSET: 0x00E50F30
// FUNCTION: fn_24300
void CMandraAttackEffect::SendMsg(CGameObject* sender, int message, int a, int b, int c, int d, int e, int f, int g, int h, int i, int, int, int, int) {
  if ((static_cast<std::int64_t>(a) << 32 | static_cast<std::uint32_t>(message)) != 14) {
    this->CRenderObject::SendMsg(sender, message, a, b, c, d, e, f, g, h, i, 0, 0, 0, 0);
    return;
  }

  const char* sprite;
  const char* action;
  switch (f) {
    case 0x42C:
      sprite = "몬스터\\hydra_atk.spr";
      action = "몬스터\\hydra_atk.act";
      break;

    case 0x6A8:
    case 0x6AD:
      sprite = "몬스터\\tha_odium_ef.spr";
      action = "몬스터\\tha_odium_ef.act";
      break;

    case 0x6F5:
    case 0x55BB:
      sprite = "몬스터\\drosera_bullet.spr";
      action = "몬스터\\drosera_bullet.act";
      break;

    case 0x75C:
    case 0x75E:
      sprite = "몬스터\\MAVKA_BULLET.spr";
      action = "몬스터\\MAVKA_BULLET.act";
      break;

    case 0x7A5:
    case 0x7A6:
    case 0x7A7:
    case 0x7A8:
    case 0x7A9:
      sprite = "몬스터\\ENTWEIHEN_BULLET.spr";
      action = "몬스터\\ENTWEIHEN_BULLET.act";
      break;

    case 0x7C4:
      sprite = "몬스터\\NEPENTHES_BULLET.spr";
      action = "몬스터\\NEPENTHES_BULLET.act";
      break;

    case 0xE94:
    case 0x500B:
      sprite = "몬스터\\JP_ABYSS_BOSS_1_atk.spr";
      action = "몬스터\\JP_ABYSS_BOSS_1_atk.act";
      break;

    case 0xECF:
      sprite = "몬스터\\jp_2017evt_atk.spr";
      action = "몬스터\\jp_2017evt_atk.act";
      break;

    case 0x5172:
      sprite = "몬스터\\summon_creeper_bullet.spr";
      action = "몬스터\\summon_creeper_bullet.act";
      break;

    case 0x5759:
    case 0x578D:
      sprite = "몬스터\\EP21_YORTUS_Bullet.spr";
      action = "몬스터\\EP21_YORTUS_Bullet.act";
      break;

    case 0x575C:
    case 0x5796:
      sprite = "몬스터\\EP21_B_YORTUS_Bullet.spr";
      action = "몬스터\\EP21_B_YORTUS_Bullet.act";
      break;

    case 0x577B:
    case 0x577C:
      sprite = "몬스터\\MEDJED_bullet.spr";
      action = "몬스터\\MEDJED_bullet.act";
      break;

    case 0x5781:
    case 0x5782:
      sprite = "몬스터\\mg_corruption_root_h_bullet.spr";
      action = "몬스터\\mg_corruption_root_h_bullet.act";
      break;

    case 0x5784:
      sprite = "몬스터\\GUIDE_OF_DEAD_bullet.spr";
      action = "몬스터\\GUIDE_OF_DEAD_bullet.act";
      break;

    case 0x5788:
      sprite = "몬스터\\E_SMART_ANDRE_Bullet.spr";
      action = "몬스터\\E_SMART_ANDRE_Bullet.act";
      break;

    case 0x57B2:
      sprite = "몬스터\\CH1_FIRE_FLOWER_bullet.spr";
      action = "몬스터\\CH1_FIRE_FLOWER_bullet.act";
      break;

    case 0x57D2:
    case 0x57D3:
      sprite = "몬스터\\CH1_SHADIEST_bullet.spr";
      action = "몬스터\\CH1_SHADIEST_bullet.act";
      break;

    default:
      sprite = "몬스터\\mandragora_atk.spr";
      action = "몬스터\\mandragora_atk.act";
      break;
  }

  this->SetSpriteName(ResourceMgr::Instance(), sprite);
  this->SetActionName(ResourceMgr::Instance(), action);
  vector3d target{0.0f, 0.0f, 0.0f};
  this->GetTargetPos(b, target);
  this->render.position.x = target.x;
  this->render.position.y = target.y;
  this->render.position.z = target.z;
  this->render.visible = 1;
  this->render.unknown_008C = timeGetTime();
  this->SetAnimationSpeed(this->GetActRes()->Speed(0));
  if (f == 0x42C) {
    this->SetAnimationSpeed(2.0f);
  }

  if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
    this->startFrame = *reinterpret_cast<const float*>(d);
  }
}
