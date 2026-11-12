#include "UI/Window/WindowId.h"
#include "UI/Quest/UIOngoingQuestInfo.h"

#include "Client/Input/InputCharset.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Render/Renderer.h"
#include "UI/Items/UIGameItem.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Quest/UIRenewQuestWnd.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/UIRegisterMgr.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cstring>
#include <format>
#include <string>

template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::pair<const unsigned int, RenewQuestOngoingInfo>, void*>*>
std::map<unsigned int, RenewQuestOngoingInfo>::_Mybase::_Find_lower_bound<unsigned int>(const unsigned int&) const;

// OFFSET: 0x00A4F580
// FUNCTION: fn_16213
bool UnusedTrueA4F580() {
  return true;
}

// OFFSET: 0x00A4F590
// FUNCTION: fn_16214
bool UnusedTrueA4F590() {
  return true;
}

// OFFSET: 0x00A4F5A0
// FUNCTION: fn_16215
bool UnusedTrueA4F5A0() {
  return true;
}

// OFFSET: 0x00A4F630
// FUNCTION: fn_16217
template <>
CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::CUIListBoxEx()
    : CUIControl(), scrollBar(nullptr), itemWidth(0), rowHeight(0), itemX(0), itemY(0), layoutFlag(0), selected(static_cast<unsigned int>(-1)),
      selectionFlag(1), rebind(0) {
}

// OFFSET: 0x00A4F7F0
// FUNCTION: fn_16218
template <>
CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::~CUIListBoxEx() {
}

// OFFSET: 0x00A4FD60
// FUNCTION: fn_16222
CUIOngoingQuestInfo* CreateOngoingQuestInfo() {
  return new CUIOngoingQuestInfo();
}

// OFFSET: 0x00497950
// FUNCTION: fn_45552
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateOngoingQuestInfo;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10009, factory);
  return true;
}();

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
void CUIOngoingQuestInfo::Initialize() {
  this->m_ownsBackgroundColor = false;
  this->m_ownsBorderColor = false;
  this->m_resolveSkin = true;
  this->Resize(342, 412);
  this->Move(0, 0);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
}

// OFFSET: 0x00A4FF10
// FUNCTION: fn_16225
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
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
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::OnWheel(int value) {
  if (!this->scrollBar->dragging && this->scrollBar->total > this->scrollBar->visible) {
    this->scrollBar->OnWheel(value);
  } else if (this->parent) {
    this->parent->OnWheel(value);
  }
}

