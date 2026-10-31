#include "UI/Window/UIShowWarpWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "Client/ReplayPacketRecorder.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "UI/Lists/UIListBox.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <string>

// OFFSET: 0x0094CA20
// FUNCTION: fn_12899
void UIShowWarpWnd::Slot15(int width, int height) {
  auto* list = new UIListBox(*this->m_context, *this->m_listContext);
  this->m_list = list;
  width -= 0x18;
  height -= 0x32;
  if (width < 0x14) {
    width = 0x14;
  }
  if (height < 0x1e) {
    height = 0x1e;
  }
  list->Configure(0x0c, 0x14, width, height, 0);
  list->backgroundRed = 0xf0;
  list->backgroundGreen = 0xf0;
  list->backgroundBlue = 0xf0;
  list->selected = -1;
  list->Invalidate();
  list->transparent = 0;
  this->AddChild(list);

  const std::string prefix = "유저인터페이스\\";
  auto* close = new UIBitmapButton(*this->m_context, *this->m_controls);
  close->SetBitmap((prefix + "btn_close" + ".bmp").c_str(), 0, 0);
  close->SetBitmap((prefix + "btn_close_a" + ".bmp").c_str(), 1, 0);
  close->SetBitmap((prefix + "btn_close_b" + ".bmp").c_str(), 2, 0);
  close->SetSize(close->BitmapWidth(), close->BitmapHeight());
  close->Move(this->geometry.width - 0x2e, this->geometry.height - 0x18);
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
  case 6:
    this->m_recorder->Record(this->geometry.id, static_cast<int>(argument1), argument2, argument3, argument4);
    if (argument1 == 201) {
      g_windowMgr.CloseWindow(0x134);
    }
    return 0;
  case 0x12:
    this->m_list->SendMsg(nullptr, 0x12, 0, 0, 0, 0);
    return 0;
  case 0x13:
    this->m_list->SendMsg(nullptr, 0x13, 0, 0, 0, 0);
    return 0;
  case 0x22: {
    const char* name = reinterpret_cast<const char*>(argument1);
    if (std::string(name) == "Random") {
      this->m_list->AddLine(LookupMessage(0xD4));
    } else if (std::string(name) == "cancel") {
      this->m_list->AddLine(LookupMessage(0xDA));
    } else {
      this->m_list->AddLine(g_session.GetMapDisplayName(g_session.GatName2RswName(name)));
    }
    return 0;
  }
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}
