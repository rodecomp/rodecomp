#include "Contents/Actors/AttackMotion.h"
#include "Contents/Items/WeaponType.h"
#include "Contents/Jobs/JobId.h"

// OFFSET: 0x00DDFB50
// FUNCTION: fn_23444
float AttackMotionLength(int job, int appearance, int equipment, int attackMode) {
  if (attackMode == 0) {
    if (job == JOB_MERCHANT) {
      return 5.85f;
    }

    if (job == JOB_THIEF) {
      return 5.75f;
    }

    return 6.0f;
  }

  if (attackMode != 1) {
    return 6.0f;
  }

  switch (job) {
  case JOB_NOVICE:
  case JOB_SUPER_NOVICE:
  case JOB_SUPER_BABY:
  case JOB_SUPER_NOVICE_E:
  case JOB_SUPER_BABY_E:
    return appearance == 1 ? 5.85f : 6.0f;
  case JOB_ASSASSIN:
  case JOB_ASSASSIN_CROSS:
  case JOB_BABY_ASSASSIN:
  case JOB_GUILLOTINE_CROSS:
  case JOB_GUILLOTINE_CROSS_T:
  case JOB_BABY_GUILLOTINE_CROSS:
  case JOB_SHADOW_CROSS:
    if (equipment == W_KATAR || (equipment >= W_DOUBLE_DD && equipment <= W_DOUBLE_SA)) {
      return 3.0f;
    }

    return 6.0f;
  default:
    return 6.0f;
  }
}
