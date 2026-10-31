#include "UI/Window/UISeekPartyListWnd.h"
#include "Client/Session.h"
#include "Contents/MessageTable.h"
#include "Contents/Party/PartyBookingMgr.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UISeekPartyWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Text/FormatString.h"

#include <cstdlib>
#include <cstring>

namespace {
#pragma pack(push, 1)
struct PACKET_CZ_PARTY_BOOKING_REQ_SEARCH {
  /* 0000 */ short packetType;
  /* 0002 */ short level;
  /* 0004 */ short mapId;
  /* 0006 */ short job;
  /* 0008 */ int lastIndex;
  /* 000C */ short pageSize;
};
#pragma pack(pop)
}

// GLOBAL: gl_24771
std::map<int, PartyBookingEntry> g_friendRows;

// OFFSET: 0x00925D70

// OFFSET: 0x009393E0
// FUNCTION: fn_12755
int UISeekPartyListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  WindowDialogs& dialogs = *reinterpret_cast<WindowDialogs*>(&g_windowMgr);

  switch (message) {
  case 6:
    switch (argument1) {
    case 201:
      g_session.m_partyBooking.m_alarm = this->m_alarmCheck->geometry.unknown_0030 != 0;
      g_windowMgr.CloseWindow(0xA9);
      return 0;
    case 202:
      if (this->m_minimized) {
        this->Resize(this->geometry.width, this->m_restoreHeight);
      } else {
        this->m_restoreHeight = this->geometry.height;
        this->Resize(this->geometry.width, 17);
      }
      this->m_minimized = !this->m_minimized;
      this->Invalidate();
      return 0;
    case 284:
      this->Invalidate();
      return 0;
    case 343:
      g_windowMgr.EnsureWindow(0xAD);
      return 0;
    case 365:
    case 380: {
      std::string text(this->m_searchEdit->Text());
      const short level = static_cast<short>(std::atoi(text.c_str()));
      for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] < '/' || text[i] > '9') {
          dialogs.Confirm(LookupMessage(1770), 0, true, false, 0, "Message", 280, 120, 0);
          return 0;
        }
      }

      if (level < 0 || level > g_partyBookingLevelMax) {
        char buffer[260];
        std::memset(buffer, 0, sizeof(buffer));
        StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), LookupMessage(2601), g_partyBookingLevelMax);
        dialogs.Confirm(buffer, 0, true, false, 0, "Message", 280, 120, 0);
        return 0;
      }

      short mapId = 0;
      if (this->m_region != 0) {
        mapId = static_cast<short>((this->m_region << 8) | this->m_mapIndex);
      }

      const int jobOffsets[9] = {0, 1, 5, 11, 24, 30, 37, 43, 56};
      short job = -1;
      if (this->m_jobCategory != 0) {
        job = static_cast<short>(this->m_jobCategoryChanged + jobOffsets[this->m_jobCategory] - 1);
      }

      int lastIndex = 0;
      if (argument1 == 365) {
        if (this->m_filterJob == job && this->m_filterLevel == level && this->m_filterMap == mapId) {
          return 0;
        }
        g_session.m_partyBooking.Clear();
      } else {
        if (!g_session.m_partyBooking.m_lastPage) {
          g_session.m_partyBooking.FlushPending();
          return 0;
        }
        lastIndex = g_session.m_partyBooking.m_lastIndex;
      }

      PACKET_CZ_PARTY_BOOKING_REQ_SEARCH packet;
      packet.packetType = ID_CZ_PARTY_BOOKING_REQ_SEARCH;
      packet.level = level;
      packet.mapId = mapId;
      packet.job = job;
      packet.lastIndex = lastIndex;
      packet.pageSize = 10;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.packetType), &packet);

      this->m_filterJob = job;
      this->m_filterLevel = level;
      this->m_filterMap = mapId;

      std::string title(LookupMessage(0x6E5));
      if (level > 0) {
        title += " Level: ";
        title += IntToString(level, 0);
      }

      if (this->m_region != 0) {
        title += " Map: ";
        title += this->m_regionCombo->GetSelectedText();
        title += " ";
        title += this->m_mapCombo->GetSelectedText();
      }

      if (this->m_jobCategory != 0) {
        title += " Job: ";
        title += this->m_jobCombo->GetSelectedText();
      }

      this->m_title->SetTextFitted(title.c_str());

      g_session.m_partyBooking.SetFilter(this->m_filterLevel, this->m_filterMap, this->m_filterJob);
      this->Invalidate();
      return 0;
    }
    case 381: {
      const int hoverIndex = this->m_hoverIndex;
      if (hoverIndex == -1) {
        return 0;
      }

      const int firstIndex = this->m_firstRow * this->m_columnCount;
      int index = 0;
      for (auto it = g_friendRows.begin(); it != g_friendRows.end(); ++it, ++index) {
        if (index < firstIndex || index != hoverIndex) {
          continue;
        }

        const PartyBookingEntry entry = it->second;
        if (entry.isEnd) {
          this->SendMsg(nullptr, 6, 0x17C, 0, 0, 0);
          g_session.m_partyBooking.EraseEntry(it->first);
        } else {
          const std::string leaderName = entry.leaderName;
          if (std::strcmp(leaderName.c_str(), g_session.DecodePlayerName()) != 0) {
            g_windowMgr.SendNotice(14, reinterpret_cast<std::intptr_t>(leaderName.c_str()), 0, 0, 0, 0);
          }
        }
        return 0;
      }
      return 0;
    }
    default:
      return 0;
    }
  case 7:
  case 9:
  case 10: {
    const int previous = this->m_firstRow;
    int next;
    if (message == 9) {
      next = previous - this->m_visibleRows + 1;
    } else if (message == 10) {
      next = previous + this->m_visibleRows - 1;
    } else {
      next = static_cast<int>(argument1) + previous;
    }

    this->m_firstRow = next;
    if (previous != next) {
      this->UpdateScrollBar();
      this->Invalidate();
      next = this->m_firstRow;
    }
    return next - previous;
  }
  case 34:
    this->m_saved = reinterpret_cast<SeekPartyListWndInfo*>(argument1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }

    this->Move(this->m_saved->x, this->m_saved->y);
    this->SendMsg(nullptr, 14, 3, this->m_saved->width, this->m_saved->height, 0);
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    return 0;
  case 39:
    if (sender == this->m_regionCombo) {
      this->SelectRegion(static_cast<int>(argument1) + 1);
      this->m_region = static_cast<int>(argument1);
    } else if (sender == this->m_mapCombo) {
      this->m_mapIndex = static_cast<int>(argument1);
    } else if (sender == this->m_jobCategoryCombo) {
      this->SelectJobCategory(static_cast<int>(argument1));
    } else if (sender == this->m_jobCombo) {
      this->m_jobCategoryChanged = static_cast<int>(argument1) + 1;
    }
    return 0;
  case 98:
    if (argument1 != 378 || g_windowMgr.CloseWindow(0xA8)) {
      return 0;
    }
    g_windowMgr.EnsureWindow(0xA8);
    return 0;
  default:
    return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
  }
}

