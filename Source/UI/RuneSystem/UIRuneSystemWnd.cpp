#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Contents/Items/RuneSystem.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/RuneSystem/UIRuneSystemWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

// GLOBAL: gl_24886
extern const int g_runeSetSlotX[6] = {87, 201, 19, 269, 87, 201};
// GLOBAL: gl_24887
extern const int g_runeSetSlotY[6] = {70, 70, 207, 207, 344, 344};

// OFFSET: 0x00816630
// FUNCTION: fn_10164
void UIRuneSetList::ScrollBy(int delta) {
  const int previous = this->position;
  int next = delta + previous;
  next = next >= 0 ? next : 0;
  next = next > this->maximum ? this->maximum : next;
  this->position = next;
  if (previous != next) {
    this->Invalidate();
  }
}

// OFFSET: 0x00816830
// FUNCTION: fn_10166
void UIRune_Decom::Reset() {
  this->unknown_007C.clear();
  this->unknown_0094.clear();
  this->m_ownedCount = 0;
  this->m_itemId = 0;
  this->m_decomList = nullptr;
  this->m_decomButton->SetDisabled(1);
  this->m_decomButton->Invalidate();

  this->m_decomType = 0;
  for (unsigned int index = 0; index < 2; ++index) {
    this->m_radioButtons[index]->Show(0);
    this->m_radioButtons[index]->Invalidate();
    this->m_radioButtons[index]->SendMsg(nullptr, 0x0D, this->m_decomType != index, 0, 0, 0);
  }
  this->Invalidate();
}

// OFFSET: 0x00816AD0
// FUNCTION: fn_10168
int UIRuneSystem_ActiveWnd::HitTestItem(int x, int y) {
  if (this->m_listId == -1 || x < 18 || y < 11 || y > 119) {
    return -1;
  }

  int column = 0;
  if (x - 18 > 0) {
    column = (x - 18) / 54;
    if (column > this->m_columns - 1) {
      column = this->m_columns - 1;
    }
  }
  const int row = y - 11 > 54 ? this->m_columns : 0;
  const int index = row + column;
  if (index < g_runeSystemMgr->GetItemListCount(this->m_listId)) {
    return index;
  }
  return -1;
}

// OFFSET: 0x00816BE0
// FUNCTION: fn_10169
int UIRune_Decom::HitTestItem(int x, int y) {
  std::vector<Decom_ItemInfo>* list = this->m_decomList;
  if (!list || x < 211 || x > 411 || y < 354 || y > 446) {
    return -1;
  }

  const int offsetX = x - 211;
  const int offsetY = y - 354;
  int index = offsetX / 54;
  if (offsetX % 54 > 38) {
    return -1;
  }
  if (offsetY - 54 > 0) {
    index += 4;
  } else if (offsetY > 38) {
    return -1;
  }

  if (static_cast<unsigned int>(index) < list->size()) {
    return index;
  }
  return -1;
}

// OFFSET: 0x00819610
// FUNCTION: fn_10181
void UIRuneSetReWard::Paint() {
  this->ClearSurface(0xFFFF00FF);
  const std::string* name;
  if (this->m_state == 3) {
    name = &this->m_texts[1];
  } else if (this->m_state == 0) {
    name = &this->m_texts[0];
  } else if (this->m_state == 1) {
    switch (this->geometry.unknown_0030) {
    case 0:
      name = &this->m_names[3];
      break;
    case 1:
    case 2:
      name = &this->m_texts[2];
      break;
    default:
      name = nullptr;
      break;
    }
  } else {
    switch (this->geometry.unknown_0030) {
    case 0:
      name = &this->m_names[0];
      break;
    case 1:
      name = &this->m_names[1];
      break;
    case 2:
      name = &this->m_names[2];
      break;
    default:
      name = nullptr;
      break;
    }
  }

  if (name && !name->empty()) {
    this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name->c_str())))->image, true);
  }
  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x0081A1D0
// FUNCTION: fn_10184
void UIRuneSystemTab::Paint() {
  ResourceMgr& resources = ResourceMgr::Instance();
  const std::string name = FormatOwnedString(this->m_label.c_str(), this->geometry.unknown_0030);
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(resources.Load(Resolve(name.c_str())))->image, false);
}

// OFFSET: 0x0081BD10
// FUNCTION: fn_10194
void UIRuneSetList::DrawScrollBar(bool drawLine) {
  const int x = this->geometry.width - 7;
  this->FillRegion(x, 0, 7, this->geometry.height, 0xDCC4B1);
  this->FillRegion(x + 1, 1, 5, this->geometry.height - 2, 0xFFFFFF);
  if (drawLine) {
    this->FillRegion(0, 0, this->geometry.width, 1, 0xD1AF9E);
  }

  if (this->maximum == 0) {
    return;
  }

  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\scrollbar_arrow_upper.bmp")));
  this->DrawBitmap(x, 1, bitmap ? &bitmap->image : nullptr, true);
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\scrollbar_arrow_down.bmp")));
  this->DrawBitmap(x, this->geometry.height - this->arrowLength - 1, bitmap ? &bitmap->image : nullptr, true);

  const int top = this->arrowLength + 1 + this->ThumbOffset();
  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\scrollBar_up.bmp")));
  this->DrawBitmap(x, top, bitmap ? &bitmap->image : nullptr, true);

  const int bottom = this->ThumbLength() - 5 + top;
  for (int y = top + 3; y < bottom; y += 3) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\scrollBar_m.bmp")));
    this->DrawBitmap(x, y, bitmap ? &bitmap->image : nullptr, true);
  }

  bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\scrollBar_down.bmp")));
  this->DrawBitmap(x, bottom, bitmap ? &bitmap->image : nullptr, true);
}

// OFFSET: 0x0081BE90
// FUNCTION: fn_10195
void UIEquipRuneSetWnd::Slot26(int, int) {
  this->parent->SendMsg(this, 0xAB, 0x158, 0, 0, 0);
}

// OFFSET: 0x0081BEC0
// FUNCTION: fn_10196
void UIRuneSet_Info::Slot26(int x, int y) {
  RuneSetData* runeSet = this->m_runeSet;
  if (!runeSet) {
    return;
  }

  int slot = -1;
  for (int index = x > 200; index < 6; index += 2) {
    if (g_runeSetSlotX[index] < x && x < g_runeSetSlotX[index] + 90 && g_runeSetSlotY[index] < y && y < g_runeSetSlotY[index] + 120) {
      slot = index;
      break;
    }
  }

  if (slot != -1 && static_cast<unsigned int>(slot) < runeSet->m_slots.size() && runeSet->m_slots[slot]) {
    if (runeSet->GetTagId() != g_runeSystemMgr->GetSelectTag()) {
      g_runeSystemMgr->SetSelectTag(this->m_runeSet->GetTagId());
      g_runeSystemMgr->SendRequest(0xBCB, nullptr, -1);
    }
    this->parent->SendMsg(this, 0xA9, 1, static_cast<std::intptr_t>(this->m_runeSet->m_slots[slot]->GetId()), 0, 0);
  }
}

// OFFSET: 0x0081BFD0
// FUNCTION: fn_10197
void UIEquipRuneSetWnd::Slot25(int, int) {
  this->parent->SendMsg(this, 0xAB, 0xC2, -1, 0, 0);
}

// OFFSET: 0x0081C000
// FUNCTION: fn_10198
void UIRuneSetList::Slot25(int x, int y) {
  if (y == 0) {
    return;
  }

  if (x < this->geometry.width - 7 && y < this->visible * this->m_itemHeight) {
    this->m_selected = y / this->m_itemHeight + this->position;
    this->parent->SendMsg(this, 0xAB, 0xC2, this->m_selected, 0, 0);
    this->Invalidate();
    return;
  }

  UIScrollBar::Slot25(x, y);
}

