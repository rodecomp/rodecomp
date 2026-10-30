#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00BF1F00
// FUNCTION: fn_19988
void CEffectPrim::RenderSpinSquares(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].alphaB <= 0) {
      continue;
    }

    EffectPrimitiveSlot& slot = this->state.slots[static_cast<short>(ec)];
    float tilt = slot.height[8];
    if (tilt >= 360.0f || tilt < 0.0f) {
      slot.height[8] = 0.0f;
      tilt = 0.0f;
    }

    vector3d vec1;
    vector3d vec2;
    vector3d vec3;
    vector3d vec4;
    vec1.y = 0.0f;
    vec2.y = 0.0f;
    vec3.y = 0.0f;
    vec4.y = 0.0f;

    const int angle1 = slot.fullDisplayAngle;
    const int angle2 = angle1 + (angle1 < 270 ? 90 : -270);
    const int angle3 = angle2 + (angle2 + 90 < 360 ? 90 : -270);
    const int angle4 = angle3 + (angle3 + 90 < 360 ? 90 : -270);
    const float radius = slot.distance * g_sinTable[static_cast<int>(tilt)] * 0.05f + slot.distance;
    vec1.x = radius * g_cosTable[angle1];
    vec1.z = radius * g_sinTable[angle1];
    vec2.x = radius * g_cosTable[angle2];
    vec2.z = radius * g_sinTable[angle2];
    vec3.x = radius * g_cosTable[angle3];
    vec3.z = radius * g_sinTable[angle3];
    vec4.x = radius * g_cosTable[angle4];
    vec4.z = radius * g_sinTable[angle4];
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

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

    for (int drop = 0, turn = 0; drop < 72; drop += 2, ++turn) {
      const short spin = static_cast<short>(static_cast<short>(turn * 45 + slot.process) % 360);
      const float offsetX = g_cosTable[spin] * 10.0f;
      const float offsetZ = g_sinTable[spin] * 10.0f;
      const float lift = static_cast<float>(drop);
      vector3d corner1;
      corner1.x = offsetX + vec1.x;
      corner1.y = vec1.y - lift;
      corner1.z = offsetZ + vec1.z;
      vector3d corner2;
      corner2.x = offsetX + vec2.x;
      corner2.y = vec2.y - lift;
      corner2.z = offsetZ + vec2.z;
      vector3d corner3;
      corner3.x = offsetX + vec3.x;
      corner3.y = vec3.y - lift;
      corner3.z = offsetZ + vec3.z;
      vector3d corner4;
      corner4.x = offsetX + vec4.x;
      corner4.y = vec4.y - lift;
      corner4.z = offsetZ + vec4.z;
      this->RenderTeiRect(&rpCnt, slot.alphaB, corner1, corner2, corner3, corner4, wtm, 1, slot.flag1[0], 1, 55, 55, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }

    this->RenderSpinSquare(wtm, &rpCnt, 2.0f, static_cast<short>(ec), 0, 25, 25, 255);
  }
}

