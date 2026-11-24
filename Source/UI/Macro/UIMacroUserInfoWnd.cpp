#include "UI/Macro/UIMacroUserInfoWnd.h"
#include "Contents/Actors/ActorPickNode.h"
#include "UI/Window/WindowId.h"
#include "UI/Window/UIEnums.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Map/MapInfoMgr.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/World/World.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Contents/World/World.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UICheckBox.h"
#include "UI/Buttons/UIRadioBtn.h"
#include "UI/Buttons/UITextButton.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Layout/SnapMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"

#include "Resource/MSGSTRINGID.h"
#include "Network/Packets/PacketId.h"
#include <algorithm>
#include <atlconv.h>
#include <cstdlib>
#include <cstring>
#include <timeapi.h>
#include <vector>

// OFFSET: 0x007EE980
// FUNCTION: fn_09831
MacroReporterEntry::MacroReporterEntry(std::string name, int id, short reportValue)
    : name(name), id(id), reportValue(reportValue) {}

// OFFSET: 0x007EEA00
// FUNCTION: fn_09832
UIMacroReporterWnd::UIMacroReporterWnd()
    : UIFrameWnd(0), m_scrollBar(nullptr), m_scrollable(false), scrollBarWidth(13), m_firstRow(0), m_visibleRows(10), activeCheckBox(nullptr),
      m_reportButton(nullptr), m_mode(0), m_edit(nullptr), reportCount(1), maxReportCount(5), m_state(nullptr), activeOnly(false) {
  this->m_tabs[0] = nullptr;
  this->m_tabs[1] = nullptr;
  this->m_tabs[2] = nullptr;
  this->m_tabs[3] = nullptr;
  this->m_namesValid = false;
  this->m_targetValid = false;
  this->m_nameCount = 0;
}

// OFFSET: 0x007EEBC0
// FUNCTION: fn_09833
UIMacroUserInfoWnd::UIMacroUserInfoWnd()
    : UIFrameWnd(0) {
}

// OFFSET: 0x007EF2C0
// FUNCTION: fn_09839
void UIMacroReporterWnd::AddPickedActor(ActorPickInfo* pick) {
  if (pick) {
    this->AddActorByAID(pick->accountId);
  }
}

// OFFSET: 0x007EF2E0
// FUNCTION: fn_09840
void UIMacroReporterWnd::AddActorByAID(int aid) {
  CGameMode* mode = g_modeMgr.GetGameMode();
  CGameActor* actor = mode->scene->GetGameActorByAID(aid);
  if (!actor) {
    return;
  }

  if (!g_session.IsPlayerJobId(actor->GetEffectiveJob(-1))) {
    return;
  }

  mode = g_modeMgr.GetGameMode();
  std::string name = mode->ActorName(aid);

  for (auto it = this->m_entries.begin(); it != this->m_entries.end(); ++it) {
    if (it->id == aid) {
      goto found;
    }
  }

  this->m_entries.push_back(MacroReporterEntry(name, aid, static_cast<short>(actor->GetEffectiveJob(-1))));

found:
  this->AddActorRow(aid, name);
  this->UpdateScrollBar();
}

// OFFSET: 0x007EF430
// FUNCTION: fn_09841
void UIMacroReporterWnd::AddActorRow(int aid, std::string name) {
  for (const auto& row : this->m_rows) {
    if (row.second->geometry.id == aid) {
      return;
    }
  }

  auto* btn1 = new UIBitmapButton();
  btn1->SetBitmap("유저인터페이스\\basic_interface\\que_close_on.bmp", 0, 0);
  btn1->SetBitmap("유저인터페이스\\basic_interface\\que_close_off.bmp", 1, 0);
  btn1->SetBitmap("유저인터페이스\\basic_interface\\que_close_off.bmp", 2, 0);
  btn1->SetSize(btn1->BitmapWidth(), btn1->BitmapHeight());
  btn1->SetId(ID_DELETE);
  btn1->m_argument = aid;
  btn1->Enable(0);
  this->AddChild(btn1);

  std::string truncated = this->EllipsizeText(0x8E, name.c_str(), 0, 13, 0);
  auto* btn2 = new UITextButton();
  btn2->Configure("", 0, 0, 13, 0, 1);
  btn2->m_hoverColor = 0x2D2D2D;
  btn2->SetLabel(truncated);
  btn2->ResizeToText();
  btn2->SetTooltip(name.c_str());
  btn2->SetId(aid);
  btn2->Enable(0);
  this->AddChild(btn2);

  this->m_rows.push_back(std::make_pair(btn1, btn2));
}