// OFFSET: 0x0081C080
// FUNCTION: fn_10199
void UIRuneSystemTab::Slot25(int x, int) {
  if (!this->parent) {
    return;
  }

  const int tabWidth = this->geometry.width / this->m_count;
  int index = x / tabWidth;
  index = index > 0 ? index : 0;
  index = index < this->m_count - 1 ? index : this->m_count - 1;

  if (index < 3 && static_cast<int>(this->geometry.unknown_0030) != index) {
    this->geometry.unknown_0030 = index;
    this->Invalidate();
    this->parent->SendMsg(this, 0x16, this->geometry.unknown_0030, 0, 0, 0);
  }
}

// OFFSET: 0x0081C0E0
// FUNCTION: fn_10200
void UIRuneSystemWnd::Slot25(int, int) {
}

// OFFSET: 0x0081C0F0
// FUNCTION: fn_10201
void UIRuneSystem_tagListWnd::Slot25(int x, int) {
  if (x < 20 || x > this->geometry.width - 23) {
    return;
  }

  const int index = (x - 20) / 27;
  if (index > -1 && index < g_runeSystemMgr->GetTagCount()) {
    CTagData* tag = g_runeSystemMgr->GetTagByIndex(index);
    if (tag) {
      g_runeSystemMgr->SetSelectTag(tag->m_id);
      g_runeSystemMgr->SendRequest(0xBCB, nullptr, -1);
      this->parent->SendMsg(this, 0x3C, 0, 0, 0, 0);
      this->Invalidate();
    }
  }
}

// OFFSET: 0x0081C190
// FUNCTION: fn_10202
void UIRuneSetReWard::Slot31(int x, int y) {
  if (this->m_state == 2) {
    UIBitmapButton::Slot31(x, y);
    return;
  }

  if (this->m_state == 3) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4200)), 0xFF, 0, 0, 0);
  }
  if (this->m_state == 0) {
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4201)), 0xFF, 0, 0, 0);
  }

  if (g_windowMgr.Capture() == this) {
    g_windowMgr.ReleaseCapture();
  }
}

// OFFSET: 0x0081C230
// FUNCTION: fn_10203
void UIDecomlist::Slot28(int, int) {
}

// OFFSET: 0x0081C240

// OFFSET: 0x0081C3D0
// FUNCTION: fn_10205
void UIRuneList::Slot28(int, int) {
}

// OFFSET: 0x0081C3E0
// FUNCTION: fn_10206
void UIRuneSetList::Slot28(int x, int y) {
  if (x < 19 || x > 256 || y < 25 || y > this->geometry.height - 10) {
    return;
  }

  const int row = (y - 1) / this->m_itemHeight;
  const int cellY = y - row * this->m_itemHeight;
  if (cellY < 24 || cellY > 61) {
    return;
  }

  const int column = (x - 19) / 40;
  if (x - column * 40 - 19 > 37) {
    return;
  }

  RuneSetData* runeSet = dynamic_cast<RuneSetData*>(g_runeSystemMgr->GetCurrentRune(0, this->position + row));
  if (!runeSet) {
    return;
  }
  if (static_cast<unsigned int>(column) >= runeSet->m_slots.size()) {
    return;
  }

  int screenX = column * 40 + 17;
  int screenY = row * this->m_itemHeight + 4;
  this->ToScreen(screenX, screenY);
  if (!g_modeMgr.gameMode) {
    return;
  }

  RuneData* rune = runeSet->m_slots[column];
  if (!rune) {
    return;
  }
  reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(rune->GetName(), screenX, screenY, 0xFFFFFF, 0, 1);
}

// OFFSET: 0x0081C6F0
// FUNCTION: fn_10208
void UIRuneSet_Info::Slot28(int x, int y) {
  RuneSetData* runeSet = this->m_runeSet;
  if (!runeSet || !runeSet->IsActive()) {
    return;
  }

  for (unsigned int index = 0; index < runeSet->m_slots.size(); ++index) {
    if (g_runeSetSlotX[index] < x && x < g_runeSetSlotX[index] + 90 && g_runeSetSlotY[index] < y && y < g_runeSetSlotY[index] + 120) {
      if (!runeSet->m_slots[index]) {
        break;
      }

      int screenX = g_runeSetSlotX[index] - 6;
      int screenY = g_runeSetSlotY[index] - 24;
      this->ToScreen(screenX, screenY);
      if (g_modeMgr.gameMode) {
        reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(runeSet->m_slots[index]->GetName(), screenX, screenY, 0xFFFFFF, 0, 1);
      }
    }
  }
}

// OFFSET: 0x0081C960
// FUNCTION: fn_10210
void UIRuneSystem_tagListWnd::Slot28(int x, int) {
  if (x < 20 || x > this->geometry.width - 23) {
    return;
  }

  const int index = (x - 20) / 27;
  if (index > -1 && index < g_runeSystemMgr->GetTagCount()) {
    std::string name = g_runeSystemMgr->GetTagByIndex(index)->m_name;
    if (!name.empty()) {
      int screenX = index * 24 + 19;
      int screenY = -18;
      this->ToScreen(screenX, screenY);
      if (g_modeMgr.gameMode) {
        reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(name.c_str(), screenX, screenY, 0xFFFFFF, 0, 1);
      }
    }
  }
}

// OFFSET: 0x0081CBF0
// FUNCTION: fn_10212
void UIRuneSet_Info::Slot19() {
  this->m_gauge->Slot19();
}

// OFFSET: 0x0081CC00
// FUNCTION: fn_10213
void UIRuneSystemWnd::Slot19() {
  switch (this->m_tab->geometry.unknown_0030) {
  case 0:
    this->m_runeSetInfo->Slot19();
    break;
  case 1:
    this->m_runeInfo->Slot19();
    break;
  case 2:
    this->m_runeDecom->Slot19();
    break;
  }
}

// OFFSET: 0x0081CF10
// FUNCTION: fn_10217
void UIRuneSetList::Slot35(int delta) {
  const int previous = this->position;
  int next = previous + (delta < 0 ? 1 : -1);
  if (next < 0) {
    next = 0;
  }
  if (next > this->maximum) {
    next = this->maximum;
  }

  this->position = next;
  if (previous != next) {
    this->Invalidate();
  }
}

// OFFSET: 0x0081D460
// FUNCTION: fn_10220
void UIRuneSet_Info::UpdateActivation() {
  this->m_activeWnd->Show(0);
  this->unknown_0084->Enable(0);
  if (this->m_runeSet && !this->m_runeSet->IsActive()) {
    this->m_activeWnd->Show(1);
    this->unknown_0084->Enable(1);
    UIBitmapButton* button = this->unknown_0084;
    button->SetDisabled(!g_runeSystemMgr->HasRequiredItems(this->m_runeSet->GetActiveType()));
    button->Invalidate();
  }
  this->Invalidate();
}

// OFFSET: 0x0081D580
// FUNCTION: fn_10221
int UIRuneSetList::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 0x3C) {
    if (argument1 > 0 && this->total != argument1) {
      int maximum = argument1 - this->visible;
      if (maximum < 0) {
        maximum = 0;
      }
      this->SetRange(maximum, this->visible, argument1);
      this->SetPosition(0);
    }
    this->Invalidate();
  } else if (message == 0xAB) {
    this->m_selected = argument1;
    this->Invalidate();
    return 0;
  }
  return 0;
}

