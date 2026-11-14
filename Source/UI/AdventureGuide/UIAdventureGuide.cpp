#include "Client/Session/Session.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/AdventureGuide/AdventureGuideMgr.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Navigation/NavigationManager.h"
#include "Contents/Quest/RenewQuestMgr.h"
#include "Contents/Tables/TipBoxTitles.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/AdventureGuide/UIAdventureGuide.h"
#include "UI/Lists/UIListBoxTemplate.h"
#include "Render/Renderer.h"
#include "UI/Lists/UIVScrollBar.h"
#include "UI/Text/UIStaticTextControl.h"
#include "UI/Text/UITextViewerControl.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Text/StringUtil.h"
#include "Contents/Items/ItemDescription.h"
#include "UI/Window/UIRegisterMgr.h"
#include "Resource/MSGSTRINGID.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include <cstring>
#include <format>
#include <list>
#include <regex>
#include <utility>

template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUITab>::UpdateScrollBar();
template void CUIListBox<AdventureGuide::CUISubTab>::UpdateScrollBar();
template <> __declspec(noinline) void CUIListBox<AdventureGuide::CUISubTab>::SelectItem(CUIListItem* item);
template <> AdventureGuide::CUISubTab* CUIListBox<AdventureGuide::CUISubTab>::CreateItem();
template <> AdventureGuide::CUITab* CUIListBox<AdventureGuide::CUITab>::CreateItem();
template <> void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::AddNewItem();
template <> void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::AddNewItem();
template <> void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::AddNewItem();
template <> void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::AddNewItem();

// OFFSET: 0x009C28B0
// FUNCTION: fn_13993
// OFFSET: 0x009C2980
// FUNCTION: fn_13994
template <class T> void CUIListBox<T>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
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

template void CUIListBox<AdventureGuide::CUISubTab>::OnCreate(int, int);
template void CUIListBox<AdventureGuide::CUITab>::OnCreate(int, int);

// OFFSET: 0x009C2A50
// FUNCTION: fn_13995
// OFFSET: 0x009C2B20
// FUNCTION: fn_13996
// OFFSET: 0x009C2BF0
// FUNCTION: fn_13997
// OFFSET: 0x009C2CC0
// FUNCTION: fn_13998
template <class T, class D> void CUIListBoxEx<T, D>::OnCreate(int width, int height) {
  this->scrollBar = new CUIVScrollBar();
  this->scrollBar->Show(0);
  this->scrollBar->SetOnChange([this](int position) { this->OnScroll(position); });
  this->AddChild(this->scrollBar);
}

template void CUIListBoxEx<AdventureGuide::CUIItemInfo, AdventureGuide::CItemData>::OnCreate(int, int);
template void CUIListBoxEx<AdventureGuide::CUIMonsterInfo, AdventureGuide::CMonsterData>::OnCreate(int, int);
template void CUIListBoxEx<AdventureGuide::CUIQuestInfo, AdventureGuide::CQuestInfo>::OnCreate(int, int);
template void CUIListBoxEx<AdventureGuide::CUITabInfo, Guide_Intro::Contents>::OnCreate(int, int);

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

// OFFSET: 0x009C2D90
// FUNCTION: fn_13999
void CUIAdventureGuide::OnCreate(int width, int height) {
  this->background = new CUIStaticImage();
  this->background->SetSize(750, 468);
  this->background->Move(0, 0);
  {
    CUIStaticImage* background = this->background;
    background->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\bg_guide.bmp", 0xB3CF9BEB7963DB89ULL)));
  }
  this->AddChild(this->background);

  this->titleBar = new CUITitleBar();
  this->titleBar->SetSize(750, 17);
  this->titleBar->Move(0, 0);
  {
    CUITitleBar* titleBar = this->titleBar;
    titleBar->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\titlebar_left.bmp", 0x1FDB81175D1557E3ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\titlebar_mid.bmp", 0xE3FB23537D2785BFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\titlebar_right.bmp", 0xB7A76BF9CD41F99FULL)));
  }
  this->AddChild(this->titleBar);

  CUIStaticText* title = new CUIStaticText();
  title->SetSize(100, 17);
  title->Move(18, 0);
  title->SetText("");
  title->SetFitText(0);
  title->SetAlign(0, 6);
  {
    title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x732B390D63FD47D7ULL)));
  }
  title->SetFontHeight(12);
  title->SetBold(0);
  title->SetColor(0);
  title->SetShadow(1, 0xFFFFFF);
  title->SetOutline(0, 0xFFFFFF);
  this->titleBar->titleText = title;
  this->titleBar->AddChild(title);

  CUIImageButton* menuButton = new CUIImageButton();
  menuButton->SetSize(11, 11);
  menuButton->Move(3, 3);
  menuButton->SetOnClick([this]() { this->OnButtonClick(); });
  menuButton->disabled = 0;
  menuButton->Invalidate();
  {
    menuButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0x097389157345A787ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0x5F93A1C3A7AD7537ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_base_on.bmp", 0x610F7DD1EFC1C91BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_base_off.bmp", 0xC5995F4B9FE96B89ULL)));
  }
  this->titleBar->AddChild(menuButton);

  CUIImageButton* closeButton = new CUIImageButton();
  closeButton->SetSize(11, 11);
  closeButton->Move(736, 3);
  closeButton->SetOnClick([this]() { this->OnClose(); });
  closeButton->disabled = 0;
  closeButton->Invalidate();
  {
    closeButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0x6BF729E30729CF29ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0x3D371F5B0F7F9327ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_on.bmp", 0x1F1D51274373C1FDULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\basic_interface\\sys_close_off.bmp", 0x5DAD2D25D9259B93ULL)));
  }
  this->titleBar->SetSecondButton(closeButton);

  this->characterInfo = new AdventureGuide::CUICharacterInfo();
  this->characterInfo->SetSize(161, 41);
  this->characterInfo->Move(1, 19);
  this->AddChild(this->characterInfo);

  this->titleText = new CUIStaticText();
  this->titleText->SetSize(104, 20);
  this->titleText->Move(362, 28);
  this->titleText->SetText("");
  this->titleText->SetFitText(0);
  this->titleText->SetAlign(6, 6);
  {
    CUIStaticText* titleText = this->titleText;
    titleText->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x9D8F2D37BFFD61B7ULL)));
  }
  this->titleText->SetFontHeight(14);
  this->titleText->SetBold(1);
  this->titleText->SetColor(0x89261B);
  this->titleText->SetShadow(0, 0xFFFFFF);
  this->titleText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->titleText);

  this->intro = new AdventureGuide::CUIIntro();
  this->intro->SetSize(637, 373);
  this->intro->Move(112, 63);
  this->AddChild(this->intro);

  this->quest = new AdventureGuide::CUIQuest();
  this->quest->SetSize(627, 387);
  this->quest->Move(122, 81);
  this->AddChild(this->quest);

  this->monster = new AdventureGuide::CUIMonster();
  this->monster->SetSize(627, 387);
  this->monster->Move(122, 81);
  this->AddChild(this->monster);

  this->item = new AdventureGuide::CUIItem();
  this->item->SetSize(627, 387);
  this->item->Move(122, 81);
  this->AddChild(this->item);

  this->tabs = new CUIListBox<AdventureGuide::CUITab>();
  this->tabs->SetSize(106, 372);
  this->tabs->Move(1, 63);
  this->tabs->SetOnSelect([this](AdventureGuide::CUITab* tab) { this->OnSelectTab(tab); });
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    tabs->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\tab_btn_normal.bmp", 0x79DF094DD9616575ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\tab_btn_press.bmp", 0x11CB35636377DD07ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\tab_btn_over.bmp", 0x4755C1355575FD37ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\tab_btn_normal.bmp", 0x9BE3EF737387AD79ULL)));
  }

  this->tabs->itemWidth = 98;
  this->tabs->rowHeight = 40;
  this->tabs->itemX = 8;
  this->tabs->itemY = 0;
  this->tabs->scrollBar->SetSize(8, 372);
  this->tabs->scrollBar->Move(0, 0);
  this->tabs->scrollBar->step = 20;
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    tabs->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0xB96B77B7255F8569ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0x875FCDF1E7A1795FULL)));
  }
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    tabs->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0x072D2D872D259D5BULL)));
  }
  {
    CUIListBox<AdventureGuide::CUITab>* tabs = this->tabs;
    tabs->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0xF535E561137F138DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0xD3E5516F89F161D1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0x5FFD7FABB5D1EF57ULL)));
  }
  this->AddChild(this->tabs);
}

