#include "UI/Quest/UIRenewQuestWnd.h"

#include "Client/Mode/ModeCursor.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/MessageTable.h"
#include "Contents/RenewQuestMgr.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Text/TextContext.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/UIStaticBitmap.h"
#include "Utility/Text/FormatString.h"

#include <cstddef>
#include <cstring>

// OFFSET: 0x007F9240
// FUNCTION: fn_09933
void UINpcSprWnd::SetSprite(int width, int height, std::string name) {
  std::string path = "npc\\" + name + ".act";
  if (ResourceMgr::Instance().Exists(path.c_str())) {
    this->m_action = static_cast<CActRes*>(ResourceMgr::Instance().Load(path.c_str()));
  }

  path = "npc\\" + name + ".spr";
  if (ResourceMgr::Instance().Exists(path.c_str())) {
    this->m_sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(path.c_str()));
  }

  if (this->m_action && this->m_sprite) {
    const ActionFrame& frame = this->m_action->Frame(0, 0);
    for (int index = 0; index < frame.cellCount; ++index) {
      const SpriteFrameCell& cell = frame.Cell(index);
      if (cell.image != -1) {
        const SpriteRleImage* image = this->m_sprite->images[cell.imageType][cell.image];
        width = image->width;
        height = image->height;
        break;
      }
    }
  }

  this->SetSize(width, height);
}

// OFFSET: 0x007F95D0
// FUNCTION: fn_09934
void UINpcSprWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->parent) {
    this->parent->Invalidate();
  }

  if (this->m_action && this->m_sprite) {
    const ActionFrame& frame = this->m_action->Frame(0, 0);
    const std::uint32_t* palette = this->m_sprite->palette;
    SpriteAtlasRecord record{};
    for (int index = 0; index < frame.cellCount; ++index) {
      const SpriteFrameCell& cell = frame.Cell(index);
      if (cell.image == -1) {
        continue;
      }

      record.v0 = 0.0f;
      record.u0 = 0.0f;
      const SpriteRleImage* image = this->m_sprite->images[cell.imageType][cell.image];
      CTexture* texture;
      if (cell.imageType == 0) {
        texture = g_renderer->atlas.Find(image, palette, record);
        if (!texture) {
          texture = g_renderer->atlas.Insert(this->m_context->renderSystem, image, palette, record, g_serviceType, g_resourceReuseDelay);
        }
      } else {
        texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
      }

      if (image->width != this->geometry.width || image->height != this->geometry.height) {
        this->Resize(image->width, image->height);
      }

      this->geometry.surface->DrawSurface(
        0, 0, texture, static_cast<int>(record.u0 * 256.0f), static_cast<int>(record.v0 * 256.0f), image->width, image->height, cell.mirrored != 0, 1, 1, 0, 0
      );
    }
  }
}

// OFFSET: 0x007F97F0
// FUNCTION: fn_09936
void UINpcSprWnd::ResetCursor(int, int) {
  this->m_context->cursor.Set(2, this->m_context->cursorTick);
}

// OFFSET: 0x007F9800
// FUNCTION: fn_09937
int UINpcSprWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  return 0;
}

// OFFSET: 0x007F9820
// FUNCTION: fn_09938
void UINpcSprWnd::SetDestination(std::string mapName, int x, int y) {
  this->m_mapName = mapName;
  this->m_destinationX = x;
  this->m_destinationY = y;
}

// OFFSET: 0x007F9A70
// FUNCTION: fn_09940
UIOngoingQuestScrollWnd::UIOngoingQuestScrollWnd(UIWindowContext& windows, int width, int height)
    : UIScrollWnd(windows, width, height, 0), m_questId(0), m_lineHeight(18) {
  this->m_huntTexts.clear();
  this->Slot15(width, height);
}

// OFFSET: 0x007F9C30
// FUNCTION: fn_09942
void Nullsub254() {}

// OFFSET: 0x007F9C40
// FUNCTION: fn_09943
void UIOngoingQuestScrollWnd::Slot15(int, int) {
  this->scrollBarWidth = 9;
}

// OFFSET: 0x007F9C50
// FUNCTION: fn_09944
void UIOngoingQuestScrollWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x007F9C70
// FUNCTION: fn_09945
void UIOngoingQuestScrollWnd::Slot35(int delta) {
  if (this->wheelScroll == 1) {
    if (delta < 0) {
      this->SendMsg(this->scrollBar, 7, 15, 0, 0, 0);
      return;
    }
    this->SendMsg(this->scrollBar, 7, -15, 0, 0, 0);
  }
}