// OFFSET: 0x0081D5F0
// FUNCTION: fn_10222
int UIRuneSetReWard::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message == 0x22) {
    if (static_cast<unsigned int>(argument1) <= 2 && argument2) {
      this->m_texts[argument1] = reinterpret_cast<const char*>(argument2);
    }
    return 0;
  }

  if (message == 0x66) {
    this->m_itemId = argument2;
    this->SetDisabled(1);
    if (argument2 == 0) {
      this->m_state = 0;
      this->Invalidate();
      return 0;
    }

    this->m_state = argument1;
    if (argument1 == 2) {
      this->SetDisabled(0);
    }
    this->Invalidate();
    return 0;
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0081DA20
// FUNCTION: fn_10224
int UIRuneSystemTab::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  switch (message) {
  case 0x4E:
    this->m_label = reinterpret_cast<const char*>(argument1);
    this->Invalidate();
    return 0;
  case 0x16:
    this->m_count = argument1;
    this->Invalidate();
    return 0;
  case 0x0D:
    this->geometry.unknown_0030 = argument1;
    this->Invalidate();
    this->parent->SendMsg(this, 0x16, this->geometry.unknown_0030, 0, 0, 0);
    break;
  }

  return 0;
}

// OFFSET: 0x0081E520
// FUNCTION: fn_10227
int UIRuneSystem_tagListWnd::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t, std::intptr_t) {
  if (message == 0xA9) {
    this->Move(0, (argument1 == 0 ? argument2 : 0) + 44);
    return 0;
  }

  if (message == 6) {
    if (argument1 == 0x13C) {
      --this->m_firstTag;
      this->m_firstTag = this->m_firstTag < 0 ? 0 : this->m_firstTag;
      this->Invalidate();
    } else if (argument1 == 0x13D) {
      const int first = ++this->m_firstTag;
      if (g_runeSystemMgr->GetTagCount() < first + 10) {
        this->m_firstTag = first - 1;
      }
      this->Invalidate();
      return 0;
    }
  }
  return 0;
}

// OFFSET: 0x0081E970
// FUNCTION: fn_10230
void UIRune_Decom::SelectDecomType(unsigned int type) {
  if (this->m_decomType > 1) {
    return;
  }

  if (this->m_decomType != type) {
    this->m_decomType = type;
    for (int index = 0; index < 2; ++index) {
      this->m_radioButtons[index]->SendMsg(nullptr, 0x0D, 1, 0, 0, 0);
    }
    this->m_radioButtons[this->m_decomType]->SendMsg(nullptr, 0x0D, 0, 0, 0, 0);
    this->m_decomList = g_runeSystemMgr->GetDecomList(this->m_itemId, this->m_decomType);
    this->Invalidate();
  }

  this->m_decomButton->SetDisabled(0);
  this->m_decomButton->Invalidate();
  if (this->m_decomType == 1 && this->m_ownedCount < static_cast<unsigned int>(g_runeSystemMgr->GetDecomItem(1))) {
    this->m_decomButton->SetDisabled(1);
    this->m_decomButton->Invalidate();
  }
}

// OFFSET: 0x0081F230
// FUNCTION: fn_10233
void UIRuneSetList::Select(int index) {
  const int previous = this->position;
  const int next = index > this->maximum ? this->maximum : index;
  this->position = next;
  if (previous != next) {
    this->Invalidate();
  }
  this->m_selected = index;
  this->Invalidate();
}

// OFFSET: 0x00816470
// FUNCTION: fn_10163
void UIRuneSetinfoList::AddLine(std::string text, std::uint32_t color, bool bold) {
  const float centered =
    static_cast<float>(this->geometry.width - this->TextWidth(this->m_context->fonts, this->m_context->locale, text.c_str(), text.size(), 0, 12, bold, 0)) *
    0.5f;
  const int x = static_cast<int>(std::max(0.0f, centered));
  RuneSetInfoLine line(text, color, bold, x);
  this->m_lines.push_back(line);
}

// OFFSET: 0x00816A40
// FUNCTION: fn_10167
void UIRuneSetinfoList::ClearLines() {
  this->m_lines.clear();
}

// OFFSET: 0x0081A100
// FUNCTION: fn_10183
void UIRuneSetinfoList::Paint() {
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\bg_runeset_desc.bmp")))->image, true);
  for (int row = 0; row < this->visible; ++row) {
    const int index = this->position + row;
    if (index >= 0 && static_cast<unsigned int>(index) < this->m_lines.size()) {
      const RuneSetInfoLine& line = this->m_lines[index];
      this->DrawTextAt(
        this->m_context->fonts, this->m_context->locale, line.x, this->m_itemHeight * row + 30, line.text.c_str(), 0, 0, 12, line.color, line.bold, 0
      );
    }
  }
  this->DrawScrollBar(false);
}

// OFFSET: 0x0081EA50
// FUNCTION: fn_10231
void UIRuneSet_Info::RefreshDescription() {
  this->m_infoList->ClearLines();
  if (!this->m_runeSet) {
    return;
  }

  const int activeCount = this->m_runeSet->CountActiveSlots();
  for (int count = 2; count < 7; ++count) {
    const std::vector<std::string>& lines = this->m_runeSet->m_descriptions[count - 1];
    const int lineCount = static_cast<int>(lines.size());
    if (lineCount == 0) {
      continue;
    }

    const bool reached = activeCount >= count;
    this->m_infoList->AddLine(FormatOwnedString(LookupMessage(4049), count).c_str(), reached ? 0xFF7B00 : 0xB0A29A, true);
    this->m_infoList->AddLine(std::string(), 0, false);
    for (int index = 0; index < lineCount; ++index) {
      this->m_infoList->AddLine(lines[index].c_str(), reached ? 0 : 0xB0A29A, false);
    }
    this->m_infoList->AddLine(std::string(), 0, false);
  }

  UIRuneSetinfoList* list = this->m_infoList;
  const int visible = (list->geometry.height - 60) / 30;
  const int total = static_cast<int>(list->m_lines.size());
  list->SetRange(std::max(total - visible, 0), visible, total);
  list->SetPosition(0);
}

// OFFSET: 0x0081CF60
// FUNCTION: fn_10218
void UIRuneSet_Info::UpdateRewards() {
  for (UIRuneSetReWard* reward : this->m_rewards) {
    reward->SendMsg(this, 0x66, 0, 0, 0, 0);
  }

  if (!this->m_runeSet || this->m_runeSet->m_reward.values[0] == 0) {
    return;
  }

  for (int index = 0; index < 7; ++index) {
    RuneSetData* runeSet = this->m_runeSet;
    const std::int64_t itemId = runeSet->m_reward.values[index + 1];
    if (itemId == 0) {
      continue;
    }

    if (index < runeSet->m_rewardStep) {
      this->m_rewards[index]->SendMsg(this, 0x66, 3, static_cast<std::intptr_t>(itemId), 0, 0);
    } else if (index == 0) {
      this->m_rewards[0]->SendMsg(this, 0x66, runeSet->IsActive() ? 2 : 1, static_cast<std::intptr_t>(itemId), 0, 0);
    } else {
      int state = 2;
      if ((index != 6 && runeSet->CountActiveSlots() <= index) || runeSet->FindLastReward(index + 1) > runeSet->m_rewardStep) {
        state = 1;
      }
      this->m_rewards[index]->SendMsg(this, 0x66, state, static_cast<std::intptr_t>(runeSet->m_reward.values[index + 1]), 0, 0);
    }
  }
}

