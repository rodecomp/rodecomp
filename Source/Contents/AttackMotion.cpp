#include "Contents/AttackMotion.h"

// OFFSET: 0x00DDFB50
// FUNCTION: fn_23444
float AttackMotionLength(int job, int appearance, int equipment, int attackMode) {
  if (attackMode == 0) {
    if (job == 5) {
      return 5.85f;
    }

    if (job == 6) {
      return 5.75f;
    }

    return 6.0f;
  }

  if (attackMode != 1) {
    return 6.0f;
  }

  switch (job) {
  case 0:
  case 23:
  case 4045:
  case 4190:
  case 4191:
    return appearance == 1 ? 5.85f : 6.0f;
  case 12:
  case 4013:
  case 4035:
  case 4059:
  case 4065:
  case 4101:
  case 4254:
    if (equipment == 16 || (equipment >= 25 && equipment <= 30)) {
      return 3.0f;
    }

    return 6.0f;
  default:
    return 6.0f;
  }
}
