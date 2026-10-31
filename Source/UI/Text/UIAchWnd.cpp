#include "Client/Session.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Text/ColoredText.h"
#include "UI/Text/UIAchWnd.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include <algorithm>
#include <cstring>
#include <new>

// GLOBAL: gl_25003
std::vector<AchTracingEntry> g_achievementTracking;

#pragma pack(push, 1)
struct PACKET_CZ_REQ_ACH_REWARD {
  /* 0000 */ std::int16_t packetType;
  /* 0002 */ int achievementId;
};
#pragma pack(pop)

namespace {
  // INLINED
  const BitmapImage* AchievementBitmap(const char* name) {
    return &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)))->image;
  }
}

// OFFSET: 0x007B2420
// FUNCTION: fn_09186
UIAchievementWnd::UIAchievementWnd(UIWindowContext& windows, UIFrameContext& frame, UIButtonContext& controls)
    : UIFrameWnd(windows, frame, 0), upper(nullptr), tab(nullptr), list(nullptr), group(0), sub(-1), m_controls(&controls) {
}

// OFFSET: 0x007B32B0

// OFFSET: 0x007B37F0
// FUNCTION: fn_09205
void UIAchListWnd::sub_7B37F0(int index) {
  this->selectedIndex = index;

  if (this->parent) {
    this->parent->SendMsg(this, 135, static_cast<unsigned>(index) < this->ids.size() ? this->ids[index] : -1, 0, 0, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x007B38A0
// FUNCTION: fn_09206
void UIAchTV::Clear() {
  UITextViewer::Clear();
  this->underline = 0;
}

// OFFSET: 0x007B38C0
// FUNCTION: fn_09207
void UIAchTab::ClearGroups() {
  this->groups.clear();
  this->unknown_007C = 0;
  this->unknown_0080 = -1;
  this->Refresh();
}

// OFFSET: 0x007B3910
// FUNCTION: fn_09208
void UIAchListWnd::Create(int width, int height, int rows) {
  this->unknown_0084 = height / rows;
  this->rowCount = rows;

  this->scrollBar = new UIAchScrollBar(*this->m_context);
  this->scrollBar->Configure(width - this->unknown_0094, 0, 1, height);
  this->AddChild(this->scrollBar);

  this->buttons = new UIButton*[this->rowCount];
  for (int index = 0; index < this->rowCount; ++index) {
    this->buttons[index] = new UIButton(*this->m_context, *this->m_controls);
    this->buttons[index]->SetSize(30, 30);
    this->buttons[index]->Move(5, this->unknown_0084 * index + 5);
    this->buttons[index]->SetId(index);
    this->buttons[index]->m_drawLabel = 0;
    this->AddChild(this->buttons[index]);
  }

  this->Slot15(width, height);
  this->Resize(width, height);
}

// OFFSET: 0x007B3DA0
// FUNCTION: fn_09209
void UIAchTab::DrawGroup(int group) {
  UIButtonContext& controls = *this->m_controls;
  int y = this->groupHeight * group;
  std::string name = this->groups[group].name;
  int textY = (this->groupHeight - this->TextHeight(controls.fonts, controls.locale, name.c_str(), 0, 0, this->fontHeight, 0, 0)) / 2 + y;

  if (group > this->unknown_007C) {
    y += std::min(static_cast<int>(this->groups[this->unknown_007C].subs.size()), this->visibleSubs) * this->subHeight;
    textY += std::min(static_cast<int>(this->groups[this->unknown_007C].subs.size()), this->visibleSubs) * this->subHeight;
  }

  if (group == this->unknown_007C) {
    this->DrawBitmap(0, y, AchievementBitmap("유저인터페이스\\achievement_re\\tab_press.bmp"), false);
    this->DrawTextAt(controls.fonts, controls.locale, 11, textY, name.c_str(), 0, 0, this->fontHeight, this->selectedGroupColor, 1, 0);

    int index = 0;
    std::map<int, std::string>& subs = this->groups[group].subs;
    int row = 0;
    for (auto it = subs.begin(); it != subs.end(); ++it, ++index) {
      if (row >= this->visibleSubs) {
        break;
      }

      if (index >= this->scrollPos) {
        const int rowY = (group + 1) * this->groupHeight + this->subHeight * row;
        ++row;
        name = this->groups[group].subs[it->first];
        const int height = this->TextHeight(controls.fonts, controls.locale, name.c_str(), 0, 0, this->fontHeight, 0, 0);
        const int rowHeight = this->subHeight;
        std::uint32_t color;

        if (it->first == this->unknown_0080) {
          this->DrawBitmap(0, rowY, AchievementBitmap("유저인터페이스\\achievement_re\\tab_sub_press.bmp"), false);
          this->DrawBitmap(12, rowY + 7, AchievementBitmap("유저인터페이스\\achievement_re\\tab_subarrow.bmp"), true);
          color = this->selectedSubColor;
        } else {
          const char* bitmap = it->first == this->hoverSub ? "유저인터페이스\\achievement_re\\tab_sub_over.bmp" : "유저인터페이스\\achievement_re\\tab_sub_out.bmp";
          this->DrawBitmap(0, rowY, AchievementBitmap(bitmap), false);
          color = this->subColor;
        }

        this->DrawTextAt(controls.fonts, controls.locale, 26, (rowHeight - height) / 2 + rowY, name.c_str(), 0, 0, this->fontHeight, color, 0, 0);
      }
    }
  } else {
    const char* bitmap = group == this->hoverGroup ? "유저인터페이스\\achievement_re\\tab_over.bmp" : "유저인터페이스\\achievement_re\\tab_out.bmp";
    this->DrawBitmap(0, y, AchievementBitmap(bitmap), false);
    this->DrawTextAt(controls.fonts, controls.locale, 11, textY, name.c_str(), 0, 0, this->fontHeight, this->groupColor, 1, 0);
  }
}

// OFFSET: 0x007B4220
// FUNCTION: fn_09210
std::string UIAchTab::Label(int group, int sub) {
  std::string name = this->groups[group].name;
  if (sub == -1) {
    return name;
  }

  return FormatOwnedString("%s", this->groups[group].subs[sub].c_str());
}

// OFFSET: 0x007B4370
// FUNCTION: fn_09211
bool UIAchTab::sub_7B4370(int x, int y, int* a, int* b) {
  if (x < 0 || x > this->tabWidth || y < 0) {
    return false;
  }

  const int top = (this->unknown_007C + 1) * this->groupHeight;
  const int extent = std::min(static_cast<int>(this->groups[this->unknown_007C].subs.size()), this->visibleSubs) * this->subHeight;
  if (y >= top) {
    if (y > top && y < extent + top) {
      *a = this->unknown_007C;
      *b = (y - top) / this->subHeight + this->scrollPos;
      return true;
    }

    y -= extent;
  }

  *a = y / this->groupHeight;
  *b = -1;
  return true;
}

// OFFSET: 0x007B4470
// FUNCTION: fn_09212
void UIAchievementWnd::CreateTab() {
  UIAchTab* tab = new (std::nothrow) UIAchTab(*this->m_context, *this->m_controls);
  this->tab = tab;
  tab->Slot15(0, 0);
  tab->tabWidth = 102;
  tab->groupHeight = 40;
  tab->subHeight = 26;
  tab->unknown_00AC = 0;
  tab->selectedGroupColor = 0x9C1810;
  tab->groupColor = 0x9C6B5A;
  tab->selectedSubColor = 0x9B6A59;
  tab->subColor = 0x9B6A59;
  tab->ClearGroups();

  tab->scrollBar = new UIAchScrollBar(*this->m_context);
  tab->scrollBar->Configure(tab->scrollBarX, 0, 1, tab->visibleSubs * tab->subHeight);
  tab->AddChild(tab->scrollBar);

  this->tab->Move(0, 63);
  this->AddChild(this->tab);

  this->tab->SetLabel(LookupMessage(2656), 0, -1);
  this->tab->SetLabel(LookupMessage(2657), 1, -1);
  this->tab->SetLabel(LookupMessage(2658), 1, 0);
  this->tab->SetLabel(LookupMessage(2659), 1, 1);
  this->tab->SetLabel(LookupMessage(2660), 1, 2);
  this->tab->SetLabel(LookupMessage(2661), 2, -1);
  this->tab->SetLabel(LookupMessage(2662), 2, 0);
  this->tab->SetLabel(LookupMessage(2663), 2, 1);
  this->tab->SetLabel(LookupMessage(2664), 2, 2);
  this->tab->SetLabel(LookupMessage(2665), 2, 3);
  this->tab->SetLabel(LookupMessage(2666), 2, 4);
  this->tab->SetLabel(LookupMessage(2667), 2, 5);
  this->tab->SetLabel(LookupMessage(2668), 3, -1);
  this->tab->SetLabel(LookupMessage(2669), 3, 0);
  this->tab->SetLabel(LookupMessage(2670), 3, 1);
  this->tab->SetLabel(LookupMessage(2671), 4, -1);
  this->tab->SetLabel(LookupMessage(2672), 4, 0);
  this->tab->SetLabel(LookupMessage(2673), 4, 1);
  this->tab->SetLabel(LookupMessage(2674), 5, -1);
  this->tab->SetLabel(LookupMessage(2675), 5, 0);
  this->tab->SetLabel(LookupMessage(2676), 5, 1);
  this->tab->SetLabel(LookupMessage(2677), 5, 2);
  this->tab->SetLabel(LookupMessage(2678), 6, -1);
}

// OFFSET: 0x007B5B60
// FUNCTION: fn_09216
void UIAchievementWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "", 0);

  this->upper = new (std::nothrow) UIAchUpperWnd(*this->m_context, *this->m_controls);
  this->upper->SetSize(this->geometry.width, 46);
  this->upper->Move(0, 17);
  this->AddChild(this->upper);

  this->CreateTab();

  this->list = new (std::nothrow) UIAchListWnd(*this->m_context, *this->m_controls);
  this->list->Create(320, 405, 5);
  this->list->Move(102, 63);
  this->AddChild(this->list);

  this->view = new UIAchViewWnd(*this->m_context, *this->m_controls);
  this->view->SetSize(328, 405);
  this->view->Move(422, 63);
  this->AddChild(this->view);

  this->summary = new UIAchSummaryWnd(*this->m_context, *this->m_controls);
  this->summary->SetSize(648, 404);
  this->summary->Move(102, 63);
  this->AddChild(this->summary);

  this->SelectTab(this->group, this->sub);
}

// OFFSET: 0x007B65F0
// FUNCTION: fn_09218
void UIAchScrollBar::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->maximum == 0) {
    return;
  }

  const BitmapImage* middle = AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_bg_mid.bmp");
  for (int index = 0; index < this->geometry.height / this->thickness; ++index) {
    this->DrawBitmap(0, this->thickness * index, middle, false);
  }

  this->DrawBitmap(0, this->geometry.height - this->thickness, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_bg_down.bmp"), false);
  this->DrawBitmap(0, 0, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_arrow_up.bmp"), true);
  this->DrawBitmap(0, this->geometry.height - this->arrowLength, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_arrow_down.bmp"), true);

  int y = this->ThumbOffset() + this->arrowLength;
  this->DrawBitmap(0, y, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_thumb_up.bmp"), true);

  const int end = this->ThumbLength() - 4 + y;
  while ((y += 4) < end) {
    this->DrawBitmap(0, y, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_thumb_mid.bmp"), true);
  }

  this->DrawBitmap(0, end, AchievementBitmap("유저인터페이스\\achievement_re\\scroll_v_thumb_down.bmp"), true);
}

// OFFSET: 0x007B6DB0
// FUNCTION: fn_09220
void UIAchTV::Paint() {
  if (this->transparent == 1) {
    this->ClearSurface(0xFFFF00FF);
    if (this->parent) {
      this->parent->Invalidate();
    }
  } else {
    this->ClearSurface(this->backgroundColor);
  }

  const int rows = this->lineHeight - this->bottomMargin == 0 ? this->geometry.height : this->geometry.height / this->lineHeight - this->bottomMargin;
  const unsigned end = std::min(static_cast<unsigned>(this->lines.size()), static_cast<unsigned>(rows + this->topLine));
  UIListBoxContext& owner = *this->m_listContext;

  int row = 0;
  for (int index = this->topLine; index < static_cast<int>(end); ++index, ++row) {
    const std::string& line = this->lines[index];
    if (this->outlineColor == 0) {
      this->DrawTextAt(owner.fonts, owner.locale, 3, this->lineHeight * row + 4, line.c_str(), line.size(), 0, 12, this->textColor, 0, 0);
    } else {
      DrawOutlinedText(*this, 3, this->lineHeight * row + 4, line.c_str(), line.size(), this->textColor, this->outlineColor, 0, 12, 0, 0);
    }

    if (this->underline == 1) {
      const std::uint32_t color = this->textColor;
      const int width = this->TextWidth(owner.fonts, owner.locale, this->lines[index].c_str(), 0, 0, 12, 0, 0);
      this->FillRegion(2, this->lineHeight * row + 10, width + 1, 1, color);
    }
  }
}

// OFFSET: 0x007B67B0
// FUNCTION: fn_09219
void UIAchSummaryWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  this->DrawBitmap(0, 0, AchievementBitmap("유저인터페이스\\achievement_re\\bg_summary.bmp"), false);
  this->DrawTextAt(controls.fonts, controls.locale, 82, 27, LookupMessage(2679), 0, 0, 12, 0xA55239, 1, 0);
  this->DrawRightAlignedText(600, 28, FormatOwnedString("%d/%d", this->gauges[0]->value, this->gauges[0]->maximum).c_str(), 0, 0, 11, 0x635AA5, 0);

  const int categories[6] = {2657, 2661, 2668, 2671, 2674, 2678};
  for (int index = 0; index < 6; ++index) {
    const int row = index / 2 * 50;
    const int column = index % 2;
    this->DrawTextAt(controls.fonts, controls.locale, column * 302 + 82, row + 107, LookupMessage(categories[index]), 0, 0, 12, 0xA55239, 1, 0);
    UICustomGageBar* gauge = this->gauges[index + 1];
    this->DrawRightAlignedText(column * 304 + 296, row + 108, FormatOwnedString("%d/%d", gauge->value, gauge->maximum).c_str(), 0, 0, 11, 0x635AA5, 0);
  }

  this->DrawTextAt(controls.fonts, controls.locale, 9, 292, LookupMessage(2651), 0, 0, 12, 0x9C6B5A, 1, 0);

  int x = 260;
  for (auto it = this->recent.rbegin(); it != this->recent.rend(); ++it) {
    std::string icon = "유저인터페이스\\achievement_re\\icon_" + g_achievementMgr->GetUnknown0068(it->first) + ".bmp";
    this->DrawBitmap(x - 242, 314, AchievementBitmap(icon.c_str()), true);
    this->DrawBitmap(x, 316, AchievementBitmap("유저인터페이스\\achievement_re\\badge_complete.bmp"), true);
    this->DrawTextAt(controls.fonts, controls.locale, x + 23, 325, FormatOwnedString("%d", g_achievementMgr->GetUnknown0048(it->first)).c_str(), 0, 0, 12, 0xA55239, 1, 0);
    this->DrawTextAt(controls.fonts, controls.locale, x - 241, 365, g_achievementMgr->GetUnknown0018(it->first).c_str(), 0, 0, 12, 0xC9977D, 0, 0);

    SessionRecord6144 record = g_session.FindRecord6144(it->first);
    if (g_achievementMgr->HasReward(it->first) == 1 && record.unknown_0030 == 0) {
      this->DrawBitmap(x + 12, 360, AchievementBitmap("유저인터페이스\\achievement_re\\list_rewardbox_not_receive.bmp"), true);
    }

    DrawOutlinedText(*this, x - 3, 366, FormatOwnedString("%4d.%2d.%2d", record.year, record.month, record.day).c_str(), 0, 0x635AA5, 0xFFFFFF, 0, 10, 0, 0);
    x += 316;
  }
}

// OFFSET: 0x007B6F60
// FUNCTION: fn_09221
void UIAchTab::Paint() {
  this->DrawBitmap(0, 0, AchievementBitmap("유저인터페이스\\achievement_re\\bg_tab.bmp"), false);

  if (this->parent) {
    this->parent->SendMsg(this, 116, 0, 0, 0, 0);
  }

  for (auto it = this->groups.begin(); it != this->groups.end(); ++it) {
    this->DrawGroup(it->first);
  }
}

// OFFSET: 0x007B7010
// FUNCTION: fn_09222
void UIAchUpperWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  int visible;

  if (this->title == LookupMessage(2679)) {
    this->DrawBitmap(0, 0, AchievementBitmap("유저인터페이스\\achievement_re\\bg_upper.bmp"), false);
    this->DrawRightAlignedText(130, 13, FormatOwnedString("%d/%d", this->unknown_0094, this->unknown_0098).c_str(), 0, 0, 11, 0, 0);
    this->DrawCenteredText(353, 6, FormatOwnedString("%d", g_session.unknown_614C).c_str(), 46, 0, 12, 0x582708, 1);
    this->DrawCenteredText(16, 19, FormatOwnedString("%d", g_session.unknown_6150).c_str(), 15, 0, 12, 0x44C58, 1);
    for (UIWindow* button : this->buttons) {
      button->Show(0);
    }

    visible = 0;
  } else {
    this->DrawBitmap(0, 0, AchievementBitmap("유저인터페이스\\achievement_re\\bg_upper.bmp"), false);
    this->DrawBitmap(575, 3, AchievementBitmap("유저인터페이스\\achievement_re\\upper_trophy.bmp"), true);
    this->DrawTextAt(controls.fonts, controls.locale, 169, 11, LookupMessage(3106), 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(controls.fonts, controls.locale, 169, 26, LookupMessage(3107), 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(controls.fonts, controls.locale, 614, 15, this->title.c_str(), 0, 0, 11, 0x582708, 0, 0);
    this->DrawRightAlignedText(130, 13, FormatOwnedString("%d/%d", this->unknown_0094, this->unknown_0098).c_str(), 0, 0, 11, 0, 0);
    this->DrawRightAlignedText(737, 15, FormatOwnedString("(%d/%d)", this->unknown_009C, this->unknown_00A0).c_str(), 0, 0, 11, 0x582708, 0);
    this->DrawCenteredText(353, 6, FormatOwnedString("%d", g_session.unknown_614C).c_str(), 46, 0, 12, 0x582708, 1);
    this->DrawCenteredText(16, 19, FormatOwnedString("%d", g_session.unknown_6150).c_str(), 15, 0, 12, 0x44C58, 1);
    for (UIWindow* button : this->buttons) {
      button->Show(1);
    }

    visible = 1;
  }

  this->unknown_00A8->Show(visible);
}

// OFFSET: 0x007B7420
// FUNCTION: fn_09223
void UIAchViewWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  this->DrawBitmap(0, 0, AchievementBitmap("유저인터페이스\\achievement_re\\bg_detail.bmp"), false);
  if (this->achievementId == -1 || this->info == nullptr) {
    return;
  }

  if (static_cast<unsigned char>(g_session.FindRecord6144(this->achievementId).unknown_0000 >> 8) == 1) {
    this->DrawBitmap(264, 3, AchievementBitmap("유저인터페이스\\achievement_re\\detail_stamp.bmp"), true);
  }

  this->DrawTextAt(controls.fonts, controls.locale, 15, 14, this->info->title.c_str(), 0, 0, 12, 0xA55239, 1, 0);

  for (unsigned index = 0; index < this->info->rewards.size(); ++index) {
    if (this->info->unknown_0064 == 0) {
      this->DrawBitmap(20, 103 + 19 * index, AchievementBitmap("유저인터페이스\\achievement_re\\detail_bullet.bmp"), true);
    } else if (this->info->unknown_0064 == 1) {
      this->DrawBitmap(20, 111 + 36 * index, AchievementBitmap("유저인터페이스\\achievement_re\\detail_bullet.bmp"), true);
    }
  }

  if (this->info->unknown_004C > 0 || this->info->unknown_0050 > 0 || this->info->unknown_0054 > 0) {
    int width = 0;
    int count = 0;
    for (UIToolTipBitmapWnd* reward : this->rewards) {
      if (reward->geometry.visible == 1) {
        width += reward->geometry.width;
        ++count;
      }
    }

    int x = static_cast<int>((this->geometry.width - count * 8 - width + 8) * 0.5);
    for (UIToolTipBitmapWnd* reward : this->rewards) {
      if (reward->geometry.visible == 1) {
        reward->Move(x, 304);
        x += 70;
      }
    }
  }
}

// OFFSET: 0x007B7650
// FUNCTION: fn_09224
void UIAchievementWnd::Paint() {
  this->ClearSurface(0xFFFFFFFF);
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, LookupMessage(2646), 0);
}

// OFFSET: 0x007B7680
// FUNCTION: fn_09225
void UICustomGageBar::Paint() {
  if (this->value < 0 || this->maximum < 0 || this->value > this->maximum) {
    return;
  }

  DrawRectangleBorder(*this, 0, 0, this->geometry.width, this->geometry.height, this->outerBorderColor);
  DrawRectangleBorder(*this, 1, 1, this->geometry.width - 2, this->geometry.height - 2, this->innerBorderColor);
  this->FillRegion(2, 2, this->geometry.width - 4, this->geometry.height - 4, this->backgroundColor);

  const float percent = static_cast<float>(this->value) * 100.0f / static_cast<float>(this->maximum);
  if (percent > 0.0f) {
    this->FillRegion(2, 2, static_cast<int>(percent) * this->geometry.width / 100, this->geometry.height - 4, this->barColor);
  }
}

// OFFSET: 0x007B7760
// FUNCTION: fn_09226
void UIToolTipBitmapWnd::Paint() {
  this->bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
  if (this->bitmap) {
    this->DrawBitmap(0, 0, &this->bitmap->image, false);
  }

  const int item = this->itemId;
  if (item > 0) {
    this->unknown_00B4 = 20;
    this->unknown_00B8 = 15;
    ITEM_INFO info;
    info.SetItemId(item);
    char path[260];
    std::memset(path, 0, sizeof(path));
    ItemDescriptions descriptions{g_itemDescriptions};
    descriptions.IconPath(path, info.itemId.c_str(), 1);
    CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
    if (icon) {
      this->DrawBitmap(20, 15, &icon->image, true);
    }
  }
}

// OFFSET: 0x007B7B60
// FUNCTION: fn_09236
void UIAchTab::Slot27(int x, int y) {
  int sub = 0;
  int group = 0;
  this->sub_7B4370(x, y, &group, &sub);
  this->hoverGroup = group;
  this->hoverSub = sub;
  this->Invalidate();
}

// OFFSET: 0x007B7BB0
// FUNCTION: fn_09237
void UIToolTipBitmapWnd::Slot32(int x, int y) {
  if (this->itemId > 0 && this->unknown_00B4 <= x && x <= this->unknown_00B4 + 24 && this->unknown_00B8 <= y && y <= this->unknown_00B8 + 24) {
    ITEM_INFO item;
    item.SetItemId(this->itemId);
    item.identified = 1;
    this->m_windows->EnsureWindow(12)->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x007B7CA0
// FUNCTION: fn_09238
void UIAchievementWnd::SelectAchievement(int id) {
  int group = -1;
  int sub = -1;
  if (g_achievementMgr->FindTab(id, group, sub) != 1) {
    return;
  }

  this->SelectTab(group, sub);

  UIAchListWnd* list = this->list;
  int index = 0;
  for (std::vector<int>::iterator it = list->ids.begin(); it != list->ids.end() && *it != id; ++it) {
    ++index;
  }

  list->selectedIndex = index;
  if (index < list->rowCount) {
    list->unknown_008C = 0;
  } else {
    list->unknown_008C = std::min(static_cast<unsigned>(index), static_cast<unsigned>(list->unknown_00AC->count - list->rowCount));
  }

  list->scrollBar->SetPosition(list->unknown_008C);
  if (list->parent) {
    list->parent->SendMsg(list, 135, static_cast<unsigned>(list->selectedIndex) < list->ids.size() ? list->ids[list->selectedIndex] : -1, 0, 0, 0);
  }

  list->Invalidate();
}

// OFFSET: 0x007B7D90
// FUNCTION: fn_09239
void UIAchListWnd::Slot35(int delta) {
  if (!this->scrollable) {
    return;
  }

  if (delta < 0) {
    this->SendMsg(this->scrollBar, 7, 3, 0, 0, 0);
    return;
  }

  this->SendMsg(this->scrollBar, 7, -3, 0, 0, 0);
}

// OFFSET: 0x007B7DD0
// FUNCTION: fn_09240
void UIAchTab::Slot35(int delta) {
  if (!this->scrollable) {
    return;
  }

  if (delta < 0) {
    this->SendMsg(this->scrollBar, 7, 1, 0, 0, 0);
    return;
  }

  this->SendMsg(this->scrollBar, 7, -1, 0, 0, 0);
}

// OFFSET: 0x007B7E10
// FUNCTION: fn_09241
void UIAchListWnd::UpdateScroll() {
  UIAchListInfo* info = this->unknown_00AC;
  if (!info || !this->scrollBar) {
    return;
  }

  const int rows = this->rowCount;
  const int overflow = std::max(info->count - rows, 0);
  this->scrollable = overflow > 0;

  if (overflow < 1) {
    this->unknown_008C = 0;
    this->scrollBar->SetRange(0, rows, info->count);
  } else {
    this->scrollBar->SetRange(overflow, rows, info->count);
    if (this->unknown_008C < 0) {
      this->unknown_008C = 0;
      this->scrollBar->SetPosition(0);
    } else {
      if (this->unknown_008C > overflow) {
        this->unknown_008C = overflow;
      }
      this->scrollBar->SetPosition(this->unknown_008C);
    }
  }

  this->scrollBar->Invalidate();

  for (int index = 0; index < this->rowCount; ++index) {
    UIButton* button = this->buttons[index];
    if (static_cast<unsigned>(this->unknown_008C + index) < static_cast<unsigned>(this->unknown_00AC->count)) {
      button->SetDisabled(0);
    } else {
      button->SetDisabled(1);
    }
    button->Invalidate();
  }
}

// OFFSET: 0x007B7F10
// FUNCTION: fn_09242
void UIAchTab::sub_7B7F10() {
  if (!this->scrollBar) {
    return;
  }

  const int count = static_cast<int>(this->groups[this->unknown_007C].subs.size());
  const int visible = this->visibleSubs;
  const int overflow = std::max(count - visible, 0);
  this->scrollable = overflow > 0;

  if (overflow < 1) {
    this->scrollPos = 0;
    this->scrollBar->SetRange(0, visible, count);
  } else {
    this->scrollBar->SetRange(overflow, visible, count);
    if (this->scrollPos < 0) {
      this->scrollPos = 0;
      this->scrollBar->SetPosition(0);
    } else {
      if (this->scrollPos > overflow) {
        this->scrollPos = overflow;
      }
      this->scrollBar->SetPosition(this->scrollPos);
    }
  }

  this->scrollBar->Move(0, (this->unknown_007C + 1) * this->groupHeight);
  this->scrollBar->Invalidate();
}

// OFFSET: 0x007B7FE0
// FUNCTION: fn_09243
void UIAchTab::Refresh() {
  int height = static_cast<int>(this->groups.size()) * this->groupHeight;
  for (auto it = this->groups.begin(); it != this->groups.end(); ++it) {
    height += static_cast<int>(this->groups[it->first].subs.size()) * this->subHeight;
  }

  this->Resize(this->tabWidth, height + 1);
  this->Invalidate();
}

// OFFSET: 0x007B8170
// FUNCTION: fn_09245
int UIAchSummaryWnd::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 98 && argument1 != -1 && this->parent) {
    static_cast<UIAchievementWnd*>(this->parent)->SelectAchievement(argument1);
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x007B81B0
// FUNCTION: fn_09246
int UIAchTab::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message != 7 && message != 9 && message != 10) {
    return 0;
  }

  const int previous = this->scrollPos;
  if (message == 9) {
    this->scrollPos = previous - this->visibleSubs + 1;
  } else {
    if (message == 10) {
      argument1 = this->visibleSubs - 1;
    }
    this->scrollPos = previous + argument1;
  }

  if (previous != this->scrollPos) {
    this->sub_7B7F10();
    this->Invalidate();
  }

  return this->scrollPos - previous;
}

// OFFSET: 0x007B8230
// FUNCTION: fn_09247
int UIAchUpperWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6 && argument1 == 215) {
    for (int index = 0; index < 2; ++index) {
      this->buttons[index]->Invalidate();
      if (sender == this->buttons[index]) {
        this->buttons[index]->SendMsg(this, 13, 1, 0, 0, 0);
        this->selected = index;
        if (this->parent) {
          this->parent->SendMsg(this, 22, -1, -1, 0, 0);
        }
      } else {
        this->buttons[index]->SendMsg(this, 13, 0, 0, 0, 0);
      }
    }
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x007B82C0
// FUNCTION: fn_09248
int UIAchViewWnd::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6) {
    if (argument1 == 474) {
      PACKET_CZ_REQ_ACH_REWARD packet;
      packet.packetType = ID_CZ_REQ_ACH_REWARD;
      packet.achievementId = this->achievementId;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.packetType), &packet);
    }
  } else if (message == 145 && argument1 != -1 && this->parent) {
    static_cast<UIAchievementWnd*>(this->parent)->SelectAchievement(argument1);
    return 0;
  }

  return 0;
}