// OFFSET: 0x007F9CB0
// FUNCTION: fn_09946
void UIOngoingQuestScrollWnd::RefreshHuntTexts() {
  const RenewQuestOngoingInfo& quest = g_renewQuestMgr->GetOngoingQuest(this->m_questId);
  TextContext context(*this->geometry.surface);
  context.SetFont(0, 12, 1, 0);

  for (int index = 0; index < quest.m_huntCount; ++index) {
    std::string text;
    if (quest.m_huntUnknown00D4[index] == 31999) {
      const std::string condition =
        g_renewQuestMgr->GetMonsterConditionText(quest.m_huntUnknown01A0[index], quest.m_huntUnknown01AC[index], quest.m_huntUnknown01B2[index]);
      text = condition;
    } else {
      const short minLevel = quest.m_huntUnknown01AC[index];
      const short maxLevel = quest.m_huntUnknown01B2[index];
      const char* name = quest.m_huntName[index];
      if (minLevel < 0 || maxLevel < 0 || (minLevel == 0 && maxLevel == 0)) {
        const std::string formatted = FormatOwnedString("%s", name);
        text = formatted;
      } else if (minLevel == 0) {
        const std::string formatted = FormatOwnedString("%s Lv.%d▼", name, maxLevel);
        text = formatted;
      } else if (maxLevel == 0) {
        const std::string formatted = FormatOwnedString("%s Lv.%d▲", name, minLevel);
        text = formatted;
      } else {
        const std::string formatted = FormatOwnedString("%s Lv.%d~Lv.%d", name, minLevel, maxLevel);
        text = formatted;
      }
    }

    const short goal = quest.m_huntUnknown0180[index];
    if (goal > 0) {
      const short count = quest.m_huntUnknown00E0[index];
      if (count == goal) {
        const std::string formatted = FormatOwnedString("%s ( %s )", text.c_str(), LookupMessage(2030));
        text = formatted;
      } else {
        const std::string formatted = FormatOwnedString("%s ( %d / %d )", text.c_str(), count, goal);
        text = formatted;
      }
    }

    const SIZE size = context.Measure(text.c_str(), static_cast<int>(text.size()));
    this->m_huntTexts[index]->Resize(size.cx + 4, 14);
    this->m_huntTexts[index]->SetText(text.c_str(), 0);
  }

  this->Invalidate();
}

// OFFSET: 0x007FAF70
// FUNCTION: fn_09949
UIRecommededQuestScrollWnd::UIRecommededQuestScrollWnd(UIWindowContext& windows, int width, int height)
    : UIScrollWnd(windows, width, height, 0), unknown_00B0(0), unknown_00B4(14) {
  this->Slot15(width, height);
}

// OFFSET: 0x007FB0C0
// FUNCTION: fn_09951
void Nullsub255() {}

// OFFSET: 0x007FB0D0
// FUNCTION: fn_09952
void UIRecommededQuestScrollWnd::Slot15(int, int) {
  this->scrollBarWidth = 9;
}

// OFFSET: 0x007FB0E0
// FUNCTION: fn_09953
void UIRecommededQuestScrollWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x007FB100
// FUNCTION: fn_09954
void UIRecommededQuestScrollWnd::Slot35(int delta) {
  if (this->wheelScroll == 1) {
    if (delta < 0) {
      this->SendMsg(this->scrollBar, 7, 15, 0, 0, 0);
      return;
    }
    this->SendMsg(this->scrollBar, 7, -15, 0, 0, 0);
  }
}

// OFFSET: 0x007FB6C0
// FUNCTION: fn_09959
void UIRenewDetailQuestInfoWnd::Slot15(int, int) {
  this->m_ongoingWnd = new UIOngoingQuestScrollWnd(*this->m_context, 342, 322);
  this->m_ongoingWnd->Move(0, 62);
  this->AddChild(this->m_ongoingWnd);

  this->m_recommendedWnd = new UIRecommededQuestScrollWnd(*this->m_context, 342, 322);
  this->m_recommendedWnd->Move(0, 62);
  this->AddChild(this->m_recommendedWnd);

  this->m_closeButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->m_closeButton->SetBitmap("유저인터페이스\\basic_interface\\btn_close.bmp", 0, 0);
  this->m_closeButton->SetBitmap("유저인터페이스\\basic_interface\\btn_close_a.bmp", 1, 0);
  this->m_closeButton->SetBitmap("유저인터페이스\\basic_interface\\btn_close_b.bmp", 2, 0);
  this->m_closeButton->SetSize(this->m_closeButton->BitmapWidth(), this->m_closeButton->BitmapHeight());
  this->m_closeButton->Move(288, 388);
  this->m_closeButton->SetId(201);
  this->AddChild(this->m_closeButton);
}

