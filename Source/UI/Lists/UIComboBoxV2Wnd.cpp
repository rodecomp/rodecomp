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
  const char* states[] = {"normal", "mouseon", "pressed", "disabled"};
  this->auxiliaryWindow = new UIBitmapButton();
  for (int state = 0; state < 4; ++state) {
    this->auxiliaryWindow->SetBitmap((prefix + states[state] + ".bmp").c_str(), state, 0);
  }

  this->auxiliaryWindow->SetSize(this->auxiliaryWindow->BitmapWidth(), this->auxiliaryWindow->BitmapHeight());
  this->auxiliaryWindow->Move(width - this->auxiliaryWindow->BitmapWidth(), 0);
  this->auxiliaryWindow->SetId(273);
  this->AddChild(this->auxiliaryWindow);

  this->list = new UIComboBoxEx();
  this->list->SetSize(width, 20);
  this->list->Move(0, this->auxiliaryWindow->BitmapHeight());
  this->AddChild(this->list);
  this->SetSize(width, height);
}
