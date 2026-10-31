#include "UI/Window/UISeekPartyMBWnd.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Window/UISeekPartyWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowDialogs.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace {
#pragma pack(push, 1)
struct PACKET_CZ_PARTY_BOOKING_REQ_REGISTER {
  /* 0000 */ short packetType;
  /* 0002 */ PARTY_BOOKING_DETAIL detail;
};
#pragma pack(pop)
}

// OFFSET: 0x00939EE0
// FUNCTION: fn_12756
int UISeekPartyMBWnd::SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) {
  WindowDialogs& dialogs = *reinterpret_cast<WindowDialogs*>(&g_windowMgr);

  switch (message) {
  case 6:
    switch (a) {
    case 184: {
      for (int i = 0; i < 6; ++i) {
        this->m_registration.detail.job[i] = -1;
      }

      int count = 0;
      for (int category = 0; category < 8; ++category) {
        std::map<unsigned short, UICheckBox*>& checks = this->m_jobChecks[category];
        for (auto it = checks.begin(); it != checks.end(); ++it) {
          if (it->second->geometry.unknown_0030) {
            if (count < 6) {
              this->m_registration.detail.job[count] = it->first;
            }
            ++count;
          }
        }
      }

      if (count == 0) {
        dialogs.Confirm(LookupMessage(1768), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }

      if (count > 6) {
        char text[100];
        std::memset(text, 0, sizeof(text));
        std::sprintf(text, LookupMessage(1769), count);
        dialogs.Confirm(text, 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }

      std::string level(this->m_levelEdit->Text());
      for (unsigned int i = 0; i < level.size(); ++i) {
        if (level[i] < '/' || level[i] > '9') {
          dialogs.Confirm(LookupMessage(1770), 0, true, false, 0, "Message", 280, 120, 0);
          return 0;
        }
      }

      this->m_registration.detail.level = static_cast<short>(std::atoi(this->m_levelEdit->Text()));
      g_partyBookingLevelLimit = this->m_registration.detail.level;
      const int maximum = g_partyBookingLevelMax;
      if (this->m_registration.detail.level >= 1 && this->m_registration.detail.level <= maximum) {
        this->m_registration.detail.mapId = static_cast<short>((this->m_type + 1) * 256 | this->m_subType);
        if (!this->m_frameContext->windows.CloseWindow(164)) {
          PACKET_CZ_PARTY_BOOKING_REQ_REGISTER packet;
          packet.packetType = ID_CZ_PARTY_BOOKING_REQ_REGISTER;
          packet.detail = this->m_registration.detail;
          GetRagConnection().Send(g_packetLengths.FixedLength(packet.packetType), &packet);
        }
      } else {
        char text[260];
        std::memset(text, 0, sizeof(text));
        StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(2601), maximum);
        dialogs.Confirm(text, 0, true, false, 0, "Message", 280, 120, 0);
      }
      break;
    }
    case 185:
    case 201:
      this->m_frameContext->windows.CloseWindow(168);
      this->m_frameContext->windows.CloseWindow(170);
      break;
    case 202:
      if (this->m_minimized) {
        this->Resize(this->geometry.width, this->m_restoreHeight);
      } else {
        this->m_restoreHeight = this->geometry.height;
        this->Resize(this->geometry.width, 17);
      }

      this->m_minimized = !this->m_minimized;
      this->Invalidate();
      break;
    }
    break;
  case 34:
    this->m_saved = reinterpret_cast<SeekPartyMBWndInfo*>(a);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }

    this->Move(this->m_saved->x, this->m_saved->y);
    this->SendMsg(nullptr, 14, 3, this->m_saved->width, this->m_saved->height, 0);
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    break;
  case 39:
    if (sender == this->m_typeCombo) {
      const int key = a + 2;
      this->m_subTypeCombo->Clear();
      auto found = this->m_subTypes.find(key);
      if (found != this->m_subTypes.end()) {
        const std::vector<std::pair<std::string, int>> entries = found->second;
        for (auto it = entries.begin(); it != entries.end(); ++it) {
          const std::pair<std::string, int> entry = *it;
          this->m_subTypeCombo->AddOption(entry.first.c_str(), entry.second);
        }
      }

      this->m_type = static_cast<short>(a);
      this->m_subTypeCombo->Select(0);
      this->m_subType = 0;
    } else if (sender == this->m_subTypeCombo) {
      this->m_subType = static_cast<short>(a);
    } else if (sender == this->m_jobCategoryCombo) {
      this->ShowJobCategory(a);
    }
    break;
  default:
    return UIFrameWnd::SendMsg(sender, message, a, b, c, d);
  }

  return 0;
}

