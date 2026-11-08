#include "Contents/MessageTable.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Input/Language.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Client/Session.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/InputFocus.h"
#include "UI/Party/UIAdvenPartyBoardWnd.h"
#include "UI/Party/UIApplyForPartyWnd.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Buttons/UIJobSlotButton.h"
#include "UI/Party/UIPartyInfoWnd.h"
#include "UI/Party/UIRegisterPartyWnd.h"
#include "UI/Text/UIStaticText.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>

namespace {
  constexpr char prefix[] = "유저인터페이스\\UIadventurerAgency\\";

  // INLINED
  void DisableButton(UIButton* button, unsigned char value) {
    button->SetDisabled(value);
    button->Invalidate();
  }
}

// OFFSET: 0x007BAC40
// FUNCTION: fn_09274
UIAdvenPartyBoardWnd::UIAdvenPartyBoardWnd()
    : UIFrameWnd(0), unknown_0104(0), unknown_0108(0), page(1), totalPages(1), unknown_0114(1), unknown_0118(0), selectedType(0) {
}

UIAdvenPartyBoardWnd::~UIAdvenPartyBoardWnd() = default;

// OFFSET: 0x007C23C0
// FUNCTION: fn_09324
void UIAdvenPartyBoardWnd::StoreInfo() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007BC220
// FUNCTION: fn_09298
void UIAdvenPartyBoardWnd::OnCreate(int, int) {
  const int level = g_session.m_level;
  this->minimum = new UIEditCtrl2();
  this->minimum->SetSize(25, 14);
  this->minimum->invalidatesParent = 1;
  this->minimum->maximum = 3;
  this->minimum->TextColor(0, 0xFFFFFFFFu);
  char minimumText[10] = {};
  _snprintf_s(minimumText, sizeof(minimumText), _TRUNCATE, "%d", std::max(1, level - 5));
  this->minimum->SetText(minimumText);
  this->minimum->Move(10, 24);
  this->minimum->Background(242, 242, 242);
  this->minimum->Show(1);
  this->AddChild(this->minimum);

  this->maximum = new UIEditCtrl2();
  this->maximum->SetSize(25, 14);
  this->maximum->invalidatesParent = 1;
  this->maximum->maximum = 3;
  this->maximum->TextColor(0, 0xFFFFFFFFu);
  char maximumText[10] = {};
  _snprintf_s(maximumText, sizeof(maximumText), _TRUNCATE, "%d", level + 5);
  this->maximum->SetText(maximumText);
  this->maximum->Move(46, 24);
  this->maximum->Background(242, 242, 242);
  this->maximum->Show(1);
  this->AddChild(this->maximum);

  const int positions[][2] = {{160, 23}, {178, 23}, {196, 23}, {214, 23}};
  const char* images[] = {"btn_job_def_on", "btn_job_def_off", "btn_job_heal_on", "btn_job_heal_off", "btn_job_atk_on", "btn_job_atk_off", "btn_job_buf_on", "btn_job_buf_off"};
  const std::string path = prefix;
  const char* tooltips[] = {LookupMessage(MSI_EXPLAIN_CHAT_UNBLOCK), LookupMessage(MSI_CANNOT_FIND_PARTYMASTER), LookupMessage(MSI_QUESTWND_LOWER), LookupMessage(MSI_EXPAND_SIGHT_ON)};

  for (int index = 0; index < 4; ++index) {
    this->jobs[index] = new UIJobSlotButton();
    this->jobs[index]->SetSize(16, 16);
    this->jobs[index]->Move(positions[index][0], positions[index][1]);
    this->jobs[index]->selectedImage = path + images[index * 2] + ".bmp";
    this->jobs[index]->unselectedImage = path + images[index * 2 + 1] + ".bmp";
    this->jobs[index]->SetSelected(0);
    this->jobs[index]->SetTooltip(tooltips[index]);
    this->AddChild(this->jobs[index]);
  }

  auto* memo = new UIEditCtrl5();
  this->memo = memo;
  memo->SetSize(116, 14);
  memo->maximum = 150;
  memo->TextColor(0, 0xFFFFFFFFu);
  memo->SetPlaceholderEnabled(true);
  memo->SetPlaceholder(LookupMessage(MSI_EXPLAIN_CHAT_BLOCK));
  memo->SetPlaceholderColor(0x646464);
  memo->Move(238, 24);
  memo->Background(255, 255, 255);
  this->AddChild(memo);

  auto* search = new UIBmpTextButton();
  search->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_search.bmp", 0, 0);
  search->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_search_over.bmp", 1, 0);
  search->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_search_press.bmp", 2, 0);
  search->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_search_disable.bmp", 3, 0);
  search->SetSize(search->BitmapWidth(), search->BitmapHeight());
  search->Move(354, 22);
  search->SetId(365);
  search->SetText(LookupMessage(MSI_ADVEN_PURPOSE));
  this->AddChild(search);
  auto* reset = new UIBmpTextButton();
  reset->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_initialize.bmp", 0, 0);
  reset->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_initialize_over.bmp", 1, 0);
  reset->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_initialize_press.bmp", 2, 0);
  reset->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_initialize_disable.bmp", 3, 0);
  reset->SetSize(reset->BitmapWidth(), reset->BitmapHeight());
  reset->Move(401, 22);
  reset->SetId(363);
  reset->SetText(LookupMessage(MSI_ADVEN_WARNING_TEXT));
  this->AddChild(reset);
  this->refresh = new UIBitmapButton();
  this->refresh->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_refresh.bmp", 0, 0);
  this->refresh->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_refresh_over.bmp", 1, 0);
  this->refresh->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_refresh_press.bmp", 2, 0);
  this->refresh->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_refresh_disable.bmp", 3, 0);
  this->refresh->SetSize(this->refresh->BitmapWidth(), this->refresh->BitmapHeight());
  this->refresh->Move(452, 22);
  this->refresh->SetId(415);
  this->AddChild(this->refresh);

  this->previous = new UIBitmapButton();
  this->previous->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_left.bmp", 0, 0);
  this->previous->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_left.bmp", 1, 0);
  this->previous->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_left.bmp", 2, 0);
  this->previous->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_left_disable.bmp", 3, 0);
  this->previous->SetSize(this->previous->BitmapWidth(), this->previous->BitmapHeight());
  this->previous->Move(199, 319);
  this->previous->SetId(246);
  this->AddChild(this->previous);
  this->next = new UIBitmapButton();
  this->next->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_right.bmp", 0, 0);
  this->next->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_right.bmp", 1, 0);
  this->next->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_right.bmp", 2, 0);
  this->next->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_right_disable.bmp", 3, 0);
  this->next->SetSize(this->next->BitmapWidth(), this->next->BitmapHeight());
  this->next->Move(265, 319);
  this->next->SetId(218);
  this->AddChild(this->next);

  const std::vector<CAdvenPartyInfo> parties = g_adventurerAgencyMgr->Entries();
  for (int index = 0; index < 10; ++index) {
    this->rows[index] = new UIPartyInfoWnd();
    this->rows[index]->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list.bmp", 0, 0);
    this->rows[index]->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_over.bmp", 1, 0);
    this->rows[index]->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_press.bmp", 2, 0);
    this->rows[index]->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_list_disable.bmp", 3, 0);
    this->rows[index]->SetSize(this->rows[index]->BitmapWidth(), this->rows[index]->BitmapHeight());
    this->rows[index]->Move(1, 73 + index * 24);
    DisableButton(this->rows[index], 1);
    this->rows[index]->SetId(379);
    this->rows[index]->m_argument = index;
    this->rows[index]->m_invalidateParent = 1;
    this->AddChild(this->rows[index]);
  }

  char pages[1024] = {};
  _snprintf_s(pages, sizeof(pages), _TRUNCATE, "%d / %d", this->page, this->totalPages);

  this->ownParty = new UIPartyInfoWnd();
  this->ownParty->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_on.bmp", 0, 0);
  this->ownParty->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_on.bmp", 1, 0);
  this->ownParty->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruiting_press.bmp", 2, 0);
  this->ownParty->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruiting_disable.bmp", 3, 0);
  this->ownParty->SetSize(this->ownParty->BitmapWidth(), this->ownParty->BitmapHeight());
  this->ownParty->Move(1, 49);
  this->ownParty->SetId(564);
  this->ownParty->Enable(0);
  this->AddChild(this->ownParty);
  this->recruit = new UIBmpTextButton();
  this->recruit->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_normal.bmp", 0, 0);
  this->recruit->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_over.bmp", 1, 0);
  this->recruit->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_press.bmp", 2, 0);
  this->recruit->SetBitmap("유저인터페이스\\UIadventurerAgency\\btn_recruit_disable.bmp", 3, 0);
  this->recruit->SetSize(this->recruit->BitmapWidth(), this->recruit->BitmapHeight());
  this->recruit->Move(1, 49);
  this->recruit->SetId(565);
  this->recruit->Enable(0);
  this->AddChild(this->recruit);

  this->types = new UIComboBoxDef2Wnd();
  this->types->dropdownHeight = 126;
  this->types->SetSize(76, 18);
  this->types->Move(76, 22);
  this->AddChild(this->types);
  this->types->AddOption(LookupMessage(MSI_ACCOUNT), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_MD), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_EDEN_GROUP), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_ETC), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_SEARCH), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_RESET), 0);
  this->types->AddOption(LookupMessage(MSI_ADVEN_JOIN_PARTY), 0);

  this->types->Select(0);
  this->pageText = new UIStaticText();
  this->pageText->SetSize(this->geometry.width, 18);
  const int textWidth = this->TextWidth("1/1", 0, 0, 12, 0, 0);
  this->pageText->Resize((textWidth + 1), 16);
  this->pageText->SetText("1/1", 0);
  const int halfWidth = textWidth / 2;
  this->pageText->Move(this->geometry.width / 2 - halfWidth, 319);
  this->AddChild(this->pageText);
  this->LoadPage(1);
  this->m_message0 = 365;

  auto* close = new UIBasicButton();
  close->Configure(42, LookupMessage(MSI_ZENY_LOTTO_BUY_10));
  close->Move(this->geometry.width - close->geometry.width - 10, 317);
  close->SetId(201);
  this->AddChild(close);
  this->stop = new UIBasicButton();
  this->stop->Configure((this->TextWidth(LookupMessage(MSI_REQUEST_PARTY), 0, 0, 12, 0, 0) + 10), LookupMessage(MSI_REQUEST_PARTY));
  this->stop->Move(close->geometry.x - this->stop->geometry.width - 5, 317);
  this->stop->SetId(410);
  this->AddChild(this->stop);
  this->stop->Enable(0);
  this->AddBigCloseButton("", 0);
}