// OFFSET: 0x0081D0B0
// FUNCTION: fn_10219
void UIRuneSet_Info::Refresh() {
  this->m_activeWnd->Show(0);
  this->unknown_0084->Enable(0);
  this->unknown_0088->Enable(0);
  this->unknown_008C->Enable(0);
  this->m_tab->Show(0);
  this->m_gauge->Show(0);
  this->m_gradeText->Show(0);
  this->m_gradeItems->Show(0);
  this->m_gradeButton->Enable(0);
  this->m_infoList->Show(0);
  for (UIRuneSetReWard* reward : this->m_rewards) {
    reward->Enable(0);
  }
  this->UpdateRewards();

  if (this->m_runeSet) {
    if (this->m_runeSet->IsActive()) {
      if (this->m_runeSet == g_runeSystemMgr->GetCurRuneSet()) {
        this->unknown_008C->Enable(1);
      } else {
        this->unknown_0088->Enable(1);
      }
    } else {
      this->m_activeWnd->Show(1);
      this->m_activeWnd->Invalidate();
      this->unknown_0084->Enable(1);
      UIBitmapButton* button = this->unknown_0084;
      button->SetDisabled(!g_runeSystemMgr->HasRequiredItems(this->m_runeSet->GetActiveType()));
      button->Invalidate();
      this->m_tab->geometry.unknown_0030 = 0;
      this->m_tab->Invalidate();
      this->m_tab->Invalidate();
    }

    this->m_tab->Show(1);
    const std::uint32_t tab = this->m_tab->geometry.unknown_0030;
    if (tab == 1) {
      if (this->m_runeSet->m_reward.values[0] == 1) {
        for (int index = 0; index < this->m_runeSet->CountSlots(); ++index) {
          this->m_rewards[index]->Enable(1);
        }
        this->m_rewards[6]->Enable(1);
      }
    } else if (tab == 2) {
      if (this->m_runeSet->m_grade < 15) {
        this->m_gradeText->Show(1);
        this->m_gauge->m_max = false;
      } else {
        this->m_gradeText->Show(0);
        this->m_gauge->m_max = true;
      }
      this->m_gauge->m_value = g_runeSystemMgr->GetGradeValue(this->m_runeSet);
      this->m_gauge->Show(1);
      this->m_gauge->Invalidate();
      this->m_gradeText->Invalidate();

      if (this->m_runeSet->m_grade < 15) {
        this->m_gradeItems->Show(1);
        UIRuneSystem_ActiveWnd* items = this->m_gradeItems;
        items->m_listId = this->m_runeSet->GetGradeValue();
        items->Invalidate();
        this->m_gradeItems->Invalidate();

        this->m_gradeButton->Enable(1);
        this->m_gradeButton->SetDisabled(0);
        this->m_gradeButton->Invalidate();
        if (g_runeSystemMgr->GetCurRuneSet() && g_runeSystemMgr->GetCurRuneSet()->IsSame(*this->m_runeSet)) {
          this->m_gradeButton->SetDisabled(1);
          this->m_gradeButton->Invalidate();
        }
        UIBitmapButton* button = this->m_gradeButton;
        button->SetDisabled(!g_runeSystemMgr->HasRequiredItems(this->m_runeSet->GetGradeValue()));
        button->Invalidate();
      }
    } else {
      this->m_infoList->Show(1);
      this->m_infoList->Invalidate();
    }
  }

  this->Invalidate();
}

// OFFSET: 0x0081F280
// FUNCTION: fn_10234
void UIRuneSystem_ActiveWnd::DrawCount(int x, int y, int count, int total, std::uint32_t color, int fontHeight) {
  const std::string text = FormatOwnedString("%d / %d", count, total);
  const int width = this->TextWidth(this->m_context->fonts, this->m_context->locale, text.c_str(), text.size(), 0, fontHeight, 0, 0);
  this->DrawTextAt(
    this->m_context->fonts, this->m_context->locale, static_cast<int>(static_cast<float>(x) - static_cast<float>(width) * 0.5f), y, text.c_str(), 0, 0,
    fontHeight, color, 0, 0
  );
}

// OFFSET: 0x0081F360
// FUNCTION: fn_10235
void UIRuneSetList::DrawRuneIcon(int x, int y, const char* name, bool active) {
  this->DrawBitmap(
    x, y, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\collection_stroke_small.bmp")))->image, false
  );
  const std::string icon = FormatOwnedString("유저인터페이스\\runesystem\\rune_icon\\%s.bmp", name);
  CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(icon.c_str())));
  this->DrawBitmap(x + 1, y + 1, &bitmap->image, false);
  if (!active) {
    this->geometry.surface->Dim(x + 1, y + 1, bitmap->image.width, bitmap->image.height, 55);
  }
}

// OFFSET: 0x008166B0
// FUNCTION: fn_10165
void UIRuneSystemWnd::SelectTab() {
  for (int index = 0; index < 3; ++index) {
    UIRuneSetList* list = this->m_lists[index];
    if (list) {
      list->Show(this->m_tab->geometry.unknown_0030 == static_cast<std::uint32_t>(index));
    }
  }
  this->m_equipRuneSet->Show(this->m_tab->geometry.unknown_0030 == 0);
  this->m_tagList->SendMsg(this, 0xA9, this->m_tab->geometry.unknown_0030, this->m_equipRuneSet->geometry.height, 0, 0);
  this->m_tagList->Show(1);
  this->m_tagList->Invalidate();

  switch (this->m_tab->geometry.unknown_0030) {
  case 0:
    this->m_runeSetInfo->Show(1);
    this->m_runeSetInfo->RefreshDescription();
    this->m_runeInfo->Show(0);
    this->m_runeDecom->Show(0);
    break;
  case 1:
    this->m_runeSetInfo->Show(0);
    this->m_runeInfo->Show(1);
    this->m_runeDecom->Show(0);
    break;
  case 2: {
    this->m_runeSetInfo->Show(0);
    this->m_runeInfo->Show(0);
    this->m_runeDecom->Show(1);
    this->m_tagList->Show(0);
    const int count = g_runeSystemMgr->UpdateOwnedDecomItems();
    this->m_lists[2]->SetRange(std::max(count - 10, 0), 10, count);
    break;
  }
  }

  this->SendMsg(this, 0x3C, 0, 0, 0, 0);
}

// OFFSET: 0x0081E7F0
// FUNCTION: fn_10229
int UIRune_Info::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    if (argument1 == 0x23C && (!this->parent || this->parent->SendMsg(this, 0x6A, 3000, 0, 0, 0) != 0) && this->m_rune) {
      g_runeSystemMgr->SendRequest(0xBCE, this->m_rune, -1);
    }
    [[fallthrough]];
  case 0x3C:
    this->m_activeWnd->Invalidate();
    this->m_activateButton->Invalidate();
    reinterpret_cast<UIRuneSet_Info*>(this)->UpdateActivation();
    break;
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0081CC40
// FUNCTION: fn_10214
void UIRuneSetReWard::Slot32(int, int) {
  this->Invalidate();
  ITEM_INFO item;
  item.SetItemId(this->m_itemId);
  item.identified = 1;
  UIWindow* window = g_windowMgr.EnsureWindow(12);
  if (window) {
    window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
  }
}

