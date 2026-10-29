#include "Contents/Effects/EffectPrimitiveRendering.h"
#include "Contents/Effects/RagEffect.h"
#include "Contents/Effects/EffectPrimitiveState.h"
#include "Contents/Effects/EffectVisibility.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Render/Projection/RenderProjection.h"
#include "Render/Primitives/RenderQueues.h"
#include "Contents/Effects/EffectPrim.h"
#include "Contents/Effects/EffectPrimitiveMotion.h"
#include "Contents/Effects/RagEffectState.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Utility/Math/AffineMatrix.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Camera/Camera.h"
#include "Contents/World/World.h"

// OFFSET: 0x00BCD960
// FUNCTION: fn_19865
void DrawEffectPrimitive15(EffectPrimitiveState& particle, const RenderObjectState& render, EffectPrimitiveRenderContext& context) {
  const float width = particle.widthSize;
  const float left = -width;
  float top = -particle.heightSize;
  float bottom = particle.heightSize;

  if ((particle.pattern & 0x80u) != 0) {
    top = top + top;
    bottom = 0.0f;
  }

  if ((particle.pattern & 8u) != 0) {
    bottom = 0.0f;
  }

  const float* matrix = particle.rotTransform;
  const float zeroX = matrix[0] * 0.0f;
  const float zeroY = matrix[1] * 0.0f;
  const float zeroZ = matrix[2] * 0.0f;
  const float topX = matrix[3] * top + zeroX;
  const float topY = matrix[4] * top + zeroY;
  const float topZ = matrix[5] * top + zeroZ;
  const float rightX = matrix[6] * width;
  const float rightY = matrix[7] * width;
  const float rightZ = matrix[8] * width;
  const float leftX = matrix[6] * left;
  const float leftY = matrix[7] * left;
  const float leftZ = matrix[8] * left;
  const float bottomX = bottom * matrix[3] + zeroX;
  const float bottomY = bottom * matrix[4] + zeroY;
  const float bottomZ = bottom * matrix[5] + zeroZ;

  const vector3d corners[4] = {
    {(rightX + topX) + matrix[9], (rightY + topY) + matrix[10], (rightZ + topZ) + matrix[11]},
    {(leftX + topX) + matrix[9], (leftY + topY) + matrix[10], (leftZ + topZ) + matrix[11]},
    {(bottomX + rightX) + matrix[9], (bottomY + rightY) + matrix[10], (bottomZ + rightZ) + matrix[11]},
    {(bottomX + leftX) + matrix[9], (bottomY + leftY) + matrix[10], (bottomZ + leftZ) + matrix[11]}
  };

  constexpr unsigned indices[2][3] = {{0, 1, 2}, {3, 2, 1}};
  constexpr float u[2][3] = {{0, 1, 0}, {1, 0, 1}};
  const float vTop = particle.v1;
  const float vBottom = particle.v2;
  const float v[2][3] = {{vTop, vTop, vBottom}, {vBottom, vBottom, vTop}};

  for (unsigned triangleIndex = 0; triangleIndex < 2; ++triangleIndex) {
    RenderTriangle& triangle = particle.triangles[triangleIndex];

    for (unsigned vertexIndex = 0; vertexIndex < 3; ++vertexIndex) {
      const vector3d& corner = corners[indices[triangleIndex][vertexIndex]];
      const float position[3] = {corner.x + render.position.x, corner.y + render.position.y, corner.z + render.position.z};
      RenderVertex projected{0, 0, 0, 0, 0, 0xFF000000u, 0, 0};
      context.projection.Project(position, context.cameraMatrix, projected);
      RenderVertex& vertex = triangle.vertices[vertexIndex];
      vertex = projected;
      vertex.diffuse = particle.argb;
      vertex.u = u[triangleIndex][vertexIndex];
      vertex.v = v[triangleIndex][vertexIndex];
    }

    triangle.texture = particle.textures[render.curMotion];
  }

  for (unsigned index = 0; index < 2; ++index) {
    RenderTriangle& triangle = particle.triangles[index];

    if (triangle.Visible()) {
      context.queues.SubmitTriangle(
        triangle, static_cast<std::uint16_t>(particle.renderFlag), context.commands, context.depthAlpha, context.depthStart, context.depthEnd
      );
    }
  }
}

// OFFSET: 0x00BCE350
// FUNCTION: fn_19866
void RenderEffectPrimitive15(
  EffectPrimitiveState& particle, const RenderObjectState& render, EffectPrimitiveRenderContext& context, const EffectVisibilityContext& visibility
) {
  if (particle.master && PartyEffectVisible(particle.master->state, visibility)) {
    DrawEffectPrimitive15(particle, render, context);
  }
}

// OFFSET: 0x00BE4210
// FUNCTION: fn_19923
void CEffectPrim::RenderTeiRect(
  int* rpCnt, short alpha, vector3d vec1, vector3d vec2, vector3d vec3, vector3d vec4, matrix& wtm, short pw, char tn, char f1, unsigned char r,
  unsigned char g, unsigned char b, float ty1, float ty2, float tx, float tx2, char extra
) {
  const RagEffectState& master = this->state.master->state;
  const int blend = pw != 0 ? 6 : 2;
  int red = r;
  if (master.unknown_11C88[0] != -1) {
    red = static_cast<unsigned char>(master.unknown_11C88[0]);
  }
  int green = g;
  if (master.unknown_11C88[1] != -1) {
    green = static_cast<unsigned char>(master.unknown_11C88[1]);
  }
  int blue = b;
  if (master.unknown_11C88[2] != -1) {
    blue = static_cast<unsigned char>(master.unknown_11C88[2]);
  }
  int destBlend = master.unknown_11C88[3];
  if (destBlend == -1) {
    destBlend = blend;
  }

  if (*rpCnt >= 510) {
    return;
  }

  const std::uint32_t color = static_cast<std::uint32_t>(((((alpha << 8) + static_cast<unsigned char>(red)) << 8) + static_cast<unsigned char>(green)) << 8) +
                              static_cast<unsigned char>(blue);

  if (f1 == 0) {
    const vector3d& position = this->render.position;
    vec1.x += position.x;
    vec1.y += position.y;
    vec1.z += position.z;
    vec2.x += position.x;
    vec2.y += position.y;
    vec2.z += position.z;
    vec3.x += position.x;
    vec3.y += position.y;
    vec3.z += position.z;
    vec4.x += position.x;
    vec4.y += position.y;
    vec4.z += position.z;
  }

  const vector3d* const corners[2][3] = {{&vec1, &vec2, &vec3}, {&vec4, &vec3, &vec1}};
  const float us[2][3] = {{tx, tx2, tx2}, {tx, tx2, tx}};
  const float vs[2][3] = {{ty2, ty2, ty1}, {ty1, ty1, ty2}};

  for (int half = 0; half < 2; ++half) {
    RenderTriangle& triangle = this->state.triangles[*rpCnt];

    if (pw == 10) {
      triangle.unknown_0064 = 2;
      triangle.unknown_0068 = 1;
    } else {
      triangle.unknown_0064 = 5;
      triangle.unknown_0068 = pw == 99 ? 6 : destBlend;
    }

    for (int index = 0; index < 3; ++index) {
      RenderVertex& vertex = triangle.vertices[index];
      vertex = TransformVert(wtm, *corners[half][index]);
      vertex.diffuse = color;
      vertex.u = us[half][index];
      vertex.v = vs[half][index];
    }

    triangle.texture = this->state.textures[tn];

    if (extra == 2) {
      for (int index = 0; index < 3; ++index) {
        RenderVertex& vertex = triangle.vertices[index];
        vertex.rhw += 0.000045f;
        vertex.z = RhwToDepth(vertex.rhw);
      }
    } else if (extra == 1 || extra == 3) {
      if (extra == 3) {
        const RenderVertex center = TransformVert(wtm, this->render.position);
        const float width = std::fabs(triangle.vertices[0].x - triangle.vertices[1].x) * 0.5f;
        triangle.vertices[0].x = center.x + width;
        triangle.vertices[1].x = center.x - width;
        triangle.vertices[2].x = half == 0 ? center.x - width : center.x + width;

        for (int index = 0; index < 3; ++index) {
          triangle.vertices[index].y += static_cast<float>(-30 * g_renderer->viewport.height / 480) * this->render.unknown_005C;
        }
      }

      for (int index = 0; index < 3; ++index) {
        triangle.vertices[index].z = 1e-09f;
        triangle.vertices[index].rhw = 1.0f;
      }
    }

    if (triangle.Visible()) {
      g_renderer->queues.SubmitTriangle(
        triangle, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
      );
      ++*rpCnt;
    }
  }
}

