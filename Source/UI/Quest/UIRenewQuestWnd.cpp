#include "UI/Quest/UIRenewQuestWnd.h"

#include "Client/Application/ApplicationWindow.h"
#include "Client/Mode/ModeCursor.h"
#include "Client/ReplayPacketRecorder.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "Contents/RenewQuestMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
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
#include "UI/Buttons/UICheckBox.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITagTextViewer.h"
#include "UI/UIStaticBitmap.h"
#include "UI/Window/UITab.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Text/FormatString.h"

#include <cstddef>
#include <cstring>

#pragma pack(push, 1)
struct PACKET_CZ_ACTIVE_QUEST {
  short packetType;
  unsigned int questId;
  unsigned char active;
};
#pragma pack(pop)

// GLOBAL: gl_25172
bool g_renewQuestTitlesPending = true;

// OFFSET: 0x007F9070
// FUNCTION: fn_09931
UINpcSprWnd::UINpcSprWnd(UIWindowContext& context)
    : UIWindow(context, 0), m_mapName(), m_action(nullptr), m_sprite(nullptr), m_destinationX(0), m_destinationY(0) {
  this->m_mapName.clear();
}

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

// OFFSET: 0x007FA100
// FUNCTION: fn_09947
int UIOngoingQuestScrollWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2,
                                     std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 0xC9) {
      g_windowMgr.CloseWindow(0x142);
    }
  } else if (message == 0x17) {
    this->RefreshHuntTexts();
  }

  return UIScrollWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007FA150

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

// OFFSET: 0x007FB140
// FUNCTION: fn_09955
int UIRecommededQuestScrollWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2,
                                        std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6 && argument1 == 0xC9) {
    g_windowMgr.CloseWindow(0x142);
  }

  return UIScrollWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007FB190
// FUNCTION: fn_09956
void UIRecommededQuestScrollWnd::SetQuest(unsigned int questId) {
  const RenewQuestRecommendedInfo& quest = g_renewQuestMgr->GetRecommendedQuest(questId);
  this->Clear();
  this->horizontalBar = nullptr;
  this->scrollBar = nullptr;
  this->unknown_00B0 = questId;

  UITagTextViewer* viewers[3];
  viewers[0] = new UITagTextViewer(*this->m_context);
  viewers[0]->SetSize(306, 121);
  viewers[0]->Move(18, 12);
  viewers[0]->m_backgroundColor = 0xFFFF00FF;
  viewers[0]->Invalidate();
  this->AddScrollChild(viewers[0]);

  viewers[1] = new UITagTextViewer(*this->m_context);
  viewers[1]->SetSize(213, 60);
  viewers[1]->Move(16, 157);
  viewers[1]->m_backgroundColor = 0xFFFF00FF;
  viewers[1]->Invalidate();
  viewers[1]->m_font = TagTextViewer::TextFont{0x2A2A2A, 12, 1, 0};
  viewers[1]->m_outlined = 1;
  viewers[1]->m_outlineColor = 0xFFFFFF;
  viewers[1]->m_lineSpacing = 4;
  this->AddScrollChild(viewers[1]);

  viewers[2] = new UITagTextViewer(*this->m_context);
  viewers[2]->SetSize(199, 83);
  viewers[2]->Move(18, 225);
  viewers[2]->m_backgroundColor = 0xFFFF00FF;
  viewers[2]->Invalidate();
  this->AddScrollChild(viewers[2]);

  for (int index = 0; index < 3; ++index) {
    for (const std::string& description : quest.m_descriptions[index]) {
      viewers[index]->SendMsg(nullptr, 0x1D, reinterpret_cast<std::intptr_t>(description.c_str()), 0, 0, 0);
    }
  }

  if (!quest.unknown_007C.empty()) {
    UINpcSprWnd* npc = new UINpcSprWnd(*this->m_context);
    npc->SetSprite(75, 145, quest.unknown_007C);
    npc->Move(this->geometry.width - npc->geometry.width - 40, this->geometry.height - npc->geometry.height - 10);
    if (!quest.unknown_0094.empty()) {
      npc->SetDestination(quest.unknown_0094, quest.unknown_00AC, quest.unknown_00B0);
    }
    this->AddScrollChild(npc);
  }

  this->Invalidate();
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

// OFFSET: 0x007FBAF0
// FUNCTION: fn_09962
int UIRenewDetailQuestInfoWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2,
                                       std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 0xC9) {
      g_windowMgr.CloseWindow(0x142);
    }

    return 0;
  }

  if (message != 0x17) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  if (this->m_recommended == 0) {
    this->m_ongoingWnd->SendMsg(this, 0x17, 0, 0, 0, 0);
  } else {
    this->m_recommendedWnd->SendMsg(this, 0x17, 0, 0, 0, 0);
  }

  return 0;
}