// OFFSET: 0x007B8810
// FUNCTION: fn_09250
void UIAchListWnd::SetAchievements(std::map<int, AchievementInfo>* achievements, int mode) {
  this->ids.clear();

  auto byProgress = [this](int left, int right) {
    return g_session.FindRecord6144(left).unknown_002C < g_session.FindRecord6144(right).unknown_002C;
  };

  if (mode == 0) {
    for (auto it = achievements->begin(); it != achievements->end(); ++it) {
      if (static_cast<unsigned char>(g_session.FindRecord6144(it->first).unknown_0000 >> 8) == 1) {
        this->ids.push_back(it->first);
      }
      std::sort(this->ids.begin(), this->ids.end(), byProgress);
    }

    for (auto it = achievements->begin(); it != achievements->end(); ++it) {
      if (static_cast<unsigned char>(g_session.FindRecord6144(it->first).unknown_0000 >> 8) == 0) {
        this->ids.push_back(it->first);
      }
    }
  } else if (mode == 1) {
    for (auto it = achievements->begin(); it != achievements->end(); ++it) {
      SessionRecord6144 record = g_session.FindRecord6144(it->first);
      if (static_cast<unsigned char>(record.unknown_0000 >> 8) == 1 && g_achievementMgr->HasReward(it->first) == 1 && record.unknown_0030 == 0) {
        this->ids.push_back(it->first);
      }
      std::sort(this->ids.begin(), this->ids.end(), byProgress);
    }

    for (auto it = achievements->begin(); it != achievements->end(); ++it) {
      if (static_cast<unsigned char>(g_session.FindRecord6144(it->first).unknown_0000 >> 8) == 0) {
        this->ids.push_back(it->first);
      }
    }
  }

  this->unknown_00AC = reinterpret_cast<UIAchListInfo*>(achievements);
  this->selectedIndex = 0;
  this->unknown_008C = 0;
  this->UpdateScroll();
  this->selectedIndex = 0;
  if (this->parent) {
    this->parent->SendMsg(this, 135, this->ids.empty() ? -1 : this->ids[0], 0, 0, 0);
  }

  this->Invalidate();
}

