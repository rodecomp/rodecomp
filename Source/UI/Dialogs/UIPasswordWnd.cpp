#include "UI/Dialogs/UIPasswordWnd.h"

#include "Contents/MessageTable.h"
#include "Security/SecondPwdRandom.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Text/UIStaticText.h"

#include <algorithm>
#include <string>

// GLOBAL: gl_25104
int g_passwordWndMode;

// OFFSET: 0x0090C970
// FUNCTION: fn_12451
void UIPasswordWnd::sub_90C970(int value) {
  this->unknown_011C = value;
  if (value == 0) {
    this->unknown_0164->SetText(LookupMessage(0xa06), 0);
  }
}

// OFFSET: 0x0090C9B0
// FUNCTION: fn_12452
void UIPasswordWnd::sub_90C9B0(int) {}

// OFFSET: 0x0090C9C0
// FUNCTION: fn_12453
void UIPasswordWnd::CreateKeypad(unsigned int seed) {
  this->unknown_00F8 = 91;
  this->unknown_00FC = 91;
  this->unknown_0100 = 130;
  this->unknown_0104 = 168;
  for (int i = 0; i < 10; ++i) {
    this->m_digits[i] = static_cast<unsigned char>(i);
  }

  std::shuffle(this->m_digits, this->m_digits + 10, CSecondPwdRandom(seed));

  std::string prefix = "유저인터페이스\\";
  const char* const commandNames[5][5] = {
    {"btn_big_ok", "btn_big_ok_a", "btn_big_ok_b", "btn_big_ok_b", nullptr},
    {"btn_big_change", "btn_big_change_a", "btn_big_change_b", "btn_big_change_c", nullptr},
    {"btn_big_unused", "btn_big_unused_a", "btn_big_unused_b", "btn_big_unused_c", nullptr},
    {"btn_big_next", "btn_big_next_a", "btn_big_next_b", "btn_big_next_b", nullptr},
    {"btn_big_cancel", "btn_big_cancel_a", "btn_big_cancel_b", "btn_big_cancel_b", nullptr},
  };
  const int commandIds[5] = {184, 292, 419, 218, 185};
  const int commandPositions[5][2] = {{414, 192}, {414, 220}, {414, 248}, {414, 192}, {414, 276}};

  for (int i = 0; i < 5; ++i) {
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->unknown_0150[i] = button;
    button->SetBitmap((prefix + commandNames[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + commandNames[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + commandNames[i][2] + ".bmp").c_str(), 2, 0);
    button->SetBitmap((prefix + commandNames[i][3] + ".bmp").c_str(), 3, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(commandPositions[i][0], commandPositions[i][1]);
    button->SetId(commandIds[i]);
    this->AddChild(button);
  }

  prefix = "유저인터페이스\\basic_interface\\";
  const int keyIds[11] = {293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303};
  const char* const keyNames[11][3] = {
    {"0-a", "0-b", "0-c"}, {"1-a", "1-b", "1-c"}, {"2-a", "2-b", "2-c"}, {"3-a", "3-b", "3-c"},
    {"4-a", "4-b", "4-c"}, {"5-a", "5-b", "5-c"}, {"6-a", "6-b", "6-c"}, {"7-a", "7-b", "7-c"},
    {"8-a", "8-b", "8-c"}, {"9-a", "9-b", "9-c"}, {"reset-a", "reset-b", "reset-c"},
  };
  const int keyPositions[11][2] = {
    {219, 78}, {279, 78}, {339, 78}, {219, 138}, {279, 138}, {339, 138}, {219, 198}, {279, 198}, {339, 198}, {219, 258}, {279, 258},
  };

  for (int i = 0; i < 10; ++i) {
    const int digit = this->m_digits[i];
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->unknown_0124[i] = button;
    button->SetBitmap((prefix + keyNames[digit][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + keyNames[digit][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + keyNames[digit][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(keyPositions[i][0], keyPositions[i][1]);
    button->SetId(keyIds[digit]);
    this->AddChild(button);
  }

  UIBitmapButton* reset = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->unknown_0124[10] = reset;
  reset->SetBitmap((prefix + keyNames[10][0] + ".bmp").c_str(), 0, 0);
  reset->SetBitmap((prefix + keyNames[10][1] + ".bmp").c_str(), 1, 0);
  reset->SetBitmap((prefix + keyNames[10][2] + ".bmp").c_str(), 2, 0);
  reset->SetSize(reset->BitmapWidth(), reset->BitmapHeight());
  reset->Move(keyPositions[10][0], keyPositions[10][1]);
  reset->SetId(keyIds[10]);
  this->AddChild(reset);

  this->unknown_0150[0]->Enable(0);
  if (g_passwordWndMode == 2) {
    this->unknown_0150[0]->Enable(1);
    this->unknown_0150[3]->Enable(0);
  } else if (g_passwordWndMode == 4 || g_passwordWndMode == 5) {
    UIButton* button = this->unknown_0150[1];
    button->SetDisabled(1);
    button->Invalidate();
    button = this->unknown_0150[2];
    button->SetDisabled(1);
    button->Invalidate();
  }
  this->unknown_0150[2]->Enable(0);

  this->unknown_0164 = new UIStaticText(*this->m_context, *this->m_controls);
  this->unknown_0164->SetSize(300, 15);
  this->unknown_0164->SetText(LookupMessage(0x771), 0);
  this->unknown_0164->Move(200, 30);
  this->AddChild(this->unknown_0164);

  UIStaticText* notice = new UIStaticText(*this->m_context, *this->m_controls);
  notice->SetSize(320, 15);
  notice->SetText(LookupMessage(0x772), 0);
  notice->Move(200, 50);
  this->AddChild(notice);
}

// OFFSET: 0x0090DE10
// FUNCTION: fn_12454
void UIPasswordWnd::sub_90DE10() {
  UIButton** slot = this->unknown_0124;
  for (int remaining = 11; remaining != 0; --remaining) {
    UIButton* button = *slot;
    button->SetDisabled(0);
    button->Invalidate();
    ++slot;
  }

  slot = this->unknown_0150;
  for (int remaining = 5; remaining != 0; --remaining) {
    UIButton* button = *slot;
    button->SetDisabled(0);
    button->Invalidate();
    ++slot;
  }

  if (g_passwordWndMode == 4 || g_passwordWndMode == 5) {
    UIButton* button = this->unknown_0150[1];
    button->SetDisabled(1);
    button->Invalidate();
    button = this->unknown_0150[2];
    button->SetDisabled(1);
    button->Invalidate();
  }
}