// OFFSET: 0x007EF680
// FUNCTION: fn_09842
std::string UIMacroReporterWnd::EllipsizeText(int maxWidth, const char* text, int font, int height, unsigned char bold) {
  std::string source(text);
  const int width = this->TextWidth(text, 0, font, height, bold, 0);
  this->TextWidth("...", 0, font, height, bold, 0);
  if (maxWidth >= width) {
    return text;
  }

  std::string fitted(text);
  std::wstring wide(ATL::CA2WEX<128>(text, CP_THREAD_ACP));

  for (;;) {
    if (wide.size() <= 1) {
      fitted = std::string(ATL::CW2AEX<128>(wide.c_str(), CP_THREAD_ACP)) + "...";
      break;
    }

    if (wide.size() >= 2) {
      const std::size_t size = wide.size();
      wide.resize((wide[size - 2] & 0x80) != 0 && (wide[size - 1] & 0x80) != 0 ? size - 2 : size - 1);
    }

    fitted = std::string(ATL::CW2AEX<128>(wide.c_str(), CP_THREAD_ACP)) + "...";

    if (maxWidth >= this->TextWidth(fitted.c_str(), 0, font, height, bold, 0)) {
      break;
    }
  }

  return fitted;
}

// OFFSET: 0x007F0480
// FUNCTION: fn_09845
bool UIMacroUserInfoWnd::IsTransmitMouseInput() {
  return true;
}

// OFFSET: 0x007F0EE0
// FUNCTION: fn_09847
void UIMacroUserInfoWnd::OnCreate(int, int) {
  this->AddCloseButton("", 0);
}

// OFFSET: 0x007F0EF0
// FUNCTION: fn_09848
void UIMacroReporterWnd::Paint() {
  if (this->m_mode == 0 || this->m_mode == 1) {
    if (this->m_entries.size() == 0) {
      UIButton* button = this->m_reportButton;
      button->SetDisabled(1);
      button->Invalidate();
    } else {
      UIButton* button = this->m_reportButton;
      button->SetDisabled(0);
      button->Invalidate();
    }
  } else if (this->m_mode == 2 || this->m_mode == 3) {
    if (this->m_namesValid == 1 && this->m_targetValid == 1) {
      UIButton* button = this->m_reportButton;
      button->SetDisabled(0);
      button->Invalidate();
    } else {
      UIButton* button = this->m_reportButton;
      button->SetDisabled(1);
      button->Invalidate();
    }
  }
  this->DrawFilledRoundedOutline(0, 0, this->geometry.width, this->geometry.height, 0xFFFFFFFF);
  this->DrawTitleBar(true, "", 0);
  this->DrawButtonBar(1, 0, 1);

  if (this->m_mode != 0 && this->m_mode != 1) {
    return;
  }

  for (int row = 0; row < this->m_visibleRows; ++row) {
    this->FillRegion(14, row * 22 + 56, 214, 18, 0xE6E6E6);
  }

  for (unsigned int index = 0; index < this->m_rows.size(); ++index) {
    this->m_rows[index].first->Enable(0);
    this->m_rows[index].second->Enable(0);
  }

  int row = 0;
  for (unsigned int index = this->m_firstRow; static_cast<int>(index) < this->m_firstRow + this->m_visibleRows; ++index, ++row) {
    if (this->m_rows.size() <= index) {
      return;
    }

    const int y = row * 22 + 57;
    std::pair<UIBitmapButton*, UITextButton*>& buttons = this->m_rows[index];
    buttons.first->Enable(1);
    buttons.first->Move(15, y + 2);
    buttons.second->Enable(1);
    buttons.second->Move(36, y);
  }
}

