#include "Client/Session.h"
#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeTooltip.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Window/WindowOrdering.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstring>
#include <string>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00AD6510
// FUNCTION: fn_17834
void ModeTooltip::Show(const char* value, int x, int y, std::uint32_t color, unsigned char option, unsigned char convert) {
  if (std::strlen(value) == 0) {
    return;
  }

  char text[1024];

  if (convert != 0) {
    g_session.ConvertTooltipText(value, text);
  } else {
    strncpy_s(text, sizeof(text), value, _TRUNCATE);
  }

  this->tick = timeGetTime();

  if (this->window) {
    this->ordering.Promote(this->window);
  } else {
    this->window = new UITransBalloonText(this->windows);
    this->window->SetSize(8, 8);
    this->rendered.push_back(this->window);
  }

  this->window->SetRichText(option);
  this->window->SetText(text, 35);
  const std::uint32_t rgb = ((color & 255) << 16) | (color & 0xFF00) | ((color >> 16) & 255);
  this->window->SetColors(rgb, 0);
  this->window->SetBackground(0x88000000u);

  const int maximumX = this->screenWidth - this->window->geometry.width + 3;
  const int maximumY = this->screenHeight - this->window->geometry.height + 3;
  const int left = x < -3 ? -3 : x;
  const int top = y < -3 ? -3 : y;

  this->window->Move(left < maximumX ? left : maximumX, top < maximumY ? top : maximumY);
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
