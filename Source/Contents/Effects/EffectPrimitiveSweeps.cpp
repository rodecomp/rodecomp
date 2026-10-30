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
