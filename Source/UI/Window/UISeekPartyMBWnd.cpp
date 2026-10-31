#include "UI/Window/UISeekPartyMBWnd.h"
#include "Contents/MessageTable.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/Packets/PacketLengths.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Lists/UIComboBoxDefWnd.h"
#include "UI/Text/UIStaticText.h"
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

// OFFSET: 0x009146D0

// OFFSET: 0x009273B0
// FUNCTION: fn_12650
void UISeekPartyMBWnd::Slot15(int width, int height) {
  char title[64];
  std::memset(title, 0, sizeof(title));
  std::sprintf(title, LookupMessage(0x698));

  this->m_titleText = new UIStaticText(*this->m_context, *this->m_controls);
  this->m_titleText->SetSize(this->geometry.width - 0x20, 0xF);
  this->m_titleText->SetTextFitted(title);
  this->m_titleText->Move(0x10, 2);
  this->m_titleText->shadow = 1;
  this->AddChild(this->m_titleText);

  this->AddCloseButton(*this->m_controls, "", 0);
  this->AddMinimizeButton(*this->m_controls, "", 0);

  std::string label;
  label = "LEVEL";
  this->m_levelText = new UIStaticText(*this->m_context, *this->m_controls);
  this->m_levelText->SetSize(this->geometry.width - 10, 0x1E);
  this->m_levelText->SetText(label.c_str(), 1);
  this->m_levelText->Move(10, 0x17);
  this->m_levelText->shadow = 1;
  this->AddChild(this->m_levelText);

  label = "MAP";
  this->unknown_00BC = new UIStaticText(*this->m_context, *this->m_controls);
  this->unknown_00BC->SetSize(this->geometry.width - 10, 0x1E);
  this->unknown_00BC->SetText(label.c_str(), 1);
  this->unknown_00BC->Move(10, 0x2D);
  this->unknown_00BC->shadow = 1;
  this->AddChild(this->unknown_00BC);

  label = "JOB";
  this->unknown_00C0 = new UIStaticText(*this->m_context, *this->m_controls);
  this->unknown_00C0->SetSize(this->geometry.width - 10, 0x1E);
  this->unknown_00C0->SetText(label.c_str(), 1);
  this->unknown_00C0->Move(10, 0x46);
  this->unknown_00C0->shadow = 1;
  this->AddChild(this->unknown_00C0);

  {
    std::string path = "유저인터페이스\\";
    this->unknown_0158 = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->unknown_0158->SetBitmap((path + "btn_ok" + ".bmp").c_str(), 0, 0);
    this->unknown_0158->SetBitmap((path + "btn_ok_a" + ".bmp").c_str(), 1, 0);
    this->unknown_0158->SetBitmap((path + "btn_ok_b" + ".bmp").c_str(), 2, 0);
    this->unknown_0158->SetSize(this->unknown_0158->BitmapWidth(), this->unknown_0158->BitmapHeight());
    this->unknown_0158->Move(this->geometry.width - 0x5D, this->geometry.height - 0x18);
    this->unknown_0158->SetId(0xB8);
    this->AddChild(this->unknown_0158);
  }

  {
    std::string path = "유저인터페이스\\";
    this->unknown_0154 = new UIBitmapButton(*this->m_context, *this->m_controls);
    this->unknown_0154->SetBitmap((path + "btn_cancel" + ".bmp").c_str(), 0, 0);
    this->unknown_0154->SetBitmap((path + "btn_cancel_a" + ".bmp").c_str(), 1, 0);
    this->unknown_0154->SetBitmap((path + "btn_cancel_b" + ".bmp").c_str(), 2, 0);
    this->unknown_0154->SetSize(this->unknown_0154->BitmapWidth(), this->unknown_0154->BitmapHeight());
    this->unknown_0154->Move(this->geometry.width - 0x2E, this->geometry.height - 0x18);
    this->unknown_0154->SetId(0xB9);
    this->AddChild(this->unknown_0154);
  }

  this->CreateJobChecks();

  this->m_levelEdit = new UIEditCtrl(*this->m_context, *this->m_editContext);
  this->m_levelEdit->SetSize(0x28, 0x10);
  this->m_levelEdit->Move(0x32, 0x14);
  this->m_levelEdit->maximum = 5;
  this->m_levelEdit->Background(0xE8, 0xE8, 0xE8);
  this->m_levelEdit->Border(0x64, 0x64, 0x64);
  this->m_levelEdit->flat = 0;
  UIEditCtrl* edit = this->m_levelEdit;
  if (const char* tooltip = LookupMessage(0x79D)) {
    edit->tooltip = tooltip;
  }
  this->AddChild(this->m_levelEdit);

  this->m_jobCategoryCombo = new UIComboBoxDefWnd(*this->m_context, *this->m_comboContext);
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

  this->m_typeCombo = new UIComboBoxDefWnd(*this->m_context, *this->m_comboContext);
  this->m_typeCombo->dropdownHeight = 0x50;
  this->m_typeCombo->SetSize(0x64, 0x12);
  this->m_typeCombo->Move(0x32, 0x2B);
  this->m_typeCombo->borderRed = 0x64;
  this->m_typeCombo->borderGreen = 0x64;
  this->m_typeCombo->borderBlue = 0x64;
  this->m_typeCombo->red = 0xE8;
  this->m_typeCombo->green = 0xE8;
  this->m_typeCombo->blue = 0xE8;
  this->AddChild(this->m_typeCombo);

  this->m_subTypeCombo = new UIComboBoxDefWnd(*this->m_context, *this->m_comboContext);
  this->m_subTypeCombo->dropdownHeight = 0x50;
  this->m_subTypeCombo->SetSize(0xBE, 0x12);
  this->m_subTypeCombo->Move(0x9B, 0x2B);
  this->m_subTypeCombo->borderRed = 0x64;
  this->m_subTypeCombo->borderGreen = 0x64;
  this->m_subTypeCombo->borderBlue = 0x64;
  this->m_subTypeCombo->red = 0xE8;
  this->m_subTypeCombo->green = 0xE8;
  this->m_subTypeCombo->blue = 0xE8;
  this->AddChild(this->m_subTypeCombo);

  this->LoadRegionMaps();
  this->InitJobCategoryCombo();
}

