#include "UI/Window/UISerachStoreInfoWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x009121F0
// FUNCTION: fn_12527
UISerachStoreInfoWnd::UISerachStoreInfoWnd()
    : UIFrameWnd(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIFrameContext*>(nullptr), 0), unknown_00DC(1), unknown_00E0(-1),
      unknown_00E4(0), unknown_00E8(25), unknown_00EC(0), unknown_00ED(1) {
  this->unknown_00F0.clear();
}

// OFFSET: 0x009296E0
// FUNCTION: fn_12653
void UISerachStoreInfoWnd::Slot15(int width, int height) {
  UIRadioBtn** radio = this->m_radios;
  for (int x = 10; x < 90; x += 40) {
    *radio = new UIRadioBtn(*this->m_context, *this->m_controls);
    (*radio)->SetSize(this->geometry.width / 2, 12);
    (*radio)->Move(x, 40);
    this->AddChild(*radio);
    ++radio;
  }
  this->m_radios[0]->SetText(LookupMessage(0x6F2));
  this->m_radios[1]->SetText(LookupMessage(0x6F3));
  this->m_radios[this->unknown_00E4]->SendMsg(this, 0xD, 1, 0, 0, 0);

  this->m_edits[1] = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edits[1]->SetSize(0x4C, 0x10);
  this->m_edits[1]->Move(100, 0x24);
  this->m_edits[1]->maximum = 0x32;
  this->m_edits[1]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[1]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[1]->flat = 0;
  this->AddChild(this->m_edits[1]);

  this->m_edits[0] = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edits[0]->SetSize(0x4C, 0x10);
  this->m_edits[0]->Move(0xBB, 0x24);
  this->m_edits[0]->maximum = 0x32;
  this->m_edits[0]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[0]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[0]->flat = 0;
  UIEditCtrl* edit = this->m_edits[0];
  if (const char* tooltip = LookupMessage(0x709)) {
    edit->tooltip = tooltip;
  }
  this->m_edits[0]->disabled = 1;
  this->m_edits[0]->Invalidate();
  this->m_edits[0]->invalidatesParent = 1;
  this->AddChild(this->m_edits[0]);

  this->m_edits[2] = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edits[2]->SetSize(0x4C, 0x10);
  this->m_edits[2]->Move(0x113, 0x24);
  this->m_edits[2]->maximum = 0x32;
  this->m_edits[2]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[2]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[2]->flat = 0;
  this->m_edits[2]->disabled = 1;
  this->m_edits[2]->Invalidate();
  this->AddChild(this->m_edits[2]);

  this->m_edits[3] = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_edits[3]->SetSize(0x4C, 0x10);
  this->m_edits[3]->Move(0x16C, 0x24);
  this->m_edits[3]->maximum = 0x32;
  this->m_edits[3]->Background(0xE8, 0xE8, 0xE8);
  this->m_edits[3]->Border(0xC0, 0xC0, 0xC0);
  this->m_edits[3]->flat = 0;
  this->m_edits[3]->disabled = 1;
  this->m_edits[3]->Invalidate();
  this->AddChild(this->m_edits[3]);

  UICheckBox* check = new UICheckBox(*this->m_context, *this->m_controls);
  check->SetSize(0xA0, 0xC);
  check->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(0x6FC));
  check->Move(100, 0x3C);
  check->SetChecked(this->unknown_00ED);
  this->AddChild(check);

  {
    std::string path = "유저인터페이스\\";
    this->m_findButton = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_findButton->SetBitmap((path + "btn_find" + ".bmp").c_str(), 0, 0);
    this->m_findButton->SetBitmap((path + "btn_find_a" + ".bmp").c_str(), 1, 0);
    this->m_findButton->SetBitmap((path + "btn_find_b" + ".bmp").c_str(), 2, 0);
    this->m_findButton->SetSize(this->m_findButton->BitmapWidth(), this->m_findButton->BitmapHeight());
    this->m_findButton->Move(this->geometry.width - 0x2D, 0x38);
    this->m_findButton->SetId(0x16D);
    this->AddChild(this->m_findButton);
  }

  this->m_prevButton = new UITextButton(*this->m_context, *this->m_controls);
  this->m_prevButton->Configure(LookupMessage(0x41D), 0x8E8E8E, 0, 0xC, 0, 0);
  this->m_prevButton->m_hoverColor = 0xEFB700;
  this->m_prevButton->Move(
      this->geometry.width / 2 - this->TextWidth(this->m_context->fonts, this->m_context->locale, LookupMessage(0x41D), 0, 0, 0xC, 0, 0) - 0x14,
      this->geometry.height - 0x15);
  this->m_prevButton->SetTooltip(LookupMessage(0x41D));
  this->m_prevButton->SetId(0x13C);
  this->AddChild(this->m_prevButton);

  this->m_nextButton = new UITextButton(*this->m_context, *this->m_controls);
  this->m_nextButton->Configure(LookupMessage(0x41E), 0x8E8E8E, 0, 0xC, 0, 0);
  this->m_nextButton->m_hoverColor = 0xEFB700;
  this->m_nextButton->Move(this->geometry.width / 2 + 0x14, this->geometry.height - 0x15);
  this->m_nextButton->SetTooltip(LookupMessage(0x41E));
  this->m_nextButton->SetId(0x13D);
  this->AddChild(this->m_nextButton);

  this->AddBaseButton(*this->m_controls);
  this->AddCloseButton(*this->m_controls, "", 0);
  this->SendMsg(this, 0x3C, 0, 0, 0, 0);
  InputFocus{g_windowMgr.hoverWindow}.Set(this->m_edits[1]);
  this->m_message0 = 0x16D;
}
