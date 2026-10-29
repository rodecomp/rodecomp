#include "Client/Input/Mouse.h"
#include "Client/Mode/CMode.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24570
CMouse g_mouse;

// OFFSET: 0x00AD5520
// FUNCTION: fn_17824
void CMode::DrawCursor() {
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("cursors.act"));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("cursors.spr"));
  const std::uint32_t ticks = (timeGetTime() - this->cursorTick) / 24;
  action->Speed(this->cursorAction);

  unsigned divisor = 4;
  switch (this->cursorAction) {
  case 5:
    divisor = 20;
    break;
  case 7:
    divisor = 10;
    break;
  case 10:
    divisor = 6;
    break;
  case 11:
    divisor = 3;
    break;
  case 13:
    divisor = 2;
    break;
  case 1: {
    const unsigned frame = ticks >> 2;
    this->cursorActionFrame = frame % action->FrameCount(1);
    if (frame / action->FrameCount(this->cursorAction) >= 1) {
      this->cursorActionFrame = action->FrameCount(this->cursorAction) - 1;
    }
    goto notify;
  }
  case 9:
    divisor = 1;
    [[fallthrough]];
  case 2:
    if (g_mouse.buttons[0] == 0) {
      this->cursorTick = timeGetTime();
      this->cursorActionFrame = 0;
    } else {
      const unsigned frame = ticks / divisor;
      this->cursorActionFrame = frame % action->FrameCount(this->cursorAction);
      if (frame / action->FrameCount(this->cursorAction) >= 1) {
        this->cursorActionFrame = action->FrameCount(this->cursorAction) - 1;
      }
    }
    goto notify;
  default:
    break;
  }

  this->cursorActionFrame = ticks / divisor % action->FrameCount(this->cursorAction);

notify:
  g_windowMgr.SendNotice(16, this->cursorActionFrame, 0, 0, 0, 0);

  const int y = static_cast<int>(static_cast<float>(g_mouse.y) + this->cursorOffsetY);
  const int x = static_cast<int>(static_cast<float>(g_mouse.x) + this->cursorOffsetX);
  if (!action || !sprite) {
    return;
  }

  SpriteAtlasRecord record{};
  const ActionFrame& frame = action->Frame(this->cursorAction, this->cursorActionFrame);
  for (int index = 0; index < frame.cellCount; ++index) {
    RenderPrimitive* primitive = g_renderer->primitives.Acquire();
    const SpriteFrameCell& cell = frame.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    const float scaleX = cell.scaleX;
    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    float left = static_cast<float>(cell.x);
    float right = static_cast<float>(image->width * (image->unknown_0004 + 1) - 1 + cell.x);
    const float top = static_cast<float>(cell.y) * cell.scaleY;
    const float bottom = static_cast<float>(image->height * (image->unknown_0006 + 1) - 1 + cell.y) * cell.scaleY;

    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, sprite->palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, sprite->palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
    }
    primitive->texture = texture;

    if (cell.mirrored & 1) {
      const float swap = left;
      left = right;
      right = swap;
    }

    const float x0 = static_cast<float>(x) + scaleX * left + 0.5f;
    const float y0 = static_cast<float>(y) + top + 0.5f;
    const float x1 = scaleX * right + static_cast<float>(x) + 0.5f;
    const float y1 = static_cast<float>(y) + bottom + 0.5f;

    RenderVertex* quad = primitive->quad;
    quad[0].x = x0;
    quad[0].y = y0;
    quad[1].x = x1;
    quad[1].y = y0;
    quad[2].x = x0;
    quad[2].y = y1;
    quad[3].x = x1;
    quad[3].y = y1;
    for (RenderVertex& vertex : primitive->quad) {
      vertex.z = 0.000001f;
      vertex.rhw = 0.999999f;
      vertex.diffuse = 0xFAFFFFFF;
    }
    quad[0].u = record.u0;
    quad[0].v = record.v0;
    quad[1].u = record.u1;
    quad[1].v = record.v0;
    quad[2].u = record.u0;
    quad[2].v = record.v1;
    quad[3].u = record.u1;
    quad[3].v = record.v1;

    g_renderer->queues.Submit(primitive, 517, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd);
  }
}
