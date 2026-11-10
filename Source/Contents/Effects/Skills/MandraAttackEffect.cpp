#include "Contents/Effects/Skills/MandraAttackEffect.h"
#include "Contents/Jobs/JobId.h"

#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00E4A4D0
// FUNCTION: fn_24254
CMandraAttackEffect::CMandraAttackEffect() {
  this->SetSpriteName("몬스터\\mandragora_atk.spr");
  this->SetActionName("몬스터\\mandragora_atk.act");
  this->startFrame = 0.0f;
}

// OFFSET: 0x00E4C310
// FUNCTION: fn_24285
bool CMandraAttackEffect::OnProcess() {
  float frame = static_cast<float>(static_cast<double>(timeGetTime() - this->render.motionStartTime) / 24.0);
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
    case JT_HYDRA:
      sprite = "몬스터\\hydra_atk.spr";
      action = "몬스터\\hydra_atk.act";
      break;

    case JT_THA_ODIUM:
    case JT_G_THA_ODIUM:
      sprite = "몬스터\\tha_odium_ef.spr";
      action = "몬스터\\tha_odium_ef.act";
      break;

    case JT_DROSERA:
    case JT_DEDSERA:
      sprite = "몬스터\\drosera_bullet.spr";
      action = "몬스터\\drosera_bullet.act";
      break;

    case JT_MAVKA:
    case JT_G_MAVKA:
      sprite = "몬스터\\MAVKA_BULLET.spr";
      action = "몬스터\\MAVKA_BULLET.act";
      break;

    case JT_ENTWEIHEN:
    case JT_G_ENTWEIHEN_R:
    case JT_G_ENTWEIHEN_H:
    case JT_G_ENTWEIHEN_M:
    case JT_G_ENTWEIHEN_S:
      sprite = "몬스터\\ENTWEIHEN_BULLET.spr";
      action = "몬스터\\ENTWEIHEN_BULLET.act";
      break;

    case JT_NEPENTHES:
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

    case JT_SUMMON_CREEPER:
      sprite = "몬스터\\summon_creeper_bullet.spr";
      action = "몬스터\\summon_creeper_bullet.act";
      break;

    case JT_EP21_YORTUS_B:
    case JT_EP21_YORTUS_B_H:
      sprite = "몬스터\\EP21_YORTUS_Bullet.spr";
      action = "몬스터\\EP21_YORTUS_Bullet.act";
      break;

    case JT_EP21_B_YORTUS_B:
    case JT_EP21_B_YORTUS_B_H:
      sprite = "몬스터\\EP21_B_YORTUS_Bullet.spr";
      action = "몬스터\\EP21_B_YORTUS_Bullet.act";
      break;

    case JT_MEDJED:
    case JT_CP_MEDJED:
      sprite = "몬스터\\MEDJED_bullet.spr";
      action = "몬스터\\MEDJED_bullet.act";
      break;

    case JT_HEKA:
    case JT_CP_HEKA:
      sprite = "몬스터\\mg_corruption_root_h_bullet.spr";
      action = "몬스터\\mg_corruption_root_h_bullet.act";
      break;

    case JT_GUIDE_OF_DEAD:
      sprite = "몬스터\\GUIDE_OF_DEAD_bullet.spr";
      action = "몬스터\\GUIDE_OF_DEAD_bullet.act";
      break;

    case JT_E_SMART_ANDRE:
      sprite = "몬스터\\E_SMART_ANDRE_Bullet.spr";
      action = "몬스터\\E_SMART_ANDRE_Bullet.act";
      break;

    case JT_CH1_FIRE_FLOWER:
      sprite = "몬스터\\CH1_FIRE_FLOWER_bullet.spr";
      action = "몬스터\\CH1_FIRE_FLOWER_bullet.act";
      break;

    case JT_CH1_SHADIEST:
    case JT_CP_CH1_SHADIEST:
      sprite = "몬스터\\CH1_SHADIEST_bullet.spr";
      action = "몬스터\\CH1_SHADIEST_bullet.act";
      break;

    default:
      sprite = "몬스터\\mandragora_atk.spr";
      action = "몬스터\\mandragora_atk.act";
      break;
  }

  this->SetSpriteName(sprite);
  this->SetActionName(action);
  vector3d target{0.0f, 0.0f, 0.0f};
  this->GetTargetPos(b, target);
  this->render.position.x = target.x;
  this->render.position.y = target.y;
  this->render.position.z = target.z;
  this->render.visible = 1;
  this->render.motionStartTime = timeGetTime();
  this->SetAnimationSpeed(this->GetActRes()->Speed(0));
  if (f == 0x42C) {
    this->SetAnimationSpeed(2.0f);
  }

  if ((static_cast<std::int64_t>(e) << 32 | static_cast<std::uint32_t>(d)) != 0) {
    this->startFrame = *reinterpret_cast<const float*>(d);
  }
}