// OFFSET: 0x00BE4C60
// FUNCTION: fn_19924
void CEffectPrim::RenderTeiRect(
  int* rpCnt, int texPart, short alphaB, short alphaT, vector3d vec1, vector3d vec2, vector3d vec3, vector3d vec4, matrix& wtm, char pw
) {
  if (*rpCnt >= 510) {
    return;
  }

  int alpha = alphaB;
  int blend = 2;
  int red = 255;
  int green = 255;
  int blue = 255;
  float textureV = 1.0f;

  switch (static_cast<int>(this->state.size)) {
  case 0:
    blend = 6;
    break;
  case 1:
    green = 175;
    blue = 175;
    break;
  case 2:
    red = 195;
    green = 195;
    blend = 6;
    break;
  case 3:
    alpha = 80;
    blend = 6;
    break;
  case 4:
    red = 100;
    green = 100;
    break;
  case 6:
    green = 100;
    blue = 100;
    break;
  case 7:
    red = 100;
    blue = 100;
    break;
  case 8:
    blue = 170;
    break;
  case 9:
    red = 140;
    green = 210;
    blue = 140;
    break;
  case 10:
    green = 89;
    blue = 182;
    break;
  case 11:
    red = 170;
    green = 170;
    break;
  case 12:
    red = 50;
    green = 50;
    blue = 50;
    blend = 6;
    break;
  case 13:
    green = 50;
    blend = 6;
    break;
  case 14:
    red = 120;
    green = 120;
    blend = 6;
    break;
  case 15:
    red = 10;
    blue = 10;
    break;
  case 16:
    red = 20;
    blue = 20;
    break;
  case 17:
    red = 0;
    green = 0;
    break;
  case 18:
    green = 0;
    blue = 0;
    break;
  case 19:
    red = 0;
    blue = 0;
    break;
  case 20:
    blue = 0;
    break;
  case 21:
    green = 0;
    break;
  case 22:
    red = 0;
    break;
  case 23:
    red = 41;
    green = 94;
    blue = 188;
    blend = 6;
    break;
  case 24:
    red = 10;
    green = 66;
    blue = 114;
    break;
  case 25:
    red = 35;
    green = 146;
    blue = 185;
    break;
  case 26:
    red = 164;
    green = 205;
    blue = 254;
    break;
  case 27:
    red = 84;
    green = 169;
    blue = 205;
    break;
  case 28:
    red = 108;
    green = 26;
    break;
  case 29:
    red = 177;
    blue = 58;
    break;
  case 30:
    red = 85;
    green = 30;
    blue = 185;
    break;
  case 31:
    red = 244;
    green = 187;
    blue = 124;
    break;
  case 32:
    red = 0;
    green = 0;
    blue = 0;
    blend = 6;
    break;
  case 40:
    red = 70;
    green = 70;
    break;
  case 41:
    red = 30;
    green = 30;
    break;
  case 42:
    red = 43;
    green = 40;
    blue = 46;
    blend = 6;
    break;
  case 43:
    red = 125;
    green = 182;
    blue = 160;
    blend = 6;
    break;
  case 44:
    red = 180;
    green = 55;
    blue = 30;
    blend = 6;
    break;
  case 45:
    red = 65;
    green = 141;
    blue = 231;
    break;
  case 46:
    red = 164;
    green = 47;
    blue = 29;
    break;
  case 47:
    red = 172;
    green = 154;
    blue = 56;
    break;
  case 48:
    red = 80;
    green = 20;
    blue = 20;
    break;
  case 49:
    red = 200;
    green = 130;
    blue = 84;
    break;
  case 50:
    green = 198;
    blue = 91;
    break;
  case 100:
    red = 250;
    green = 250;
    textureV = 0.9f;
    blend = 6;
    break;
  default:
    break;
  }

  const RagEffectState& master = this->state.master->state;
  if (master.unknown_11C88[3] != -1) {
    blend = master.unknown_11C88[3];
  }
  if (master.unknown_11C88[0] != -1) {
    red = master.unknown_11C88[0];
  }
  if (master.unknown_11C88[1] != -1) {
    green = master.unknown_11C88[1];
  }
  if (master.unknown_11C88[2] != -1) {
    blue = master.unknown_11C88[2];
  }
  const std::uint32_t color = static_cast<std::uint32_t>(((((alpha << 8) + red) << 8) + green) << 8) + blue;

  float left;
  float right;
  if (texPart < 100) {
    left = static_cast<float>(texPart) / 21.0f;
    if (texPart == 0) {
      left = 0.0f;
    }
    right = static_cast<float>(texPart + 1) / 21.0f;
  } else if (texPart == 100) {
    left = 0.35f;
    right = 0.45f;
  } else if (texPart == 101) {
    left = 0.45f;
    right = 0.55f;
  } else {
    left = 0.55f;
    right = 0.65f;
  }

  const vector3d& position = this->render.position;
  vec1.x += position.x;
  vec1.y += position.y;
  vec1.z += position.z;
  vec2.x += position.x;
  vec2.y += position.y;
  vec2.z += position.z;
  vec3.x += position.x;
  vec3.y += position.y;
  vec3.z += position.z;
  vec4.x += position.x;
  vec4.y += position.y;
  vec4.z += position.z;

  const vector3d* const corners[2][3] = {{&vec1, &vec2, &vec3}, {&vec4, &vec3, &vec1}};
  const float us[2][3] = {{left, right, right}, {left, right, left}};
  const float vs[2][3] = {{textureV, textureV, 0.0f}, {0.0f, 0.0f, textureV}};

  for (int half = 0; half < 2; ++half) {
    RenderTriangle& triangle = this->state.triangles[*rpCnt];
    triangle.unknown_0064 = 5;
    triangle.unknown_0068 = blend;

    for (int index = 0; index < 3; ++index) {
      RenderVertex& vertex = triangle.vertices[index];
      vertex = TransformVert(wtm, *corners[half][index]);
      vertex.diffuse = color;
      vertex.u = us[half][index];
      vertex.v = vs[half][index];
    }

    triangle.texture = this->state.textures[pw];

    if (triangle.Visible()) {
      g_renderer->queues.SubmitTriangle(
        triangle, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
      );
      ++*rpCnt;
    }
  }
}

// OFFSET: 0x00BE5640
// FUNCTION: fn_19925
void CEffectPrim::RenderTeiRect(
  int* rpCnt, int texPart, short alphaB, short alphaT, vector3d vec1, vector3d vec2, vector3d vec3, vector3d vec4, matrix& wtm, char pw, unsigned char r,
  unsigned char g, unsigned char b
) {
  const RagEffectState& master = this->state.master->state;
  const int blend = pw != 0 ? 6 : 2;
  int red = r;
  if (master.unknown_11C88[0] != -1) {
    red = static_cast<unsigned char>(master.unknown_11C88[0]);
  }
  int green = g;
  if (master.unknown_11C88[1] != -1) {
    green = static_cast<unsigned char>(master.unknown_11C88[1]);
  }
  int blue = b;
  if (master.unknown_11C88[2] != -1) {
    blue = static_cast<unsigned char>(master.unknown_11C88[2]);
  }
  int destBlend = master.unknown_11C88[3];
  if (destBlend == -1) {
    destBlend = blend;
  }

  if (*rpCnt >= 510) {
    return;
  }

  const std::uint32_t color = static_cast<std::uint32_t>(((((alphaB << 8) + static_cast<unsigned char>(red)) << 8) + static_cast<unsigned char>(green)) << 8) +
                              static_cast<unsigned char>(blue);
  float top = static_cast<float>(texPart) / 21.0f;
  if (texPart == 0) {
    top = 0.0f;
  }
  const float bottom = static_cast<float>(texPart + 1) / 21.0f;

  const vector3d& position = this->render.position;
  vec1.x += position.x;
  vec1.y += position.y;
  vec1.z += position.z;
  vec2.x += position.x;
  vec2.y += position.y;
  vec2.z += position.z;
  vec3.x += position.x;
  vec3.y += position.y;
  vec3.z += position.z;
  vec4.x += position.x;
  vec4.y += position.y;
  vec4.z += position.z;

  const vector3d* const corners[2][3] = {{&vec1, &vec2, &vec3}, {&vec4, &vec3, &vec1}};
  const float us[2][3] = {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}};
  const float vs[2][3] = {{top, bottom, bottom}, {top, bottom, top}};

  for (int half = 0; half < 2; ++half) {
    RenderTriangle& triangle = this->state.triangles[*rpCnt];
    triangle.unknown_0064 = 5;
    triangle.unknown_0068 = destBlend;

    for (int index = 0; index < 3; ++index) {
      RenderVertex& vertex = triangle.vertices[index];
      vertex = TransformVert(wtm, *corners[half][index]);
      vertex.diffuse = color;
      vertex.u = us[half][index];
      vertex.v = vs[half][index];
    }

    triangle.texture = this->state.textures[0];

    if (triangle.Visible()) {
      g_renderer->queues.SubmitTriangle(
        triangle, static_cast<std::uint16_t>(this->state.renderFlag), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
      );
      ++*rpCnt;
    }
  }
}

