#include "UI/Window/UINoteWnd.h"
#include "UI/Window/UIEnums.h"
#include "Contents/MessageTable.h"
#include "UI/Buttons/UIBasicButton.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText_LF.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"

#include "Resource/MSGSTRINGID.h"
// OFFSET: 0x0094C2D0
// FUNCTION: fn_12895
void UINoteWnd::OnCreate(int width, int height) {
  UIBasicButton* button = new UIBasicButton();
  button->Configure(60, LookupMessage(MSI_ZENY_LOTTO_BUY_10));
  button->Move(width - 68, height - 26);
  button->SetId(201);
  this->AddChild(button);

  this->m_scroll = new UINoteWnd_Scroll(width - 35, height - 59);
  this->m_scroll->Move(16, 16);
  this->AddChild(this->m_scroll);
}

// OFFSET: 0x0094D870
// FUNCTION: fn_12907
void UINoteWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFFFFFF);
  DrawRoundedBorder(*this, 2, 2, this->geometry.width - 4, this->geometry.height - 4, 0xC8C8C8);
  this->FillRegion(8, 8, this->geometry.width - 16, this->geometry.height - 38, 0xF7F7F7);
}

// OFFSET: 0x0094D8E0
// FUNCTION: fn_12908
void UINoteWnd_Scroll::Paint() {
  this->ClearSurface(0xF7F7F7);
}

// OFFSET: 0x0094EE80
// FUNCTION: fn_12919
void UINoteWnd_Scroll::OnLBtnDown(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnDown(x + 23, y + 27);
  }
}

// OFFSET: 0x0094EF80
// FUNCTION: fn_12924
void UINoteWnd_Scroll::OnLBtnUp(int x, int y) {
  if (this->parent) {
    this->parent->OnLBtnUp(x + 23, y + 27);
  }
}

// OFFSET: 0x0094F2C0
// FUNCTION: fn_12935
void UINoteWnd_Scroll::OnWheel(int delta) {
  if (this->wheelScroll == 1) {
    if (delta < 0) {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, 12, 0, 0, 0);
    } else {
      this->SendMsg(this->scrollBar, UM_SCROLLVERT, -12, 0, 0, 0);
    }
  }
}

// OFFSET: 0x00946D00
// FUNCTION: fn_12852
UINoteWnd::UINoteWnd()
    : UIFrameWnd(0), m_scroll(nullptr) {
}

// OFFSET: 0x00950840
// FUNCTION: fn_12944
int UINoteWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == UM_COMMAND) {
    if (argument1 == ID_CLOSE) {
      g_windowMgr.CloseWindow(this->geometry.id);
    }
  } else if (message == UM_SETINFO) {
    UINoteWnd_Scroll* scroll = this->m_scroll;
    const std::string text = LookupMessage(static_cast<int>(argument1));
    UIStaticText_LF* label = new UIStaticText_LF(0);
    scroll->m_text = label;
    label->SetSize(scroll->geometry.width - 13, 12);
    label->Move(0, 0);
    label->SetText(text);
    scroll->AddScrollChild(label);
    scroll->horizontalBar->Show(0);
    scroll->scrollBar->Show(0);
    scroll->Invalidate();
    this->Invalidate();
  }

  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