// OFFSET: 0x009458B0
// FUNCTION: fn_12835
void UISeekPartyMBWnd::LoadRegionMaps() {
  g_luaScript->HasTable("map_list");
  std::string unused;

  int regionCount = 2;
  for (;; ++regionCount) {
    int regionId = 0;
    const char* regionName = nullptr;
    if (g_luaScript->Call("queryRegionInfo", "d>ds", regionCount, &regionId, &regionName)) {
      if (!regionName) {
        break;
      }
      this->m_typeCombo->AddOption(regionName, 0);
    }
  }

  g_luaScript->HasTable("map_list");
  for (int region = 2; region < regionCount; ++region) {
    std::vector<std::pair<std::string, int>> maps;
    for (int index = 1;; ++index) {
      int mapId = 0;
      int level = 0;
      const char* mapName = nullptr;
      int red = 0;
      int green = 0;
      int blue = 0;
      if (g_luaScript->Call("queryMapInfo", "dd>ddsddd", region, index, &mapId, &level, &mapName, &red, &green, &blue)) {
        if (!mapName) {
          break;
        }
        std::string name;
        name = mapName;
        const int color = (static_cast<unsigned char>(blue) << 16) | (static_cast<unsigned char>(green) << 8) | static_cast<unsigned char>(red);
        maps.push_back(std::pair<std::string, int>(name, color));
      }
    }
    this->m_subTypes.insert(std::pair<int, std::vector<std::pair<std::string, int>>>(region, maps));
  }

  this->m_typeCombo->Select(0);
  this->SendMsg(this->m_typeCombo, 0x27, 0, 0, 0, 0);
}