// OFFSET: 0x007BC070
// FUNCTION: fn_09296
void UIAdvenPartyBoardWnd::LoadPage(int pageValue) {
  DisableButton(this->previous, 1);
  this->previous->Invalidate();
  DisableButton(this->next, 1);
  this->next->Invalidate();
  DisableButton(this->refresh, 1);
  this->refresh->Invalidate();
  g_adventurerAgencyMgr->RequestList(pageValue);

  if (g_session.unknown_5464 == 1) {
    g_adventurerAgencyMgr->RequestMyParty();
  }
}

// OFFSET: 0x007C2120
// FUNCTION: fn_09322
void UIAdvenPartyBoardWnd::Registration(bool value) {

  if (g_session.unknown_5464 != 0) {
    this->recruit->SetTooltip("");

    if (value) {
      this->ownParty->Enable(1);
      DisableButton(this->ownParty, 0);
      this->recruit->Enable(0);
      DisableButton(this->recruit, 1);
      this->stop->Enable(1);
      DisableButton(this->stop, 0);
    } else {
      this->ownParty->Enable(0);
      DisableButton(this->ownParty, 1);
      this->recruit->Enable(1);
      DisableButton(this->recruit, 0);
      this->stop->Enable(0);
      DisableButton(this->stop, 1);
    }
  } else {
    this->recruit->SetTooltip(LookupMessage(MSI_PARTY_RECRUIT_ERROR));
    this->ownParty->Enable(0);
    DisableButton(this->ownParty, 1);
    this->recruit->Enable(1);
    DisableButton(this->recruit, 1);
    static_cast<UIBmpTextButton*>(this->recruit)->SetText(std::string());
  }

  this->InvalidateChildren();
}