// OFFSET: 0x009DBF20
// FUNCTION: fn_45467
void CUIAdventureGuide::OnClose() {
  g_windowMgr.CloseWindow(WID_ADVENTUREGUIDE);
}

// OFFSET: 0x009C14F0
// FUNCTION: fn_13991
void CUIAdventureGuide::Initialize() {
  this->m_ownsBackgroundColor = false;
  this->m_ownsBorderColor = false;
  this->Resize(750, 468);
  this->Move((g_renderer->viewport.width - this->geometry.width) / 2, (g_renderer->viewport.height - this->geometry.height) / 2);
  this->OnCreate(this->geometry.width, this->geometry.height);
  this->OnInit();
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
    this->page->LoadPage(this->type, subTab->id);
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
    this->navigationTarget = item->description;
  }
  const bool selected = !this->navigationTarget.empty();
  this->findButton->Show(selected);
  this->navigationButton->Show(selected);
}

// OFFSET: 0x009DC270
// FUNCTION: fn_14319
void CUIMonster::OnSelectMonster(CUIMonsterInfo* item, const CMonsterData&) {
  if (item) {
    this->navigationTarget = item->nameText->displayText;
  }
  this->navigationButton->Show(!this->navigationTarget.empty());
}

// OFFSET: 0x009DC2C0
// FUNCTION: fn_14320
void CUIQuest::OnSelectQuest(CUIQuestInfo* item, const CQuestInfo&) {
  if (item) {
    this->navigationTarget = item->title;
  }
  const bool selected = !this->navigationTarget.empty();
  this->findButton->Show(selected);
  this->navigationButton->Show(selected);
}

// OFFSET: 0x009DC320
// FUNCTION: fn_14321
void CUIItem::BindItem(CUIItemInfo* item, const CItemData& data) {
  ITEM_INFO info;
  info.SetItemId(data.id);
  item->location = this->location;
  info.identified = 1;
  item->itemId = data.id;
  const ITEM_DESC& desc = FindItemDescription(std::atoi(info.itemId.c_str()));
  const std::string image = "유저인터페이스\\collection\\" + std::string(desc.identifiedResource) + ".bmp";
  item->nameText->SetText(std::string(desc.identifiedName));
  item->image->SetImage(image);
  item->SetStatusLines(data.first, data.second, data.third);
  item->SetDescription(data.type, info.Description());
  item->sourcesViewer->Clear();
  for (const std::string& value : data.values) {
    item->sourcesViewer->AddLine(value);
  }
  item->description = data.text;
}

// OFFSET: 0x009DC770
// FUNCTION: fn_14323
void __stdcall BindQuestInfo(CUIQuestInfo* owner, const CQuestInfo& data) {
  switch (data.type) {
  case 1:
    owner->SetTitle(data.id);
    owner->SetViewerLines(data.rewards);
    owner->title = data.title;
    owner->statusViewer->Clear();
    owner->statusViewer->AddLine(data.text);
    owner->questId = data.questId;
    {
      const bool isNew = g_renewQuestMgr->IsNewQuest(data.questId);
      owner->stampImage->Show(isNew);
    }
    break;
  case 3:
    owner->SetTitle(data.id);
    owner->SetViewerLines(data.rewards);
    owner->title = data.title;
    owner->statusViewer->Clear();
    owner->statusViewer->AddLine(data.text);
    owner->stampImage->Show(0);
    return;
  case 4:
    owner->SetTip(data.id);
    owner->SetViewerLines(data.rewards);
    owner->title = data.title;
    owner->statusViewer->Clear();
    owner->statusViewer->AddLine(data.text);
    owner->stampImage->Show(0);
    return;
  }
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
void CUIIntro::OnInit() {
  const AdventureGuideIntroPage& intro = g_adventureGuideMgr->introPage;
  if (intro.texts.empty()) {
    return;
  }

  for (const auto& [id, text] : intro.texts) {
    this->contents->data.emplace_back(text.first, text.second);
  }

  this->contents->UpdateScrollBar();
  this->image->SetImage(intro.title);
  this->contents->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DD1C0
// FUNCTION: fn_14333
void CUIItem::OnInit() {
}

// OFFSET: 0x009DD6B0
// FUNCTION: fn_14338
void CUIQuest::OnInit() {
}

// OFFSET: 0x009DD1D0
// FUNCTION: fn_14334
void CUIItemInfo::OnInit() {
  this->descriptionViewer->scrollBar->SetHidden(1);
  this->optionsViewer->scrollBar->SetHidden(1);
  this->sourcesViewer->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DEF40
// FUNCTION: fn_14358
void CUIItemInfo::SetDescription(int type, const std::vector<const char*>& description) {
  this->descriptionViewer->Clear();

  if (type == 5) {
    this->type = 5;
  } else if (type == 6) {
    this->type = 4;
  }

  int lines = 0;
  for (const char* value : description) {
    if (lines > 6) {
      break;
    }

    std::string text(value);
    constexpr char highlight[] = {'^', '7', '7', '7', '7', '7', '7'};
    if (text.find(highlight, 0, sizeof(highlight)) == std::string::npos) {
      continue;
    }

    std::cmatch match;
    std::regex pattern("\\^000000");
    std::regex_search(text.c_str(), text.c_str() + text.size(), match, pattern);
    while (!match.empty()) {
      std::string line = match.prefix().str() + match[0].str();
      this->descriptionViewer->AddLine(line);
      ++lines;
      text = match.suffix().str();
      text.erase(0, text.find_first_not_of(" \r\n\t\v\f"));
      std::regex_search(text.c_str(), text.c_str() + text.size(), match, pattern);
    }
  }
}

// OFFSET: 0x009DD210
// FUNCTION: fn_14335
void CUIMonster::OnInit() {
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
void CUIMonsterInfo::OnInit() {
  this->detailsViewer->scrollBar->SetHidden(1);
  this->valuesViewer->scrollBar->SetHidden(1);
}

// OFFSET: 0x009DD6C0
// FUNCTION: fn_14339
void CUIQuestInfo::OnInit() {
  this->titleViewer->scrollBar->SetHidden(1);
  CUIVScrollBar* first = this->titleViewer->scrollBar;
  first->dragging = 0;
  first->Invalidate();
  this->linesViewer->scrollBar->SetHidden(1);
  CUIVScrollBar* last = this->statusViewer->scrollBar;
  last->dragging = 1;
  last->Invalidate();
}

// OFFSET: 0x009DD720
// FUNCTION: fn_14340
void CUISubTab::OnInit() {
  this->stampImage->Show(0);
}

// OFFSET: 0x009DD730
// FUNCTION: fn_14341
void CUITab::OnInit() {
}

// OFFSET: 0x009DD740
// FUNCTION: fn_14342
void CUITabInfo::OnInit() {
}

// OFFSET: 0x009DC0F0
// FUNCTION: fn_14316
void CUIIntro::BindContents(CUITabInfo* item, const Guide_Intro::Contents& contents) {
  item->titleText->SetText(contents.first);
  item->descriptionText->SetText(contents.second);
}

// OFFSET: 0x009E0350
// FUNCTION: fn_14361
void CUIQuestInfo::SetTitle(int id) {
  this->titleViewer->Clear();

  const TipBoxEntry& entry = g_itemTagOwner->GetTip(id);
  if (entry.title.empty() || entry.paragraphs.empty()) {
    this->titleViewer->AddLine(std::to_string(id));
    return;
  }

  std::string text = entry.title;
  std::cmatch match;
  std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("Q(.+)\\("));
  const std::string whitespace(" \r\n\t\v\f");
  if (match.size() > 1) {
    text = match[1].str();
  }

  text.erase(0, text.find_first_not_of(whitespace));
  text.erase(text.find_last_not_of(whitespace) + 1);
  this->titleViewer->SetBold(1);
  this->titleViewer->AddLine(text);

  text = entry.paragraphs[0];
  std::regex_search(text.c_str(), text.c_str() + text.size(), match, std::regex("\n"));
  if (match.size() > 0) {
    text = match.prefix().str();
  }

  text.erase(0, text.find_first_not_of(whitespace));
  text.erase(text.find_last_not_of(whitespace) + 1);
  this->titleViewer->SetBold(0);
  this->titleViewer->AddLine(text);
}

// OFFSET: 0x009DFAD0
// FUNCTION: fn_14359
void CUIItemInfo::SetStatusLines(bool first, bool second, bool third) {
  this->optionsViewer->Clear();
  if (first == true) {
    this->optionsViewer->SetTextColor(0xC86464u);
    this->optionsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_ENCHANT_IMPOSSIBLE)));
  } else {
    this->optionsViewer->SetTextColor(0x6464C8u);
    this->optionsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_GRADE_ENCHANT_POSSIBLE)));
  }
  if (second == true) {
    this->optionsViewer->SetTextColor(0xC86464u);
    this->optionsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_GRADE_ENCHANT_IMPOSSIBLE)));
  } else {
    this->optionsViewer->SetTextColor(0x6464C8u);
    this->optionsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_RUNESYSTEM_EQUIP_OUT)));
  }
  if (third == true) {
    this->optionsViewer->SetTextColor(0xC86464u);
    this->optionsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_RUNESYSTEM_EQUIP_NOTPUT)));
  } else {
    this->optionsViewer->SetTextColor(0x6464C8u);
    this->optionsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_RUNESYSTEM_RUNESET_TITLE)));
  }
}

