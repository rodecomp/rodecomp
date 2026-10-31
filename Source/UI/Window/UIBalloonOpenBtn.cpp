#include "UI/Window/UIBalloonOpenBtn.h"

#include "Contents/SpecialPopupMgr.h"
#include "Render/Renderer.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <mmsystem.h>
#include <windows.h>

// GLOBAL: gl_24910
std::int8_t g_balloonFontMode;

// OFFSET: 0x0094D250
// FUNCTION: fn_12902
void UIBalloonOpenBtn::Paint() {
  this->ClearSurface(0xFFFFFFFF);
}

// OFFSET: 0x0094F0E0
// FUNCTION: fn_12931
void UIBalloonOpenBtn::Slot19() {
  if (this->m_balloon) {
    const std::int64_t elapsed = static_cast<std::int64_t>(timeGetTime()) - this->m_openTime;
    if (elapsed > this->m_openDelay) {
      reinterpret_cast<void(__thiscall*)(UIWindowMgr*, int)>(0x00A8CF20)(&g_windowMgr, 0x166);
    }
  }
}

// OFFSET: 0x009468D0
// FUNCTION: fn_12847
UIBalloonOpenBtn::UIBalloonOpenBtn(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_openTime(0), m_openDelay(4000), m_balloonId(0), m_balloon(nullptr), m_button(nullptr), m_controls(&controls) {
}

// OFFSET: 0x00950200
// FUNCTION: fn_12939
int UIBalloonOpenBtn::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6 && argument1 == 510) {
    if (argument2 > 0) {
      this->m_balloonId = static_cast<int>(argument2);
    }

    if (this->m_balloon == nullptr) {
      this->m_balloon = new UITransBalloonText(*this->m_context, *reinterpret_cast<BalloonTextContext*>(nullptr));
      this->m_balloon->SetSize(8, 8);
      g_windowMgr.AddRenderedFront(this->m_balloon);
      if (this->m_balloon == nullptr) {
        return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
      }
    }

    PopupData* popup = g_specialPopupMgr->GetPopup(this->m_balloonId);
    if (popup == nullptr) {
      const std::string text = FormatOwnedString("balloon No GetPopupData : %d ", this->m_balloonId);
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(text.c_str()), 0xFFFF0000, 0, 0, 0);
      return 0;
    }

    this->m_balloon->m_fontMode = g_balloonFontMode;
    this->m_balloon->m_richText = 1;
    this->m_balloon->SetLine("", popup->m_color);
    for (const std::string& description : popup->m_descriptions) {
      this->m_balloon->AddLine(description.c_str(), popup->m_color);
    }

    this->m_balloon->Move(g_renderer->viewport.centerX + 40, g_renderer->viewport.centerY);
    this->Show(0);
    this->m_openTime = timeGetTime();
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
