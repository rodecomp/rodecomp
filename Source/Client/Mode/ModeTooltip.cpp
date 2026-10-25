#include "Client/Mode/ModeTooltip.h"
#include "UI/Text/UITransBalloonText.h"
#include "UI/Window/WindowOrdering.h"
#include "UI/Window/WindowRetirement.h"

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

  char copied[1024];
  std::string converted;
  const char* text;

  if (convert == 0) {
    strncpy_s(copied, sizeof(copied), value, _TRUNCATE);
    text = copied;
  } else {
    converted = value;

    for (char* current = converted.data(); *current != '\0'; current = CharNextExA(this->text.codePage, current, 0)) {
      if (*current == '_') {
        *current = ' ';
      }
    }

    text = converted.c_str();
  }

  this->tick = timeGetTime();

  if (this->window) {
    this->ordering.Promote(this->window);
  } else {
    this->window = new UITransBalloonText(this->windows, this->text);
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
void ModeTooltip::Expire(WindowRetirement& retirement) {
  if (!this->window) {
    return;
  }

  if (timeGetTime() < this->tick + 100u) {
    return;
  }

  retirement.Schedule(this->window);
  this->window = nullptr;
}