// OFFSET: 0x007B8C20
// FUNCTION: fn_09251
void UIToolTipBitmapWnd::Set(const char* name, const char* text, int item) {
  if (text) {
    this->tooltip = std::string(text);
  }

  this->bitmapName = name;
  this->bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  if (this->bitmap) {
    this->Resize(this->bitmap->image.width, this->bitmap->image.height);
  }

  this->itemId = item;
}

// OFFSET: 0x007B8DC0
// FUNCTION: fn_09252
void UIAchSummaryWnd::Show(int visible) {
  this->geometry.visible = visible;
  for (UIWindow* button : this->buttons) {
    button->Slot27(-1, -1);
  }
}

// OFFSET: 0x007B8DF0
// FUNCTION: fn_09253
void UIAchievementWnd::SelectTab(int group, int sub) {
  if (group == 0) {
    this->view->Show(0);
    this->list->Show(0);
  } else {
    this->view->Show(1);
    this->list->Show(1);
  }
  this->summary->Show(group == 0);

  this->group = group;
  this->sub = sub;
  this->tab->unknown_007C = group;
  this->tab->unknown_0080 = sub;
  this->tab->Invalidate();

  UIAchUpperWnd* upper = this->upper;
  const std::string title = group == 0 ? std::string(LookupMessage(2679)) : this->tab->Label(this->group, this->sub);
  upper->title = title;
  this->upper->Invalidate();

  if (g_achievementMgr->GetGroupAchievements(this->achievements, this->group, this->sub) == 1) {
    this->list->SetAchievements(&this->achievements, this->upper->selected);
  }

  this->SendMsg(nullptr, 60, 0, 0, 0, 0);
}

