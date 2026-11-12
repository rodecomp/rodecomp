#include "UI/Skill/UIGraffiStrboxWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"

#include <string>

template __declspec(noinline) std::string::basic_string(std::_String_constructor_concat_tag, const std::string&, const char*, std::size_t, const char*, std::size_t);

// OFFSET: 0x008E1210
// FUNCTION: fn_12174
void UIGraffiStrboxWnd::OnCreate(int, int) {
  this->edit = new UIEditCtrl();
  this->edit->SetSize(240, 16);
  this->edit->Move(22, 50);
  this->edit->maximum = 20;
  this->edit->Background(242, 242, 242);
  this->AddChild(this->edit);
  g_windowMgr.SetFocus(this->edit);

  const std::string prefix("유저인터페이스\\");
  const char* names[2][3] = {{"btn_ok", "btn_ok_a", "btn_ok_b"}, {"btn_cancel", "btn_cancel_a", "btn_cancel_b"}};
  int ids[2] = {184, 185};
  int positions[2][2] = {{189, 96}, {234, 96}};

  for (int index = 0; index < 2; ++index) {
    auto* button = new UIBitmapButton();
    button->SetBitmap((prefix + names[index][0] + ".bmp").c_str(), 0, 0);
    button->SetBitmap((prefix + names[index][1] + ".bmp").c_str(), 1, 0);
    button->SetBitmap((prefix + names[index][2] + ".bmp").c_str(), 2, 0);
    button->SetSize(button->BitmapWidth(), button->BitmapHeight());
    button->Move(positions[index][0], positions[index][1]);
    button->SetId(ids[index]);
    this->AddChild(button);
  }

  this->m_message0 = 184;
}

// OFFSET: 0x008C7C50
// FUNCTION: fn_11942
UIGraffiStrboxWnd::UIGraffiStrboxWnd()
    : UIFrameWnd(0) {
}