// OFFSET: 0x00A53F80
// FUNCTION: fn_16236
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::OnScroll(int position) {
  if (this->items.empty()) {
    return;
  }
  if (this->data.size() == 0) {
    for (OngoingQuestInfo::CUIRewardItem* item : this->items) {
      item->index = -1;
      item->Show(0);
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

  for (const auto end = this->items.end(); it != end; ++it) {
    OngoingQuestInfo::CUIRewardItem* item = *it;
    item->index = -1;
    item->Show(0);
  }
}

// OFFSET: 0x00A55650
// FUNCTION: fn_16300
void CUIOngoingQuestInfo::OnFirstAction() {
}

// OFFSET: 0x00A55660
// FUNCTION: fn_45468
void CUIOngoingQuestInfo::OnClose() {
  g_windowMgr.CloseWindow(WID_ONGOINGQUESTINFO);
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
void CUIContents::OnInit() {
}

// OFFSET: 0x00A55940
// FUNCTION: fn_16309
void CUIRewardItem::OnInit() {
}

// OFFSET: 0x00A55B50
// FUNCTION: fn_16312
void CUIScrollWindow::OnWheel(int value) {
  this->scrollBar->OnWheel(value);
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

  item->nameText->SetText(info.DisplayName(0));
  item->countText->SetText(FormatOwnedString(LookupMessage(MSI_EA), data.count));
}

}

// OFFSET: 0x00A558B0
// FUNCTION: fn_16308
void CUIOngoingQuestInfo::OnInit() {
  this->titleBar->SetTitle(LookupMessage(MSI_RENEW_QUEST_DETAIL_REWARD));
  UIWindow* window = g_windowMgr.LookupWindow(WID_RENEWQUESTUI);
  if (window) {
    this->Move(window->geometry.x + window->geometry.width, window->geometry.y);
  } else {
    this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
  }
}

namespace OngoingQuestInfo {

// OFFSET: 0x00A55A90
// FUNCTION: fn_16311
void CUIScrollWindow::OnInit() {
  this->firstContents->EnableClip();
  this->secondContents->EnableClip();
  this->rewards->EnableClip();
  this->firstContents->title->SetText(LookupMessage(MSI_RENEW_QUEST_DETAIL_EXP));
  this->secondContents->title->SetText(LookupMessage(MSI_RENEW_QUEST_DETAIL_JEXP));
  this->rewards->title->SetText(LookupMessage(MSI_RENEW_QUEST_DETAIL_ITEM));
}

}

namespace OngoingQuestInfo {

// OFFSET: 0x00A55950
// FUNCTION: fn_16310
void CUIRewards::OnInit() {
  this->firstLabel->SetText(LookupMessage(MSI_RENEW_QUEST_CHECK_TOOTIP1_ITEM));
  this->secondLabel->SetText(LookupMessage(MSI_RENEW_QUEST_CHECK_TOOTIP2_ITEM));
  this->thirdLabel->SetText(LookupMessage(MSI_RENEW_QUEST_CHECK_TOOTIP3_ITEM));
  CUIListBoxEx<CUIRewardItem, CRewardItemData>* list = this->rewardList;
  list->layoutFlag = 1;
  list->Invalidate();
  this->npcSprite = new UINpcSprWnd();
  this->npcSprite->SetSize(75, 145);
  this->npcSprite->Show(0);
  this->AddChild(this->npcSprite);
}

}

// OFFSET: 0x00A55B70
// FUNCTION: fn_16313
void CUIOngoingQuestInfo::UpdateHuntInfo() {
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

// OFFSET: 0x00A56050
// FUNCTION: fn_16317
void CUIOngoingQuestInfo::SetQuest(unsigned int id) {
  const auto found = g_renewQuestMgr->m_ongoingQuests.find(id);
  if (found != g_renewQuestMgr->m_ongoingQuests.end()) {
    const RenewQuestOngoingInfo& quest = found->second;
    this->titleText->SetText(quest.m_title);
    this->dateText->SetText(quest.GetDateString());
    this->scrollWindow->SetQuest(id);
    this->questId = id;
  }
}

namespace OngoingQuestInfo {

// OFFSET: 0x00A56150
// FUNCTION: fn_16318
void CUIScrollWindow::SetQuest(unsigned int id) {
  this->firstContents->textViewer->Clear();
  this->secondContents->textViewer->Clear();
  this->rewards->rewardList->ClearData();

  const auto found = g_renewQuestMgr->m_ongoingQuests.find(id);
  if (found != g_renewQuestMgr->m_ongoingQuests.end()) {
    const RenewQuestOngoingInfo& quest = found->second;
    for (const std::string& description : quest.m_descriptions) {
      this->firstContents->textViewer->AddLine(description);
    }
    for (int index = 0; index < quest.m_huntCount; ++index) {
      this->secondContents->textViewer->AddLine(quest.GetHuntText(index));
    }
    this->rewards->firstText->SetText(quest.baseExpReward);
    this->rewards->secondText->SetText(quest.jobExpReward);
    for (const auto& reward : quest.m_rewards) {
      this->rewards->rewardList->AddData(CRewardItemData{static_cast<int>(reward.first), static_cast<int>(reward.second)}, false);
    }
    this->rewards->rewardList->UpdateScrollBar();

    CUIRewards* rewards = this->rewards;
    if (quest.npcSprite.empty()) {
      rewards->npcSprite->Show(0);
    } else {
      rewards->npcSprite->SetSprite(75, 145, quest.npcSprite);
      if (!quest.destinationMap.empty()) {
        rewards->npcSprite->SetDestination(quest.destinationMap, quest.destinationX, quest.destinationY);
      }
      rewards->npcSprite->Move(rewards->geometry.width - rewards->npcSprite->geometry.width - 23, 9);
      rewards->npcSprite->Show(1);
    }
  } else {
    g_windowMgr.Confirm(std::format("Not found Quest Info = %lu", id).c_str(), 0, true, false, 0, "Message", 280, 120, 0);
  }

  this->firstContents->UpdateHeight();
  this->secondContents->UpdateHeight();
  this->rewards->UpdateHeight();

  const int total = this->ContentHeight();
  this->scrollBar->SetTotal((std::max)(this->geometry.height, total));
  this->scrollBar->SetPosition(0);
}

}

// OFFSET: 0x00A50970
// FUNCTION: fn_16227
void CUIOngoingQuestInfo::OnCreate(int width, int height) {
  this->titleBar = new CUITitleBar();
  this->titleBar->SetSize(342, 17);
  this->titleBar->Move(0, 0);
  this->titleBar->SetImages(
    "유저인터페이스\\basic_interface\\titlebar_left.bmp", "유저인터페이스\\basic_interface\\titlebar_mid.bmp",
    "유저인터페이스\\basic_interface\\titlebar_right.bmp"
  );
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  title->SetText("QUEST");
  title->SetFitText(0);
  title->SetAlign(0, 6);
  title->SetFont("Gulim");
  title->SetFontHeight(13);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(0, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  CUITitleBar* titleBar = this->titleBar;
  titleBar->titleText = title;
  titleBar->AddChild(title);

  CUIImageButton* menuButton = new CUIImageButton();
  menuButton->SetSize(11, 11);
  menuButton->Move(3, 3);
  menuButton->SetOnClick([this]() { this->OnFirstAction(); });
  menuButton->disabled = 0;
  menuButton->Invalidate();
  menuButton->SetImages(
    "유저인터페이스\\basic_interface\\sys_base_off.bmp", "유저인터페이스\\basic_interface\\sys_base_on.bmp",
    "유저인터페이스\\basic_interface\\sys_base_on.bmp", "유저인터페이스\\basic_interface\\sys_base_off.bmp"
  );
  this->titleBar->AddChild(menuButton);

  this->background = new CUIStaticImage();
  this->background->SetSize(342, 395);
  this->background->Move(0, 17);
  this->background->SetImage("유저인터페이스\\Renew_QuestUI\\bg_detail_quest.bmp");
  this->AddChild(this->background);

  this->titleText = new CUIStaticText();
  this->titleText->SetSize(342, 12);
  this->titleText->Move(0, 39);
  this->titleText->SetText("");
  this->titleText->SetFitText(1);
  this->titleText->SetAlign(6, 0);
  this->titleText->SetFont("Gulim");
  this->titleText->SetFontHeight(12);
  this->titleText->SetBold(1);
  this->titleText->SetColor(0x55321D);
  this->titleText->SetShadow(0, 0xFFFFFF);
  this->titleText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->titleText);

  this->scrollWindow = new OngoingQuestInfo::CUIScrollWindow();
  this->scrollWindow->SetSize(342, 322);
  this->scrollWindow->Move(0, 62);
  this->AddChild(this->scrollWindow);

  this->dateText = new CUIStaticText();
  this->dateText->SetSize(270, 12);
  this->dateText->Move(13, 393);
  this->dateText->SetText("");
  this->dateText->SetFitText(0);
  this->dateText->SetAlign(0, 0);
  this->dateText->SetFont("Gulim");
  this->dateText->SetFontHeight(12);
  this->dateText->SetBold(0);
  this->dateText->SetColor(0x161BD2);
  this->dateText->SetShadow(0, 0xFFFFFF);
  this->dateText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->dateText);

  this->closeButton = new CUIImageButton();
  this->closeButton->SetSize(42, 20);
  this->closeButton->Move(288, 388);
  this->closeButton->SetOnClick([this]() { this->OnClose(); });
  CUIImageButton* closeButton = this->closeButton;
  closeButton->disabled = 0;
  closeButton->Invalidate();
  this->closeButton->SetImages(
    "유저인터페이스\\basic_interface\\btn_close.bmp", "유저인터페이스\\basic_interface\\btn_close_b.bmp",
    "유저인터페이스\\basic_interface\\btn_close_a.bmp","유저인터페이스\\basic_interface\\btn_close.bmp"
  );
  this->AddChild(this->closeButton);
}

namespace OngoingQuestInfo {

// OFFSET: 0x00A4FFE0
// FUNCTION: fn_16226
void CUIContents::OnCreate(int width, int height) {
  this->icon = new CUIStaticImage();
  this->icon->SetSize(10, 9);
  this->icon->Move(9, 9);
  this->icon->SetImage("유저인터페이스\\Renew_QuestUI\\img_poring.bmp");
  this->AddChild(this->icon);

  this->title = new CUIStaticText();
  this->title->SetSize(50, 12);
  this->title->Move(25, 9);
  this->title->SetText("", 0);
  this->title->SetFitText(0);
  this->title->SetAlign(0, 0);
  this->title->SetFont("Gulim");
  this->title->SetFontHeight(12);
  this->title->SetBold(0);
  this->title->SetColor(0);
  this->title->SetShadow(0, 0xFFFFFF);
  this->title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->title);

  this->textViewer = new CUITextViewer();
  this->textViewer->SetSize(290, 20);
  this->textViewer->Move(32, 30);
  this->textViewer->SetTextOffset(0, 0);
  this->textViewer->SetFont("Gulim");
  this->textViewer->SetFontHeight(12);
  this->textViewer->SetBold(0);
  this->textViewer->SetTextColor(0x38302E);
  this->textViewer->SetOutline(0, 0xFFFFFF);
  this->textViewer->SetLineSpacing(6);
  this->textViewer->SetBackgroundColor(0xFFFF00FF);
  this->textViewer->SetPadding(4, 4, 4, 4);
  this->textViewer->SetScrollBarSize(7, 20);
  this->textViewer->SetScrollStep(18);
  this->textViewer->SetScrollBarButtonImages(
    "유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_upper.bmp", "유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_down.bmp"
  );
  this->textViewer->SetScrollBarBackground("");
  this->textViewer->SetScrollBarThumbImages(
    "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_up.bmp", "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_mid.bmp",
    "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_down.bmp"
  );
  this->AddChild(this->textViewer);
}

// OFFSET: 0x00A51900
// FUNCTION: fn_16228
void CUIRewardItem::OnCreate(int width, int height) {
  this->background = new CUIStaticImage();
  this->background->SetSize(38, 38);
  this->background->Move(0, 0);
  this->background->SetImage("유저인터페이스\\Renew_QuestUI\\img_questIocn.bmp");
  this->AddChild(this->background);

  this->gameItem = new CUIGameItem();
  this->gameItem->SetMode(0);
  this->gameItem->Move(7, 7);
  this->gameItem->showNameTooltip = 1;
  this->gameItem->allowItemInfo = 1;
  this->AddChild(this->gameItem);

  this->nameText = new CUIStaticText();
  this->nameText->SetSize(170, 12);
  this->nameText->Move(46, 5);
  this->nameText->SetText("");
  this->nameText->SetFitText(0);
  this->nameText->SetAlign(0, 0);
  this->nameText->SetFont("Gulim");
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(0);
  this->nameText->SetColor(0);
  this->nameText->SetShadow(0, 0xFFFFFF);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->nameText);

  this->countText = new CUIStaticText();
  this->countText->SetSize(170, 12);
  this->countText->Move(46, 20);
  this->countText->SetText("");
  this->countText->SetFitText(0);
  this->countText->SetAlign(0, 0);
  this->countText->SetFont("Gulim");
  this->countText->SetFontHeight(12);
  this->countText->SetBold(0);
  this->countText->SetColor(0xA0551D);
  this->countText->SetShadow(0, 0xFFFFFF);
  this->countText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->countText);
}

