#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIItemStoreComboBox.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIWindow.h"

#include <cstdio>
#include <string>

// OFFSET: 0x0086D530

// OFFSET: 0x00886200
// FUNCTION: fn_11325
void UIItemStoreComboBoxWnd::Slot15(int, int) {
  std::string path = "유저인터페이스\\basic_interface\\";
  const char* pressed = this->pink == 1 ? "combo_btn_pink_b" : "combo_btn_b";
  const char* disabled = this->pink == 1 ? "combo_btn_pink_c" : "combo_btn_c";

  auto& controls = this->m_controls->buttons;
  this->button = new UIBitmapButton(*this->m_context, controls);
  this->button->SetBitmap((path + "combo_btn_a" + ".bmp").c_str(), 0, 0);
  this->button->SetBitmap((path + pressed + ".bmp").c_str(), 1, 0);
  this->button->SetBitmap((path + disabled + ".bmp").c_str(), 2, 0);
  this->button->SetSize(this->button->BitmapWidth(), this->button->BitmapHeight());
  this->button->Move(this->geometry.width - this->button->BitmapWidth(), this->geometry.height - this->button->BitmapHeight());
  this->button->SetId(273);
  this->AddChild(this->button);

  UIItemStoreComboBox* items = new UIItemStoreComboBox(*this->m_context, this->m_controls->combo);
  this->list = items;
  items->pink = this->pink != 0;
  this->list->SetSize(62, 45);
  this->list->Move(0, 0);
  this->list->Show(0);
  this->AddChild(this->list);

  char text[12] = {};
  std::sprintf(text, "(NO SELECT)");
  this->label = new UIStaticText(*this->m_context, controls);
  this->label->SetSize(this->geometry.width - this->button->BitmapWidth(), this->button->BitmapHeight());
  this->label->SetText(text, 0);
  this->label->Move(2, 3);
  this->label->shadow = 1;
  this->AddChild(this->label);
}

// OFFSET: 0x0086D590
// FUNCTION: fn_11048
UIItemStoreComboBoxWnd::UIItemStoreComboBoxWnd(UIWindowContext& windows, UIComboBoxHeaderContext& controls) : UIComboBoxDefWnd(windows, controls), pink(0) {
}
