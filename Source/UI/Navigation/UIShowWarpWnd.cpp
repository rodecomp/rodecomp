#include "UI/Navigation/UIShowWarpWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "Client/Replay/Replay.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <string>

// OFFSET: 0x0094CA20
// FUNCTION: fn_12899
void UIShowWarpWnd::OnCreate(int width, int height) {
  this->m_list = new UIListBox();
  width -= 0x18;
  height -= 0x32;
  if (width < 0x14) {
    width = 0x14;
  }
  if (height < 0x1e) {
    height = 0x1e;
  }
  this->m_list->Configure(0x0c, 0x14, width, height, 0);
  this->m_list->backgroundRed = 0xf0;
  this->m_list->backgroundGreen = 0xf0;
  this->m_list->backgroundBlue = 0xf0;
  this->m_list->selected = -1;
  this->m_list->Invalidate();
  this->m_list->selectable = 0;
  this->AddChild(this->m_list);

  const std::string prefix = "유저인터페이스\\";
  const int closeX = this->geometry.width - 0x2e;
  const int closeY = this->geometry.height - 0x18;
  auto* close = new UIBitmapButton();
  close->SetBitmap((prefix + "btn_close" + ".bmp").c_str(), 0, 0);
  close->SetBitmap((prefix + "btn_close_a" + ".bmp").c_str(), 1, 0);
  close->SetBitmap((prefix + "btn_close_b" + ".bmp").c_str(), 2, 0);
  close->SetSize(close->BitmapWidth(), close->BitmapHeight());
  close->Move(closeX, closeY);
  close->SetId(0xc9);
  this->AddChild(close);
  this->m_message0 = 0xb8;
}

// OFFSET: 0x0094ECE0
// FUNCTION: fn_12914
void UIShowWarpWnd::Paint() {
  this->DrawBorder(0, 0, 0, 0);
}

// OFFSET: 0x00951210
// FUNCTION: fn_12949
int UIShowWarpWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case UM_DOWNPUSH:
    this->m_list->SendMsg(nullptr, UM_DOWNPUSH, 0, 0, 0, 0);
    return 0;

  case UM_UPPUSH:
    this->m_list->SendMsg(nullptr, UM_UPPUSH, 0, 0, 0, 0);
    return 0;

  case UM_SETINFO: {
    const char* name = reinterpret_cast<const char*>(argument1);
    if (std::string(name) == "Random") {
      this->m_list->AddLine(LookupMessage(MSI_RANDOM_POS));
    } else if (std::string(name) == "cancel") {
      this->m_list->AddLine(LookupMessage(MSI_CANCEL));
    } else {
      this->m_list->AddLine(g_session.GetMapDisplayName(g_session.GatName2RswName(name)));
    }
    return 0;
  }

  case UM_COMMAND:
    this->RecordResponse(static_cast<int>(argument1), argument2, argument3, argument4);
    if (argument1 == ID_CLOSE) {
      g_windowMgr.CloseWindow(WID_SHOWWARPWND);
    }
    return 0;

  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x009470C0
// FUNCTION: fn_12858
UIShowWarpWnd::UIShowWarpWnd()
    : UIFrameWnd(0) {
}
