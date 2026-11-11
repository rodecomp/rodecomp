#include "UI/Party/UISeekPartyMBWnd.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"
#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
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
#include "UI/Party/UISeekPartyWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowManagerWindows.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
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

  switch (message) {
  case UM_COMMAND:
    switch (a) {
    case ID_OK: {
      for (int i = 0; i < 6; ++i) {
        this->m_registration.detail.job[i] = -1;
      }

      int count = 0;
      for (int category = 0; category < 8; ++category) {
        std::map<unsigned short, UICheckBox*>& checks = this->m_jobChecks[category];
        auto it = checks.begin();
        auto end = checks.end();
        if (it == end) {
          break;
        }

        for (; it != end; ++it) {
          if (it->second->geometry.state) {
            if (count < 6) {
              this->m_registration.detail.job[count] = it->first;
            }
            ++count;
          }
        }
      }

      if (count == 0) {
        g_windowMgr.Confirm(LookupMessage(MSI_PARTY_BOOKING_ERR2), 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }

      if (count > 6) {
        char text[100];
        std::memset(text, 0, sizeof(text));
        std::sprintf(text, LookupMessage(MSI_PARTY_BOOKING_ERR3), count);
        g_windowMgr.Confirm(text, 0, true, false, 0, "Message", 280, 120, 0);
        break;
      }

      std::string level(this->m_levelEdit->Text());
      for (unsigned int i = 0; i < level.size(); ++i) {
        if (level[i] < '/' || level[i] > '9') {
          g_windowMgr.Confirm(LookupMessage(MSI_PARTY_BOOKING_ERR4), 0, true, false, 0, "Message", 280, 120, 0);
          return 0;
        }
      }

      this->m_registration.detail.level = static_cast<short>(std::atoi(this->m_levelEdit->Text()));
      g_session.m_partyBookingLevelLimit = this->m_registration.detail.level;
      const int maximum = g_session.m_maxLevel;
      if (this->m_registration.detail.level >= 1 && this->m_registration.detail.level <= maximum) {
        this->m_registration.detail.mapId = static_cast<short>((this->m_type + 1) * 256 | this->m_subType);
        if (!g_windowMgr.CloseWindow(WID_SEEKPARTYWND)) {
          PACKET_CZ_PARTY_BOOKING_REQ_REGISTER packet;
          packet.packetType = ID_CZ_PARTY_BOOKING_REQ_REGISTER;
          packet.detail = this->m_registration.detail;
          GetRagConnection().Send(reinterpret_cast<PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
        }
      } else {
        char text[260];
        std::memset(text, 0, sizeof(text));
        StringFormat(text, sizeof(text), static_cast<std::size_t>(-1), LookupMessage(MSI_FAILED_TO_WRITE_MAIL), maximum);
        g_windowMgr.Confirm(text, 0, true, false, 0, "Message", 280, 120, 0);
      }
      break;
    }
    case ID_CANCEL:
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_SEEKPARTYMBWND);
      g_windowMgr.CloseWindow(WID_JOBLISTWND);
      break;
    case ID_MINI:
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
  case UM_SETINFO:
    this->m_saved = reinterpret_cast<SeekPartyMBWndInfo*>(a);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }

    this->Move(this->m_saved->x, this->m_saved->y);
    this->SendMsg(nullptr, UM_RESIZE, 3, this->m_saved->width, this->m_saved->height, 0);
    this->SendMsg(nullptr, UM_REFRESHITEM, 0, 0, 0, 0);
    break;
  case UM_COMBOBOX_SELECTED:
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

  for (int i = 0; i < 4; ++i) {
    UICheckBox* check = new UICheckBox();
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
      UICheckBox* check = new UICheckBox();
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
  this->m_jobCategoryCombo->AddOption(LookupMessage(MSI_PARTY_BOOKING_TYPE), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(MSI_PARTY_BOOKING_TYPE_1), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(MSI_PARTY_BOOKING_TYPE_2), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(MSI_PARTY_BOOKING_TYPE_3_1), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(MSI_PARTY_BOOKING_TYPE_3_2), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(MSI_PARTY_BOOKING_TYPE_1PLUS), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(MSI_PARTY_BOOKING_TYPE_2PLUS), 0);
  this->m_jobCategoryCombo->AddOption(LookupMessage(MSI_PARTY_BOOKING_TYPE_ETC), 0);
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
      const auto end = checks.end();
      if (it == end) {
        return;
      }

      for (; it != end; ++it) {
        UICheckBox* check = it->second;
        check->SetHidden(true);
        check->m_disabled = 1;
      }
    }
  }

  std::map<unsigned short, UICheckBox*>& checks = this->m_jobChecks[category];
  auto it = checks.begin();
  const auto end = checks.end();
  if (it == end) {
    return;
  }

  for (; it != end; ++it) {
    UICheckBox* check = it->second;
    check->SetHidden(false);
    check->m_disabled = 0;
  }

  this->m_jobCategory = category;
  this->Invalidate();
}

