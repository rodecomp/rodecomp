#include "UI/UIReputeWnd.h"
#include "Contents/MessageTable.h"
#include "Contents/ReputeMgr.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIComboBoxEx.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>
#include <new>

// GLOBAL: gl_25000
template <>
CReputeMgr* Singleton<CReputeMgr>::s_instance = nullptr;

// OFFSET: 0x00610D60
// FUNCTION: fn_04320
template <>
CReputeMgr* Singleton<CReputeMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) CReputeMgr;
  }

  return s_instance;
}

// OFFSET: 0x0060EF80
// FUNCTION: fn_04309
UIReputeWnd::UIReputeWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_groupCombo(nullptr), m_searchEdit(nullptr),
      m_perPage(8), m_page(1), m_pageCount(1), m_groupSelected(false), m_totalPoints(0), m_reputes() {}

// OFFSET: 0x0060F170
// FUNCTION: fn_04312
bool UIReputeWnd::Slot02() {
  return true;
}

// OFFSET: 0x0060F180
// FUNCTION: fn_04313
void UIReputeWnd::Slot15(int width, int height) {
  this->AddBaseButton(*this->m_controls);
  this->AddCloseButton(*this->m_controls, "", 0);

  this->m_groupCombo = new UIComboBoxDefWnd(*this->m_context, *this->m_comboContext);
  this->m_groupCombo->dropdownHeight = 160;
  this->m_groupCombo->SetSize(122, 18);
  this->m_groupCombo->Move(8, 25);
  this->m_groupCombo->red = 239;
  this->m_groupCombo->green = 239;
  this->m_groupCombo->blue = 239;
  this->m_groupCombo->borderRed = 99;
  this->m_groupCombo->borderGreen = 99;
  this->m_groupCombo->borderBlue = 99;
  this->AddChild(this->m_groupCombo);

  this->m_searchEdit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_searchEdit->SetSize(200, 18);
  this->m_searchEdit->Move(136, 25);
  this->m_searchEdit->maximum = 20;
  this->m_searchEdit->preserveInput = 1;
  this->m_searchEdit->Background(239, 239, 239);
  this->m_searchEdit->Border(99, 99, 99);
  this->m_searchEdit->flat = 0;
  this->AddChild(this->m_searchEdit);

  UIBitmapButton* search = new UIBitmapButton(*this->m_context, *this->m_controls);
  search->SetBitmap("유저인터페이스\\Reputation\\btn_search.bmp", 0, 0);
  search->SetBitmap("유저인터페이스\\Reputation\\btn_search_over.bmp", 1, 0);
  search->SetBitmap("유저인터페이스\\Reputation\\btn_search_press.bmp", 2, 0);
  search->SetSize(search->BitmapWidth(), search->BitmapHeight());
  const std::string searchLabel = LookupMessage(3777);
  search->label = searchLabel;
  search->Move(336, 25);
  search->SetId(321);
  this->AddChild(search);
  this->m_message0 = 321;

  this->m_prevButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->m_prevButton->SetBitmap("유저인터페이스\\Reputation\\arrow_prev.bmp", 0, 0);
  this->m_prevButton->SetBitmap("유저인터페이스\\Reputation\\arrow_prev.bmp", 1, 0);
  this->m_prevButton->SetBitmap("유저인터페이스\\Reputation\\arrow_prev.bmp", 2, 0);
  this->m_prevButton->SetSize(this->m_prevButton->BitmapWidth(), this->m_prevButton->BitmapHeight());
  this->m_prevButton->Move(567, 402);
  this->m_prevButton->Enable(0);
  this->m_prevButton->SetTooltip(LookupMessage(1053));
  this->m_prevButton->SetId(316);
  this->AddChild(this->m_prevButton);

  this->m_nextButton = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->m_nextButton->SetBitmap("유저인터페이스\\Reputation\\arrow_next.bmp", 0, 0);
  this->m_nextButton->SetBitmap("유저인터페이스\\Reputation\\arrow_next.bmp", 1, 0);
  this->m_nextButton->SetBitmap("유저인터페이스\\Reputation\\arrow_next.bmp", 2, 0);
  this->m_nextButton->SetSize(this->m_nextButton->BitmapWidth(), this->m_nextButton->BitmapHeight());
  this->m_nextButton->Move(623, 402);
  this->m_nextButton->Enable(0);
  this->m_nextButton->SetTooltip(LookupMessage(1054));
  this->m_nextButton->SetId(317);
  this->AddChild(this->m_nextButton);

  UIBasicButton* close = new UIBasicButton(*this->m_context, *this->m_controls);
  close->Configure(42, LookupMessage(3045));
  close->Move(589, 425);
  close->SetId(201);
  this->AddChild(close);

  this->m_groupCombo->AddOption(LookupMessage(3829), 0);
  const std::vector<std::string> groups = Singleton<CReputeMgr>::GetInstance()->GetGroupNames();
  for (auto it = groups.begin(); it != groups.end(); ++it) {
    this->m_groupCombo->AddOption(*it, 0);
  }

  this->m_groupCombo->Select(0);
  this->m_reputes = Singleton<CReputeMgr>::GetInstance()->GetAllReputes();
  this->SetPage(1);
}