// OFFSET: 0x007FBB70
// FUNCTION: fn_09963
void UIRenewDetailQuestInfoWnd::SetQuest(unsigned int questId, bool recommended) {
  if (questId == 0) {
    return;
  }

  this->m_recommended = recommended;
  this->m_questId = questId;
  this->m_timeText = "";
  if (recommended) {
    this->m_ongoingWnd->Show(0);
    this->m_recommendedWnd->Show(1);
    this->m_recommendedWnd->SetQuest(this->m_questId);
    this->m_imageName = g_renewQuestMgr->GetRecommendedQuest(questId).unknown_00B4;
    if (!this->m_imageName.empty()) {
      this->m_imageName = "유저인터페이스\\renew_questui\\" + this->m_imageName;
    }
  } else {
    const RenewQuestOngoingInfo& quest = g_renewQuestMgr->GetOngoingQuest(questId);
    if (quest.m_title.empty()) {
      MessageBoxA(hWnd, FormatOwnedString("Not found Quest Info = %lu", questId).c_str(), "Error", 0);
      return;
    }

    this->m_recommendedWnd->Show(0);
    this->m_ongoingWnd->Show(1);
    this->m_ongoingWnd->SetQuest(this->m_questId);
    if (quest.m_year > 1970) {
      const std::string timeText =
        FormatOwnedString("%s [%02d/%02d/%02d %02d:%02d]", LookupMessage(2195), quest.m_year, quest.m_month, quest.m_day, quest.m_hour, quest.m_minute);
      this->m_timeText = timeText;
    }
  }

  this->Invalidate();
}

// OFFSET: 0x007FBFD0
// FUNCTION: fn_09967
UIRenewQuestWnd::UIRenewQuestWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_tabs(nullptr), m_scrollBar(nullptr), m_optionCheckBox(nullptr), m_closeButton(nullptr), m_tab(0), m_scroll(0),
      m_selected(0), m_scrollable(false), m_pressed(0), m_controls(&controls) {
  std::memset(&this->m_rows, 0, sizeof(this->m_rows));
  this->m_questIds[0].clear();
  this->m_questIds[1].clear();
  this->m_questIds[2].clear();
}

