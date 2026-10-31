#include "UI/UIMacroUserInfoWnd.h"

#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/MapInfoMgr.h"
#include "Contents/MessageTable.h"
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
#include "UI/Layout/WindowSnapping.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/FrameFocus.h"
#include "UI/Window/InputFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Window/WindowDialogs.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <atlconv.h>
#include <cstdlib>
#include <cstring>
#include <timeapi.h>
#include <vector>

// OFFSET: 0x007EE980
// FUNCTION: fn_09831
MacroReporterEntry::MacroReporterEntry(std::string name, int id, short unknown_001C)
    : name(name), id(id), unknown_001C(unknown_001C) {}

// OFFSET: 0x007EEA00
// FUNCTION: fn_09832
UIMacroReporterWnd::UIMacroReporterWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_scrollBar(nullptr), m_scrollable(false), unknown_00C8(13), m_firstRow(0), m_visibleRows(10), unknown_00D4(nullptr),
      m_reportButton(nullptr), m_mode(0), m_edit(nullptr), unknown_00F4(1), unknown_00F8(5), m_state(nullptr), unknown_010C(false), m_controls(&controls) {
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
UIMacroUserInfoWnd::UIMacroUserInfoWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls)
    : UIFrameWnd(windows, frames, 0), m_controls(&controls) {
}

// OFFSET: 0x007EF2C0
// FUNCTION: fn_09839
void UIMacroReporterWnd::sub_7EF2C0(MacroReporterEntry* entry) {
  if (entry) {
    this->sub_7EF2E0(entry->id);
  }
}

// OFFSET: 0x007EF2E0
// FUNCTION: fn_09840
void UIMacroReporterWnd::sub_7EF2E0(int aid) {
  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  CGameActor* actor = static_cast<CGameActor*>(mode->scene->GetGameActorByAID(aid));
  if (!actor) {
    return;
  }

  if (!g_session.IsPlayerJobId(actor->GetEffectiveJob(-1))) {
    return;
  }

  mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  std::string name = mode->ActorName(aid);

  for (auto it = this->m_entries.begin(); it != this->m_entries.end(); ++it) {
    if (it->id == aid) {
      goto found;
    }
  }

  this->m_entries.push_back(MacroReporterEntry(name, aid, static_cast<short>(actor->GetEffectiveJob(-1))));

found:
  this->sub_7EF430(aid, name);
  this->UpdateScrollBar();
}

struct AccessibleButton : public UIButton {
  using UIButton::m_argument;
};

struct AccessibleTextButton : public UITextButton {
  using UITextButton::m_hoverColor;
};

// OFFSET: 0x007EF430
// FUNCTION: fn_09841
void UIMacroReporterWnd::sub_7EF430(int aid, std::string name) {
  for (std::size_t i = 0; i < this->m_rows.size(); ++i) {
    if (this->m_rows[i].second->geometry.id == aid) {
      return;
    }
  }

  auto* btn1 = new UIBitmapButton();
  btn1->SetBitmap("유저인터페이스\\basic_interface\\que_close_on.bmp", 0, 0);
  btn1->SetBitmap("유저인터페이스\\basic_interface\\que_close_off.bmp", 1, 0);
  btn1->SetBitmap("유저인터페이스\\basic_interface\\que_close_off.bmp", 2, 0);
  btn1->SetSize(btn1->BitmapWidth(), btn1->BitmapHeight());
  btn1->SetId(211);
  reinterpret_cast<AccessibleButton*>(btn1)->m_argument = aid;
  btn1->Enable(0);
  this->AddChild(btn1);

  std::string truncated = this->sub_7EF680(0x8E, name.c_str(), 0, 13, 0);
  auto* btn2 = new UITextButton(*this->m_context, *this->m_controls);
  btn2->Configure("", 0, 0, 13, 0, 1);
  static_cast<AccessibleTextButton*>(btn2)->m_hoverColor = 0x2D2D2D;
  btn2->label = truncated;
  btn2->ResizeToText();
  btn2->SetTooltip(name.c_str());
  btn2->SetId(aid);
  btn2->Enable(0);
  this->AddChild(btn2);

  this->m_rows.push_back(std::make_pair(btn1, btn2));
}