// OFFSET: 0x007C1BC0
// FUNCTION: fn_09317
void UIAdvenPartyBoardWnd::SetOwnParty(CAdvenPartyInfo value) {
  this->ownParty->SetInfo(value);
  this->Registration(true);
}

// INLINED
__forceinline void UIPartyInfoWnd::Reset() {
  this->m_drawLabel = 0;
  this->memo = "";
  this->levels = "";
  this->tanker = 1;
  this->dealer = 1;
  this->assist = 1;
  this->type = 0;
  this->name = "";
  this->m_disabled = 1;
  this->Invalidate();
}

// INLINED
__forceinline void UIAdvenPartyBoardWnd::RefreshRows() {
  for (int index = 0; index < 10; ++index) {
    this->rows[index]->Reset();
  }

  const auto entries = g_adventurerAgencyMgr->Entries();

  int index = 0;

  for (auto entry = entries.begin(); entry != entries.end(); ++entry) {
    if (index > 9) {
      break;
    }

    this->rows[index]->SetInfo(*entry);
    this->rows[index]->Invalidate();
    ++index;
  }

  char text[1024] = {};
  _snprintf_s(text, sizeof(text), _TRUNCATE, "%d / %d", this->page, this->totalPages);
  const int width = this->TextWidth(text, 0, 0, 12, 0, 0);
  this->pageText->Resize((width + 1), 16);
  this->pageText->SetText(text, 0);
  const int halfWidth = width / 2;
  this->pageText->Move(this->geometry.width / 2 - halfWidth, 319);
  this->previous->Move(this->geometry.width / 2 - this->previous->geometry.width - halfWidth - 3, 319);
  this->next->Move(halfWidth + this->geometry.width / 2 + 3, 319);
  this->Registration(g_adventurerAgencyMgr->registered);
  this->SendMsg(nullptr, UM_UPDATE_PARTY_PAGINATION, 0, 0, 0, 0);
  this->Invalidate();
  this->InvalidateChildren();
}

