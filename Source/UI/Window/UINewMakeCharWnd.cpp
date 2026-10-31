#include "UI/Window/UINewMakeCharWnd.h"

#include "Client/Session.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Contents/SpriteRidingOffset.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ImfResource.h"
#include "Resource/PaletteResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/TextureNames.h"

#include <cstdio>
#include <cstring>
#include <new>

// OFFSET: 0x007DB320
// FUNCTION: fn_09617
void UIHairBitmap::Slot25(int x, int y) {
  if (this->parent) {
    this->parent->Slot25(x, y);
  }
}

// OFFSET: 0x007DB300
// FUNCTION: fn_09616
void UIHairBitmap::Slot26(int x, int y) {
  if (this->parent) {
    this->parent->Slot26(x, y);
  }
}

// OFFSET: 0x007DB380
// FUNCTION: fn_09620
void UIHairBitmap::Slot27(int x, int y) {
  if (this->parent) {
    this->parent->Slot27(x, y);
  }
}

// OFFSET: 0x007DB360
// FUNCTION: fn_09619
void UIHairBitmap::Slot28(int x, int y) {
  if (this->parent) {
    this->parent->Slot28(x, y);
  }
}

// OFFSET: 0x007DB3A0
// FUNCTION: fn_09621
void UIHairBitmap::Slot29(int x, int y) {
  if (this->parent) {
    this->parent->Slot29(x, y);
  }
}

// OFFSET: 0x007DB3C0
// FUNCTION: fn_09622
void UIHairBitmap::ResetCursor(int x, int y) {
  if (this->parent) {
    this->parent->ResetCursor(x, y);
  }
}

// OFFSET: 0x007DB340
// FUNCTION: fn_09618
void UIHairBitmap::Slot31(int x, int y) {
  if (this->parent) {
    this->parent->Slot31(x, y);
  }
}

// OFFSET: 0x007DB030
// FUNCTION: fn_09615
void UINewMakeCharWnd::Paint() {
  CBitmapRes* background =
    static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\make_character_ver2\\bg_makeBg.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 0x2C, 0x11, LookupMessage(0xD1B), 0, 0, 0xC, 0x5F5F5F, 1, 0);

  for (int i = 0; i < 2; ++i) {
    CImfRes* offsets = static_cast<CImfRes*>(ResourceMgr::Instance().Load(this->m_raceOffsets.c_str()));
    CActRes* action = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->m_raceActions[i].c_str()));
    CSprRes* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(this->m_raceSprites[i].c_str()));
    const ActionFrame& frame = action->Frame(this->m_action, this->m_frame);
    POINT offset = offsets->GetLongPoint(i, this->m_action, this->m_frame);
    const std::uint32_t* palette = sprite->palette;

    if (i == 1) {
      CActRes* first = static_cast<CActRes*>(ResourceMgr::Instance().Load(this->m_raceActions[0].c_str()));
      const ActionFrame& firstFrame = first->Frame(this->m_action, this->m_frame);
      if (frame.anchorCount != 0 && firstFrame.anchorCount != 0 && frame.anchors[0].unknown_0008 == firstFrame.anchors[0].unknown_0008) {
        offset.x += firstFrame.anchors[0].x - frame.anchors[0].x;
        offset.y += firstFrame.anchors[0].y - frame.anchors[0].y;
      }

      if (this->m_racePaletteEnabled[0] > 0) {
        palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->m_racePalettes[0].c_str()))->palette;
      }
    } else if (this->m_racePaletteEnabled[1] > 0) {
      palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(this->m_racePalettes[1].c_str()))->palette;
    }

    const SpriteFrameCell& cell = frame.Cell(i);
    if (cell.image == -1) {
      continue;
    }

    const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
    SpriteAtlasRecord record{};
    CTexture* texture;
    if (cell.imageType == 0) {
      texture = g_renderer->atlas.Find(image, palette, record);
      if (!texture) {
        texture = g_renderer->atlas.Insert(this->m_context->renderSystem, image, palette, record, g_serviceType, g_resourceReuseDelay);
      }
    } else {
      texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
    }

    this->geometry.surface->DrawSurface(
      this->m_viewX + offset.x + cell.x, this->m_viewY + offset.y + cell.y, texture, static_cast<int>(record.u0 * 256.0f),
      static_cast<int>(record.v0 * 256.0f), image->width, image->height, cell.mirrored != 0, image->unknown_0004 + 1, image->unknown_0006 + 1, 0, 0
    );
  }
}