// OFFSET: 0x0081CD10
// FUNCTION: fn_10215
void UIRuneSystem_ActiveWnd::Slot32(int x, int y) {
  if (this->m_listId == -1) {
    return;
  }

  const int index = this->HitTestItem(x, y);
  if (index <= -1) {
    return;
  }

  const int itemId = g_runeSystemMgr->GetItemListEntry(this->m_listId, index).first;
  if (itemId) {
    ITEM_INFO item;
    item.SetItemId(itemId);
    item.identified = 1;
    UIWindow* window = g_windowMgr.EnsureWindow(12);
    if (window) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x0081CE10
// FUNCTION: fn_10216
void UIRune_Decom::Slot32(int x, int y) {
  if (!this->m_decomList) {
    return;
  }

  const int index = this->HitTestItem(x, y);
  if (index != -1) {
    ITEM_INFO item;
    item.SetItemId((*this->m_decomList)[index].m_itemId);
    item.identified = 1;
    UIWindow* window = g_windowMgr.EnsureWindow(12);
    if (window) {
      window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
    }
  }
}

// OFFSET: 0x0081AC30
// FUNCTION: fn_10189
void UIRuneSystem_Decom_RadioBtn::Paint() {
  UIButtonContext& controls = *this->m_controls;
  this->ClearSurface(0xFFFF00FF);
  if (this->checkedBitmap.empty() || this->uncheckedBitmap.empty() || this->m_disabledBitmap.empty()) {
    return;
  }

  std::uint32_t color = this->textColor;
  CBitmapRes* bitmap;
  if (this->enabled == 0) {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_disabledBitmap.c_str())));
    color = 0xECD5C4;
  } else {
    bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve((this->geometry.unknown_0030 ? this->uncheckedBitmap : this->checkedBitmap).c_str())));
  }

  if (bitmap) {
    this->DrawBitmap(0, static_cast<int>((this->geometry.height - bitmap->image.height) * 0.5f), &bitmap->image, true);
  }
  if (!this->text.empty()) {
    const int textHeight = this->TextHeight(controls.fonts, controls.locale, this->text.c_str(), 0, 0, this->fontHeight, 0, 0);
    this->DrawTextAt(
      controls.fonts, controls.locale, bitmap->image.width + 3, static_cast<int>((this->geometry.height - textHeight) * 0.5f), this->text.c_str(), 0, 0,
      this->fontHeight, color, 0, 0
    );
  }
}

// OFFSET: 0x0081A280
// FUNCTION: fn_10185
void UIRuneSystemWnd::Paint() {
  this->DrawBitmap(
    0, 16, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\runesystem\\bg_left_title.bmp").c_str())))->image,
    false
  );
  this->DrawBitmap(
    0, this->geometry.height - 36,
    &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\runesystem\\bg_left_down.bmp").c_str())))->image, false
  );
  this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, true, LookupMessage(4050), 0);
}

// OFFSET: 0x0081E3F0
// FUNCTION: fn_10226
int UIRuneSystem_DecomResultWnd::SendMsg(UIWindow*, int message, std::intptr_t argument1, std::intptr_t, std::intptr_t, std::intptr_t) {
  if (message == 6) {
    if (argument1 == 0xB8) {
      g_windowMgr.CloseWindow(0x16A);
    }
  } else if (message == 0x0D) {
    const RuneDecomResultPacket* packet = reinterpret_cast<const RuneDecomResultPacket*>(argument1);
    this->m_total = 0;
    for (int index = 0; index < 8; ++index) {
      this->m_itemIds[index] = packet->itemIds[index];
      this->m_counts[index] = packet->counts[index];
      this->m_total += this->m_counts[index];
    }
    this->Invalidate();
  }
  return 0;
}

// OFFSET: 0x00818880
// FUNCTION: fn_10176
void UIDecomlist::Paint() {
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\bg_decom_list.bmp")))->image, true);
  if (g_runeSystemMgr->GetOwnedDecomItemCount() == 0) {
    this->DrawBitmap(56, 175, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\decom_empty.bmp")))->image, true);
    return;
  }

  for (int row = 0; row < this->visible; ++row) {
    const int index = this->position + row;
    const int itemId = g_runeSystemMgr->GetOwnedDecomItem(index);
    if (!itemId) {
      continue;
    }

    const int y = this->m_itemHeight * row + 4;
    const std::string background = FormatOwnedString("유저인터페이스\\runesystem\\list_decom_item_1_%d.bmp", index == this->m_selected);
    this->DrawBitmap(0, y, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background.c_str())))->image, true);

    ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, itemId, 0);
    char text[128];
    std::memset(text, 0, sizeof(text));
    ItemDescriptions descriptions{g_itemDescriptions};
    descriptions.IconPath(text, item.itemId.c_str(), 1);
    CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
    if (!icon) {
      icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\item\\사과.bmp")));
    }
    this->DrawBitmap(19, y + 5, &icon->image, true);

    std::memset(text, 0, sizeof(text));
    int capacity = sizeof(text);
    descriptions.Name(item, text, capacity, 0);
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 57, y + 8, text, 0, 0, 13, 0, 0, 0);
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 58, y + 28, FormatOwnedString("%d", item.quantity).c_str(), 0, 0, 13, 0, 0, 0);
  }
  this->DrawScrollBar(false);
}

// OFFSET: 0x00818B90
// FUNCTION: fn_10177
void UIEquipRuneSetWnd::Paint() {
  RuneSetData* runeSet = g_runeSystemMgr->GetCurRuneSet();
  ResourceMgr& resources = ResourceMgr::Instance();
  if (!runeSet) {
    this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(resources.Load(Resolve("유저인터페이스\\runesystem\\bg_runeequip_empty.bmp")))->image, false);
    return;
  }

  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(resources.Load(Resolve("유저인터페이스\\runesystem\\bg_runeequip.bmp")))->image, false);
  int x = 25;
  for (unsigned int index = 0; index < runeSet->m_slots.size(); ++index, x += 40) {
    RuneData* rune = runeSet->m_slots[index];
    if (!rune) {
      continue;
    }

    const std::string name = FormatOwnedString("유저인터페이스\\runesystem\\rune_icon\\%s.bmp", rune->GetResName());
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name.c_str())));
    if (bitmap) {
      this->DrawBitmap(x, 35, &bitmap->image, false);
      if (!runeSet->m_slots[index]->IsActive()) {
        this->geometry.surface->Dim(x, 35, bitmap->image.width, bitmap->image.height, 55);
      }
    }
  }

  int titleX = 37;
  if (runeSet->m_grade) {
    const std::string grade = FormatOwnedString("+%d", runeSet->m_grade);
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 37, 12, grade.c_str(), 0, 0, 13, 0xFCDDE5, 1, 0);
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 37, 11, grade.c_str(), 0, 0, 13, 0xFF7E00, 1, 0);
    titleX = runeSet->m_grade > 9 ? 62 : 55;
  }

  titleX += 2;
  const std::string title = FormatOwnedString(LookupMessage(4042), runeSet->GetName());
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, titleX, 12, title.c_str(), 0, 0, 13, 0xFCDEE6, 1, 0);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, titleX, 11, title.c_str(), 0, 0, 13, 0x864D43, 1, 0);
  const std::string tag = FormatOwnedString("유저인터페이스\\runesystem\\tag\\bt_tag_%d_equip.bmp", runeSet->GetTagId());
  this->DrawBitmap(9, 6, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(tag.c_str())))->image, false);
}

// OFFSET: 0x00818EB0
// FUNCTION: fn_10178
void UIGage_Circle::Paint() {
  if (this->m_max) {
    this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_maxBitmap.c_str())))->image, false);
    return;
  }

  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_fullBitmap.c_str())))->image, false);
  CBitmapRes* empty = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->m_emptyBitmap.c_str())));
  this->DrawBitmapRegion(
    0, 0, &empty->image, true, false, 0, 0, empty->image.width,
    static_cast<int>((1.0f - static_cast<float>(this->m_value) * 0.00001f) * static_cast<float>(empty->image.height))
  );

  const std::string text = FormatOwnedString("%.2f%%", static_cast<double>(static_cast<float>(this->m_value) * 0.001f));
  const int width = this->TextWidth(this->m_context->fonts, this->m_context->locale, text.c_str(), text.size(), 0, 12, 0, 0);
  this->DrawTextAt(
    this->m_context->fonts, this->m_context->locale, static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f), this->m_textY, text.c_str(), 0,
    0, 12, 0xFFFFFF, 1, 0
  );
}

