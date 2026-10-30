#include "UI/Window/UIRoMapWnd.h"

#include "Contents/MapSkyMgr.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
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