// OFFSET: 0x007B8FA0
// FUNCTION: fn_09254
void UIAchTab::SetLabel(const char* text, int group, int sub) {
  if (sub == -1) {
    this->groups[group].name = text;
  } else {
    this->groups[group].subs[sub] = text;
  }

  this->unknown_007C = 0;
  this->unknown_0080 = -1;
  this->Refresh();
}

// OFFSET: 0x007BA460
// FUNCTION: fn_09267
bool UIAchTracingWnd::Slot08() {
  return true;
}

// OFFSET: 0x007BAA20
// FUNCTION: fn_09271
void UIAchTracingWnd::Slot11() {
  if (this->info) {
    this->info->x = this->geometry.x;
    this->info->y = this->geometry.y;
  }
}

// OFFSET: 0x007BA470
// FUNCTION: fn_09268
void UIAchTracingWnd::Slot15(int, int) {
  for (int index = 0; index < 3; ++index) {
    this->toggleButtons[index] = new UITextButton(*this->m_context, *this->m_controls);
    this->toggleButtons[index]->Configure("", 0, 0, 12, 1, 0);
    this->toggleButtons[index]->SetId(478);
    this->AddChild(this->toggleButtons[index]);

    this->openButtons[index] = new UITextButton(*this->m_context, *this->m_controls);
    this->openButtons[index]->Configure("▶", 0, 0, 12, 1, 0);
    this->openButtons[index]->SetId(479);
    this->AddChild(this->openButtons[index]);
  }
}