// OFFSET: 0x009E07B0
// FUNCTION: fn_14362
void CUIQuestInfo::SetViewerLines(const std::vector<std::string>& lines) {
  this->linesViewer->Clear();
  for (const std::string& line : lines) {
    this->linesViewer->AddLine(line);
  }
}

// OFFSET: 0x009DDA40
// FUNCTION: fn_14347
void CUIIntro::LoadPage(int, int) {
}

// OFFSET: 0x009DDA50
// FUNCTION: fn_14348
void CUIItem::LoadPage(int type, int id) {
  this->itemList->ClearData();
  this->location = id;
  std::list<std::pair<int, AdventureGuideEntry>> entries;
  g_adventureGuideMgr->GetEntryList(entries, type, id);
  for (const auto& [key, entry] : entries) {
    CItemData data;
    data.id = key;
    data.first = entry.firstFlag;
    data.second = entry.secondFlag;
    data.third = entry.thirdFlag;
    data.categoryId = entry.minLevel;
    data.categoryIndex = entry.maxLevel;
    data.text = entry.text;
    data.values = entry.values;
    data.type = type;
    this->itemList->data.push_back(data);
  }
  this->itemList->UpdateScrollBar();
  this->findButton->Show(0);
  this->navigationButton->Show(0);
}

// OFFSET: 0x009DDE50
// FUNCTION: fn_14350
void CUIMonster::LoadPage(int, int) {
  this->navigationButton->Show(0);
}

// OFFSET: 0x009DDE60
// FUNCTION: fn_14351
void CUIQuest::LoadPage(int type, int id) {
  this->questList->ClearData();
  std::list<std::pair<int, AdventureGuideContents>> contents;
  g_adventureGuideMgr->GetContentsList(contents, type, id);
  for (const auto& [key, content] : contents) {
    CQuestInfo data;
    data.id = key;
    data.categoryType = content.flag;
    data.categoryId = content.minLevel;
    data.categoryIndex = content.maxLevel;
    data.questId = content.questId;
    data.title = content.first;
    data.text = content.second;
    data.rewards = content.values;
    data.type = type;
    this->questList->data.push_back(data);
    if (type == 1 && !g_renewQuestMgr->IsNewQuest(content.questId)) {
      g_renewQuestMgr->AddStatusRequest(content.questId);
    }
  }
  this->questList->UpdateScrollBar();
  if (type == 1) {
    g_renewQuestMgr->SendQuestStatusRequest();
  }
  this->findButton->Show(0);
  this->navigationButton->Show(0);
}

// OFFSET: 0x009DE410
// FUNCTION: fn_14353
void CUITab::SetSelected(bool selected) {
  CUIListItem::SetSelected(selected);
  const bool expand = selected && this->subTabs && !this->subTabs->geometry.visible;
  if (this->subTabs) {
    this->subTabs->Show(expand);
  }
  this->Resize(this->geometry.width, this->collapsedHeight + (expand ? this->subTabs->geometry.height : 0));
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
    if (item != entry) {
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
    if (item != entry) {
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

// OFFSET: 0x009DC950
// FUNCTION: fn_14329
void CUIAdventureGuide::OnInit() {
  this->titleBar->SetTitle(LookupMessage(MSI_RUNESYSTEM_CLOSE_RODEX));

  CAdventureGuideMgr* mgr = g_adventureGuideMgr;
  if (mgr->m_intros.empty()) {
    return;
  }

  std::map<int, AdventureGuide::CUIPage*> pages = {
      {0, this->intro},
      {1, this->quest},
      {2, this->monster},
      {3, this->quest},
      {4, this->quest},
      {5, this->item},
      {6, this->item},
  };

  const int level = g_session.m_level;
  for (const auto& [type, intro] : mgr->m_intros) {
    AdventureGuide::CUITab* tab = this->tabs->CreateItem();
    tab->type = type;
    tab->labelText->SetText(intro.text);
    tab->title = intro.title;
    tab->collapsedHeight = this->tabs->rowHeight;

    auto page = pages.find(static_cast<int>(tab->type));
    if (pages.end() != page) {
      tab->page = page->second;
    }

    if (!intro.details.empty()) {
      std::map<int, AdventureGuideDetail> details;
      std::copy_if(intro.details.begin(), intro.details.end(), std::inserter(details, details.end()),
                   [type, level](const std::pair<const int, AdventureGuideDetail>& entry) {
                     return g_adventureGuideMgr->IsAvailable(type, entry.first);
                   });
      if (details.empty()) {
        continue;
      }
      CUIListBox<AdventureGuide::CUISubTab>* subTabs = tab->CreateSubTabList();
      subTabs->Resize(subTabs->geometry.width, subTabs->rowHeight * static_cast<int>(details.size()));
      for (const auto& [id, detail] : details) {
        AdventureGuide::CUISubTab* subTab = subTabs->CreateItem();
        subTab->labelText->SetText(detail.text);
        subTab->id = id;
        subTab->questId = detail.questId;
        if (tab->type == 1) {
          const bool isNew = g_renewQuestMgr->IsNewQuest(detail.questId);
          subTab->stampImage->Show(isNew);
          if (!isNew) {
            g_renewQuestMgr->AddStatusRequest(detail.questId);
          }
        }
      }
      tab->Resize(tab->geometry.width, this->tabs->rowHeight + subTabs->geometry.height);
    }
  }

  if (!this->tabs->items.empty()) {
    AdventureGuide::CUITab* first = this->tabs->items.front();
    if (first) {
      this->tabs->SelectItem(first);
    }
  }
  g_renewQuestMgr->SendQuestStatusRequest();
}

// OFFSET: 0x009DE7D0
// FUNCTION: fn_14356
void AdventureGuide::CUIQuestInfo::SetTip(int id) {
  this->titleViewer->Clear();

  const TipBoxEntry& tip = g_itemTagOwner->GetTip(id);
  if (tip.title.empty() || tip.paragraphs.empty()) {
    this->titleViewer->AddLine(std::to_string(id));
    return;
  }

  std::string title = tip.title;
  std::match_results<const char*> match;
  std::regex_search(title.c_str(), title.c_str() + title.size(), match, std::regex("\\#"));
  const std::string whitespace(" \r\n\t\v\f");
  if (!match.empty()) {
    title = match.prefix().str();
  }

  title.erase(0, title.find_first_not_of(whitespace));
  title.erase(title.find_last_not_of(whitespace) + 1);
  this->titleViewer->SetBold(true);
  this->titleViewer->AddLine(title);

  title = tip.paragraphs.front();
  std::regex_search(title.c_str(), title.c_str() + title.size(), match, std::regex("\n"));
  if (!match.empty()) {
    title = match.prefix().str();
  }
  title.erase(0, title.find_first_not_of(whitespace));
  title.erase(title.find_last_not_of(whitespace) + 1);
  this->titleViewer->SetBold(false);
  this->titleViewer->AddLine(title);
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
        return subTab->questId == static_cast<unsigned int>(questId);
      });
      if (found != subTabs.end()) {
        (*found)->stampImage->Show(1);
      }
    }

    AdventureGuide::CUIQuest* quest = dynamic_cast<AdventureGuide::CUIQuest*>(tab->page);
    if (quest) {
      std::vector<AdventureGuide::CUIQuestInfo*> items = quest->questList->items;
      auto found = std::find_if(items.begin(), items.end(), [questId](AdventureGuide::CUIQuestInfo* item) {
        return item->questId == questId;
      });
      if (found != items.end()) {
        (*found)->stampImage->Show(1);
      }
    }
  }
}