// OFFSET: 0x007EF680
// FUNCTION: fn_09842
std::string UIMacroReporterWnd::sub_7EF680(int maxWidth, const char* text, int font, int height, unsigned char bold) {
  std::string source(text);
  const int width = this->TextWidth(this->m_controls->fonts, this->m_controls->locale, text, 0, font, height, bold, 0);
  this->TextWidth(this->m_controls->fonts, this->m_controls->locale, "...", 0, font, height, bold, 0);
  if (maxWidth >= width) {
    return text;
  }

  std::string fitted(text);
  std::wstring wide(ATL::CA2WEX<128>(text, CP_THREAD_ACP));
  while (wide.size() > 1) {
    if (wide.size() >= 2) {
      const std::size_t size = wide.size();
      const std::size_t cut = (wide[size - 2] & 0x80) != 0 && (wide[size - 1] & 0x80) != 0 ? 2 : 1;
      wide.resize(size - cut);
    }

    fitted = ATL::CW2AEX<128>(wide.c_str(), CP_THREAD_ACP);
    fitted += "...";
    if (this->TextWidth(this->m_controls->fonts, this->m_controls->locale, fitted.c_str(), 0, font, height, bold, 0) <= maxWidth) {
      return fitted;
    }
  }

  fitted = ATL::CW2AEX<128>(wide.c_str(), CP_THREAD_ACP);
  fitted += "...";
  return fitted;
}

// OFFSET: 0x007F0480
// FUNCTION: fn_09845
bool UIMacroUserInfoWnd::Slot08() {
  return true;
}

// OFFSET: 0x007F0EE0
// FUNCTION: fn_09847
void UIMacroUserInfoWnd::Slot15(int, int) {
  this->AddCloseButton(*this->m_controls, "", 0);
}

// OFFSET: 0x007F0EF0
// FUNCTION: fn_09848
void UIMacroReporterWnd::Paint() {
  if (this->m_mode == 0 || this->m_mode == 1) {
    this->m_reportButton->SetDisabled(this->m_entries.empty());
    this->m_reportButton->Invalidate();
  } else if (this->m_mode == 2 || this->m_mode == 3) {
    this->m_reportButton->SetDisabled(!(this->m_namesValid == 1 && this->m_targetValid == 1));
    this->m_reportButton->Invalidate();
  }

  DrawFilledRoundedBorder(*this, 0, 0, this->geometry.width, this->geometry.height, 0xFFFFFFFF);
  this->DrawTitleBar(this->m_controls->fonts, this->m_controls->locale, true, "", 0);
  this->DrawButtonBar(1, 0, 1);

  if (this->m_mode != 0 && this->m_mode != 1) {
    return;
  }

  for (int row = 0, y = 56; row < this->m_visibleRows; ++row, y += 22) {
    this->FillRegion(14, y, 214, 18, 0xE6E6E6);
  }

  for (unsigned int index = 0; index < this->m_rows.size(); ++index) {
    this->m_rows[index].first->Enable(0);
    this->m_rows[index].second->Enable(0);
  }

  int y = 57;
  for (unsigned int index = this->m_firstRow; static_cast<int>(index) < this->m_firstRow + this->m_visibleRows; ++index, y += 22) {
    if (index >= this->m_rows.size()) {
      return;
    }

    this->m_rows[index].first->Enable(1);
    this->m_rows[index].first->Move(15, y + 2);
    this->m_rows[index].second->Enable(1);
    this->m_rows[index].second->Move(36, y);
  }
}