// OFFSET: 0x007FC4A0
// FUNCTION: fn_09972
void UIRenewQuestWnd::Slot15(int, int) {
  this->m_tabs = new UITab(*this->m_context);
  this->m_tabs->SetSize(29, 404);
  this->m_tabs->tabCount = 4;
  this->m_tabs->Move(0, 17);
  this->m_tabs->style = -1;
  this->AddChild(this->m_tabs);
  this->m_tabs->SendMsg(this, 13, this->m_tab, 0, 0, 0);

  if (!g_windowMgr.window_0428) {
    g_windowMgr.EnsureWindow(204);
  }
  const bool checked = g_windowMgr.window_0428->geometry.visible != 0;
  this->m_optionCheckBox = this->AddStyledCheckBox(
    *this->m_controls, 213, 10, 12, LookupMessage(2019), 38, 447, checked, nullptr, 12, 0x212121, "유저인터페이스\\renew_questui\\checkbox_on.bmp",
    "유저인터페이스\\renew_questui\\checkbox_off.bmp"
  );

  this->m_closeButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->m_closeButton->SetBitmap("유저인터페이스\\basic_interface\\btn_close.bmp", 0, 0);
  this->m_closeButton->SetBitmap("유저인터페이스\\basic_interface\\btn_close_a.bmp", 1, 0);
  this->m_closeButton->SetBitmap("유저인터페이스\\basic_interface\\btn_close_b.bmp", 2, 0);
  this->m_closeButton->SetSize(this->m_closeButton->BitmapWidth(), this->m_closeButton->BitmapHeight());
  this->m_closeButton->Move(327, 442);
  this->m_closeButton->SetId(201);
  this->AddChild(this->m_closeButton);

  this->m_scrollBar = new UIRenewQuestScrollBar(*this->m_context);
  this->m_scrollBar->Configure(374, 17, 1, 422);
  this->m_scroll = 0;
  this->AddChild(this->m_scrollBar);

  for (int index = 0; index < 9; ++index) {
    const int y = 46 + index * 46;
    UICheckBox* check = this->AddStyledCheckBox(
      *this->m_controls, 389 + index, 14, 14, nullptr, 350, y - 19, true, nullptr, 0, 0, "유저인터페이스\\renew_questui\\bt_check_on.bmp",
      "유저인터페이스\\renew_questui\\bt_check_off.bmp"
    );
    this->m_rows.checkBoxes[index] = check;
    check->m_message = 0;
    this->m_rows.checkBoxes[index]->SetTooltip(LookupMessage(3409));
    this->m_rows.checkBoxes[index]->SetHidden(true);

    UICheckBox* lock = this->AddStyledCheckBox(
      *this->m_controls, 389 + index, 20, 17, nullptr, 344, y, true, nullptr, 0, 0, "유저인터페이스\\renew_questui\\bt_lock_open.bmp",
      "유저인터페이스\\renew_questui\\bt_lock.bmp"
    );
    this->m_rows.lockBoxes[index] = lock;
    lock->m_message = 1;
    this->m_rows.lockBoxes[index]->SetHidden(true);

    this->m_rows.icons[index] = new UIStaticBitmap(*this->m_context, *this->m_controls);
    this->m_rows.icons[index]->SetBitmap("유저인터페이스\\renew_questui\\ico_nq.bmp");
    this->m_rows.icons[index]->SetSize(this->m_rows.icons[index]->geometry.width, this->m_rows.icons[index]->geometry.height);
    this->m_rows.icons[index]->Move(46, y - 15);
    this->m_rows.icons[index]->Show(0);
    this->AddChild(this->m_rows.icons[index]);
  }

  this->SendMsg(this, 0x17, 0, 0, 0, 0);
}

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
// FUNCTION: fn_09974
void UIRenewQuestWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawPanelBackground(0, 0, FormatOwnedString("유저인터페이스\\renew_questui\\bg_quest%d.bmp", this->m_tab + 1).c_str());
  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 45, 2, LookupMessage(3400), 0, 0, 13, 0, 0, 0);

  for (int index = 0; index < 9; ++index) {
    const int y = index * 46;
    const unsigned int row = this->m_scroll + index;
    const std::vector<unsigned int>& ids = this->m_questIds[this->m_tab];
    if (row >= ids.size()) {
      break;
    }

    const RenewQuestInfo* quest;
    if (this->m_tab == 1) {
      quest = g_renewQuestMgr->FindRecommendedQuest(ids[row]);
    } else {
      quest = g_renewQuestMgr->FindOngoingQuest(ids[row]);
    }
    if (!quest) {
      break;
    }

    const char* summary;
    int summaryY;
    if (row == static_cast<unsigned int>(this->m_selected)) {
      const char* background = this->m_pressed ? "유저인터페이스\\renew_questui\\bg_questList_press.bmp" : "유저인터페이스\\renew_questui\\bg_questList_check.bmp";
      CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background)));
      this->DrawBitmap(38, this->m_pressed + 24 + y, bitmap ? &bitmap->image : nullptr, true);
      this->m_rows.icons[index]->Move(46, this->m_pressed + 31 + y);
      this->DrawTextAt(
        this->m_controls->fonts, this->m_controls->locale, 80, this->m_pressed + 30 + y, quest->m_title.empty() ? "" : quest->m_title.c_str(), 0, 0, 12,
        0xE47B2E, 1, 0
      );
      summary = quest->unknown_004C.empty() ? "" : quest->unknown_004C.c_str();
      summaryY = this->m_pressed + y;
    } else {
      CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\renew_questui\\bg_questList.bmp")));
      this->DrawBitmap(38, y + 24, bitmap ? &bitmap->image : nullptr, true);
      this->m_rows.icons[index]->Move(46, y + 31);
      this->DrawTextAt(
        this->m_controls->fonts, this->m_controls->locale, 80, y + 30, quest->m_title.empty() ? "" : quest->m_title.c_str(), 0, 0, 12, 0x955322, 1, 0
      );
      summary = quest->unknown_004C.empty() ? "" : quest->unknown_004C.c_str();
      summaryY = y;
    }

    this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 80, summaryY + 48, summary, 0, 0, 12, 0x38302E, 0, 0);
  }
}

