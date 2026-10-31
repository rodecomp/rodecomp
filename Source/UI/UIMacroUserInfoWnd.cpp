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
#include "UI/Buttons/UITextButton.h"
#include "UI/Layout/SnapMgr.h"
#include "UI/Layout/WindowSnapping.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/Window/FrameFocus.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <atlconv.h>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x007EE980
// FUNCTION: fn_09831
MacroReporterEntry::MacroReporterEntry(std::string name, int id, short unknown_001C)
    : name(name), id(id), unknown_001C(unknown_001C) {}

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