// OFFSET: 0x00944370
// FUNCTION: fn_12831
void UISeekPartyMBWnd::CreateJobChecks() {
  int counts[8] = {4, 6, 13, 6, 7, 6, 13, 8};
  int message = 0;
  int starts[8] = {1, 5, 11, 24, 30, 37, 43, 56};

  for (int i = 0; i < counts[0]; ++i) {
    UICheckBox* check = new UICheckBox(*this->m_context, *this->m_controls);
    check->SetSize(110, 12);
    const int job = i + 1;
    check->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(0x6AF + job));
    if (i < 8) {
      check->Move(54, 99 + i * 25);
    } else {
      check->Move(180, 99 + (i - 8) * 25);
    }
    check->SetChecked(0);
    check->SetHidden(true);
    check->m_disabled = 1;
    this->AddChild(check);
    this->m_jobChecks[0].emplace(static_cast<unsigned short>(job), check);
  }

  for (int category = 1; category < 8; ++category) {
    for (int i = 0; i < counts[category]; ++i) {
      UICheckBox* check = new UICheckBox(*this->m_context, *this->m_controls);
      check->SetSize(110, 12);
      check->SetImages("유저인터페이스\\checkbox_1.bmp", "유저인터페이스\\checkbox_0.bmp", LookupMessage(0x65D + message));
      if (i < 8) {
        check->Move(54, 99 + i * 25);
      } else {
        check->Move(180, 99 + (i - 8) * 25);
      }
      check->SetChecked(0);
      check->SetHidden(true);
      check->m_disabled = 1;
      this->AddChild(check);
      this->m_jobChecks[category].emplace(static_cast<unsigned short>(starts[category] + i), check);
      ++message;
    }
  }

  this->ShowJobCategory(0);
}

// OFFSET: 0x009447D0
// FUNCTION: fn_12832
void UISeekPartyMBWnd::InitJobCategoryCombo() {
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DB), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DC), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DD), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DE), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6DF), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E0), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E1), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(0x6E2), 0);
  this->m_jobCategoryCombo->Select(0);
}

// OFFSET: 0x00946150
// FUNCTION: fn_12838
void UISeekPartyMBWnd::ShowJobCategory(int category) {
  if (category == this->m_jobCategory) {
    return;
  }

  if (this->m_jobCategory != -1) {
    for (int i = 0; i < 8; ++i) {
      std::map<unsigned short, UICheckBox*>& checks = this->m_jobChecks[i];
      auto it = checks.begin();
      if (it == checks.end()) {
        return;
      }

      for (; it != checks.end(); ++it) {
        UICheckBox* check = it->second;
        check->SetHidden(true);
        check->m_disabled = 1;
      }
    }
  }

  std::map<unsigned short, UICheckBox*>& checks = this->m_jobChecks[category];
  auto it = checks.begin();
  if (it == checks.end()) {
    return;
  }

  for (; it != checks.end(); ++it) {
    UICheckBox* check = it->second;
    check->SetHidden(false);
    check->m_disabled = 0;
  }

  this->m_jobCategory = category;
  this->Invalidate();
}

// OFFSET: 0x00911EE0
// FUNCTION: fn_12524
UISeekPartyMBWnd::UISeekPartyMBWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), unknown_00BC(nullptr), unknown_00C0(nullptr), m_levelEdit(nullptr), m_typeCombo(nullptr), m_subTypeCombo(nullptr), m_jobCategoryCombo(nullptr),
      m_jobChecks(), m_subTypes(), unknown_0154(nullptr), unknown_0158(nullptr), m_jobCategory(-1), m_subType(0), m_restoreHeight(0), m_minimized(0),
      m_controls(&controls), m_titleText(nullptr), m_levelText(nullptr) {
}