// OFFSET: 0x007FCC90
// FUNCTION: fn_09975
void UIRenewQuestWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  if (x >= 38 && x <= 368 && y >= 24 && y <= 438) {
    const unsigned int row = (y - 24) / 46 + this->m_scroll;
    if (row < this->m_questIds[this->m_tab].size()) {
      this->m_pressed = 1;
      this->m_selected = row;
      this->Invalidate();
      UIRenewDetailQuestInfoWnd* detail = static_cast<UIRenewDetailQuestInfoWnd*>(g_windowMgr.EnsureWindow(0x142));
      if (detail) {
        const std::vector<unsigned int>& ids = this->m_questIds[this->m_tab];
        const bool recommended = this->m_tab == 1;
        if (ids.empty()) {
          detail->SetQuest(0, recommended);
        } else {
          detail->SetQuest(ids[this->m_selected], recommended);
        }
      }
    }
  }
}

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

// OFFSET: 0x007FCDF0
// FUNCTION: fn_09978
void UIRenewQuestWnd::RefreshRows() {
  for (int i = 0; i < 9; ++i) {
    unsigned int index = this->m_scroll + i;
    if (this->m_tab == 1) {
      const std::vector<unsigned int>& ids = this->m_questIds[1];
      if (index < ids.size()) {
        const RenewQuestRecommendedInfo& quest = g_renewQuestMgr->GetRecommendedQuest(ids[index]);
        this->m_rows.lockBoxes[i]->geometry.unknown_0030 = quest.m_state;
        this->m_rows.lockBoxes[i]->Invalidate();
        this->m_rows.lockBoxes[i]->SetTooltip(LookupMessage(3408));
        this->m_rows.lockBoxes[i]->SetHidden(false);
        this->m_rows.checkBoxes[i]->SetHidden(true);
        std::string bitmap = quest.m_icon.empty() ? std::string("유저인터페이스\\renew_questui\\ico_nq.bmp") : "유저인터페이스\\renew_questui\\" + quest.m_icon;
        this->m_rows.icons[i]->SetBitmap(bitmap.c_str());
        this->m_rows.icons[i]->SetTooltip(quest.m_iconTooltip.c_str());
        this->m_rows.icons[i]->Show(1);
        this->m_rows.icons[i]->Invalidate();
        continue;
      }
    } else {
      const std::vector<unsigned int>& ids = this->m_questIds[this->m_tab];
      if (index < ids.size()) {
        const RenewQuestOngoingInfo& quest = g_renewQuestMgr->GetOngoingQuest(ids[index]);
        if (this->m_tab == 3) {
          this->m_rows.checkBoxes[i]->SetHidden(true);
          this->m_rows.lockBoxes[i]->SetHidden(true);
        } else {
          this->m_rows.lockBoxes[i]->geometry.unknown_0030 = quest.m_state;
          this->m_rows.lockBoxes[i]->Invalidate();
          this->m_rows.lockBoxes[i]->SetTooltip(LookupMessage(3407));
          this->m_rows.lockBoxes[i]->SetHidden(false);
          this->m_rows.checkBoxes[i]->geometry.unknown_0030 = quest.m_simpleView;
          this->m_rows.checkBoxes[i]->Invalidate();
          this->m_rows.checkBoxes[i]->SetHidden(false);
        }
        std::string bitmap = quest.m_icon.empty() ? std::string("유저인터페이스\\renew_questui\\ico_nq.bmp") : "유저인터페이스\\renew_questui\\" + quest.m_icon;
        this->m_rows.icons[i]->SetBitmap(bitmap.c_str());
        this->m_rows.icons[i]->SetTooltip(quest.m_iconTooltip.c_str());
        this->m_rows.icons[i]->Show(1);
        this->m_rows.icons[i]->Invalidate();
        continue;
      }
    }

    this->m_rows.checkBoxes[i]->SetHidden(true);
    this->m_rows.lockBoxes[i]->SetHidden(true);
    this->m_rows.icons[i]->Show(0);
  }
}