// OFFSET: 0x00819090
// FUNCTION: fn_10179
void UIRuneList::Paint() {
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\bg_rune_list.bmp")))->image, true);
  for (int row = 0; row < 9; ++row) {
    const int index = this->position + row;
    RuneData* rune = g_runeSystemMgr->GetCurrentRune(1, index);
    if (!rune) {
      continue;
    }

    const int y = this->m_itemHeight * row + 1;
    const bool active = rune->IsActive();
    const std::string background = FormatOwnedString("유저인터페이스\\runesystem\\list_rune_item_%d_%d.bmp", active, index == this->m_selected);
    this->DrawBitmap(0, y, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background.c_str())))->image, true);
    this->DrawRuneIcon(18, y + 6, rune->GetResName(), active);
    const std::string name = FormatOwnedString(LookupMessage(4043), rune->GetName());
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, 62, y + 9, name.c_str(), 0, 0, 12, 0, 0, 0);
  }
  this->DrawScrollBar(true);
}

// OFFSET: 0x00819240
// FUNCTION: fn_10180
void UIRuneSetList::Paint() {
  this->ClearSurface(0xFF00FF);
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\bg_runeset_list.bmp")))->image, true);
  if (!g_runeSystemMgr->GetTag(-1)->m_locked) {
    this->DrawBitmap(57, 100, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\loding.bmp")))->image, false);
    return;
  }

  for (int row = 0; row < 5; ++row) {
    const int index = this->position + row;
    RuneSetData* runeSet = dynamic_cast<RuneSetData*>(g_runeSystemMgr->GetCurrentRune(0, index));
    if (!runeSet) {
      continue;
    }

    const int y = this->m_itemHeight * row + 1;
    const std::string background = FormatOwnedString("유저인터페이스\\runesystem\\list_runeset_item_%d_%d.bmp", runeSet->IsActive(), index == this->m_selected);
    this->DrawBitmap(0, y, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background.c_str())))->image, true);
    for (unsigned int slot = 0; slot < runeSet->m_slots.size(); ++slot) {
      if (runeSet->m_slots[slot]) {
        this->DrawRuneIcon(this->unknown_00B8 * slot + 20, y + 25, runeSet->m_slots[slot]->GetResName(), runeSet->m_slots[slot]->IsActive());
      }
    }

    const std::string title = FormatOwnedString(LookupMessage(4042), runeSet->GetName());
    const int titleWidth = this->TextWidth(this->m_context->fonts, this->m_context->locale, title.c_str(), title.size(), 0, 12, 0, 0);
    std::string grade;
    int gradeWidth = 0;
    if (runeSet->m_grade) {
      grade = FormatOwnedString("+%d", runeSet->m_grade);
      gradeWidth = this->TextWidth(this->m_context->fonts, this->m_context->locale, grade.c_str(), grade.size(), 0, 12, 0, 0);
    }

    int x = static_cast<int>(static_cast<float>(this->geometry.width - gradeWidth - titleWidth) * 0.5f);
    if (runeSet->m_grade) {
      this->DrawTextAt(this->m_context->fonts, this->m_context->locale, x, y + 7, grade.c_str(), 0, 0, 12, 0xFF7E00, 0, 0);
      x += gradeWidth + 5;
    }
    this->DrawTextAt(this->m_context->fonts, this->m_context->locale, x, y + 7, title.c_str(), 0, 0, 12, 0, 0, 0);
  }
  this->DrawScrollBar(true);
}

// OFFSET: 0x0081C510
// FUNCTION: fn_10207
void UIRuneSetReWard::Slot28(int, int) {
  if (!this->m_itemId) {
    return;
  }

  ITEM_INFO item;
  item.SetItemId(this->m_itemId);
  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  if (std::string("Unknown Item") == naming.DisplayName(item, 0)) {
    return;
  }

  item.identified = 1;
  int x = 0;
  int y = 0;
  this->ToScreen(x, y);
  ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
  if (tooltip) {
    tooltip->Show(naming.DisplayName(item, 0).c_str(), x, y - 10, 0xFFFFFF, 0, 1);
  }
}

// OFFSET: 0x0081C7F0
// FUNCTION: fn_10209
void UIRuneSystem_ActiveWnd::Slot28(int x, int y) {
  if (this->m_listId == -1) {
    return;
  }

  const int index = this->HitTestItem(x, y);
  if (index <= -1) {
    return;
  }

  const int itemId = g_runeSystemMgr->GetItemListEntry(this->m_listId, index).first;
  if (!itemId) {
    return;
  }

  ITEM_INFO item;
  item.SetItemId(itemId);
  item.identified = 1;
  int tipX = index % 4 * 54 + 14;
  int tipY = index > 3 ? 44 : -10;
  this->ToScreen(tipX, tipY);
  ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
  if (tooltip) {
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    tooltip->Show(naming.DisplayName(item, 0).c_str(), tipX, tipY, 0xFFFFFF, 0, 1);
  }
}

// OFFSET: 0x0081CA80
// FUNCTION: fn_10211
void UIRune_Decom::Slot28(int x, int y) {
  if (!this->m_decomList) {
    return;
  }

  const int index = this->HitTestItem(x, y);
  if (index == -1) {
    return;
  }

  ITEM_INFO item;
  item.SetItemId((*this->m_decomList)[index].m_itemId);
  item.identified = 1;
  int tipX = index % 4 * 54 + 211;
  int tipY = index > 3 ? 387 : 333;
  this->ToScreen(tipX, tipY);
  ModeTooltip* tooltip = reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode);
  if (tooltip) {
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    tooltip->Show(naming.DisplayName(item, 0).c_str(), tipX, tipY, 0xFFFFFF, 0, 1);
  }
}

// OFFSET: 0x0081A390
// FUNCTION: fn_10186
void UIRuneSystem_ActiveWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->m_listId == -1) {
    return;
  }

  CBitmapRes* blue = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\iconslot_blue.bmp")));
  CBitmapRes* red = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\iconslot_red.bmp")));
  int y = 11;
  for (int row = 0; row < this->m_rows; ++row, y += 54) {
    int x = 18;
    for (int column = 0; column < this->m_columns; ++column, x += 54) {
      this->DrawBitmap(x, y, &blue->image, true);
      const std::pair<int, int>& entry = g_runeSystemMgr->GetItemListEntry(this->m_listId, this->m_columns * row + column);
      const int itemId = entry.first;
      const int required = entry.second;
      if (!itemId) {
        continue;
      }

      ITEM_INFO item;
      item.SetItemId(itemId);
      item.identified = 1;
      char path[128];
      std::memset(path, 0, sizeof(path));
      ItemDescriptions descriptions{g_itemDescriptions};
      descriptions.IconPath(path, item.itemId.c_str(), 1);
      CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
      if (!icon) {
        icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\item\\사과.bmp")));
      }

      ITEM_INFO owned = LookupClientItem(g_session.m_inventoryItems, itemId, 0);
      const bool lacking = required > owned.quantity;
      owned.identified = 1;
      this->DrawBitmap(x, y, &(lacking ? red : blue)->image, true);
      if (icon) {
        this->DrawBitmap(x + 8, y + 4, &icon->image, true);
      }
      this->DrawCount(x + 18, y + 39, owned.quantity > required ? required : owned.quantity, required, lacking ? 0xFF : 0x875F51, 10);
    }
  }
}