// OFFSET: 0x007BA590
// FUNCTION: fn_09269
void UIAchTracingWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  this->ClearSurface(0xFFFF00FF);

  int y = 10;
  for (int index = 0; index < 3; ++index) {
    this->toggleButtons[index]->Enable(0);
    this->openButtons[index]->Enable(0);
  }

  for (int pass = 0; pass < 2; ++pass) {
    UITextButton** buttons = this->toggleButtons;
    for (unsigned int index = 0; index < g_achievementTracking.size(); ++index, ++buttons) {
      if (pass == 0 ? g_achievementTracking[index].pinned == 1 : g_achievementTracking[index].pinned != 1) {
        continue;
      }

      buttons[0]->Enable(1);
      buttons[3]->Enable(1);
      UITextButton* toggle = buttons[0];
      const std::string label = FormatOwnedString("● [%s]", g_achievementTracking[index].name.c_str());
      toggle->label = label;
      toggle->ResizeToText();
      buttons[0]->Move(10, y);
      buttons[0]->m_value = g_achievementTracking[index].id;
      buttons[3]->Move(this->geometry.width - 20, y);
      buttons[3]->m_value = g_achievementTracking[index].id;

      if (!g_achievementTracking[index].expanded) {
        this->DrawTextAt(
          controls.fonts, controls.locale, buttons[0]->geometry.width + 16, y + 1,
          FormatOwnedString("(+%d)", g_achievementTracking[index].conditions.size()).c_str(), 0, 0, 12, 0xFFFFFF, 1, 0
        );
        this->DrawTextAt(
          controls.fonts, controls.locale, buttons[0]->geometry.width + 15, y,
          FormatOwnedString("(+%d)", g_achievementTracking[index].conditions.size()).c_str(), 0, 0, 12, 0, 1, 0
        );
      }

      AchTracingEntry& entry = g_achievementTracking[index];
      y += 15;
      if (entry.expanded == 1) {
        for (const AchTracingCondition& condition : entry.conditions) {
          this->DrawTextAt(controls.fonts, controls.locale, 21, y + 1, condition.text.c_str(), 0, 0, 12, 0, 0, 0);
          std::uint32_t color = 0xFFFFFF;
          if (condition.completed) {
            color = 0xDFF6F;
          }
          this->DrawTextAt(controls.fonts, controls.locale, 20, y, condition.text.c_str(), 0, 0, 12, color, 0, 0);
          y += 15;
        }
      }
      y += 5;
    }
  }
}

