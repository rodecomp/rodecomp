#include "Contents/Actors/AttackMotion.h"
#include "Contents/Items/WeaponType.h"
#include "Contents/Jobs/JobId.h"

// OFFSET: 0x00DDFB50
// FUNCTION: fn_23444
float AttackMotionLength(int job, int appearance, int equipment, int attackMode) {
  if (attackMode == 0) {
    if (job == JT_MERCHANT) {
      return 5.85f;
    }

    if (job == JT_THIEF) {
      return 5.75f;
    }

    return 6.0f;
  }

  if (attackMode != 1) {
    return 6.0f;
  }

  switch (job) {
  case JT_NOVICE:
  case JT_SUPERNOVICE:
  case JT_SUPERNOVICE_B:
  case JT_SUPERNOVICE2:
  case JT_SUPERNOVICE2_B:
    return appearance == 1 ? 5.85f : 6.0f;
  case JT_ASSASSIN:
  case JT_ASSASSIN_H:
  case JT_ASSASSIN_B:
  case JT_GUILLOTINE_CROSS:
  case JT_GUILLOTINE_CROSS_H:
  case JT_GUILLOTINE_CROSS_B:
  case JT_SHADOW_CROSS:
    if (equipment == W_KATAR || (equipment >= W_DOUBLE_DD && equipment <= W_DOUBLE_SA)) {
      return 3.0f;
    }

    return 6.0f;
  default:
    return 6.0f;
  }
}
