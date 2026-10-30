#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00BEBC80
// FUNCTION: fn_19972
void CEffectPrim::RenderGroundArcGlow(matrix& wtm) {
  vector3d prevB{};
  vector3d curB{};
  vector3d curT{};
  vector3d prevT{};
  int rpCnt = 0;
  short alpha;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int i = 0;
    for (int offset = 0; offset <= slot.fullDisplayAngle; offset += step) {
      int angle = (slot.rotStart + offset) % 360;
      if (slot.fullDisplayAngle == 360 && i == 20) {
        angle = slot.rotStart;
      }

      const float rise = slot.height[i];
      const float radius = g_sinTable[slot.rotStart] + slot.distance;
      const float riseX = rise * g_cosTable[slot.riseAngle];
      curB.x = radius * g_cosTable[angle];
      curB.y = 0.0f;
      curB.z = radius * g_sinTable[angle];
      curT.x = riseX * g_cosTable[angle] + curB.x;
      curT.y = -(rise * g_sinTable[slot.riseAngle]);
      curT.z = riseX * g_sinTable[angle] + curB.z;

      if (i > 0) {
        int texture;
        if (slot.alphaT == 10) {
          texture = slot.process / 3 % static_cast<int>(this->state.totalTexture);
        } else {
          texture = slot.process % static_cast<int>(this->state.totalTexture);
          this->state.size = 0.0f;
          this->RenderTeiRect(&rpCnt, i - 1, slot.alphaB, slot.alphaB, prevB, curB, curT, prevT, wtm, static_cast<char>(texture));
        }

        if (slot.alphaT <= 2) {
          this->state.size = 17.0f;
          alpha = slot.alphaB * 3;
        } else if (slot.alphaT == 3) {
          this->state.size = 18.0f;
          alpha = slot.alphaB * 3;
        } else if (slot.alphaT == 4) {
          this->state.size = 19.0f;
          alpha = slot.alphaB * 2;
        } else if (slot.alphaT == 5) {
          this->state.size = 20.0f;
          alpha = slot.alphaB * 2;
        } else if (slot.alphaT == 7) {
          this->state.size = 21.0f;
          alpha = slot.alphaB * 2;
        } else if (slot.alphaT == 8) {
          this->state.size = 22.0f;
          alpha = slot.alphaB * 2;
        } else if (slot.alphaT == 9) {
          this->state.size = 30.0f;
          alpha = slot.alphaB;
          if (!g_serviceOptions.Get(229)) {
            alpha = slot.alphaB * 2;
          }
        } else if (slot.alphaT == 6) {
          this->state.size = 5.0f;
          alpha = slot.alphaB * 2;
        } else if (slot.alphaT == 10) {
          this->state.size = 6.0f;
          alpha = slot.alphaB * 3;
        } else if (slot.alphaT == 11) {
          this->state.size = 32.0f;
          alpha = slot.alphaB;
          if (!g_serviceOptions.Get(229)) {
            alpha = slot.alphaB * 2;
          }
        }

        if (alpha > 255) {
          alpha = 255;
        }
        this->RenderTeiRect(&rpCnt, i - 1, alpha, slot.alphaB, prevB, curB, curT, prevT, wtm, static_cast<char>(texture));
      }

      ++i;
      if (i >= 21) {
        i = 20;
      }
      prevT = curT;
      prevB = curB;
    }
  }
}

// OFFSET: 0x00BEC150
// FUNCTION: fn_19973
void CEffectPrim::RenderGroundArc(matrix& wtm) {
  vector3d prevB{};
  vector3d curB{};
  vector3d curT{};
  vector3d prevT{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.process <= 0) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int i = 0;
    for (int offset = 0; offset <= slot.fullDisplayAngle; offset += step) {
      int angle = (slot.rotStart + offset) % 360;
      if (slot.fullDisplayAngle == 360 && i == 20) {
        angle = slot.rotStart;
      }

      const float rise = slot.height[i];
      const float riseX = rise * g_cosTable[slot.riseAngle];
      curB.x = slot.distance * g_cosTable[angle];
      curB.y = 5.0f;
      curB.z = slot.distance * g_sinTable[angle];
      curT.x = riseX * g_cosTable[angle] + curB.x;
      curT.z = riseX * g_sinTable[angle] + curB.z;
      curT.y = 5.0f - rise * g_sinTable[slot.riseAngle];

      if (i > 0) {
        this->RenderTeiRect(
          &rpCnt, i - 1, slot.alphaB, slot.alphaB, prevB, curB, curT, prevT, wtm,
          static_cast<char>(slot.process % static_cast<int>(this->state.totalTexture))
        );
      }

      ++i;
      if (i >= 21) {
        i = 20;
      }
      prevT = curT;
      prevB = curB;
    }
  }
}

