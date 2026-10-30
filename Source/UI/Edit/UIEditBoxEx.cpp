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

// OFFSET: 0x009E5A60
// FUNCTION: fn_14447
void CUIEditCtrlEx::ResetDragState() {
  this->unknown_01D4 = -1;
  this->unknown_01D8 = 0;
  this->unknown_01DC = 0;
}

// OFFSET: 0x009E5A80
// FUNCTION: fn_14448
void CUIEditBoxEx::Resize(int width, int height) {
  UIWindow::Resize(width, height);
  this->edit->Resize(width - this->editRight - this->editLeft, height - this->editBottom - this->editTop);
  this->edit->Move(this->editLeft, this->editTop);
}

// OFFSET: 0x009E5AE0
// FUNCTION: fn_14449
int CUIEditBoxEx::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  if (message == 0 && this->onSubmit) {
    this->onSubmit();
  }
  return UIWindow::SendMsg(sender, message, a, b, c, d);
}

// OFFSET: 0x009E6090
// FUNCTION: fn_14451
void CUIEditBoxEx::SetAlignment(int value) {
  CUIEditCtrlEx* edit = this->edit;
  edit->alignment = value;
  edit->Invalidate();
}

// OFFSET: 0x009E60D0
// FUNCTION: fn_14452
void CUIEditBoxEx::SetBackgroundColor(std::uint32_t color) {
  this->backgroundColor = color;
  this->Invalidate();
}

// OFFSET: 0x009E6320
// FUNCTION: fn_14454
void CUIEditBoxEx::SetTextColor(std::uint32_t color) {
  CUIEditCtrlEx* edit = this->edit;
  edit->normalColor = color;
  edit->textColor = color;
  edit->selectedAll = 1;
}

// OFFSET: 0x009E6370
// FUNCTION: fn_14455
void CUIEditBoxEx::SetHighlight(int value) {
  CUIEditCtrlEx* edit = this->edit;
  edit->unknown_018C = value;
  edit->unknown_0190 = 1;
  edit->Invalidate();
}

// OFFSET: 0x009E63C0
// FUNCTION: fn_14456
void CUIEditBoxEx::SetBorderColor(std::uint32_t color) {
  this->borderColor = color;
  this->Invalidate();
}

// OFFSET: 0x009E63E0
// FUNCTION: fn_14457
void CUIEditBoxEx::SetBorderWidth(int width) {
  this->borderWidth = width;
  this->Invalidate();
}

// OFFSET: 0x009E6430
// FUNCTION: fn_14458
void CUIEditBoxEx::SetMaximum(int value) {
  this->edit->maximum = value;
  this->edit->SetText(this->edit->Text());
}

// OFFSET: 0x009E6470
// FUNCTION: fn_14459
void CUIEditBoxEx::SetEditBounds(int left, int top, int right, int bottom) {
  this->editLeft = left;
  this->editTop = top;
  this->editRight = right;
  this->editBottom = bottom;
  this->Resize(this->geometry.width, this->geometry.height);
}

// OFFSET: 0x009E64B0
// FUNCTION: fn_14460
void CUIEditBoxEx::SetMask(char value) {
  this->edit->mask = value;
}

// OFFSET: 0x009E64D0
// FUNCTION: fn_14461
void CUIEditBoxEx::SetText(const std::string& text) {
  this->edit->SetText(text.c_str());
}
