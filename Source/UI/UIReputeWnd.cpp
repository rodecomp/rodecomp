#include "UI/UIReputeWnd.h"
#include "Contents/MessageTable.h"
#include "Contents/ReputeMgr.h"
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

// OFFSET: 0x0060F170
// FUNCTION: fn_04312
bool UIReputeWnd::Slot02() {
  return true;
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