// OFFSET: 0x007BA920
// FUNCTION: fn_09270
int UIAchTracingWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 34) {
    this->info = reinterpret_cast<AchTracingWndInfo*>(argument1);
    if (!this->info->IsValid()) {
      this->info->Reset();
    }
    this->Move(this->info->x, this->info->y);
  } else if (message == 98) {
    if (argument1 == 478) {
      auto it = std::find_if(g_achievementTracking.begin(), g_achievementTracking.end(), [argument2](AchTracingEntry entry) {
        return argument2 == entry.id;
      });
      if (it != g_achievementTracking.end()) {
        it->expanded = !it->expanded;
        this->Invalidate();
        return 0;
      }
    } else if (argument1 == 479) {
      UIWindow* window = this->m_frameContext->windows.EnsureWindow(270);
      if (window) {
        static_cast<UIAchievementWnd*>(window)->SelectAchievement(argument2);
        this->Invalidate();
        return 0;
      }
    }
  } else {
    return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  this->Invalidate();
  return 0;
}

// OFFSET: 0x007B7A20
// FUNCTION: fn_09234
void UICustomGageBar::Slot28(int x, int y) {
  std::string label = FormatOwnedString("%s( %d / %d )", this->text.c_str(), this->value, this->maximum);
  int tipX = 0;
  int tipY = -20;
  this->ToScreen(tipX, tipY);
  reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(label.c_str(), tipX, tipY, 0xFFFFFFFFu, 0, 1);
}