// OFFSET: 0x00BD2AB0
// FUNCTION: fn_19875
void CEffectPrim::RenderHalfSphere2(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 10;
    int level = 0;

    for (int ring = 10; ring != 0; --ring) {
      for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
        const int angle = (slot.rotStart + count) % 360;
        const float cs = g_cosTable[angle];
        const float sn = g_sinTable[angle];
        const float dist = slot.distance;
        slot.vecBNow = {dist * cs, 0.0f, dist * sn};

        const float radius = slot.height[level];
        const float spread = radius * g_cosTable[slot.riseAngle];
        slot.vecTNow = {spread * cs + slot.vecBNow.x, -(radius * g_sinTable[slot.riseAngle]), spread * sn + slot.vecBNow.z};

        if (level > 0) {
          this->RenderTeiRect(&rpCnt, level - 1, slot.alphaB, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0);
        }

        ++level;
        if (level >= 21) {
          level = 20;
        }

        slot.vecBPre = slot.vecBNow;
        slot.vecTPre = slot.vecTNow;
      }
    }
  }
}

// OFFSET: 0x00BDAC40
// FUNCTION: fn_19894
void CEffectPrim::RenderHalfSphere(matrix& wtm) {
  vector3d vec1{};
  vector3d vec3{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 10;

    for (int angle2 = 0; angle2 < 180; angle2 += 18) {
      for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
        int angle = slot.rotStart + count;

        if (angle >= 360) {
          angle -= 360;
        }

        if (slot.fullDisplayAngle == 360 && count == 360) {
          angle = slot.rotStart;
        }

        const float cs1 = g_cosTable[angle];
        const float sn1 = g_sinTable[angle];
        const float dist = slot.distance;

        float ring = g_sinTable[angle2] * dist;
        const vector3d vec2{cs1 * ring + slot.vecTNow.x, g_cosTable[angle2] * dist - slot.maxHeight, sn1 * ring + slot.vecTNow.z};
        ring = g_sinTable[angle2 + 18] * dist;
        const vector3d vec4{cs1 * ring + slot.vecTNow.x, g_cosTable[angle2 + 18] * dist - slot.maxHeight, sn1 * ring + slot.vecTNow.z};

        if (count > 0) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 1, 0, 0, 250, 250, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }

        vec1 = vec2;
        vec3 = vec4;
      }
    }
  }
}

