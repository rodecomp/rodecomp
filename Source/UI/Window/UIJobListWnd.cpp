#include "UI/Window/UIJobListWnd.h"

#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "UI/Buttons/UIButton.h"

#include <windows.h>

// OFFSET: 0x0092C640
// FUNCTION: fn_12664
void UIJobListWnd::Paint() {
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, "", 0);
  unsigned red, green, blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));
  this->DrawButtonBar(2, 0, 1);
}