// OFFSET: 0x006104F0
// FUNCTION: fn_04315
int UIReputeWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 201:
      this->ClearUpdated();
      this->m_frameContext->windows.CloseWindow(346);
      break;
    case 316:
      this->ClearUpdated();
      this->SetPage(this->m_page - 1);
      this->Invalidate();
      break;
    case 317:
      this->ClearUpdated();
      this->SetPage(this->m_page + 1);
      this->Invalidate();
      break;
    case 321: {
      std::string keyword(this->m_searchEdit->Text() ? this->m_searchEdit->Text() : "");
      if (keyword.size() == 0) {
        this->SendMsg(this->m_groupCombo, 39, 0, 0, 0, 0);
      } else {
        this->ClearUpdated();
        this->m_reputes.clear();
        this->m_reputes = Singleton<CReputeMgr>::GetInstance()->FindReputes(keyword);
        this->SetPage(1);
        this->Invalidate();
      }
      break;
    }
    }
    break;
  case 22:
    if (a) {
      int index;
      if (std::string(reinterpret_cast<const char*>(a)) == LookupMessage(3829)) {
        index = 0;
      } else {
        index = this->m_groupCombo->list->FindLine(std::string(reinterpret_cast<const char*>(a)));
        if (index == -1) {
          break;
        }
      }

      this->m_groupCombo->Select(index);
      this->SendMsg(this->m_groupCombo, 39, 0, 0, 0, 0);
    }
    break;
  case 39:
    if (sender == this->m_groupCombo) {
      std::string group = this->m_groupCombo->GetSelectedText();
      if (group.size() != 0) {
        this->m_searchEdit->SetText("");
        this->ClearUpdated();
        this->FilterByGroup(group);
        this->SetPage(1);
        this->Invalidate();
      }
    }
    break;
  case 136: {
    std::string keyword(this->m_searchEdit->Text() ? this->m_searchEdit->Text() : "");
    if (keyword.size() == 0) {
      std::string group = this->m_groupCombo->GetSelectedText();
      if (group.size() == 0) {
        break;
      }

      this->FilterByGroup(group);
    } else {
      this->m_reputes.clear();
      this->m_reputes = Singleton<CReputeMgr>::GetInstance()->FindReputes(keyword);
    }

    this->SetPage(this->m_page);
    this->Invalidate();
    break;
  }
  }

  return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x006109D0
// FUNCTION: fn_04316
void UIReputeWnd::FilterByGroup(const std::string& group) {
  this->m_reputes.clear();
  if (group == LookupMessage(3829)) {
    this->m_reputes = Singleton<CReputeMgr>::GetInstance()->GetAllReputes();
    this->m_groupSelected = false;
    return;
  }

  this->m_reputes = Singleton<CReputeMgr>::GetInstance()->GetGroupReputes(group);
  this->m_groupSelected = true;
  std::int64_t total = 0;
  for (auto it = this->m_reputes.begin(); it != this->m_reputes.end(); ++it) {
    const ReputeInfo* info = *it;
    if (info) {
      total += info->point;
    }
  }

  this->m_totalPoints = total;
}

// OFFSET: 0x00610BE0
// FUNCTION: fn_04317
void UIReputeWnd::SetPage(unsigned int page) {
  if (this->m_reputes.empty()) {
    this->m_pageCount = 1;
  } else {
    this->m_pageCount = (this->m_reputes.size() - 1) / this->m_perPage + 1;
  }

  this->m_page = std::max(1u, std::min(this->m_pageCount, page));
  this->m_prevButton->Enable(this->m_page > 1);
  this->m_nextButton->Enable(this->m_page < this->m_pageCount);
}

// OFFSET: 0x00610C80
// FUNCTION: fn_04318
void UIReputeWnd::ClearUpdated() {
  for (unsigned int i = 0; i < this->m_perPage; ++i) {
    const unsigned int index = (this->m_page - 1) * this->m_perPage + i;
    if (index >= this->m_reputes.size()) {
      return;
    }

    const ReputeInfo* info = this->m_reputes[index];
    if (info) {
      const_cast<ReputeInfo*>(info)->updated = false;
    }
  }
}