// OFFSET: 0x007FD420
// FUNCTION: fn_09979
void UIRenewQuestWnd::UpdateScroll() {
  const std::vector<unsigned int>& ids = this->m_questIds[this->m_tab];
  if (ids.begin() != ids.end() && this->m_scrollBar) {
    int maximum = static_cast<int>(ids.size()) - 9;
    if (maximum < 0) {
      maximum = 0;
    }

    this->m_scrollable = maximum > 0;
    if (maximum > 0) {
      this->m_scrollBar->SetRange(maximum, 9, static_cast<int>(ids.size()));
      if (this->m_scroll < 0) {
        this->m_scroll = 0;
        this->m_scrollBar->SetPosition(0);
      } else {
        if (this->m_scroll > maximum) {
          this->m_scroll = maximum;
        }
        this->m_scrollBar->SetPosition(this->m_scroll);
      }
    } else {
      this->m_scroll = 0;
      this->m_scrollBar->SetRange(0, 9, static_cast<int>(ids.size()));
    }

    this->m_scrollBar->Invalidate();
  }

  this->RefreshRows();
}

// OFFSET: 0x007FD500
// FUNCTION: fn_09980
void UIRenewQuestWnd::RefreshQuestLists(int type, unsigned int questId) {
  if (type == 0 || type == 1) {
    this->m_questIds[0].clear();
    this->m_questIds[2].clear();
    this->m_questIds[3].clear();
    CRenewQuestMgr* mgr = g_renewQuestMgr;
    for (auto it = mgr->m_ongoingQuests.begin(); it != mgr->m_ongoingQuests.end(); ++it) {
      if (it->second.unknown_01F8 == 1) {
        this->m_questIds[3].push_back(it->first);
      } else if (it->second.m_state == 1) {
        this->m_questIds[0].push_back(it->first);
      } else {
        this->m_questIds[2].push_back(it->first);
      }
    }
  }

  if (type == 0 || type == 2) {
    this->m_questIds[1].clear();
    CRenewQuestMgr* mgr = g_renewQuestMgr;
    for (auto it = mgr->m_recommendedQuests.begin(); it != mgr->m_recommendedQuests.end(); ++it) {
      if (it->second.m_state == 1) {
        this->m_questIds[1].push_back(it->first);
      }
    }
    for (auto it = mgr->m_recommendedQuests.begin(); it != mgr->m_recommendedQuests.end(); ++it) {
      if (it->second.m_state == 0) {
        this->m_questIds[1].push_back(it->first);
      }
    }
  }

  if (questId == 0) {
    this->m_selected = 0;
    return;
  }

  const std::vector<unsigned int>& ids = this->m_questIds[this->m_tab];
  unsigned int index = 0;
  const unsigned int count = static_cast<unsigned int>(ids.size());
  if (count != 0) {
    const unsigned int* id = ids.data();
    do {
      if (questId == *id) {
        break;
      }
      ++index;
      ++id;
    } while (index < count);
  }

  this->m_selected = index;
  if (this->m_selected >= 9) {
    this->m_scroll = this->m_selected - 8;
    this->UpdateScroll();
    this->Invalidate();
  }

  UIRenewDetailQuestInfoWnd* detail = static_cast<UIRenewDetailQuestInfoWnd*>(g_windowMgr.EnsureWindow(0x142));
  if (detail) {
    const std::vector<unsigned int>& current = this->m_questIds[this->m_tab];
    const bool recommended = this->m_tab == 1;
    if (current.empty()) {
      detail->SetQuest(0, recommended);
    } else {
      detail->SetQuest(current[this->m_selected], recommended);
    }
  }
}

// INLINED
static void SetRecommendedQuestTitle(CRenewQuestMgr* mgr, unsigned int id, std::string title) {
  mgr->m_recommendedQuests[id].m_title = title;
}

// INLINED
static void SetRecommendedQuestSummary(CRenewQuestMgr* mgr, unsigned int id, std::string summary) {
  mgr->m_recommendedQuests[id].unknown_004C = summary;
}