// OFFSET: 0x00A53170
// FUNCTION: fn_16230
void CUIScrollWindow::OnCreate(int width, int height) {
  this->firstContents = new CUIContents();
  this->firstContents->SetSize(335, 59);
  this->firstContents->Move(0, 0);
  this->AddChild(this->firstContents);

  this->firstLine = new CUIStaticImage();
  this->firstLine->SetSize(334, 1);
  this->firstLine->Move(1, 59);
  this->firstLine->SetImage("유저인터페이스\\Renew_QuestUI\\img_line.bmp");
  this->AddChild(this->firstLine);

  this->secondContents = new CUIContents();
  this->secondContents->SetSize(335, 59);
  this->secondContents->Move(0, 60);
  this->AddChild(this->secondContents);

  this->secondLine = new CUIStaticImage();
  this->secondLine->SetSize(334, 1);
  this->secondLine->Move(1, 119);
  this->secondLine->SetImage("유저인터페이스\\Renew_QuestUI\\img_line.bmp");
  this->AddChild(this->secondLine);

  this->rewards = new CUIRewards();
  this->rewards->SetSize(335, 122);
  this->rewards->Move(0, 120);
  this->AddChild(this->rewards);

  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->SetSize(7, 322);
  this->scrollBar->Move(335, 0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->scrollBar->SetTotal(322);
  this->scrollBar->SetVisible(322);
  this->scrollBar->step = 15;
  this->scrollBar->SetButtonImages(
    "유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_upper.bmp", "유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_down.bmp"
  );
  this->scrollBar->SetBackground("");
  this->scrollBar->SetThumbImages(
    "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_up.bmp", "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_mid.bmp",
    "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_down.bmp"
  );
  this->AddChild(this->scrollBar);
}

}

