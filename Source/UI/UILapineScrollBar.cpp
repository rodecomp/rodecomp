#include "UI/UIIngredientInfoWnd.h"

// OFFSET: 0x005FF810
// FUNCTION: fn_04149
void UIIngredientInfoWnd::Slot10(int argument1, int argument2, std::intptr_t argument3) {
  this->parent->Slot10(argument1, argument2, argument3);
}

// OFFSET: 0x005FF910
// FUNCTION: fn_04152
void UIIngredientInfoWnd::Move(int x, int y) {
  UIWindow::Move(x, y);
}
