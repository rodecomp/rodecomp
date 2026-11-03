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
// FUNCTION: fn_12649
void UISeekPartyListWnd::Slot15(int width, int height) {
  std::string label;

  {
    std::string title(LookupMessage(0x699));
    this->m_title = new UIStaticText();
    this->m_title->SetSize(this->geometry.width - 0x20, 0xF);
    this->m_title->SetTextFitted(title.c_str());
    this->m_title->Move(0x10, 2);
    this->m_title->shadow = 1;
    this->AddChild(this->m_title);
  }

  this->AddCloseButton("", 0);
  this->AddMinimizeButton("", 0);

  label = "LEVEL";
  this->m_levelLabel = new UIStaticText();
  this->m_levelLabel->SetSize(this->geometry.width - 10, 0x1E);
  this->m_levelLabel->SetText(label.c_str(), 1);
  this->m_levelLabel->Move(10, 0x17);
  this->m_levelLabel->shadow = 1;
  this->AddChild(this->m_levelLabel);

  this->m_searchEdit = new UIEditCtrl();
  this->m_searchEdit->SetSize(0x28, 0x10);
  this->m_searchEdit->Move(0x32, 0x14);
  this->m_searchEdit->maximum = 5;
  this->m_searchEdit->Background(0xE8, 0xE8, 0xE8);
  this->m_searchEdit->Border(0x64, 0x64, 0x64);
  this->m_searchEdit->flat = 0;
  this->AddChild(this->m_searchEdit);

  label = "MAP";
  this->m_mapLabel = new UIStaticText();
  this->m_mapLabel->SetSize(this->geometry.width - 10, 0x1E);
  this->m_mapLabel->SetText(label.c_str(), 1);
  this->m_mapLabel->Move(10, 0x2D);
  this->m_mapLabel->shadow = 1;
  this->AddChild(this->m_mapLabel);

  label = "JOB";
  this->m_jobLabel = new UIStaticText();
  this->m_jobLabel->SetSize(this->geometry.width - 10, 0x1E);
  this->m_jobLabel->SetText(label.c_str(), 1);
  this->m_jobLabel->Move(10, 0x46);
  this->m_jobLabel->shadow = 1;
  this->AddChild(this->m_jobLabel);

  label = LookupMessage(0x72A);
  this->m_footerText = new UIStaticText();
  this->m_footerText->SetSize(this->geometry.width - 10, 0x1E);
  this->m_footerText->SetText(label.c_str(), 0);
  this->m_footerText->Move(100, this->geometry.height - 0x14);
  this->AddChild(this->m_footerText);

  this->m_filterLabel0 = new UIStaticText();
  this->m_filterLabel0->SetSize(0x32, 0x14);
  this->m_filterLabel0->SetText(LookupMessage(0x6A1), 0);
  this->m_filterLabel0->Move(0x21, 0x68);
  this->AddChild(this->m_filterLabel0);

  this->m_filterLabel1 = new UIStaticText();
  this->m_filterLabel1->SetSize(0x32, 0x14);
  this->m_filterLabel1->SetText(LookupMessage(0x197), 0);
  this->m_filterLabel1->Move(0x6A, 0x68);
  this->AddChild(this->m_filterLabel1);

  this->m_filterLabel2 = new UIStaticText();
  this->m_filterLabel2->SetSize(0x32, 0x14);
  this->m_filterLabel2->SetText(LookupMessage(0x198), 0);
  this->m_filterLabel2->Move(0xDC, 0x68);
  this->AddChild(this->m_filterLabel2);

  this->m_filterLabel3 = new UIStaticText();
  this->m_filterLabel3->SetSize(0x32, 0x14);
  this->m_filterLabel3->SetText(LookupMessage(0x6A2), 0);
  this->m_filterLabel3->Move(0x16D, 0x68);
  this->AddChild(this->m_filterLabel3);

  this->m_scrollBar = new UIScrollBar();
  this->m_scrollBar->Configure(0x203, 0x78, 1, 200);
  this->AddChild(this->m_scrollBar);

  this->m_jobCategoryCombo = new UIComboBoxDefWnd();
  this->m_jobCategoryCombo->dropdownHeight = 0x50;
  this->m_jobCategoryCombo->SetSize(0x96, 0x12);
  this->m_jobCategoryCombo->Move(0x32, 0x44);
  this->m_jobCategoryCombo->borderRed = 0x64;
  this->m_jobCategoryCombo->borderGreen = 0x64;
  this->m_jobCategoryCombo->borderBlue = 0x64;
  this->m_jobCategoryCombo->red = 0xE8;
  this->m_jobCategoryCombo->green = 0xE8;
  this->m_jobCategoryCombo->blue = 0xE8;
  this->AddChild(this->m_jobCategoryCombo);

  this->m_jobCombo = new UIComboBoxDefWnd();
  this->m_jobCombo->dropdownHeight = 0x50;
  this->m_jobCombo->SetSize(200, 0x12);
  this->m_jobCombo->Move(0xDC, 0x44);
  this->m_jobCombo->borderRed = 0x64;
  this->m_jobCombo->borderGreen = 0x64;
  this->m_jobCombo->borderBlue = 0x64;
  this->m_jobCombo->red = 0xE8;
  this->m_jobCombo->green = 0xE8;
  this->m_jobCombo->blue = 0xE8;
  this->AddChild(this->m_jobCombo);

  this->m_regionCombo = new UIComboBoxDefWnd();
  this->m_regionCombo->dropdownHeight = 0x50;
  this->m_regionCombo->SetSize(0x96, 0x12);
  this->m_regionCombo->Move(0x32, 0x2B);
  this->m_regionCombo->borderRed = 0x64;
  this->m_regionCombo->borderGreen = 0x64;
  this->m_regionCombo->borderBlue = 0x64;
  this->m_regionCombo->red = 0xE8;
  this->m_regionCombo->green = 0xE8;
  this->m_regionCombo->blue = 0xE8;
  this->AddChild(this->m_regionCombo);

  this->m_mapCombo = new UIComboBoxDefWnd();
  this->m_mapCombo->dropdownHeight = 0x50;
  this->m_mapCombo->SetSize(200, 0x12);
  this->m_mapCombo->Move(0xDC, 0x2B);
  this->m_mapCombo->borderRed = 0x64;
  this->m_mapCombo->borderGreen = 0x64;
  this->m_mapCombo->borderBlue = 0x64;
  this->m_mapCombo->red = 0xE8;
  this->m_mapCombo->green = 0xE8;
  this->m_mapCombo->blue = 0xE8;
  this->AddChild(this->m_mapCombo);

  this->m_alarmCheck = new UICheckBox();
  this->m_alarmCheck->SetSize(0x3C, 0x14);
  this->m_alarmCheck->SetSize(100, 0xC);
  this->m_alarmCheck->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(0x69E));
  this->m_alarmCheck->Move(0x1AE, 0x2B);
  this->m_alarmCheck->SetChecked(g_session.m_partyBooking.m_alarm);
  this->AddChild(this->m_alarmCheck);

  {
    std::string path = "유저인터페이스\\";
    this->m_okButton = new UIBitmapButton();
    this->m_okButton->SetBitmap((path + "btn_ok" + ".bmp").c_str(), 0, 0);
    this->m_okButton->SetBitmap((path + "btn_ok_a" + ".bmp").c_str(), 1, 0);
    this->m_okButton->SetBitmap((path + "btn_ok_b" + ".bmp").c_str(), 2, 0);
    this->m_okButton->SetSize(this->m_okButton->BitmapWidth(), this->m_okButton->BitmapHeight());
    this->m_okButton->Move(this->geometry.width - 0x2D, this->geometry.height - 0x18);
    this->m_okButton->SetId(0xC9);
    this->AddChild(this->m_okButton);
  }

  {
    std::string path = "유저인터페이스\\";
    this->m_findButton = new UIBitmapButton();
    this->m_findButton->SetBitmap((path + "btn_find" + ".bmp").c_str(), 0, 0);
    this->m_findButton->SetBitmap((path + "btn_find_a" + ".bmp").c_str(), 1, 0);
    this->m_findButton->SetBitmap((path + "btn_find_b" + ".bmp").c_str(), 2, 0);
    this->m_findButton->SetSize(this->m_findButton->BitmapWidth(), this->m_findButton->BitmapHeight());
    this->m_findButton->Move(0x1E0, 0x25);
    this->m_findButton->SetId(0x16D);
    this->AddChild(this->m_findButton);
  }

  {
    std::string path = "유저인터페이스\\basic_interface\\";
    this->m_chatButton = new UIBitmapButton();
    this->m_chatButton->SetBitmap((path + "direct_chat" + ".bmp").c_str(), 0, 0);
    this->m_chatButton->SetBitmap((path + "direct_chat" + ".bmp").c_str(), 1, 0);
    this->m_chatButton->SetBitmap((path + "direct_chat" + ".bmp").c_str(), 2, 0);
    this->m_chatButton->SetSize(this->m_chatButton->BitmapWidth(), this->m_chatButton->BitmapHeight());
    this->m_chatButton->Move(0x46, this->geometry.height - 0x18);
    this->m_chatButton->SetId(0x17D);
    this->AddChild(this->m_chatButton);
  }

  this->m_refreshButton = new UITextButton();
  this->m_refreshButton->Configure(LookupMessage(0x6E3), 0, 0, 0xC, 0, 0);
  this->m_refreshButton->m_hoverColor = 0xEFB700;
  this->m_refreshButton->SetTooltip(LookupMessage(0x6E4));
  this->m_refreshButton->Move(10, this->geometry.height - 0x16);
  this->m_refreshButton->SetId(0x17A);
  this->AddChild(this->m_refreshButton);

  {
    std::string path = "유저인터페이스\\battle_field\\btn_help";
    this->m_tabButton = new UIBitmapButton();
    this->m_tabButton->SetBitmap((path + ".bmp").c_str(), 0, 0);
    this->m_tabButton->SetBitmap((path + "_a.bmp").c_str(), 1, 0);
    this->m_tabButton->SetBitmap((path + "_b.bmp").c_str(), 2, 0);
    this->m_tabButton->SetSize(this->m_tabButton->BitmapWidth(), this->m_tabButton->BitmapHeight());
    this->m_tabButton->Move(this->geometry.width - 0x6E, this->geometry.height - 0x18);
    this->m_tabButton->SetId(0x157);
    this->AddChild(this->m_tabButton);
  }

  this->m_firstRow = 0;
  this->m_hoverIndex = -1;
  this->m_columnCount = 1;
  this->m_visibleRows = 5;
  this->m_scrollable = 0;
  this->unknown_01B4 = 0;
  this->InitRegionFilter();
  this->InitJobFilter();
  this->UpdateScrollBar();
}

