#include "Client/Session/Session.h"
#include "Client/Mode/CMode.h"
#include "Render/Renderer.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Window/UIWindowMgr.h"

#include <algorithm>
#include <cstring>
#include <string>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00AD6510
// FUNCTION: fn_17834
void CMode::ShowTooltip(const char* value, int x, int y, std::uint32_t color, unsigned char option, unsigned char convert) {
  const std::uint32_t red = color & 255;
  const std::uint32_t blue = (color >> 16) & 255;
  if (std::strlen(value) == 0) {
    return;
  }

  char text[1024];
  if (convert != 0) {
    g_session.ConvertTooltipText(value, text);
  } else {
    strncpy_s(text, sizeof(text), value, _TRUNCATE);
  }

  this->tooltipTick = timeGetTime();

  if (!this->tooltip) {
    this->tooltip = new UITransBalloonText();
    if (!this->tooltip) {
      return;
    }
    this->tooltip->SetSize(8, 8);
    g_windowMgr.AddRendered(this->tooltip);
  } else {
    g_windowMgr.Promote(this->tooltip);
  }

  this->tooltip->SetRichText(option);
  this->tooltip->SetText(text, 35);
  this->tooltip->SetColors((red << 16) | (color & 0xFF00) | blue, 0);
  this->tooltip->SetBackground(0x88000000u);

  const int maximumX = g_renderer->viewport.width - this->tooltip->geometry.width + 3;
  const int maximumY = g_renderer->viewport.height - this->tooltip->geometry.height + 3;
  const int left = (std::max)(x, -3);
  const int top = (std::max)(y, -3);

  this->tooltip->Move((std::min)(left, maximumX), (std::min)(top, maximumY));
}

// OFFSET: 0x00AD67A0
// FUNCTION: fn_17843
void CMode::ExpireTooltip() {
  if (!this->tooltip) {
    return;
  }

  if (timeGetTime() < this->tooltipTick + 100u) {
    return;
  }

  g_windowMgr.Schedule(this->tooltip);
  this->tooltip = nullptr;
}

// OFFSET: 0x00AD64B0
// FUNCTION: fn_17833
std::uint32_t CMode::ClearTooltip() {
  if (!this->tooltip) {
    return 0;
  }

  g_windowMgr.Schedule(this->tooltip);
  this->tooltip = nullptr;
  this->tooltipTick = timeGetTime();
  return this->tooltipTick;
}
