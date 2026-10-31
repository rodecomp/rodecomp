#include "UI/Window/UIAccountLimitedToolWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Text/UITextViewer.h"
#include "Utility/Text/FormatString.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

#pragma pack(push, 1)
struct AccountLimitedSearchPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ char name[50];
};

struct AccountLimitedDeletePacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int accountId;
};

struct AccountLimitedAddPacket {
  /* 0000 */ std::int16_t id;
  /* 0002 */ int accountId;
  /* 0006 */ int itemId;
  /* 000A */ __time32_t start;
  /* 000E */ __time32_t end;
  /* 0012 */ __time32_t period;
};
#pragma pack(pop)

static_assert(sizeof(AccountLimitedSearchPacket) == 52);
static_assert(sizeof(AccountLimitedDeletePacket) == 6);
static_assert(sizeof(AccountLimitedAddPacket) == 22);

struct AccountLimitedRange {
  /* 0000 */ int minimum;
  /* 0004 */ int maximum;
};

// OFFSET: 0x0094A450
// FUNCTION: fn_12890
void UIAccountLimitedToolWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "", 0);

  const int editPositions[15][2] = {
    {90, 25},   {90, 77},   {90, 99},   {125, 99}, {160, 99}, {268, 99},  {305, 99},  {90, 121},
    {125, 121}, {160, 121}, {268, 121}, {305, 121}, {90, 151}, {240, 151}, {285, 151},
  };
  for (int i = 0; i < 15; ++i) {
    this->m_edits[i] = new UIEditCtrl(*this->m_context, *this->m_editContext);
    this->m_edits[i]->flat = 1;
    this->m_edits[i]->disabled = 1;
    this->m_edits[i]->Invalidate();

    switch (i) {
    case 0:
      this->m_edits[i]->SetSize(200, 17);
      this->m_edits[i]->flat = 0;
      this->m_edits[i]->disabled = 0;
      this->m_edits[i]->Invalidate();
      break;
    case 1:
      this->m_edits[i]->SetSize(60, 17);
      break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
      this->m_edits[i]->SetSize(25, 17);
      break;
    case 12:
      this->m_edits[i]->SetSize(80, 17);
      break;
    case 13:
    case 14:
      this->m_edits[i]->SetSize(35, 17);
      break;
    }

    this->m_edits[i]->maximum = 24;
    this->m_edits[i]->Move(editPositions[i][0], editPositions[i][1]);
    this->m_edits[i]->Background(242, 242, 242);
    this->AddChild(this->m_edits[i]);
  }

  this->m_accountIdText = new UIStaticText(*this->m_context, *this->m_controls);
  this->m_accountIdText->SetSize(200, 17);
  this->m_accountIdText->Move(90, 47);
  this->m_accountIdText->background = 0xFF969696;
  this->m_accountIdText->Invalidate();
  this->AddChild(this->m_accountIdText);

  const int buttonPositions[3][2] = {{this->geometry.width - 70, 22}, {15, 184}, {260, 184}};
  this->m_message0 = 561;
  const std::string prefix = "유저인터페이스\\";
  const int buttonIds[3] = {321, 561, 211};
  const char* buttonNames[3][4] = {
    {"btn_search", "btn_search_a", "btn_search_b", "btn_search_c"},
    {"btn_AddItem", "btn_AddItem_a", "btn_AddItem_b", "btn_AddItem_c"},
    {"btn_deleteItem", "btn_deleteItem_a", "btn_deleteItem_b", "btn_deleteItem_c"},
  };
  for (int i = 0; i < 3; ++i) {
    this->m_buttons[i] = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->m_buttons[i]->SetBitmap((prefix + buttonNames[i][0] + ".bmp").c_str(), 0, 0);
    this->m_buttons[i]->SetBitmap((prefix + buttonNames[i][1] + ".bmp").c_str(), 1, 0);
    this->m_buttons[i]->SetBitmap((prefix + buttonNames[i][2] + ".bmp").c_str(), 2, 0);
    this->m_buttons[i]->SetBitmap((prefix + buttonNames[i][3] + ".bmp").c_str(), 3, 0);
    this->m_buttons[i]->SetSize(this->m_buttons[i]->BitmapWidth(), this->m_buttons[i]->BitmapHeight());
    this->m_buttons[i]->Move(buttonPositions[i][0], buttonPositions[i][1]);
    this->m_buttons[i]->SetId(buttonIds[i]);
    this->m_buttons[i]->Enable(1);
    this->AddChild(this->m_buttons[i]);
    this->m_buttons[i]->SetDisabled(i == 0 ? 0 : 1);
    this->m_buttons[i]->Invalidate();
  }

  this->m_viewer = new UITextViewer(*this->m_context, *this->m_textContext);
  const int buttonHeight = this->m_buttons[1]->geometry.height;
  this->m_viewer->Configure(10, buttonHeight + 189, this->geometry.width - 20, this->geometry.height - buttonHeight - 199, 0);
  this->m_viewer->backgroundColor = 0xFF969696;
  this->m_viewer->Invalidate();
  this->AddChild(this->m_viewer);
}