// OFFSET: 0x009393E0
// FUNCTION: fn_12755
int UISeekPartyListWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {

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
          g_windowMgr.Confirm(LookupMessage(1770), 0, true, false, 0, "Message", 280, 120, 0);
          return 0;
        }
      }

      if (level < 0 || level > g_partyBookingLevelMax) {
        char buffer[260];
        std::memset(buffer, 0, sizeof(buffer));
        StringFormat(buffer, sizeof(buffer), static_cast<std::size_t>(-1), LookupMessage(2601), g_partyBookingLevelMax);
        g_windowMgr.Confirm(buffer, 0, true, false, 0, "Message", 280, 120, 0);
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
  g_session.m_hotkeyScript->HasTable("map_list");
  std::string regionName;

  int region = 1;
  for (;; ++region) {
    int id = 0;
    const char* name = nullptr;
    if (g_session.m_hotkeyScript->Call("queryRegionInfo", "d>ds", region, &id, &name)) {
      if (!name) {
        break;
      }

      this->m_regionCombo->AddOption(name, 0);
    }
  }

  g_session.m_hotkeyScript->HasTable("map_list");
  for (int current = 1; current < region; ++current) {
    std::vector<SeekPartyMapEntry> maps;
    int x = 0;
    int y = 0;
    const char* name = nullptr;
    int red = 0;
    int green = 0;
    int blue = 0;
    for (int index = 1;; ++index) {
      if (g_session.m_hotkeyScript->Call("queryMapInfo", "dd>ddsddd", current, index, &x, &y, &name, &red, &green, &blue)) {
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

// OFFSET: 0x00911CD0
// FUNCTION: fn_12523
UISeekPartyListWnd::UISeekPartyListWnd()
    : UIFrameWnd(0), m_title(nullptr), m_levelLabel(nullptr),
      m_mapLabel(nullptr), m_jobLabel(nullptr), m_filterLabel0(nullptr), m_filterLabel1(nullptr), m_filterLabel2(nullptr), m_filterLabel3(nullptr),
      unknown_00D4(nullptr), m_footerText(nullptr), m_searchEdit(nullptr), m_regionCombo(nullptr), m_mapCombo(nullptr), m_jobCategoryCombo(nullptr), m_jobCombo(nullptr), m_alarmCheck(nullptr),
      m_findButton(nullptr), m_okButton(nullptr), m_chatButton(nullptr), m_scrollBar(nullptr), m_regionMaps(), m_jobOptions(), m_region(0),
      m_jobCategory(0), m_jobCategoryChanged(0), m_restoreHeight(0), m_minimized(0), m_filterLevel(-5), m_filterMap(-5), m_filterJob(-5) {
  std::memset(this->unknown_010C, 0, sizeof(this->unknown_010C));
  std::memset(this->unknown_0194, 0, sizeof(this->unknown_0194));
  this->unknown_01C1[0] = 1;
  this->unknown_01C1[1] = 0;
  this->unknown_01C1[2] = 0;
  std::memset(this->unknown_01CA, 0, sizeof(this->unknown_01CA));
  this->m_refreshButton = nullptr;
}
