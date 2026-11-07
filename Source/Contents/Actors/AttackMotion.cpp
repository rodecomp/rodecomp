#include "Contents/Actors/AttackMotion.h"
#include "Contents/Items/WeaponType.h"
#include "Contents/Jobs/JobId.h"

// OFFSET: 0x00DDFB50
// FUNCTION: fn_23444
float AttackMotionLength(int job, int appearance, int equipment, int attackMode) {
  switch (attackMode) {
  case 0:
    switch (job) {
    case JT_MERCHANT:
      return 5.85f;
    case JT_THIEF:
      return 5.75f;
    }
    break;
  case 1:
    switch (job) {
    case JT_NOVICE:
    case JT_SUPERNOVICE:
    case JT_SUPERNOVICE_B:
    case JT_SUPERNOVICE2:
    case JT_SUPERNOVICE2_B:
      if (appearance == 1) {
        return 5.85f;
      }
      break;
    case JT_ASSASSIN:
    case JT_ASSASSIN_H:
    case JT_ASSASSIN_B:
    case JT_GUILLOTINE_CROSS:
    case JT_GUILLOTINE_CROSS_H:
    case JT_GUILLOTINE_CROSS_B:
    case JT_SHADOW_CROSS:
      switch (equipment) {
      case W_KATAR:
      case W_DOUBLE_DD:
      case W_DOUBLE_SS:
      case W_DOUBLE_AA:
      case W_DOUBLE_DS:
      case W_DOUBLE_DA:
      case W_DOUBLE_SA:
        return 3.0f;
      }
      break;
    }
    break;
  }
  return 6.0f;
}
