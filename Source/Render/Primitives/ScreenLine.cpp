#include "UI/Window/UIRoMapWnd.h"

#include "Contents/MapSkyMgr.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/ScreenGeometry.h"
#include "Render/TextureQuad.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Chat/UISubChatHisWnd.h"
#include "Utility/Math/AffineMatrix.h"

#include <cmath>
#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_25021
std::uint32_t g_roMapRecordColors[12] = {
  0x66DC143C, 0x66FFA500, 0x66FFFF00, 0x66ADFF2F, 0x6600BFFF, 0x66483D8B, 0x669400D3, 0x66FF1493, 0x66FFF5EE, 0x66F5FFFA, 0x66F0F8FF, 0x66778899,
};

// OFFSET: 0x00918190
// FUNCTION: fn_12591
void UIRoMapWnd::DrawRecordAreas() {
  int color = 0;
  for (std::list<WorldViewRecord>::const_iterator it = this->m_records.begin(); it != this->m_records.end(); ++it) {
    if (g_worldViewMgr.currentWorld != it->worldIndex) {
      continue;
    }

    if (color == 12) {
      color = 0;
    }
    DrawAlphaRectangle(
      g_renderer->primitives, g_renderer->queues, it->left, it->top, it->right - it->left, it->bottom - it->top, g_roMapRecordColors[color],
      g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
    );
    ++color;
  }
}

// OFFSET: 0x00918300
// FUNCTION: fn_12593
void UISubChatHisWnd::Slot43() {
  int x = 0;
  int y = 0;
  this->ToScreen(x, y);
  DrawAlphaRectangle(
    g_renderer->primitives, g_renderer->queues, x + 1, y + 1, this->m_inputWidth - 2, this->m_inputHeight, 0x66000000, g_renderer->fog.supported != 0,
    g_depthStart, g_depthEnd
  );
  DrawAlphaRectangle(
    g_renderer->primitives, g_renderer->queues, this->m_state.unknown_0000 + 1, this->m_state.unknown_0004 + 1, this->m_state.unknown_0008 - 2,
    this->m_state.unknown_000C - 2, this->m_state.unknown_0014, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );
}

// OFFSET: 0x009189C0
// FUNCTION: fn_12595
void UIRoMapWnd::DrawKafraIcon(float x, float y) {
  const float scale = static_cast<float>(g_worldViewMgr.screenScale) * 0.2f + g_depthAverageHalf;
  CSprRes* sprite = dynamic_cast<CSprRes*>(ResourceMgr::Instance().Load("npc\\4_f_kafra6.spr"));
  CActRes* action = dynamic_cast<CActRes*>(ResourceMgr::Instance().Load("npc\\4_f_kafra6.act"));
  if (!sprite || !action) {
    return;
  }

  const std::vector<ActionFrame>& frames = action->Animation(0);
  const int count = static_cast<int>(frames.size());
  if (count == 0) {
    return;
  }

  const int tick = static_cast<int>(timeGetTime() % 4000);
  const int step = tick == 0 ? 0 : tick / (4000 / count);
  const int last = count > step ? step : count - 1;
  const int frame = last < 0 ? 0 : last;
  const std::uint32_t* palette = sprite->palette;

  const SpriteFrameCell* cell = &frames[0].Cell(0);
  if (cell->image == -1) {
    return;
  }

  const SpriteRleImage* image = sprite->images[cell->imageType][cell->image];
  if (!image) {
    return;
  }

  const int top = static_cast<int>(y - (static_cast<float>(image->height) * scale + 23.0f));
  const int lift = top <= 0 ? top : 0;
  if (frame > 0) {
    cell = &frames[frame].Cell(0);
    if (cell->image == -1) {
      return;
    }

    image = sprite->images[cell->imageType][cell->image];
    if (!image) {
      return;
    }
  }

  SpriteAtlasRecord record{};
  CTexture* texture;
  if (cell->imageType == 0) {
    texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(this->m_context->renderSystem, image, palette, record, g_serviceType, g_resourceReuseDelay);
    }
  } else {
    texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
  }

  const int height = static_cast<int>(static_cast<float>(image->height) * scale);
  const int halfWidth = static_cast<int>(static_cast<float>(image->width) * scale * g_depthAverageHalf);
  RenderPrimitive* primitive = g_renderer->primitives.Acquire();
  RenderVertex* vertices = primitive->vertices;
  const float left = x - static_cast<float>(halfWidth);
  const float right = static_cast<float>(halfWidth) + x;
  const float upper = y - static_cast<float>(height) - static_cast<float>(lift);
  const float lower = y - static_cast<float>(lift);
  vertices[0].x = left;
  vertices[0].y = upper;
  vertices[1].y = upper;
  vertices[2].x = left;
  vertices[1].x = right;
  vertices[3].x = right;
  vertices[3].z = 0.8f;
  vertices[2].z = 0.8f;
  vertices[1].z = 0.8f;
  vertices[0].z = 0.8f;
  vertices[2].y = lower;
  vertices[3].y = lower;
  vertices[3].rhw = 1.0f;
  vertices[2].rhw = 1.0f;
  vertices[1].rhw = 1.0f;
  vertices[0].rhw = 1.0f;
  vertices[3].diffuse = 0xFFFFFFFF;
  vertices[2].diffuse = 0xFFFFFFFF;
  vertices[1].diffuse = 0xFFFFFFFF;
  vertices[0].diffuse = 0xFFFFFFFF;
  vertices[0].u = record.u0;
  vertices[0].v = record.v0;
  vertices[1].u = record.u1;
  vertices[1].v = record.v0;
  vertices[2].u = record.u0;
  vertices[2].v = record.v1;
  vertices[3].u = record.u1;
  vertices[3].v = record.v1;
  primitive->texture = texture;
  primitive->unknown_0018 = 2;
  primitive->unknown_001C = 4;
  g_renderer->queues.Submit(primitive, 513, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  this->m_kafraTop = static_cast<int>(vertices[0].y);
  this->m_kafraBottom = static_cast<int>(vertices[3].y);
  this->m_kafraLeft = static_cast<int>(vertices[0].x);
  this->m_kafraRight = static_cast<int>(vertices[3].x);
}