// OFFSET: 0x009431D0
// FUNCTION: fn_12815
void UISeekPartyListWnd::NotifyNewAd() {
  if (this->m_alarmCheck->geometry.unknown_0030 != 0) {
    g_soundMgr->PlayWave("party_alarm.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(LookupMessage(0x6D9)), 0xFFFF00, 15, 0, 0);
  }
}

// OFFSET: 0x009435F0
// FUNCTION: fn_12825
bool UISeekPartyListWnd::MatchesFilter(PARTY_BOOKING_AD_INFO info) {
  if (this->m_filterLevel > 0 && (this->m_filterLevel > info.detail.level || this->m_filterLevel + 15 < info.detail.level)) {
    return false;
  }

  if (this->m_filterMap > 0 && this->m_filterMap != info.detail.mapId) {
    return false;
  }

  if (this->m_filterJob > 0) {
    for (int i = 0; i < 6; ++i) {
      if (this->m_filterJob == info.detail.job[i]) {
        return true;
      }
    }
    return false;
  }

  return true;
}

// OFFSET: 0x00933630
// FUNCTION: fn_12727
void UISeekPartyListWnd::Slot35(int delta) {
  if (this->m_scrollable != 0) {
    int step = this->m_visibleRows - 1;
    if (step > 3) {
      step = 3;
    }
    if (delta >= 0) {
      step = -step;
    }
    this->SendMsg(this->m_scrollBar, 7, step, 0, 0, 0);
  }
}

// OFFSET: 0x0091BB10
// FUNCTION: fn_12607
int UISeekPartyListWnd::HitTestRow(int x, int y) {
  if (x > this->geometry.width - 0x14 || x < 0x14) {
    return -1;
  }

  if (y < 0x78 || y > this->geometry.height - 0x1e) {
    return -1;
  }

  const int row = (y - 0x78) / 0x28;
  int found = 0;
  int index = 0;
  const int firstIndex = this->m_firstRow * this->m_columnCount;
  for (auto it = g_friendRows.begin(); it != g_friendRows.end(); ++it, ++index) {
    if (index < firstIndex) {
      continue;
    }

    if (found == this->m_columnCount * row) {
      return index;
    }

    ++found;
  }

  return -1;
}

// OFFSET: 0x00930720
// FUNCTION: fn_12687
void UISeekPartyListWnd::Slot26(int x, int y) {
  if (y < 0x78) {
    return;
  }

  const int previous = this->m_hoverIndex;
  const int index = this->HitTestRow(x, y);
  this->m_hoverIndex = index;
  if (previous != index) {
    this->Invalidate();
  }

  this->SendMsg(nullptr, 6, 0x17d, 0, 0, 0);
}

// OFFSET: 0x00931090
// FUNCTION: fn_12698
void UISeekPartyListWnd::Slot25(int x, int y) {
  if (y < 0x78) {
    UIFrameWnd::Slot25(x, y);
    return;
  }

  const int previous = this->m_hoverIndex;
  const int index = this->HitTestRow(x, y);
  this->m_hoverIndex = index;
  if (previous != index) {
    this->Invalidate();
  }
}

// OFFSET: 0x00933740
// FUNCTION: fn_12729
void UISeekPartyListWnd::UpdateScrollBar() {
  if (!this->m_scrollBar) {
    return;
  }

  const int count = g_friendRows.size();
  const int rows = count / this->m_columnCount + (count % this->m_columnCount != 0 ? 1 : 0);

  const int visible = this->m_visibleRows;
  int range = rows - visible;
  if (range < 0) {
    range = 0;
  }

  this->m_scrollable = range != 0;
  if (range != 0) {
    this->m_scrollBar->SetRange(range, visible, rows);
    if (this->m_firstRow < 0) {
      this->m_firstRow = 0;
    }
    if (this->m_firstRow > rows - this->m_visibleRows) {
      this->m_firstRow = rows - this->m_visibleRows;
    }
    this->m_scrollBar->SetPosition(this->m_firstRow);
  } else {
    this->m_firstRow = 0;
    this->m_scrollBar->SetRange(0, visible, rows);
  }

  this->m_scrollBar->Invalidate();
}

// OFFSET: 0x00943F40
// FUNCTION: fn_12830
void UISeekPartyListWnd::InitRegionFilter() {
  g_luaScript->HasTable("map_list");
  std::string regionName;

  int region = 1;
  for (;; ++region) {
    int id = 0;
    const char* name = nullptr;
    if (g_luaScript->Call("queryRegionInfo", "d>ds", region, &id, &name)) {
      if (!name) {
        break;
      }

      this->m_regionCombo->AddOption(name, 0);
    }
  }

  g_luaScript->HasTable("map_list");
  for (int current = 1; current < region; ++current) {
    std::vector<SeekPartyMapEntry> maps;
    int x = 0;
    int y = 0;
    const char* name = nullptr;
    int red = 0;
    int green = 0;
    int blue = 0;
    for (int index = 1;; ++index) {
      if (g_luaScript->Call("queryMapInfo", "dd>ddsddd", current, index, &x, &y, &name, &red, &green, &blue)) {
        if (!name) {
          break;
        }

        std::string mapName(name);
        const unsigned int color =
          (static_cast<unsigned char>(blue) << 16) | (static_cast<unsigned char>(green) << 8) | static_cast<unsigned char>(red);
        maps.push_back(SeekPartyMapEntry{mapName, color});
      }
    }

    this->m_regionMaps.insert(std::make_pair(current, maps));
  }

  this->m_regionCombo->Select(0);
  this->SelectRegion(1);
}

// OFFSET: 0x00943830
// FUNCTION: fn_12829
void UISeekPartyListWnd::InitJobFilter() {
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E6), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DB), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DC), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DD), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DE), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DF), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E0), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E1), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E2), 0);

  int counts[9] = {0, 0, 6, 13, 6, 7, 6, 13, 8};
  int message = 0;
  this->m_jobOptions[0].push_back(std::string(LookupMessage(0x6E6)));

  for (int i = 0; i < 4; ++i) {
    std::string name(LookupMessage(0x6B0 + i));
    this->m_jobOptions[1].push_back(name);
  }

  for (int category = 2; category < 9; ++category) {
    for (int i = 0; i < counts[category]; ++i) {
      std::string name(LookupMessage(0x65D + message));
      this->m_jobOptions[category].push_back(name);
      ++message;
    }
  }

  this->m_jobCategoryCombo->Select(0);
  this->SelectJobCategory(0);
}

// OFFSET: 0x00945CF0
// FUNCTION: fn_12836
void UISeekPartyListWnd::SelectRegion(int region) {
  this->m_mapCombo->Clear();

  auto it = this->m_regionMaps.find(region);
  if (it != this->m_regionMaps.end()) {
    std::vector<SeekPartyMapEntry> maps = it->second;
    for (SeekPartyMapEntry entry : maps) {
      this->m_mapCombo->AddOption(entry.name.c_str(), entry.color);
    }

    this->m_mapCombo->Select(0);
    this->m_mapIndex = 0;
    this->Invalidate();
  }
}

// OFFSET: 0x00945FB0
// FUNCTION: fn_12837
void UISeekPartyListWnd::SelectJobCategory(int category) {
  if (category == this->m_jobCategory && this->m_jobCategory != 0) {
    return;
  }

  this->m_jobCombo->Clear();
  for (const std::string& option : this->m_jobOptions[category]) {
    this->m_jobCombo->AddOption(option, 0);
  }

  this->m_jobCombo->Select(0);
  this->m_jobCategory = category;
  this->m_jobCategoryChanged = 1;
  this->Invalidate();
}
