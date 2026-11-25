#include <new>
#include "Client/Input/Mouse.h"
#include "Client/Mode/CMode.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Map/MapSkyMgr.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Primitives/RenderPrimitive.h"
#include "Render/Primitives/RenderPrimitivePool.h"
#include "Render/Primitives/RenderQueues.h"
#include "Render/Renderer.h"
#include "Render/Primitives/TextureQuad.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24570
CMouse g_mouse;

namespace {
struct PrimitivePoolAccess : RenderPrimitivePool {
  // INLINED
  inline RenderPrimitive* AcquireLocal() {
    if (this->m_commands.end() == this->m_current) {
      this->m_commands.emplace_back();
      this->m_current = this->m_commands.end();
      return &this->m_commands.back();
    }

    RenderPrimitive* command = &*this->m_current;
    ++this->m_current;
    new (command) RenderPrimitive;

    return command;
  }
};
}

// OFFSET: 0x00AD5520
// FUNCTION: fn_17824
void CMode::DrawCursor() {
  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load("cursors.act"));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("cursors.spr"));
  const std::uint32_t ticks = (timeGetTime() - this->cursorTick) / 24;
  action->Speed(this->cursorAction);

  unsigned divisor;
  switch (this->cursorAction) {
  case CURSOR_ACTION_BUTTON:
    divisor = 4;
    break;
  case CURSOR_ACTION_ATTACK:
    divisor = 20;
    break;
  case CURSOR_ACTION_WARP:
    divisor = 10;
    break;
  case CURSOR_ACTION_PICKUP:
    divisor = 1;
    break;
  case CURSOR_ACTION_SKILL:
    divisor = 6;
    break;
  case CURSOR_ACTION_SKILL_TARGET:
    divisor = 3;
    break;
  case CURSOR_ACTION_MOVE_BLOCKED:
    divisor = 2;
    break;
  default:
    divisor = 4;
    break;
  }

  switch (this->cursorAction) {
  case CURSOR_ACTION_BUTTON:
  case CURSOR_ACTION_PICKUP:
    if (g_mouse.buttons[0] != 0) {
      const unsigned frame = ticks / divisor;
      this->cursorActionFrame = frame % action->FrameCount(this->cursorAction);
      if (frame / action->FrameCount(this->cursorAction) >= 1) {
        this->cursorActionFrame = action->FrameCount(this->cursorAction) - 1;
      }
    } else {
      this->cursorTick = timeGetTime();
      this->cursorActionFrame = 0;
    }
    break;
  case CURSOR_ACTION_INTERACT: {
    const unsigned frame = ticks / divisor;
    this->cursorActionFrame = frame % action->FrameCount(this->cursorAction);
    if (frame / action->FrameCount(this->cursorAction) >= 1) {
      this->cursorActionFrame = action->FrameCount(this->cursorAction) - 1;
    }
    break;
  }
  default:
    this->cursorActionFrame = ticks / divisor % action->FrameCount(this->cursorAction);
    break;
  }

  g_windowMgr.SendNotice(UIM_SET_CURSOR_ACTION_FRAME, this->cursorActionFrame, 0, 0, 0, 0);

  const int y = static_cast<int>(static_cast<float>(g_mouse.y) + this->cursorOffsetY);
  const int x = static_cast<int>(static_cast<float>(g_mouse.x) + this->cursorOffsetX);
  if (!action || !sprite) {
    return;
  }

  SpriteAtlasRecord record{};
  const ActionFrame& frame = action->Frame(this->cursorAction, this->cursorActionFrame);
  for (int index = 0; index < frame.cellCount; ++index) {
    RenderPrimitive* primitive = static_cast<PrimitivePoolAccess&>(g_renderer->primitives).AcquireLocal();
    const SpriteFrameCell& cell = frame.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    const float scaleX = cell.scaleX;
    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    const float left = static_cast<float>(cell.x);
    const float right = static_cast<float>(image->width * (image->doubleWidth + 1) - 1 + cell.x);
    const float top = static_cast<float>(cell.y) * cell.scaleY;
    const float bottom = static_cast<float>(image->height * (image->doubleHeight + 1) - 1 + cell.y) * cell.scaleY;

    CTexture* texture;
    if (cell.imageType == 0) {
      auto& atlasRef = g_renderer->atlas;
      texture = atlasRef.Find(image, sprite->palette, record);
      if (!texture) {
        texture = atlasRef.Insert(image, sprite->palette, record);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->surface));
    }
    primitive->texture = texture;

    float scaledLeft;
    float scaledRight;
    if (cell.mirrored & 1) {
      scaledLeft = scaleX * right;
      scaledRight = scaleX * left;
    } else {
      scaledLeft = scaleX * left;
      scaledRight = scaleX * right;
    }

    const float x0 = static_cast<float>(x) + scaledLeft + g_depthAverageHalf;
    const float y0 = static_cast<float>(y) + top + g_depthAverageHalf;
    const float x1 = scaledRight + static_cast<float>(x) + g_depthAverageHalf;
    const float y1 = static_cast<float>(y) + bottom + g_depthAverageHalf;

    RenderVertex* quad = primitive->quad;
    quad[0].x = x0;
    quad[0].y = y0;
    quad[1].x = x1;
    quad[1].y = y0;
    quad[2].x = x0;
    quad[2].y = y1;
    quad[3].x = x1;
    quad[3].y = y1;
    quad[0].z = 0.000001f;
    quad[0].rhw = 0.999999f;
    quad[0].u = record.u0;
    quad[0].v = record.v0;
    quad[0].diffuse = 0xFAFFFFFF;
    quad[1].z = 0.000001f;
    quad[1].rhw = 0.999999f;
    quad[1].u = record.u1;
    quad[1].v = record.v0;
    quad[1].diffuse = 0xFAFFFFFF;
    quad[2].z = 0.000001f;
    quad[2].rhw = 0.999999f;
    quad[2].u = record.u0;
    quad[2].v = record.v1;
    quad[2].diffuse = 0xFAFFFFFF;
    quad[3].z = 0.000001f;
    quad[3].rhw = 0.999999f;
    quad[3].u = record.u1;
    quad[3].v = record.v1;
    quad[3].diffuse = 0xFAFFFFFF;

    g_renderer->Submit(primitive, RSF_ALPHA | RSF_CUSTOM_BLEND | RSF_NO_DEPTH);
  }
}