// OFFSET: 0x009DC140
// FUNCTION: fn_14317
void CUIAdventureGuide::OnSelectTab(AdventureGuide::CUITab* tab) {
  this->titleText->SetText(tab->title);
  AdventureGuide::CUIPage* page = tab->page;
  if (!page) {
    return;
  }

  const int type = tab->type;
  CUIListBox<AdventureGuide::CUISubTab>* subTabs = tab->subTabs;
  if (subTabs) {
    if (!subTabs->items.empty()) {
      AdventureGuide::CUISubTab* first = subTabs->items.front();
      if (first) {
        subTabs->SelectItem(first);
        if (first != subTabs->selected) {
          return;
        }
      }
    }

    if (subTabs->selected) {
      page->LoadPage(type, subTabs->selected->id);
    } else {
      page->LoadPage(type, 0);
    }
  } else {
    page->LoadPage(type, 0);
  }
}

namespace AdventureGuide {

// OFFSET: 0x009D8F50
// FUNCTION: fn_14273
__declspec(noinline) CUICharacterInfo::CUICharacterInfo() : CUIControl(), jobIcon(nullptr), level(nullptr), jobLevel(nullptr), name(nullptr) {
}

// OFFSET: 0x009D8FE0
// FUNCTION: fn_14274
__declspec(noinline) CUIIntro::CUIIntro() : CUIPage(), banner(nullptr), image(nullptr), contents(nullptr) {
}

// OFFSET: 0x009C5400
// FUNCTION: fn_14000
void CUICharacterInfo::OnCreate(int width, int height) {
  this->jobIcon = new CUIStaticImage();
  this->jobIcon->SetSize(25, 25);
  this->jobIcon->Move(10, 8);
  this->jobIcon->SetImage(std::string(""));
  this->AddChild(this->jobIcon);

  this->level = new CUIStaticText();
  this->level->SetSize(50, 12);
  this->level->Move(46, 6);
  this->level->SetText("");
  this->level->SetFitText(0);
  this->level->SetAlign(0, 0);
  // GLOBAL: gl_25126
  this->level->SetFont(std::string(OBFUSCATE_SOURCE_TEXT_NAMED("Gulim", 0x758F1D0B4F7705B9ULL, levelFont)));
  this->level->SetFontHeight(12);
  this->level->SetBold(0);
  this->level->SetColor(0);
  this->level->SetShadow(1, 0xFFFFFF);
  this->level->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->level);

  this->jobLevel = new CUIStaticText();
  this->jobLevel->SetSize(50, 12);
  this->jobLevel->Move(97, 6);
  this->jobLevel->SetText("");
  this->jobLevel->SetFitText(0);
  this->jobLevel->SetAlign(0, 0);
  // GLOBAL: gl_25127
  this->jobLevel->SetFont(std::string(OBFUSCATE_SOURCE_TEXT_NAMED("Gulim", 0x17A9E5BF39AD4737ULL, jobLevelFont)));
  this->jobLevel->SetFontHeight(12);
  this->jobLevel->SetBold(0);
  this->jobLevel->SetColor(0);
  this->jobLevel->SetShadow(1, 0xFFFFFF);
  this->jobLevel->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->jobLevel);

  this->name = new CUIStaticText();
  this->name->SetSize(100, 12);
  this->name->Move(45, 22);
  this->name->SetText("");
  this->name->SetFitText(1);
  this->name->SetAlign(0, 0);
  // GLOBAL: gl_25128
  this->name->SetFont(std::string(OBFUSCATE_SOURCE_TEXT_NAMED("Gulim", 0x13E33FA1A591AB67ULL, nameFont)));
  this->name->SetFontHeight(12);
  this->name->SetBold(0);
  this->name->SetColor(0);
  this->name->SetShadow(1, 0xFFFFFF);
  this->name->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->name);
}

// OFFSET: 0x009C8D00
// FUNCTION: fn_14003
void CUIItemInfo::OnCreate(int width, int height) {
  this->image = new CUIStaticImage();
  this->image->SetSize(75, 100);
  this->image->Move(45, 14);
  this->image->SetImage(std::string(""));
  this->AddChild(this->image);

  this->nameText = new CUIStaticText();
  this->nameText->SetSize(280, 12);
  this->nameText->Move(165, 13);
  this->nameText->SetText("");
  this->nameText->SetFitText(0);
  this->nameText->SetAlign(0, 0);
  {
    CUIStaticText* name = this->nameText;
    name->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x192D654D29337BE5ULL)));
  }
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(1);
  this->nameText->SetColor(0xCE6B52);
  this->nameText->SetShadow(0, 0xFFFFFF);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->nameText);

  CUITextViewer* description = new CUITextViewer();
  this->descriptionViewer = description;
  description->SetSize(173, 97);
  description->Move(165, 26);
  description->SetTextOffset(0, 6);
  {
    description->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xB3E307A5F753D5E3ULL)));
  }
  description->SetFontHeight(12);
  description->SetBold(0);
  description->SetTextColor(0);
  description->SetOutline(0, 0xFFFF00FF);
  description->SetLineSpacing(6);
  description->SetBackgroundColor(0xFFFF00FF);
  description->SetPadding(0, 0, 0, 0);
  description->SetScrollBarSize(8, 97);
  description->SetScrollStep(20);
  {
    description->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0xDB27CB557D038759ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0x3FA3AFA7035DE3FFULL)));
  }
  {
    description->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0x87E3D1C50D572DEFULL)));
  }
  {
    description->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0x51BB39B763BBBD8DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0xADC51BF9C98B413DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0xDF8117FF49CFF551ULL)));
  }
  this->AddChild(description);

  CUITextViewer* options = new CUITextViewer();
  this->optionsViewer = options;
  options->SetSize(115, 90);
  options->Move(338, 26);
  options->SetTextOffset(0, 8);
  {
    options->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x114901C7E37F6D49ULL)));
  }
  options->SetFontHeight(12);
  options->SetBold(0);
  options->SetTextColor(0);
  options->SetOutline(0, 0xFFFF00FF);
  options->SetLineSpacing(6);
  options->SetBackgroundColor(0xFFFF00FF);
  options->SetPadding(0, 0, 0, 0);
  options->SetScrollBarSize(8, 90);
  options->SetScrollStep(20);
  {
    options->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0x7D5D0F110B6B8371ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0xF715514F5961B75BULL)));
  }
  {
    options->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0x4191BB5531FD3999ULL)));
  }
  {
    options->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0xB19D1FA9116F39D3ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0x9569C9C7E70909AFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0xD7A749EB27D5BF6DULL)));
  }
  this->AddChild(options);

  CUITextViewer* sources = new CUITextViewer();
  this->sourcesViewer = sources;
  sources->SetSize(108, 118);
  sources->Move(480, 5);
  sources->SetTextOffset(0, 6);
  {
    sources->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xB195637DAFCB0D59ULL)));
  }
  sources->SetFontHeight(12);
  sources->SetBold(0);
  sources->SetTextColor(0);
  sources->SetOutline(0, 0xFFFF00FF);
  sources->SetLineSpacing(6);
  sources->SetBackgroundColor(0xFFFF00FF);
  sources->SetPadding(0, 0, 0, 0);
  sources->SetScrollBarSize(8, 118);
  sources->SetScrollStep(20);
  {
    sources->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0xADADEB4DF7813B55ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0xD9452D81E5CD99C1ULL)));
  }
  {
    sources->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0xFD69A9B3F311818DULL)));
  }
  {
    sources->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0x7F2DFFC1ABE91F2BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0xD98F5345098D630DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0x9B0F234DEFDF7593ULL)));
  }
  this->AddChild(sources);
}

