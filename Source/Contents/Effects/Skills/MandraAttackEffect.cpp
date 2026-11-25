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
void CMandraAttackEffect::SendMsg(CGameObject* sender, std::int64_t message, std::int64_t b, std::int64_t c, std::int64_t d, std::int64_t e, std::int64_t f, std::int64_t g) {
  if (message != 14) {
    this->CRenderObject::SendMsg(sender, message, b, c, d, e, 0, 0);
    return;
  }

  const int job = static_cast<int>(d);
  const char* sprite;
  const char* action;
  switch (job) {
    case JT_HYDRA:
      action = "몬스터\\hydra_atk.act";
      sprite = "몬스터\\hydra_atk.spr";
      break;

    case JT_THA_ODIUM:
    case JT_G_THA_ODIUM:
      action = "몬스터\\tha_odium_ef.act";
      sprite = "몬스터\\tha_odium_ef.spr";
      break;

    case JT_DROSERA:
    case JT_DEDSERA:
      action = "몬스터\\drosera_bullet.act";
      sprite = "몬스터\\drosera_bullet.spr";
      break;

    case JT_MAVKA:
    case JT_G_MAVKA:
      action = "몬스터\\MAVKA_BULLET.act";
      sprite = "몬스터\\MAVKA_BULLET.spr";
      break;

    case JT_ENTWEIHEN:
    case JT_G_ENTWEIHEN_R:
    case JT_G_ENTWEIHEN_H:
    case JT_G_ENTWEIHEN_M:
    case JT_G_ENTWEIHEN_S:
      action = "몬스터\\ENTWEIHEN_BULLET.act";
      sprite = "몬스터\\ENTWEIHEN_BULLET.spr";
      break;

    case JT_NEPENTHES:
      action = "몬스터\\NEPENTHES_BULLET.act";
      sprite = "몬스터\\NEPENTHES_BULLET.spr";
      break;

    case 0xE94:
    case 0x500B:
      action = "몬스터\\JP_ABYSS_BOSS_1_atk.act";
      sprite = "몬스터\\JP_ABYSS_BOSS_1_atk.spr";
      break;

    case 0xECF:
      action = "몬스터\\jp_2017evt_atk.act";
      sprite = "몬스터\\jp_2017evt_atk.spr";
      break;

    case JT_SUMMON_CREEPER:
      action = "몬스터\\summon_creeper_bullet.act";
      sprite = "몬스터\\summon_creeper_bullet.spr";
      break;

    case JT_EP21_YORTUS_B:
      action = "몬스터\\EP21_YORTUS_Bullet.act";
      sprite = "몬스터\\EP21_YORTUS_Bullet.spr";
      break;

    case JT_EP21_B_YORTUS_B:
      action = "몬스터\\EP21_B_YORTUS_Bullet.act";
      sprite = "몬스터\\EP21_B_YORTUS_Bullet.spr";
      break;

    case JT_EP21_YORTUS_B_H:
      action = "몬스터\\EP21_YORTUS_Bullet.act";
      sprite = "몬스터\\EP21_YORTUS_Bullet.spr";
      break;

    case JT_EP21_B_YORTUS_B_H:
      action = "몬스터\\EP21_B_YORTUS_Bullet.act";
      sprite = "몬스터\\EP21_B_YORTUS_Bullet.spr";
      break;

    case JT_HEKA:
    case JT_CP_HEKA:
      action = "몬스터\\mg_corruption_root_h_bullet.act";
      sprite = "몬스터\\mg_corruption_root_h_bullet.spr";
      break;

    case JT_MEDJED:
    case JT_CP_MEDJED:
      action = "몬스터\\MEDJED_bullet.act";
      sprite = "몬스터\\MEDJED_bullet.spr";
      break;

    case JT_GUIDE_OF_DEAD:
      action = "몬스터\\GUIDE_OF_DEAD_bullet.act";
      sprite = "몬스터\\GUIDE_OF_DEAD_bullet.spr";
      break;

    case JT_E_SMART_ANDRE:
      action = "몬스터\\E_SMART_ANDRE_Bullet.act";
      sprite = "몬스터\\E_SMART_ANDRE_Bullet.spr";
      break;

    case JT_CH1_SHADIEST:
    case JT_CP_CH1_SHADIEST:
      action = "몬스터\\CH1_SHADIEST_bullet.act";
      sprite = "몬스터\\CH1_SHADIEST_bullet.spr";
      break;

    case JT_CH1_FIRE_FLOWER:
      action = "몬스터\\CH1_FIRE_FLOWER_bullet.act";
      sprite = "몬스터\\CH1_FIRE_FLOWER_bullet.spr";
      break;

    default:
      action = "몬스터\\mandragora_atk.act";
      sprite = "몬스터\\mandragora_atk.spr";
      break;
  }

  this->SetSpriteName(sprite);
  this->SetActionName(action);
  vector3d target{0.0f, 0.0f, 0.0f};
  this->GetTargetPos(static_cast<std::uint32_t>(b), target);
  this->render.position = target;
  this->render.visible = 1;
  this->render.motionStartTime = timeGetTime();
  this->SetAnimationSpeed(this->GetActRes()->Speed(0));
  if (job == JT_HYDRA) {
    this->SetAnimationSpeed(2.0f);
  }

  if (c != 0) {
    this->startFrame = *reinterpret_cast<const float*>(static_cast<std::intptr_t>(c));
  }
}
