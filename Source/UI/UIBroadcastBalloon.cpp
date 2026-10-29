#include "UI/UIBroadcastBalloon.h"
#include "Render/Primitives/AlphaRectangle.h"
#include "Render/Renderer.h"
#include "Render/TextureQuad.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/WindowBorder.h"

#include <windows.h>

// OFFSET: 0x007C27B0
// FUNCTION: fn_09330
UIBroadcastBalloon::UIBroadcastBalloon(UIWindowContext& windows, UIButtonContext& controls) : UIWindow(windows, 0), m_controls(&controls) {
}

// OFFSET: 0x007C2D70
// FUNCTION: fn_09333
void UIBroadcastBalloon::AddMessage(int category, const BroadcastBalloonMessage& message) {
  std::list<BroadcastBalloonMessage>& messages = this->m_messages[category];
  messages.push_back(message);

  unsigned int limit = 3;
  if (category == 2) {
    limit = 1;
  }
  if (messages.size() > limit) {
    messages.pop_front();
  }

  BroadcastBalloonMessage& last = messages.back();
  int width = 0;
  int height = 0;
  for (const BroadcastBalloonLine& line : last.lines) {
    const SIZE size = this->TextSize(this->m_controls->fonts, this->m_controls->locale, line.text.c_str(), 0, 0, last.fontHeight, 0, 0);
    if (width < size.cx) {
      width = size.cx;
    }
    height += size.cy;
  }
  last.width = width + 12;
  last.height = height + (static_cast<int>(last.lines.size()) + 1) * 4;

  const std::size_t count = this->m_messages.size();
  if (count != 0) {
    int total = 0;
    for (const auto& entry : this->m_messages) {
      int sum = 0;
      for (const BroadcastBalloonMessage& item : entry.second) {
        sum += item.height;
      }
      total += sum;
      if (entry.second.size() > 1) {
        total += static_cast<int>(entry.second.size()) * 10 - 10;
      }
    }
    if (count > 1) {
      total = static_cast<int>(count) * 17 - 17 + total;
    }
    this->Resize(this->geometry.width, total);
  }
}

// OFFSET: 0x007C31C0
// FUNCTION: fn_09334
void UIBroadcastBalloon::Slot43() {
  int y = 0;
  for (const auto& entry : this->m_messages) {
    for (const BroadcastBalloonMessage& message : entry.second) {
      if (message.lines.size() != 0) {
        DrawAlphaRectangle(
          g_renderer->primitives, g_renderer->queues, this->geometry.x + 1 + (this->geometry.width - message.width) / 2, this->geometry.y + 1 + y,
          message.width - 2, message.height - 2, message.backgroundColor, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
        );
        y += message.height + 10;
      }
    }
    y += 7;
  }
}

// OFFSET: 0x007C32C0
// FUNCTION: fn_09335
bool UIBroadcastBalloon::Slot42(void*) {
  return true;
}

// OFFSET: 0x007C32D0
// FUNCTION: fn_09336
void UIBroadcastBalloon::Paint() {
  this->ClearSurface(0xFFFF00FF);
  int y = 0;
  for (const auto& entry : this->m_messages) {
    for (const BroadcastBalloonMessage& message : entry.second) {
      if (message.lines.size() != 0) {
        int x = (this->geometry.width - message.width) / 2;
        DrawRoundedBorder(*this, x, y, message.width, message.height, 0xFF5C5C5C);
        y += 4;
        for (const BroadcastBalloonLine& line : message.lines) {
          this->sub_A86510(x + 6, y, line.text.c_str(), 0, line.color, line.outlineColor, 0, message.fontHeight, 0);
          int lineHeight = this->TextHeight(this->m_controls->fonts, this->m_controls->locale, line.text.c_str(), 0, 0, message.fontHeight, 0, 0);
          y += 4 + lineHeight;
        }
        y += 10;
      }
    }
    y += 7;
  }
}

// OFFSET: 0x007C3430
// FUNCTION: fn_09337
void UIBroadcastBalloon::Slot19() {
  if (this->m_messages.size() == 0) {
    this->Show(0);
    return;
  }

  this->Show(1);
  for (auto& entry : this->m_messages) {
    std::list<BroadcastBalloonMessage>& messages = entry.second;
    for (auto it = messages.begin(); it != messages.end();) {
      if (it->start + it->duration < std::chrono::system_clock::now()) {
        it = messages.erase(it);
        this->Invalidate();
      } else {
        ++it;
      }
    }
  }
}

// OFFSET: 0x007C3580
// FUNCTION: fn_09338
bool UIBroadcastBalloon::CanHitTest() {
  return false;
}