// OFFSET: 0x00911EE0
// FUNCTION: fn_12524
UISeekPartyMBWnd::UISeekPartyMBWnd()
    : UIFrameWnd(0), m_titleText(nullptr), m_levelText(nullptr), typeLabel(nullptr), jobLabel(nullptr), m_levelEdit(nullptr), m_typeCombo(nullptr), m_subTypeCombo(nullptr), m_jobCategoryCombo(nullptr),
      m_jobChecks(), m_subTypes(), cancelButton(nullptr), confirmButton(nullptr), m_jobCategory(-1), m_subType(0), m_restoreHeight(0), m_minimized(0) {
}

// OFFSET: 0x009146D0
// FUNCTION: fn_12542
UISeekPartyMBWnd::~UISeekPartyMBWnd() = default;

// OFFSET: 0x009273B0
// FUNCTION: fn_12650
void UISeekPartyMBWnd::OnCreate(int width, int height) {
  char title[64];
  std::memset(title, 0, sizeof(title));
  std::sprintf(title, LookupMessage(MSI_SEEK_PARTY_MEMBER));

  this->m_titleText = new UIStaticText();
  this->m_titleText->SetSize(this->geometry.width - 0x20, 0xF);
  this->m_titleText->SetTextFitted(title);
  this->m_titleText->Move(0x10, 2);
  this->m_titleText->shadow = 1;
  this->AddChild(this->m_titleText);

  this->AddCloseButton("", 0);
  this->AddMinimizeButton("", 0);

  std::string label;
  label = "LEVEL";
  this->m_levelText = new UIStaticText();
  this->m_levelText->SetSize(this->geometry.width - 10, 0x1E);
  this->m_levelText->SetText(label.c_str(), 1);
  this->m_levelText->Move(10, 0x17);
  this->m_levelText->shadow = 1;
  this->AddChild(this->m_levelText);

  label = "MAP";
  this->typeLabel = new UIStaticText();
  this->typeLabel->SetSize(this->geometry.width - 10, 0x1E);
  this->typeLabel->SetText(label.c_str(), 1);
  this->typeLabel->Move(10, 0x2D);
  this->typeLabel->shadow = 1;
  this->AddChild(this->typeLabel);

  label = "JOB";
  this->jobLabel = new UIStaticText();
  this->jobLabel->SetSize(this->geometry.width - 10, 0x1E);
  this->jobLabel->SetText(label.c_str(), 1);
  this->jobLabel->Move(10, 0x46);
  this->jobLabel->shadow = 1;
  this->AddChild(this->jobLabel);

  {
    std::string path = "유저인터페이스\\";
    this->confirmButton = new UIBitmapButton();
    this->confirmButton->SetBitmap((path + "btn_ok" + ".bmp").c_str(), 0, 0);
    this->confirmButton->SetBitmap((path + "btn_ok_a" + ".bmp").c_str(), 1, 0);
    this->confirmButton->SetBitmap((path + "btn_ok_b" + ".bmp").c_str(), 2, 0);
    this->confirmButton->SetSize(this->confirmButton->BitmapWidth(), this->confirmButton->BitmapHeight());
    this->confirmButton->Move(this->geometry.width - 0x5D, this->geometry.height - 0x18);
    this->confirmButton->SetId(0xB8);
    this->AddChild(this->confirmButton);
  }

  {
    std::string path = "유저인터페이스\\";
    this->cancelButton = new UIBitmapButton();
    this->cancelButton->SetBitmap((path + "btn_cancel" + ".bmp").c_str(), 0, 0);
    this->cancelButton->SetBitmap((path + "btn_cancel_a" + ".bmp").c_str(), 1, 0);
    this->cancelButton->SetBitmap((path + "btn_cancel_b" + ".bmp").c_str(), 2, 0);
    this->cancelButton->SetSize(this->cancelButton->BitmapWidth(), this->cancelButton->BitmapHeight());
    this->cancelButton->Move(this->geometry.width - 0x2E, this->geometry.height - 0x18);
    this->cancelButton->SetId(0xB9);
    this->AddChild(this->cancelButton);
  }

  this->CreateJobChecks();

  this->m_levelEdit = new UIEditCtrl();
  this->m_levelEdit->SetSize(0x28, 0x10);
  this->m_levelEdit->Move(0x32, 0x14);
  this->m_levelEdit->maximum = 5;
  this->m_levelEdit->Background(0xE8, 0xE8, 0xE8);
  this->m_levelEdit->Border(0x64, 0x64, 0x64);
  this->m_levelEdit->flat = 0;
  UIEditCtrl* edit = this->m_levelEdit;
  if (const char* tooltip = LookupMessage(MSI_SEEK_PARTY_LEVEL_HELP)) {
    edit->tooltip = tooltip;
  }
  this->AddChild(this->m_levelEdit);

  this->m_jobCategoryCombo = new UIComboBoxDefWnd();
  this->m_jobCategoryCombo->dropdownHeight = 0x50;
  this->m_jobCategoryCombo->SetSize(0x96, 0x12);
  this->m_jobCategoryCombo->Move(0x32, 0x44);
  this->m_jobCategoryCombo->SetBorderColor(0x64, 0x64, 0x64);
  this->m_jobCategoryCombo->SetColor(0xE8, 0xE8, 0xE8);
  this->AddChild(this->m_jobCategoryCombo);

  this->m_typeCombo = new UIComboBoxDefWnd();
  this->m_typeCombo->dropdownHeight = 0x50;
  this->m_typeCombo->SetSize(0x64, 0x12);
  this->m_typeCombo->Move(0x32, 0x2B);
  this->m_typeCombo->SetBorderColor(0x64, 0x64, 0x64);
  this->m_typeCombo->SetColor(0xE8, 0xE8, 0xE8);
  this->AddChild(this->m_typeCombo);

  this->m_subTypeCombo = new UIComboBoxDefWnd();
  this->m_subTypeCombo->dropdownHeight = 0x50;
  this->m_subTypeCombo->SetSize(0xBE, 0x12);
  this->m_subTypeCombo->Move(0x9B, 0x2B);
  this->m_subTypeCombo->SetBorderColor(0x64, 0x64, 0x64);
  this->m_subTypeCombo->SetColor(0xE8, 0xE8, 0xE8);
  this->AddChild(this->m_subTypeCombo);

  this->LoadRegionMaps();
  this->InitJobCategoryCombo();
}