// INLINED
void UIAdvenPartyBoardWnd::UpdatePagination() {
  if (this->page > this->totalPages) {
    this->page = this->totalPages;
  } else if (this->page != this->totalPages) {
    DisableButton(this->next, 0);
    this->next->Invalidate();
  }

  if (this->page == 1) {
    DisableButton(this->previous, 1);
  } else {
    DisableButton(this->previous, 0);
  }

  this->previous->Invalidate();
  this->Invalidate();

  if (g_session.unknown_5464 == 0) {
    this->ownParty->SetState(3);
  }

  DisableButton(this->refresh, 0);
  this->refresh->Invalidate();
}

// INLINED
void UIAdvenPartyBoardWnd::ResetSearch() {
  const int level = g_session.m_level;
  char minimumText[10] = {};
  _snprintf_s(minimumText, sizeof(minimumText), _TRUNCATE, "%d", std::max(1, level - 5));
  this->minimum->SetText(minimumText);
  char maximumText[10] = {};
  _snprintf_s(maximumText, sizeof(maximumText), _TRUNCATE, "%d", level + 5);
  this->maximum->SetText(maximumText);
  this->types->Select(0);

  for (int index = 0; index < 4; ++index) {
    this->jobs[index]->SetState(0);
    this->jobs[index]->SetSelected(0);
  }

  this->searching = 0;
  this->memo->SetText("");
  this->InvalidateChildren();
}

// INLINED
__forceinline void UIAdvenPartyBoardWnd::Search(int trigger) {
  this->searching = 1;
  const auto digits = [](const char* text) {
    if (!*text) {
      return false;
    }

    for (; *text; ++text) {
      if (!std::isdigit(static_cast<unsigned char>(*text))) {
        return false;
      }
    }

    return true;
  };

  if (!digits(this->minimum->Text())) {
    char number[10] = {};
    _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", std::max(1, (g_session.m_level - 5)));
    this->minimum->SetText(number);
  }

  if (!digits(this->maximum->Text())) {
    char number[10] = {};
    _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", (g_session.m_level + 5));
    this->maximum->SetText(number);
  }

  CAdvenPartyInfo filter;
  filter.tanker = this->jobs[0]->selected;
  filter.healer = this->jobs[1]->selected;
  filter.dealer = this->jobs[2]->selected;
  filter.assist = this->jobs[3]->selected;

  if (this->minimum->Length() > 0) {
    filter.minimumLevel = std::atoi(this->minimum->Text());
  }

  if (this->minimum->Length() > 0) {
    filter.maximumLevel = std::atoi(this->maximum->Text());
  }

  if (this->selectedType != 0) {
    filter.type = this->selectedType;
  }

  filter.memo = this->memo->Text();
  filter.characterName = g_session.DecodePlayerName();

  DisableButton(this->previous, 1);
  this->previous->Invalidate();
  DisableButton(this->next, 1);
  this->next->Invalidate();
  DisableButton(this->refresh, 1);
  this->refresh->Invalidate();

  if (trigger != 246 && trigger != 218) {
    this->page = 1;
  }

  g_adventurerAgencyMgr->RequestSearch(filter, this->page);
}

