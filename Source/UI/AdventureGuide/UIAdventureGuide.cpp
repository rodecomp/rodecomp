#include "Contents/AdventureGuideMgr.h"
#include "Contents/MessageTable.h"
#include "Contents/RenewQuestMgr.h"
#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "Render/Renderer.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "Utility/Text/FormatString.h"

#include <list>
#include <utility>

template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUITab>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUISubTab>::UpdateScrollBar();

// OFFSET: 0x009C28B0
// OFFSET: 0x009C2980
// FUNCTION: fn_13993
// FUNCTION: fn_13994
template <class T> void CUIListBox<T>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) {
    int y = 0;
    for (T* item : this->items) {
      if (position <= item->geometry.height + y && y < this->geometry.height + position) {
        item->Show(1);
        item->Move(this->itemX, this->itemY - position + y);
      } else {
        item->Show(0);
      }
      y += item->geometry.height;
    }
  });
  this->AddChild(this->scrollBar);
}

template void CUIListBox<AdventureGuide::CUISubTab>::Slot15(int, int);
template void CUIListBox<AdventureGuide::CUITab>::Slot15(int, int);

// OFFSET: 0x009C2A50
// OFFSET: 0x009C2B20
// OFFSET: 0x009C2BF0
// OFFSET: 0x009C2CC0
// FUNCTION: fn_13995
// FUNCTION: fn_13996
// FUNCTION: fn_13997
// FUNCTION: fn_13998
template <class T, class D> void CUIListBoxEx<T, D>::Slot15(int width, int height) {
  this->scrollBar = new CUIVScrollBar(*this->m_context);
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

template void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::Slot15(int, int);
template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::Slot15(int, int);
template void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::Slot15(int, int);
template void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::Slot15(int, int);

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

// OFFSET: 0x009DB710
// FUNCTION: fn_14308
void CUIAdventureGuide::OnButtonClick() {
}

// OFFSET: 0x009DBFD0
// FUNCTION: fn_14313
void CRenewQuestMgr::AddStatusRequest(unsigned int id) {
  this->m_statusRequests.insert(id);
}

namespace AdventureGuide {

// OFFSET: 0x009DC0B0
// FUNCTION: fn_14314
void CUITab::OnSelectSubTab(CUISubTab* subTab) {
  if (subTab) {
    this->page->Slot54(this->type, subTab->id);
  }
}

// OFFSET: 0x009DC0E0
// FUNCTION: fn_14315
void CUIIntro::OnSelectContents(CUITabInfo*, const Guide_Intro::Contents&) {
}

// OFFSET: 0x009DC210
// FUNCTION: fn_14318
void CUIItem::OnSelectItem(CUIItemInfo* item, const CItemData&) {
  if (item) {
    this->unknown_008C = item->unknown_0108;
  }
  const bool selected = !this->unknown_008C.empty();
  this->unknown_0084->Show(selected);
  this->unknown_0088->Show(selected);
}

// OFFSET: 0x009DC270
// FUNCTION: fn_14319
void CUIMonster::OnSelectMonster(CUIMonsterInfo* item, const CMonsterData&) {
  if (item) {
    this->unknown_0088 = item->unknown_00E8->unknown_0098;
  }
  this->unknown_0084->Show(!this->unknown_0088.empty());
}

// OFFSET: 0x009DC2C0
// FUNCTION: fn_14320
void CUIQuest::OnSelectQuest(CUIQuestInfo* item, const CQuestInfo&) {
  if (item) {
    this->unknown_008C = item->unknown_00F8;
  }
  const bool selected = !this->unknown_008C.empty();
  this->unknown_0084->Show(selected);
  this->unknown_0088->Show(selected);
}

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

// OFFSET: 0x009DC0F0
// FUNCTION: fn_14316
void CUIIntro::BindContents(CUITabInfo* item, const Guide_Intro::Contents& contents) {
  item->unknown_00E8->SetText(contents.first.c_str(), contents.first.size());
  item->unknown_00EC->SetText(contents.second.c_str(), contents.second.size());
}

// OFFSET: 0x009DFAD0
// FUNCTION: fn_14359
void CUIQuestInfo::SetStatusLines(bool first, bool second, bool third) {
  CUITextViewer* viewer = reinterpret_cast<CUITextViewer*>(this->unknown_00F4);
  viewer->Clear();
  if (first == true) {
    viewer->SetTextColor(0xC86464u);
    viewer->AddLine(FormatOwnedString(LookupMessage(0xFC2)));
  } else {
    viewer->SetTextColor(0x6464C8u);
    viewer->AddLine(FormatOwnedString(LookupMessage(0xFC3)));
  }
  if (second == true) {
    viewer->SetTextColor(0xC86464u);
    viewer->AddLine(FormatOwnedString(LookupMessage(0xFC4)));
  } else {
    viewer->SetTextColor(0x6464C8u);
    viewer->AddLine(FormatOwnedString(LookupMessage(0xFC5)));
  }
  if (third == true) {
    viewer->SetTextColor(0xC86464u);
    viewer->AddLine(FormatOwnedString(LookupMessage(0xFC6)));
  } else {
    viewer->SetTextColor(0x6464C8u);
    viewer->AddLine(FormatOwnedString(LookupMessage(0xFC7)));
  }
}

// OFFSET: 0x009E07B0
// FUNCTION: fn_14362
void CUIQuestInfo::SetViewerLines(const std::vector<std::string>& lines) {
  reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->Clear();
  for (const std::string& line : lines) {
    reinterpret_cast<CUITextViewer*>(this->unknown_00F0)->AddLine(line);
  }
}

// OFFSET: 0x009DDA40
// FUNCTION: fn_14347
void CUIIntro::Slot54(int, int) {
}

// OFFSET: 0x009DDA50
// FUNCTION: fn_14348
void CUIItem::Slot54(int type, int id) {
  this->itemList->ClearData();
  this->location = id;
  std::list<std::pair<int, AdventureGuideEntry>> entries;
  g_adventureGuideMgr->GetEntryList(entries, type, id);
  for (const auto& [key, entry] : entries) {
    CItemData data;
    data.id = key;
    data.first = entry.unknown_0000;
    data.second = entry.unknown_0001;
    data.third = entry.unknown_0002;
    data.unknown_0008 = entry.unknown_0004;
    data.unknown_000C = entry.unknown_0008;
    data.text = entry.text;
    data.values = entry.values;
    data.type = type;
    this->itemList->data.push_back(data);
  }
  this->itemList->UpdateScrollBar();
  this->unknown_0084->Show(0);
  this->unknown_0088->Show(0);
}

// OFFSET: 0x009DDE50
// FUNCTION: fn_14350
void CUIMonster::Slot54(int, int) {
  this->unknown_0084->Show(0);
}

// OFFSET: 0x009DDE60
// FUNCTION: fn_14351
void CUIQuest::Slot54(int type, int id) {
  this->questList->ClearData();
  std::list<std::pair<int, AdventureGuideContents>> contents;
  g_adventureGuideMgr->GetContentsList(contents, type, id);
  for (const auto& [key, content] : contents) {
    CQuestInfo data;
    data.id = key;
    data.unknown_0004 = content.unknown_0000;
    data.unknown_0008 = content.unknown_0004;
    data.unknown_000C = content.unknown_0008;
    data.questId = content.unknown_000C;
    data.title = content.first;
    data.text = content.second;
    data.rewards = content.values;
    data.type = type;
    this->questList->data.push_back(data);
    if (type == 1 && !g_renewQuestMgr->IsNewQuest(content.unknown_000C)) {
      g_renewQuestMgr->AddStatusRequest(content.unknown_000C);
    }
  }
  this->questList->UpdateScrollBar();
  if (type == 1) {
    g_renewQuestMgr->SendQuestStatusRequest();
  }
  this->unknown_0084->Show(0);
  this->unknown_0088->Show(0);
}


// OFFSET: 0x009DE410
// FUNCTION: fn_14353
void CUITab::SetSelected(bool selected) {
  CUIListItem::SetSelected(selected);
  const bool expand = selected && this->subTabs && !this->subTabs->geometry.visible;
  if (this->subTabs) {
    this->subTabs->Show(expand);
  }
  this->Resize(this->geometry.width, this->unknown_0110 + (expand ? this->subTabs->geometry.height : 0));
  CUIListBox<CUITab>* list = dynamic_cast<CUIListBox<CUITab>*>(this->parent);
  if (list) {
    list->UpdateScrollBar();
  }
  if (this->page) {
    this->page->Show(selected);
  }
}

}

// OFFSET: 0x009DE4C0
// FUNCTION: fn_14354
template <>
void CUIListBox<AdventureGuide::CUISubTab>::SelectItem(CUIListItem* item) {
  if (this->locked) {
    return;
  }
  for (AdventureGuide::CUISubTab* entry : this->items) {
    if (entry != item) {
      entry->SetSelected(false);
    }
  }
  item->SetSelected(true);
  if (this->selected != item) {
    AdventureGuide::CUISubTab* selected = dynamic_cast<AdventureGuide::CUISubTab*>(item);
    if (selected) {
      this->selected = selected;
      if (this->onSelect) {
        this->onSelect(selected);
      }
    }
  }
}

// OFFSET: 0x009DE560
// FUNCTION: fn_14355
template <>
void CUIListBox<AdventureGuide::CUITab>::SelectItem(CUIListItem* item) {
  if (this->locked) {
    return;
  }
  for (AdventureGuide::CUITab* entry : this->items) {
    if (entry != item) {
      entry->SetSelected(false);
    }
  }
  item->SetSelected(true);
  if (this->selected != item) {
    AdventureGuide::CUITab* selected = dynamic_cast<AdventureGuide::CUITab*>(item);
    if (selected) {
      this->selected = selected;
      if (this->onSelect) {
        this->onSelect(selected);
      }
    }
  }
}

// OFFSET: 0x009DEBF0
// FUNCTION: fn_14357
void CUIAdventureGuide::MarkQuest(int questId) {
  for (AdventureGuide::CUITab* tab : this->tabs->items) {
    if (tab->type != 1) {
      continue;
    }

    if (tab->subTabs) {
      std::list<AdventureGuide::CUISubTab*> subTabs = tab->subTabs->items;
      auto found = std::find_if(subTabs.begin(), subTabs.end(), [questId](AdventureGuide::CUISubTab* subTab) {
        return subTab->unknown_00F4 == static_cast<unsigned int>(questId);
      });
      if (found != subTabs.end()) {
        (*found)->unknown_00EC->Show(1);
      }
    }

    AdventureGuide::CUIQuest* quest = dynamic_cast<AdventureGuide::CUIQuest*>(tab->page);
    if (quest) {
      std::vector<AdventureGuide::CUIQuestInfo*> items = quest->questList->items;
      auto found = std::find_if(items.begin(), items.end(), [questId](AdventureGuide::CUIQuestInfo* item) {
        return item->questId == questId;
      });
      if (found != items.end()) {
        (*found)->unknown_00E8->Show(1);
      }
    }
  }
}