// OFFSET: 0x0081A6D0
// FUNCTION: fn_10187
void UIRuneSystem_DecomItemWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->m_listId == -1) {
    return;
  }

  int y = 51;
  for (int row = 0; row < this->m_rows; ++row, y += 69) {
    int x = 35;
    for (int column = 0; column < this->m_columns; ++column, x += 52) {
      const std::pair<int, int>& entry = g_runeSystemMgr->GetItemListEntry(this->m_listId, this->m_columns * row + column);
      const int itemId = entry.first;
      const int required = entry.second;
      if (!itemId) {
        break;
      }

      ITEM_INFO item;
      item.SetItemId(itemId);
      item.identified = 1;
      char path[128];
      std::memset(path, 0, sizeof(path));
      ItemDescriptions descriptions{g_itemDescriptions};
      descriptions.IconPath(path, item.itemId.c_str(), 1);
      CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
      if (!icon) {
        icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\item\\사과.bmp")));
      }

      ITEM_INFO owned = LookupClientItem(g_session.m_inventoryItems, itemId, 0);
      const bool lacking = required > owned.quantity;
      owned.identified = 1;
      if (icon) {
        this->DrawBitmap(x - 9, y - 36, &icon->image, true);
      }
      this->DrawCount(x, y, required, owned.quantity, lacking ? 0xB4B4B4 : 0, 10);
    }
  }
}

// OFFSET: 0x0081A980
// FUNCTION: fn_10188
void UIRuneSystem_DecomResultWnd::Paint() {
  ResourceMgr& resources = ResourceMgr::Instance();
  this->DrawBitmap(
    0, 0,
    &static_cast<CBitmapRes*>(resources.Load(Resolve(FormatOwnedString("유저인터페이스\\runesystem\\bg_decomResult_%d.bmp", this->m_total > 0).c_str())))->image,
    false
  );
  if (this->m_total == 0) {
    return;
  }

  int index = 0;
  for (int y = 77; y < 185; y += 54) {
    for (int x = 19; x < 235; x += 54, ++index) {
      if (!this->m_itemIds[index]) {
        continue;
      }

      ITEM_INFO item;
      item.SetItemId(this->m_itemIds[index]);
      item.identified = 1;
      char path[128];
      std::memset(path, 0, sizeof(path));
      ItemDescriptions descriptions{g_itemDescriptions};
      descriptions.IconPath(path, item.itemId.c_str(), 1);
      CBitmapRes* icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
      if (!icon) {
        icon = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\item\\사과.bmp")));
      }
      if (icon) {
        this->DrawBitmap(x + 8, y - 35, &icon->image, true);
      }

      const std::string count = FormatOwnedString("%d", this->m_counts[index]);
      const int width = this->TextWidth(this->m_context->fonts, this->m_context->locale, count.c_str(), count.size(), 0, 12, 0, 0);
      this->DrawTextAt(this->m_context->fonts, this->m_context->locale, x - static_cast<int>(width * 0.5f) + 19, y, count.c_str(), 0, 0, 12, 0, 0, 0);
    }
  }
}

// OFFSET: 0x0081AD80
// FUNCTION: fn_10190
void UIRuneSystem_tagListWnd::Paint() {
  const std::string background = FormatOwnedString("유저인터페이스\\runesystem\\bg_tag%d.bmp", (this->geometry.y == 44) + 1);
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background.c_str())))->image, true);
  CBitmapRes* empty = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\tag\\bt_tag_bg.bmp")));
  int x = 22;
  for (int index = 0; index < this->m_visibleCount; ++index, x += 27) {
    CTagData* tag = g_runeSystemMgr->GetTagByIndex(this->m_firstTag + index);
    if (!tag) {
      this->DrawBitmap(x, 4, &empty->image, true);
      continue;
    }

    const std::string name = FormatOwnedString("유저인터페이스\\runesystem\\tag\\bt_tag_%d%s", tag->m_id, g_runeSystemMgr->GetTag(-1) == tag ? "_1.bmp" : "_0.bmp");
    this->DrawBitmap(x, 4, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name.c_str())))->image, true);
  }
}

// OFFSET: 0x0081B7C0
// FUNCTION: fn_10192
void UIRune_Info::Paint() {
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\bg_main.bmp")))->image, true);
  if (!this->m_rune) {
    return;
  }

  ResourceMgr& resources = ResourceMgr::Instance();
  CBitmapRes* bitmap =
    static_cast<CBitmapRes*>(resources.Load(Resolve(FormatOwnedString("유저인터페이스\\runesystem\\rune\\%s.bmp", this->m_rune->GetResName()).c_str())));
  if (this->m_rune->IsActive()) {
    this->DrawBitmap(225, 134, &bitmap->image, true);
    this->DrawBitmap(214, 123, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\card_stone_frame.bmp")))->image, true);
  } else {
    this->DrawBitmap(221, 80, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\rune_edge_big.bmp")))->image, true);
    this->DrawBitmap(225, 84, &bitmap->image, true);
    this->geometry.surface->Dim(225, 84, bitmap->image.width, bitmap->image.height, 55);
  }

  const std::string name = FormatOwnedString("Rune of %s", this->m_rune->GetName());
  const int width = this->TextWidth(this->m_context->fonts, this->m_context->locale, name.c_str(), name.size(), 0, 13, 1, 0);
  const int x = static_cast<int>(static_cast<float>(this->geometry.width - width) * 0.5f + 2.0f);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, x, 25, name.c_str(), 0, 0, 13, 0xFFF0E1, 1, 0);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, x - 1, 26, name.c_str(), 0, 0, 13, 0x864D43, 1, 0);
}

// OFFSET: 0x0081BA00
// FUNCTION: fn_10193
void UIRune_ViewInfo::Paint() {
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\runesystem\\View_bg_main.bmp")))->image, true);
  const int total = g_runeSystemMgr->GetTag(-1)->GetRuneCount(1);
  const int active = g_runeSystemMgr->GetTag(-1)->m_activeCount[1];
  const int center = static_cast<int>(static_cast<float>(this->m_gauge->geometry.width) * 0.5f + static_cast<float>(this->m_gauge->geometry.x));

  const std::string activeText = FormatOwnedString("%d", active);
  const int activeWidth = this->TextWidth(this->m_context->fonts, this->m_context->locale, activeText.c_str(), activeText.size(), 0, 12, 0, 0);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, center - activeWidth - 7, 406, activeText.c_str(), 0, 0, 12, 0xFF7E00, 0, 0);
  const std::string totalText = FormatOwnedString("/ %d", active, total);
  this->DrawTextAt(this->m_context->fonts, this->m_context->locale, center - 3, 406, totalText.c_str(), 0, 0, 12, 0, 0, 0);

  const std::string title = "수집 현황";
  const int titleWidth = this->TextWidth(this->m_context->fonts, this->m_context->locale, title.c_str(), title.size(), 0, 13, 0, 0);
  this->DrawTextAt(
    this->m_context->fonts, this->m_context->locale, static_cast<int>(261.0f - static_cast<float>(titleWidth) * 0.5f), 27, title.c_str(), 0, 0, 13, 0x864D43,
    1, 0
  );

  const std::string tagName = g_runeSystemMgr->GetTag(-1)->m_name;
  const int tagWidth = this->TextWidth(this->m_context->fonts, this->m_context->locale, tagName.c_str(), tagName.size(), 0, 13, 0, 0);
  this->DrawTextAt(
    this->m_context->fonts, this->m_context->locale, static_cast<int>(261.0f - static_cast<float>(tagWidth) * 0.5f), 74, tagName.c_str(), 0, 0, 13, 0xDB632A,
    1, 0
  );
}

// OFFSET: 0x00814BC0
// FUNCTION: fn_10136
UIRuneSystemTab::UIRuneSystemTab(UIWindowContext& windows) : UIWindow(windows, 0), m_count(4), m_label("유저인터페이스\\runesystem\\runesystam_tab_%d.bmp") {
  this->geometry.unknown_0030 = 0;
}