// OFFSET: 0x009CC580
// FUNCTION: fn_14005
void CUIMonsterInfo::OnCreate(int width, int height) {
  this->nameText = new CUIStaticText();
  this->nameText->SetSize(280, 12);
  this->nameText->Move(165, 19);
  this->nameText->SetText("");
  this->nameText->SetFitText(0);
  this->nameText->SetAlign(0, 0);
  {
    CUIStaticText* name = this->nameText;
    name->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xEF6D1D893F2B2DC5ULL)));
  }
  this->nameText->SetFontHeight(12);
  this->nameText->SetBold(0);
  this->nameText->SetColor(0xCE6B52);
  this->nameText->SetShadow(1, 0xDEDEDE);
  this->nameText->SetOutline(0, 0xFFFFFF);
  this->AddChild(this->nameText);

  this->detailsViewer = new CUITextViewer();
  this->detailsViewer->SetSize(288, 84);
  this->detailsViewer->Move(165, 39);
  this->detailsViewer->SetTextOffset(0, 6);
  {
    CUITextViewer* details = this->detailsViewer;
    details->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x39A39F87DD5F535BULL)));
  }
  this->detailsViewer->SetFontHeight(12);
  this->detailsViewer->SetBold(0);
  this->detailsViewer->SetTextColor(0);
  this->detailsViewer->SetOutline(0, 0xFFFF00FF);
  this->detailsViewer->SetLineSpacing(6);
  this->detailsViewer->SetBackgroundColor(0xFFFF00FF);
  this->detailsViewer->SetPadding(0, 0, 0, 0);
  this->detailsViewer->SetScrollBarSize(8, 84);
  this->detailsViewer->SetScrollStep(20);
  {
    CUITextViewer* details = this->detailsViewer;
    details->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0x6FE153231D879777ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0x33133B977BCD03CDULL)));
  }
  {
    CUITextViewer* details = this->detailsViewer;
    details->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0x2FEB8739E5975BD1ULL)));
  }
  {
    CUITextViewer* details = this->detailsViewer;
    details->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0x6BA1C1132F033B75ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0xA12DAB392545D72FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0x8783C33941E15117ULL)));
  }
  this->AddChild(this->detailsViewer);

  this->valuesViewer = new CUITextViewer();
  this->valuesViewer->SetSize(108, 118);
  this->valuesViewer->Move(480, 5);
  this->valuesViewer->SetTextOffset(0, 6);
  {
    CUITextViewer* values = this->valuesViewer;
    values->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x271B45771F83DD9FULL)));
  }
  this->valuesViewer->SetFontHeight(12);
  this->valuesViewer->SetBold(0);
  this->valuesViewer->SetTextColor(0);
  this->valuesViewer->SetOutline(0, 0xFFFF00FF);
  this->valuesViewer->SetLineSpacing(6);
  this->valuesViewer->SetBackgroundColor(0xFFFF00FF);
  this->valuesViewer->SetPadding(0, 0, 0, 0);
  this->valuesViewer->SetScrollBarSize(8, 118);
  this->valuesViewer->SetScrollStep(20);
  {
    CUITextViewer* values = this->valuesViewer;
    values->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0xC70319B5EB61598FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0x0B8745519B1F95D7ULL)));
  }
  {
    CUITextViewer* values = this->valuesViewer;
    values->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0xA597A7E7492B39D5ULL)));
  }
  {
    CUITextViewer* values = this->valuesViewer;
    values->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0xFFABE3D5D1BBEF73ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0x0399459DAF454F0FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0xD5A78DB38D8DAFABULL)));
  }
  this->AddChild(this->valuesViewer);
}

// OFFSET: 0x009CFAF0
// FUNCTION: fn_14007
void CUIQuestInfo::OnCreate(int width, int height) {
  this->stampImage = new CUIStaticImage();
  this->stampImage->SetSize(85, 70);
  this->stampImage->Move(250, 10);
  {
    CUIStaticImage* stamp = static_cast<CUIStaticImage*>(this->stampImage);
    stamp->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\stamp_completed.bmp", 0xF14F79170939C7F1ULL)));
  }
  this->AddChild(this->stampImage);

  this->titleViewer = new CUITextViewer();
  this->titleViewer->SetSize(360, 57);
  this->titleViewer->Move(25, 13);
  this->titleViewer->SetTextOffset(0, 0);
  {
    CUITextViewer* title = this->titleViewer;
    title->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x35E5656B5DEDCD75ULL)));
  }
  this->titleViewer->SetFontHeight(12);
  this->titleViewer->SetBold(1);
  this->titleViewer->SetTextColor(0);
  this->titleViewer->SetOutline(0, 0xFFFF00FF);
  this->titleViewer->SetLineSpacing(6);
  this->titleViewer->SetBackgroundColor(0xFFFF00FF);
  this->titleViewer->SetPadding(15, 5, 5, 5);
  this->titleViewer->SetScrollBarSize(8, 87);
  this->titleViewer->SetScrollStep(20);
  {
    CUITextViewer* title = this->titleViewer;
    title->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0x4941FD9F21BBB9FDULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0xE7978FB5A94DD7FDULL)));
  }
  {
    CUITextViewer* title = this->titleViewer;
    title->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0xB7FD8F670FC7B96BULL)));
  }
  {
    CUITextViewer* title = this->titleViewer;
    title->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0x39FF7FB50DEB998FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0x2FE993D1DDE1F197ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0xAD075B8DB599F1CDULL)));
  }
  this->AddChild(this->titleViewer);

  this->linesViewer = new CUITextViewer();
  this->linesViewer->SetSize(200, 87);
  this->linesViewer->Move(400, 5);
  this->linesViewer->SetTextOffset(0, 6);
  {
    CUITextViewer* lines = this->linesViewer;
    lines->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0x2F8B0F37118D157FULL)));
  }
  this->linesViewer->SetFontHeight(12);
  this->linesViewer->SetBold(1);
  this->linesViewer->SetTextColor(0);
  this->linesViewer->SetOutline(0, 0xFFFF00FF);
  this->linesViewer->SetLineSpacing(6);
  this->linesViewer->SetBackgroundColor(0xFFFF00FF);
  this->linesViewer->SetPadding(15, 5, 5, 5);
  this->linesViewer->SetScrollBarSize(8, 87);
  this->linesViewer->SetScrollStep(20);
  {
    CUITextViewer* lines = this->linesViewer;
    lines->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0x0F15B16B5BAFD57FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0x7DE349D717D7F123ULL)));
  }
  {
    CUITextViewer* lines = this->linesViewer;
    lines->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0x7321C3814F8799E1ULL)));
  }
  {
    CUITextViewer* lines = this->linesViewer;
    lines->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0x7941D76335CFA14FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0xCB1D253B7FE1F563ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0x7115D9F973F913C9ULL)));
  }
  this->AddChild(this->linesViewer);

  this->statusViewer = new CUITextViewer();
  this->statusViewer->SetSize(360, 13);
  this->statusViewer->Move(25, 73);
  this->statusViewer->SetTextOffset(0, 0);
  {
    CUITextViewer* status = this->statusViewer;
    status->SetFont(std::string(OBFUSCATE_SOURCE_TEXT("Gulim", 0xE55723712993051DULL)));
  }
  this->statusViewer->SetFontHeight(12);
  this->statusViewer->SetBold(0);
  this->statusViewer->SetTextColor(0);
  this->statusViewer->SetOutline(0, 0xFFFF00FF);
  this->statusViewer->SetLineSpacing(6);
  this->statusViewer->SetBackgroundColor(0xFFFF00FF);
  this->statusViewer->SetPadding(15, 0, 3, 0);
  this->statusViewer->SetScrollBarSize(8, 12);
  this->statusViewer->SetScrollStep(1);
  this->statusViewer->SetScrollBarButtonImages("", "");
  this->statusViewer->SetScrollBarBackground("");
  this->statusViewer->SetScrollBarThumbImages("", "", "");
  this->AddChild(this->statusViewer);
}