// OFFSET: 0x007F10F0
// FUNCTION: fn_09849
void UIMacroUserInfoWnd::Paint() {
  this->ClearSurface(0xFAFAFA);
  this->DrawTextAt(5, 5, this->m_name.c_str(), 0, 0, 12, 0, 1, 0);
  this->DrawTextAt(5, 20, this->m_message.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x007F1160
// FUNCTION: fn_09850
void UIMacroReporterWnd::OnLBtnDown(int x, int y) {
  if (this->activeOnly == 1 && y > 17) {
    return;
  }

  if (this->geometry.id != g_windowMgr.lastWindowId) {
    g_windowMgr.BringToFront(this->geometry.id);
    g_windowMgr.lastWindowId = this->geometry.id;
  }

  g_windowMgr.SetCapture(this);
  int screenX = x;
  int screenY = y;
  this->ToScreen(screenX, screenY);
  this->m_dragX = screenX;
  this->m_dragY = screenY;
  this->m_originX = this->geometry.x;
  this->m_originY = this->geometry.y;
  g_snapMgr.CaptureOrigins(this);
}

// OFFSET: 0x007F11F0
// FUNCTION: fn_09851
void UIMacroReporterWnd::OnLBtnUp(int, int) {
  if (this != g_windowMgr.Capture()) {
    return;
  }

  g_windowMgr.lastWindowId = -1;
  g_windowMgr.ReleaseCapture();
  g_snapMgr.Refresh(this);
}

// OFFSET: 0x007F1230
// FUNCTION: fn_09852
void UIMacroReporterWnd::OnProcess() {
  for (auto it = this->m_removedRows.begin(); it != this->m_removedRows.end();) {
    if (!(*it)->Disabled()) {
      ++it;
    } else {
      this->RemoveChild(*it, 1);
      it = this->m_removedRows.erase(it);
    }
  }

  this->Invalidate();
}

// OFFSET: 0x007F12A0
// FUNCTION: fn_09853
void UIMacroReporterWnd::OnWheel(int delta) {
  if (this->m_scrollable == 1) {
    if (delta < 0) {
      this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, 3, 0, 0, 0);
    } else {
      this->SendMsg(this->m_scrollBar, UM_SCROLLVERT, -3, 0, 0, 0);
    }
  }
}

// OFFSET: 0x007F12E0
// FUNCTION: fn_09854
void UIMacroReporterWnd::UpdateScrollBar() {
  const int count = static_cast<int>(this->m_rows.size());
  if (count == 0 || !this->m_scrollBar) {
    return;
  }

  int maximum = count - this->m_visibleRows;
  if (maximum < 0) {
    maximum = 0;
  }
  this->m_scrollable = maximum > 0;
  if (maximum > 0) {
    this->m_scrollBar->SetRange(maximum, this->m_visibleRows, static_cast<int>(this->m_rows.size()));
    if (this->m_firstRow < 0) {
      this->m_firstRow = 0;
    } else if (this->m_firstRow > maximum) {
      this->m_firstRow = maximum;
    }
    this->m_scrollBar->SetPosition(this->m_firstRow);
  } else {
    this->m_firstRow = 0;
    this->m_scrollBar->SetRange(0, this->m_visibleRows, static_cast<int>(this->m_rows.size()));
  }

  this->m_scrollBar->Invalidate();
  this->Invalidate();
}

// OFFSET: 0x007EFE30
// FUNCTION: fn_09843
bool UIMacroReporterWnd::ValidateNames(unsigned char notify) {
  this->m_nameCount = 0;
  bool invalid = false;

  for (int i = 0; i < this->m_visibleRows; ++i) {
    std::string name = this->m_names[i];
    if (name.empty()) {
      continue;
    }

    name += ".rsw";
    const char* displayName = g_session.GetMapDisplayName(name.c_str());
    const std::string nowhereMapName = LookupMessage(MSI_NOWHERE);
    if (nowhereMapName.compare(displayName) == 0) {
      if (notify == 1) {
        g_windowMgr.SendNotice(
          1,
          reinterpret_cast<std::intptr_t>(FormatOwnedString("%d번 %s", i + 1, LookupMessage(MSI_MACRO_REPORTER_UNKNWON_MAP)).c_str()),
          0xFF,
          0,
          0,
          0
        );
      }
      invalid = true;
    } else {
      ++this->m_nameCount;
    }
  }

  if (this->m_nameCount > 0 && !invalid) {
    return true;
  }

  return false;
}

// OFFSET: 0x007F01A0
// FUNCTION: fn_09844
bool UIMacroReporterWnd::ValidateTarget(unsigned char notify) {
  std::string text = this->m_target;
  if (text.empty() || text.size() - 4 > 4) {
    if (notify == 1) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s", LookupMessage(MSI_MACRO_REPORTER_WRONG_NUMBER)).c_str()), 0xFF, 0, 0, 0);
    }
    return false;
  }

  const int value = std::atoi(text.c_str());
  char buffer[16] = {};
  if (text.size() != 0) {
    std::memset(buffer, '0', text.size());
  }

  if (value == 0 && text.compare(buffer) != 0) {
    if (notify == 1) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s", LookupMessage(MSI_MACRO_REPORTER_WRONG_NUMBER)).c_str()), 0xFF, 0, 0, 0);
    }
    return false;
  }

  text = "1" + this->m_target;
  std::memset(buffer, 0, sizeof(buffer));
  int scale = 1;
  for (std::size_t index = 1; index < text.size(); ++index) {
    scale *= 10;
  }
  _itoa(scale + value, buffer, 10);

  if (text.compare(buffer) != 0) {
    if (notify == 1) {
      g_windowMgr.SendNotice(UIM_PUSHINTOCHATHISTORY, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s", LookupMessage(MSI_MACRO_REPORTER_WRONG_NUMBER)).c_str()), 0xFF, 0, 0, 0);
    }
    return false;
  }

  return true;
}