// OFFSET: 0x0094CF50
// FUNCTION: fn_12901
void UIAccountLimitedToolWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, false, LookupMessage(3650), 0);
  this->DrawPanelBackground(17, 0, "");
  this->DrawTextAt(controls.fonts, controls.locale, 5, 28, LookupMessage(3651), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 50, LookupMessage(3652), 0, 0, 12, 0, 0, 0);
  this->FillRegion(3, 70, this->geometry.width - 6, 2, 0x99938A);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 80, LookupMessage(3653), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 102, LookupMessage(3654), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 117, 102, "/", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 152, 102, "/", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 187, 102, "(YY/MM/DD)", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 296, 102, ":", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 335, 102, "(HH:MM)", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 124, LookupMessage(3655), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 117, 124, "/", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 152, 124, "/", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 187, 124, "(YY/MM/DD)", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 296, 124, ":", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 335, 124, "(HH:MM)", 0, 0, 12, 0, 0, 0);
  this->FillRegion(3, 144, this->geometry.width - 6, 2, 0x99938A);
  this->DrawTextAt(controls.fonts, controls.locale, 5, 154, LookupMessage(3916), 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 172, 154, "(DD)", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 278, 154, ":", 0, 0, 12, 0, 0, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 322, 154, "(HH:MM)", 0, 0, 12, 0, 0, 0);
  this->FillRegion(3, 174, this->geometry.width - 6, 2, 0x99938A);
}

