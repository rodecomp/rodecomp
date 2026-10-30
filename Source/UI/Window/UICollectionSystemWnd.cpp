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

#include <windows.h>
#include <timeapi.h>

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
