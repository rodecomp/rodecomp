#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/ActionSpriteDrawing.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Utility/Math/AffineMatrix.h"

// OFFSET: 0x00AD5A70
// FUNCTION: fn_17825
void DrawActionSprite(
  CRenderSystemBase& system, int x, int y, const CActRes* action, const CSprRes* sprite, int animation, int frame, float scale, float angle,
  std::uint32_t color, std::uint16_t flags
) {
  if (!action || !sprite) {
    return;
  }

  SpriteAtlasRecord record{};
  const ActionFrame& current = action->Frame(animation, frame);

  for (int index = 0; index < current.cellCount; ++index) {
    RenderPrimitive* primitive = g_renderer->primitives.Acquire();
    const SpriteFrameCell& cell = current.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    const float scaleX = cell.scaleX;
    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    const float left = static_cast<float>(cell.x) * scale;
    const float scaledLeft = left * scaleX;
    const float right = static_cast<float>(image->width * (image->unknown_0004 + 1) - 1 + cell.x) * scale;
    const float top = static_cast<float>(cell.y) * scale * cell.scaleY;
    const float scaledRight = right * scaleX;
    const float bottom = static_cast<float>(image->height * (image->unknown_0006 + 1) - 1 + cell.y) * scale * cell.scaleY;

    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, sprite->palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(system, image, sprite->palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
    }
    primitive->texture = texture;

    float x0;
    float y0;
    float x1;
    float y1;
    float x2;
    float y2;
    float x3;
    float y3;
    if (angle == 0.0f) {
      float first = scaledLeft;
      float second = right;
      if (cell.mirrored & 1) {
        first = scaledRight;
        second = left;
      }

      const float originX = static_cast<float>(x);
      const float originY = static_cast<float>(y);
      x0 = first + originX;
      y0 = originY + top;
      x1 = scaleX * second + originX;
      y1 = originY + top;
      x2 = first + originX;
      y2 = originY + bottom;
      x3 = scaleX * second + originX;
      y3 = originY + bottom;
    } else {
      const int degrees = static_cast<int>(angle);
      const float cosine = ClientCosine(degrees);
      const float sine = ClientSine(degrees);
      const float originX = static_cast<float>(x);
      const float originY = static_cast<float>(y);
      x0 = cosine * scaledLeft - sine * top + originX;
      y0 = sine * scaledLeft + cosine * top + originY;
      x1 = cosine * scaledRight - sine * top + originX;
      y1 = sine * scaledRight + cosine * top + originY;
      x2 = cosine * scaledRight - sine * bottom + originX;
      y2 = cosine * bottom + sine * scaledRight + originY;
      x3 = cosine * scaledLeft - sine * bottom + originX;
      y3 = cosine * bottom + sine * scaledLeft + originY;
    }

    RenderVertex* vertices = primitive->quad;
    vertices[0].x = x0 + 0.5f;
    vertices[0].y = y0 + 0.5f;
    vertices[1].x = x1 + 0.5f;
    vertices[1].y = y1 + 0.5f;
    vertices[2].x = x2 + 0.5f;
    vertices[2].y = y2 + 0.5f;
    vertices[3].x = x3 + 0.5f;
    vertices[3].y = y3 + 0.5f;

    vertices[0].z = 0.000001f;
    vertices[0].rhw = 0.999999f;
    vertices[0].u = record.u0;
    vertices[0].v = record.v0;
    vertices[0].diffuse = color;
    vertices[1].z = 0.000001f;
    vertices[1].rhw = 0.999999f;
    vertices[1].u = record.u1;
    vertices[1].v = record.v0;
    vertices[1].diffuse = color;
    vertices[2].z = 0.000001f;
    vertices[2].rhw = 0.999999f;
    vertices[2].u = record.u0;
    vertices[2].v = record.v1;
    vertices[2].diffuse = color;
    vertices[3].z = 0.000001f;
    vertices[3].rhw = 0.999999f;
    vertices[3].u = record.u1;
    vertices[3].v = record.v1;
    vertices[3].diffuse = color;

    g_renderer->queues.Submit(primitive, flags, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }
}
