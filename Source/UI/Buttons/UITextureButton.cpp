#include "UI/Buttons/ButtonState.h"
#include "Client/Mode/ModeMessage.h"
#include "UI/Window/UIEnums.h"
#include "Client/Replay/Replay.h"
#include "UI/Window/WindowPolicy.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Contents/Tables/MessageTable.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeEvents.h"
#include "UI/Buttons/UITextureButton.h"
#include "UI/Window/UIWindowMgr.h"

#include "Resource/MSGSTRINGID.h"

// OFFSET: 0x008506F0
// FUNCTION: fn_45632
UITextureButton::UITextureButton()
    : UIButton() {
  this->textureWidth = 0;
  this->textureHeight = 0;
  this->toggleEnabled = 0;
  this->toggled = 0;
}

// OFFSET: 0x0085E070
// FUNCTION: fn_10822
void UITextureButton::Paint() {
}

// OFFSET: 0x0085F060
// FUNCTION: fn_10856
void UITextureButton::OnLBtnUp(int x, int y) {
  if (this->m_disabled == 1) {
    return;
  }

  if (this != g_windowMgr.Capture()) {
    return;
  }

  if (x < 0 || x >= this->geometry.width || y < 0 || y >= this->geometry.height) {
    this->geometry.state = BUTTON_STATE_NORMAL;
  } else {
    if (this->toggleEnabled == 1) {
      this->toggled = this->toggled == 0;
    }

    this->geometry.state = BUTTON_STATE_HOVER;
    if (this->parent) {
      if (g_windowMgr.WindowPolicy(3, this->parent->geometry.id) > 0) {
        int id = this->geometry.id;
        if (id == ID_OK || id == ID_SEARCH) {
          this->parent->RecordEditTexts();
          id = this->geometry.id;
        }

        this->parent->SendMsg(this, UM_COMMAND, id, this->m_argument, 0, 0);
      }
    } else {
      g_modeMgr.currentMode->SendMsg(MM_COMMAND, this->geometry.id, this->m_argument, 0, 0);
    }
  }

  this->Invalidate();
  g_windowMgr.ReleaseCapture();
}