// OFFSET: 0x00BEC3D0
// FUNCTION: fn_19974
void CEffectPrim::RenderGroundArc2(matrix& wtm) {
  vector3d prevB{};
  vector3d curB{};
  vector3d curT{};
  vector3d prevT{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.process <= 0) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int i = 0;
    for (int offset = 0; offset <= slot.fullDisplayAngle; offset += step) {
      int angle = (slot.rotStart + offset) % 360;
      if (slot.fullDisplayAngle == 360 && i == 20) {
        angle = slot.rotStart;
      }

      const float rise = slot.height[i];
      const float riseX = rise * g_cosTable[slot.riseAngle];
      curB.x = slot.distance * g_cosTable[angle];
      curB.y = 5.0f;
      curB.z = slot.distance * g_sinTable[angle];
      curT.x = riseX * g_cosTable[angle] + curB.x;
      curT.z = riseX * g_sinTable[angle] + curB.z;
      curT.y = 5.0f - rise * g_sinTable[slot.riseAngle];

      if (i > 0) {
        this->RenderTeiRect(
          &rpCnt, i - 1, slot.alphaB, slot.alphaB, prevB, curB, curT, prevT, wtm,
          static_cast<char>(slot.process % static_cast<int>(this->state.totalTexture))
        );
      }

      ++i;
      if (i >= 21) {
        i = 20;
      }
      prevT = curT;
      prevB = curB;
    }
  }
}

// OFFSET: 0x00BEC650
// FUNCTION: fn_19975
void CEffectPrim::RenderGroundArcStep(matrix& wtm) {
  vector3d prevB{};
  vector3d curB{};
  vector3d curT{};
  vector3d prevT{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int i = 0;
    for (int offset = 0; offset <= slot.fullDisplayAngle; offset += step) {
      int angle = (slot.rotStart + offset) % 360;
      if (slot.fullDisplayAngle == 360 && i == 20) {
        angle = slot.rotStart;
      }

      curB.x = slot.distance * g_cosTable[angle];
      curB.z = slot.distance * g_sinTable[angle];
      if (slot.maxHeight == 6.001f) {
        curB.y = -2.0f;
      } else if (slot.maxHeight == 24.001f) {
        curB.y = -1.0f;
      } else {
        curB.y = 0.0f;
      }

      const float rise = slot.height[i];
      const float riseX = rise * g_cosTable[slot.riseAngle];
      float riseY = rise * g_sinTable[slot.riseAngle];
      curT.x = riseX * g_cosTable[angle] + curB.x;
      curT.z = riseX * g_sinTable[angle] + curB.z;
      if (slot.maxHeight == 6.001f) {
        riseY += 2.0f;
      } else if (slot.maxHeight == 24.001f) {
        riseY += 1.0f;
      }
      curT.y = -riseY;

      if (i > 0) {
        this->RenderTeiRect(
          &rpCnt, i - 1, slot.alphaB, slot.alphaB, prevB, curB, curT, prevT, wtm,
          static_cast<char>(slot.process % static_cast<int>(this->state.totalTexture))
        );
      }

      ++i;
      if (i >= 21) {
        i = 20;
      }
      prevB = curB;
      prevT = curT;
    }
  }
}