// OFFSET: 0x007F25C0
// FUNCTION: fn_09857
int UIMacroUserInfoWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message == UM_COMMAND && argument1 == ID_CLOSE) {
    g_windowMgr.CloseWindow(WID_MACROUSERINFOWND);
  }
  return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007F26C0
// FUNCTION: fn_09858
void UIMacroReporterWnd::StoreInfo() {
  if (this->m_state) {
    this->m_state->width = this->geometry.width;
    this->m_state->height = this->geometry.height;
    this->m_state->x = this->geometry.x;
    this->m_state->y = this->geometry.y;
    this->m_state->activeOnly = this->activeOnly;
    this->m_state->entries.clear();
    this->m_state->entries.resize(this->m_entries.size());
    for (unsigned int index = 0; index < this->m_entries.size(); ++index) {
      this->m_state->entries[index].name = this->m_entries[index].name;
      this->m_state->entries[index].id = this->m_entries[index].id;
      this->m_state->entries[index].reportValue = this->m_entries[index].reportValue;
    }
  }
}

// OFFSET: 0x007F13C0
// FUNCTION: fn_09855
void UIMacroReporterWnd::SendMacroUserInfoPacket(std::uint16_t first, std::uint16_t second, std::uint8_t third) {
  struct Packet {
    std::int16_t id;
    std::uint16_t first;
    std::uint16_t second;
    std::uint8_t third;
  } packet{ID_CZ_REQ_PLAYER_AID_IN_RANGE, first, second, third};

  CRagConnection* connection = &GetRagConnection();
  const int length = GetRagConnection().GetPacketSize(packet.id);
  connection->Send(length, &packet);
}