// OFFSET: 0x009458B0
// FUNCTION: fn_12835
void UISeekPartyMBWnd::LoadRegionMaps() {
  struct RegionInfo {
    int id;
    const char* name;
  };
  struct MapInfo {
    int id;
    int level;
    const char* name;
    int red;
    int green;
    int blue;
  };
  struct MapOption {
    std::string name;
    int color;
  };

  g_session.m_hotkeyScript->HasTable("map_list");
  std::string unused;

  int regionCount = 2;
  for (;; ++regionCount) {
    RegionInfo regionInfo = {};
    if (g_session.m_hotkeyScript->Call("queryRegionInfo", "d>ds", regionCount, &regionInfo.id, &regionInfo.name)) {
      if (!regionInfo.name) {
        break;
      }
      this->m_typeCombo->AddOption(regionInfo.name, 0);
    }
  }

  g_session.m_hotkeyScript->HasTable("map_list");
  for (int region = 2; region < regionCount; ++region) {
    MapInfo mapInfo = {};
    std::vector<std::pair<std::string, int>> maps;
    for (int index = 1;; ++index) {
      if (g_session.m_hotkeyScript->Call("queryMapInfo", "dd>ddsddd", region, index, &mapInfo.id, &mapInfo.level, &mapInfo.name, &mapInfo.red, &mapInfo.green, &mapInfo.blue)) {
        if (!mapInfo.name) {
          break;
        }
        MapOption option;
        option.name = mapInfo.name;
        option.color = (static_cast<unsigned char>(mapInfo.blue) << 16) | (static_cast<unsigned char>(mapInfo.green) << 8) | static_cast<unsigned char>(mapInfo.red);
        maps.push_back(std::pair<std::string, int>(option.name, option.color));
      }
    }
    this->m_subTypes.insert(std::pair<int, std::vector<std::pair<std::string, int>>>(region, maps));
  }

  this->m_typeCombo->Select(0);
  this->SendMsg(this->m_typeCombo, UM_COMBOBOX_SELECTED, 0, 0, 0, 0);
}