// OFFSET: 0x00BECEB0
// FUNCTION: fn_19977
void CEffectPrim::RenderGroundSquare(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int angle0 = slot.rotStart;
    const double radius = (g_sinTable[slot.riseAngle] + 1.0f) * (slot.distance * 0.1f) + slot.distance * 0.8;
    const int angle1 = angle0 + (angle0 < 270 ? 90 : -270);
    const int angle2 = angle1 + (angle1 + 90 < 360 ? 90 : -270);
    const int angle3 = angle2 + (angle2 + 90 < 360 ? 90 : -270);

    slot.vecBPre.x = static_cast<float>(g_cosTable[angle0] * radius);
    slot.vecBPre.z = static_cast<float>(g_sinTable[angle0] * radius);
    slot.vecBPre.y = slot.height[0];
    slot.vecBNow.y = slot.height[0];
    slot.vecBNow.x = static_cast<float>(g_cosTable[angle1] * radius);
    slot.vecBNow.z = static_cast<float>(g_sinTable[angle1] * radius);
    slot.vecTNow.y = slot.height[0];
    slot.vecTNow.x = static_cast<float>(g_cosTable[angle2] * radius);
    slot.vecTNow.z = static_cast<float>(g_sinTable[angle2] * radius);
    slot.vecTPre.y = slot.height[0];
    slot.vecTPre.x = static_cast<float>(g_cosTable[angle3] * radius);
    slot.vecTPre.z = static_cast<float>(g_sinTable[angle3] * radius);

    this->RenderTeiRect(
      &rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, slot.height[2] == 1.0f,
      static_cast<char>(slot.height[1]), 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
    );
  }
}

// OFFSET: 0x00BED140
// FUNCTION: fn_19978
void CEffectPrim::RenderGroundSquareBlend(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int angle0 = slot.rotStart;
    const double radius = (g_sinTable[slot.riseAngle] + 1.0f) * (slot.distance * 0.1f) + slot.distance * 0.8;
    const int angle1 = angle0 + (angle0 < 270 ? 90 : -270);
    const int angle2 = angle1 + (angle1 + 90 < 360 ? 90 : -270);
    const int angle3 = angle2 + (angle2 + 90 < 360 ? 90 : -270);

    slot.vecBPre.x = static_cast<float>(g_cosTable[angle0] * radius);
    slot.vecBPre.z = static_cast<float>(g_sinTable[angle0] * radius);
    slot.vecBPre.y = slot.height[0];
    slot.vecBNow.y = slot.height[0];
    slot.vecBNow.x = static_cast<float>(g_cosTable[angle1] * radius);
    slot.vecBNow.z = static_cast<float>(g_sinTable[angle1] * radius);
    slot.vecTNow.y = slot.height[0];
    slot.vecTNow.x = static_cast<float>(g_cosTable[angle2] * radius);
    slot.vecTNow.z = static_cast<float>(g_sinTable[angle2] * radius);
    slot.vecTPre.y = slot.height[0];
    slot.vecTPre.x = static_cast<float>(g_cosTable[angle3] * radius);
    slot.vecTPre.z = static_cast<float>(g_sinTable[angle3] * radius);

    short alpha = static_cast<short>(slot.process % 256 * 2);
    if (alpha > 255) {
      alpha = 511 - alpha;
    }

    this->RenderTeiRect(
      &rpCnt, alpha, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f,
      1.0f, 0
    );
    this->RenderTeiRect(
      &rpCnt, 255 - alpha, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 1, 1, 0, 255, 255, 255, 0.0f, 1.0f,
      0.0f, 1.0f, 0
    );
  }
}

// OFFSET: 0x00BEFD80
// FUNCTION: fn_19983
void CEffectPrim::RenderFanWedge(matrix& wtm) {
  vector3d tip{};
  vector3d left{};
  vector3d right{};
  vector3d back{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const int angle = slot.riseAngle;
    const float dist = slot.distance;
    const float base = slot.height[0];
    const int leftAngle = angle + (angle >= 7 ? -7 : 353);
    const float rotSin = g_sinTable[slot.rotStart];
    const float rotCos = g_cosTable[slot.rotStart];
    const int rightAngle = angle + (angle < 353 ? 7 : -353);
    const int backAngle = angle + (angle >= 270 ? -270 : 90);
    left.x = dist * g_cosTable[leftAngle] * rotCos;
    left.y = dist * g_cosTable[leftAngle] * rotSin + base;
    left.z = dist * g_sinTable[leftAngle];
    right.x = dist * g_cosTable[rightAngle] * rotCos;
    right.y = dist * g_cosTable[rightAngle] * rotSin + base;
    right.z = dist * g_sinTable[rightAngle];
    back.x = g_cosTable[backAngle] * 7.0f * rotCos;
    back.y = g_cosTable[backAngle] * 7.0f * rotSin + base;
    back.z = g_sinTable[backAngle] * 7.0f;
    const int tipAngle = angle < 90 ? angle + 270 : angle - 90;
    tip.x = g_cosTable[tipAngle] * 7.0f * rotCos;
    tip.y = g_cosTable[tipAngle] * 7.0f * rotSin + base;
    tip.z = g_sinTable[tipAngle] * 7.0f;

    this->RenderTeiRect(
      &rpCnt, slot.alphaB, tip, left, right, back, wtm, 0, static_cast<unsigned char>(static_cast<int>(slot.height[1])), 0, 105,
      105, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
    );
  }
}

