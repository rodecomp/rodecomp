#include "UI/Guild/UIGuild_Storage_Log.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Lists/UITabControl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// GLOBAL: gl_25103
std::vector<GuildStorageLogEntry> g_guildStorageLogs[3];

// OFFSET: 0x009469D0
// FUNCTION: fn_12849
UIGuild_Storage_Log::UIGuild_Storage_Log() : UIFrameWnd(0) {
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
  case UM_COMMAND:
    switch (a) {
    case ID_OK:
      g_windowMgr.CloseWindow(WID_GUILD_STORAGE_LOG);
      break;

    case ID_NEXT: {
      const unsigned page = this->m_page;
      if (this->m_category == 0) {
        if (page >= this->m_pageCounts[0]) {
          break;
        }
      } else if (this->m_category == 1) {
        if (page >= this->m_pageCounts[1]) {
          break;
        }
      } else if (page >= this->m_pageCounts[2]) {
        break;
      }

      this->m_state = 1;
      this->m_page = page + 1;
      this->unknown_0174 = 0;
      this->unknown_0188 = this->unknown_0184;
      this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
      return 0;
    }

    case ID_PREV: {
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
        this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
        return 0;
      }
      break;
    }
    }

    return 0;

  case UM_TAB:
    this->m_category = static_cast<int>(a);
    this->m_page = 1;
    this->m_state = 1;
    this->unknown_0174 = 0;
    this->m_offset = 0;
    this->unknown_0188 = this->unknown_0184;
    this->SendMsg(this, UM_REFRESH, 0, 0, 0, 0);
    return 0;

  case UM_REFRESH:
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

// INLINED
__forceinline void UIGuild_Storage_Log::FillLogRows(int index) {
  const std::vector<GuildStorageLogEntry>& logs = g_guildStorageLogs[index];
  char number[256];
  std::memset(number, 0, 255);
  for (auto it = logs.begin(); it != logs.end(); ++it) {
    if (static_cast<unsigned>(this->m_state) > 10 || this->m_offset >= this->unknown_0178[index]) {
      break;
    }

    std::string date;
    date = logs[this->m_offset].unknown_003C;
    std::uint32_t flag = logs[this->m_offset].unknown_0008;
    reinterpret_cast<std::vector<std::uint32_t>*>(&this->unknown_0184)->push_back(flag);
    _itoa(logs[this->m_offset].unknown_0004, number, 10);

    if (logs[this->m_offset].unknown_0070 == 1) {
      this->m_dateTexts[this->m_state - 1]->SetColor(255, 0, 0);
      this->m_nameTexts[this->m_state - 1]->SetColor(255, 0, 0);
      this->m_itemTexts[this->m_state - 1]->SetColor(255, 0, 0);
      this->m_countTexts[this->m_state - 1]->SetColor(255, 0, 0);
    } else {
      this->m_dateTexts[this->m_state - 1]->SetColor(0, 0, 0);
      this->m_nameTexts[this->m_state - 1]->SetColor(0, 0, 0);
      this->m_itemTexts[this->m_state - 1]->SetColor(0, 0, 0);
      this->m_countTexts[this->m_state - 1]->SetColor(0, 0, 0);
    }

    this->m_dateTexts[this->m_state - 1]->SetTextFitted(date.c_str());
    this->m_nameTexts[this->m_state - 1]->SetTextFitted(number);
    this->m_itemTexts[this->m_state - 1]->SetTextFitted(logs[this->m_offset].unknown_0009);
    this->m_countTexts[this->m_state - 1]->SetTextFitted(logs[this->m_offset].unknown_0021);
    this->unknown_0174 += 15;
    ++this->m_state;
    ++this->m_offset;
  }
}

// OFFSET: 0x00948FE0
// FUNCTION: fn_12883
void UIGuild_Storage_Log::FillRows() {
  const unsigned page = this->m_page;
  char pages[10] = {};
  if (this->m_category == 0) {
    std::sprintf(pages, "%02d / %02d", page, this->m_pageCounts[0]);
    this->DrawTextAt(175, 228, pages, 0, 0, 12, 0, 0, 0);
    this->FillLogRows(0);
  } else if (this->m_category == 1) {
    if (this->m_pageCounts[1] == 0) {
      std::sprintf(pages, "%02d / 01", page);
    } else {
      std::sprintf(pages, "%02d / %02d", page, this->m_pageCounts[1]);
    }
    this->DrawTextAt(175, 228, pages, 0, 0, 12, 0, 0, 0);
    this->FillLogRows(1);
  } else {
    if (this->m_pageCounts[2] == 0) {
      std::sprintf(pages, "%02d / 01", page);
    } else {
      std::sprintf(pages, "%02d / %02d", page, this->m_pageCounts[2]);
    }
    this->DrawTextAt(175, 228, pages, 0, 0, 12, 0, 0, 0);
    this->FillLogRows(2);
  }
}

// OFFSET: 0x0094D290
// FUNCTION: fn_12904
void UIGuild_Storage_Log::Paint() {
  this->DrawTitleBar(true, LookupMessage(0x9F3), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawButtonBar(2, 0, 1);
  CBitmapRes* out = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\out.bmp")));
  CBitmapRes* in = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\in.bmp")));

  for (int tab = 0; tab < static_cast<int>(this->unknown_0158->labels.size()); ++tab) {
    this->unknown_0158->colors[tab] = 0xFFFFFFFF;
    this->unknown_0158->Invalidate();
  }
  this->unknown_0158->colors[this->m_category] = 0xC89623;
  this->unknown_0158->Invalidate();

  this->DrawTextAt(234, 23, LookupMessage(0x9F4), 0, 0, 11, 0xFF, 0, 0);
  this->DrawTextAt(6, 47, LookupMessage(0x9EE), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(70, 47, LookupMessage(0x9EA), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(150, 47, LookupMessage(0x9EB), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(200, 47, LookupMessage(0x9EC), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(320, 47, LookupMessage(0x9ED), 0, 0, 12, 0, 0, 0);
  this->FillRows();

  int y = 60;
  for (const std::uint32_t* it = this->unknown_0184; it != this->unknown_0188; ++it) {
    this->DrawBitmap(6, y, *it == 1 ? in : out, true);
    y += 15;
  }
}
