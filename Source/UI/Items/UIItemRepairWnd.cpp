#include "UI/Items/UIItemRepairWnd.h"

#include "Contents/MessageTable.h"
#include "UI/Items/UIMakeTargetListWnd.h"
#include "UI/Items/UIMakeTargetProcessWnd.h"
#include "UI/Items/UIMakeTargetResultWnd.h"
#include "UI/Items/UIWeaponRefineWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIListBox.h"

#include <string>

// OFFSET: 0x009A3860
// FUNCTION: fn_13609
void UIItemRepairWnd::Paint() {
  auto& controls = *this->m_controls;
  const int titleHeight = this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(812), 0);
  this->DrawPanelBackground(titleHeight, 0, "");
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x009A0B00
// FUNCTION: fn_13604
void UIItemRepairWnd::Slot15(int width, int height) {
  this->m_list = new UIListBox(*this->m_context, *this->m_listContext);
  this->m_list->Configure(12, 22, width - 24, height - 55, 0);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->m_list->SetId(184);
  this->AddChild(this->m_list);

  const std::string prefix("유저인터페이스\\");
  int positions[2][2] = {{width - 91, height - 24}, {width - 46, height - 24}};
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};

  for (int i = 0; i < 2; ++i) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_message0 = 184;
}

// OFFSET: 0x009A1590
// FUNCTION: fn_13605
void UIMakeTargetListWnd::Slot15(int width, int height) {
  this->m_list = new UIListBox(*this->m_context, *this->m_listContext);
  this->m_list->Configure(12, 22, width - 24, height - 55, 0);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->m_list->SetId(184);
  this->AddChild(this->m_list);

  const std::string prefix("유저인터페이스\\");
  int positions[2][2] = {{width - 91, height - 24}, {width - 46, height - 24}};
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};

  for (int i = 0; i < 2; ++i) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_message0 = 184;
}

// OFFSET: 0x009A2020
// FUNCTION: fn_13606
void UIMakeTargetProcessWnd::Slot15(int width, int height) {
  const std::string prefix("유저인터페이스\\");
  int positions[2][2] = {{width - 91, height - 24}, {width - 46, height - 24}};
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};

  for (int i = 0; i < 2; ++i) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_message0 = 184;
}

// OFFSET: 0x009A29B0
// FUNCTION: fn_13607
void UIMakeTargetResultWnd::Slot15(int width, int height) {
  const std::string prefix("유저인터페이스\\");

  auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
  button->SetBitmap((prefix + "btn_ok" + ".bmp").c_str(), 0, 0);
  button->SetBitmap((prefix + "btn_ok_a" + ".bmp").c_str(), 1, 0);
  button->SetBitmap((prefix + "btn_ok_b" + ".bmp").c_str(), 2, 0);
  button->SetSize(button->BitmapWidth(), button->BitmapHeight());
  button->Move(width - 91, height - 24);
  button->SetId(184);
  this->AddChild(button);

  this->m_message0 = 184;
}

// OFFSET: 0x009A2DD0
// FUNCTION: fn_13608
void UIWeaponRefineWnd::Slot15(int width, int height) {
  this->m_list = new UIListBox(*this->m_context, *this->m_listContext);
  this->m_list->Configure(12, 22, width - 24, height - 55, 0);
  this->m_list->backgroundRed = 240;
  this->m_list->backgroundGreen = 240;
  this->m_list->backgroundBlue = 240;
  this->m_list->SetId(184);
  this->AddChild(this->m_list);

  const std::string prefix("유저인터페이스\\");
  int positions[2][2] = {{width - 91, height - 24}, {width - 46, height - 24}};
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};

  for (int i = 0; i < 2; ++i) {
    auto* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((prefix + names[i][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[i][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[i][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[i][0], positions[i][1]);
    button->SetId(ids[i]);
    this->AddChild(button);
  }

  this->m_message0 = 184;
}