namespace OngoingQuestInfo {
// OFFSET: 0x00A54D00
// FUNCTION: fn_16289
CUIContents::CUIContents() : CUIControl(), icon(nullptr), title(nullptr), textViewer(nullptr) {
}
}

// OFFSET: 0x00A54D90
// FUNCTION: fn_16290
CUIOngoingQuestInfo::CUIOngoingQuestInfo()
    : titleBar(nullptr), backgroundStorage{}, titleText(nullptr), scrollWindow(nullptr), dateText(nullptr), closeButtonStorage{}, questId(0) {
}

namespace OngoingQuestInfo {
// OFFSET: 0x00A54F20
// FUNCTION: fn_16292
CUIRewards::CUIRewards()
    : CUIControl(), icon(nullptr), title(nullptr), firstLabel(nullptr), firstText(nullptr), secondLabel(nullptr), secondText(nullptr), thirdLabel(nullptr),
      rewardList(nullptr), npcSprite(nullptr) {
}

// OFFSET: 0x00A54FF0
// FUNCTION: fn_16293
CUIScrollWindow::CUIScrollWindow()
    : CUIControl(), firstContents(nullptr), firstLine(nullptr), secondContents(nullptr), secondLine(nullptr), rewards(nullptr), scrollBar(nullptr) {
}
}

