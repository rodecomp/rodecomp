#include "UI/Edit/UIEditBoxEx.h"
#include "Network/TextEncoding.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Lists/UIScrollBar.h"

#include <cstdlib>
#include <windows.h>

// OFFSET: 0x009E4310

// OFFSET: 0x009E4330

// OFFSET: 0x009E43D0

// OFFSET: 0x009E45E0

// OFFSET: 0x009E4970

// OFFSET: 0x009E4A30

// OFFSET: 0x009E4B20

// OFFSET: 0x009E4B50
// FUNCTION: fn_14440
void CUIEditCtrlEx::Invalidate() {
  this->Root()->InvalidateChildren();
}

// OFFSET: 0x009E4B60

// OFFSET: 0x009E4B80

// OFFSET: 0x009E4BD0
// FUNCTION: fn_14443
void CUIEditCtrlEx::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->PaintText();
}

// OFFSET: 0x009E4F90