// OFFSET: 0x007B7AF0
// FUNCTION: fn_09235
void UIToolTipBitmapWnd::Slot28(int x, int y) {
  this->Slot27(x, y);
  if (this->tooltip.size() != 0) {
    x = 0;
    y = -20;
    this->ToScreen(x, y);
    reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(this->tooltip.c_str(), x, y, 0xFFFFFFFFu, 0, 1);
  }
}

// OFFSET: 0x007B50C0
// FUNCTION: fn_09213
void UIAchSummaryWnd::Slot15(int, int) {
  for (int index = 0; index < this->unknown_007C; ++index) {
    UICustomGageBar* gauge = new (std::nothrow) UICustomGageBar(*this->m_context);
    if (index == 0) {
      gauge->Slot15(558, 10);
      gauge->Resize(558, 10);
      gauge->value = 0;
      gauge->maximum = 100;
      gauge->Move(43, 49);
    } else {
      gauge->Slot15(254, 10);
      gauge->Resize(254, 10);
      gauge->value = 0;
      gauge->maximum = 100;
      gauge->Move((index - 1) % 2 * 304 + 43, (index - 1) / 2 * 50 + 123);
    }
    gauge->barColor = 0xC78A71;
    gauge->backgroundColor = 0xFFFFFF;
    gauge->outerBorderColor = 0xFFFFFF;
    gauge->innerBorderColor = 0xDAA896;
    this->AddChild(gauge);
    this->gauges.push_back(gauge);
  }

  UIWindow** slot = this->buttons;
  for (int x = 62; x < 694; x += 316) {
    UITextButton* button = new UITextButton(*this->m_context, *this->m_controls);
    *slot = button;
    button->Configure("", 0x997836, 0, 12, 1, 0);
    static_cast<UITextButton*>(*slot)->m_hoverColor = 0x635AA5;
    (*slot)->Move(x, 325);
    (*slot)->SetId(-1);
    this->AddChild(*slot);
    ++slot;
  }
}
