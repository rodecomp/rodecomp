#include "UI/UIReputeWnd.h"
#include "Contents/MessageTable.h"
#include "Contents/ReputeMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIComboBoxEx.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <new>
#include <string>

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

// OFFSET: 0x0060F8F0
// FUNCTION: fn_04314
void UIReputeWnd::Paint() {
  auto& controls = *this->m_controls;
  this->ClearSurface(0xFFFFFFFF);
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, false, LookupMessage(3827), 0);
  const int barHeight = this->DrawButtonBar(2, 0, 1);
  DrawRectangleBorder(*this, 0, titleHeight - 1, this->geometry.width, this->geometry.height - barHeight - titleHeight + 2, 0xC6C6C6);
  DrawRectangleBorder(*this, 0, titleHeight, this->geometry.width, 1, 0);

  if (Singleton<CReputeMgr>::GetInstance()->m_waitingForPoints) {
    DrawRoundedBorder(*this, 225, 155, 190, 140, 0xCE9484);
    DrawFilledRoundedBorder(*this, 226, 156, 188, 138, 0xFFE7DE);
    DrawFilledRoundedBorder(*this, 231, 161, 178, 128, 0xFFFFFF);
    CBitmapRes* bitmap =
      static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\notice_poring.bmp")));
    this->DrawBitmap(288, 182, &bitmap->image, true);
    this->DrawCenteredText(225, 260, LookupMessage(3830), 190, 0, 15, 0xC89482, 1);
    return;
  }

  for (unsigned int i = 0; i < this->m_perPage; ++i) {
    const unsigned int index = (this->m_page - 1) * this->m_perPage + i;
    if (index >= this->m_reputes.size()) {
      break;
    }

    const ReputeInfo* info = this->m_reputes[index];
    if (!info) {
      continue;
    }

    const int left = (i & 1) * 316;
    const int top = (i >> 1) * 86;
    const char* background = info->updated ? "유저인터페이스\\Reputation\\bg_highlight.bmp" : "유저인터페이스\\Reputation\\bg_info.bmp";
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background)));
    this->DrawBitmap(left + 7, top + 50, &bitmap->image, false);
    this->DrawTextAt(controls.fonts, controls.locale, left + 12, top + 56, info->name.c_str(), 0, 0, 12, 0, 1, 0);

    const std::int64_t point = info->point;
    if (point < 0) {
      const std::int64_t value = -point;
      for (std::uint64_t k = 0; k < info->unknown_0020 / 1000; ++k) {
        bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\Reputation\\light_empty.bmp")));
        this->DrawBitmap(left + 12 + static_cast<int>(k) * 14, top + 72, &bitmap->image, true);
      }

      for (std::uint64_t k = 0; k < value / 1000; ++k) {
        bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\Reputation\\light_red.bmp")));
        this->DrawBitmap(left + 12 + static_cast<int>(k) * 14, top + 72, &bitmap->image, true);
      }

      const std::uint64_t remain = value % 1000;
      this->FillRegion(left + 12, top + 100, static_cast<int>(static_cast<float>(remain) / 1000.0f * 299.0f), 6, 0xFF);
      this->DrawRightAlignedText(left + 311, top + 114, FormatOwnedString(LookupMessage(3828), remain).c_str(), 0, 0, 12, 0, 1);
    } else {
      for (std::uint64_t k = 0; k < info->unknown_0018 / 1000; ++k) {
        bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\Reputation\\light_empty.bmp")));
        this->DrawBitmap(left + 12 + static_cast<int>(k) * 14, top + 72, &bitmap->image, true);
      }

      for (std::uint64_t k = 0; k < info->point / 1000; ++k) {
        bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\Reputation\\light_blue.bmp")));
        this->DrawBitmap(left + 12 + static_cast<int>(k) * 14, top + 72, &bitmap->image, true);
      }

      const std::uint64_t remain = info->point % 1000;
      this->FillRegion(left + 12, top + 100, static_cast<int>(static_cast<float>(remain) / 1000.0f * 299.0f), 6, 0xCD9279);
      this->DrawRightAlignedText(left + 311, top + 114, FormatOwnedString(LookupMessage(3828), remain).c_str(), 0, 0, 12, 0, 1);
    }
  }

  if (this->m_groupSelected) {
    this->FillRegion(375, 26, 256, 16, 0xEFEFEF);
    DrawRectangleBorder(*this, 374, 25, 258, 18, 0x636363);
    this->DrawTextAt(controls.fonts, controls.locale, 377, 29, "GROUP :", 0, 0, 12, 0, 1, 0);
    this->DrawRightAlignedText(627, 29, FormatOwnedString("%I64d P", this->m_totalPoints).c_str(), 0, 0, 12, 0, 1);

    const char* light;
    if (this->m_totalPoints == 0) {
      light = "유저인터페이스\\Reputation\\light_empty.bmp";
    } else if (this->m_totalPoints > 0) {
      light = "유저인터페이스\\Reputation\\light_blue.bmp";
    } else {
      light = "유저인터페이스\\Reputation\\light_red.bmp";
    }

    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(light)));
    this->DrawBitmap(439, 28, &bitmap->image, true);
  }

  const std::string separator = " / ";
  const unsigned int separatorWidth = this->TextWidth(controls.fonts, controls.locale, separator.c_str(), 0, 0, 12, 0, 0);
  this->DrawCenteredText(574, 402, separator.c_str(), 49, 0, 12, 0, 1);
  this->DrawRightAlignedText(static_cast<int>((49 - separatorWidth) * 0.5 + 574.0), 402, std::to_string(this->m_page).c_str(), 0, 0, 12, 0, 1);
  this->DrawTextAt(
    controls.fonts, controls.locale, static_cast<int>((separatorWidth + 49) * 0.5 + 574.0), 402, std::to_string(this->m_pageCount).c_str(), 0, 0, 12, 0, 1, 0
  );
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