// OFFSET: 0x007FD7F0
// FUNCTION: fn_09981
int UIRenewQuestWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3,
                             std::intptr_t argument4) {
  if (message == 22 || argument1 == 0xD5) {
    reinterpret_cast<WindowReplayRecorder*>(this)->RecordValues(message, argument1, argument2, argument3, argument4);
  }

  switch (message) {
  case 6:
    switch (argument1) {
    case 0xC9:
      g_windowMgr.CloseWindow(0x141);
      return 0;
    case 0xD5:
      if (g_windowMgr.window_0428) {
        g_windowMgr.window_0428->Show(argument2);
        return 0;
      }
      break;
    case 0x185:
    case 0x186:
    case 0x187:
    case 0x188:
    case 0x189:
    case 0x18A:
    case 0x18B:
    case 0x18C:
    case 0x18D:
      if (sender) {
        const unsigned int questId = this->m_questIds[this->m_tab][this->m_scroll + argument1 - 0x185];
        const int checkMessage = static_cast<UICheckBox*>(sender)->m_message;
        if (this->m_tab == 1) {
          if (checkMessage == 1) {
            g_renewQuestMgr->SetRecommendedQuestActive(questId, argument2 != 0);
            this->SendMsg(this, 0x17, 2, 0, 0, 0);
            g_renewQuestMgr->SaveRecommendedActiveList();
            return 0;
          }
        } else if (checkMessage == 0) {
          g_renewQuestMgr->SetOngoingSimpleView(questId, argument2 != 0);
          g_renewQuestMgr->SaveOngoingSimpleViewList();
          if (g_windowMgr.window_0428) {
            g_windowMgr.window_0428->Invalidate();
            return 0;
          }
        } else {
          PACKET_CZ_ACTIVE_QUEST packet;
          packet.questId = questId;
          packet.packetType = 0x2B6;
          packet.active = argument2 != 0;
          GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
          return 0;
        }
      }
      break;
    }
    break;
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_scroll;
    int delta = argument1;
    if (message == 9) {
      delta = -8;
    } else if (message == 10) {
      delta = 8;
    }
    this->m_scroll = previous + delta;
    if (previous != this->m_scroll) {
      this->UpdateScroll();
      this->Invalidate();
    }
    return this->m_scroll - previous;
  }
  case 0x16:
    if (this->m_tab != argument1) {
      this->m_tab = argument1;
      if (argument1 < 0) {
        this->m_tab = 0;
      } else if (argument1 > 3) {
        this->m_tab = 3;
      } else if (argument1 == 1 && g_renewQuestTitlesPending) {
        std::vector<unsigned int>& recommended = this->m_questIds[1];
        auto it = recommended.begin();
        if (it != recommended.end()) {
          for (; it != recommended.end(); ++it) {
            if (!g_renewQuestMgr->GetRecommendedQuest(*it).m_state) {
              break;
            }
          }
          if (it == recommended.end()) {
            it = recommended.begin();
          }
          UIRenewDetailQuestInfoWnd* detail = static_cast<UIRenewDetailQuestInfoWnd*>(g_windowMgr.EnsureWindow(0x142));
          if (detail) {
            detail->SetQuest(*it, true);
          }
        }

        CRenewQuestMgr* mgr = g_renewQuestMgr;
        if (!mgr->m_recommendedQuests.empty()) {
          for (auto quest = mgr->m_recommendedQuests.begin(); quest != mgr->m_recommendedQuests.end(); ++quest) {
            std::string title = quest->second.m_title;
            title = this->FitDisplayText(250, title.c_str(), 0, 12, 1);
            SetRecommendedQuestTitle(g_renewQuestMgr, quest->first, title);
            std::string summary = quest->second.unknown_004C;
            summary = this->FitDisplayText(250, summary.c_str(), 0, 12, 0);
            SetRecommendedQuestSummary(g_renewQuestMgr, quest->first, summary);
          }
        }
        g_renewQuestTitlesPending = false;
      }
      this->m_selected = 0;
      this->m_scroll = 0;
      this->UpdateScroll();
      this->Invalidate();
    }
    return 0;
  case 0x17:
    if (argument2 != 0) {
      bool found = false;
      for (int tab = 0; tab < 3 && !found; ++tab) {
        for (unsigned int id : this->m_questIds[tab]) {
          if (static_cast<unsigned int>(argument2) == id) {
            this->SendMsg(this, 0x16, tab, 0, 0, 0);
            found = true;
            break;
          }
        }
      }
      if (!found) {
        reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm("not found quest info", 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }
    }
    this->RefreshQuestLists(argument1, argument2);
    this->UpdateScroll();
    this->Invalidate();
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  return 0;
}