// OFFSET: 0x00814D40
// FUNCTION: fn_10137
UIRuneSystemWnd::UIRuneSystemWnd(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0), m_nextTick(0) {
  RuneSystemOpenPacket packet;
  packet.id = 0x0BE0;
  packet.open = 1;
  GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x00814EE0
// FUNCTION: fn_10139
UIRuneSystem_DecomResultWnd::UIRuneSystem_DecomResultWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_total(0) {
  UIBitmapButton* button = new UIBitmapButton(windows, controls);
  this->AddChild(button);
  button->SetBitmap("유저인터페이스\\runesystem\\bt_ok_nomal.bmp", 0, 0);
  button->SetBitmap("유저인터페이스\\runesystem\\bt_ok_mouseon.bmp", 1, 0);
  button->SetBitmap("유저인터페이스\\runesystem\\bt_ok_pressed.bmp", 2, 0);
  button->SetBitmap("유저인터페이스\\runesystem\\bt_ok_disabled.bmp", 3, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(static_cast<int>(static_cast<float>(240 - button->geometry.width) * 0.5f), 154);
  button->SetId(0xB8);
  std::memset(this->m_itemIds, 0, sizeof(this->m_itemIds) + sizeof(this->m_counts));
}

// OFFSET: 0x008150A0
// FUNCTION: fn_10141
UIRuneSystem_tagListWnd::UIRuneSystem_tagListWnd(UIWindowContext& windows, UIButtonContext& controls)
    : UIWindow(windows, 0), m_firstTag(0), m_visibleCount(9) {
  this->m_rightButton = new UIBitmapButton(windows, controls);
  this->AddChild(this->m_rightButton);
  this->m_rightButton->SetBitmap("유저인터페이스\\runesystem\\bt_tag_right_nomal.bmp", 0, 0);
  this->m_rightButton->SetBitmap("유저인터페이스\\runesystem\\bt_tag_right_mouseon.bmp", 1, 0);
  this->m_rightButton->SetBitmap("유저인터페이스\\runesystem\\bt_tag_right_pressed.bmp", 2, 0);
  this->m_rightButton->SetBitmap("유저인터페이스\\runesystem\\bt_tag_right_disable.bmp", 3, 0);
  this->m_rightButton->SetSize(this->m_rightButton->BitmapWidth(), this->m_rightButton->BitmapHeight());
  this->m_rightButton->Move(267, 9);
  this->m_rightButton->SetId(0x13D);

  this->m_leftButton = new UIBitmapButton(windows, controls);
  this->AddChild(this->m_leftButton);
  this->m_leftButton->SetBitmap("유저인터페이스\\runesystem\\bt_tag_left_nomal.bmp", 0, 0);
  this->m_leftButton->SetBitmap("유저인터페이스\\runesystem\\bt_tag_left_mouseon.bmp", 1, 0);
  this->m_leftButton->SetBitmap("유저인터페이스\\runesystem\\bt_tag_left_pressed.bmp", 2, 0);
  this->m_leftButton->SetBitmap("유저인터페이스\\runesystem\\bt_tag_left_disable.bmp", 3, 0);
  this->m_leftButton->SetSize(this->m_leftButton->BitmapWidth(), this->m_leftButton->BitmapHeight());
  this->m_leftButton->Move(7, 9);
  this->m_leftButton->SetId(0x13C);

  if (g_runeSystemMgr->GetTagCount() < this->m_visibleCount) {
    this->m_rightButton->SetDisabled(1);
    this->m_rightButton->Invalidate();
    this->m_leftButton->SetDisabled(1);
    this->m_leftButton->Invalidate();
  }
}

// OFFSET: 0x0081D6B0
// FUNCTION: fn_10223
int UIRuneSet_Info::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    if (argument1 > 0x158) {
      switch (argument1) {
      case 0x1FD:
        if (!this->m_runeSet) {
          reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(4048), 0, true, false, 0, "Message", 280, 120, 0);
        } else if (!this->parent || this->parent->SendMsg(this, 0x6A, 100, 0, 0, 0) != 0) {
          g_runeSystemMgr->SendRequest(0xBD2, this->m_runeSet, -1);
        }
        break;
      case 0x23C:
        if (!this->m_runeSet) {
          reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(4048), 0, true, false, 0, "Message", 280, 120, 0);
        } else if (!this->parent || this->parent->SendMsg(this, 0x6A, 100, 0, 0, 0) != 0) {
          g_runeSystemMgr->SendRequest(0xBD0, this->m_runeSet, -1);
        }
        break;
      case 0x24E:
      case 0x24F:
      case 0x250:
      case 0x251:
      case 0x252:
      case 0x253:
      case 0x254:
        if (!this->m_runeSet->IsRewardStep(argument1 - 0x24E)) {
          g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(4199)), 0xFF, 0, 0, 0);
        } else {
          g_runeSystemMgr->SendRequest(0xC16, this->m_runeSet, argument1 - 0x24D);
        }
        break;
      }
    } else if (argument1 == 0x158) {
      if (!this->parent || this->parent->SendMsg(this, 0x6A, 1500, 0, 0, 0) != 0) {
        this->parent->SendMsg(this, 0xAB, 0x158, 0, 0, 0);
      }
    } else if (argument1 == 0xC3) {
      if (this->m_runeSet && (!this->parent || this->parent->SendMsg(this, 0x6A, 1500, 0, 0, 0) != 0)) {
        this->parent->SendMsg(this, 0xAB, 0xC3, reinterpret_cast<std::intptr_t>(this->m_runeSet), 0, 0);
      }
    }
    break;
  case 7:
  case 9:
  case 10:
    this->m_infoList->ScrollBy(argument1);
    this->Invalidate();
    break;
  case 0x16:
    if (!this->m_runeSet) {
      break;
    }
    if (!this->m_runeSet->IsActive()) {
      this->m_tab->geometry.unknown_0030 = 0;
      this->m_tab->Invalidate();
      reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(LookupMessage(4047), 0, true, false, 0, "Message", 280, 120, 0);
    }
    this->Refresh();
    break;
  case 0x3C:
    this->Refresh();
    break;
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x0081ED20
// FUNCTION: fn_10232
void UIRune_Decom::SetItem(int itemId) {
  ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, itemId, 0);
  item.identified = 1;
  if (this->m_itemId != itemId) {
    this->m_itemId = itemId;
    ItemDescriptions descriptions{g_itemDescriptions};
    bool languagePrefix = false;
    WORD language = g_serverLanguage->language;
    ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
    this->unknown_007C = naming.DisplayName(item, 0);

    std::string resource;
    if (IsItemCard(std::atoi(item.itemId.c_str()))) {
      const ItemCardName card = FindItemCardName(std::atoi(item.itemId.c_str()));
      resource = card.name;
    } else {
      resource = ItemResourcePath(item);
    }

    this->unknown_0094 = FormatOwnedString("유저인터페이스\\Illust\\%s.bmp", resource.c_str());
    if (!ResourceMgr::Instance().Exists(this->unknown_0094.c_str())) {
      this->unknown_0094 = FormatOwnedString("유저인터페이스\\cardbmp\\%s.bmp", resource.c_str());
      if (!ResourceMgr::Instance().Exists(this->unknown_0094.c_str())) {
        this->unknown_0094 = "";
      }
    }

    this->m_decomList = g_runeSystemMgr->GetDecomList(this->m_itemId, this->m_decomType);
    this->Invalidate();
  }

  for (int index = 0; index < 2; ++index) {
    this->m_radioButtons[index]->Show(item.quantity != 0);
  }
  if (item.quantity) {
    if (this->m_ownedCount != static_cast<unsigned int>(item.quantity)) {
      this->m_ownedCount = item.quantity;
    }
    this->SelectDecomType(this->m_decomType);
  }
}
