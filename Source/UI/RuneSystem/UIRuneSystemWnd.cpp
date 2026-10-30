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

// OFFSET: 0x0081C6F0

// OFFSET: 0x0081C960

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