// OFFSET: 0x0094F410
// FUNCTION: fn_12938
int UIAccountLimitedToolWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 321) {
      const std::string name = this->m_edits[0]->Text();
      if (name.empty()) {
        this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2512)), 60, 0, 0);
        if (this->m_viewer->verticalOverflow != 0) {
          this->m_viewer->SendMsg(nullptr, 7, this->m_viewer->LineCount(), 0, 0, 0);
        }
        this->m_edits[0]->SelectAll();
      } else if (name.size() < 50) {
        AccountLimitedSearchPacket packet;
        packet.id = 0xB53;
        std::memset(packet.name, 0, sizeof(packet.name));
        strncpy_s(packet.name, sizeof(packet.name), name.c_str(), name.size());
        GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
      }
    } else if (argument1 == 201) {
      g_modeMgr.GetGameMode()->SendMsg(319, 0, 0, 0, 0);
    } else if (argument1 == 211) {
      AccountLimitedDeletePacket packet;
      packet.id = 0xB4A;
      packet.accountId = std::atoi(this->m_accountIdText->GetText().c_str());
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
    } else if (argument1 == 561) {
      if (std::atoi(this->m_edits[1]->Text()) < 1) {
        this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2508)), 60, 0, 0);
        if (this->m_viewer->verticalOverflow != 0) {
          this->m_viewer->SendMsg(nullptr, 7, this->m_viewer->LineCount(), 0, 0, 0);
        }
        this->m_edits[1]->SelectAll();
        return 0;
      }

      const AccountLimitedRange ranges[5] = {{23, 999}, {1, 12}, {1, 31}, {0, 23}, {0, 59}};
      for (int i = 2; i < 12; ++i) {
        if (this->m_edits[i]->Length() == 0) {
          this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2509)), 60, 0, 0);
          if (this->m_viewer->verticalOverflow != 0) {
            this->m_viewer->SendMsg(nullptr, 7, this->m_viewer->LineCount(), 0, 0, 0);
            this->m_edits[i]->SelectAll();
            return 0;
          }
        }

        const AccountLimitedRange& range = ranges[i % 5 - 2];
        if (std::atoi(this->m_edits[i]->Text()) < range.minimum || std::atoi(this->m_edits[i]->Text()) > range.maximum) {
          this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2510)), 60, 0, 0);
          if (this->m_viewer->verticalOverflow != 0) {
            this->m_viewer->SendMsg(nullptr, 7, this->m_viewer->LineCount(), 0, 0, 0);
          }
          this->m_edits[i]->SelectAll();
          return 0;
        }
      }

      if (std::atoi(this->m_edits[12]->Text()) > 14) {
        this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(3917)), 60, 0, 0);
        this->m_edits[12]->SelectAll();
        return 0;
      }

      __time32_t startTime = _time32(nullptr);
      tm* startDate = _localtime32(&startTime);
      const int startYear = std::atoi(this->m_edits[2]->Text());
      startDate->tm_year = startYear > 2000 ? startYear - 1900 : startYear + 100;
      startDate->tm_mon = std::atoi(this->m_edits[3]->Text()) - 1;
      startDate->tm_mday = std::atoi(this->m_edits[4]->Text());
      startDate->tm_hour = std::atoi(this->m_edits[5]->Text());
      startDate->tm_min = std::atoi(this->m_edits[6]->Text());
      startDate->tm_sec = 0;
      startTime = _mktime32(startDate);

      __time32_t endTime = _time32(nullptr);
      tm* endDate = _localtime32(&endTime);
      const int endYear = std::atoi(this->m_edits[7]->Text());
      startDate->tm_year = endYear > 2000 ? endYear - 1900 : endYear + 100;
      endDate->tm_mon = std::atoi(this->m_edits[8]->Text()) - 1;
      endDate->tm_mday = std::atoi(this->m_edits[9]->Text());
      endDate->tm_hour = std::atoi(this->m_edits[10]->Text());
      endDate->tm_min = std::atoi(this->m_edits[11]->Text());
      endDate->tm_sec = 0;
      endTime = _mktime32(endDate);

      __time32_t period = _time32(nullptr);
      tm* periodDate = _localtime32(&period);
      const time_t days = std::atoi(this->m_edits[12]->Text());
      periodDate->tm_mday = days != 0 ? static_cast<int>(days) : 15;
      periodDate->tm_hour = std::atoi(this->m_edits[13]->Text());
      periodDate->tm_min = std::atoi(this->m_edits[14]->Text());
      periodDate->tm_sec = 0;
      period = _mktime32(periodDate);

      AccountLimitedAddPacket packet;
      packet.id = 0xBAC;
      packet.accountId = std::atoi(this->m_accountIdText->GetText().c_str());
      packet.itemId = std::atoi(this->m_edits[1]->Text());
      packet.start = startTime;
      packet.end = endTime;
      packet.period = period;
      GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
    }
  } else if (message == 64) {
    switch (argument1) {
    case 0:
      this->m_accountIdText->SetNumber(argument2);
      this->m_buttons[1]->SetDisabled(0);
      this->m_buttons[1]->Invalidate();
      this->m_buttons[2]->SetDisabled(0);
      this->m_buttons[2]->Invalidate();
      for (int i = 1; i < 15; ++i) {
        this->m_edits[i]->flat = 0;
        this->m_edits[i]->disabled = 0;
        this->m_edits[i]->Invalidate();
      }
      break;
    case 1:
      switch (argument2) {
      case 0: {
        const std::string text = FormatOwnedString(
          LookupMessage(4024), this->m_edits[0]->Text(), std::atoi(this->m_edits[1]->Text()), std::atoi(this->m_edits[3]->Text()),
          std::atoi(this->m_edits[4]->Text()), std::atoi(this->m_edits[5]->Text()), std::atoi(this->m_edits[6]->Text()), std::atoi(this->m_edits[7]->Text()),
          std::atoi(this->m_edits[8]->Text()), std::atoi(this->m_edits[9]->Text()), std::atoi(this->m_edits[10]->Text()), std::atoi(this->m_edits[11]->Text())
        );
        this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(text.c_str()), 60, 0, 0);
        this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2515)), 60, 0, 0);
        if (this->m_viewer->verticalOverflow != 0) {
          this->m_viewer->SendMsg(nullptr, 7, this->m_viewer->LineCount(), 0, 0, 0);
        }
        this->m_buttons[2]->SetDisabled(0);
        this->m_buttons[2]->Invalidate();
        for (int i = 1; i < 15; ++i) {
          this->m_edits[i]->flat = 1;
          this->m_edits[i]->disabled = 1;
          this->m_edits[i]->Invalidate();
        }
        break;
      }
      case 1:
        this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2516)), 60, 0, 0);
        if (this->m_viewer->verticalOverflow != 0) {
          this->m_viewer->SendMsg(nullptr, 7, this->m_viewer->LineCount(), 0, 0, 0);
        }
        this->m_buttons[1]->SetDisabled(1);
        this->m_buttons[1]->Invalidate();
        this->m_buttons[2]->SetDisabled(1);
        this->m_buttons[2]->Invalidate();
        break;
      case 3:
        this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2516)), 60, 0, 0);
        if (this->m_viewer->verticalOverflow != 0) {
          this->m_viewer->SendMsg(nullptr, 7, this->m_viewer->LineCount(), 0, 0, 0);
        }
        this->m_buttons[1]->SetDisabled(1);
        this->m_buttons[1]->Invalidate();
        this->m_buttons[2]->SetDisabled(1);
        this->m_buttons[2]->Invalidate();
        break;
      }
      break;
    case 2:
      if (argument2 == 1) {
        this->m_viewer->SendMsg(nullptr, 29, reinterpret_cast<std::intptr_t>(LookupMessage(2518)), 60, 0, 0);
        if (this->m_viewer->verticalOverflow != 0) {
          this->m_viewer->SendMsg(nullptr, 7, this->m_viewer->LineCount(), 0, 0, 0);
        }
      } else {
        this->m_viewer->SendMsg(
          nullptr, 29, reinterpret_cast<std::intptr_t>(FormatOwnedString(LookupMessage(2519), this->m_edits[0]->Text()).c_str()), 60, 0, 0
        );
        if (this->m_viewer->verticalOverflow != 0) {
          this->m_viewer->SendMsg(nullptr, 7, this->m_viewer->LineCount(), 0, 0, 0);
        }
        this->m_edits[0]->SetText("");
        this->m_accountIdText->SetText("", 0);
        for (int i = 1; i < 15; ++i) {
          this->m_edits[i]->SetText("");
          this->m_edits[i]->flat = 1;
          this->m_edits[i]->disabled = 1;
          this->m_edits[i]->Invalidate();
        }
        this->m_buttons[1]->SetDisabled(1);
        this->m_buttons[1]->Invalidate();
        this->m_buttons[2]->SetDisabled(1);
        this->m_buttons[2]->Invalidate();
        this->InvalidateChildren();
      }
      break;
    }
  }

  return 0;
}

// OFFSET: 0x00946800
// FUNCTION: fn_12846
UIAccountLimitedToolWnd::UIAccountLimitedToolWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls, UIEditContext& edits, UITextViewerContext& texts)
    : UIFrameWnd(windows, frames, 0), m_accountIdText(nullptr), m_viewer(nullptr), m_controls(&controls), m_editContext(&edits), m_textContext(&texts) {
  SecureZeroMemory(this->m_edits, sizeof(this->m_edits));
  SecureZeroMemory(this->m_buttons, sizeof(this->m_buttons));
}