// OFFSET: 0x007F0490
// FUNCTION: fn_09846
void UIMacroReporterWnd::OnCreate(int, int) {
  this->AddBaseButton();
  this->AddCloseButton("", 0);

  this->m_scrollBar = new UIScrollBar();
  this->m_scrollBar->Configure(186, 56, 1, 216);
  this->AddChild(this->m_scrollBar);

  this->activeCheckBox = new UICheckBox();
  this->activeCheckBox->SetSize(42, 20);
  this->activeCheckBox->SetBitmaps("유저인터페이스\\btn_q_active_a.bmp", "유저인터페이스\\btn_q_active.bmp");
  this->activeCheckBox->Move(202, 27);
  this->activeCheckBox->SetChecked(0);
  this->AddChild(this->activeCheckBox);

  this->m_reportButton = new UIBitmapButton();
  this->m_reportButton->SetBitmap("유저인터페이스\\btn_ok.bmp", 0, 0);
  this->m_reportButton->SetBitmap("유저인터페이스\\btn_ok_a.bmp", 1, 0);
  this->m_reportButton->SetBitmap("유저인터페이스\\btn_ok_b.bmp", 2, 0);
  this->m_reportButton->SetBitmap("유저인터페이스\\btn_ok_c.bmp", 3, 0);
  this->m_reportButton->SetSize(this->m_reportButton->BitmapWidth(), this->m_reportButton->BitmapHeight());
  this->m_reportButton->Move(202, 285);
  this->m_reportButton->SetId(ID_SEND);
  this->AddChild(this->m_reportButton);

  std::string labels[4] = {LookupMessage(MSI_MACRO_REPORTER_SELECT_MODE_CHAR), LookupMessage(MSI_MACRO_REPORTER_SELECT_MODE_CELL), LookupMessage(MSI_MACRO_REPORTER_SELECT_MODE_MAP), LookupMessage(MSI_MACRO_REPORTER_SELECT_MODE_QUIZ)};
  std::string tooltips[4] = {LookupMessage(MSI_MACRO_REPORTER_TOOLTIP_CHAR), LookupMessage(MSI_MACRO_REPORTER_TOOLTIP_CELL), LookupMessage(MSI_MACRO_REPORTER_TOOLTIP_MAP), LookupMessage(MSI_MACRO_REPORTER_TOOLTIP_QUIZ)};

  int index = 0;
  for (; index < 2; ++index) {
    this->m_tabs[index] = new UIRadioBtn();
    this->m_tabs[index]->SetSize(50, 12);
    this->m_tabs[index]->Move(14 + index * 55, 31);
    this->m_tabs[index]->SetText(labels[index].c_str());
    this->m_tabs[index]->SetId(ID_RADIOBTN);
    this->m_tabs[index]->value = index;
    this->m_tabs[index]->SendMsg(this, UM_SETSTATE, index == this->m_mode, 0, 0, 0);
    this->m_tabs[index]->SetTooltip(tooltips[index].c_str());
    this->AddChild(this->m_tabs[index]);
  }
  for (; index < 4; ++index) {
    this->m_tabs[index] = new UIRadioBtn();
    this->m_tabs[index]->SetSize(50, 12);
    this->m_tabs[index]->Move(70 + index * 30, 31);
    this->m_tabs[index]->SetText(labels[index].c_str());
    this->m_tabs[index]->SetId(ID_RADIOBTN);
    this->m_tabs[index]->value = index;
    this->m_tabs[index]->SendMsg(this, UM_SETSTATE, index == this->m_mode, 0, 0, 0);
    this->m_tabs[index]->SetTooltip(tooltips[index].c_str());
    this->AddChild(this->m_tabs[index]);
  }

  for (int row = 0; row < this->m_visibleRows; ++row) {
    this->m_fields[row] = new UIEditCtrl();
    this->m_fields[row]->SetSize(214, 18);
    this->m_fields[row]->Move(14, row * 22 + 56);
    this->m_fields[row]->flat = 0;
    this->m_fields[row]->Border(125, 125, 125);
    this->m_fields[row]->preserveInput = 1;
    this->m_fields[row]->invalidatesParent = 1;
    UIEditCtrl* field = this->m_fields[row];
    field->disabled = 1;
    field->Invalidate();
    this->m_fields[row]->SetTooltip(LookupMessage(MSI_MACRO_REPORTER_TOOLTIP_MAP_QUIZ_INPUT));
    this->AddChild(this->m_fields[row]);
    this->m_names[row] = "";
  }
  this->m_target = "";

  this->m_edit = new UIEditCtrl();
  this->m_edit->SetSize(15, 14);
  this->m_edit->Move(this->TextWidth(labels[1].c_str(), 0, 0, 12, 0, 0) + 83, 30);
  this->m_edit->flat = 0;
  this->m_edit->Border(125, 125, 125);
  this->m_edit->maximum = 1;
  this->m_edit->preserveInput = 1;
  this->m_edit->invalidatesParent = 1;
  this->m_edit->SetText(IntToString(this->reportCount, 0).c_str());
  this->m_edit->SetTooltip(LookupMessage(MSI_MACRO_REPORTER_TOOLTIP_RANGE));
  this->AddChild(this->m_edit);
  if (this->m_mode != 1) {
    UIEditCtrl* edit = this->m_edit;
    edit->disabled = 1;
    edit->Invalidate();
  }

  this->m_entries.clear();
}