// OFFSET: 0x007F10F0
// FUNCTION: fn_09849
void UIMacroUserInfoWnd::Paint() {
  this->ClearSurface(0xFAFAFA);
  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 5, 5, this->m_name.c_str(), 0, 0, 12, 0, 1, 0);
  this->DrawTextAt(this->m_controls->fonts, this->m_controls->locale, 5, 20, this->m_message.c_str(), 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x007F1160
// FUNCTION: fn_09850
void UIMacroReporterWnd::Slot25(int x, int y) {
  if (this->unknown_010C == 1 && y >= 18) {
    return;
  }

  if (g_dragWindowId != this->geometry.id) {
    g_windowMgr.BringToFront(this->geometry.id);
    g_dragWindowId = this->geometry.id;
  }

  g_windowMgr.SetCapture(this);
  this->ToScreen(x, y);
  this->m_dragX = x;
  this->m_dragY = y;
  this->m_originX = this->geometry.x;
  this->m_originY = this->geometry.y;
  this->m_frameContext->snapping.graph.CaptureOrigins(this);
}

// OFFSET: 0x007F11F0
// FUNCTION: fn_09851
void UIMacroReporterWnd::Slot31(int, int) {
  if (g_windowMgr.Capture() != this) {
    return;
  }

  g_dragWindowId = -1;
  g_windowMgr.ReleaseCapture();
  this->m_frameContext->snapping.graph.Refresh(this);
}

// OFFSET: 0x007F1230
// FUNCTION: fn_09852
void UIMacroReporterWnd::Slot19() {
  for (auto it = this->m_removedRows.begin(); it != this->m_removedRows.end();) {
    if ((*it)->Disabled()) {
      this->RemoveChild(*it, 1);
      it = this->m_removedRows.erase(it);
    } else {
      ++it;
    }
  }

  this->Invalidate();
}

// OFFSET: 0x007F12A0
// FUNCTION: fn_09853
void UIMacroReporterWnd::Slot35(int delta) {
  if (this->m_scrollable == 1) {
    this->SendMsg(this->m_scrollBar, 7, delta < 0 ? 3 : -3, 0, 0, 0);
  }
}

// OFFSET: 0x007F12E0
// FUNCTION: fn_09854
void UIMacroReporterWnd::UpdateScrollBar() {
  const int count = static_cast<int>(this->m_rows.size());
  if (count == 0 || !this->m_scrollBar) {
    return;
  }

  const int maximum = (std::max)(count - this->m_visibleRows, 0);
  this->m_scrollable = maximum > 0;
  if (maximum < 1) {
    this->m_firstRow = 0;
    this->m_scrollBar->SetRange(0, this->m_visibleRows, count);
  } else {
    this->m_scrollBar->SetRange(maximum, this->m_visibleRows, count);
    if (this->m_firstRow < 0) {
      this->m_firstRow = 0;
      this->m_scrollBar->SetPosition(0);
    } else {
      if (maximum < this->m_firstRow) {
        this->m_firstRow = maximum;
      }
      this->m_scrollBar->SetPosition(this->m_firstRow);
    }
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

    std::string displayName = g_mapInfoMgr->GetDisplayName(name + ".rsw");
    if (displayName == LookupMessage(187)) {
      if (notify == 1) {
        g_windowMgr.SendNotice(
          1,
          reinterpret_cast<std::intptr_t>(FormatOwnedString("%d번 %s", i + 1, LookupMessage(4010)).c_str()),
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
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s", LookupMessage(4011)).c_str()), 0xFF, 0, 0, 0);
    }
    return false;
  }

  const int value = std::atoi(text.c_str());
  char digits[16] = {};
  if (text.size() != 0) {
    std::memset(digits, '0', text.size());
  }

  if (value == 0 && text.compare(digits) != 0) {
    if (notify == 1) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s", LookupMessage(4011)).c_str()), 0xFF, 0, 0, 0);
    }
    return false;
  }

  text = "1" + this->m_target;
  char number[16] = {};
  int scale = 1;
  for (std::size_t index = 1; index < text.size(); ++index) {
    scale *= 10;
  }
  _itoa(scale + value, number, 10);

  if (text.compare(number) != 0) {
    if (notify == 1) {
      g_windowMgr.SendNotice(1, reinterpret_cast<std::intptr_t>(FormatOwnedString("%s", LookupMessage(4011)).c_str()), 0xFF, 0, 0, 0);
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
  if (message == 6 && argument1 == 201) {
    g_windowMgr.CloseWindow(287);
  }
  return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007F26C0
// FUNCTION: fn_09858
void UIMacroReporterWnd::Slot11() {
  if (this->m_state) {
    this->m_state->width = this->geometry.width;
    this->m_state->height = this->geometry.height;
    this->m_state->x = this->geometry.x;
    this->m_state->y = this->geometry.y;
    this->m_state->unknown_0011 = this->unknown_010C;
    this->m_state->entries.clear();
    this->m_state->entries.resize(this->m_entries.size());
    for (unsigned int index = 0; index < this->m_entries.size(); ++index) {
      this->m_state->entries[index] = this->m_entries[index];
    }
  }
}

// OFFSET: 0x007F13C0
// FUNCTION: fn_09855
void __stdcall SendMacroUserInfoPacket(std::uint16_t first, std::uint16_t second, std::uint8_t third) {
  struct Packet {
    std::uint16_t id;
    std::uint16_t first;
    std::uint16_t second;
    std::uint8_t third;
  } packet{0x0A6C, first, second, third};

  CRagConnection* connection = &GetRagConnection();
  const PacketLengths& lengths = reinterpret_cast<const PacketLengths&>(GetRagConnection());
  connection->Send(lengths.FixedLength(packet.id), &packet);
}

// OFFSET: 0x007F0490
// FUNCTION: fn_09846
void UIMacroReporterWnd::Slot15(int, int) {
  this->AddBaseButton(*this->m_controls);
  this->AddCloseButton(*this->m_controls, "", 0);

  this->m_scrollBar = new UIScrollBar(*this->m_context);
  this->m_scrollBar->Configure(186, 56, 1, 216);
  this->AddChild(this->m_scrollBar);

  UICheckBox* check = new UICheckBox(*this->m_context, *this->m_controls);
  this->unknown_00D4 = check;
  check->SetSize(42, 20);
  check->SetBitmaps("유저인터페이스\\btn_q_active_a.bmp", "유저인터페이스\\btn_q_active.bmp");
  check->Move(202, 27);
  check->SetChecked(0);
  this->AddChild(check);

  UIBitmapButton* report = new UIBitmapButton(*this->m_context, *this->m_controls);
  this->m_reportButton = report;
  report->SetBitmap("유저인터페이스\\btn_ok.bmp", 0, 0);
  report->SetBitmap("유저인터페이스\\btn_ok_a.bmp", 1, 0);
  report->SetBitmap("유저인터페이스\\btn_ok_b.bmp", 2, 0);
  report->SetBitmap("유저인터페이스\\btn_ok_c.bmp", 3, 0);
  report->SetSize(report->BitmapWidth(), report->BitmapHeight());
  report->Move(202, 285);
  report->SetId(345);
  this->AddChild(report);

  std::string labels[4] = {LookupMessage(2887), LookupMessage(2888), LookupMessage(4006), LookupMessage(4007)};
  std::string tooltips[4] = {LookupMessage(2889), LookupMessage(2890), LookupMessage(4008), LookupMessage(4009)};

  int index = 0;
  for (; index < 2; ++index) {
    UIRadioBtn* tab = new UIRadioBtn(*this->m_context, *this->m_controls);
    this->m_tabs[index] = tab;
    tab->SetSize(50, 12);
    tab->Move(14 + index * 55, 31);
    tab->SetText(labels[index].c_str());
    tab->SetId(215);
    tab->value = index;
    tab->SendMsg(this, 13, index == this->m_mode, 0, 0, 0);
    if (const char* tooltip = tooltips[index].c_str()) {
      tab->tooltip = tooltip;
    }
    this->AddChild(tab);
  }
  for (; index < 4; ++index) {
    UIRadioBtn* tab = new UIRadioBtn(*this->m_context, *this->m_controls);
    this->m_tabs[index] = tab;
    tab->SetSize(50, 12);
    tab->Move(70 + index * 30, 31);
    tab->SetText(labels[index].c_str());
    tab->SetId(215);
    tab->value = index;
    tab->SendMsg(this, 13, index == this->m_mode, 0, 0, 0);
    if (const char* tooltip = tooltips[index].c_str()) {
      tab->tooltip = tooltip;
    }
    this->AddChild(tab);
  }

  int y = 56;
  for (int row = 0; row < this->m_visibleRows; ++row) {
    UIEditCtrl* field = new UIEditCtrl();
    this->m_fields[row] = field;
    field->SetSize(214, 18);
    field->Move(14, y);
    field->flat = 0;
    field->Border(125, 125, 125);
    field->preserveInput = 1;
    field->invalidatesParent = 1;
    field->disabled = 1;
    field->Invalidate();
    if (const char* tooltip = LookupMessage(4012)) {
      field->tooltip = tooltip;
    }
    this->AddChild(field);
    this->m_names[row] = "";
    y += 22;
  }
  this->m_target = "";

  UIEditCtrl* target = new UIEditCtrl();
  this->m_edit = target;
  target->SetSize(15, 14);
  target->Move(this->TextWidth(this->m_controls->fonts, this->m_controls->locale, labels[1].c_str(), 0, 0, 12, 0, 0) + 83, 30);
  target->flat = 0;
  target->Border(125, 125, 125);
  target->maximum = 1;
  target->preserveInput = 1;
  target->invalidatesParent = 1;
  target->SetText(IntToString(this->unknown_00F4, 0).c_str());
  if (const char* tooltip = LookupMessage(2891)) {
    target->tooltip = tooltip;
  }
  this->AddChild(target);
  if (this->m_mode != 1) {
    target->disabled = 1;
    target->Invalidate();
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
}

// OFFSET: 0x007F1420
// FUNCTION: fn_09856
int UIMacroReporterWnd::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  switch (message) {
  case 0:
    if (this->m_mode == 2) {
      if (sender) {
        const int index = (sender->geometry.y - 56) / 22;
        this->m_names[index] = static_cast<UIEditCtrl*>(sender)->Text();
      }
    } else if (this->m_mode == 3) {
      if (sender && sender == this->m_fields[0]) {
        this->m_target = static_cast<UIEditCtrl*>(this->m_fields[0])->Text();
      }
    } else if (sender == this->m_edit) {
      UIEditCtrl* edit = static_cast<UIEditCtrl*>(this->m_edit);
      std::string text = edit->Text();
      if (text.size() > 1) {
        edit->SetText(IntToString(this->unknown_00F4, 0).c_str());
      } else if (text.c_str()[0] >= '0' && text.c_str()[0] <= '9') {
        this->unknown_00F4 = std::atoi(text.c_str());
        if (this->unknown_00F4 > this->unknown_00F8) {
          this->unknown_00F4 = this->unknown_00F8;
          edit->SetText(IntToString(this->unknown_00F8, 0).c_str());
        }
      } else {
        edit->SetText(IntToString(this->unknown_00F4, 0).c_str());
      }
    }
    break;

  case 6:
    switch (argument1) {
    case 201:
      g_windowMgr.CloseWindow(286);
      break;

    case 211: {
      for (auto it = this->m_entries.begin(); it != this->m_entries.end(); ++it) {
        if (argument2 == it->id) {
          for (auto next = it + 1; next != this->m_entries.end(); ++next, ++it) {
            it->name.swap(next->name);
            it->id = next->id;
            it->unknown_001C = next->unknown_001C;
          }
          this->m_entries.pop_back();
          break;
        }
      }
      for (auto it = this->m_rows.begin(); it != this->m_rows.end(); ++it) {
        if (sender == it->first) {
          it->first->SetDisabled(1);
          it->first->Invalidate();
          this->m_removedRows.push_back(it->first);
          it->second->SetDisabled(1);
          it->second->Invalidate();
          this->m_removedRows.push_back(it->second);
          this->m_rows.erase(it);
          break;
        }
      }
      this->UpdateScrollBar();
      break;
    }

    case 213:
      if (this->unknown_00D4 != sender) {
        break;
      }
      this->unknown_010C = argument2 != 0;
      switch (this->m_mode) {
      case 0:
      case 1:
        if (argument2) {
          this->geometry.targetAlpha = 128;
        } else {
          this->geometry.targetAlpha = 255;
        }
        this->geometry.alphaStart = timeGetTime();
        break;
      case 2:
      case 3:
        this->m_namesValid = this->ValidateNames(1);
        this->m_targetValid = this->ValidateTarget(1);
        break;
      }
      break;

    case 215: {
      if (this->unknown_010C == true) {
        switch (this->m_mode) {
        case 0:
        case 1:
          if (argument2 == 2 || argument2 == 3) {
            static_cast<UICheckBox*>(this->unknown_00D4)->SetChecked(0);
          }
          break;
        case 2:
        case 3:
          if (argument2 == 0 || argument2 == 1) {
            static_cast<UICheckBox*>(this->unknown_00D4)->SetChecked(0);
          }
          break;
        }
      }

      if (this->m_mode == 2) {
        this->ValidateNames(0);
      } else if (this->m_mode == 3) {
        this->ValidateTarget(0);
      }

      this->m_mode = argument2;
      for (int index = 0; index < 4; ++index) {
        this->m_tabs[index]->SendMsg(this, 13, 0, 0, 0, 0);
      }
      sender->SendMsg(this, 13, 1, 0, 0, 0);

      UIEditCtrl* edit = static_cast<UIEditCtrl*>(this->m_edit);
      if (sender == this->m_tabs[1]) {
        edit->disabled = 0;
      } else {
        edit->disabled = 1;
      }
      edit->Invalidate();

      if (sender == this->m_tabs[0] || sender == this->m_tabs[1]) {
        InputFocus focus{g_windowMgr.hoverWindow};
        focus.Set(nullptr);
        for (int index = 0; index < this->m_visibleRows; ++index) {
          UIEditCtrl* field = static_cast<UIEditCtrl*>(this->m_fields[index]);
          field->disabled = 1;
          field->Invalidate();
          field->SetText("");
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
          UIEditCtrl* field = static_cast<UIEditCtrl*>(this->m_fields[index]);
          field->disabled = 0;
          field->Invalidate();
          if (sender == this->m_tabs[2]) {
            field->SetText(this->m_names[index].c_str());
          } else if (sender == this->m_tabs[3]) {
            if (index == 0) {
              field->SetText(this->m_target.c_str());
            } else {
              field->SetText("");
            }
          }
        }
      }
      break;
    }

    case 345:
      if (this->m_mode == 2 || this->m_mode == 3) {
        this->m_namesValid = this->ValidateNames(1);
        this->m_targetValid = this->ValidateTarget(1);
        if (this->m_namesValid == true && this->m_targetValid == true) {
          PACKET_CZ_MACRO_REPORT_HEADER header;
          header.packetType = 0xBDA;
          strncpy_s(header.target, sizeof(header.target), this->m_target.c_str(), _TRUNCATE);
          header.count = static_cast<std::uint8_t>(this->m_nameCount);
          const int length = this->m_nameCount * 16 + sizeof(header);
          header.type = 6;
          char buffer[2048];
          std::memset(buffer + sizeof(header), 0, sizeof(buffer) - sizeof(header));
          if (length > sizeof(buffer)) {
            break;
          }
          header.packetLength = static_cast<std::uint16_t>(length);
          std::memcpy(buffer, &header, sizeof(header));
          for (int index = 0; index < this->m_nameCount; ++index) {
            if (!this->m_names[index].empty()) {
              char name[16];
              strncpy_s(name, sizeof(name), (this->m_names[index] + ".gat").c_str(), _TRUNCATE);
              memcpy_s(buffer + sizeof(header) + index * 16, 16, name, sizeof(name));
            }
          }
          GetRagConnection().Send(length, buffer);
        }
      } else {
        std::vector<int> accounts;
        for (auto it = this->m_entries.begin(); it != this->m_entries.end(); ++it) {
          if (std::find(accounts.begin(), accounts.end(), it->id) == accounts.end()) {
            accounts.push_back(it->id);
          }
        }
        const int result = reinterpret_cast<WindowDialogs*>(&g_windowMgr)
                             ->Confirm(FormatOwnedString(LookupMessage(2876), accounts.size()).c_str(), 2, false, false, 0, "Message", 280, 120, 0);
        if (result != 188) {
          for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            PACKET_CZ_MACRO_REPORT_ACCOUNT packet;
            packet.packetType = 0xA56;
            packet.accountId = *it;
            GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packet.packetType), &packet);
          }
          this->m_entries.clear();
          for (auto it = this->m_rows.begin(); it != this->m_rows.end(); ++it) {
            it->first->SetDisabled(1);
            it->first->Invalidate();
            this->m_removedRows.push_back(it->first);
            it->second->SetDisabled(1);
            it->second->Invalidate();
            this->m_removedRows.push_back(it->second);
          }
          this->m_rows.clear();
        }
      }
      break;
    }
    break;

  case 7:
  case 9:
  case 10: {
    const int previous = this->m_firstRow;
    if (message == 9) {
      this->m_firstRow = previous - this->m_visibleRows + 1;
    } else if (message == 10) {
      this->m_firstRow = previous + (this->m_visibleRows - 1);
    } else {
      this->m_firstRow = previous + argument1;
    }
    if (previous != this->m_firstRow) {
      this->UpdateScrollBar();
    }
    return this->m_firstRow - previous;
  }

  case 34: {
    this->m_state = reinterpret_cast<MacroReporterState*>(argument1);
    if (!reinterpret_cast<MacroReporterWndInfo*>(this->m_state)->IsValid()) {
      reinterpret_cast<MacroReporterWndInfo*>(this->m_state)->Reset();
    }
    this->Resize(this->m_state->width, this->m_state->height);
    this->Move(this->m_state->x, this->m_state->y);
    this->unknown_010C = this->m_state->unknown_0011;
    static_cast<UICheckBox*>(this->unknown_00D4)->SetChecked(this->unknown_010C);
    this->m_entries.clear();
    for (unsigned int index = 0; index < this->m_state->entries.size(); ++index) {
      const MacroReporterEntry& entry = this->m_state->entries[index];
      this->m_entries.push_back(MacroReporterEntry(entry.name, entry.id, entry.unknown_001C));
      this->sub_7EF430(entry.id, entry.name);
    }
    this->UpdateScrollBar();
    break;
  }

  case 98: {
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

    if (!g_windowMgr.CloseWindow(287)) {
      UIMacroUserInfoWnd* info = static_cast<UIMacroUserInfoWnd*>(g_windowMgr.EnsureWindow(287));
      info->Move(this->geometry.x + 15, this->geometry.y + row * 22 + 78);
      const unsigned short job = entry->unknown_001C;
      std::string name = entry->name;
      info->m_name = name;
      info->m_message = g_session.GetJobName(job, -1);
      info->Invalidate();
    }
    break;
  }
  }

  return this->UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
