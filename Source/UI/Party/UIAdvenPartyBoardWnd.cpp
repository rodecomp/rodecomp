#include "Client/Input/TextInputBuffer.h"
#include "Contents/Party/AdventurerAgencyMgr.h"
#include "Contents/Party/AgencyRequestContext.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
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
#include "UI/Window/WindowManagerWindows.h"
#include "UI/Window/WindowOrdering.h"

#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>

namespace {
  constexpr char prefix[] = "유저인터페이스\\UIadventurerAgency\\";
}

// OFFSET: 0x007BAC40
// FUNCTION: fn_09274
UIAdvenPartyBoardWnd::UIAdvenPartyBoardWnd(UIWindowContext& windows, UIFrameContext& frames, UIAdvenPartyBoardContext& board)
    : UIFrameWnd(windows, frames, 0), unknown_0104(0), unknown_0108(0), page(1), totalPages(1), unknown_0114(1), unknown_0118(0), selectedType(0),
      m_board(&board) {
}

// OFFSET: 0x007BB590
UIAdvenPartyBoardWnd::~UIAdvenPartyBoardWnd() = default;

// OFFSET: 0x007C23C0
// FUNCTION: fn_09324
void UIAdvenPartyBoardWnd::Slot11() {
  if (this->saved) {
    this->saved->width = this->geometry.width;
    this->saved->height = this->geometry.height;
    this->saved->x = this->geometry.x;
    this->saved->y = this->geometry.y;
  }
}