// OFFSET: 0x00BF1210
// FUNCTION: fn_19986
void CEffectPrim::RenderFanSlice(matrix& wtm) {
  vector3d left{};
  vector3d right{};
  vector3d center{};
  vector3d center2{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const int angle = slot.riseAngle;
    const float dist = slot.distance;
    const float base = slot.height[0];
    const int leftAngle = angle + (angle >= 5 ? -5 : 355);
    const int rightAngle = angle + (angle < 355 ? 5 : -355);
    left.x = dist * g_cosTable[leftAngle] * g_cosTable[slot.rotStart];
    left.y = dist * g_cosTable[leftAngle] * g_sinTable[slot.rotStart] + base;
    left.z = dist * g_sinTable[leftAngle];
    right.x = dist * g_cosTable[rightAngle] * g_cosTable[slot.rotStart];
    right.y = dist * g_cosTable[rightAngle] * g_sinTable[slot.rotStart] + base;
    right.z = dist * g_sinTable[rightAngle];
    center.x = 0.0f;
    center.y = base;
    center.z = 0.0f;
    center2.x = 0.0f;
    center2.y = base;
    center2.z = 0.0f;

    this->RenderTeiRect(&rpCnt, slot.alphaB, left, right, center, center2, wtm, 0, 0, 0, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
  }
}

// OFFSET: 0x00BF0010
// FUNCTION: fn_19984
void CEffectPrim::RenderFanTrail(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const int angle = slot.riseAngle;
    const float size = slot.maxHeight;
    const float base = slot.height[0];
    const int low = angle - 20;
    const int high = angle + 20;
    const int leftAngle = low < 0 ? low + 360 : low;
    const int rightAngle = high < 360 ? high : high - 360;
    const int backRightAngle = high - 180 < 0 ? high - 180 + 360 : high - 180;
    const int backLeftAngle = low + 180 < 360 ? low + 180 : low + 180 - 360;
    const float rotCos = g_cosTable[slot.rotStart];
    const float rotSin = g_sinTable[slot.rotStart];

    vector3d left;
    left.x = size * g_cosTable[leftAngle] * rotCos;
    left.y = size * g_cosTable[leftAngle] * rotSin + base;
    left.z = size * g_sinTable[leftAngle];
    vector3d right;
    right.x = size * g_cosTable[rightAngle] * rotCos;
    right.y = size * g_cosTable[rightAngle] * rotSin + base;
    right.z = size * g_sinTable[rightAngle];
    vector3d backRight;
    backRight.x = size * g_cosTable[backRightAngle] * rotCos;
    backRight.y = size * g_cosTable[backRightAngle] * rotSin + base;
    backRight.z = size * g_sinTable[backRightAngle];
    vector3d backLeft;
    backLeft.x = size * g_cosTable[backLeftAngle] * rotCos;
    backLeft.y = size * g_cosTable[backLeftAngle] * rotSin + base;
    backLeft.z = size * g_sinTable[backLeftAngle];

    const float dirCos = g_cosTable[angle];
    const float dirSin = g_sinTable[angle];

    for (short step = 0; step < 6; ++step) {
      const float dist = step * slot.maxHeight + slot.distance;
      vector3d offset;
      offset.x = dist * dirCos;
      offset.y = 0.0f;
      offset.z = dist * dirSin;
      if (dist > 100.0f) {
        break;
      }

      if (dist > 0.0f) {
        vector3d vec1;
        vec1.x = backRight.x + offset.x;
        vec1.y = backRight.y + offset.y;
        vec1.z = backRight.z + offset.z;
        vector3d vec2;
        vec2.x = left.x + offset.x;
        vec2.y = left.y + offset.y;
        vec2.z = left.z + offset.z;
        vector3d vec3;
        vec3.x = right.x + offset.x;
        vec3.y = right.y + offset.y;
        vec3.z = right.z + offset.z;
        vector3d vec4;
        vec4.x = backLeft.x + offset.x;
        vec4.y = backLeft.y + offset.y;
        vec4.z = backLeft.z + offset.z;
        this->RenderTeiRect(
          &rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, static_cast<unsigned char>(static_cast<int>(slot.height[1])), 0,
          105, 105, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0
        );
      }
    }
  }
}

