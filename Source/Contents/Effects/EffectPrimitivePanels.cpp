#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/MapSkyMgr.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00BF4630
// FUNCTION: fn_19995
void CEffectPrim::RenderFoldingRibbon(matrix& wtm) {
  vector3d vec[4];
  for (int i = 0; i < 4; ++i) {
    vec[i].x = 0.0f;
    vec[i].y = 0.0f;
    vec[i].z = 0.0f;
  }
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const unsigned char shade = slot.height[3] == 1.0f ? 30 : 155;

    {
      const float dist = slot.distance;
      const float edge = 12.0f - slot.height[9] * 24.0f;
      const float cs = g_cosTable[slot.riseAngle];
      const float sn = g_sinTable[slot.riseAngle];
      const int angle = slot.riseAngle + (slot.riseAngle < 270 ? 90 : -270);
      vec[0].x = dist * cs - -12.0f * sn + g_cosTable[angle] * 12.0f;
      vec[0].y = slot.height[0];
      vec[0].z = -12.0f * cs + dist * sn + g_sinTable[angle] * 12.0f;
      vec[1].x = dist * cs - edge * sn + g_cosTable[angle] * 12.0f;
      vec[1].y = slot.height[0];
      vec[1].z = edge * cs + dist * sn + g_sinTable[angle] * 12.0f;
      vec[2].x = -dist * cs - edge * sn + g_cosTable[angle] * 12.0f;
      vec[2].y = slot.height[0];
      vec[2].z = edge * cs + -dist * sn + g_sinTable[angle] * 12.0f;
      vec[3].x = -dist * cs - -12.0f * sn + g_cosTable[angle] * 12.0f;
      vec[3].y = slot.height[0];
      vec[3].z = -12.0f * cs + -dist * sn + g_sinTable[angle] * 12.0f;
    }
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec[0], vec[1], vec[2], vec[3], wtm, 0, static_cast<char>(slot.height[1]), 0, shade, shade, 255, 0.0f, 1.0f, slot.height[9], 1.0f, 0);

    const float dist = slot.distance;
    const float edge = 12.0f - slot.height[9] * 24.0f;
    const float cs = g_cosTable[slot.riseAngle];
    const float sn = g_sinTable[slot.riseAngle];
    const int angle = slot.riseAngle + (slot.riseAngle < 270 ? 90 : -270);
    vec[0].x = dist * cs - edge * sn + g_cosTable[angle] * 12.0f;
    vec[0].y = slot.height[0];
    vec[0].z = edge * cs + dist * sn + g_sinTable[angle] * 12.0f;
    vec[1].x = dist * cs - 12.0f * sn + g_cosTable[angle] * 12.0f;
    vec[1].y = slot.height[0];
    vec[1].z = 12.0f * cs + dist * sn + g_sinTable[angle] * 12.0f;
    vec[2].x = -dist * cs - 12.0f * sn + g_cosTable[angle] * 12.0f;
    vec[2].y = slot.height[0];
    vec[2].z = 12.0f * cs + -dist * sn + g_sinTable[angle] * 12.0f;
    vec[3].x = -dist * cs - edge * sn + g_cosTable[angle] * 12.0f;
    vec[3].y = slot.height[0];
    vec[3].z = edge * cs + -dist * sn + g_sinTable[angle] * 12.0f;
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec[0], vec[1], vec[2], vec[3], wtm, 0, static_cast<char>(slot.height[1]), 0, shade, shade, 255, 0.0f, 1.0f, 0.0f, slot.height[9], 0);

    int bend = angle - 20;
    if (bend < 0) {
      bend += 360;
    }

    float rest = 1.0f - slot.height[9];
    vec[0].x = g_cosTable[bend] * (rest * 32.0f) + vec[1].x;
    vec[0].z = g_sinTable[bend] * (rest * 32.0f) + vec[1].z;
    vec[3].x = g_cosTable[bend] * (rest * 40.0f) + vec[2].x;
    vec[3].z = g_sinTable[bend] * (rest * 40.0f) + vec[2].z;
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec[1], vec[0], vec[3], vec[2], wtm, 0, static_cast<char>(slot.height[1]), 0, shade, shade, 255, 0.0f, 1.0f, slot.height[9], 1.0f, 0);

    vector3d mid1 = vec[0];
    vector3d mid2 = vec[3];
    vec[0].x = g_cosTable[bend] * 32.0f + vec[1].x;
    vec[0].z = g_sinTable[bend] * 32.0f + vec[1].z;
    vec[3].x = g_cosTable[bend] * 40.0f + vec[2].x;
    vec[3].z = g_sinTable[bend] * 40.0f + vec[2].z;
    vec[1] = mid1;
    vec[2] = mid2;
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec[1], vec[0], vec[3], vec[2], wtm, 0, static_cast<char>(slot.height[1]), 0, shade, shade, 255, 0.0f, 1.0f, 0.0f, slot.height[9], 0);

    bend -= 20;
    if (bend < 0) {
      bend += 360;
    }

    rest = 1.0f - slot.height[9];
    vec[1] = vec[0];
    vec[2] = vec[3];
    vec[0].x = g_cosTable[bend] * (rest * 32.0f) + vec[1].x;
    vec[0].z = g_sinTable[bend] * (rest * 32.0f) + vec[1].z;
    vec[3].x = g_cosTable[bend] * (rest * 40.0f) + vec[2].x;
    vec[3].z = g_sinTable[bend] * (rest * 40.0f) + vec[2].z;
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec[1], vec[0], vec[3], vec[2], wtm, 0, static_cast<char>(slot.height[1]), 0, shade, shade, 255, 0.0f, 1.0f, slot.height[9], 1.0f, 0);

    mid1 = vec[0];
    mid2 = vec[3];
    vec[0].x = g_cosTable[bend] * 32.0f + vec[1].x;
    vec[0].z = g_sinTable[bend] * 32.0f + vec[1].z;
    vec[3].x = g_cosTable[bend] * 40.0f + vec[2].x;
    vec[3].z = g_sinTable[bend] * 40.0f + vec[2].z;
    vec[1] = mid1;
    vec[2] = mid2;
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec[1], vec[0], vec[3], vec[2], wtm, 0, static_cast<char>(slot.height[1]), 0, shade, shade, 255, 0.0f, 1.0f, 0.0f, slot.height[9], 0);
  }
}