// OFFSET: 0x009DCE10
// FUNCTION: fn_14330
void CUICharacterInfo::OnInit() {
  this->jobIcon->SetImage(std::format("유저인터페이스\\renewalparty\\icon_jobs_{}.bmp", g_session.m_job));
  const int level = g_session.m_level;
  this->level->SetText(std::format("Lv. {}", level));
  const int jobLevel = g_session.m_jobLevel;
  this->jobLevel->SetText(std::format("Jv. {}", jobLevel));
  this->name->SetText(g_session.DecodePlayerName());
}

// OFFSET: 0x009DD750
// FUNCTION: fn_14343
void CUIItemInfo::OnRBtnDown(int, int) {
  ITEM_INFO info;
  info.SetItemId(this->itemId);
  info.identity[0] = this->type;
  info.identified = 1;
  int location;
  if (this->type == 5) {
    location = this->location <= 9 ? 2 : 0x22;
  } else if (this->type == 4) {
    switch (this->location) {
    case 0:
      location = 0x100;
      break;
    case 1:
      location = 0x200;
      break;
    case 2:
      location = 1;
      break;
    case 3:
      location = 0x10;
      break;
    case 4:
      location = 4;
      break;
    case 5:
      location = 0x20;
      break;
    case 6:
      location = 0x40;
      break;
    case 7:
      location = 0x88;
      break;
    case 8:
      location = 8;
      break;
    case 9:
      location = 0x80;
      break;
    default:
      location = 0;
      break;
    }
  } else {
    location = 0;
  }
  info.identity[2] = location;

  UIWindow* window = g_windowMgr.EnsureWindow(WID_ITEMCOLLECTIONWND);
  if (window) {
    window->SendMsg(nullptr, UM_ITEMEXPLANATION, reinterpret_cast<std::intptr_t>(&info), 0, 0, 0);
  }
}

// OFFSET: 0x009DC720
// FUNCTION: fn_14322
void CUIMonster::BindMonster(CUIMonsterInfo* item, const CMonsterData& data) {
  item->SetMonster(data.name);
  item->valuesViewer->Clear();
  for (const std::string& value : data.values) {
    item->valuesViewer->AddLine(value);
  }
}

// OFFSET: 0x009DFCB0
// FUNCTION: fn_14360
void CUIMonsterInfo::SetMonster(const std::string& title) {
  this->detailsViewer->Clear();

  std::string name;
  std::string spriteName;
  int x = 0;
  int size = 0;
  int element = 0;
  int elementLevel = 0;
  int race = 0;
  int type = 0;
  if (!g_navigationManager.GetMobInfo(title, &name, &spriteName, &x, &size, &element, &elementLevel, &race, &type)) {
    this->nameText->SetText(title);
    return;
  }

  this->nameText->SetText(name.c_str(), name.size());
  this->sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName + ".spr"));
  this->action = static_cast<CActRes*>(ResourceMgr::Instance().Load(spriteName + ".act"));

  if (element == 1) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY4), elementLevel));
  } else if (element == 2) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY5), elementLevel));
  } else if (element == 3) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY6), elementLevel));
  } else if (element == 4) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY7), elementLevel));
  } else if (element == 5) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY8), elementLevel));
  } else if (element == 6) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY9), elementLevel));
  } else if (element == 7) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_PROPERTY0), elementLevel));
  } else if (element == 8) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_SCALE1), elementLevel));
  } else if (element == 9) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_SCALE2), elementLevel));
  } else {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_SCALE0), elementLevel));
  }

  if (race == 1) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE4)));
  } else if (race == 2) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE5)));
  } else if (race == 3) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE6)));
  } else if (race == 4) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE7)));
  } else if (race == 5) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE8)));
  } else if (race == 6) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE9)));
  } else if (race == 7) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE0)));
  } else if (race == 8) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_MOVECLICK)));
  } else if (race == 9) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_NPCMOVE)));
  } else {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_NPCCLICK)));
  }

  if (size == 1) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE1)));
  } else if (size == 2) {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE2)));
  } else {
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_RACE3)));
  }

  if (type == 0x12D) {
    this->detailsViewer->SetTextColor(0x6464FFu);
    this->detailsViewer->AddLine(FormatOwnedString(LookupMessage(MSI_NAVIGATION_TARGET_D_D)));
  }
}

}

// OFFSET: 0x009DC910
// FUNCTION: fn_14327
void AdventureGuide::CUIMonsterInfo::Paint() {
  CUIListItem::Paint();
  this->DrawActionSprite(this->sprite, this->action, 16, 2, 124, 124, 0, true, nullptr);
}

namespace AdventureGuide {

// OFFSET: 0x009C1550
// FUNCTION: fn_13992
CUIListBox<CUISubTab>* CUITab::CreateSubTabList() {
  this->subTabs = new CUIListBox<CUISubTab>();
  this->subTabs->SetSize(98, 26);
  this->subTabs->Move(0, 40);
  this->subTabs->SetOnSelect([this](CUISubTab* subTab) { this->OnSelectSubTab(subTab); });

  CUIListBox<CUISubTab>* list = this->subTabs;
  {
    list->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\tab_btn_sub_normal.bmp", 0x6BC977B3B5993503ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\tab_btn_sub_press.bmp", 0xB5F3F779237315F3ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\tab_btn_sub_over.bmp", 0x67DD5BAB1919D9D1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\tab_btn_sub_normal.bmp", 0xB5D7B5AF4FEDFF57ULL)));
  }

  {
    CUIListBox<CUISubTab>* subTabs = this->subTabs;
    subTabs->itemWidth = 98;
    subTabs->rowHeight = 26;
  }
  {
    CUIListBox<CUISubTab>* subTabs = this->subTabs;
    subTabs->itemX = 0;
    subTabs->itemY = 0;
  }
  this->subTabs->scrollBar->SetSize(9, 26);
  this->subTabs->scrollBar->Move(89, 0);
  this->subTabs->scrollBar->step = 20;
  {
    this->subTabs->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0x9B9D51AD3515F779ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0x038535BB092533B7ULL)));
  }
  {
    this->subTabs->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0xFB2BC75769E777D3ULL)));
  }
  {
    this->subTabs->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0x71A509DB43792991ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0xDD69EDFFA18F537DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0x8BBF792B47E72173ULL)));
  }

  this->AddChild(this->subTabs);
  return this->subTabs;
}

}

// OFFSET: 0x009C1230
// FUNCTION: fn_13986
CUIAdventureGuide* CreateAdventureGuide() {
  return new CUIAdventureGuide();
}

// OFFSET: 0x00487B10
// FUNCTION: fn_45546
static bool s_registered = [] {
  std::function<CUIFrameWnd*()> factory = CreateAdventureGuide;
  Singleton<CUIRegisterMgr>::GetInstance()->Register(10002, factory);
  return true;
}();

// OFFSET: 0x009DB280
// FUNCTION: fn_14306
template <>
AdventureGuide::CUISubTab* CUIListBox<AdventureGuide::CUISubTab>::CreateItem() {
  AdventureGuide::CUISubTab* item = new AdventureGuide::CUISubTab();
  item->SetSize(this->itemWidth, this->rowHeight);
  item->Show(0);
  item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
  item->onHover = [this](CUIListItem* listItem) {
    for (AdventureGuide::CUISubTab* other : this->items) {
      other->SetHighlighted(other == listItem);
    }
  };
  item->onPress = [this](CUIListItem* listItem) {
    this->SelectItem(listItem);
  };
  this->AddChild(item);
  this->items.push_back(item);
  this->UpdateScrollBar();
  this->Invalidate();
  return item;
}

// OFFSET: 0x009DB4B0
// FUNCTION: fn_14307
template <>
AdventureGuide::CUITab* CUIListBox<AdventureGuide::CUITab>::CreateItem() {
  AdventureGuide::CUITab* item = new AdventureGuide::CUITab();
  item->SetSize(this->itemWidth, this->rowHeight);
  item->Show(0);
  item->SetImageResources(this->normalImage, this->hoverImage, this->pressedImage, this->disabledImage);
  item->onHover = [this](CUIListItem* listItem) {
    for (AdventureGuide::CUITab* other : this->items) {
      other->SetHighlighted(other == listItem);
    }
  };
  item->onPress = [this](CUIListItem* listItem) {
    this->SelectItem(listItem);
  };
  this->AddChild(item);
  this->items.push_back(item);
  this->UpdateScrollBar();
  this->Invalidate();
  return item;
}