// INLINED
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::SelectItem(CUIListItem* item) {
  if (this->layoutFlag) {
    return;
  }
  for (OngoingQuestInfo::CUIRewardItem* other : this->items) {
    if (item != other) {
      other->SetSelected(0);
    }
  }
  item->SetSelected(1);
  if (item->index == static_cast<int>(this->selected) && this->selectionFlag) {
    return;
  }
  OngoingQuestInfo::CUIRewardItem* selectedItem = dynamic_cast<OngoingQuestInfo::CUIRewardItem*>(item);
  if (!selectedItem) {
    return;
  }
  this->selected = selectedItem->index;
  if (this->onSelect && static_cast<int>(this->selected) >= 0 && this->selected < this->data.size()) {
    auto node = this->data.begin();
    std::advance(node, static_cast<int>(this->selected));
    this->onSelect(selectedItem, *node);
  }
}

// OFFSET: 0x00A53A90
// FUNCTION: fn_16233
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::AddNewItem() {
  if (this->itemWidth <= 0 || this->rowHeight <= 0) {
    return;
  }

  for (OngoingQuestInfo::CUIRewardItem* item : this->items) {
    this->RemoveChild(item, 1);
  }
  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);

  if (this->geometry.height >= this->rowHeight) {
    unsigned count = static_cast<unsigned>(this->geometry.height / this->rowHeight) + 1;
    if (this->geometry.height % this->rowHeight >= 2) {
      ++count;
    }
    this->items.resize(count);
    for (OngoingQuestInfo::CUIRewardItem*& slot : this->items) {
      OngoingQuestInfo::CUIRewardItem* item = new OngoingQuestInfo::CUIRewardItem();
      item->SetSize(this->itemWidth, this->rowHeight);
      item->Show(0);
      item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
      item->onHover = [this](CUIListItem* listItem) {
        for (OngoingQuestInfo::CUIRewardItem* other : this->items) {
          other->SetHighlighted(other == listItem);
        }
      };
      item->onPress = [this](CUIListItem* listItem) {
        this->SelectItem(listItem);
      };
      this->AddChild(item);
      slot = item;
    }
  }

  this->scrollBar->SetTotal(this->geometry.height);
  this->scrollBar->SetVisible(this->geometry.height);
}

