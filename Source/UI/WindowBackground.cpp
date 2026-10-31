#include "Client/Session.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Skills/ClientSkills.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowBorder.h"

#include <cstring>
#include <string>
#include <windows.h>

// OFFSET: 0x008D03E0
// FUNCTION: fn_12085
void UIWindow::DrawPanelBackground(int y, int width, const char* bitmapName) {
  if (std::strlen(bitmapName) != 0) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(bitmapName)));
    if (bitmap) {
      this->DrawBitmap(0, y, &bitmap->image, false);
    }
    return;
  }

  if (width == 0) {
    width = this->geometry.width;
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  DrawRoundedBorder(*this, 0, y, width, this->geometry.height - y, RGB(red, green, blue));
  this->FillRegion(0, y, width, 5, RGB(red, green, blue));

  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(1, y, width - 2, this->geometry.height - y - 2, RGB(red, green, blue));
  this->FillRegion(2, this->geometry.height - 2, width - 4, 1, RGB(red, green, blue));
}

// OFFSET: 0x008D0530
// FUNCTION: fn_12086
void UIWindow::DrawPanelBackgroundAt(int x, int y, int width, const char* bitmapName) {
  if (bitmapName && std::strlen(bitmapName) != 0) {
    auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(bitmapName)));
    if (bitmap) {
      this->DrawBitmap(x, y, &bitmap->image, false);
    }
    return;
  }

  if (width == 0) {
    width = this->geometry.width - x;
  }

  const int height = this->geometry.height;
  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 14, 6, red, green, blue);
  DrawRoundedBorder(*this, x, y, width, height - y, RGB(red, green, blue));
  this->FillRegion(x, y, width, 5, RGB(red, green, blue));

  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(x + 1, y, width - 2, height - y - 2, RGB(red, green, blue));
  this->FillRegion(x + 2, height - 2, width - 4, 1, RGB(red, green, blue));
}

// OFFSET: 0x008D0680
// FUNCTION: fn_12087
void UIWindow::DrawRobeSprite(int job, int sex, int robe, int x, int y, bool overlay, int animation, int frame) {
  if (robe <= 0) {
    return;
  }

  std::string sprName = g_session.GetRobeSprName(job, job, sex, robe, false, job);
  if (!ResourceMgr::Instance().Exists(sprName.c_str())) {
    return;
  }

  std::string actName = g_session.GetRobeActName(job, job, sex, robe, false, job);
  if (!ResourceMgr::Instance().Exists(actName.c_str())) {
    return;
  }

  CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(actName.c_str()));
  CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(sprName.c_str()));
  const SpriteFrameCell& cell = action->Frame(animation, frame).Cell(0);
  if (cell.image == -1) {
    return;
  }

  SpriteAtlasRecord record{};
  record.u0 = 0.0f;
  record.v0 = 0.0f;
  const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
  CTexture* texture;
  if (cell.imageType == 0) {
    texture = g_renderer->atlas.Find(image, sprite->palette, record);
    if (!texture) {
      texture = g_renderer->atlas.Insert(this->m_context->renderSystem, image, sprite->palette, record, g_serviceType, g_resourceReuseDelay);
    }
  } else {
    texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
  }

  if (overlay) {
    const std::uint32_t color = this->Color();
    this->AddSpriteCellOverlay(
      x, y, reinterpret_cast<int>(action), reinterpret_cast<int>(sprite), image, &cell, GetSkillFactor(job), 0.0f, color, sprite->palette, 1.0f
    );
    return;
  }

  this->geometry.surface->DrawSurface(
    x + cell.x, y + cell.y, texture, static_cast<int>(record.u0 * 256.0f), static_cast<int>(record.v0 * 256.0f), image->width, image->height,
    cell.mirrored != 0, static_cast<short>(image->unknown_0004) + 1, static_cast<short>(image->unknown_0006) + 1, 0, 0
  );
}