// OFFSET: 0x007FB860
// FUNCTION: fn_09960
void UIRenewDetailQuestInfoWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  if (this->m_recommended) {
    this->DrawPanelBackground(0, 0, "유저인터페이스\\renew_questui\\bg_questSub2.bmp");
    this->DrawCenteredText(
      this->geometry.width >> 1, 39, g_renewQuestMgr->GetRecommendedQuest(this->m_questId).m_title.c_str(),
      static_cast<int>(g_renewQuestMgr->GetRecommendedQuest(this->m_questId).m_title.size()), 0, 12, 0x55321D, 1
    );
    if (!this->m_imageName.empty()) {
      CBitmapRes* image = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_imageName.c_str())));
      this->DrawBitmap(3, 64, image ? &image->image : nullptr, false);
    }

    CTexture* texture = g_texMgr.Load("유저인터페이스\\renew_questui\\bg_quest01.bmp", 0, false, true, false);
    if (texture) {
      this->geometry.surface->DrawScaledSurface(9, 70, 324, 138, texture, texture->Width(), texture->Height(), 1.0f, 1.0f, 0x80FFFFFF, false, 0, 0, 0, 0, 0);
    }

    texture = g_texMgr.Load("유저인터페이스\\renew_questui\\bg_quest02.bmp", 0, false, true, false);
    if (texture) {
      this->geometry.surface->DrawScaledSurface(9, 283, 208, 92, texture, texture->Width(), texture->Height(), 1.0f, 1.0f, 0x80FFFFFF, false, 0, 0, 0, 0, 0);
    }
  } else {
    this->DrawPanelBackground(0, 0, "유저인터페이스\\renew_questui\\bg_questSub.bmp");
    this->DrawCenteredText(
      this->geometry.width >> 1, 39, g_renewQuestMgr->GetOngoingQuest(this->m_questId).m_title.c_str(),
      static_cast<int>(g_renewQuestMgr->GetOngoingQuest(this->m_questId).m_title.size()), 0, 12, 0x55321D, 1
    );
    this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 12, 393, this->m_timeText.c_str(), 0, 0, 12, 0x161BD2, 0, 0);
  }

  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 16, 2, LookupMessage(0xD48), 0, 0, 13, 0, 0, 0);
}

// OFFSET: 0x007FBAD0
// FUNCTION: fn_09961
void UIRenewDetailQuestInfoWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x007FBFD0

// OFFSET: 0x007FC870
// FUNCTION: fn_09973
void UIRenewQuestScrollBar::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->maximum == 0) {
    return;
  }

  CBitmapRes* upper = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\renew_questui\\bt_scrollbar_arrow_upper.bmp")));
  this->DrawBitmap(1, 0, upper ? &upper->image : nullptr, true);
  CBitmapRes* lower = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\renew_questui\\bt_scrollbar_arrow_down.bmp")));
  this->DrawBitmap(1, this->geometry.height - this->arrowLength - 1, lower ? &lower->image : nullptr, true);

  const int top = this->ThumbOffset() + this->arrowLength;
  CBitmapRes* thumbTop = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\renew_questui\\bt_scrollBar_up.bmp")));
  this->DrawBitmap(0, top, thumbTop ? &thumbTop->image : nullptr, true);
  const int bottom = this->ThumbLength() - 4 + top;
  for (int y = top + 4; y < bottom; y += 4) {
    CBitmapRes* thumbMiddle = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\renew_questui\\bt_scrollBar_up.bmp")));
    this->DrawBitmap(0, y, thumbMiddle ? &thumbMiddle->image : nullptr, true);
  }
  CBitmapRes* thumbBottom = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\renew_questui\\bt_scrollBar_down.bmp")));
  this->DrawBitmap(0, bottom, thumbBottom ? &thumbBottom->image : nullptr, true);
}


// OFFSET: 0x007FC9B0

// OFFSET: 0x007FCD80
// FUNCTION: fn_09976
void UIRenewQuestWnd::Slot31(int x, int y) {
  this->m_pressed = 0;
  this->Invalidate();
  UIFrameWnd::Slot31(x, y);
}

// OFFSET: 0x007FCDB0
// FUNCTION: fn_09977
void UIRenewQuestWnd::Slot35(int delta) {
  if (this->m_scrollable) {
    if (delta < 0) {
      this->SendMsg(this->m_scrollBar, 7, 3, 0, 0, 0);
      return;
    }
    this->SendMsg(this->m_scrollBar, 7, -3, 0, 0, 0);
  }
}
