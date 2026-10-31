#include "UI/Item/UIItemStoreWnd.h"
#include "UI/Text/UIStaticText.h"

#include <string>

// OFFSET: 0x00D532D0
// FUNCTION: fn_21919
void UIItemStoreWnd::SetTitle(std::string title) {
  this->title->SetTextFitted(title.c_str());
}
