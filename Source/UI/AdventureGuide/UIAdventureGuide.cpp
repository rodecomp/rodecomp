#include "Contents/AdventureGuideMgr.h"
#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "Render/Renderer.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"

template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUITab>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUISubTab>::UpdateScrollBar();

// OFFSET: 0x009C12B0
// FUNCTION: fn_13987
template <>
void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::ClearItems() {
  for (AdventureGuide::CUIItemInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x009C1310
// FUNCTION: fn_13988
template <>
void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::ClearItems() {
  for (AdventureGuide::CUIMonsterInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x009C1370
// FUNCTION: fn_13989
template <>
void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::ClearItems() {
  for (AdventureGuide::CUIQuestInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x009C13D0
// FUNCTION: fn_13990
template <>
void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::ClearItems() {
  for (AdventureGuide::CUITabInfo* item : this->items) {
    this->RemoveChild(item, 1);
  }

  this->items.clear();
  this->selected = static_cast<unsigned int>(-1);
}

// OFFSET: 0x009C14F0
// FUNCTION: fn_13991
void CUIAdventureGuide::Slot53() {
  this->unknown_00B4 = 0;
  this->Resize(750, 468);
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
  this->Slot15(this->geometry.width, this->geometry.height);
  this->Slot16();
}

namespace AdventureGuide {

// OFFSET: 0x009DC8E0
// FUNCTION: fn_14324
void CUIIntro::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009DC8F0
// FUNCTION: fn_14325
void CUIItem::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009DC900
// FUNCTION: fn_14326
void CUIMonster::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009DC940
// FUNCTION: fn_14328
void CUIQuest::Paint() {
  this->ClearSurface(0xFFFF00FF);
}

// OFFSET: 0x009DCFF0
// FUNCTION: fn_14331
void CUIIntro::Slot16() {
  const std::string& title = g_adventureGuideMgr->unknown_0014;
  const std::map<int, AdventureGuideText>& texts = g_adventureGuideMgr->unknown_002C;
  if (texts.empty()) {
    return;
  }

  for (const auto& text : texts) {
    this->contents->data.emplace_back(text.second.first, text.second.second);
  }

  this->contents->UpdateScrollBar();
  this->image->SetImage(title);
  this->contents->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DD1C0
// FUNCTION: fn_14333
void CUIItem::Slot16() {
}

// OFFSET: 0x009DD6B0
// FUNCTION: fn_14338
void CUIQuest::Slot16() {
}

// OFFSET: 0x009DD1D0
// FUNCTION: fn_14334
void CUIItemInfo::Slot16() {
  this->unknown_00F0->scrollBar->SetHidden(1);
  this->unknown_00F4->scrollBar->SetHidden(1);
  this->unknown_00F8->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DD210
// FUNCTION: fn_14335
void CUIMonster::Slot16() {
  std::list<AdventureGuideEpisode> episodes;
  g_adventureGuideMgr->GetEpisodeList(episodes);
  for (const AdventureGuideEpisode& episode : episodes) {
    CMonsterData data;
    data.name = episode.name;
    data.values = episode.values;
    this->monsterList->data.push_back(data);
  }
  this->monsterList->UpdateScrollBar();
}

// OFFSET: 0x009DD680
// FUNCTION: fn_14337
void CUIMonsterInfo::Slot16() {
  this->unknown_00EC->scrollBar->SetHidden(1);
  this->unknown_00F0->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DD6C0
// FUNCTION: fn_14339
void CUIQuestInfo::Slot16() {
  this->unknown_00EC->scrollBar->SetHidden(1);
  CUIVScrollBar* first = this->unknown_00EC->scrollBar;
  first->dragging = 0;
  first->Invalidate();
  this->unknown_00F0->scrollBar->SetHidden(1);
  CUIVScrollBar* last = this->unknown_00F4->scrollBar;
  last->dragging = 1;
  last->Invalidate();
}

// OFFSET: 0x009DD720
// FUNCTION: fn_14340
void CUISubTab::Slot16() {
  this->unknown_00EC->Show(0);
}

// OFFSET: 0x009DD730
// FUNCTION: fn_14341
void CUITab::Slot16() {
}

// OFFSET: 0x009DD740
// FUNCTION: fn_14342
void CUITabInfo::Slot16() {
}

// OFFSET: 0x009DDA40
// FUNCTION: fn_14347
void CUIIntro::Slot54(int, int) {
}

// OFFSET: 0x009DDE50
// FUNCTION: fn_14350
void CUIMonster::Slot54(int, int) {
  this->unknown_0084->Show(0);
}

}