// OFFSET: 0x00BF7370
// FUNCTION: fn_20002
void CEffectPrim::RenderStandingPanels(matrix& wtm) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  vector3d vec4{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const char frame = static_cast<char>(slot.process % 30);

    float dist = slot.distance;
    float cs = g_cosTable[slot.rotStart];
    float sn = g_sinTable[slot.rotStart];
    vec1.x = cs * (dist * g_depthAverageHalf) - sn * 0.0f;
    vec1.y = -dist + -dist;
    vec1.z = sn * (dist * g_depthAverageHalf) + cs * 0.0f;
    vec2.x = cs * (dist * -0.5f) - sn * 0.0f;
    vec2.y = -dist + -dist;
    vec2.z = sn * (dist * -0.5f) + cs * 0.0f;
    vec3.x = cs * (dist * -0.5f) - sn * dist;
    vec3.y = -dist + -dist;
    vec3.z = sn * (dist * -0.5f) + cs * dist;
    vec4.x = cs * (dist * g_depthAverageHalf) - sn * dist;
    vec4.y = -dist + -dist;
    vec4.z = sn * (dist * g_depthAverageHalf) + cs * dist;
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, frame + 87, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    dist = slot.distance;
    cs = g_cosTable[slot.rotStart];
    sn = g_sinTable[slot.rotStart];
    vec1.x = cs * (dist * g_depthAverageHalf) - sn * 0.0f;
    vec1.y = 0.0f;
    vec1.z = sn * (dist * g_depthAverageHalf) + cs * 0.0f;
    vec2.x = cs * (dist * -0.5f) - sn * 0.0f;
    vec2.y = 0.0f;
    vec2.z = sn * (dist * -0.5f) + cs * 0.0f;
    vec3.x = cs * (dist * -0.5f) - sn * 0.0f;
    vec3.y = -dist;
    vec3.z = sn * (dist * -0.5f) + cs * 0.0f;
    vec4.x = cs * (dist * g_depthAverageHalf) - sn * 0.0f;
    vec4.y = -dist;
    vec4.z = sn * (dist * g_depthAverageHalf) + cs * 0.0f;
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, frame, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    dist = slot.distance;
    cs = g_cosTable[slot.rotStart];
    sn = g_sinTable[slot.rotStart];
    vec1.x = cs * (dist * g_depthAverageHalf) - sn * 0.0f;
    vec1.y = -dist;
    vec1.z = sn * (dist * g_depthAverageHalf) + cs * 0.0f;
    vec2.x = cs * (dist * -0.5f) - sn * 0.0f;
    vec2.y = -dist;
    vec2.z = sn * (dist * -0.5f) + cs * 0.0f;
    vec3.x = cs * (dist * -0.5f) - sn * 0.0f;
    vec3.y = -dist + -dist;
    vec3.z = sn * (dist * -0.5f) + cs * 0.0f;
    vec4.x = cs * (dist * g_depthAverageHalf) - sn * 0.0f;
    vec4.y = -dist + -dist;
    vec4.z = sn * (dist * g_depthAverageHalf) + cs * 0.0f;
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, frame + 30, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    dist = slot.distance;
    cs = g_cosTable[slot.rotStart];
    sn = g_sinTable[slot.rotStart];
    vec1.x = cs * (dist * 0.47f) - sn * -1.0f;
    vec1.y = dist * -0.0f;
    vec1.z = sn * (dist * 0.47f) + cs * -1.0f;
    vec2.x = cs * (dist * -0.47f) - sn * -1.0f;
    vec2.y = dist * -0.0f;
    vec2.z = sn * (dist * -0.47f) + cs * -1.0f;
    vec3.x = cs * (dist * -0.47f) - sn * -1.0f;
    vec3.y = dist * -0.6f;
    vec3.z = sn * (dist * -0.47f) + cs * -1.0f;
    vec4.x = cs * (dist * 0.47f) - sn * -1.0f;
    vec4.y = dist * -0.6f;
    vec4.z = sn * (dist * 0.47f) + cs * -1.0f;
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, static_cast<char>(slot.process % 27) + 60, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BF7960
// FUNCTION: fn_20003
void CEffectPrim::RenderHoverSquares(matrix& wtm) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  vector3d vec4{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];
    if (slot.life != 1) {
      continue;
    }

    vec1.y = 0.0f;
    vec2.y = 0.0f;
    vec3.y = 0.0f;
    vec4.y = 0.0f;
    const unsigned short red = static_cast<unsigned short>(slot.flag1[10] * 2.57f);
    const unsigned short green = static_cast<unsigned short>(slot.flag1[11] * 2.57f);
    const unsigned short blue = static_cast<unsigned short>(slot.flag1[12] * 2.57f);
    vec1.x = slot.distance * g_cosTable[45];
    vec1.z = slot.distance * g_sinTable[45];
    vec2.x = slot.distance * g_cosTable[135];
    vec2.z = slot.distance * g_sinTable[135];
    vec3.x = slot.distance * g_cosTable[225];
    vec3.z = slot.distance * g_sinTable[225];
    vec4.x = slot.distance * g_cosTable[315];
    vec4.z = slot.distance * g_sinTable[315];
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    const float x1 = vec1.x;
    const float y1 = vec1.y;
    const float z1 = vec1.z;
    const float x2 = vec2.x;
    const float y2 = vec2.y;
    const float z2 = vec2.z;
    const float x3 = vec3.x;
    const float y3 = vec3.y;
    const float z3 = vec3.z;
    const float x4 = vec4.x;
    const float y4 = vec4.y;
    const float z4 = vec4.z;

    vec1.x = slot.vecBPre.x + x1;
    vec1.y = slot.vecBPre.y + y1;
    vec1.z = slot.vecBPre.z + z1;
    vec2.x = slot.vecBPre.x + x2;
    vec2.y = slot.vecBPre.y + y2;
    vec2.z = slot.vecBPre.z + z2;
    vec3.x = slot.vecBPre.x + x3;
    vec3.y = slot.vecBPre.y + y3;
    vec3.z = slot.vecBPre.z + z3;
    vec4.x = slot.vecBPre.x + x4;
    vec4.y = slot.vecBPre.y + y4;
    vec4.z = slot.vecBPre.z + z4;
    int alpha = 50;
    if (vec1.y < -4.0f) {
      alpha = static_cast<int>((vec1.y + 4.0f) * 15.0f + 50.0f);
    }
    if (vec1.y > -0.5f) {
      alpha = static_cast<int>(vec1.y * -100.0f);
    }
    if (vec1.y <= 0.0f) {
      this->RenderTeiRect(&rpCnt, static_cast<short>(alpha < 0 ? 0 : alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }

    vec1.x = slot.vecBNow.x + x1;
    vec1.y = slot.vecBNow.y + y1;
    vec1.z = slot.vecBNow.z + z1;
    vec2.x = slot.vecBNow.x + x2;
    vec2.y = slot.vecBNow.y + y2;
    vec2.z = slot.vecBNow.z + z2;
    vec3.x = slot.vecBNow.x + x3;
    vec3.y = slot.vecBNow.y + y3;
    vec3.z = slot.vecBNow.z + z3;
    vec4.x = slot.vecBNow.x + x4;
    vec4.y = slot.vecBNow.y + y4;
    vec4.z = slot.vecBNow.z + z4;
    alpha = 50;
    if (vec1.y < -4.0f) {
      alpha = static_cast<int>((vec1.y + 4.0f) * 15.0f + 50.0f);
    }
    if (vec1.y > -0.5f) {
      alpha = static_cast<int>(vec1.y * -100.0f);
    }
    if (vec1.y <= 0.0f) {
      this->RenderTeiRect(&rpCnt, static_cast<short>(alpha < 0 ? 0 : alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }

    vec1.x = slot.vecTPre.x + x1;
    vec1.y = slot.vecTPre.y + y1;
    vec1.z = slot.vecTPre.z + z1;
    vec2.x = slot.vecTPre.x + x2;
    vec2.y = slot.vecTPre.y + y2;
    vec2.z = slot.vecTPre.z + z2;
    vec3.x = slot.vecTPre.x + x3;
    vec3.y = slot.vecTPre.y + y3;
    vec3.z = slot.vecTPre.z + z3;
    vec4.x = slot.vecTPre.x + x4;
    vec4.y = slot.vecTPre.y + y4;
    vec4.z = slot.vecTPre.z + z4;
    alpha = 50;
    if (vec1.y < -4.0f) {
      alpha = static_cast<int>((vec1.y + 4.0f) * 15.0f + 50.0f);
    }
    if (vec1.y > -0.5f) {
      alpha = static_cast<int>(vec1.y * -100.0f);
    }
    if (vec1.y <= 0.0f) {
      this->RenderTeiRect(&rpCnt, static_cast<short>(alpha < 0 ? 0 : alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }

    vec1.x = slot.vecTNow.x + x1;
    vec1.y = slot.vecTNow.y + y1;
    vec1.z = slot.vecTNow.z + z1;
    vec2.x = slot.vecTNow.x + x2;
    vec2.y = slot.vecTNow.y + y2;
    vec2.z = slot.vecTNow.z + z2;
    vec3.x = slot.vecTNow.x + x3;
    vec3.y = slot.vecTNow.y + y3;
    vec3.z = slot.vecTNow.z + z3;
    vec4.x = slot.vecTNow.x + x4;
    vec4.y = slot.vecTNow.y + y4;
    vec4.z = slot.vecTNow.z + z4;
    alpha = 50;
    if (vec1.y < -4.0f) {
      alpha = static_cast<int>((vec1.y + 4.0f) * 15.0f + 50.0f);
    }
    if (vec1.y > -0.5f) {
      alpha = static_cast<int>(vec1.y * -100.0f);
    }
    if (vec1.y <= 0.0f) {
      this->RenderTeiRect(&rpCnt, static_cast<short>(alpha < 0 ? 0 : alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}
