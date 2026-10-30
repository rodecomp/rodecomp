#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00BF5060
// FUNCTION: fn_19996
void CEffectPrim::RenderTwinGlowSquares(matrix& wtm) {
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
    int alpha = static_cast<int>(slot.height[12]);
    alpha = alpha < 0 ? 0 : alpha;
    if (slot.flag1[0] == 1) {
      this->RenderTeiRect(&rpCnt, alpha / 3, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      this->RenderTeiRect(&rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else if (slot.flag1[0] == 2) {
      this->RenderTeiRect(&rpCnt, alpha, vec2, vec1, vec4, vec3, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else if (slot.flag1[0] == 3) {
      this->RenderTeiRect(&rpCnt, alpha, vec2, vec1, vec4, vec3, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else {
      this->RenderTeiRect(&rpCnt, alpha / 3, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      this->RenderTeiRect(&rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
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
    alpha = static_cast<int>(slot.height[13]);
    alpha = alpha < 0 ? 0 : alpha;
    if (slot.flag1[0] == 1) {
      this->RenderTeiRect(&rpCnt, alpha / 3, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      this->RenderTeiRect(&rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 50, 50, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else if (slot.flag1[0] == 2) {
      this->RenderTeiRect(&rpCnt, alpha, vec2, vec1, vec4, vec3, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    } else if (slot.flag1[0] != 3) {
      this->RenderTeiRect(&rpCnt, alpha / 3, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
      this->RenderTeiRect(&rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 50, 50, 255, 0.0f, 1.0f, 0.0f, 1.0f, 1);
    }
  }
}