// OFFSET: 0x00BF23A0
// FUNCTION: fn_19989
void CEffectPrim::RenderSpinSquare(
  matrix& wtm, int* rpCnt, float scale, short slotIndex, char pw, unsigned char red, unsigned char green, unsigned char blue
) {
  EffectPrimitiveSlot& slot = this->state.slots[slotIndex];
  float tilt = slot.height[8];
  if (tilt >= 360.0f || tilt < 0.0f) {
    slot.height[8] = 0.0f;
    tilt = 0.0f;
  }

  vector3d vec1;
  vector3d vec2;
  vector3d vec3;
  vector3d vec4;
  vec1.y = 0.0f;
  vec2.y = 0.0f;
  vec3.y = 0.0f;
  vec4.y = 0.0f;

  const int angle1 = slot.fullDisplayAngle;
  const int angle2 = angle1 + (angle1 < 270 ? 90 : -270);
  const int angle3 = angle2 + (angle2 + 90 < 360 ? 90 : -270);
  const int angle4 = angle3 + (angle3 + 90 < 360 ? 90 : -270);
  const float radius = slot.distance * g_sinTable[static_cast<int>(tilt)] * 0.05f + slot.distance * scale;
  vec1.x = radius * g_cosTable[angle1];
  vec1.z = radius * g_sinTable[angle1];
  vec2.x = radius * g_cosTable[angle2];
  vec2.z = radius * g_sinTable[angle2];
  vec3.x = radius * g_cosTable[angle3];
  vec3.z = radius * g_sinTable[angle3];
  vec4.x = radius * g_cosTable[angle4];
  vec4.z = radius * g_sinTable[angle4];
  this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

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

  for (int drop = 0, turn = 0; drop < 72; drop += 2, ++turn) {
    const short spin = static_cast<short>(static_cast<short>(turn * 45 + slot.process) % 360);
    const float offsetX = g_cosTable[spin] * 10.0f;
    const float offsetZ = g_sinTable[spin] * 10.0f;
    const float lift = static_cast<float>(drop);
    vector3d corner1;
    corner1.x = offsetX + vec1.x;
    corner1.y = vec1.y - lift;
    corner1.z = offsetZ + vec1.z;
    vector3d corner2;
    corner2.x = offsetX + vec2.x;
    corner2.y = vec2.y - lift;
    corner2.z = offsetZ + vec2.z;
    vector3d corner3;
    corner3.x = offsetX + vec3.x;
    corner3.y = vec3.y - lift;
    corner3.z = offsetZ + vec3.z;
    vector3d corner4;
    corner4.x = offsetX + vec4.x;
    corner4.y = vec4.y - lift;
    corner4.z = offsetZ + vec4.z;
    this->RenderTeiRect(rpCnt, slot.alphaB, corner1, corner2, corner3, corner4, wtm, pw, slot.flag1[0], 1, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BF27D0
// FUNCTION: fn_19990
void CEffectPrim::RenderTrailingArc(matrix& wtm) {
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

    const int texture = slot.process % static_cast<int>(this->state.totalTexture);
    const int step = slot.fullDisplayAngle / 20;
    int i = 0;
    for (int offset = 0; offset <= slot.fullDisplayAngle; offset += step) {
      int angle = (slot.rotStart + offset) % 360;
      if (slot.fullDisplayAngle == 360 && i == 20) {
        angle = slot.rotStart;
      }

      const float baseX = slot.distance * g_cosTable[angle];
      const float baseZ = slot.distance * g_sinTable[angle];
      const float rise = slot.height[i];
      const float riseX = rise * g_cosTable[slot.riseAngle];
      curT.y = -(rise * g_sinTable[slot.riseAngle]);
      curB.x = baseX - this->state.slots[0].vecTNow.x * 0.75f;
      curB.z = baseZ - this->state.slots[0].vecTNow.z * 0.75f;
      curT.z = riseX * g_sinTable[angle] + baseZ - this->state.slots[0].vecTNow.z;
      curT.x = riseX * g_cosTable[angle] + baseX - this->state.slots[0].vecTNow.x;

      if (i > 0) {
        this->state.size = slot.vecTPre.x;
        this->RenderTeiRect(&rpCnt, i - 1, slot.alphaB, slot.alphaB, prevB, curB, curT, prevT, wtm, static_cast<char>(texture));
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

// OFFSET: 0x00BF34D0
// FUNCTION: fn_19992
void CEffectPrim::RenderBladeQuads(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    int angle = slot.riseAngle;
    int angle1 = angle + (angle < 2 ? 358 : -2);
    int angle2 = angle + (angle < 358 ? 2 : -358);
    int angle3 = angle + (angle < 182 ? 178 : -182);
    int angle4 = angle + (angle < 178 ? 182 : -178);
    vector3d vec1;
    vector3d vec2;
    vector3d vec3;
    vector3d vec4;
    vec1.x = slot.distance * g_cosTable[angle1] * g_cosTable[slot.rotStart] + slot.vecBNow.x;
    vec1.y = slot.distance * g_cosTable[angle1] * g_sinTable[slot.rotStart] + slot.height[0] + slot.vecBNow.y;
    vec1.z = slot.distance * g_sinTable[angle1] + slot.vecBNow.z;
    vec2.x = slot.distance * g_cosTable[angle2] * g_cosTable[slot.rotStart] + slot.vecBNow.x;
    vec2.y = slot.distance * g_cosTable[angle2] * g_sinTable[slot.rotStart] + slot.height[0] + slot.vecBNow.y;
    vec2.z = slot.distance * g_sinTable[angle2] + slot.vecBNow.z;
    vec3.x = slot.distance * g_cosTable[angle3] * g_cosTable[slot.rotStart] + slot.vecBNow.x;
    vec3.y = slot.distance * g_cosTable[angle3] * g_sinTable[slot.rotStart] + slot.height[0] + slot.vecBNow.y;
    vec3.z = slot.distance * g_sinTable[angle3] + slot.vecBNow.z;
    vec4.x = slot.distance * g_cosTable[angle4] * g_cosTable[slot.rotStart] + slot.vecBNow.x;
    vec4.y = slot.distance * g_cosTable[angle4] * g_sinTable[slot.rotStart] + slot.height[0] + slot.vecBNow.y;
    vec4.z = slot.distance * g_sinTable[angle4] + slot.vecBNow.z;

    if (slot.flag1[2] == 0) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 0, 250, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (slot.flag1[2] == 1) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 250, 250, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (slot.flag1[2] == 2) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 250, 100, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (slot.flag1[2] == 3) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 200, 200, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }

    angle = slot.riseAngle;
    angle1 = angle + (angle < 8 ? 352 : -8);
    angle2 = angle + (angle < 352 ? 8 : -352);
    angle3 = angle + (angle < 188 ? 172 : -188);
    angle4 = angle + (angle < 172 ? 188 : -172);
    vec1.x = slot.distance * g_cosTable[angle1] * g_cosTable[slot.rotStart] + slot.vecBNow.x;
    vec1.y = slot.distance * g_cosTable[angle1] * g_sinTable[slot.rotStart] + slot.height[0] + slot.vecBNow.y;
    vec1.z = slot.distance * g_sinTable[angle1] + slot.vecBNow.z;
    vec2.x = slot.distance * g_cosTable[angle2] * g_cosTable[slot.rotStart] + slot.vecBNow.x;
    vec2.y = slot.distance * g_cosTable[angle2] * g_sinTable[slot.rotStart] + slot.height[0] + slot.vecBNow.y;
    vec2.z = slot.distance * g_sinTable[angle2] + slot.vecBNow.z;
    vec3.x = slot.distance * g_cosTable[angle3] * g_cosTable[slot.rotStart] + slot.vecBNow.x;
    vec3.y = slot.distance * g_cosTable[angle3] * g_sinTable[slot.rotStart] + slot.height[0] + slot.vecBNow.y;
    vec3.z = slot.distance * g_sinTable[angle3] + slot.vecBNow.z;
    vec4.x = slot.distance * g_cosTable[angle4] * g_cosTable[slot.rotStart] + slot.vecBNow.x;
    vec4.y = slot.distance * g_cosTable[angle4] * g_sinTable[slot.rotStart] + slot.height[0] + slot.vecBNow.y;
    vec4.z = slot.distance * g_sinTable[angle4] + slot.vecBNow.z;

    if (slot.flag1[2] == 0) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 250, 0, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (slot.flag1[2] == 1) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 250, 250, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (slot.flag1[2] == 2) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 166, 210, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (slot.flag1[2] == 3) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 5, 5, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BF3BD0
// FUNCTION: fn_19993
void CEffectPrim::RenderGlowSquare(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1) {
      continue;
    }

    vector3d vec1;
    vector3d vec2;
    vector3d vec3;
    vector3d vec4;
    vec1.x = slot.distance * g_cosTable[45];
    vec1.y = 0.0f;
    vec1.z = slot.distance * g_sinTable[45];
    vec2.x = slot.distance * g_cosTable[135];
    vec2.y = 0.0f;
    vec2.z = slot.distance * g_sinTable[135];
    vec3.x = slot.distance * g_cosTable[225];
    vec3.y = 0.0f;
    vec3.z = slot.distance * g_sinTable[225];
    vec4.x = slot.distance * g_cosTable[315];
    vec4.y = 0.0f;
    vec4.z = slot.distance * g_sinTable[315];
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    vector3d corner1;
    vector3d corner2;
    vector3d corner3;
    vector3d corner4;
    corner1.x = slot.vecBNow.x + vec1.x;
    corner1.y = slot.vecBNow.y + vec1.y;
    corner1.z = slot.vecBNow.z + vec1.z;
    corner2.x = slot.vecBNow.x + vec2.x;
    corner2.y = slot.vecBNow.y + vec2.y;
    corner2.z = slot.vecBNow.z + vec2.z;
    corner3.x = slot.vecBNow.x + vec3.x;
    corner3.y = slot.vecBNow.y + vec3.y;
    corner3.z = slot.vecBNow.z + vec3.z;
    corner4.x = slot.vecBNow.x + vec4.x;
    corner4.y = slot.vecBNow.y + vec4.y;
    corner4.z = slot.vecBNow.z + vec4.z;

    const short alpha = slot.alphaB < 0 ? 0 : slot.alphaB;
    if (slot.flag1[0] == 0) {
      this->RenderTeiRect(&rpCnt, alpha, corner1, corner2, corner3, corner4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else if (slot.flag1[0] == 1) {
      this->RenderTeiRect(&rpCnt, alpha, corner1, corner2, corner3, corner4, wtm, 0, 0, 0, 255, 160, 160, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else if (slot.flag1[0] == 2) {
      this->RenderTeiRect(&rpCnt, alpha, corner1, corner2, corner3, corner4, wtm, 0, 0, 0, 255, 90, 90, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    }

    corner1.x = vec1.x * 1.1f + slot.vecBNow.x;
    corner1.y = vec1.y * 1.1f + slot.vecBNow.y;
    corner1.z = vec1.z * 1.1f + slot.vecBNow.z;
    corner2.x = vec2.x * 1.1f + slot.vecBNow.x;
    corner2.y = vec2.y * 1.1f + slot.vecBNow.y;
    corner2.z = vec2.z * 1.1f + slot.vecBNow.z;
    corner3.x = vec3.x * 1.1f + slot.vecBNow.x;
    corner3.y = vec3.y * 1.1f + slot.vecBNow.y;
    corner3.z = vec3.z * 1.1f + slot.vecBNow.z;
    corner4.x = vec4.x * 1.1f + slot.vecBNow.x;
    corner4.y = vec4.y * 1.1f + slot.vecBNow.y;
    corner4.z = vec4.z * 1.1f + slot.vecBNow.z;
    this->RenderTeiRect(&rpCnt, alpha, corner1, corner2, corner3, corner4, wtm, 0, 0, 0, 255, 1, 1, 0.0f, 1.0f, 0.0f, 1.0f, 1);
  }
}

// OFFSET: 0x00BF4110
// FUNCTION: fn_19994
void CEffectPrim::RenderArcWalls(matrix& wtm) {
  vector3d prevB{};
  vector3d prevT{};
  vector3d curB{};
  vector3d curT{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 4;
    int i = 0;
    for (int offset = 0; offset <= slot.fullDisplayAngle; offset += step) {
      int angle = (slot.rotStart + offset) % 360;
      if (slot.fullDisplayAngle == 360 && i == 4) {
        angle = slot.rotStart;
      }

      const float baseX = slot.distance * g_cosTable[angle];
      const float baseZ = slot.distance * g_sinTable[angle];
      const float rise = slot.height[i];
      const float riseX = rise * g_cosTable[slot.riseAngle];
      curB.x = baseX + slot.vecBNow.x + slot.vecTPre.x;
      curB.y = slot.vecBNow.y + 0.0f + slot.vecTPre.y;
      curB.z = baseZ + slot.vecBNow.z + slot.vecTPre.z;
      curT.y = slot.vecBNow.y - rise * g_sinTable[slot.riseAngle] + slot.vecTPre.y;
      curT.x = riseX * g_cosTable[angle] + baseX + slot.vecBNow.x + slot.vecTPre.x;
      curT.z = riseX * g_sinTable[angle] + baseZ + slot.vecBNow.z + slot.vecTPre.z;

      if (i > 0) {
        this->RenderTeiRect(&rpCnt, slot.alphaB / 5, prevB, curB, prevT, curT, wtm, 1, 0, 0, 255, 10, 10, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }

      ++i;
      if (i > 4) {
        i = 4;
      }
      prevB = curB;
      prevT = curT;
    }

    i = 0;
    for (int offset = 0; offset <= slot.fullDisplayAngle; offset += step) {
      int angle = (slot.rotStart + 45 + offset) % 360;
      if (slot.fullDisplayAngle == 360 && i == 4) {
        angle = slot.rotStart;
      }

      const float baseX = slot.distance * g_cosTable[angle] * 1.5f;
      const float baseZ = slot.distance * g_sinTable[angle] * 1.5f;
      const float rise = slot.height[i];
      const float riseX = rise * g_cosTable[slot.riseAngle];
      curB.x = baseX + slot.vecBNow.x + slot.vecTPre.x;
      curB.y = slot.vecBNow.y + 0.0f + slot.vecTPre.y;
      curB.z = baseZ + slot.vecBNow.z + slot.vecTPre.z;
      curT.y = slot.vecBNow.y - rise * g_sinTable[slot.riseAngle] + slot.vecTPre.y;
      curT.x = riseX * g_cosTable[angle] + baseX + slot.vecBNow.x + slot.vecTPre.x;
      curT.z = riseX * g_sinTable[angle] + baseZ + slot.vecBNow.z + slot.vecTPre.z;

      if (i > 0) {
        this->RenderTeiRect(&rpCnt, i - 1, slot.alphaB, slot.alphaB, prevB, curB, curT, prevT, wtm, 0);
      }

      ++i;
      if (i > 4) {
        i = 4;
      }
      prevB = curB;
      prevT = curT;
    }
  }
}
