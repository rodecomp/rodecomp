#include "UI/Quest/UIOngoingQuestInfo.h"

#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/RenewQuestMgr.h"
#include "Render/Renderer.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Quest/UIRenewQuestWnd.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstring>
#include <string>

template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::OnScroll(int position);

// OFFSET: 0x00A4F630
// FUNCTION: fn_16217
template <>
CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::CUIListBoxEx(UIWindowContext& context)
    : CUIControl(context), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), unknown_00C4(0), selected(static_cast<unsigned int>(-1)),
      unknown_00E0(1), rebind(0) {
}

// OFFSET: 0x00A4FDE0
// FUNCTION: fn_16223
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::ClearItems() {
  for (OngoingQuestInfo::CUIRewardItem* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x00A4FE70
// FUNCTION: fn_16224
void CUIOngoingQuestInfo::Slot53() {
  this->unknown_00B4 = 0;
  this->m_resolveSkin = true;
  this->Resize(342, 412);
  this->Move(0, 0);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
}

// OFFSET: 0x00A4FF10
// FUNCTION: fn_16225
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

// OFFSET: 0x00A539F0
// FUNCTION: fn_16231
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A53A00
// FUNCTION: fn_16232
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::Slot35(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->Slot35(value);
  } else if (this->parent) {
    this->parent->Slot35(value);
  }
}

// OFFSET: 0x00A53F80
// FUNCTION: fn_16236
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::OnScroll(int position) {
  auto it = this->items.begin();
  if (it == this->items.end()) {
    return;
  }
  if (this->data.size() == 0) {
    for (; it != this->items.end(); ++it) {
      (*it)->index = -1;
      (*it)->Show(0);
    }
    return;
  }

  const int first = (*it)->index;
  unsigned int row = position / this->rowHeight;
  if (first != -1) {
    if (first < static_cast<int>(row)) {
      if (row - first < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.begin() + (row - first), this->items.end());
      }
    } else if (first > static_cast<int>(row)) {
      if (first - row < static_cast<unsigned int>(this->items.size())) {
        std::rotate(this->items.begin(), this->items.end() - (first - row), this->items.end());
      }
    }
  }

  it = this->items.begin();
  for (int y = this->itemY - position % this->rowHeight; y < this->geometry.height; y += this->rowHeight, ++it, ++row) {
    if (it == this->items.end() || row >= this->data.size()) {
      break;
    }
    OngoingQuestInfo::CUIRewardItem* item = *it;
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
      item->Show(1);
    }
  }

  for (; it != this->items.end(); ++it) {
    (*it)->index = -1;
    (*it)->Show(0);
  }
}

// OFFSET: 0x00A55650
// FUNCTION: fn_16300
void CUIOngoingQuestInfo::OnFirstAction() {
}

namespace OngoingQuestInfo {

// OFFSET: 0x00A55710
// FUNCTION: fn_16301
void CUIRewards::OnPrepareItem(CUIRewardItem*, const CRewardItemData&) {
}

// OFFSET: 0x00A55860
// FUNCTION: fn_16303
void CUIContents::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A55870
// FUNCTION: fn_16304
void CUIRewardItem::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A55880
// FUNCTION: fn_16305
void CUIRewards::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A55890
// FUNCTION: fn_16306
void CUIScrollWindow::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x00A558A0
// FUNCTION: fn_16307
void CUIContents::Slot16() {
}

// OFFSET: 0x00A55940
// FUNCTION: fn_16309
void CUIRewardItem::Slot16() {
}

// OFFSET: 0x00A55B50
// FUNCTION: fn_16312
void CUIScrollWindow::Slot35(int value) {
  this->scrollBar->Slot35(value);
}

// OFFSET: 0x00A55E10
// FUNCTION: fn_16314
void CUIContents::UpdateHeight() {
  this->textViewer->Show(!this->textViewer->lines.empty());
  if (!this->textViewer->lines.empty()) {
    CUITextViewer* viewer = this->textViewer;
    viewer->Resize(viewer->geometry.width, viewer->contentHeight + viewer->paddingBottom + viewer->paddingTop);
  }

  int bottom = 0;
  for (UIWindow* child : this->m_children) {
    if (child->geometry.visible) {
      bottom = (std::max)(bottom, child->geometry.y + child->geometry.height);
    }
  }
  UIWindow::Resize(this->geometry.width, bottom + 9);
}

// OFFSET: 0x00A55EA0
// FUNCTION: fn_16315
void CUIRewards::UpdateHeight() {
  this->rewardList->Show(this->rewardList->data.size() != 0);
  if (this->rewardList->data.size() != 0) {
    this->rewardList->Resize(this->rewardList->geometry.width, this->rewardList->rowHeight * this->rewardList->data.size() + this->rewardList->itemY);
  }

  int bottom = 0;
  for (UIWindow* child : this->m_children) {
    if (child->geometry.visible) {
      bottom = (std::max)(bottom, child->geometry.y + child->geometry.height);
    }
  }
  UIWindow::Resize(this->geometry.width, bottom + 9);
}

// OFFSET: 0x00A55F20
// FUNCTION: fn_16316
void CUIScrollWindow::OnScroll(int position) {
  int y = -position;
  for (UIWindow* child : this->m_children) {
    if (child != this->scrollBar) {
      child->Move(child->geometry.x, y);
      y = child->geometry.y + child->geometry.height;
    }
  }
}

}