// OFFSET: 0x00917D70
// FUNCTION: fn_12590
void UIRoMapWnd::DrawArrow(
  float x0, float y0, float x1, float y1, float startLeft, float startTop, float startRight, float startBottom, float endLeft, float endTop, float endRight,
  float endBottom, int bodyWidth, CTexture* body, int headWidth, CTexture* head, float progress, std::uint32_t color
) {
  float dy = y1 - y0;
  float dx = x1 - x0;
  const float length = 1.0f / SquareRoot(dy * dy + dx * dx);
  const float dirY = length * dy;
  const float dirX = length * dx;
  float angle = std::acos(dirX);
  if (dirY < 0.0f) {
    angle = 6.283184f - angle;
  }
  angle += 1.570796f;
  const float sine = std::sin(angle);
  const float cosine = std::cos(angle);
  const float normal = 1.0f / SquareRoot(cosine * cosine + sine * sine);
  const float normalX = normal * cosine;
  const float normalY = normal * sine;
  const float offsetX = static_cast<float>(bodyWidth) * normalX;
  const float offsetY = static_cast<float>(bodyWidth) * normalY;
  float sideX0 = offsetX + x0;
  float sideX1 = offsetX + x1;
  float sideY1 = offsetY + y1;
  float sideY0 = offsetY + y0;

  while (x0 > startLeft && startRight > x0 && y0 > startTop && startBottom > y0) {
    x0 += dirX;
    y0 += dirY;
  }
  while (x1 > endLeft && endRight > x1 && y1 > endTop && endBottom > y1) {
    x1 -= dirX;
    y1 -= dirY;
  }
  while (sideX0 > startLeft && startRight > sideX0 && sideY0 > startTop && startBottom > sideY0) {
    sideX0 += dirX;
    sideY0 += dirY;
  }
  while (sideX1 > endLeft && endRight > sideX1 && sideY1 > endTop && endBottom > sideY1) {
    sideX1 -= dirX;
    sideY1 -= dirY;
  }

  const float midX = (x1 - x0) * progress + x0;
  const float midY = (y1 - y0) * progress + y0;
  float cornerX;
  float cornerY;
  if (progress == 1.0f) {
    cornerX = (sideX1 - sideX0) + sideX0;
    cornerY = (sideY1 - sideY0) + sideY0;
  } else {
    cornerX = midX + offsetX;
    cornerY = midY + offsetY;
  }
  DrawScreenQuad(x0, y0, sideX0, sideY0, midX, midY, cornerX, cornerY, body, color);

  if (progress < 1.0f) {
    const float width = static_cast<float>(bodyWidth);
    const float size = static_cast<float>(headWidth);
    const float tipX = (offsetX * g_depthAverageHalf + midX) - width * dirX;
    const float tipY = (offsetY * g_depthAverageHalf + midY) - width * dirY;
    const float halfX = size * normalX * g_depthAverageHalf;
    const float halfY = size * normalY * g_depthAverageHalf;
    DrawScreenQuad(tipX, tipY, tipX - halfX, tipY - halfY, tipX + halfX, tipY + halfY, size * dirX + tipX, size * dirY + tipY, head, color);
  }
}
