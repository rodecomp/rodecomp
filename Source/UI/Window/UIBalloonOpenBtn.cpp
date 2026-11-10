#include "UI/Window/UIBalloonOpenBtn.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"

#include "Client/Session.h"
#include "Contents/Events/SpecialPopupMgr.h"
#include "Render/Renderer.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>
#include <timeapi.h>

// GLOBAL: gl_24910
std::int8_t g_balloonFontMode;

// OFFSET: 0x0094D250
// FUNCTION: fn_12902
void UIBalloonOpenBtn::Paint() {
  this->ClearSurface(0xFFFFFFFF);
}

// OFFSET: 0x0094F0E0
// FUNCTION: fn_12931
void UIBalloonOpenBtn::OnProcess() {
  if (this->m_balloon) {
    const std::int64_t elapsed = static_cast<std::int64_t>(timeGetTime()) - this->m_openTime;
    if (elapsed > this->m_openDelay) {
      g_windowMgr.CloseWindow(WID_BALLOONOPENBTN);
    }
  }
}

// OFFSET: 0x009468D0
// FUNCTION: fn_12847
UIBalloonOpenBtn::UIBalloonOpenBtn()
    : UIFrameWnd(0), m_openTime(0), m_balloonId(0), m_balloon(nullptr), m_button(nullptr) {
  this->m_openDelay = 4000;
}

// OFFSET: 0x00950200
// FUNCTION: fn_12939
int UIBalloonOpenBtn::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == UM_COMMAND && argument1 == 510) {
    if (argument2 > 0) {
      this->m_balloonId = static_cast<int>(argument2);
    }

    if (this->m_balloon == nullptr) {
      this->m_balloon = new UITransBalloonText();
      this->m_balloon->SetSize(8, 8);
      g_windowMgr.AddRenderedFront(this->m_balloon);
    }

    if (this->m_balloon != nullptr) {
      PopupData* popup = g_specialPopupMgr->GetPopup(this->m_balloonId);
      if (popup == nullptr) {
        g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("balloon No GetPopupData : %d ", this->m_balloonId).c_str()), 0xFFFF0000, 0, 0, 0);
        return 0;
      }

      this->m_balloon->m_fontMode = static_cast<std::int8_t>(g_session.chatFont[0]);
      this->m_balloon->m_richText = 1;
      this->m_balloon->SetLine("", popup->m_color);
      for (auto it = popup->m_descriptions.begin(); it != popup->m_descriptions.end(); ++it) {
        this->m_balloon->AddLine(it->c_str(), popup->m_color);
      }

      this->m_balloon->Move(g_renderer->viewport.centerX + 40, g_renderer->viewport.centerY);
      this->Show(0);
      this->m_openTime = timeGetTime();
    }
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
