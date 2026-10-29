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

// OFFSET: 0x00BCD960
// FUNCTION: fn_19865
void DrawEffectPrimitive15(EffectPrimitiveState& particle, const RenderObjectState& render, EffectPrimitiveRenderContext& context) {
  const float width = particle.unknown_0354;
  const float left = -width;
  float top = -particle.unknown_0360;
  float bottom = particle.unknown_0360;

  if ((particle.flags & 0x80u) != 0) {
    top = top + top;
    bottom = 0.0f;
  }

  if ((particle.flags & 8u) != 0) {
    bottom = 0.0f;
  }

  const float* matrix = particle.matrix2;
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
  const float vTop = particle.unknown_02E4;
  const float vBottom = particle.unknown_02E8;
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
      vertex.diffuse = particle.unknown_01C0;
      vertex.u = u[triangleIndex][vertexIndex];
      vertex.v = v[triangleIndex][vertexIndex];
    }

    triangle.texture = particle.textures[render.curMotion];
  }

  for (unsigned index = 0; index < 2; ++index) {
    RenderTriangle& triangle = particle.triangles[index];

    if (triangle.Visible()) {
      context.queues.SubmitTriangle(
        triangle, static_cast<std::uint16_t>(particle.unknown_01C4), context.commands, context.depthAlpha, context.depthStart, context.depthEnd
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
        triangle, static_cast<std::uint16_t>(this->state.unknown_01C4), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
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

  switch (static_cast<int>(this->state.scale)) {
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
        triangle, static_cast<std::uint16_t>(this->state.unknown_01C4), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
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
        triangle, static_cast<std::uint16_t>(this->state.unknown_01C4), g_renderer->commands, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
      );
      ++*rpCnt;
    }
  }
}