// OFFSET: 0x00BDAEF0
// FUNCTION: fn_19895
void CEffectPrim::RenderHermode(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    int angle = slot.rotStart;
    const float snA = g_sinTable[angle] * 2.0f;
    const float rx = slot.distance * 0.1f * (g_sinTable[slot.riseAngle] + 1.0f);
    const double radius = slot.distance * 0.8 + rx;
    const float y = slot.height[0] + snA;

    slot.vecBPre = {static_cast<float>(g_cosTable[angle] * radius), y, static_cast<float>(g_sinTable[angle] * radius)};
    angle += angle >= 270 ? -270 : 90;
    slot.vecBNow = {static_cast<float>(g_cosTable[angle] * radius), y, static_cast<float>(g_sinTable[angle] * radius)};
    angle += angle + 90 >= 360 ? -270 : 90;
    slot.vecTNow = {static_cast<float>(g_cosTable[angle] * radius), y, static_cast<float>(g_sinTable[angle] * radius)};
    angle += angle + 90 >= 360 ? -270 : 90;
    slot.vecTPre = {static_cast<float>(g_cosTable[angle] * radius), y, static_cast<float>(g_sinTable[angle] * radius)};

    vector3d vec1 = slot.vecBPre;
    vec1.y -= slot.distance;
    vector3d vec2 = slot.vecBNow;
    vec2.y -= slot.distance;
    vector3d vec3 = slot.vecTPre;
    vec3.y -= slot.distance;
    vector3d vec4 = slot.vecTNow;
    vec4.y -= slot.distance;

    const unsigned char shade = static_cast<unsigned char>(slot.flag1[0]);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0, 0, 0, shade, shade, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, 0, 0, 0, shade, shade, 220, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, 0, 0, 0, shade, shade, 190, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, 0, 0, 0, shade, shade, 130, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, 0, 0, 0, shade, shade, 160, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 0, 0, 0, shade, shade, 250, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BDB530
// FUNCTION: fn_19896
void CEffectPrim::RenderHitLine3(matrix& wtm) {
  vector3d vec1{};
  vector3d vec3{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0 || slot.process <= 0) {
      continue;
    }

    for (int j = 0; static_cast<float>(j) < slot.vecTNow.y; ++j) {
      float speed = 0.0f;

      if (slot.height[j] > 0.0f) {
        speed += slot.height[j] / 90.0f * slot.distance;
      }

      vector3d vecA{
        speed * g_cosTable[slot.riseAngle], -slot.distance * g_sinTable[static_cast<std::int16_t>(slot.height[j])] * slot.vecTNow.x,
        speed * g_sinTable[slot.riseAngle]
      };

      speed = 0.0f;

      if (slot.height[j + 1] > 0.0f) {
        speed += slot.height[j + 1] / 90.0f * slot.distance;
      }

      vector3d vecB{
        speed * g_cosTable[slot.riseAngle], -slot.distance * g_sinTable[static_cast<std::int16_t>(slot.height[j + 1])] * slot.vecTNow.x,
        speed * g_sinTable[slot.riseAngle]
      };

      std::int16_t angle;
      Get_Angle(vecA.x - vecB.x, vecA.z - vecB.z, &angle);
      vecA.x += slot.vecBNow.x;
      vecA.y += slot.vecBNow.y;
      vecA.z += slot.vecBNow.z;
      vecB.x += slot.vecBNow.x;
      vecB.y += slot.vecBNow.y;
      vecB.z += slot.vecBNow.z;
      angle += 90;

      if (angle >= 360) {
        angle -= 360;
      }

      for (int i = 0; i <= 3; ++i) {
        const float radius = slot.maxHeight;
        const float tx = radius * g_cosTable[i * 120];
        const float ry = radius * g_sinTable[i * 120] - 5.0f;
        const float rx = tx * g_cosTable[angle];
        const float rz = tx * g_sinTable[angle];
        const vector3d vec2{vecA.x + rx, vecA.y + ry, vecA.z + rz};
        const vector3d vec4{vecB.x + rx, vecB.y + ry, vecB.z + rz};

        if (i > 0) {
          const int alpha = slot.alphaB - j * 10;

          if (alpha > 0) {
            if (slot.alphaT == 0) {
              this->RenderTeiRect(&rpCnt, alpha, vec2, vec4, vec3, vec1, wtm, 0, 0, 1, 255, 200, 150 - (5 - j) * 30, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.alphaT == 1) {
              this->RenderTeiRect(&rpCnt, alpha, vec2, vec4, vec3, vec1, wtm, 0, 0, 1, 120 - (5 - j) * 22, 120 - (5 - j) * 22, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.alphaT == 2) {
              this->RenderTeiRect(&rpCnt, alpha, vec2, vec4, vec3, vec1, wtm, 0, 0, 1, 255, 200, 150 - (3 - j) * 50, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.alphaT >= 3) {
              const unsigned char grey = static_cast<unsigned char>(255 - (5 - j) * 30);
              this->RenderTeiRect(&rpCnt, alpha, vec2, vec4, vec3, vec1, wtm, 0, 0, 1, grey, grey, grey, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            }
          }
        }

        vec1 = vec2;
        vec3 = vec4;
      }

      if (slot.alphaT >= 3) {
        for (int i = 0; i <= 3; ++i) {
          const float radius = slot.maxHeight * 4.0f;
          const float tx = radius * g_cosTable[i * 120];
          const float ry = radius * g_sinTable[i * 120] - 5.0f;
          const float rx = tx * g_cosTable[angle];
          const float rz = tx * g_sinTable[angle];
          const vector3d vec2{vecA.x + rx, vecA.y + ry, vecA.z + rz};
          const vector3d vec4{vecB.x + rx, vecB.y + ry, vecB.z + rz};

          if (i > 0) {
            const int alpha = static_cast<int>(static_cast<float>(slot.alphaB - j * 10) * 0.5f);

            if (alpha > 0 && slot.alphaT == 3) {
              this->RenderTeiRect(&rpCnt, alpha, vec2, vec4, vec3, vec1, wtm, 0, 0, 1, 0, 0, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            }
          }

          vec1 = vec2;
          vec3 = vec4;
        }
      }
    }
  }
}

// OFFSET: 0x00BDBC50
// FUNCTION: fn_19897
void CEffectPrim::RenderHitLine(matrix& wtm) {
  vector3d vec1{};
  vector3d vec3{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0 || slot.process <= 0) {
      continue;
    }

    for (int j = 0; j < 20; ++j) {
      float speed = 0.0f;
      float radius = 1.0f;

      for (int i = 0; i < slot.flag1[j]; ++i) {
        speed += slot.distance * 2.0f * radius;
        radius *= 0.7f;
      }

      if (slot.height[j] > 0.0f) {
        speed += slot.distance * radius * (slot.height[j] / 90.0f);
      }

      radius = 1.0f;

      for (int i = 0; i < slot.flag1[j]; ++i) {
        radius *= 0.7f;
      }

      vector3d vecA{
        speed * g_cosTable[slot.riseAngle], -slot.distance * g_sinTable[static_cast<std::int16_t>(slot.height[j])] * 1.5f * radius,
        speed * g_sinTable[slot.riseAngle]
      };

      speed = 0.0f;
      radius = 1.0f;

      for (int i = 0; i < slot.flag1[j + 1]; ++i) {
        speed += slot.distance * 2.0f * radius;
        radius *= 0.7f;
      }

      if (slot.height[j + 1] > 0.0f) {
        speed += slot.distance * radius * (slot.height[j + 1] / 90.0f);
      }

      radius = 1.0f;

      for (int i = 0; i < slot.flag1[j + 1]; ++i) {
        radius *= 0.7f;
      }

      vector3d vecB{
        speed * g_cosTable[slot.riseAngle], -slot.distance * g_sinTable[static_cast<std::int16_t>(slot.height[j + 1])] * 1.5f * radius,
        speed * g_sinTable[slot.riseAngle]
      };

      std::int16_t angle;
      Get_Angle(vecA.x - vecB.x, vecA.z - vecB.z, &angle);
      vecA.x += slot.vecBNow.x;
      vecA.y += slot.vecBNow.y;
      vecA.z += slot.vecBNow.z;
      vecB.x += slot.vecBNow.x;
      vecB.y += slot.vecBNow.y;
      vecB.z += slot.vecBNow.z;
      angle = static_cast<std::int16_t>((angle + 90) % 360);

      for (int i = 0; i <= 3; ++i) {
        const float tx = slot.maxHeight * g_cosTable[i * 120];
        const float ry = slot.maxHeight * g_sinTable[i * 120];
        const float rx = tx * g_cosTable[angle];
        const float rz = tx * g_sinTable[angle];
        const vector3d vec2{vecA.x + rx, vecA.y + ry, vecA.z + rz};
        const vector3d vec4{vecB.x + rx, vecB.y + ry, vecB.z + rz};

        if (i > 0) {
          const int alpha = slot.alphaB - j * 5;

          if (alpha > 0) {
            if (slot.alphaT == 2) {
              const unsigned char shade = static_cast<unsigned char>(200 - (20 - j) * 7);
              this->RenderTeiRect(&rpCnt, alpha, vec1, vec2, vec4, vec3, wtm, 0, 0, 1, 255, shade, shade, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else if (slot.alphaT == 8) {
              const unsigned char shade = static_cast<unsigned char>(150 - (20 - j) * 7);
              this->RenderTeiRect(&rpCnt, alpha, vec1, vec2, vec4, vec3, wtm, 0, 0, 1, shade, shade, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            } else {
              this->RenderTeiRect(&rpCnt, alpha, vec1, vec2, vec4, vec3, wtm, 0, 0, 1, 255, 200, 150 - (20 - j) * 7, 0.0f, 1.0f, 0.0f, 1.0f, 0);
            }
          }
        }

        vec1 = vec2;
        vec3 = vec4;
      }
    }
  }
}

// OFFSET: 0x00BDD980
// FUNCTION: fn_19902
void CEffectPrim::RenderMagnum2(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (rpCnt >= 256 || slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int order = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
      int angle = slot.rotStart + count;

      if (angle >= 360) {
        angle -= 360;
      }

      if (slot.fullDisplayAngle == 360 && order == 20) {
        angle = slot.rotStart;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];
      slot.vecBNow.x = cs1 * slot.distance;
      slot.vecBNow.y = -3.0f;
      slot.vecBNow.z = sn1 * slot.distance;

      const float rx = g_cosTable[slot.riseAngle] * slot.maxHeight;
      const float ry = g_sinTable[slot.riseAngle] * slot.maxHeight;
      slot.vecTNow.x = cs1 * rx + slot.vecBNow.x;
      slot.vecTNow.y = -(ry + 3.0f);
      slot.vecTNow.z = sn1 * rx + slot.vecBNow.z;

      if (order > 0 && slot.alphaB > 0) {
        this->RenderTeiRect(&rpCnt, order - 1, slot.alphaB, slot.alphaB, slot.vecBPre, slot.vecBNow, slot.vecTNow, slot.vecTPre, wtm, 0);
      }

      ++order;

      if (order >= 21) {
        order = 20;
      }

      slot.vecBPre = slot.vecBNow;
      slot.vecTPre = slot.vecTNow;
    }
  }
}

// OFFSET: 0x00BD8870
// FUNCTION: fn_19889
void CEffectPrim::RenderRipple(matrix& wtm) {
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

    for (int ring = 10; ring >= 1; --ring) {
      const char kind = slot.flag1[4];
      int spread = 10 - ring;
      if (kind != 2) {
        spread *= 3;
      }

      const float rx = static_cast<float>(spread) + slot.distance;
      slot.vecBNow = {rx * g_cosTable[45], 0.0f, rx * g_sinTable[45]};
      slot.vecTNow = {rx * g_cosTable[135], 0.0f, rx * g_sinTable[135]};
      slot.vecBPre = {rx * g_cosTable[225], 0.0f, rx * g_sinTable[225]};
      slot.vecTPre = {rx * g_cosTable[315], 0.0f, rx * g_sinTable[315]};

      const float base = slot.height[0];
      vec1 = {slot.vecBNow.x + base, slot.vecBNow.y, slot.vecBNow.z};
      vec2 = {slot.vecTNow.x + base, slot.vecTNow.y, slot.vecTNow.z};
      vec3 = {slot.vecTPre.x + base, slot.vecTPre.y, slot.vecTPre.z};
      vec4 = {slot.vecBPre.x + base, slot.vecBPre.y, slot.vecBPre.z};
      this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

      const float drop = kind == 2 ? 40.0f : 30.0f;
      vec1.y -= drop;
      vec2.y -= drop;
      vec3.y -= drop;
      vec4.y -= drop;

      const int alpha = static_cast<int>(slot.height[ring - 1]);
      if (alpha > 0) {
        const unsigned char shade = ring == 10 ? 55 : 5;
        this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec2, vec1, vec4, vec3, wtm, 0, slot.flag1[0], 0, shade, shade, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    }
  }
}

// OFFSET: 0x00BDE720
// FUNCTION: fn_19905
void CEffectPrim::RenderPeongUp(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vector3d vec1{g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx};
    vector3d vec2{g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx};
    vector3d vec3{g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx};
    vector3d vec4{g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx};
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    const vector3d& base = slot.vecBPre;
    vec1 = {vec1.x + base.x, vec1.y + base.y, vec1.z + base.z};
    vec2 = {vec2.x + base.x, vec2.y + base.y, vec2.z + base.z};
    vec3 = {vec3.x + base.x, vec3.y + base.y, vec3.z + base.z};
    vec4 = {vec4.x + base.x, vec4.y + base.y, vec4.z + base.z};

    if (vec1.y <= 0.0f) {
      if (slot.flag1[0] == 2) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 105, 105, 225, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      } else if (slot.flag1[0] == 3) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 55, 55, 55, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      } else {
        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    }
  }
}

// OFFSET: 0x00BDEA00
// FUNCTION: fn_19906
void CEffectPrim::RenderGrid(matrix& wtm) {
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

    for (int x = -150; x < 150; x += 30) {
      for (int z = -90; z < 90; z += 30) {
        const float size = slot.distance;
        const float base = slot.maxHeight;
        vec1 = {base + static_cast<float>(x), -10.0f, base + static_cast<float>(z)};
        vec2 = {size + static_cast<float>(x) + base, -10.0f, vec1.z};
        vec3 = {vec2.x, -10.0f, size + static_cast<float>(z) + base};
        vec4 = {vec1.x, -10.0f, vec3.z};
        this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

        unsigned char red;
        unsigned char green;
        unsigned char blue;
        const char color = slot.flag1[10];
        if (color == 0) {
          red = 255;
          green = 255;
          blue = 255;
        } else if (color == 1) {
          red = 255;
          green = 0;
          blue = 0;
        } else if (color == 3) {
          red = 255;
          green = 255;
          blue = 255;
        } else {
          red = 0;
          green = 0;
          blue = 255;
        }

        this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    }
  }
}

// OFFSET: 0x00BCFC00
// FUNCTION: fn_19867
void CEffectPrim::RenderRing(matrix& wtm) {
  vector3d corners[4]{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float dist = slot.distance;
    const float cs = g_cosTable[slot.riseAngle];
    const float sn = g_sinTable[slot.riseAngle];
    const vector3d& center = slot.vecBNow;
    const int angles[4] = {90, 180, 270, 0};

    for (int index = 0; index < 4; ++index) {
      const float radial = dist * g_cosTable[angles[index]];
      corners[index] = {cs * radial + center.x, dist * g_sinTable[angles[index]] + center.y, sn * radial + center.z};
    }

    this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 0, 0, 1, 60, 60, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BD85F0
// FUNCTION: fn_19888
void CEffectPrim::RenderPentagon(matrix& wtm) {
  vector3d vec1{};
  vector3d vec4{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const vector3d& top = slot.vecTPre;
    const float radius = slot.maxHeight;

    for (int point = 0; point <= 5; ++point) {
      int angle = slot.rotStart + point * 72;
      if (angle >= 360) {
        angle -= 360;
      }

      const float dx = radius * g_cosTable[angle];
      const float dz = radius * g_sinTable[angle];
      const vector3d vec2{dx + slot.vecTNow.x, slot.vecTNow.y, dz + slot.vecTNow.z};
      const vector3d vec3{top.x + dx, top.y, top.z + dz};

      if (point != 0) {
        if (slot.flag1[0] == 13) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 80, 80, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 1, 230, 255, 230, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      }

      vec1 = vec2;
      vec4 = vec3;
    }
  }
}

// OFFSET: 0x00BE0990

// OFFSET: 0x00BDD0D0
// FUNCTION: fn_19900
void CEffectPrim::RenderBeam(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float lift = slot.height[3];
    const float length = slot.distance * g_sinTable[slot.fullDisplayAngle] + slot.distance;
    const float reach = length * g_cosTable[slot.riseAngle];
    const vector3d tip{reach * g_cosTable[slot.rotStart], length * g_sinTable[slot.riseAngle] - lift, reach * g_sinTable[slot.rotStart]};
    const unsigned char red = static_cast<unsigned char>(static_cast<int>(slot.height[0]));
    const unsigned char green = static_cast<unsigned char>(static_cast<int>(slot.height[1]));
    const unsigned char blue = static_cast<unsigned char>(static_cast<int>(slot.height[2]));

    this->RenderTeiRect(&rpCnt, slot.alphaB, vector3d{0.5f, -lift, 0.0f}, tip, tip, vector3d{-0.5f, -lift, 0.0f}, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, vector3d{0.0f, -lift, 0.5f}, tip, tip, vector3d{0.0f, -lift, -0.5f}, wtm, 0, 0, 0, red, green, blue, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BD0A20
// FUNCTION: fn_19870
void CEffectPrim::RenderCone(matrix& wtm) {
  vector3d vec1{};
  vector3d vec4{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0 || slot.fullDisplayAngle < 0) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int texPart = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
      int angle = slot.riseAngle + count;
      if (angle >= 360) {
        angle -= 360;
      }

      if (slot.fullDisplayAngle == 360 && count == 360) {
        angle = slot.riseAngle;
      }

      const float scale = slot.height[0];
      const float cs = g_cosTable[angle];
      const float inner = slot.distance * g_sinTable[angle];
      const float outerRadius = scale * slot.distance * 0.1f + slot.distance;
      const float outer = outerRadius * g_sinTable[angle];
      const vector3d vec2{slot.vecTNow.x + inner * g_cosTable[slot.rotStart], slot.distance * cs - slot.maxHeight, slot.vecTNow.z + inner * g_sinTable[slot.rotStart]};
      const vector3d vec3{
        outer * g_cosTable[slot.rotStart] - slot.vecTNow.x * scale, outerRadius * cs - slot.maxHeight, outer * g_sinTable[slot.rotStart] - slot.vecTNow.z * scale
      };

      if (count > 0) {
        this->RenderTeiRect(&rpCnt, texPart - 1, slot.alphaB, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0);
      }

      if (++texPart > 20) {
        texPart = 20;
      }

      vec1 = vec2;
      vec4 = vec3;
    }
  }
}

// OFFSET: 0x00BDE460
// FUNCTION: fn_19904
void CEffectPrim::RenderPeong(matrix& wtm) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  vector3d vec4{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vec1 = {g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx};
    vec2 = {g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx};
    vec3 = {g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx};
    vec4 = {g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx};
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    const vector3d& base = slot.vecBPre;
    vec1 = {vec1.x + base.x, vec1.y + base.y, vec1.z + base.z};
    vec2 = {vec2.x + base.x, vec2.y + base.y, vec2.z + base.z};
    vec3 = {vec3.x + base.x, vec3.y + base.y, vec3.z + base.z};
    vec4 = {vec4.x + base.x, vec4.y + base.y, vec4.z + base.z};
    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BD2CF0
// FUNCTION: fn_19876
void CEffectPrim::RenderFloorRect(matrix& wtm) {
  vector3d vec1{};
  vector3d vec2{};
  vector3d vec3{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const int rot = slot.riseAngle;
    const float back = slot.maxHeight;
    const float width = slot.height[0];
    const int left = rot >= 90 ? rot - 90 : rot + 270;
    const float baseX = width * g_cosTable[left] + back * g_cosTable[left];
    const float baseZ = width * g_sinTable[left] + back * g_sinTable[left];
    const float length = slot.distance;
    const int forward = rot < 0 ? rot + 360 : rot;
    const int backward = rot - 180 < 0 ? rot - 180 + 360 : rot - 180;
    const int right = rot < 270 ? rot + 90 : rot - 270;
    const float sideX = back * g_cosTable[right] + width * g_cosTable[left];
    const float sideZ = back * g_sinTable[right] + width * g_sinTable[left];

    vec3 = {length * g_cosTable[forward] + baseX, 0.0f, length * g_sinTable[forward] + baseZ};
    vec2 = {length * g_cosTable[backward] + baseX, 0.0f, length * g_sinTable[backward] + baseZ};
    vec1 = {length * g_cosTable[forward] + sideX, 0.0f, length * g_sinTable[forward] + sideZ};
    const vector3d vec4{length * g_cosTable[backward] + sideX, 0.0f, length * g_sinTable[backward] + sideZ};

    this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec4, vec2, vec3, wtm, 1, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BE0B40
// FUNCTION: fn_19913
void CEffectPrim::RenderRotatedRing(matrix& wtm) {
  vector3d corners[4]{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float dist = slot.distance;
    const float cs = g_cosTable[slot.rotStart];
    const float sn = g_sinTable[slot.rotStart];
    const vector3d& center = slot.vecBNow;
    int angle = slot.riseAngle;

    for (int index = 0; index < 4; ++index) {
      angle = angle < 270 ? angle + 90 : angle - 270;
      const float radial = dist * g_cosTable[angle];
      corners[index] = {cs * radial + center.x, dist * g_sinTable[angle] + center.y, sn * radial + center.z};
    }

    this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 0, 0, 1, 80, 255, 80, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BE1A80
// FUNCTION: fn_19917
void CEffectPrim::RenderFloorSquare(matrix& wtm) {
  vector3d corners[4]{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float dist = slot.distance;
    const vector3d& center = slot.vecBNow;
    const float y = center.y + 0.0f;
    int angle = slot.riseAngle + 45 < 360 ? slot.riseAngle + 45 : slot.riseAngle - 315;

    for (int index = 0; index < 4; ++index) {
      corners[index] = {dist * g_cosTable[angle] + center.x, y, dist * g_sinTable[angle] + center.z};
      angle = angle + 90 < 360 ? angle + 90 : angle - 270;
    }

    if (slot.flag1[1] == 1) {
      const char frame = slot.process > 0 ? static_cast<char>(slot.process / 6 % 4) : 0;
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 1, frame, 1, 205, 205, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (slot.flag1[1] == 2) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 1, 0, 1, 0, 255, 0, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 1, 0, 1, 200, 200, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BE0E30
// FUNCTION: fn_19914
void CEffectPrim::RenderFloorSquare2(matrix& wtm) {
  vector3d corners[4]{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float dist = slot.distance;
    const vector3d& center = slot.vecBNow;
    const float y = center.y + 0.0f;
    int angle = slot.riseAngle < 270 ? slot.riseAngle + 90 : slot.riseAngle - 270;

    for (int index = 0; index < 4; ++index) {
      corners[index] = {dist * g_cosTable[angle] + center.x, y, dist * g_sinTable[angle] + center.z};
      angle = angle + 90 < 360 ? angle + 90 : angle - 270;
    }

    switch (slot.flag1[3]) {
    case 0:
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 0, 0, 1, 80, 80, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 1:
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 0, 0, 1, 80, 255, 80, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 2:
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 1, 0, 1, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    case 3:
      this->RenderTeiRect(&rpCnt, slot.alphaB, corners[0], corners[1], corners[2], corners[3], wtm, 1, 0, 1, 70, 70, 70, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      break;
    }
  }
}

// OFFSET: 0x00BD1CB0
// FUNCTION: fn_19873
void CEffectPrim::RenderColorPlane(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float height = -slot.maxHeight;
    const vector3d vec1{slot.vecBNow.x, 0.0f, slot.vecBNow.z};
    const vector3d vec2{slot.vecBPre.x, 0.0f, slot.vecBPre.z};
    const vector3d vec3{slot.vecBPre.x, height, slot.vecBPre.z};
    const vector3d vec4{slot.vecBNow.x, height, slot.vecBNow.z};
    const float kind = slot.height[0];

    if (kind == 1.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 255, 155, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (kind == 2.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 155, 255, 155, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (kind == 3.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 100, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else if (kind == 4.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 150, 150, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BDF0A0
// FUNCTION: fn_19908
void CEffectPrim::RenderRainbow(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;

    for (int rcolor = 0; rcolor < 7; ++rcolor) {
      int order = 0;
      const float inner = static_cast<float>(1.0 - rcolor * 0.05f);
      const float outer = static_cast<float>(1.0 - (rcolor + 1) * 0.05f);

      for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
        int angle = slot.rotStart + count;

        if (angle >= 360) {
          angle -= 360;
        }

        const float cs = g_cosTable[slot.riseAngle];
        const float sn = g_sinTable[slot.riseAngle];
        const float x1 = g_cosTable[angle] * slot.distance;
        const float y1 = g_sinTable[angle] * slot.distance;
        const vector3d vec1{cs * (x1 * inner), y1 * inner * 2.0f, sn * (x1 * inner)};
        const vector3d vec4{cs * (x1 * outer), y1 * outer * 2.0f, sn * (x1 * outer)};

        int next = angle + step;

        if (next >= 360) {
          next -= 360;
        }

        const float x2 = g_cosTable[next] * slot.distance;
        const float y2 = g_sinTable[next] * slot.distance;
        const vector3d vec2{cs * (x2 * inner), y2 * inner * 2.0f, sn * (x2 * inner)};
        const vector3d vec3{cs * (x2 * outer), y2 * outer * 2.0f, sn * (x2 * outer)};

        unsigned char red;
        unsigned char green;
        unsigned char blue;

        switch (rcolor) {
        case 0:
          red = 255;
          green = 0;
          blue = 0;
          break;
        case 1:
          red = 255;
          green = 126;
          blue = 0;
          break;
        case 2:
          red = 255;
          green = 255;
          blue = 0;
          break;
        case 3:
          red = 0;
          green = 255;
          blue = 0;
          break;
        case 4:
          red = 0;
          green = 0;
          blue = 255;
          break;
        case 5:
          red = 115;
          green = 50;
          blue = 200;
          break;
        case 6:
          red = 255;
          green = 0;
          blue = 255;
          break;
        }

        this->RenderTeiRect(&rpCnt, order, slot.alphaB, slot.alphaB, vec2, vec3, vec4, vec1, wtm, 0, red, green, blue);

        ++order;

        if (order >= 21) {
          order = 20;
        }
      }
    }
  }
}

// OFFSET: 0x00BDF440
// FUNCTION: fn_19909
void CEffectPrim::RenderRectUp2(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 2; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const bool distant = 10.0f <= slot.distance;
    const unsigned char r = distant ? 100 : 200;
    const unsigned char g = distant ? 100 : 250;
    const unsigned char b = distant ? 250 : 200;

    slot.vecBPre = {slot.height[0], -1.0f, slot.height[1]};
    slot.vecBNow = {slot.height[0], -1.0f, -slot.height[1]};
    slot.vecTNow = {-slot.height[0], -1.0f, -slot.height[1]};
    slot.vecTPre = {-slot.height[0], -1.0f, slot.height[1]};

    const float y = -1.0f - slot.height[2];
    const vector3d vec1{slot.vecBPre.x, y, slot.vecBPre.z};
    const vector3d vec2{slot.vecBNow.x, y, slot.vecBNow.z};
    const vector3d vec3{slot.vecTPre.x, y, slot.vecTPre.z};
    const vector3d vec4{slot.vecTNow.x, y, slot.vecTNow.z};

    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }

  for (int ec = 2; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];
    float left = 0.0f;

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const bool distant = 10.0f <= slot.distance;
    const unsigned char r = distant ? 50 : 150;
    const unsigned char g = distant ? 50 : 250;
    const unsigned char b = distant ? 250 : 150;
    const float w = slot.height[0];
    const float h = slot.height[1];
    const float dist = slot.distance;

    slot.vecBPre = {w, -1.0f, h};
    slot.vecBNow = {w, -1.0f, -h};
    slot.vecTNow = {-w, -1.0f, -h};
    slot.vecTPre = {-w, -1.0f, h};

    if (dist < w * 2.0f) {
      slot.vecBPre.z = h - dist;
      slot.vecBNow.z = slot.vecBPre.z - slot.height[3];

      if (slot.vecBNow.z < -h) {
        left = -h - slot.vecBNow.z;
        slot.vecBNow.z = -h;
        slot.vecTNow.x = slot.vecBNow.x - left;
      }
    } else if (dist < w * 4.0f) {
      slot.vecBNow.x = w - (dist - w * 2.0f);
      slot.vecTNow.x = slot.vecBNow.x - slot.height[3];

      if (slot.vecTNow.x < -w) {
        left = -w - slot.vecTNow.x;
        slot.vecTNow.x = -w;
        slot.vecTPre.z = slot.vecTNow.z + left;
      }
    } else if (dist < w * 6.0f) {
      slot.vecTNow.z = dist - w * 4.0f - h;
      slot.vecTPre.z = slot.vecTNow.z + slot.height[3];

      if (slot.vecTPre.z > h) {
        left = slot.vecTPre.z - h;
        slot.vecTPre.z = h;
        slot.vecBPre.x = slot.vecTPre.x + left;
      }
    } else {
      slot.vecTPre.x = dist - w * 6.0f - w;
      slot.vecBPre.x = slot.vecTPre.x + slot.height[3];

      if (slot.vecBPre.x > w) {
        left = slot.vecBPre.x - w;
        slot.vecBPre.x = w;
        slot.vecBNow.z = slot.vecBPre.z - left;
      }
    }

    const float y = -1.0f - slot.height[2];
    const vector3d vec1{slot.vecBPre.x, y, slot.vecBPre.z};
    const vector3d vec2{slot.vecBNow.x, y, slot.vecBNow.z};
    const vector3d vec3{slot.vecTPre.x, y, slot.vecTPre.z};
    const vector3d vec4{slot.vecTNow.x, y, slot.vecTNow.z};

    if (dist < w * 2.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

      if (left > 0.0f) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    } else if (dist < w * 4.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

      if (left > 0.0f) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    } else if (dist < w * 6.0f) {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

      if (left > 0.0f) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    } else {
      this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

      if (left > 0.0f) {
        this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, 0, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      }
    }
  }
}

// OFFSET: 0x00BE0010
// FUNCTION: fn_19910
void CEffectPrim::RenderRectUp(matrix& wtm) {
  int rpCnt = 0;
  unsigned char r;
  unsigned char g;
  unsigned char b;
  short p;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    const float cs1 = g_cosTable[slot.rotStart];
    const float sn1 = g_sinTable[slot.rotStart];
    float rx = slot.height[0];
    float rz = slot.height[1];
    slot.vecBPre = {cs1 * rx - sn1 * rz, -1.0f, sn1 * rx + cs1 * rz};
    rz = -slot.height[1];
    slot.vecBNow = {cs1 * rx - sn1 * rz, -1.0f, sn1 * rx + cs1 * rz};
    rx = -slot.height[0];
    slot.vecTNow = {cs1 * rx - sn1 * rz, -1.0f, sn1 * rx + cs1 * rz};
    rz = slot.height[1];
    slot.vecTPre = {cs1 * rx - sn1 * rz, -1.0f, sn1 * rx + cs1 * rz};

    const float y = -1.0f - slot.height[2];
    const vector3d vec1{slot.vecBPre.x, y, slot.vecBPre.z};
    const vector3d vec2{slot.vecBNow.x, y, slot.vecBNow.z};
    const vector3d vec3{slot.vecTPre.x, y, slot.vecTPre.z};
    const vector3d vec4{slot.vecTNow.x, y, slot.vecTNow.z};

    if (slot.height[3] == 0.0f) {
      r = 255;
      g = 255;
      b = 255;
      p = 0;
    } else if (slot.height[3] == 1.0f) {
      r = 235;
      g = 255;
      b = 235;
      p = 1;
    } else if (slot.height[3] == 2.0f) {
      r = 80;
      g = 80;
      b = 80;
      p = 1;
    } else if (slot.height[3] == 4.0f) {
      r = 255;
      g = 255;
      b = static_cast<unsigned char>(static_cast<int>(slot.height[4] + 0.0f + slot.height[5]));
      p = 1;
    }

    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBPre, slot.vecBNow, vec2, vec1, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecBNow, slot.vecTNow, vec4, vec2, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTNow, slot.vecTPre, vec3, vec4, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    this->RenderTeiRect(&rpCnt, slot.alphaB, slot.vecTPre, slot.vecBPre, vec1, vec3, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);

    if (slot.height[3] == 2.0f) {
      int alpha = slot.alphaB * 2;

      if (alpha > 255) {
        alpha = 255;
      }

      this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), slot.vecBPre, slot.vecTNow, vec4, vec1, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
      this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), slot.vecBNow, slot.vecTPre, vec3, vec2, wtm, p, 0, 0, r, g, b, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BE0680
// FUNCTION: fn_19911
void CEffectPrim::RenderSMA2(matrix& wtm) {
  vector3d vec1{};
  vector3d vec3{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int order = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
      int angle = (slot.rotStart + count) % 360;

      if (slot.fullDisplayAngle == 360 && order == 20) {
        angle = slot.rotStart;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];
      const float rx = g_cosTable[slot.riseAngle] * slot.height[order];
      const float ry = g_sinTable[slot.riseAngle] * slot.height[order];
      const vector3d& delta = this->state.deltaPos;
      const vector3d vec2{cs1 * slot.distance + delta.x, delta.y + 0.0f, sn1 * slot.distance + delta.z};
      const vector3d vec4{cs1 * rx + cs1 * slot.distance + delta.x, delta.y - ry, sn1 * rx + sn1 * slot.distance + delta.z};

      if (order > 0) {
        const float tx1 = static_cast<float>(order - 1) / 21.0f;
        const float tx2 = static_cast<float>(order) / 21.0f;

        if (slot.alphaT == 99) {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 99, 0, 1, 70, 70, 70, 0.0f, 1.0f, tx1, tx2, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 0, 0, 1, 90, 90, 255, 0.0f, 1.0f, tx1, tx2, 0);
        }
      }

      ++order;

      if (order >= 21) {
        order = 20;
      }

      vec1 = vec2;
      vec3 = vec4;
    }
  }
}

// OFFSET: 0x00BE1150
// FUNCTION: fn_19915
void CEffectPrim::RenderSaintWing(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    for (int i = 0; i < 10; ++i) {
      const float rx = slot.distance;
      int angle = slot.rotStart - 65;

      if (angle >= 360) {
        angle -= 360;
      }

      if (angle < 0) {
        angle += 360;
      }

      const vector3d vec1{rx * g_cosTable[angle], slot.height[0], rx * g_sinTable[angle]};
      int angle2 = slot.rotStart + 65;

      if (angle2 >= 360) {
        angle2 -= 360;
      }

      if (angle2 < 0) {
        angle2 += 360;
      }

      const vector3d vec4{rx * g_cosTable[angle2], slot.height[0], rx * g_sinTable[angle2]};
      const float rz = i < 5 ? slot.maxHeight : slot.maxHeight + (0.5f * g_sinTable[slot.riseAngle] + 0.5f);

      angle = slot.rotStart;

      if (angle >= 360) {
        angle -= 360;
      }

      if (angle < 0) {
        angle += 360;
      }

      const float sn1 = g_sinTable[angle];
      const float cs1 = g_cosTable[angle];
      const int add = i < 5 ? (i - i % 5) * 9 + i % 5 * 3 + slot.fullDisplayAngle : (i - i % 5) * 9 + i % 5 * 3;
      const float lx = g_cosTable[add] * rz;
      const float ly = g_sinTable[add] * rz;

      slot.vecBNow.z = sn1 * lx;
      slot.vecBNow.x = cs1 * lx;
      const vector3d vec2{vec1.x + slot.vecBNow.x, vec1.y - ly, vec1.z + slot.vecBNow.z};
      const vector3d vec3{vec4.x + slot.vecBNow.x, vec4.y - ly, vec4.z + slot.vecBNow.z};

      if (ec == 1) {
        if (i % 5 < 4) {
          int alpha = slot.alphaB;

          for (int b = 3; b >= i % 5; --b) {
            alpha = static_cast<int>(static_cast<float>(alpha) * 0.7f);
          }

          this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 105, 105, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec1, vec2, vec3, vec4, wtm, 1, 0, 0, 205, 205, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      } else if (ec == 0) {
        if (i % 5 < 4) {
          int alpha = slot.alphaB;

          for (int b = 3; b >= i % 5; --b) {
            alpha = static_cast<int>(static_cast<float>(alpha) * 0.7f);
          }

          this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec4, vec3, vec2, vec1, wtm, 0, 0, 0, 105, 105, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        } else {
          this->RenderTeiRect(&rpCnt, slot.alphaB, vec4, vec3, vec2, vec1, wtm, 1, 0, 0, 205, 205, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
        }
      }
    }
  }
}

// OFFSET: 0x00BE1820
// FUNCTION: fn_19916
void CEffectPrim::RenderSlash1(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1 || slot.alphaB <= 0) {
      continue;
    }

    for (int i = 0; i <= 2; ++i) {
      int angle = slot.rotStart - 1 + i;

      if (angle < 0) {
        angle += 360;
      } else if (angle >= 360) {
        angle -= 360;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];
      float rx = slot.height[0];
      const vector3d vec1{cs1 * rx, -0.0f, sn1 * rx};
      const vector3d vec2{vec1.x, i == 1 ? vec1.y - slot.maxHeight * 0.2f : vec1.y - slot.maxHeight * 0.1f, vec1.z};
      rx = slot.height[0] + slot.distance;
      const vector3d vec3{cs1 * rx, -0.0f, sn1 * rx};
      const vector3d vec4{vec3.x, i == 1 ? vec3.y - slot.maxHeight : vec3.y - slot.maxHeight * 0.4f, vec3.z};
      const int alpha = i == 1 ? slot.alphaB : slot.alphaB / 3;

      this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec1, vec3, vec4, vec2, wtm, 0, 0, 0, 255, 255, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
    }
  }
}

// OFFSET: 0x00BE1D70
// FUNCTION: fn_19918
void CEffectPrim::RenderSoulLink(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vector3d vec1{g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx};
    vector3d vec2{g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx};
    vector3d vec3{g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx};
    vector3d vec4{g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx};
    const std::int16_t rot0 = this->state.slots[0].alphaT;
    this->Tei_TowardScreen(vec1, vec2, vec3, vec4, rot0, this->state.slots[1].alphaT);

    const float length = slot.vecBPre.x;
    const vector3d vecA{length * g_cosTable[rot0], slot.vecBPre.y, length * g_sinTable[rot0]};
    vec1 = {vec1.x + vecA.x, vec1.y + vecA.y, vec1.z + vecA.z};
    vec2 = {vec2.x + vecA.x, vec2.y + vecA.y, vec2.z + vecA.z};
    vec3 = {vec3.x + vecA.x, vec3.y + vecA.y, vec3.z + vecA.z};
    vec4 = {vec4.x + vecA.x, vec4.y + vecA.y, vec4.z + vecA.z};
    const int alpha = static_cast<int>(slot.height[13]);

    if (vec1.y > 0.0f) {
      continue;
    }

    if (slot.process < 180) {
      CGameMode* gameMode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
      CRagEffect* effect = nullptr;

      if (static_cast<float>(g_session.m_aid) == slot.height[10]) {
        effect = gameMode->scene->player->LaunchEffect(506, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      } else if (auto* actor = static_cast<CGameActor*>(gameMode->scene->GetGameActorByAID(static_cast<std::uint32_t>(slot.height[10])))) {
        effect = actor->LaunchEffect(506, vector3d{0.0f, 0.0f, 0.0f}, 0.0f);
      }

      if (effect) {
        vector3d position = vecA;
        effect->SendMsg(nullptr, 14, 0, reinterpret_cast<int>(&position), 0, 0, 0, 0);
      }
    }

    this->RenderTeiRect(&rpCnt, static_cast<short>(alpha), vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 125, 125, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// INLINED
static __forceinline void RenderSoulParticleQuad(
  CEffectPrim& prim, int* rpCnt, const EffectPrimitiveSlot& slot, const vector3d* corners, const vector3d& base, float countValue, unsigned char extraShade, matrix& wtm
) {
  int count = static_cast<int>(countValue);

  if (count < 0) {
    count = 0;
  }

  const vector3d vec1{corners[0].x + base.x, corners[0].y + base.y, corners[0].z + base.z};
  const vector3d vec2{corners[1].x + base.x, corners[1].y + base.y, corners[1].z + base.z};
  const vector3d vec3{corners[2].x + base.x, corners[2].y + base.y, corners[2].z + base.z};
  const vector3d vec4{corners[3].x + base.x, corners[3].y + base.y, corners[3].z + base.z};

  if (vec1.y > 0.0f) {
    return;
  }

  const short alpha = static_cast<short>(count);

  if (slot.height[15] == 0.0f) {
    prim.RenderTeiRect(rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 80, 80, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.height[15] == 1.0f) {
    prim.RenderTeiRect(rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 200, 200, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.height[15] == 2.0f) {
    prim.RenderTeiRect(rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 160, 160, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.height[15] == 3.0f) {
    prim.RenderTeiRect(rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, 255, 120, 120, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  } else if (slot.height[15] == 4.0f) {
    prim.RenderTeiRect(rpCnt, alpha, vec1, vec2, vec3, vec4, wtm, 0, 0, 0, extraShade, extraShade, 255, 0.0f, 1.0f, 0.0f, 1.0f, 0);
  }
}

// OFFSET: 0x00BE2190
// FUNCTION: fn_19919
void CEffectPrim::RenderSoulParticle(matrix& wtm) {
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const float rx = slot.distance;
    vector3d corners[4] = {
      {g_cosTable[45] * rx, 0.0f, g_sinTable[45] * rx},
      {g_cosTable[135] * rx, 0.0f, g_sinTable[135] * rx},
      {g_cosTable[225] * rx, 0.0f, g_sinTable[225] * rx},
      {g_cosTable[315] * rx, 0.0f, g_sinTable[315] * rx},
    };
    this->Tei_TowardScreen(corners[0], corners[1], corners[2], corners[3], this->state.slots[0].alphaT, this->state.slots[1].alphaT);

    RenderSoulParticleQuad(*this, &rpCnt, slot, corners, slot.vecBPre, slot.height[12], 40, wtm);
    RenderSoulParticleQuad(*this, &rpCnt, slot, corners, slot.vecBNow, slot.height[13], 10, wtm);
    RenderSoulParticleQuad(*this, &rpCnt, slot, corners, slot.vecTPre, slot.height[14], 70, wtm);
  }
}

// OFFSET: 0x00BE2B10
// FUNCTION: fn_19920
void CEffectPrim::RenderSphereWind(matrix& wtm) {
  vector3d vec1{};
  vector3d vec3{};
  int rpCnt = 0;

  for (int ec = 0; ec < 4; ++ec) {
    const EffectPrimitiveSlot& slot = this->state.slots[ec];

    if (slot.life != 1) {
      continue;
    }

    const int step = slot.fullDisplayAngle / 20;
    int order = 0;

    for (int count = 0; count <= slot.fullDisplayAngle; count += step) {
      int angle = slot.rotStart + count;

      if (angle >= 360) {
        angle -= 360;
      } else if (angle < 0) {
        angle += 360;
      }

      if (slot.fullDisplayAngle == 360 && order == 20) {
        angle = slot.rotStart;
      }

      const float cs1 = g_cosTable[angle];
      const float sn1 = g_sinTable[angle];
      const vector3d vecM{cs1 * (g_cosTable[slot.riseAngle] * slot.distance), cs1 * (g_sinTable[slot.riseAngle] * slot.distance), sn1 * slot.distance};
      const vector3d vecA{-(slot.maxHeight * g_sinTable[slot.riseAngle]), slot.maxHeight * g_cosTable[slot.riseAngle], 0.0f};
      const vector3d vec2{vecM.x + vecA.x, vecM.y + vecA.y + slot.height[0], vecM.z + vecA.z};
      const vector3d vec4{vecM.x - vecA.x, vecM.y - vecA.y + slot.height[0], vecM.z - vecA.z};

      if (order > 0) {
        this->RenderTeiRect(&rpCnt, order - 1, slot.alphaB, slot.alphaB, vec1, vec2, vec4, vec3, wtm, 0);
      }

      ++order;

      if (order >= 21) {
        order = 20;
      }

      vec1 = vec2;
      vec3 = vec4;
    }
  }
}