// OFFSET: 0x00BEE900
// FUNCTION: fn_19980
void CEffectPrim::RenderTwinFacingSquares(matrix& wtm) {
  int rpCnt = 0;
  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life != 1 || slot.alphaB <= 0) {
    return;
  }

  vector3d vec1;
  vector3d vec2;
  vector3d vec3;
  vector3d vec4;
  vec1.y = 0.0f;
  vec2.y = 0.0f;
  vec3.y = 0.0f;
  vec4.y = 0.0f;

  float radius = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance;
  int angle = slot.process * 2 % 360;
  vec1.x = radius * g_cosTable[angle];
  vec1.z = radius * g_sinTable[angle];
  angle = (angle + 90) % 360;
  vec2.x = radius * g_cosTable[angle];
  vec2.z = radius * g_sinTable[angle];
  angle = (angle + 90) % 360;
  vec3.x = radius * g_cosTable[angle];
  vec3.z = radius * g_sinTable[angle];
  angle = (angle + 90) % 360;
  vec4.x = radius * g_cosTable[angle];
  vec4.z = radius * g_sinTable[angle];
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, slot.alphaT, this->state.slots[1].alphaT);

  vec1.x = slot.vecBNow.x + vec1.x;
  vec1.y = slot.vecBNow.y + vec1.y;
  vec1.z = slot.vecBNow.z + vec1.z;
  vec2.x = slot.vecBNow.x + vec2.x;
  vec2.y = slot.vecBNow.y + vec2.y;
  vec2.z = slot.vecBNow.z + vec2.z;
  vec3.x = slot.vecBNow.x + vec3.x;
  vec3.y = slot.vecBNow.y + vec3.y;
  vec3.z = slot.vecBNow.z + vec3.z;
  vec4.x = slot.vecBNow.x + vec4.x;
  vec4.y = slot.vecBNow.y + vec4.y;
  vec4.z = slot.vecBNow.z + vec4.z;

  if (slot.flag1[0] == 10) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 11) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 200, 200, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 12) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 13) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 100, 150, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }

  vec1.y = 0.0f;
  vec2.y = 0.0f;
  vec3.y = 0.0f;
  vec4.y = 0.0f;
  radius = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f +
           (g_sinTable[slot.process * 5 % 90] * 2.0f + 1.0f) * slot.distance;
  angle = slot.process * 2 % 360;
  vec1.x = radius * g_cosTable[angle];
  vec1.z = radius * g_sinTable[angle];
  angle = (angle + 90) % 360;
  vec2.x = radius * g_cosTable[angle];
  vec2.z = radius * g_sinTable[angle];
  angle = (angle + 90) % 360;
  vec3.x = radius * g_cosTable[angle];
  vec3.z = radius * g_sinTable[angle];
  angle = (angle + 90) % 360;
  vec4.x = radius * g_cosTable[angle];
  vec4.z = radius * g_sinTable[angle];
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, slot.alphaT, this->state.slots[1].alphaT);

  vec1.x = vec1.x + slot.vecBNow.x;
  vec1.y = vec1.y + slot.vecBNow.y;
  vec1.z = slot.vecBNow.z + vec1.z;
  vec2.x = vec2.x + slot.vecBNow.x;
  vec2.y = vec2.y + slot.vecBNow.y;
  vec2.z = vec2.z + slot.vecBNow.z;
  vec3.x = vec3.x + slot.vecBNow.x;
  vec3.y = vec3.y + slot.vecBNow.y;
  vec3.z = vec3.z + slot.vecBNow.z;
  vec4.x = vec4.x + slot.vecBNow.x;
  vec4.y = vec4.y + slot.vecBNow.y;
  vec4.z = vec4.z + slot.vecBNow.z;

  if (slot.flag1[0] == 10) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 1, 0, 255, 20, 20, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 11) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 200, 200, 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 12) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.flag1[0] == 13) {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 100, 150, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else {
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BEEF80
// FUNCTION: fn_19981
void CEffectPrim::RenderPillarSquares(matrix& wtm) {
  int rpCnt = 0;
  EffectPrimitiveSlot& slot = this->state.slots[0];

  if (slot.life != 1 || slot.alphaB <= 0) {
    return;
  }

  vector3d vec1;
  vector3d vec2;
  vector3d vec3;
  vector3d vec4;
  vec1.y = 0.0f;
  vec2.y = 0.0f;
  vec3.y = 0.0f;
  vec4.y = 0.0f;

  float radius = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance;
  vec1.x = radius * g_cosTable[0];
  vec1.z = radius * g_sinTable[0];
  vec2.x = radius * g_cosTable[90];
  vec2.z = radius * g_sinTable[90];
  vec3.x = radius * g_cosTable[180];
  vec3.z = radius * g_sinTable[180];
  vec4.x = radius * g_cosTable[270];
  vec4.z = radius * g_sinTable[270];
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, slot.alphaT, this->state.slots[1].alphaT);

  vec1.x = slot.vecBNow.x + vec1.x;
  vec1.y = slot.vecBNow.y + vec1.y;
  vec1.z = slot.vecBNow.z + vec1.z;
  vec2.x = slot.vecBNow.x + vec2.x;
  vec2.y = slot.vecBNow.y + vec2.y;
  vec2.z = slot.vecBNow.z + vec2.z;
  vec3.x = slot.vecBNow.x + vec3.x;
  vec3.y = slot.vecBNow.y + vec3.y;
  vec3.z = slot.vecBNow.z + vec3.z;
  vec4.x = slot.vecBNow.x + vec4.x;
  vec4.y = slot.vecBNow.y + vec4.y;
  vec4.z = slot.vecBNow.z + vec4.z;
  this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 68, 42, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);

  vec1.y = 0.0f;
  vec2.y = 0.0f;
  vec3.y = 0.0f;
  vec4.y = 0.0f;
  radius = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance * g_depthAverageHalf;
  vec1.x = radius * g_cosTable[0];
  vec1.z = radius * g_sinTable[0];
  vec2.x = radius * g_cosTable[90];
  vec2.z = radius * g_sinTable[90];
  vec3.x = radius * g_cosTable[180];
  vec3.z = radius * g_sinTable[180];
  vec4.x = radius * g_cosTable[270];
  vec4.z = radius * g_sinTable[270];
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, slot.alphaT, this->state.slots[1].alphaT);

  vec1.x = slot.vecBNow.x + vec1.x;
  vec1.y = slot.vecBNow.y + vec1.y;
  vec1.z = slot.vecBNow.z + vec1.z;
  vec2.x = slot.vecBNow.x + vec2.x;
  vec2.y = slot.vecBNow.y + vec2.y;
  vec2.z = slot.vecBNow.z + vec2.z;
  vec3.x = slot.vecBNow.x + vec3.x;
  vec3.y = slot.vecBNow.y + vec3.y;
  vec3.z = slot.vecBNow.z + vec3.z;
  vec4.x = slot.vecBNow.x + vec4.x;
  vec4.y = slot.vecBNow.y + vec4.y;
  vec4.z = slot.vecBNow.z + vec4.z;
  this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 155, 155, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& pillar = this->state.slots[ec];
    const int angle = pillar.rotStart;
    const float height = pillar.height[12] * g_depthAverageHalf;
    const int opposite = angle + (angle < 180 ? 180 : -180);

    pillar.vecTPre.y = height;
    pillar.vecTPre.z = g_sinTable[angle] * height + g_cosTable[angle] * 0.0f;
    pillar.vecTPre.x = g_cosTable[angle] * height - g_sinTable[angle] * 0.0f;
    pillar.vecTNow.y = height;
    pillar.vecTNow.x = g_cosTable[opposite] * height - g_sinTable[opposite] * 0.0f;
    pillar.vecTNow.z = g_sinTable[opposite] * height + g_cosTable[opposite] * 0.0f;

    pillar.vecTPre.x = pillar.vecTPre.x + slot.vecBNow.x;
    pillar.vecTPre.y = slot.vecBNow.y + pillar.vecTPre.y;
    pillar.vecTPre.z = slot.vecBNow.z + pillar.vecTPre.z;
    pillar.vecTNow.x = slot.vecBNow.x + pillar.vecTNow.x;
    pillar.vecTNow.y = slot.vecBNow.y + pillar.vecTNow.y;
    pillar.vecTNow.z = slot.vecBNow.z + pillar.vecTNow.z;

    vector3d lowNow;
    lowNow.x = pillar.vecTNow.x;
    lowNow.y = pillar.vecTNow.y - pillar.height[12];
    lowNow.z = pillar.vecTNow.z;
    vector3d lowPre;
    lowPre.x = pillar.vecTPre.x;
    lowPre.y = pillar.vecTPre.y - pillar.height[12];
    lowPre.z = pillar.vecTPre.z;

    unsigned char shade;
    if (ec == 0) {
      shade = 105;
    } else if (ec == 1) {
      shade = 35;
    } else {
      shade = ec == 2 ? 75 : 5;
    }

    if (pillar.height[12] > 0.0f) {
      this->RenderTeiRect(
        &rpCnt, pillar.riseAngle, pillar.vecTPre, pillar.vecTNow, lowNow, lowPre, wtm, 0, 1, 0, shade, shade, 255, 0.0f, 1.0f, 0.0f,
        1.0f, 0
      );
    }
  }
}

