#include "UI/Window/UIRebirthWnd.h"

#include "Client/Mode/ModeMgr.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIWindowMgr.h"

#include <windows.h>

#include <string>

// OFFSET: 0x00924A60
// FUNCTION: fn_12647
void UIRebirthWnd::Slot15(int width, int height) {
  const std::string path = "유저인터페이스\\";
  const char* const names[5][5] = {
    {"esc_05a", "esc_05b", "esc_05c"},
    {"esc_04a", "esc_04b", "esc_04c"},
    {"esc_01a", "esc_01b", "esc_01c"},
    {"esc_03a", "esc_03b", "esc_03c"},
    {"esc_02a", "esc_02b", "esc_02c"},
  };
  const int ids[5] = {353, 220, 252, 251, 185};
  const RECT positions[5] = {{29, 22}, {29, 45}, {29, 68}, {29, 91}, {29, 119}};

  for (int index = 0; index < 5; ++index) {
    UIBitmapButton* button = new UIBitmapButton(*this->m_context, *this->m_controls);
    button->SetBitmap((path + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((path + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((path + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index].left, positions[index].top);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->m_message0 = 252;
}

// OFFSET: 0x00938E30
// FUNCTION: fn_12753
int UIRebirthWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  CMode* mode = g_modeMgr.GetGameMode();
  if (message != 6) {
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }

  switch (argument1) {
  case 185:
    g_windowMgr.CloseWindow(129);
    break;
  case 220:
    mode->SendMsg(25, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(129);
    break;
  case 251:
    g_windowMgr.CloseWindow(129);
    mode->SendMsg(88, 0, 0, 0, 0);
    break;
  case 252:
    mode->SendMsg(25, 1, 0, 0, 0);
    g_windowMgr.CloseWindow(129);
    g_windowMgr.CloseWindow(269);
    break;
  case 353:
    mode->SendMsg(250, 0, 0, 0, 0);
    g_windowMgr.CloseWindow(129);
    break;
  }
  return 0;
}

// OFFSET: 0x00911950
// FUNCTION: fn_12521
UIRebirthWnd::UIRebirthWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls) : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
}