// OFFSET: 0x00A53CF0
// FUNCTION: fn_16234
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::LoadSkin(bool resolveSkin) {
  CResPtr<CBitmapRes>* images = reinterpret_cast<CResPtr<CBitmapRes>*>(reinterpret_cast<std::uint8_t*>(this) + 0x80);
  images[0] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[0], resolveSkin));
  images[1] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[1], resolveSkin));
  images[2] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->imageNames[2], resolveSkin));
  images[3] = CResPtr<CBitmapRes>(this->LoadBitmapResource(this->disabledName, resolveSkin));
  for (OngoingQuestInfo::CUIRewardItem* item : this->items) {
    item->SetImageResources(images[0], images[1], images[2], images[3]);
  }
}

// OFFSET: 0x00A53F00
// FUNCTION: fn_16235
template <>
void CUIListBoxEx<OngoingQuestInfo::CUIRewardItem, OngoingQuestInfo::CRewardItemData>::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->AddNewItem();
  this->scrollBar->Resize(this->scrollBar->geometry.width, height);
  this->scrollBar->SetTotal(static_cast<int>(this->data.size()) * this->rowHeight + this->itemY);
  this->scrollBar->SetVisible(this->geometry.height);
  this->scrollBar->SetPosition(this->scrollBar->position);
  this->rebind = 1;
}

namespace OngoingQuestInfo {
// OFFSET: 0x00A54EA0
// FUNCTION: fn_16291
CUIRewardItem::CUIRewardItem() : CUIListItem(), background(nullptr), gameItem(nullptr), nameText(nullptr), countText(nullptr) {
}
}

namespace OngoingQuestInfo {
// OFFSET: 0x00A51ED0
// FUNCTION: fn_16229
void CUIRewards::OnCreate(int width, int height) {
  this->icon = new CUIStaticImage();
  this->icon->SetSize(10, 9);
  this->icon->Move(9, 9);
  this->icon->SetImage("유저인터페이스\\Renew_QuestUI\\img_poring.bmp");
  this->AddChild(this->icon);

  this->title = new CUIStaticText();
  this->title->SetSize(50, 12);
  this->title->Move(25, 9);
  this->title->SetText("");
  this->title->SetFitText(0);
  this->title->SetAlign(0, 0);
  this->title->SetFont("Gulim");
  this->title->SetFontHeight(12);
  this->title->SetBold(0);
  this->title->SetColor(0);
  this->title->SetShadow(0, 0xFFFFFF);
  this->title->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->title);

