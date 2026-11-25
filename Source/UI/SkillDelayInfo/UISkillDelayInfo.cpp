#include "UI/Window/WindowId.h"
#include "Contents/Skills/SkillId.h"
#include "UI/Window/UIEnums.h"
#include "UI/SkillDelayInfo/SkillDelayInfo.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "Resource/ResourceMgr.h"
#include "Contents/Skin/SkinMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Script/LuaScript.h"
#include "Utility/Text/StringUtil.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include <algorithm>
#include <cstring>
#include <iterator>
#include <map>
#include <string>
#include <vector>
#include "Resource/MSGSTRINGID.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

namespace {

}

struct SkillDelayInfoFreezeCallback {
  virtual ~SkillDelayInfoFreezeCallback() = default;

  /* 0004 */ CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* list;

  void operator()(CUIListItem* const& item);
};

struct SkillDelayInfoSelectCallback {
  virtual ~SkillDelayInfoSelectCallback() = default;

  /* 0004 */ CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* list;

  void operator()(CUIListItem* const& item);
};

// OFFSET: 0x00A71140
// FUNCTION: fn_16799
template <>
CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x00A71300
// FUNCTION: fn_16800
template <>
CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::~CUIListBoxEx() {
}

// OFFSET: 0x00A718D0
// FUNCTION: fn_16805
CUISkillDelayInfo* CreateSkillDelayInfo() {
  return new CUISkillDelayInfo();
}

struct UIRegistration {
  // INLINED
  __forceinline UIRegistration(int id, std::function<CUIFrameWnd*()> factory) {
    Singleton<CUIRegisterMgr>::GetInstance()->Register(id, factory);
  }
};

// OFFSET: 0x0049C2D0
// FUNCTION: fn_45556
static UIRegistration s_skillDelayInfoRegistered(10030, CreateSkillDelayInfo);

// OFFSET: 0x00A71950
// FUNCTION: fn_16806
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::ClearItems() {
  for (SkillDelayInfo::CUIDelayItem* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A719E0
// FUNCTION: fn_16807
void CUISkillDelayInfo::Initialize() {
  this->m_ownsBackgroundColor = false;
  this->m_ownsBorderColor = false;
  this->m_resolveSkin = true;
  this->Resize(345, 379);
  this->Move(0, 0);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
  this->SetSkinColors(0, 0, 0, 0);
}

// OFFSET: 0x00A719B0
// FUNCTION: fn_45783
template <>
__forceinline void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::SetListSize(int width, int height, int itemWidth, int rowHeight) {
  this->itemWidth = itemWidth;
  this->rowHeight = rowHeight;
  this->SetSize(width, height);
}

// OFFSET: 0x00A71B50
// FUNCTION: fn_16808
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(WINDOW_HIDDEN);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

namespace SkillDelayInfo {

// OFFSET: 0x00A71A40
// FUNCTION: fn_45782
__forceinline CUITime* CUIDelayItem::AddTime() {
  CUITime* time = new CUITime();
  time->SetSize(70, 21);
  this->AddChild(time);
  this->times.push_back(time);
  return time;
}

// OFFSET: 0x00A71D60
// FUNCTION: fn_16809
void CUIDelayItem::OnCreate(int width, int height) {
  this->level = new CUIStaticText();
  this->level->SetSize(44, 21);
  this->level->Move(0, 0);
  this->level->SetText("");
  this->level->SetFitText(0);
  this->level->SetAlign(6, 6);
  // GLOBAL: gl_25165
  this->level->SetFont(std::string(OBFUSCATE_ARRAY_TEXT("Gulim", 0xE33F2FF779878559ULL, std::uint8_t)));
  this->level->SetFontHeight(12);
  this->level->SetBold(0);
  this->level->SetColor(0);
  this->level->SetShadow(0, 0xFFFFFF);
  this->level->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->level);

  for (int index = 0; index < 4; ++index) {
    this->AddTime();
  }

  this->times[0]->Move(44, 0);
  this->times[1]->Move(114, 0);
  this->times[2]->Move(184, 0);
  this->times[3]->Move(254, 0);
}

// OFFSET: 0x00A75A00
// FUNCTION: fn_16811
void CUITime::OnCreate(int width, int height) {
  this->text = new CUIStaticText();
  this->text->SetSize(60, 21);
  this->text->Move(5, 0);
  this->text->SetText("");
  this->text->SetFitText(0);
  this->text->SetAlign(6, 6);
  // GLOBAL: gl_25166
  this->text->SetFont(std::string(OBFUSCATE_ARRAY_TEXT("Gulim", 0xD5A78DB38D8DAFABULL, std::uint8_t)));
  this->text->SetFontHeight(12);
  this->text->SetBold(0);
  this->text->SetColor(0);
  this->text->SetShadow(0, 0xFFFFFF);
  this->text->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->text);
}

}