// OFFSET: 0x00BEF6C0
// FUNCTION: fn_19982
void CEffectPrim::RenderBladeFins(matrix& wtm) {
  vector3d base{};
  vector3d outer{};
  vector3d inner{};
  vector3d base2{};
  int rpCnt = 0;
  unsigned char red;
  unsigned char green;
  unsigned char blue;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    base2.x = 0.0f;
    base2.y = -6.0f;
    base2.z = 0.0f;
    base.x = 0.0f;
    base.y = -6.0f;
    base.z = 0.0f;
    outer.y = -6.0f;
    inner.y = -6.0f;

    int angle = slot.riseAngle - 25;
    if (angle < 0) {
      angle += 360;
    }
    inner.x = slot.height[0] * g_cosTable[angle];
    inner.z = slot.height[0] * g_sinTable[angle];
    angle -= 90;
    if (angle < 0) {
      angle += 360;
    }
    outer.x = slot.maxHeight * g_cosTable[angle] + inner.x;
    outer.z = slot.maxHeight * g_sinTable[angle] + inner.z;

    if (slot.flag1[0] == 1) {
      this->RenderTeiRect(&rpCnt, slot.alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 89, 182, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      red = 255;
      green = 25;
      blue = 25;
    } else if (slot.flag1[0] == 2) {
      this->RenderTeiRect(&rpCnt, slot.alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      red = 255;
      green = 255;
      blue = 255;
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      red = 25;
      green = 25;
      blue = 255;
    }
    this->RenderTeiRect(&rpCnt, slot.alphaB, base, outer, inner, base2, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 1);

    angle = slot.riseAngle + 25;
    if (angle >= 360) {
      angle -= 360;
    }
    inner.x = slot.height[0] * g_cosTable[angle];
    inner.z = slot.height[0] * g_sinTable[angle];
    angle -= 90;
    if (angle < 0) {
      angle += 360;
    }
    outer.x = slot.maxHeight * g_cosTable[angle] + inner.x;
    outer.z = slot.maxHeight * g_sinTable[angle] + inner.z;

    if (slot.flag1[0] == 1) {
      this->RenderTeiRect(&rpCnt, slot.alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 89, 182, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      red = 255;
      green = 25;
      blue = 25;
    } else if (slot.flag1[0] == 2) {
      this->RenderTeiRect(&rpCnt, slot.alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      red = 255;
      green = 255;
      blue = 255;
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB / 4, base, outer, inner, base2, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      red = 25;
      green = 25;
      blue = 255;
    }
    this->RenderTeiRect(&rpCnt, slot.alphaB, base, outer, inner, base2, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 1);
  }
}

// OFFSET: 0x00BF1400
// FUNCTION: fn_19987
void CEffectPrim::RenderPolygonFrames(matrix& wtm) {
  vector3d p0{};
  vector3d p1{};
  vector3d p2{};
  vector3d p3{};
  int rpCnt = 0;
  float radius;
  float top;
  int angle;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    radius = slot.distance;
    angle = slot.fullDisplayAngle;
    p0.x = radius * g_cosTable[angle];
    p0.y = 0.0f;
    p0.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 60;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    p1.x = radius * g_cosTable[angle];
    p1.y = 0.0f;
    p1.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 120;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 50;
      if (angle < 0) {
        angle += 360;
      }
    }
    p2.x = radius * g_cosTable[angle];
    p2.y = 0.0f;
    p2.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 60;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    p3.x = radius * g_cosTable[angle];
    p3.y = 0.0f;
    p3.z = radius * g_sinTable[angle];

    top = p1.z > p0.z ? p1.z : p0.z;
    top = p2.z > top ? p2.z : top;
    top = p3.z > top ? p3.z : top;
    p0.z -= top;
    p1.z -= top;
    p2.z -= top;
    p3.z -= top;
    this->Tei_TowardScreen(p0, p1, p2, p3, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    p0.x = slot.vecBNow.x + p0.x;
    p0.y = slot.vecBNow.y + p0.y;
    p0.z = slot.vecBNow.z + p0.z;
    p1.x = slot.vecBNow.x + p1.x;
    p1.y = slot.vecBNow.y + p1.y;
    p1.z = slot.vecBNow.z + p1.z;
    p2.x = slot.vecBNow.x + p2.x;
    p2.y = slot.vecBNow.y + p2.y;
    p2.z = slot.vecBNow.z + p2.z;
    p3.x = slot.vecBNow.x + p3.x;
    p3.y = slot.vecBNow.y + p3.y;
    p3.z = slot.vecBNow.z + p3.z;

    switch (slot.flag1[2]) {
    case 0:
    case 3:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p0, p1, p2, p3, wtm, 1, slot.flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 19:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 205, 255, 205, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 20:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 255, 5, 5, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 255, 155, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 21:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 1, slot.flag1[0], 1, 225, 255, 225, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    }

    radius = slot.distance * g_sinTable[static_cast<int>(slot.height[8])] * 0.05f + slot.distance;
    angle = slot.fullDisplayAngle;
    p0.x = radius * g_cosTable[angle];
    p0.y = 0.0f;
    p0.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 60;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    p1.x = radius * g_cosTable[angle];
    p1.y = 0.0f;
    p1.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 120;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 50;
      if (angle < 0) {
        angle += 360;
      }
    }
    p2.x = radius * g_cosTable[angle];
    p2.y = 0.0f;
    p2.z = radius * g_sinTable[angle];
    if (slot.height[10] == 0.0f) {
      angle -= 90;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 1.0f) {
      angle -= 60;
      if (angle < 0) {
        angle += 360;
      }
    } else if (slot.height[10] == 2.0f) {
      angle -= 130;
      if (angle < 0) {
        angle += 360;
      }
    }
    p3.x = radius * g_cosTable[angle];
    p3.y = 0.0f;
    p3.z = radius * g_sinTable[angle];

    top = p1.z > p0.z ? p1.z : p0.z;
    top = p2.z > top ? p2.z : top;
    top = p3.z > top ? p3.z : top;
    p0.z -= top;
    p1.z -= top;
    p2.z -= top;
    p3.z -= top;
    this->Tei_TowardScreen(p0, p1, p2, p3, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    p0.x = slot.vecBNow.x + p0.x;
    p0.y = slot.vecBNow.y + p0.y;
    p0.z = slot.vecBNow.z + p0.z;
    p1.x = slot.vecBNow.x + p1.x;
    p1.y = slot.vecBNow.y + p1.y;
    p1.z = slot.vecBNow.z + p1.z;
    p2.x = slot.vecBNow.x + p2.x;
    p2.y = slot.vecBNow.y + p2.y;
    p2.z = slot.vecBNow.z + p2.z;
    p3.x = slot.vecBNow.x + p3.x;
    p3.y = slot.vecBNow.y + p3.y;
    p3.z = slot.vecBNow.z + p3.z;

    switch (slot.flag1[2]) {
    case 0:
    case 3:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p0, p1, p2, p3, wtm, 1, slot.flag1[0], 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 19:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 105, 255, 105, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 20:
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 255, 5, 5, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, slot.alphaB, p1, p0, p3, p2, wtm, 0, slot.flag1[0], 1, 255, 155, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    }
  }
}