// OFFSET: 0x007BC220
// FUNCTION: fn_09298
void UIAdvenPartyBoardWnd::Slot15(int, int) {
  auto& owner = *this->m_board;
  auto& controls = owner.buttons;

  this->minimum = new UIEditCtrl2(*this->m_context, owner.edits);
  this->minimum->SetSize(25, 14);
  this->minimum->invalidatesParent = 1;
  this->minimum->maximum = 3;
  this->minimum->TextColor(0, 0xFFFFFFFFu);
  char number[10] = {};
  _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", std::max((owner.playerLevel - 5), 1));
  this->minimum->SetText(number);
  this->minimum->Move(10, 24);
  this->minimum->Background(242, 242, 242);
  this->minimum->Show(1);
  this->AddChild(this->minimum);

  this->maximum = new UIEditCtrl2(*this->m_context, owner.edits);
  this->maximum->SetSize(25, 14);
  this->maximum->invalidatesParent = 1;
  this->maximum->maximum = 3;
  this->maximum->TextColor(0, 0xFFFFFFFFu);
  _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", (owner.playerLevel + 5));
  this->maximum->SetText(number);
  this->maximum->Move(46, 24);
  this->maximum->Background(242, 242, 242);
  this->maximum->Show(1);
  this->AddChild(this->maximum);
  const char* roles[] = {"def", "heal", "atk", "buf"};
  const int tooltips[] = {3420, 3491, 3444, 3508};

  for (int index = 0; index < 4; ++index) {
    auto* job = new UIJobSlotButton(*this->m_context, controls);
    this->jobs[index] = job;
    job->SetSize(16, 16);
    job->Move(160 + index * 18, 23);
    job->selectedImage = std::string(prefix) + "btn_job_" + roles[index] + "_on.bmp";
    job->unselectedImage = std::string(prefix) + "btn_job_" + roles[index] + "_off.bmp";
    job->SetSelected(0);
    job->SetTooltip(controls.services.MessageText(tooltips[index]));
    this->AddChild(job);
  }

  auto* memo = new UIEditCtrl5(*this->m_context, owner.edits);
  this->memo = memo;
  memo->SetSize(116, 14);
  memo->maximum = 150;
  memo->TextColor(0, 0xFFFFFFFFu);
  memo->placeholderEnabled = 1;
  memo->placeholder = controls.services.MessageText(3419);
  memo->placeholderColor = 0x646464;
  memo->Move(238, 24);
  memo->Background(255, 255, 255);
  this->AddChild(memo);
  const auto artwork = [&](UIBitmapButton* button, const char* name) {
    const char* suffixes[] = {"", "_over", "_press", "_disable"};

    for (int state = 0; state < 4; ++state) {
      button->SetBitmap((std::string(prefix) + name + suffixes[state] + ".bmp").c_str(), state, 0);
    }

    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  };

  auto* search = new UIBmpTextButton(*this->m_context, controls);
  artwork(search, "btn_search");
  search->Move(354, 22);
  search->SetId(365);
  search->SetText(controls.services.MessageText(3366));
  this->AddChild(search);

  auto* reset = new UIBmpTextButton(*this->m_context, controls);
  artwork(reset, "btn_initialize");
  reset->Move(401, 22);
  reset->SetId(363);
  reset->SetText(controls.services.MessageText(3367));
  this->AddChild(reset);

  this->refresh = new UIBitmapButton(*this->m_context, controls);
  artwork(this->refresh, "btn_refresh");
  this->refresh->Move(452, 22);
  this->refresh->SetId(415);
  this->AddChild(this->refresh);

  const auto arrow = [&](const char* name, int id, int x) {
    auto* button = new UIBitmapButton(*this->m_context, controls);

    for (int state = 0; state < 4; ++state) {
      button->SetBitmap((std::string(prefix) + name + (state == 3 ? "_disable.bmp" : ".bmp")).c_str(), state, 0);
    }

    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(x, 319);
    button->SetId(id);
    this->AddChild(button);

    return button;
  };
  this->previous = arrow("btn_list_left", 246, 199);
  this->next = arrow("btn_list_right", 218, 265);

  for (int index = 0; index < 10; ++index) {
    auto* row = new UIPartyInfoWnd(*this->m_context, controls, owner.rows);
    this->rows[index] = row;
    artwork(row, "btn_list");
    row->Move(1, 73 + index * 24);
    row->SetDisabled(1);
    row->Invalidate();
    row->SetId(379);
    row->m_argument = index;
    row->m_invalidateParent = 1;
    this->AddChild(row);
  }

  this->ownParty = new UIPartyInfoWnd(*this->m_context, controls, owner.rows);
  const char* ownImages[] = {"btn_recruit_on.bmp", "btn_recruit_on.bmp", "btn_recruiting_press.bmp", "btn_recruiting_disable.bmp"};

  for (int state = 0; state < 4; ++state) {
    this->ownParty->SetBitmap((std::string(prefix) + ownImages[state]).c_str(), state, 0);
  }

  this->ownParty->SetSize(this->ownParty->BitmapWidth(), this->ownParty->BitmapHeight());
  this->ownParty->Move(1, 49);
  this->ownParty->SetId(564);
  this->ownParty->Enable(0);
  this->AddChild(this->ownParty);

  auto* recruit = new UIBmpTextButton(*this->m_context, controls);
  this->recruit = recruit;
  artwork(recruit, "btn_recruit");
  recruit->Move(1, 49);
  recruit->SetId(565);
  recruit->Enable(0);
  this->AddChild(recruit);

  this->types = new UIComboBoxDef2Wnd(*this->m_context, owner.combos);
  this->types->dropdownHeight = 126;
  this->types->SetSize(76, 18);
  this->types->Move(76, 22);
  this->AddChild(this->types);

  for (int type = 0; type <= 6; ++type) {
    this->types->AddOption(UIPartyInfoWnd::TypeName(type, controls.services), 0);
  }

  this->types->Select(0);
  this->pageText = new UIStaticText(*this->m_context, controls);
  this->pageText->SetSize(this->geometry.width, 18);
  const int textWidth = this->TextWidth(controls.fonts, controls.locale, "1/1", 0, 0, 12, 0, 0);
  this->pageText->Resize((textWidth + 1), 16);
  this->pageText->SetText("1/1", 0);
  this->pageText->Move(this->geometry.width / 2 - textWidth / 2, 319);
  this->AddChild(this->pageText);
  this->LoadPage(1);
  this->m_message0 = 365;

  auto* close = new UIBasicButton(*this->m_context, controls);
  close->Configure(42, controls.services.MessageText(3045));
  close->Move(this->geometry.width - close->geometry.width - 10, 317);
  close->SetId(201);
  this->AddChild(close);

  this->stop = new UIBasicButton(*this->m_context, controls);
  const char* stopText = controls.services.MessageText(3375);
  this->stop->Configure((this->TextWidth(controls.fonts, controls.locale, stopText, 0, 0, 12, 0, 0) + 10), stopText);
  this->stop->Move(close->geometry.x - this->stop->geometry.width - 5, 317);
  this->stop->SetId(410);
  this->AddChild(this->stop);
  this->stop->Enable(0);

  auto* titleClose = new UIBitmapButton(*this->m_context, controls);
  const std::string basic = std::string("유저인터페이스\\basic_interface\\");
  titleClose->SetBitmap((basic + "sys_close_off.bmp").c_str(), 0, 0);
  titleClose->SetBitmap((basic + "sys_close_on.bmp").c_str(), 1, 0);
  titleClose->SetBitmap((basic + "sys_close_on.bmp").c_str(), 2, 0);
  titleClose->SetSize(titleClose->BitmapWidth(), titleClose->BitmapHeight());
  titleClose->Move(this->geometry.width - titleClose->BitmapWidth(), 0);
  titleClose->SetId(201);
  titleClose->SetTooltip("");
  this->AddChild(titleClose);
}

