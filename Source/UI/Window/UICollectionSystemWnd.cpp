#include "UI/Window/UICollectionSystemWnd.h"

#include "Client/Mode/ModeMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Window/WindowManagerWindows.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00910890
// FUNCTION: fn_12504
CUICollectionSystemWnd::CUICollectionSystemWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_frameDelay(15), unknown_00B8(-1), unknown_00BC(0),
      m_frame(0), m_lastFrame(-1), m_paused(false), m_lastTick(0) {
  this->m_action = static_cast<CActRes*>(ResourceMgr::Instance().Load("collection_slotmachine.act"));
  this->m_sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("collection_slotmachine.spr"));
}

// OFFSET: 0x0091EC30
// FUNCTION: fn_12635
void CUICollectionSystemWnd::Slot15(int, int) {}

// OFFSET: 0x0092B830
// FUNCTION: fn_12658
void CUICollectionSystemWnd::Paint() {
  const ActionFrame& current = this->m_action->Frame(this->unknown_00BC, this->m_frame);
  const std::uint32_t* palette = this->m_sprite->palette;
  SpriteAtlasRecord record{};

  for (int index = 0; index < current.cellCount; ++index) {
    const SpriteFrameCell& cell = current.Cell(index);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = this->m_sprite->images[0][cell.image];
    CTexture* texture = g_renderer->atlas.Find(image, palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(this->m_context->renderSystem, image, palette, record, g_serviceType, g_resourceReuseDelay);
    }
    this->geometry.surface->DrawSurface(
      this->geometry.width / 2 + cell.x, this->geometry.height / 2 + cell.y, texture, static_cast<int>(record.u0 * 256.0f),
      static_cast<int>(record.v0 * 256.0f), image->width, image->height, cell.mirrored != 0, image->unknown_0004 + 1, image->unknown_0006 + 1, 0, 0
    );
  }

  const std::uint32_t now = timeGetTime();
  if (!this->m_paused && static_cast<int>(now - this->m_lastTick) > this->m_frameDelay) {
    ++this->m_frame;
    if (this->m_frame >= static_cast<int>(this->m_action->FrameCount(this->unknown_00BC))) {
      this->m_frame = 0;
    }
    this->m_lastTick = now;
  }
}

// OFFSET: 0x00930A40
// FUNCTION: fn_12691
void CUICollectionSystemWnd::Slot25(int, int) {
  if (this->unknown_00BC == 0) {
    this->unknown_00BC = 1;
    g_modeMgr.GetGameMode()->SendMsg(282, this->unknown_00B8, 0, 0, 0);
  }
}

// OFFSET: 0x00932E20
// FUNCTION: fn_12717
void CUICollectionSystemWnd::Slot19() {
  this->Invalidate();
  const std::uint32_t now = timeGetTime();
  if (this->m_frame == this->m_lastFrame) {
    if (!this->m_paused) {
      this->m_paused = true;
      this->m_lastTick = now;
    }
    if (static_cast<int>(now - this->m_lastTick) > 2000) {
      this->m_frameContext->windows.CloseWindow(196);
    }
  }
}

// OFFSET: 0x00934740
// FUNCTION: fn_12741
int CUICollectionSystemWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t b, std::intptr_t, std::intptr_t) {
  if (message == 80) {
    if (a == 0) {
      this->unknown_00B8 = b;
      return 0;
    }
    this->m_lastFrame = b;
    this->m_frame = 0;
    this->m_frameDelay = 200;
  }
  return 0;
}
