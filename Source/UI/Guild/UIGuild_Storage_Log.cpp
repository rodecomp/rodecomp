#include "UI/Guild/UIGuild_Storage_Log.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>

// GLOBAL: gl_25103
std::vector<GuildStorageLogEntry> g_guildStorageLogs[3];

// OFFSET: 0x009469D0
// FUNCTION: fn_12849
UIGuild_Storage_Log::UIGuild_Storage_Log(UIWindowContext& windows, UIFrameContext& frames) : UIFrameWnd(windows, frames, 0) {
  this->unknown_0184 = 0;
  this->unknown_0188 = 0;
  this->unknown_018C = 0;
  this->m_category = 0;
  this->m_offset = 0;
  this->m_state = 1;
  this->unknown_0174 = 0;
  const int first = static_cast<int>(g_guildStorageLogs[0].size());
  this->unknown_0178[0] = first;
  const int second = static_cast<int>(g_guildStorageLogs[1].size());
  this->unknown_0178[1] = second;
  const int third = static_cast<int>(g_guildStorageLogs[2].size());
  this->m_page = 1;
  this->unknown_0178[2] = third;
  this->m_pageCounts[0] = (first + 9) / 10;
  this->m_pageCounts[1] = (second + 9) / 10;
  this->m_pageCounts[2] = (third + 9) / 10;
}

// OFFSET: 0x00950450
// FUNCTION: fn_12941
int UIGuild_Storage_Log::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  switch (message) {
  case 6:
    switch (a) {
    case 184:
      this->m_frameContext->windows.CloseWindow(253);
      break;

    case 218: {
      const unsigned page = this->m_page;
      bool hasNext;

      if (this->m_category == 0) {
        hasNext = page < this->m_pageCounts[0];
      } else if (this->m_category == 1) {
        hasNext = page < this->m_pageCounts[1];
      } else {
        hasNext = page < this->m_pageCounts[2];
      }

      if (hasNext) {
        this->m_state = 1;
        this->m_page = page + 1;
        this->unknown_0174 = 0;
        this->unknown_0188 = this->unknown_0184;
        this->SendMsg(this, 60, 0, 0, 0, 0);
        return 0;
      }
      break;
    }

    case 246: {
      const unsigned page = this->m_page;
      if (page > 1) {
        const int state = this->m_state;
        this->m_state = 1;
        this->unknown_0174 = 0;

        unsigned offset = this->m_offset;
        if (state == 11) {
          offset -= 20;
        } else {
          offset = offset - offset % 10 - 10;
        }

        this->m_offset = std::max(0u, offset);
        this->m_page = std::max(1u, page - 1);
        this->unknown_0188 = this->unknown_0184;
        this->SendMsg(this, 60, 0, 0, 0, 0);
        return 0;
      }
      break;
    }
    }

    return 0;

  case 22:
    this->m_category = static_cast<int>(a);
    this->m_page = 1;
    this->m_state = 1;
    this->unknown_0174 = 0;
    this->m_offset = 0;
    this->unknown_0188 = this->unknown_0184;
    this->SendMsg(this, 60, 0, 0, 0, 0);
    return 0;

  case 60:
    for (int i = 0; i < 10; ++i) {
      this->m_dateTexts[i]->SetText("", 0);
      this->m_nameTexts[i]->SetText("", 0);
      this->m_itemTexts[i]->SetText("", 0);
      this->m_countTexts[i]->SetText("", 0);
    }

    this->Invalidate();
    return 0;

  default:
    return this->UIWindow::SendMsg(sender, message, a, b, c, d);
  }
}
