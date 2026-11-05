#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Utility/Math/AffineMatrix.h"

#include <algorithm>
#include <cstdlib>

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
  vector3d prevT{};
  vector3d curB{};
  vector3d curT{};
  vector3d prevB{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    if (this->state.slots[ec].life != 1 || this->state.slots[ec].process <= 0) {
      continue;
    }

    const int texture = this->state.slots[ec].process % static_cast<int>(this->state.totalTexture);
    const int step = this->state.slots[ec].fullDisplayAngle / 20;
    int i = 0;
    for (int offset = 0; offset <= this->state.slots[ec].fullDisplayAngle; offset += step) {
      int angle = (this->state.slots[ec].rotStart + offset) % 360;
      if (this->state.slots[ec].fullDisplayAngle == 360 && i == 20) {
        angle = this->state.slots[ec].rotStart;
      }

      const float baseX = this->state.slots[ec].distance * g_cosTable[angle];
      const float baseZ = this->state.slots[ec].distance * g_sinTable[angle];
      const float rise = this->state.slots[ec].height[i];
      const float riseX = rise * g_cosTable[this->state.slots[ec].riseAngle];
      curT.y = -(rise * g_sinTable[this->state.slots[ec].riseAngle]);
      curB.x = baseX - this->state.slots[0].vecTNow.x * 0.75f;
      curB.y = 0.0f;
      curB.z = baseZ - this->state.slots[0].vecTNow.z * 0.75f;
      curT.z = riseX * g_sinTable[angle] + baseZ - this->state.slots[0].vecTNow.z;
      curT.x = riseX * g_cosTable[angle] + baseX - this->state.slots[0].vecTNow.x;

      if (i > 0) {
        this->state.size = this->state.slots[ec].vecTPre.x;
        this->RenderTeiRect(&rpCnt, i - 1, this->state.slots[ec].alphaB, this->state.slots[ec].alphaB, prevB, curB, curT, prevT, wtm, static_cast<char>(texture));
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
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  vector3d vec4{};
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
    if (this->state.slots[ec].life != 1) {
      continue;
    }

    const int step = this->state.slots[ec].fullDisplayAngle / 4;
    int i = 0;
    for (int offset = 0; offset <= this->state.slots[ec].fullDisplayAngle; offset += step) {
      int angle = (this->state.slots[ec].rotStart + offset) % 360;
      if (this->state.slots[ec].fullDisplayAngle == 360 && i == 4) {
        angle = this->state.slots[ec].rotStart;
      }

      const float baseX = this->state.slots[ec].distance * g_cosTable[angle];
      const float baseZ = this->state.slots[ec].distance * g_sinTable[angle];
      const float rise = this->state.slots[ec].height[i];
      const float riseX = rise * g_cosTable[this->state.slots[ec].riseAngle];
      curB.x = baseX + this->state.slots[ec].vecBNow.x + this->state.slots[ec].vecTPre.x;
      curB.y = this->state.slots[ec].vecBNow.y + 0.0f + this->state.slots[ec].vecTPre.y;
      curB.z = baseZ + this->state.slots[ec].vecBNow.z + this->state.slots[ec].vecTPre.z;
      curT.y = this->state.slots[ec].vecBNow.y - rise * g_sinTable[this->state.slots[ec].riseAngle] + this->state.slots[ec].vecTPre.y;
      curT.x = riseX * g_cosTable[angle] + baseX + this->state.slots[ec].vecBNow.x + this->state.slots[ec].vecTPre.x;
      curT.z = riseX * g_sinTable[angle] + baseZ + this->state.slots[ec].vecBNow.z + this->state.slots[ec].vecTPre.z;

      if (i > 0) {
        this->RenderTeiRect(&rpCnt, this->state.slots[ec].alphaB / 5, prevB, curB, prevT, curT, wtm, 1, 0, 0, 255, 10, 10, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }

      ++i;
      if (i > 4) {
        i = 4;
      }
      prevB = curB;
      prevT = curT;
    }

    i = 0;
    for (int offset = 0; offset <= this->state.slots[ec].fullDisplayAngle; offset += step) {
      int angle = (this->state.slots[ec].rotStart + 45 + offset) % 360;
      if (this->state.slots[ec].fullDisplayAngle == 360 && i == 4) {
        angle = this->state.slots[ec].rotStart;
      }

      const float baseX = this->state.slots[ec].distance * g_cosTable[angle] * 1.5f;
      const float baseZ = this->state.slots[ec].distance * g_sinTable[angle] * 1.5f;
      const float rise = this->state.slots[ec].height[i];
      const float riseX = rise * g_cosTable[this->state.slots[ec].riseAngle];
      curB.x = baseX + this->state.slots[ec].vecBNow.x + this->state.slots[ec].vecTPre.x;
      curB.y = this->state.slots[ec].vecBNow.y + 0.0f + this->state.slots[ec].vecTPre.y;
      curB.z = baseZ + this->state.slots[ec].vecBNow.z + this->state.slots[ec].vecTPre.z;
      curT.y = this->state.slots[ec].vecBNow.y - rise * g_sinTable[this->state.slots[ec].riseAngle] + this->state.slots[ec].vecTPre.y;
      curT.x = riseX * g_cosTable[angle] + baseX + this->state.slots[ec].vecBNow.x + this->state.slots[ec].vecTPre.x;
      curT.z = riseX * g_sinTable[angle] + baseZ + this->state.slots[ec].vecBNow.z + this->state.slots[ec].vecTPre.z;

      if (i > 0) {
        this->RenderTeiRect(&rpCnt, i - 1, this->state.slots[ec].alphaB, this->state.slots[ec].alphaB, prevB, curB, curT, prevT, wtm, 0);
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

// OFFSET: 0x00BF2AC0
// FUNCTION: fn_19991
void CEffectPrim::RenderJitterSweeps(matrix& wtm) {
  vector3d lastBottom = {};
  vector3d currentBottom = {};
  vector3d lastTop = {};
  vector3d currentTop = {};
  vector3d bottoms[20];
  vector3d bottomsCopy[20];
  vector3d tops[20];
  vector3d topsCopy[20];
  for (int j = 0; j < 20; ++j) {
    bottoms[j] = {0.0f, 0.0f, 0.0f};
  }
  for (int j = 0; j < 20; ++j) {
    bottomsCopy[j] = {0.0f, 0.0f, 0.0f};
  }
  for (int j = 0; j < 20; ++j) {
    tops[j] = {0.0f, 0.0f, 0.0f};
  }
  for (int j = 0; j < 20; ++j) {
    topsCopy[j] = {0.0f, 0.0f, 0.0f};
  }
  int count = 0;

  for (int layer = 0; layer < 4; ++layer) {
    EffectPrimitiveSlot& slot = this->state.slots[layer];
    float jitterY[21];
    float jitterX[21];
    float jitterZ[21];
    for (int i = 0; i < slot.vecTNow.y; ++i) {
      int ry = std::rand() % 7;
      jitterY[i] = static_cast<float>(ry >= 0 ? ry : 0) * 0.1f - 0.3f;
      int rx = std::rand() % 7;
      jitterX[i] = static_cast<float>(rx >= 0 ? rx : 0) * 0.1f - 0.3f;
      int rz = std::rand() % 7;
      jitterZ[i] = static_cast<float>(rz >= 0 ? rz : 0) * 0.1f - 0.3f;
    }

    if (slot.life != 1 || slot.alphaB <= 0 || slot.process <= 0) {
      continue;
    }

    for (int i = 0; i < slot.vecTNow.y; ++i) {
      float height1 = slot.height[i];
      float height2 = slot.height[i + 1];
      float radius1 = 0.0f;
      if (height1 > 0.0f) {
        radius1 += height1 / 90.0f * slot.distance;
      }
      float radius2 = 0.0f;
      if (height2 > 0.0f) {
        radius2 += height2 / 90.0f * slot.distance;
      }
      const short angle = slot.riseAngle;
      vector3d start;
      vector3d end;
      start.x = g_cosTable[angle] * radius1 + jitterX[i];
      start.y = -slot.distance * g_sinTable[static_cast<short>(height1)] * slot.vecTNow.x + (jitterY[i] - 2.0f);
      start.z = g_sinTable[angle] * radius1 + jitterZ[i];
      end.x = g_cosTable[angle] * radius2 + jitterX[i + 1];
      end.y = -slot.distance * g_sinTable[static_cast<short>(height2)] * slot.vecTNow.x + (jitterY[i + 1] - 2.0f);
      end.z = g_sinTable[angle] * radius2 + jitterZ[i + 1];
      short facing = static_cast<short>(height2);
      Get_Angle(start.x - end.x, start.z - end.z, &facing);
      start.x += slot.vecBNow.x;
      start.y += slot.vecBNow.y;
      start.z += slot.vecBNow.z;
      end.x += slot.vecBNow.x;
      end.y += slot.vecBNow.y;
      end.z += slot.vecBNow.z;
      facing += 90;
      if (facing > 359) {
        facing -= 360;
      }

      if (i > 0 && i < 19) {
        for (int ring = 0, turn = 0; turn <= 360; ++ring, turn += 120) {
          const float rise = slot.maxHeight * g_sinTable[turn];
          const float radial = slot.maxHeight * g_cosTable[turn];
          const float dx = radial * g_cosTable[facing];
          const float dz = radial * g_sinTable[facing];
          currentBottom.x = start.x + dx;
          currentBottom.y = start.y + rise;
          currentBottom.z = start.z + dz;
          currentTop.x = end.x + dx;
          currentTop.y = end.y + rise;
          currentTop.z = end.z + dz;
          if (ring > 0) {
            const int alpha = slot.alphaB - i * 5;
            if (alpha > 0 && layer == 0) {
              unsigned char green;
              unsigned char blue;
              if (this->state.slots[0].fullDisplayAngle == 3) {
                int rb = std::rand() % 26;
                int rg = std::rand() % 51;
                blue = static_cast<unsigned char>((rb >= 0 ? rb : 0) + 100);
                green = static_cast<unsigned char>(rg >= 0 ? rg : 0);
              } else {
                green = static_cast<unsigned char>(i * 3);
                blue = green;
              }
              this->RenderTeiRect(&count, static_cast<short>(alpha), currentBottom, currentTop, lastTop, lastBottom,
                                  wtm, 0, 0, 1, 255, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            }
          }
          lastBottom = currentBottom;
          lastTop = currentTop;
        }

        if (layer == 0) {
          bottoms[i] = currentBottom;
          bottomsCopy[i] = currentBottom;
          tops[i] = currentTop;
          topsCopy[i] = currentTop;
        } else {
          int alpha = slot.alphaB;
          if (layer == 1) {
            alpha /= 3;
          } else if (layer == 2) {
            alpha /= 2;
          }
          this->RenderTeiRect(&count, static_cast<short>(alpha), currentBottom, currentTop, tops[i], bottoms[i],
                              wtm, 0, 0, 1, 255, 0, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      }
    }
  }
}