namespace AdventureGuide {

// OFFSET: 0x009DB720
// FUNCTION: fn_14309
void CUIItem::RequestNavigation() {
  std::string name;
  int x = -1;
  int y = -1;
  std::vector<std::string> parts = Split(this->navigationTarget, ",");
  if (parts.size() > 2) {
    name = parts[0];
    x = std::atoi(parts[1].c_str());
    y = std::atoi(parts[2].c_str());
  }

  if (x == -1 || y == -1) {
    g_navigationManager.StartNavigation(name, 1, 5, false, -1, -1, 1002);
  } else {
    g_navigationManager.StartNavigation(name, 0, 5, false, x, y, 1002);
  }
}

// OFFSET: 0x009DB8F0
// FUNCTION: fn_14310
void CUIQuest::RequestNavigation() {
  std::string name;
  int x = -1;
  int y = -1;
  std::vector<std::string> parts = Split(this->navigationTarget, ",");
  if (parts.size() > 2) {
    name = parts[0];
    x = std::atoi(parts[1].c_str());
    y = std::atoi(parts[2].c_str());
  }

  if (x == -1 || y == -1) {
    g_navigationManager.StartNavigation(name, 1, 5, false, -1, -1, 1002);
  } else {
    g_navigationManager.StartNavigation(name, 0, 5, false, x, y, 1002);
  }
}

// OFFSET: 0x009DBB80
// FUNCTION: fn_14311
void CUIItem::SearchNavigation() {
  const std::size_t comma = this->navigationTarget.find(',');
  if (comma == std::string::npos) {
    return;
  }

  std::string name = this->navigationTarget.substr(0, comma);
  g_navigationManager.searchText = name;
  g_navigationManager.UpdateSearchResults();
  g_navigationManager.selectedPath = 0;
  g_navigationManager.selectedPathNode = 0;
  g_navigationManager.SelectNodeState();

  UIWindow* window = g_windowMgr.LookupWindow(WID_NAVIGATIONV4WND);
  if (!window) {
    window = g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
  }
  window->SendMsg(nullptr, UM_COMMAND, 499, 0, 0, 0);
}

// OFFSET: 0x009DBCB0
// FUNCTION: fn_14312
void CUIQuest::SearchNavigation() {
  const std::size_t comma = this->navigationTarget.find(',');
  if (comma == std::string::npos) {
    return;
  }

  std::string name = this->navigationTarget.substr(0, comma);
  g_navigationManager.searchText = name;
  g_navigationManager.UpdateSearchResults();
  g_navigationManager.selectedPath = 0;
  g_navigationManager.selectedPathNode = 0;
  if (!g_navigationManager.SelectNodeState()) {
    this->RequestNavigation();
    g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(LookupMessage(4241)), 0, 0, 0, 0);
    return;
  }

  UIWindow* window = g_windowMgr.LookupWindow(WID_NAVIGATIONV4WND);
  if (!window) {
    window = g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
  }
  window->Invalidate();
  window->SendMsg(nullptr, UM_COMMAND, 499, 0, 0, 0);
}

}

namespace AdventureGuide {

// OFFSET: 0x009C5BB0
// FUNCTION: fn_14001
void CUIIntro::OnCreate(int width, int height) {
  this->banner = new CUIStaticImage();
  this->banner->SetSize(630, 216);
  this->banner->Move(0, 0);
  {
    this->banner->SetImage(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\bg_banner.bmp", 0xC517FB932D95BB6DULL)));
  }
  this->AddChild(this->banner);

  this->image = new CUIStaticImage();
  this->image->SetSize(583, 162);
  this->image->Move(22, 27);
  this->image->SetImage(std::string(""));
  this->AddChild(this->image);

  CUIListBoxEx<CUITabInfo, Guide_Intro::Contents>* contents = new CUIListBoxEx<CUITabInfo, Guide_Intro::Contents>();
  this->contents = contents;
  contents->itemWidth = 603;
  contents->rowHeight = 44;
  contents->SetSize(603, 166);
  this->contents->Move(10, 207);
  this->contents->SetOnSelect([this](CUITabInfo* item, const Guide_Intro::Contents& contents) { this->OnSelectContents(item, contents); });
  this->contents->SetBinder([this](CUITabInfo* item, const Guide_Intro::Contents& contents) { this->BindContents(item, contents); });
  {
    this->contents->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_1.bmp", 0x23BBD58F8B5FF5C9ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_1.bmp", 0x0B7109397F6B1387ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_1.bmp", 0x03772763F96D9DA5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_1.bmp", 0x050505679903579BULL)));
  }
  {
    CUIListBoxEx<CUITabInfo, Guide_Intro::Contents>* contents = this->contents;
    contents->itemX = 0;
    contents->itemY = 0;
  }
  this->contents->scrollBar->SetSize(8, 166);
  this->contents->scrollBar->Move(595, 0);
  this->contents->scrollBar->step = 44;
  {
    this->contents->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0xDD3913D5D7F97BE1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0xBF2B4B31397FF5A7ULL)));
  }
  {
    this->contents->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0x83EFBFBB37EDBDBDULL)));
  }
  {
    this->contents->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0xED796785D1DF137FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0xD97D5DBDBB2F2709ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0xA5FD29D7AB6DC59FULL)));
  }
  this->contents->AddNewItem();
  this->AddChild(this->contents);
}

// OFFSET: 0x009C6F50
// FUNCTION: fn_14002
void CUIItem::OnCreate(int width, int height) {
  CUIListBoxEx<CUIItemInfo, CItemData>* itemList = new CUIListBoxEx<CUIItemInfo, CItemData>();
  this->itemList = itemList;
  itemList->itemWidth = 603;
  itemList->rowHeight = 128;
  itemList->SetSize(627, 355);
  this->itemList->Move(0, 0);
  this->itemList->SetOnSelect([this](CUIItemInfo* item, const CItemData& data) { this->OnSelectItem(item, data); });
  this->itemList->SetBinder([this](CUIItemInfo* item, const CItemData& data) { this->BindItem(item, data); });
  {
    this->itemList->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_3_normal.bmp", 0xC9712D676B5B3943ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_3_press.bmp", 0xED15938D333D2DB9ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_3_over.bmp", 0xBD2BA1C11B01E73BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_3_normal.bmp", 0x4B755DB19953E311ULL)));
  }
  {
    CUIListBoxEx<CUIItemInfo, CItemData>* itemList = this->itemList;
    itemList->itemX = 0;
    itemList->itemY = 0;
  }
  this->itemList->scrollBar->SetSize(8, 355);
  this->itemList->scrollBar->Move(619, 0);
  this->itemList->scrollBar->step = 20;
  {
    this->itemList->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0x4331A17F33095FEBULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0xD1056FF94D4D2773ULL)));
  }
  {
    this->itemList->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0x0F97E779A933D917ULL)));
  }
  {
    this->itemList->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0x3789496BE3E1D141ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0x41CFCB831F6B6B91ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0x5DE7712FAB613F75ULL)));
  }
  this->itemList->AddNewItem();
  this->AddChild(this->itemList);

  this->findButton = new CUIImageButton();
  this->findButton->SetSize(62, 20);
  this->findButton->Move(471, 363);
  this->findButton->SetOnClick([this]() { this->RequestNavigation(); });
  {
    CUIImageButton* button = this->findButton;
    button->disabled = 0;
    button->Invalidate();
  }
  {
    this->findButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_find_normal.bmp", 0x81F58DED5D470D01ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_find_press.bmp", 0x454D31872BB3EDCBULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_find_over.bmp", 0x85E1F5A7A5755B7FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_find_disable.bmp", 0xAD6153019F93A527ULL)));
  }
  this->AddChild(this->findButton);

  this->navigationButton = new CUIImageButton();
  this->navigationButton->SetSize(62, 20);
  this->navigationButton->Move(540, 363);
  this->navigationButton->SetOnClick([this]() { this->SearchNavigation(); });
  {
    CUIImageButton* button = this->navigationButton;
    button->disabled = 0;
    button->Invalidate();
  }
  {
    this->navigationButton->SetImages(std::string(OBFUSCATE_TEXT("유저인터페이스\\AdventureGuide\\btn_move_normal.bmp", 0xBFF5C517A1A77F59ULL)), std::string(OBFUSCATE_TEXT("유저인터페이스\\AdventureGuide\\btn_move_press.bmp", 0x659975C121557D3DULL)), std::string(OBFUSCATE_TEXT("유저인터페이스\\AdventureGuide\\btn_move_over.bmp", 0xB7A96F8701BB4B6BULL)), std::string(OBFUSCATE_TEXT("유저인터페이스\\AdventureGuide\\btn_move_disable.bmp", 0xA31F69E7EFED01C9ULL)));
  }
  this->AddChild(this->navigationButton);
}