// OFFSET: 0x007C0420
// FUNCTION: fn_09312
int UIAdvenPartyBoardWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const int value = static_cast<int>(a);

  switch (message) {
  case UM_SETINFO:
    this->saved = reinterpret_cast<AdvenPartyBoardWndInfo*>(a);

    if (!this->saved->IsValid()) {
      this->saved->Reset();
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  case UM_REFRESH:
    this->RefreshRows();
    return 0;
  case UM_UPDATE_PARTY_PAGINATION:
    this->UpdatePagination();
    return 0;
  case UM_COMMAND:
    switch (value) {
    case ID_RESET:
      this->ResetSearch();
      break;
    case ID_PREV:
      this->page = (this->page - 1);

      if (this->searching == 1) {
        this->SendMsg(nullptr, UM_COMMAND, ID_PARTY_SEARCH, this->page, 246, 0);
      } else {
        this->LoadPage(this->page);
      }

      break;
    case ID_NEXT:
      this->page = (this->page + 1);
      DisableButton(this->previous, 1);
      this->previous->Invalidate();
      DisableButton(this->next, 1);
      this->next->Invalidate();

      if (this->searching == 1) {
        this->SendMsg(nullptr, UM_COMMAND, ID_PARTY_SEARCH, this->page, 218, 0);
      } else {
        this->LoadPage(this->page);
      }

      break;
    case ID_PARTY_SEARCH:
      this->Search(static_cast<int>(c));
      break;
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_ADVENPARTYBOARDWND);
      break;
    case ID_SELECT_PARTY_LISTING: {
      const auto& info = this->rows[static_cast<int>(b)]->info;

      if (info.accountId == g_session.m_aid) {
        g_windowMgr.CloseWindow(WID_APPLYFORPARTYWND);
        auto* window = static_cast<UIRegisterPartyWnd*>(g_windowMgr.EnsureWindow(WID_REGISTERPARTYWND));

        if (window) {
          window->SetInfo(info);
          g_windowMgr.Promote(window);
        }
      } else {
        g_windowMgr.CloseWindow(WID_REGISTERPARTYWND);
        auto* window = static_cast<UIApplyForPartyWnd*>(g_windowMgr.EnsureWindow(WID_APPLYFORPARTYWND));

        if (window) {
          window->SetInfo(info);
          g_windowMgr.Promote(window);
        }
      }

      break;
    }
    case ID_EDIT_OWN_PARTY_LISTING: {
      g_windowMgr.CloseWindow(WID_APPLYFORPARTYWND);
      auto* window = static_cast<UIRegisterPartyWnd*>(g_windowMgr.EnsureWindow(WID_REGISTERPARTYWND));

      if (window) {
        window->SetInfo(this->ownParty->info);
        g_windowMgr.Promote(window);
      }

      break;
    }
    case ID_REGISTER_PARTY_LISTING: {
      g_windowMgr.CloseWindow(WID_APPLYFORPARTYWND);
      UIWindow* window = g_windowMgr.EnsureWindow(WID_REGISTERPARTYWND);

      if (window) {
        g_windowMgr.Promote(window);
      }

      break;
    }
    case 410:
      g_adventurerAgencyMgr->RequestStop();
      break;
    case ID_PARTY_RELOAD_FIRST_PAGE:
      this->page = 1;
      this->searching = 0;
      this->LoadPage(1);
      break;
    default:
      break;
    }

    return 0;
  case UM_COMBOBOX_SELECTED:
    if (sender == this->types) {
      this->selectedType = value;
    }

    return 0;
  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x007BF520
// FUNCTION: fn_09302
void UIAdvenPartyBoardWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawBitmap(0, 0, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\UIadventurerAgency\\bg_agencyList.bmp"))), false);
  this->DrawTitleBar(true, LookupMessage(MSI_ADVENTURER_AGENCY_RECRUIT_STOP_FAILED), 0);
  this->DrawButtonBar(2, 0, 1);
  this->DrawTextAt(36, 25, "~", 0, 0, 12, 0, 0, 0);
}
