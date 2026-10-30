#include "UI/UICheckAttendanceWnd.h"

#include "Client/Mode/ModeCursor.h"
#include "Render/Renderer.h"
#include "Render/ScreenRectangle.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"

#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x007DCAD0

// OFFSET: 0x007DCB40

// OFFSET: 0x007DCBD0
// FUNCTION: fn_09636
void UICheckAttendanceWnd_Banner::AddBanner(std::string image, std::string url, int action, int argument) {
  AttendanceBanner entry{image, url, action, argument};
  this->m_entries.push_back(entry);
}

// OFFSET: 0x007DCCE0
// FUNCTION: fn_09637
void UICheckAttendanceWnd::VisitChildren() {
  CTexture* texture = g_texMgr.Load("유저인터페이스\\check_attendance\\bt_slot_off.tga", 0, false, true, false);
  for (int day = 0; day < this->m_checkedDays; ++day) {
    if (texture == nullptr) {
      continue;
    }

    if (day == this->m_checkedDays - 1 && this->m_blinking == 0) {
      texture = g_texMgr.Load("유저인터페이스\\check_attendance\\bt_slot_complete.tga", 0, false, true, false);
    }

    DrawTextureRegion(
      g_renderer->primitives, g_renderer->queues, static_cast<float>(this->geometry.x + (day % 5) * 62 + 26),
      static_cast<float>(this->BannerHeight() + 95 + (day / 5) * 64 + this->geometry.y), static_cast<float>(texture->Width()),
      static_cast<float>(texture->Height()), 0xFFFFFFFF, texture, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
    );
  }
}

// OFFSET: 0x007DCE60
// FUNCTION: fn_09638
int UICheckAttendanceWnd::SlotAt(int x, int y) {
  const int top = this->BannerHeight() + 95;
  const int column = x - 26;
  if (static_cast<unsigned int>(column) <= 306 && top <= y && y <= top + 252 && column % 62 <= 58) {
    const int row = y - top;
    if (row % 64 <= 60) {
      return (column / 62) % 5 + (row / 64) % 4 * 5;
    }
  }
  return -1;
}

// OFFSET: 0x007DDBB0
// FUNCTION: fn_09642
void UICheckAttendanceWnd::Slot25(int x, int y) {
  if (this->SlotAt(x, y) == this->m_checkedDays) {
    this->m_highlight = 2;
    this->Invalidate();
  }
}

// OFFSET: 0x007DDBF0
// FUNCTION: fn_09643
void UICheckAttendanceWnd::Slot31(int x, int y) {
  const int slot = this->SlotAt(x, y);
  if (slot == this->m_checkedDays) {
    this->SendMsg(nullptr, 6, 184, slot, 0, 0);
  }
}

// OFFSET: 0x007DDEF0

// OFFSET: 0x007DDF00
// FUNCTION: fn_09647
void UICheckAttendanceWnd::Slot19() {
  if (this->m_blinking != 0) {
    const int highlight = timeGetTime() / 100 & 1;
    if (this->m_highlight != 2 && this->m_highlight != highlight) {
      this->m_highlight = highlight;
      this->Invalidate();
    }
  }
}

// OFFSET: 0x007DDF50
// FUNCTION: fn_09648
void UICheckAttendanceWnd_Banner::Slot19() {
  if (this->m_lastSwitch + 5000 < timeGetTime()) {
    const unsigned int next = this->m_current + 1 < this->m_entries.size() ? this->m_current + 1 : 0;
    this->SendMsg(this->m_buttons[next], 6, 215, 0, 0, 0);
  }
}

// OFFSET: 0x007DE460
// FUNCTION: fn_09652
int UICheckAttendanceWnd_Banner::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message == 6 && argument1 == 215) {
    for (unsigned int index = 0; index < this->m_buttons.size(); ++index) {
      UIWindow* button = this->m_buttons[index];
      if (sender == button) {
        button->SendMsg(this, 13, 1, 0, 0, 0);
        this->m_current = index;
      } else {
        button->SendMsg(this, 13, 0, 0, 0, 0);
      }
    }
    this->m_lastSwitch = timeGetTime();
    this->Invalidate();
  }
  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