// OFFSET: 0x009CDD40
// FUNCTION: fn_14006
void CUIQuest::OnCreate(int width, int height) {
  CUIListBoxEx<CUIQuestInfo, CQuestInfo>* questList = new CUIListBoxEx<CUIQuestInfo, CQuestInfo>();
  this->questList = questList;
  questList->itemWidth = 603;
  questList->rowHeight = 97;
  questList->SetSize(627, 355);
  this->questList->Move(0, 0);
  this->questList->SetOnSelect([this](CUIQuestInfo* item, const CQuestInfo& data) { this->OnSelectQuest(item, data); });
  this->questList->SetBinder([this](CUIQuestInfo* item, const CQuestInfo& data) { BindQuestInfo(item, data); });
  {
    this->questList->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_2_normal.bmp", 0x793F370985ED674FULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_2_press.bmp", 0x176F138D5DD7F365ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_2_over.bmp", 0xFBD599E117C547C1ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_2_normal.bmp", 0x0F41EBBDA32D1B17ULL)));
  }
  {
    CUIListBoxEx<CUIQuestInfo, CQuestInfo>* questList = this->questList;
    questList->itemX = 0;
    questList->itemY = 0;
  }
  this->questList->scrollBar->SetSize(8, 355);
  this->questList->scrollBar->Move(619, 0);
  this->questList->scrollBar->step = 20;
  {
    this->questList->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0x2D51E3B7236175EDULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0xF3CB6F7711A71B6DULL)));
  }
  {
    this->questList->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0x839B27DF8F5967B3ULL)));
  }
  {
    this->questList->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0x9FED7DA189490D5DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0x452FEF0D7FC95565ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0x4F637DC51307B759ULL)));
  }
  this->questList->AddNewItem();
  this->AddChild(this->questList);

  this->findButton = new CUIImageButton();
  this->findButton->SetSize(62, 20);
  this->findButton->Move(471, 363);
  this->findButton->SetOnClick([this]() { this->RequestNavigation(); });
  {
    CUIImageButton* button = this->findButton;
    button->disabled = 0;
    button->Invalidate();
  }
  {
    this->findButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_find_normal.bmp", 0x2DCB7567394DC1ABULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_find_press.bmp", 0xB39FA7917BCFBB8DULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_find_over.bmp", 0x8D1D0FF1AF2B13F7ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_find_disable.bmp", 0x4F99DF03C3EF31B7ULL)));
  }
  this->AddChild(this->findButton);

  this->navigationButton = new CUIImageButton();
  this->navigationButton->SetSize(62, 20);
  this->navigationButton->Move(540, 363);
  this->navigationButton->SetOnClick([this]() { this->SearchNavigation(); });
  {
    CUIImageButton* button = this->navigationButton;
    button->disabled = 0;
    button->Invalidate();
  }
  {
    this->navigationButton->SetImages(std::string(OBFUSCATE_TEXT("유저인터페이스\\AdventureGuide\\btn_move_normal.bmp", 0x9171997D6963C9F7ULL)), std::string(OBFUSCATE_TEXT("유저인터페이스\\AdventureGuide\\btn_move_press.bmp", 0xCFF3899D95AF355FULL)), std::string(OBFUSCATE_TEXT("유저인터페이스\\AdventureGuide\\btn_move_over.bmp", 0xE52DF30DCBAF7125ULL)), std::string(OBFUSCATE_TEXT("유저인터페이스\\AdventureGuide\\btn_move_disable.bmp", 0x234977DDB54BC71DULL)));
  }
  this->AddChild(this->navigationButton);
}

// OFFSET: 0x009CAE30
// FUNCTION: fn_14004
void CUIMonster::OnCreate(int width, int height) {
  CUIListBoxEx<CUIMonsterInfo, CMonsterData>* monsterList = new CUIListBoxEx<CUIMonsterInfo, CMonsterData>();
  this->monsterList = monsterList;
  monsterList->itemWidth = 603;
  monsterList->rowHeight = 128;
  monsterList->SetSize(627, 355);
  this->monsterList->Move(0, 0);
  this->monsterList->SetOnSelect([this](CUIMonsterInfo* item, const CMonsterData& data) { this->OnSelectMonster(item, data); });
  this->monsterList->SetBinder([this](CUIMonsterInfo* item, const CMonsterData& data) { this->BindMonster(item, data); });
  {
    this->monsterList->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_3_normal.bmp", 0xE33F2FF779878559ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_3_press.bmp", 0xF15F8785CDE369BFULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_3_over.bmp", 0x792FFD1325418FB5ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\list_3_normal.bmp", 0x49D33529F1556333ULL)));
  }
  {
    CUIListBoxEx<CUIMonsterInfo, CMonsterData>* monsterList = this->monsterList;
    monsterList->itemX = 0;
    monsterList->itemY = 0;
  }
  this->monsterList->scrollBar->SetSize(8, 355);
  this->monsterList->scrollBar->Move(619, 0);
  this->monsterList->scrollBar->step = 20;
  {
    this->monsterList->SetScrollBarButtonImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_up.bmp", 0x01CB4F1DF9D3F931ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_arrow_down.bmp", 0xC5B1BBCD9BC9C997ULL)));
  }
  {
    this->monsterList->SetScrollBarBackground(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_bg_mid.bmp", 0xC1BB8111EDD183E1ULL)));
  }
  {
    this->monsterList->SetScrollBarThumbImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_up.bmp", 0x1767DDE18D779D8BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_mid.bmp", 0x0B23D10F2F49D72BULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\scroll_thumb_down.bmp", 0x136F2F9915D1DBCFULL)));
  }
  this->monsterList->AddNewItem();
  this->AddChild(this->monsterList);

  this->navigationButton = new CUIImageButton();
  this->navigationButton->SetSize(62, 20);
  this->navigationButton->Move(540, 363);
  this->navigationButton->SetOnClick([this]() { this->SearchNavigation(); });
  {
    CUIImageButton* button = this->navigationButton;
    button->disabled = 0;
    button->Invalidate();
  }
  {
    this->navigationButton->SetImages(std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_navi_normal.bmp", 0x6BDF334F2BFDAD51ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_navi_press.bmp", 0x674D9F3975573D41ULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_navi_over.bmp", 0x79E915E9AF6F55DDULL)), std::string(OBFUSCATE_SOURCE_TEXT("유저인터페이스\\AdventureGuide\\btn_navi_disbale.bmp", 0xB7AFBD2FD9FD7501ULL)));
  }
  this->AddChild(this->navigationButton);
}

// OFFSET: 0x009DBAC0
// FUNCTION: fn_45477
void CUIMonster::SearchNavigation() {
  g_navigationManager.searchText = this->navigationTarget;
  g_navigationManager.UpdateSearchResults();
  g_navigationManager.selectedPath = 0;
  g_navigationManager.selectedPathNode = 0;
  g_navigationManager.SelectNodeState();
  if (g_windowMgr.LookupWindow(WID_NAVIGATIONV4WND)) {
    g_navigationManager.SetViewMode(1);
    g_navigationManager.ClearSavedTarget();
    g_windowMgr.CloseWindow(WID_NAVIGATIONRUIDEWND);
    static_cast<UINavigationV4Wnd*>(g_windowMgr.navigationWnd)->RefreshLayout();
    g_windowMgr.Promote(g_windowMgr.LookupWindow(WID_NAVIGATIONV4WND));
  } else {
    g_windowMgr.EnsureWindow(WID_NAVIGATIONV4WND);
  }
}

}
