#include "UI/Lists/UIComboBoxV2Wnd.h"

#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Lists/UIComboBoxEx.h"

// OFFSET: 0x0086CF40
// FUNCTION: fn_11039
UIComboBoxV2Wnd::UIComboBoxV2Wnd() : UIWindow(0), selected(-1), auxiliaryWindow(nullptr), list(nullptr) {
}

// OFFSET: 0x0087C7C0
// FUNCTION: fn_45607
void UIComboBoxV2Wnd::Initialize(int width, int height, std::string prefix) {
  this->auxiliaryWindow = new UIBitmapButton();
  const char* states[4] = {"normal", "mouseon", "pressed", "disabled"};
  for (int state = 0; state <= 3; ++state) {
    this->auxiliaryWindow->SetBitmap((prefix + states[state] + ".bmp").c_str(), state, 0);
  }

  const int buttonWidth = this->auxiliaryWindow->BitmapWidth();
  const int buttonHeight = this->auxiliaryWindow->BitmapHeight();
  this->auxiliaryWindow->SetSize(buttonWidth, buttonHeight);
  this->auxiliaryWindow->Move(width - buttonWidth, 0);
  this->auxiliaryWindow->SetId(273);
  this->AddChild(this->auxiliaryWindow);

  this->list = new UIComboBoxEx();
  this->list->SetSize(width, 20);
  this->list->Move(0, buttonHeight);
  this->AddChild(this->list);
  this->SetSize(width, height);
}