namespace OngoingQuestInfo {

// OFFSET: 0x00A55720
// FUNCTION: fn_16302
void CUIRewards::OnBindItem(CUIRewardItem* item, const CRewardItemData& data) {
  ITEM_INFO info;
  info.SetItemId(data.itemId);
  info.identified = 1;
  item->gameItem->SetItem(info);

  ItemDescriptions descriptions{g_itemDescriptions};
  bool languagePrefix = false;
  WORD language = g_serverLanguage->language;
  ItemNaming naming{descriptions, g_itemCardNames, *g_playerNameDictionary, languagePrefix, g_serviceType, language};
  std::string name = naming.DisplayName(info, 0);
  item->nameText->SetText(name.c_str(), name.size());
  std::string count = FormatOwnedString(LookupMessage(0xB6), data.count);
  item->countText->SetText(count.c_str(), count.size());
}

}

// OFFSET: 0x00A558B0
// FUNCTION: fn_16308
void CUIOngoingQuestInfo::Slot16() {
  const char* title = LookupMessage(0xD48);
  this->titleBar->SetTitle(title, std::strlen(title));
  UIWindow* window = g_windowMgr.LookupWindow(0x2718);
  if (window) {
    this->Move(window->geometry.x + window->geometry.width, window->geometry.y);
  } else {
    this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
  }
}

namespace OngoingQuestInfo {

// OFFSET: 0x00A55A90
// FUNCTION: fn_16311
void CUIScrollWindow::Slot16() {
  this->firstContents->EnableClip();
  this->secondContents->EnableClip();
  this->rewards->EnableClip();
  const char* first = LookupMessage(0xD49);
  this->firstContents->title->SetText(first, std::strlen(first));
  const char* second = LookupMessage(0xD4A);
  this->secondContents->title->SetText(second, std::strlen(second));
  const char* reward = LookupMessage(0xD4B);
  this->rewards->title->SetText(reward, std::strlen(reward));
}

}

namespace OngoingQuestInfo {

// OFFSET: 0x00A55950
// FUNCTION: fn_16310
void CUIRewards::Slot16() {
  const char* first = LookupMessage(0xD4C);
  this->firstLabel->SetText(first, std::strlen(first));
  const char* second = LookupMessage(0xD4D);
  this->secondLabel->SetText(second, std::strlen(second));
  const char* third = LookupMessage(0xD4E);
  this->thirdLabel->SetText(third, std::strlen(third));
  this->rewardList->unknown_00C4 = 1;
  this->rewardList->Invalidate();
  this->npcSprite = new UINpcSprWnd(*this->m_context);
  this->npcSprite->SetSize(75, 145);
  this->npcSprite->Show(0);
  this->AddChild(this->npcSprite);
}

}

// OFFSET: 0x00A55B70
// FUNCTION: fn_16313
void CUIOngoingQuestInfo::sub_A55B70() {
  OngoingQuestInfo::CUIScrollWindow* window = this->scrollWindow;
  const unsigned int questId = this->questId;
  window->secondContents->textViewer->Clear();
  const auto found = g_renewQuestMgr->m_ongoingQuests.find(questId);
  if (found != g_renewQuestMgr->m_ongoingQuests.end()) {
    for (int index = 0; index < found->second.m_huntCount; ++index) {
      window->secondContents->textViewer->AddLine(found->second.GetHuntText(index));
    }
  }

  window->firstContents->UpdateHeight();
  window->secondContents->UpdateHeight();
  window->rewards->UpdateHeight();

  const int total = window->ContentHeight();
  window->scrollBar->SetTotal((std::max)(window->geometry.height, total));
  window->scrollBar->SetPosition(0);
}
