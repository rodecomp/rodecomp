#include "Client/Replay.h"
#include "UI/Window/WindowPolicy.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/UINewSelectCharWnd.h"
#include "Contents/MessageTable.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeEvents.h"
#include "UI/Buttons/UITextureButton.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x0085E070
// FUNCTION: fn_10822
void UITextureButton::Paint() {
}

// OFFSET: 0x0085F060
// OFFSET: 0x03DE9CE2
// FUNCTION: fn_10856
void UITextureButton::Slot31(int x, int y) {
  if (this->m_disabled == 1) {
    return;
  }

  if (g_windowMgr.Capture() != this) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.unknown_0030 = 0;
  } else {
    if (this->unknown_011C == 1) {
      this->unknown_011D = this->unknown_011D == 0;
    }

    this->geometry.unknown_0030 = 1;
    UIWindow* parent = this->parent;
    if (parent) {
      if (WindowPolicy(g_windowMgr.policy[0].actions, g_replayAnimationScaling != 0, 3, parent->geometry.id) > 0) {
        int id = this->geometry.id;
        if (id == 185 && this->m_context->mode && static_cast<CMode*>(g_charViewMode)->cursorType == 2) {
          const char* text = LookupMessage(17);
          if (reinterpret_cast<WindowDialogs*>(&g_windowMgr)->Confirm(text, 2, true, false, 0, "Message", 280, 120, 0) == 187) {
            this->m_context->mode->SendModeMessage(2, 0, 0, 0, 0);
          }
        } else {
          if (id == 184 || id == 321) {
            RecordEditTextsNoArg(parent);
            id = this->geometry.id;
          }

          this->parent->SendMsg(this, 6, id, this->m_argument, 0, 0);
        }
      }
    } else {
      this->m_context->mode->SendModeMessage(0, this->geometry.id, this->m_argument, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}