// OFFSET: 0x007BC070
// FUNCTION: fn_09296
void UIAdvenPartyBoardWnd::LoadPage(int pageValue) {
  this->previous->SetDisabled(1);
  this->previous->Invalidate();
  this->previous->Invalidate();
  this->next->SetDisabled(1);
  this->next->Invalidate();
  this->next->Invalidate();
  this->refresh->SetDisabled(1);
  this->refresh->Invalidate();
  this->refresh->Invalidate();
  g_adventurerAgencyMgr->RequestList(pageValue, this->m_board->requests);

  if (this->m_board->partyMember == 1) {
    g_adventurerAgencyMgr->RequestMyParty(this->m_board->requests);
  }
}

// OFFSET: 0x007C2120
// FUNCTION: fn_09322
void UIAdvenPartyBoardWnd::Registration(bool value) {
  auto& controls = this->m_board->buttons;

  if (this->m_board->partyMember != 0) {
    this->recruit->SetTooltip("");
    this->ownParty->Enable(value ? 1 : 0);
    this->ownParty->SetDisabled(value ? 0 : 1);
    this->ownParty->Invalidate();
    this->recruit->Enable(value ? 0 : 1);
    this->recruit->SetDisabled(value ? 1 : 0);
    this->recruit->Invalidate();
    this->stop->Enable(value ? 1 : 0);
    this->stop->SetDisabled(value ? 0 : 1);
    this->stop->Invalidate();
  } else {
    this->recruit->SetTooltip(controls.services.MessageText(2047));
    this->ownParty->Enable(0);
    this->ownParty->SetDisabled(1);
    this->ownParty->Invalidate();
    this->recruit->Enable(1);
    this->recruit->SetDisabled(1);
    this->recruit->Invalidate();
    static_cast<UIBmpTextButton*>(this->recruit)->text.clear();
    this->recruit->Invalidate();
  }

  this->Invalidate();
}

// OFFSET: 0x007C1BC0
// FUNCTION: fn_09317
void UIAdvenPartyBoardWnd::SetOwnParty(CAdvenPartyInfo value) {
  this->ownParty->SetInfo(value);
  this->Registration(true);
}

// INLINED
void UIAdvenPartyBoardWnd::RefreshRows() {
  for (auto* row : this->rows) {
    row->Reset();
  }

  const auto entries = g_adventurerAgencyMgr->Entries();

  for (std::size_t index = 0; index < entries.size() && index < 10; ++index) {
    this->rows[index]->SetInfo(entries[index]);
    this->rows[index]->Invalidate();
  }

  char text[1024] = {};
  _snprintf_s(text, sizeof(text), _TRUNCATE, "%d / %d", this->page, this->totalPages);
  auto& controls = this->m_board->buttons;
  const int width = this->TextWidth(controls.fonts, controls.locale, text, 0, 0, 12, 0, 0);
  this->pageText->Resize((width + 1), 16);
  this->pageText->SetText(text, 0);
  this->pageText->Move(this->geometry.width / 2 - width / 2, 319);
  this->previous->Move(this->geometry.width / 2 - this->previous->geometry.width - width / 2 - 3, 319);
  this->next->Move(width / 2 + this->geometry.width / 2 + 3, 319);
  this->Registration(g_adventurerAgencyMgr->registered);
  this->SendMsg(nullptr, 165, 0, 0, 0, 0);
  this->Invalidate();
  this->Invalidate();
}

