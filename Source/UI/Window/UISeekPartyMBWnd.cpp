#include "UI/Window/UISeekPartyMBWnd.h"
#include "Contents/MessageTable.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Lists/UIComboBoxDefWnd.h"

// OFFSET: 0x00944370
// FUNCTION: fn_12831
void UISeekPartyMBWnd::CreateJobChecks() {
  int counts[8] = {4, 6, 13, 6, 7, 6, 13, 8};
  int message = 0;
  int starts[8] = {1, 5, 11, 24, 30, 37, 43, 56};

  for (int i = 0; i < counts[0]; ++i) {
    UICheckBox* check = new UICheckBox(*this->m_context, *this->m_controls);
    check->SetSize(110, 12);
    const int job = i + 1;
    check->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(0x6AF + job));
    if (i < 8) {
      check->Move(54, 99 + i * 25);
    } else {
      check->Move(180, 99 + (i - 8) * 25);
    }
    check->SetChecked(0);
    check->SetHidden(true);
    check->m_disabled = 1;
    this->AddChild(check);
    this->m_jobChecks[0].emplace(static_cast<unsigned short>(job), check);
  }

  for (int category = 1; category < 8; ++category) {
    for (int i = 0; i < counts[category]; ++i) {
      UICheckBox* check = new UICheckBox(*this->m_context, *this->m_controls);
      check->SetSize(110, 12);
      check->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(0x65D + message));
      if (i < 8) {
        check->Move(54, 99 + i * 25);
      } else {
        check->Move(180, 99 + (i - 8) * 25);
      }
      check->SetChecked(0);
      check->SetHidden(true);
      check->m_disabled = 1;
      this->AddChild(check);
      this->m_jobChecks[category].emplace(static_cast<unsigned short>(starts[category] + i), check);
      ++message;
    }
  }

  this->ShowJobCategory(0);
}

// OFFSET: 0x009447D0
// FUNCTION: fn_12832
void UISeekPartyMBWnd::InitJobCategoryCombo() {
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DB), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DC), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DD), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DE), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DF), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E0), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E1), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E2), 0);
  this->m_jobCategoryCombo->Select(0);
}

// OFFSET: 0x00946150
// FUNCTION: fn_12838
void UISeekPartyMBWnd::ShowJobCategory(int category) {
  if (category == this->m_jobCategory) {
    return;
  }

  if (this->m_jobCategory != -1) {
    for (int i = 0; i < 8; ++i) {
      std::map<unsigned short, UICheckBox*>& checks = this->m_jobChecks[i];
      auto it = checks.begin();
      if (it == checks.end()) {
        return;
      }

      for (; it != checks.end(); ++it) {
        UICheckBox* check = it->second;
        check->SetHidden(true);
        check->m_disabled = 1;
      }
    }
  }

  std::map<unsigned short, UICheckBox*>& checks = this->m_jobChecks[category];
  auto it = checks.begin();
  if (it == checks.end()) {
    return;
  }

  for (; it != checks.end(); ++it) {
    UICheckBox* check = it->second;
    check->SetHidden(false);
    check->m_disabled = 0;
  }

  this->m_jobCategory = category;
  this->Invalidate();
}