namespace {
#pragma pack(push, 1)
  struct PACKET_CZ_MACRO_REPORT_HEADER {
    /* 0000 */ std::uint16_t packetType;
    /* 0002 */ std::uint16_t packetLength;
    /* 0004 */ char target[16];
    /* 0014 */ std::uint8_t type;
    /* 0015 */ std::uint8_t count;
  };

  struct PACKET_CZ_MACRO_REPORT_ACCOUNT {
    /* 0000 */ short packetType;
    /* 0002 */ std::uint32_t accountId;
  };
#pragma pack(pop)

  struct MacroReportScratch {
    /* 0000 */ char name[16];
    /* 0010 */ PACKET_CZ_MACRO_REPORT_HEADER header;
  };
}

// OFFSET: 0x007F1420
// FUNCTION: fn_09856
int UIMacroReporterWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  MacroReportScratch report;
  switch (message) {
  case UM_SETINFO: {
    this->m_state = reinterpret_cast<MacroReporterState*>(argument1);
    if (!this->m_state->IsValid()) {
      this->m_state->Reset();
    }
    this->Resize(this->m_state->width, this->m_state->height);
    this->Move(this->m_state->x, this->m_state->y);
    this->activeOnly = this->m_state->activeOnly;
    this->activeCheckBox->SetChecked(this->activeOnly);
    this->m_entries.clear();
    for (unsigned int index = 0; index < this->m_state->entries.size(); ++index) {
      const MacroReporterEntry& entry = this->m_state->entries[index];
      this->m_entries.push_back(MacroReporterEntry(entry.name, entry.id, entry.reportValue));
      this->AddActorRow(entry.id, entry.name);
    }
    this->UpdateScrollBar();
    break;
  }

  case UM_COMMAND:
    switch (argument1) {
    case ID_CLOSE:
      g_windowMgr.CloseWindow(WID_MACROREPORTERWND);
      break;

    case ID_DELETE: {
      for (auto it = this->m_entries.begin(); it != this->m_entries.end(); ++it) {
        if (argument2 == it->id) {
          this->m_entries.erase(it);
          break;
        }
      }
      for (auto it = this->m_rows.begin(); it != this->m_rows.end(); ++it) {
        if (sender == it->first) {
          it->first->SetDisabled(1);
          it->first->Invalidate();
          UIButton* first = it->first;
          this->m_removedRows.push_back(first);
          it->second->SetDisabled(1);
          it->second->Invalidate();
          UIButton* second = it->second;
          this->m_removedRows.push_back(second);
          this->m_rows.erase(it);
          break;
        }
      }
      this->UpdateScrollBar();
      break;
    }

    case ID_CHECK:
      if (this->activeCheckBox != sender) {
        break;
      }
      this->activeOnly = argument2 != 0;
      if (this->m_mode == 2 || this->m_mode == 3) {
        this->m_namesValid = this->ValidateNames(1);
        this->m_targetValid = this->ValidateTarget(1);
      } else if (this->m_mode == 0 || this->m_mode == 1) {
        if (argument2) {
          this->geometry.targetAlpha = 128;
          this->geometry.alphaStart = timeGetTime();
        } else {
          this->geometry.targetAlpha = 255;
          this->geometry.alphaStart = timeGetTime();
        }
      }
      break;

    case ID_SEND:
      if (this->m_mode == 2 || this->m_mode == 3) {
        this->m_namesValid = this->ValidateNames(1);
        this->m_targetValid = this->ValidateTarget(1);
        if (this->m_namesValid == true && this->m_targetValid == true) {
          report.header.packetType = ID_CZ_MACRO_REPORT_USER;
          strncpy_s(report.header.target, sizeof(report.header.target), this->m_target.c_str(), _TRUNCATE);
          report.header.count = static_cast<std::uint8_t>(this->m_nameCount);
          const int length = this->m_nameCount * 16 + sizeof(report.header);
          report.header.type = 6;
          char buffer[2048];
          std::memset(buffer + sizeof(report.header), 0, sizeof(buffer) - sizeof(report.header));
          if (length > sizeof(buffer)) {
            break;
          }
          report.header.packetLength = static_cast<std::uint16_t>(length);
          std::memcpy(buffer, &report.header, sizeof(report.header));
          for (int index = 0; index < this->m_nameCount; ++index) {
            if (!this->m_names[index].empty()) {
              strncpy_s(report.name, sizeof(report.name), (this->m_names[index] + ".gat").c_str(), _TRUNCATE);
              memcpy_s(buffer + sizeof(report.header) + index * 16, 16, report.name, sizeof(report.name));
            }
          }
          GetRagConnection().Send(length, buffer);
        }
      } else {
        std::vector<unsigned int> accounts;
        for (auto it = this->m_entries.begin(); it != this->m_entries.end(); ++it) {
          if (std::find(accounts.begin(), accounts.end(), it->id) == accounts.end()) {
            accounts.push_back(it->id);
          }
        }
        const int result = g_windowMgr
                             .Confirm(FormatOwnedString(LookupMessage(MSI_MACRO_REPORTER), accounts.size()).c_str(), 2, false, false, 0, "Message", 280, 120, 0);
        if (result == ID_NO) {
          break;
        }
        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
          PACKET_CZ_MACRO_REPORT_ACCOUNT packet;
          packet.packetType = ID_CZ_REQ_APPLY_MACRO_DETECTOR;
          packet.accountId = *it;
          GetRagConnection().Send(GetRagConnection().GetPacketSize(packet.packetType), &packet);
        }
        this->m_entries.clear();
        for (auto it = this->m_rows.begin(); it != this->m_rows.end(); ++it) {
          it->first->SetDisabled(1);
          it->first->Invalidate();
          UIButton* first = it->first;
          this->m_removedRows.emplace_back(first);
          it->second->SetDisabled(1);
          it->second->Invalidate();
          UIButton* second = it->second;
          this->m_removedRows.emplace_back(second);
        }
        this->m_rows.clear();
      }
      break;

    case ID_RADIOBTN: {
      if (this->activeOnly == true) {
        if (this->m_mode == 0 || this->m_mode == 1) {
          if (argument2 == 2 || argument2 == 3) {
            this->activeCheckBox->SetChecked(0);
          }
        } else if (this->m_mode == 2 || this->m_mode == 3) {
          if (argument2 == 0 || argument2 == 1) {
            this->activeCheckBox->SetChecked(0);
          }
        }
      }

      if (this->m_mode == 2) {
        this->ValidateNames(0);
      } else if (this->m_mode == 3) {
        this->ValidateTarget(0);
      }

      this->m_mode = argument2;
      for (int index = 0; index < 4; ++index) {
        this->m_tabs[index]->SendMsg(this, UM_SETSTATE, 0, 0, 0, 0);
      }
      sender->SendMsg(this, UM_SETSTATE, 1, 0, 0, 0);

      if (sender == this->m_tabs[1]) {
        UIEditCtrl* edit = this->m_edit;
        edit->disabled = 0;
        edit->Invalidate();
      } else {
        UIEditCtrl* edit = this->m_edit;
        edit->disabled = 1;
        edit->Invalidate();
      }

      if (sender == this->m_tabs[0] || sender == this->m_tabs[1]) {
        g_windowMgr.SetFocus(nullptr);
        for (int index = 0; index < this->m_visibleRows; ++index) {
          this->m_fields[index]->disabled = 1;
          this->m_fields[index]->Invalidate();
          this->m_fields[index]->SetText("");
        }
        for (auto it = this->m_rows.begin(); it != this->m_rows.end(); ++it) {
          it->first->SetDisabled(0);
          it->first->Invalidate();
          it->second->SetDisabled(0);
          it->second->Invalidate();
          it->first->Enable(1);
          it->second->Enable(1);
        }
      } else {
        for (auto it = this->m_rows.begin(); it != this->m_rows.end(); ++it) {
          it->first->SetDisabled(1);
          it->first->Invalidate();
          it->second->SetDisabled(1);
          it->second->Invalidate();
          it->first->Enable(0);
          it->second->Enable(0);
        }
        for (int index = 0; index < this->m_visibleRows; ++index) {
          this->m_fields[index]->disabled = 0;
          this->m_fields[index]->Invalidate();
          if (sender == this->m_tabs[2]) {
            this->m_fields[index]->SetText(this->m_names[index].c_str());
          } else if (sender == this->m_tabs[3]) {
            if (index == 0) {
              this->m_fields[index]->SetText(this->m_target.c_str());
            } else {
              this->m_fields[index]->SetText("");
            }
          }
        }
      }
      break;
    }
    }
    break;

  case UM_DEFPUSH:
    if (this->m_mode == 2) {
      if (sender) {
        const int index = (sender->geometry.y - 56) / 22;
        this->m_names[index] = static_cast<UIEditCtrl*>(sender)->Text();
      }
    } else if (this->m_mode == 3) {
      if (sender && sender == this->m_fields[0]) {
        this->m_target = this->m_fields[0]->Text();
      }
    } else if (sender == this->m_edit) {
      std::string text = this->m_edit->Text();
      if (text.size() > 1) {
        this->m_edit->SetText(IntToString(this->reportCount, 0).c_str());
      } else if (text.c_str()[0] >= '0' && text.c_str()[0] <= '9') {
        this->reportCount = std::atoi(text.c_str());
        if (this->reportCount > this->maxReportCount) {
          this->reportCount = this->maxReportCount;
          this->m_edit->SetText(IntToString(this->maxReportCount, 0).c_str());
        }
      } else {
        this->m_edit->SetText(IntToString(this->reportCount, 0).c_str());
      }
    }
    break;

  case UM_TEXT_LBTN_UP: {
    auto entry = this->m_entries.begin();
    for (; entry != this->m_entries.end(); ++entry) {
      if (argument1 == entry->id) {
        break;
      }
    }
    if (entry == this->m_entries.end()) {
      break;
    }

    unsigned int row = 0;
    for (; row < this->m_rows.size(); ++row) {
      if (sender == this->m_rows[row].second) {
        break;
      }
    }

    if (!g_windowMgr.CloseWindow(WID_MACROUSERINFOWND)) {
      UIMacroUserInfoWnd* info = static_cast<UIMacroUserInfoWnd*>(g_windowMgr.EnsureWindow(WID_MACROUSERINFOWND));
      info->Move(this->geometry.x + 15, this->geometry.y + row * 22 + 78);
      const unsigned short job = entry->reportValue;
      std::string name = entry->name;
      info->m_name = name;
      info->m_message = g_session.GetJobName(job, -1);
      info->Invalidate();
    }
    break;
  }

  case UM_SCROLLVERT:
  case UM_SCROLLVERTPAGEUP:
  case UM_SCROLLVERTPAGEDOWN: {
    const int previous = this->m_firstRow;
    if (message == UM_SCROLLVERTPAGEUP) {
      this->m_firstRow = previous - this->m_visibleRows + 1;
    } else if (message == UM_SCROLLVERTPAGEDOWN) {
      this->m_firstRow = previous + (this->m_visibleRows - 1);
    } else {
      this->m_firstRow = previous + argument1;
    }
    if (previous != this->m_firstRow) {
      this->UpdateScrollBar();
    }
    return this->m_firstRow - previous;
  }
  }

  return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