// OFFSET: 0x007DB3E0
// FUNCTION: fn_09623
void UINewMakeCharWnd::Slot19() {
  this->m_raceViews[this->m_race]->Invalidate();
}

// OFFSET: 0x007DB400
// FUNCTION: fn_09624
int UIHairStyleButton::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 13: {
    bool previous = this->m_selected;
    this->m_selected = argument1 != 0;
    if (previous != (argument1 != 0)) {
      if (argument1) {
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 0, 0);
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 1, 0);
      } else {
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_normal.bmp", 0, 0);
        this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_over.bmp", 1, 0);
      }
      this->SetBitmap("유저인터페이스\\make_character_ver2\\bt_hairStyle_select.bmp", 2, 0);
      this->Invalidate();
    }
    break;
  }
  case 40: {
    char name[128];
    memset(name, 0, sizeof(name));
    if (this->m_race == 0) {
      if (this->m_style > 22) {
        if (this->m_preview) {
          this->m_preview->Enable(0);
          this->InvalidateChildren();
        }
        return 0;
      }
      this->m_preview->Enable(1);
      sprintf_s(
        name, this->m_sex == 0 ? "유저인터페이스\\make_character_ver2\\img_hairStyle_girl%02d.bmp" : "유저인터페이스\\make_character_ver2\\img_hairStyle%02d.bmp",
        this->m_style + 1
      );
    } else if (this->m_race == 1) {
      if (this->m_style > 5) {
        if (this->m_preview) {
          this->m_preview->Enable(0);
          this->InvalidateChildren();
        }
        return 0;
      }
      this->m_preview->Enable(1);
      sprintf_s(
        name,
        this->m_sex == 0 ? "유저인터페이스\\make_character_ver2\\img_hairStyle_doramGirl%02d.bmp" : "유저인터페이스\\make_character_ver2\\img_hairStyle_doramBoy%02d.bmp",
        this->m_style + 1
      );
    }
    if (this->m_preview) {
      this->m_preview->SetBitmap(name, 0, 0);
      this->m_preview->SetBitmap(name, 1, 0);
      this->m_preview->SetBitmap(name, 2, 0);
      this->m_preview->SetBitmap("유저인터페이스\\make_character_ver2\\img_hairStyle_none.bmp", 3, 0);
    }
    this->InvalidateChildren();
    break;
  }
  default:
    UIBitmapButton::SendMsg(sender, message, argument1, argument2, argument3, argument4);
    break;
  }
  return 0;
}

// OFFSET: 0x007DA290
// FUNCTION: fn_09613
void UIHairStyleButton::Slot15(int width, int height) {
  this->m_preview = new (std::nothrow) UIHairBitmap(*this->m_context, *this->m_buttonContext);
  this->m_preview->SetSize(36, 37);
  this->AddChild(this->m_preview);
  this->SendMsg(nullptr, 40, 0, 0, 0, 0);
}

// OFFSET: 0x007DBFD0
// FUNCTION: fn_09626
void UINewMakeCharWnd::sub_7DBFD0() {
  const std::string bodyAct = GetBodyActionResourceName(this->m_job, this->m_sex, this->m_job, 0);
  this->m_raceActions[0].assign(bodyAct);

  const std::string bodySpr = g_session.GetBodySprName(this->m_job, this->m_sex, this->m_job, 0);
  this->m_raceSprites[0].assign(bodySpr);

  int head = this->m_head;
  const std::string headAct = g_session.GetHeadActName(this->m_job, this->m_sex, &head);
  this->m_raceActions[1].assign(headAct);

  const std::string headSpr = g_session.GetHeadSprName(this->m_job, this->m_sex, &head);
  this->m_raceSprites[1].assign(headSpr);

  const int bodyPalette = this->m_bodyPalette;
  this->m_head = static_cast<short>(head);
  this->m_racePaletteEnabled[1] = bodyPalette;
  if (bodyPalette > 0) {
    const std::string bodyPal = g_session.GetBodyPalName(this->m_job, this->m_sex, bodyPalette, this->m_job, 0, 0);
    this->m_racePalettes[1].assign(bodyPal);
  }

  const int headPalette = this->m_headPalette;
  this->m_racePaletteEnabled[0] = headPalette;
  if (headPalette > 0) {
    const std::string headPal = g_session.GetHeadPalName(head, this->m_job, this->m_sex, headPalette);
    this->m_racePalettes[0].assign(headPal);
  }

  this->m_raceOffsets = GetBodyImfName(this->m_job, this->m_sex, this->m_head, this->m_costumeJob);
}