// OFFSET: 0x00A75C80
// FUNCTION: fn_16812
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A75C90
// FUNCTION: fn_16813
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00A75D20
// FUNCTION: fn_16814
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  const auto end = this->items.end();
  for (auto it = this->items.begin(); it != end; ++it) {
    this->RemoveChild(*it, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned int count = static_cast<unsigned int>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (SkillDelayInfo::CUIDelayItem*& slot : this->items) {
      SkillDelayInfo::CUIDelayItem* item = new SkillDelayInfo::CUIDelayItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(WINDOW_HIDDEN);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
      item->SetOnHover([this](CUIListItem* listItem) {
        for (SkillDelayInfo::CUIDelayItem* row : this->items) {
          row->SetHighlighted(listItem == row);
        }
      });
      item->SetOnPress([this](CUIListItem* listItem) {
        if (this->layoutFlag) {
          return;
        }
        for (SkillDelayInfo::CUIDelayItem* row : this->items) {
          if (listItem != row) {
            row->SetSelected(false);
          }
        }

        listItem->SetSelected(true);
        if (listItem->index == static_cast<int>(this->selected) && this->selectionFlag) {
          return;
        }
        SkillDelayInfo::CUIDelayItem* delayItem = dynamic_cast<SkillDelayInfo::CUIDelayItem*>(listItem);
        if (!delayItem) {
          return;
        }
        this->selected = delayItem->index;
        if (this->onSelect && delayItem->index >= 0 && static_cast<unsigned int>(delayItem->index) < this->data.size()) {
          auto node = this->data.begin();
          for (int index = delayItem->index; index > 0; --index) {
            ++node;
          }
          this->onSelect(delayItem, *node);
        }
      });
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A75F80
// FUNCTION: fn_16815
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::LoadSkin(bool resolveSkin) {
  this->normalImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  this->hoverImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  this->pressedImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  this->disabledImage = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (SkillDelayInfo::CUIDelayItem* item : this->items) {
    item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
  }
}

// OFFSET: 0x00A76190
// FUNCTION: fn_16816
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(this->rowHeight * static_cast<int>(this->data.size()) + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

// OFFSET: 0x00A76210
// FUNCTION: fn_16817
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::OnScroll(int position) {
  if (this->items.empty()) {
    return;
  }
  if (this->data.size() == 0) {
    for (SkillDelayInfo::CUIDelayItem* item : this->items) {
      item->index = -1;
      item->Show(WINDOW_HIDDEN);
    }
    return;
  }

  const int first = this->items.front()->index;
  unsigned int row = position / this->rowHeight;
  if (first != -1) {
    if (first < static_cast<int>(row)) {
      if (row - first < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.begin() + (row - first), this->items.end());
      }
    } else if (first > static_cast<int>(row)) {
      const unsigned int distance = first - row;
      if (distance < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.end() - distance, this->items.end());
      }
    }
  }

  auto it = this->items.begin();
  for (int y = this->itemY - position % this->rowHeight; y < this->geometry.height; y += this->rowHeight, ++it, ++row) {
    if (it == this->items.end() || row >= this->data.size()) {
      break;
    }
    SkillDelayInfo::CUIDelayItem* item = *it;
    if (item) {
      if (this->rebind || row != static_cast<unsigned int>(item->index)) {
        item->index = row;
        if (this->binder) {
          auto node = this->data.begin();
          std::advance(node, static_cast<int>(row));
          this->binder(item, *node);
        }
      }
      item->SetSelected(row == this->selected);
      item->Move(this->itemX, y);
      item->Show(WINDOW_VISIBLE);
    }
  }

  for (const auto end = this->items.end(); it != end; ++it) {
    SkillDelayInfo::CUIDelayItem* item = *it;
    item->index = -1;
    item->Show(WINDOW_HIDDEN);
  }
}

// OFFSET: 0x00A768F0
// FUNCTION: fn_16833
void SkillDelayInfoFreezeCallback::operator()(CUIListItem* const& item) {
  CUIListItem* selectedItem = item;
  for (SkillDelayInfo::CUIDelayItem* row : this->list->items) {
    row->SetHighlighted(selectedItem == row);
  }
}

// OFFSET: 0x00A76960
// FUNCTION: fn_16835
void SkillDelayInfoSelectCallback::operator()(CUIListItem* const& item) {
  CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* list = this->list;
  CUIListItem* selectedItem = item;
  if (list->layoutFlag) {
    return;
  }
  for (SkillDelayInfo::CUIDelayItem* row : list->items) {
    if (selectedItem != row) {
      row->SetSelected(false);
    }
  }

  selectedItem->SetSelected(true);
  if (selectedItem->index == static_cast<int>(list->selected) && list->selectionFlag) {
    return;
  }
  SkillDelayInfo::CUIDelayItem* delayItem = dynamic_cast<SkillDelayInfo::CUIDelayItem*>(selectedItem);
  if (!delayItem) {
    return;
  }
  const int selectedIndex = delayItem->index;
  list->selected = selectedIndex;
  if (list->onSelect && selectedIndex >= 0 && static_cast<unsigned int>(selectedIndex) < list->data.size()) {
    auto node = list->data.begin();
    for (int index = selectedIndex; index > 0; --index) {
      ++node;
    }
    list->onSelect(delayItem, *node);
  }
}

// OFFSET: 0x00A77200
// FUNCTION: fn_16872
CUISkillDelayInfo::CUISkillDelayInfo()
    : titleBar(nullptr), skillNameText(nullptr), labels{nullptr, nullptr, nullptr}, columns{nullptr, nullptr, nullptr, nullptr}, delayList(nullptr), description(nullptr), backgroundColor(0), borderColor(0), skillId(7999) {
}

namespace SkillDelayInfo {
// OFFSET: 0x00A77160
// FUNCTION: fn_16871
CUIDelayItem::CUIDelayItem() : CUIListItem(), level(nullptr), times(), backgroundColor(0), borderColor(0) {
}
}

namespace SkillDelayInfo {
// OFFSET: 0x00A77350
// FUNCTION: fn_16873
CUITime::CUITime() : CUIControl(), text(nullptr) {
}
}

namespace SkillDelayInfo {
// OFFSET: 0x00A778C0
// FUNCTION: fn_16877
CUITime::~CUITime() = default;
}

// OFFSET: 0x00A77A70
// FUNCTION: fn_16879
void CUISkillDelayInfo::OnButtonClick() {
}

// OFFSET: 0x00A77A80
// FUNCTION: fn_46631
template <>
void CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>::ClearData() {
  this->data.clear();
  this->selected = static_cast<unsigned int>(-1);
  this->UpdateScrollBar();
}

// OFFSET: 0x00A77B00
// FUNCTION: fn_45472
void CUISkillDelayInfo::OnClose() {
  g_windowMgr.CloseWindow(WID_SKILLDELAYINFO);
}

// OFFSET: 0x00A77B10
// FUNCTION: fn_16880
void CUISkillDelayInfo::OnSelectItem(SkillDelayInfo::CUIDelayItem*, const SkillDelayInfo::CDelayItemData&) {
}

// OFFSET: 0x00A77B20
// FUNCTION: fn_16881
void CUISkillDelayInfo::OnUpdateItem(SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data) {
  item->level->SetText(std::to_string(data.levelIndex + 1));

  for (int index = 0; index < 4; ++index) {
    const auto found = data.delayTimes.find(index);
    if (data.delayTimes.end() != found) {
      item->times[index]->SetTime(found->second);
    } else {
      item->times[index]->SetTime(0);
    }
  }
}

namespace SkillDelayInfo {

// OFFSET: 0x00A77CE0
// FUNCTION: fn_16882
void CUIDelayItem::Paint() {
  this->backgroundColor = g_skinInfo.PaletteColor(SkinPaletteCell{0, 2}, true);
  this->borderColor = g_skinInfo.PaletteColor(SkinPaletteCell{6, 2}, true);
  this->DrawRectOutline(0, 0, 325, 22, this->borderColor);
  this->FillRegion(1, 0, 43, 21, this->backgroundColor);
  this->FillRegion(44, 0, 280, 21, 0xFFFFFF);
  this->DrawRectOutline(44, 0, 1, 21, this->borderColor);
  this->DrawRectOutline(114, 0, 1, 21, this->borderColor);
  this->DrawRectOutline(184, 0, 1, 21, this->borderColor);
  this->DrawRectOutline(254, 0, 1, 21, this->borderColor);
}

// OFFSET: 0x00A77F00
// FUNCTION: fn_16884
void CUITime::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A77F10
// FUNCTION: fn_16885
void CUIDelayItem::OnInit() {
  this->backgroundColor = g_skinInfo.PaletteColor(SkinPaletteCell{0, 2}, true);
  this->borderColor = g_skinInfo.PaletteColor(SkinPaletteCell{6, 2}, true);
}

// OFFSET: 0x00A78180
// FUNCTION: fn_16887
void CUITime::OnInit() {
}

// OFFSET: 0x00A78A10
// FUNCTION: fn_16890
void CUITime::SetTime(int milliseconds) {
  this->text->SetText(FormatDecimal(static_cast<float>(milliseconds) / 1000.0f, 3, true) + LookupMessage(MSI_SECOND));
}
}

// OFFSET: 0x00A78190
// FUNCTION: fn_16888
int CUISkillDelayInfo::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message != UM_REFRESH) {
    return CUIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
  if (this->skillId != 7999) {
    this->SetSkill(reinterpret_cast<void*>(this->skillId));
  }
  return 0;
}

// INLINED
static void AddDelayItem(CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* list, SkillDelayInfo::CDelayItemData&& item) {
  list->data.push_back(std::move(item));
}

// OFFSET: 0x00A781C0
// FUNCTION: fn_16889
void CUISkillDelayInfo::SetSkill(void* skill) {
  int skillId = reinterpret_cast<int>(skill);
  this->skillId = skillId;
  this->delayList->ClearData();

  const char* skillDescription = nullptr;
  g_session.m_hotkeyScript->Call("GetSkillDescript", "ddd>s", g_session.m_job, skillId, 0, &skillDescription);
  if (!IsNullOrEmpty(skillDescription)) {
    std::vector<std::string> lines = Split(skillDescription, "\r\n");
    if (!lines.empty()) {
      this->skillNameText->SetText(lines[0].c_str(), lines[0].size());
    }
  }

  int skillFlag = 0;
  std::vector<std::map<int, long>> delays;
  g_session.LoadSkillDelayList(skillId, delays, skillFlag);
  for (unsigned int index = 0; index < delays.size(); ++index) {
    auto* list = this->delayList;
    AddDelayItem(list, SkillDelayInfo::CDelayItemData{static_cast<int>(index), delays[index]});
  }
  this->delayList->UpdateScrollBar();

  this->description->Clear();
  if (static_cast<bool>(static_cast<unsigned int>(skillFlag) >> 12 & 1)) {
    this->description->AddLine(LookupMessage(MSI_SKILL_DELAY_FLAG_DISABLE_FIXEDCASTING_REDUCTION));
  }
  if (static_cast<bool>(static_cast<unsigned int>(skillFlag) >> 2 & 1)) {
    if (!this->description->lines.empty()) {
      this->description->AddLine("\n");
    }
    this->description->AddLine(LookupMessage(MSI_SKILL_DELAY_FLAG_NOREDUCT));
  }
  if (skillId == SO_EL_ACTION) {
    if (const char* message = LookupMessage(MSI_SKILL_DELAY_INFO_SO_EL_ACTION)) {
      this->description->AddLine(message);
    }
  }

  this->delayList->Resize(this->delayList->geometry.width, this->delayList->data.size() * this->delayList->rowHeight + this->delayList->itemY);
  if (this->description->lines.empty()) {
    this->description->Show(WINDOW_HIDDEN);
    this->Resize(this->geometry.width, this->delayList->geometry.y + this->delayList->geometry.height + 10);
  } else {
    this->description->Show(WINDOW_VISIBLE);
    this->description->Resize(this->description->geometry.width, this->description->contentHeight + this->description->paddingBottom + this->description->paddingTop);
    this->description->Move(this->description->geometry.x, this->delayList->geometry.y + this->delayList->geometry.height + 10);
    this->Resize(this->geometry.width, this->description->geometry.y + this->description->geometry.height + 10);
  }
}

// OFFSET: 0x00A77DD0
// FUNCTION: fn_16883
void CUISkillDelayInfo::Paint() {
  CUIFrameWnd::Paint();
  this->backgroundColor = g_skinInfo.PaletteColor(SkinPaletteCell{0, 2}, true);
  this->borderColor = g_skinInfo.PaletteColor(SkinPaletteCell{6, 2}, true);
  this->FillRegion(11, 28, 323, 23, this->backgroundColor);
  this->DrawRectOutline(10, 27, 325, 25, this->borderColor);
  this->DrawRectOutline(10, 58, 325, 46, this->borderColor);
  this->FillRegion(11, 59, 323, 44, this->backgroundColor);
  this->DrawRectOutline(11, 81, 323, 1, this->borderColor);
  this->DrawRectOutline(54, 59, 1, 44, this->borderColor);
  this->DrawRectOutline(194, 59, 1, 44, this->borderColor);
  this->DrawRectOutline(124, 83, 1, 21, this->borderColor);
  this->DrawRectOutline(264, 83, 1, 21, this->borderColor);
}

// OFFSET: 0x00A77F70
// FUNCTION: fn_16886
void CUISkillDelayInfo::OnInit() {
  this->backgroundColor = g_skinInfo.PaletteColor(SkinPaletteCell{0, 2}, true);
  this->borderColor = g_skinInfo.PaletteColor(SkinPaletteCell{6, 2}, true);

  this->titleBar->SetTitle(LookupMessage(MSI_SKILL_DELAY_INFO));
  this->columns[0]->SetText(LookupMessage(MSI_SKILL_DELAY_INFO_CAST_FIXED));
  this->columns[1]->SetText(LookupMessage(MSI_SKILL_DELAY_INFO_CAST_STAT));
  this->columns[2]->SetText(LookupMessage(MSI_SKILL_DELAY_INFO_GLOBAL_POST));
  this->columns[3]->SetText(LookupMessage(MSI_SKILL_DELAY_INFO_SINGLE_POST));
  this->labels[0]->SetText(LookupMessage(MSI_SKILL_DELAY_INFO_CASTING));
  this->labels[1]->SetText(LookupMessage(MSI_SKILL_DELAY_INFO_COOLTIME));
  this->labels[2]->SetText(LookupMessage(MSI_SKILL_DELAY_INFO_LEVEL));

  auto* list = this->delayList;
  list->layoutFlag = 1;
  list->Invalidate();
  this->description->scrollBar->SetHidden(1);
}

#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00A720A0
// FUNCTION: fn_16810
void CUISkillDelayInfo::OnCreate(int width, int height) {
  this->titleBar = new CUITitleBar();
  this->titleBar->SetSize(345, 17);
  this->titleBar->Move(0, 0);
  this->titleBar->SetImages(
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\titlebar_left.bmp", 0xF15F8785CDE369BFULL),
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\titlebar_mid.bmp", 0x792FFD1325418FB5ULL),
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\titlebar_right.bmp", 0x49D33529F1556333ULL)
  );
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  title->SetText(OBFUSCATE_TEXT("캐스팅 정보", 0x01CB4F1DF9D3F931ULL));
  title->SetFitText(0);
  title->SetAlign(0, 6);
  title->SetFont(OBFUSCATE_TEXT("Gulim", 0xC5B1BBCD9BC9C997ULL));
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  CUITitleBar* titleBar = this->titleBar;
  titleBar->titleText = title;
  titleBar->AddChild(title);

  CUIImageButton* menuButton = new CUIImageButton();
  menuButton->SetSize(11, 11);
  menuButton->Move(3, 3);
  menuButton->SetOnClick([this]() { this->OnButtonClick(); });
  menuButton->disabled = 0;
  menuButton->Invalidate();
  menuButton->SetImages(
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0xC1BB8111EDD183E1ULL),
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0x1767DDE18D779D8BULL),
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0x0B23D10F2F49D72BULL),
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0x136F2F9915D1DBCFULL)
  );
  this->titleBar->AddChild(menuButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(331, 3);
  closeButton->SetOnClick([this]() { g_windowMgr.CloseWindow(WID_SKILLDELAYINFO); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  closeButton->SetImages(
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0x6BDF334F2BFDAD51ULL),
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0x674D9F3975573D41ULL),
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0x79E915E9AF6F55DDULL),
    OBFUSCATE_TEXT("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0xB7AFBD2FD9FD7501ULL)
  );
  this->titleBar->SetSecondButton(closeButton);

  this->skillNameText = new CUIStaticText();
  this->skillNameText->SetSize(325, 25);
  this->skillNameText->Move(10, 27);
  this->skillNameText->SetText("");
  this->skillNameText->SetFitText(1);
  this->skillNameText->SetAlign(6, 6);
  this->skillNameText->SetFont(OBFUSCATE_TEXT("Gulim", 0x192D654D29337BE5ULL));
  this->skillNameText->SetFontHeight(13);
  this->skillNameText->SetBold(1);
  this->skillNameText->SetColor(0);
  this->skillNameText->SetShadow(0, 0xFFFFFF);
  this->skillNameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->skillNameText);

  this->labels[0] = new CUIStaticText();
  this->labels[0]->SetSize(140, 22);
  this->labels[0]->Move(54, 58);
  this->labels[0]->SetText(OBFUSCATE_TEXT("캐스팅", 0xB3E307A5F753D5E3ULL));
  this->labels[0]->SetFitText(0);
  this->labels[0]->SetAlign(6, 6);
  this->labels[0]->SetFont(OBFUSCATE_TEXT("Gulim", 0xDB27CB557D038759ULL));
  this->labels[0]->SetFontHeight(13);
  this->labels[0]->SetBold(1);
  this->labels[0]->SetColor(0);
  this->labels[0]->SetShadow(0, 0xFFFFFF);
  this->labels[0]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labels[0]);

  this->labels[1] = new CUIStaticText();
  this->labels[1]->SetSize(140, 22);
  this->labels[1]->Move(194, 58);
  this->labels[1]->SetText(OBFUSCATE_TEXT("쿨타임", 0x3FA3AFA7035DE3FFULL));
  this->labels[1]->SetFitText(0);
  this->labels[1]->SetAlign(6, 6);
  this->labels[1]->SetFont(OBFUSCATE_TEXT("Gulim", 0x87E3D1C50D572DEFULL));
  this->labels[1]->SetFontHeight(13);
  this->labels[1]->SetBold(1);
  this->labels[1]->SetColor(0);
  this->labels[1]->SetShadow(0, 0xFFFFFF);
  this->labels[1]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labels[1]);

  this->labels[2] = new CUIStaticText();
  this->labels[2]->SetSize(44, 22);
  this->labels[2]->Move(10, 82);
  this->labels[2]->SetText(OBFUSCATE_TEXT("레벨", 0x51BB39B763BBBD8DULL));
  this->labels[2]->SetFitText(1);
  this->labels[2]->SetAlign(6, 6);
  this->labels[2]->SetFont(OBFUSCATE_TEXT("Gulim", 0xADC51BF9C98B413DULL));
  this->labels[2]->SetFontHeight(12);
  this->labels[2]->SetBold(1);
  this->labels[2]->SetColor(0);
  this->labels[2]->SetShadow(0, 0xFFFFFF);
  this->labels[2]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->labels[2]);

  this->columns[0] = new CUIStaticText();
  this->columns[0]->SetSize(70, 22);
  this->columns[0]->Move(54, 82);
  this->columns[0]->SetText(OBFUSCATE_TEXT("고정", 0xDF8117FF49CFF551ULL));
  this->columns[0]->SetFitText(1);
  this->columns[0]->SetAlign(6, 6);
  this->columns[0]->SetFont(OBFUSCATE_TEXT("Gulim", 0x114901C7E37F6D49ULL));
  this->columns[0]->SetFontHeight(12);
  this->columns[0]->SetBold(1);
  this->columns[0]->SetColor(0);
  this->columns[0]->SetShadow(0, 0xFFFFFF);
  this->columns[0]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[0]);

  this->columns[1] = new CUIStaticText();
  this->columns[1]->SetSize(70, 22);
  this->columns[1]->Move(124, 82);
  this->columns[1]->SetText(OBFUSCATE_TEXT("변동", 0x7D5D0F110B6B8371ULL));
  this->columns[1]->SetFitText(1);
  this->columns[1]->SetAlign(6, 6);
  this->columns[1]->SetFont(OBFUSCATE_TEXT("Gulim", 0xF715514F5961B75BULL));
  this->columns[1]->SetFontHeight(12);
  this->columns[1]->SetBold(1);
  this->columns[1]->SetColor(0);
  this->columns[1]->SetShadow(0, 0xFFFFFF);
  this->columns[1]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[1]);

  this->columns[2] = new CUIStaticText();
  this->columns[2]->SetSize(70, 22);
  this->columns[2]->Move(194, 82);
  this->columns[2]->SetText(OBFUSCATE_TEXT("글로벌", 0x4191BB5531FD3999ULL));
  this->columns[2]->SetFitText(1);
  this->columns[2]->SetAlign(6, 6);
  this->columns[2]->SetFont(OBFUSCATE_TEXT("Gulim", 0xB19D1FA9116F39D3ULL));
  this->columns[2]->SetFontHeight(12);
  this->columns[2]->SetBold(1);
  this->columns[2]->SetColor(0);
  this->columns[2]->SetShadow(0, 0xFFFFFF);
  this->columns[2]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[2]);

  this->columns[3] = new CUIStaticText();
  this->columns[3]->SetSize(70, 22);
  this->columns[3]->Move(264, 82);
  this->columns[3]->SetText(OBFUSCATE_TEXT("스킬", 0x9569C9C7E70909AFULL));
  this->columns[3]->SetFitText(1);
  this->columns[3]->SetAlign(6, 6);
  this->columns[3]->SetFont(OBFUSCATE_TEXT("Gulim", 0xD7A749EB27D5BF6DULL));
  this->columns[3]->SetFontHeight(12);
  this->columns[3]->SetBold(1);
  this->columns[3]->SetColor(0);
  this->columns[3]->SetShadow(0, 0xFFFFFF);
  this->columns[3]->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->columns[3]);

  CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>* listBox = new CUIListBoxEx<SkillDelayInfo::CUIDelayItem, SkillDelayInfo::CDelayItemData>();
  this->delayList = listBox;
  listBox->SetListSize(325, 220, 325, 22);
  this->delayList->Move(10, 104);
  this->delayList->SetOnSelect([this](SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data) { this->OnSelectItem(item, data); });
  this->delayList->SetBinder([this](SkillDelayInfo::CUIDelayItem* item, const SkillDelayInfo::CDelayItemData& data) { this->OnUpdateItem(item, data); });
  this->delayList->SetImages("", "", "", "");
  auto* delayList = this->delayList;
  delayList->itemX = 0;
  delayList->itemY = 0;
  this->delayList->scrollBar->SetSize(13, 100);
  this->delayList->scrollBar->Move(422, 0);
  this->delayList->scrollBar->step = 10;
  this->delayList->SetScrollBarButtonImages(
    OBFUSCATE_TEXT("유저인터페이스\\scroll0up.bmp", 0xB195637DAFCB0D59ULL),
    OBFUSCATE_TEXT("유저인터페이스\\scroll0down.bmp", 0xADADEB4DF7813B55ULL)
  );
  this->delayList->SetScrollBarBackground(OBFUSCATE_TEXT("유저인터페이스\\scroll0mid.bmp", 0xD9452D81E5CD99C1ULL));
  this->delayList->SetScrollBarThumbImages(
    OBFUSCATE_TEXT("유저인터페이스\\scroll0bar_up.bmp", 0xFD69A9B3F311818DULL),
    OBFUSCATE_TEXT("유저인터페이스\\scroll0bar_mid.bmp", 0x7F2DFFC1ABE91F2BULL),
    OBFUSCATE_TEXT("유저인터페이스\\scroll0bar_down.bmp", 0xD98F5345098D630DULL)
  );
  this->delayList->AddNewItem();
  this->AddChild(this->delayList);

  this->description = new CUITextViewer();
  this->description->SetSize(330, 35);
  this->description->Move(11, 334);
  this->description->SetTextOffset(0, 0);
  this->description->SetFont(OBFUSCATE_TEXT("Gulim", 0x9B0F234DEFDF7593ULL));
  this->description->SetFontHeight(11);
  this->description->SetBold(false);
  this->description->SetTextColor(0xFF);
  this->description->SetOutline(false, 0xFFFFFF);
  this->description->SetLineSpacing(6);
  this->description->SetBackgroundColor(0xFFFFFF);
  this->description->SetPadding(3, 3, 3, 3);
  this->description->SetScrollBarSize(13, 35);
  this->description->SetScrollStep(1);
  this->description->SetScrollBarButtonImages(
    OBFUSCATE_TEXT("유저인터페이스\\scroll0up.bmp", 0xC9712D676B5B3943ULL),
    OBFUSCATE_TEXT("유저인터페이스\\scroll0down.bmp", 0xED15938D333D2DB9ULL)
  );
  this->description->SetScrollBarBackground(OBFUSCATE_TEXT("유저인터페이스\\scroll0mid.bmp", 0xBD2BA1C11B01E73BULL));
  this->description->SetScrollBarThumbImages(
    OBFUSCATE_TEXT("유저인터페이스\\scroll0bar_up.bmp", 0x4B755DB19953E311ULL),
    OBFUSCATE_TEXT("유저인터페이스\\scroll0bar_mid.bmp", 0x4331A17F33095FEBULL),
    OBFUSCATE_TEXT("유저인터페이스\\scroll0bar_down.bmp", 0xD1056FF94D4D2773ULL)
  );
  this->AddChild(this->description);
}