// INLINED
void UIAdvenPartyBoardWnd::UpdatePagination() {
  if (this->page <= this->totalPages) {
    if (this->page != this->totalPages) {
      this->next->SetDisabled(0);
      this->next->Invalidate();
      this->next->Invalidate();
    }
  } else {
    this->page = this->totalPages;
  }

  this->previous->SetDisabled(this->page == 1);
  this->previous->Invalidate();
  this->previous->Invalidate();
  this->Invalidate();

  if (this->m_board->partyMember == 0) {
    this->recruit->geometry.unknown_0030 = 3;
    this->recruit->Invalidate();
  }

  this->refresh->SetDisabled(0);
  this->refresh->Invalidate();
  this->refresh->Invalidate();
}

// INLINED
void UIAdvenPartyBoardWnd::ResetSearch() {
  char number[10] = {};
  _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", std::max((this->m_board->playerLevel - 5), 1));
  this->minimum->SetText(number);
  _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", (this->m_board->playerLevel + 5));
  this->maximum->SetText(number);
  this->types->Select(0);

  for (auto* job : this->jobs) {
    job->geometry.unknown_0030 = 0;
    job->Invalidate();
    job->SetSelected(0);
  }

  this->searching = 0;
  this->memo->SetText("");
  this->Invalidate();
}

// INLINED
void UIAdvenPartyBoardWnd::Search(int trigger) {
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
  char number[10] = {};

  if (!digits(this->minimum->Text())) {
    _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", std::max((this->m_board->playerLevel - 5), 1));
    this->minimum->SetText(number);
  }

  if (!digits(this->maximum->Text())) {
    _snprintf_s(number, sizeof(number), _TRUNCATE, "%d", (this->m_board->playerLevel + 5));
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
  filter.characterName = this->m_board->characterName;

  for (auto* button : {this->previous, this->next, this->refresh}) {
    button->SetDisabled(1);
    button->Invalidate();
    button->Invalidate();
  }

  if (trigger != 246 && trigger != 218) {
    this->page = 1;
  }

  g_adventurerAgencyMgr->RequestSearch(filter, this->page, this->m_board->requests);
}

// OFFSET: 0x007C0420
// FUNCTION: fn_09312
int UIAdvenPartyBoardWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  const int value = static_cast<int>(a);

  switch (message) {
  case 6:
    switch (value) {
    case 201:
      this->m_board->requests.windows.CloseWindow(324);
      break;
    case 218:
      this->page = (this->page + 1);
      this->previous->SetDisabled(1);
      this->previous->Invalidate();
      this->previous->Invalidate();
      this->next->SetDisabled(1);
      this->next->Invalidate();
      this->next->Invalidate();

      if (this->searching == 1) {
        this->SendMsg(nullptr, 6, 365, this->page, 218, 0);
      } else {
        this->LoadPage(this->page);
      }

      break;
    case 246:
      this->page = (this->page - 1);

      if (this->searching == 1) {
        this->SendMsg(nullptr, 6, 365, this->page, 246, 0);
      } else {
        this->LoadPage(this->page);
      }

      break;
    case 363:
      this->ResetSearch();
      break;
    case 365:
      this->Search(static_cast<int>(c));
      break;
    case 379: {
      const auto& info = this->rows[static_cast<int>(b)]->info;

      if (info.accountId == this->m_board->rows.accountId) {
        this->m_board->requests.windows.CloseWindow(345);
        auto* window = static_cast<UIRegisterPartyWnd*>(this->m_board->requests.windows.EnsureWindow(323));

        if (window) {
          window->SetInfo(info);
          this->m_board->ordering.Promote(window);
        }
      } else {
        this->m_board->requests.windows.CloseWindow(323);
        auto* window = static_cast<UIApplyForPartyWnd*>(this->m_board->requests.windows.EnsureWindow(345));

        if (window) {
          window->SetInfo(info);
          this->m_board->ordering.Promote(window);
        }
      }

      break;
    }
    case 410:
      g_adventurerAgencyMgr->RequestStop(this->m_board->requests, this->m_board->stopCompletion);
      break;
    case 415:
      this->page = 1;
      this->searching = 0;
      this->LoadPage(1);
      break;
    case 564: {
      this->m_board->requests.windows.CloseWindow(345);
      auto* window = static_cast<UIRegisterPartyWnd*>(this->m_board->requests.windows.EnsureWindow(323));

      if (window) {
        window->SetInfo(this->ownParty->info);
        this->m_board->ordering.Promote(window);
      }

      break;
    }
    case 565: {
      this->m_board->requests.windows.CloseWindow(345);
      UIWindow* window = this->m_board->requests.windows.EnsureWindow(323);

      if (window) {
        this->m_board->ordering.Promote(window);
      }

      break;
    }
    default:
      break;
    }

    return 0;
  case 34:
    this->saved = reinterpret_cast<PartyBoardPlacement*>(a);

    if (this->saved->width <= 0 || this->saved->height <= 0 || this->saved->x < 0 || this->saved->x > this->m_board->screenWidth - this->saved->width ||
        this->saved->y < 0 || this->saved->y > this->m_board->screenHeight - this->saved->height) {
      *this->saved = {200, 200, 480, 340};
    }

    this->Move(this->saved->x, this->saved->y);
    return 0;
  case 39:
    if (sender == this->types) {
      this->selectedType = value;
    }

    return 0;
  case 60:
    this->RefreshRows();
    return 0;
  case 165:
    this->UpdatePagination();
    return 0;
  default:
    return this->UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }
}

