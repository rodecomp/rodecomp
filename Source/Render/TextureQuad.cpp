#include "Client/Mode/CMode.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Utility/Math/AffineMatrix.h"

#include <windows.h>

// GLOBAL: gl_24520
float g_depthStart;
// GLOBAL: gl_24521
float g_depthEnd;

// OFFSET: 0x0055D310
// FUNCTION: fn_02101
float RhwToDepth(float rhw) {
  return ((1.0f / rhw - 10.0f) * 1.0067114f) * rhw;
}

// OFFSET: 0x00AD5FA0
// FUNCTION: fn_17826
void CMode::DrawTexture(const RECT& rect, int angle, float rhw, std::uint32_t color, CTexture* texture, TextureRegion region) {
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  RenderVertex* vertices = primitive->vertices;
  primitive->texture = texture;
  const float bottomZ = RhwToDepth(rhw);
  const float topZ = RhwToDepth(rhw);

  if (angle != 0) {
    const float cosine = ClientCosine(angle);
    const float sine = ClientSine(angle);
    const float width = static_cast<float>(rect.right - rect.left);
    const float height = static_cast<float>(rect.bottom - rect.top);
    const float right = width * 0.5f;
    const float bottom = height * 0.5f;
    const float centerX = static_cast<float>(rect.left) + right;
    const float centerY = static_cast<float>(rect.top) + bottom;
    const float left = -width * 0.5f;
    const float top = -height * 0.5f;

    vertices[0].x = left * cosine - top * sine + centerX;
    vertices[0].y = top * cosine + left * sine + centerY;
    vertices[1].x = right * cosine - top * sine + centerX;
    vertices[1].y = right * sine + top * cosine + centerY;
    vertices[2].x = left * cosine - bottom * sine + centerX;
    vertices[3].x = right * cosine - bottom * sine + centerX;
    vertices[2].y = bottom * cosine + left * sine + centerY;
    vertices[3].y = bottom * cosine + right * sine + centerY;
  } else {
    vertices[0].x = static_cast<float>(rect.left);
    vertices[0].y = static_cast<float>(rect.top);
    vertices[1].x = static_cast<float>(rect.right);
    vertices[1].y = static_cast<float>(rect.top);
    vertices[2].x = static_cast<float>(rect.left);
    vertices[2].y = static_cast<float>(rect.bottom);
    vertices[3].x = static_cast<float>(rect.right);
    vertices[3].y = static_cast<float>(rect.bottom);
  }

  const float scaleU = static_cast<float>(texture->ImageWidth()) / static_cast<float>(texture->TextureWidth());
  const float scaleV = static_cast<float>(texture->ImageHeight()) / static_cast<float>(texture->TextureHeight());
  const float margin = 0.0f;

  vertices[0].z = topZ;
  vertices[0].rhw = rhw;
  vertices[0].diffuse = color;
  vertices[0].u = region.left * scaleU + margin;
  vertices[0].v = region.top * scaleV + margin;
  vertices[1].z = topZ;
  vertices[1].rhw = rhw;
  vertices[1].diffuse = color;
  vertices[1].u = region.right * scaleU - margin;
  vertices[1].v = region.top * scaleV + margin;
  vertices[2].z = bottomZ;
  vertices[2].rhw = rhw;
  vertices[2].diffuse = color;
  vertices[2].u = region.left * scaleU + margin;
  vertices[2].v = region.bottom * scaleV - margin;
  vertices[3].z = bottomZ;
  vertices[3].rhw = rhw;
  vertices[3].diffuse = color;
  vertices[3].u = region.right * scaleU - margin;
  vertices[3].v = region.bottom * scaleV - margin;

  g_renderer->queues.Submit(primitive, 0x201, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
}