  this->firstLabel = new CUIStaticText();
  this->firstLabel->SetSize(50, 12);
  this->firstLabel->Move(32, 30);
  this->firstLabel->SetText("");
  this->firstLabel->SetFitText(0);
  this->firstLabel->SetAlign(0, 0);
  this->firstLabel->SetFont("Gulim");
  this->firstLabel->SetFontHeight(12);
  this->firstLabel->SetBold(0);
  this->firstLabel->SetColor(0);
  this->firstLabel->SetShadow(0, 0xFFFFFF);
  this->firstLabel->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->firstLabel);

  this->firstText = new CUIStaticText();
  this->firstText->SetSize(150, 12);
  this->firstText->Move(110, 30);
  this->firstText->SetText("");
  this->firstText->SetFitText(0);
  this->firstText->SetAlign(0, 0);
  this->firstText->SetFont("Gulim");
  this->firstText->SetFontHeight(12);
  this->firstText->SetBold(0);
  this->firstText->SetColor(0xA0551D);
  this->firstText->SetShadow(0, 0xFFFFFF);
  this->firstText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->firstText);

  this->secondLabel = new CUIStaticText();
  this->secondLabel->SetSize(100, 12);
  this->secondLabel->Move(32, 51);
  this->secondLabel->SetText("");
  this->secondLabel->SetFitText(0);
  this->secondLabel->SetAlign(0, 0);
  this->secondLabel->SetFont("Gulim");
  this->secondLabel->SetFontHeight(12);
  this->secondLabel->SetBold(0);
  this->secondLabel->SetColor(0);
  this->secondLabel->SetShadow(0, 0xFFFFFF);
  this->secondLabel->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->secondLabel);

  this->secondText = new CUIStaticText();
  this->secondText->SetSize(150, 12);
  this->secondText->Move(110, 51);
  this->secondText->SetText("");
  this->secondText->SetFitText(0);
  this->secondText->SetAlign(0, 0);
  this->secondText->SetFont("Gulim");
  this->secondText->SetFontHeight(12);
  this->secondText->SetBold(0);
  this->secondText->SetColor(0xA0551D);
  this->secondText->SetShadow(0, 0xFFFFFF);
  this->secondText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->secondText);

  this->thirdLabel = new CUIStaticText();
  this->thirdLabel->SetSize(100, 12);
  this->thirdLabel->Move(32, 72);
  this->thirdLabel->SetText("");
  this->thirdLabel->SetFitText(0);
  this->thirdLabel->SetAlign(0, 0);
  this->thirdLabel->SetFont("Gulim");
  this->thirdLabel->SetFontHeight(12);
  this->thirdLabel->SetBold(0);
  this->thirdLabel->SetColor(0);
  this->thirdLabel->SetShadow(0, 0xFFFFFF);
  this->thirdLabel->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->thirdLabel);

  CUIListBoxEx<CUIRewardItem, CRewardItemData>* rewardList = new CUIListBoxEx<CUIRewardItem, CRewardItemData>();
  this->rewardList = rewardList;
  rewardList->itemWidth = 225;
  rewardList->rowHeight = 41;
  rewardList->SetSize(225, 41);
  this->rewardList->Move(110, 72);
  this->rewardList->onSelect = [this](CUIRewardItem* item, const CRewardItemData& data) { this->OnPrepareItem(item, data); };
  this->rewardList->binder = [this](CUIRewardItem* item, const CRewardItemData& data) { this->OnBindItem(item, data); };
  this->rewardList->SetImages("", "", "", "");
  this->rewardList->itemX = 0;
  this->rewardList->itemY = 0;
  this->rewardList->scrollBar->SetSize(7, 41);
  this->rewardList->scrollBar->Move(218, 0);
  this->rewardList->scrollBar->step = 41;
  this->rewardList->scrollBar->SetButtonImages(
    "유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_upper.bmp", "유저인터페이스\\Renew_QuestUI\\bt_scrollbar_arrow_down.bmp"
  );
  this->rewardList->scrollBar->SetBackground("");
  this->rewardList->scrollBar->SetThumbImages(
    "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_up.bmp", "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_mid.bmp",
    "유저인터페이스\\Renew_QuestUI\\bt_scrollBar_down.bmp"
  );
  this->rewardList->AddNewItem();
  this->AddChild(this->rewardList);
}
}
