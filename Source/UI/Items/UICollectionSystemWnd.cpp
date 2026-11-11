#include "Render/DeviceContext/DCSurface.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Items/UICollectionSystemWnd.h"

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
CUICollectionSystemWnd::CUICollectionSystemWnd()
    : UIFrameWnd(0), m_frameDelay(15), collectionId(-1), animationIndex(0),
      m_frame(0), m_lastFrame(-1), m_paused(false), m_lastTick(0) {
  this->m_action = static_cast<CActRes*>(ResourceMgr::Instance().Load("collection_slotmachine.act"));
  this->m_sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load("collection_slotmachine.spr"));
}

// OFFSET: 0x0091EC30
// FUNCTION: fn_12635
void CUICollectionSystemWnd::OnCreate(int, int) {}

// OFFSET: 0x0092B830
// FUNCTION: fn_12658
void CUICollectionSystemWnd::Paint() {
  const ActionFrame& current = this->m_action->Frame(this->animationIndex, this->m_frame);
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
      texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
    }
    this->geometry.surface->DrawSurface(
      this->geometry.width / 2 + cell.x, this->geometry.height / 2 + cell.y, texture, static_cast<int>(record.u0 * 256.0f),
      static_cast<int>(record.v0 * 256.0f), image->width, image->height, cell.mirrored != 0, image->doubleWidth + 1, image->doubleHeight + 1, 0, 0
    );
  }

  const std::uint32_t now = timeGetTime();
  if (!this->m_paused && static_cast<int>(now - this->m_lastTick) > this->m_frameDelay) {
    ++this->m_frame;
    if (this->m_frame >= static_cast<int>(this->m_action->FrameCount(this->animationIndex))) {
      this->m_frame = 0;
    }
    this->m_lastTick = now;
  }
}

// OFFSET: 0x00930A40
// FUNCTION: fn_12691
void CUICollectionSystemWnd::OnLBtnDown(int, int) {
  if (this->animationIndex == 0) {
    this->animationIndex = 1;
    g_modeMgr.GetGameMode()->SendMsg(MM_TRYCOLLECTION, this->collectionId, 0, 0, 0);
  }
}

// OFFSET: 0x00932E20
// FUNCTION: fn_12717
void CUICollectionSystemWnd::OnProcess() {
  this->Invalidate();
  const std::uint32_t now = timeGetTime();
  if (this->m_frame == this->m_lastFrame) {
    if (!this->m_paused) {
      this->m_paused = true;
      this->m_lastTick = now;
    }
    if (static_cast<int>(now - this->m_lastTick) > 2000) {
      g_windowMgr.CloseWindow(WID_COLLECTIONSYSTEMWND);
    }
  }
}

// OFFSET: 0x00934740
// FUNCTION: fn_12741
int CUICollectionSystemWnd::SendMsg(UIWindow*, int message, std::intptr_t a, std::intptr_t b, std::intptr_t, std::intptr_t) {
  if (message == UM_SET_TARGET) {
    if (a == 0) {
      this->collectionId = b;
      return 0;
    }
    this->m_lastFrame = b;
    this->m_frame = 0;
    this->m_frameDelay = 200;
  }
  return 0;
}