// OFFSET: 0x007BF520
// FUNCTION: fn_09302
void UIAdvenPartyBoardWnd::Paint() {
  auto& controls = this->m_board->buttons;
  this->geometry.surface->Fill(nullptr, 0xFFFF00FFu);
  const auto load = [&](const char* path) { return static_cast<CBitmapRes*>(controls.resources.Load(path)); };
  auto* background = load((std::string(prefix) + "bg_agencyList.bmp").c_str());
  this->DrawBitmap(0, 0, background ? &background->image : nullptr, false);
  constexpr char basic[] = "유저인터페이스\\basic_interface\\";
  const auto skin = [&](const char* name) { return load((std::string(basic) + name).c_str()); };
  int left = 0;
  int right = 0;
  int titleHeight = 0;

  if (auto* bitmap = skin("titlebar_left.bmp")) {
    this->DrawBitmap(0, 0, &bitmap->image, false);
    left = bitmap->image.width;
  }

  if (auto* bitmap = skin("titlebar_right.bmp")) {
    right = bitmap->image.width;
    this->DrawBitmap(this->geometry.width - right, 0, &bitmap->image, false);
  }

  if (auto* bitmap = skin("titlebar_mid.bmp")) {
    titleHeight = bitmap->image.height;
    const int extent = this->geometry.width - right - left;
    const int count = extent / bitmap->image.width + (extent % bitmap->image.width != 0);

    for (int index = 0; index < count; ++index, left = (left + bitmap->image.width)) {
      this->DrawBitmap(left, 0, &bitmap->image, false);
    }
  }

  const char* title = controls.services.MessageText(3504);
  this->DrawTextAt(controls.fonts, controls.locale, 19, titleHeight - 13, title, 0, 0, 12, 0xFFFFFF, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 18, titleHeight - 14, title, 0, 0, 12, 0, 0, 0);
  auto* icon = skin("sys_base_off.bmp");
  this->DrawBitmap(3, 3, icon ? &icon->image : nullptr, true);
  auto* barLeft = skin("btnbar_left2.bmp");
  auto* barRight = skin("btnbar_right2.bmp");
  auto* barMiddle = skin("btnbar_mid2.bmp");
  const int barHeight = barLeft ? barLeft->image.height : 0;
  const int leftWidth = barLeft ? barLeft->image.width : 0;
  const int rightWidth = barRight ? barRight->image.width : 0;
  this->DrawBitmap(0, this->geometry.height - barHeight, barLeft ? &barLeft->image : nullptr, false);
  this->DrawBitmap(this->geometry.width - rightWidth, this->geometry.height - barHeight, barRight ? &barRight->image : nullptr, false);

  if (barMiddle) {
    const int extent = this->geometry.width - rightWidth - leftWidth;
    const int count = extent / barMiddle->image.width + (extent % barMiddle->image.width != 0);

    for (int index = 0, x = leftWidth; index < count; ++index, x = (x + barMiddle->image.width)) {
      this->DrawBitmap(x, this->geometry.height - barHeight, &barMiddle->image, false);
    }
  }

  this->DrawTextAt(controls.fonts, controls.locale, 36, 25, "~", 0, 0, 12, 0, 0, 0);
}
