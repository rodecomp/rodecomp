#include "UI/UICheckAttendanceWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeCursor.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Mode/ModeTooltip.h"
#include "Client/Session.h"
#include "Contents/CheckAttendanceMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Render/Renderer.h"
#include "Render/ScreenRectangle.h"
#include "Render/TextureQuad.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "Contents/MessageTable.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Script/LuaScript.h"
#include "UI/Buttons/UIBmpRadioBtn.h"
#include "UI/Buttons/UIBmpTextButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Window/UIWindowMgr.h"
#include "Utility/Text/FormatString.h"

#include <windows.h>
#include <shellapi.h>
#include <timeapi.h>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <list>
#include <string>

struct CheckAttendanceRequestPacket {
  /* 0000 */ std::int16_t id;
};

// OFFSET: 0x007DC6B0
// FUNCTION: fn_09631
UICheckAttendanceWnd::UICheckAttendanceWnd(UIWindowContext& windows, UIFrameContext& frames)
    : UIFrameWnd(windows, frames, 0), m_blinking(0), m_checkedDays(0), m_daysLeft(0),
      m_highlight(0), m_notice(nullptr) {
  this->m_rewardCount = static_cast<int>(g_checkAttendanceMgr->m_rewards.size());
}

// OFFSET: 0x007DCAD0

// OFFSET: 0x007DCB40

// OFFSET: 0x007DCBD0
// FUNCTION: fn_09636
void UICheckAttendanceWnd_Banner::AddBanner(std::string image, std::string url, int action, int argument) {
  AttendanceBanner entry{image, url, action, argument};
  this->m_entries.push_back(entry);
}

// OFFSET: 0x007DCCE0
// FUNCTION: fn_09637
void UICheckAttendanceWnd::VisitChildren() {
  CTexture* texture = g_texMgr.Load("유저인터페이스\\check_attendance\\bt_slot_off.tga", 0, false, true, false);
  for (int day = 0; day < this->m_checkedDays; ++day) {
    if (texture == nullptr) {
      continue;
    }

    if (day == this->m_checkedDays - 1 && this->m_blinking == 0) {
      texture = g_texMgr.Load("유저인터페이스\\check_attendance\\bt_slot_complete.tga", 0, false, true, false);
    }

    DrawTextureRegion(
      g_renderer->primitives, g_renderer->queues, static_cast<float>(this->geometry.x + (day % 5) * 62 + 26),
      static_cast<float>(this->BannerHeight() + 95 + (day / 5) * 64 + this->geometry.y), static_cast<float>(texture->Width()),
      static_cast<float>(texture->Height()), 0xFFFFFFFF, texture, g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
    );
  }
}

// OFFSET: 0x007DCE60
// FUNCTION: fn_09638
int UICheckAttendanceWnd::SlotAt(int x, int y) {
  const int top = this->BannerHeight() + 95;
  const int column = x - 26;
  if (static_cast<unsigned int>(column) <= 306 && top <= y && y <= top + 252 && column % 62 <= 58) {
    const int row = y - top;
    if (row % 64 <= 60) {
      return (column / 62) % 5 + (row / 64) % 4 * 5;
    }
  }
  return -1;
}

// INLINED
UICheckAttendanceWnd_Banner::UICheckAttendanceWnd_Banner(UIWindowContext& windows, UIButtonContext& controls)
    : UIWindow(windows, 0), m_current(0), m_lastSwitch(timeGetTime()), m_controls(&controls) {
}

// OFFSET: 0x007DCF50
// FUNCTION: fn_09639
void UICheckAttendanceWnd::Slot15(int, int) {
  tm end;
  end.tm_mday = g_checkAttendanceMgr->m_config1;
  end.tm_hour = 15;
  end.tm_min = 0;
  end.tm_sec = 0;
  end.tm_year = end.tm_mday / 10000 - 1900;
  end.tm_mon = end.tm_mday % 10000 / 100 - 1;
  end.tm_mday = end.tm_mday % 100;
  __time32_t now;
  _time32(&now);
  __time32_t remaining = _mktime32(&end) - now;
  if (remaining < 1) {
    this->m_daysLeft = 0;
  } else {
    this->m_daysLeft = _localtime32(&remaining)->tm_yday;
  }

  this->m_notice = new UIBmpTextButton(*this->m_context, *this->m_controls);
  this->m_notice->SetBitmap("유저인터페이스\\check_attendance\\bt_ok_normal.bmp", 0, 0);
  this->m_notice->SetBitmap("유저인터페이스\\check_attendance\\bt_ok_normal.bmp", 1, 0);
  this->m_notice->SetBitmap("유저인터페이스\\check_attendance\\bt_ok_press.bmp", 2, 0);
  this->m_notice->SetSize(this->m_notice->BitmapWidth(), this->m_notice->BitmapHeight());
  this->m_notice->SetText(LookupMessage(3045));
  this->m_notice->AddTextStyle(0, 14, 0x313131, 1, 1, -1000, -1000);
  this->m_notice->AddTextStyle(1, 14, 0x313131, 1, 1, -1000, -1000);
  this->m_notice->AddTextStyle(2, 14, 0x313131, 1, 1, -1000, -1000);
  this->m_notice->Move(170, 376);
  this->m_notice->SetId(201);
  this->AddChild(this->m_notice);

  this->m_banner = new UICheckAttendanceWnd_Banner(*this->m_context, *this->m_controls);
  this->m_banner->SetSize(441, 41);
  this->m_banner->Move(this->geometry.width / 2 - this->m_banner->geometry.width / 2, 85);
  this->AddChild(this->m_banner);
}

// OFFSET: 0x007DD2E0
// FUNCTION: fn_09640
void UICheckAttendanceWnd::Paint() {
  UIWindowContext& controls = *this->m_context;
  ItemDescriptions descriptions{g_itemDescriptions};

  const char* backgroundName = this->m_banner->m_entries.size() != 0 ? "유저인터페이스\\check_attendance\\attendance_bg_ad.bmp"
                                                                     : "유저인터페이스\\check_attendance\\attendance_bg.bmp";
  CBitmapRes* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(backgroundName)));
  this->DrawBitmap(0, 0, background ? &background->image : nullptr, false);

  this->DrawCenteredText(0, 41, FormatOwnedString(LookupMessage(3465), g_session.m_serverName.c_str()).c_str(), this->geometry.width, 0, 13, 0x203F66, 1);

  const int start = g_checkAttendanceMgr->m_config0;
  const int end = g_checkAttendanceMgr->m_config1;
  this->DrawCenteredText(
    0, 70, FormatOwnedString(LookupMessage(3466), start % 10000 / 100, start % 100, end % 10000 / 100, end % 100).c_str(), this->geometry.width, 0, 12,
    0x3950A4, 1
  );

  const int bannerHeight = this->BannerHeight();
  this->DrawCenteredText(345, bannerHeight + 289, LookupMessage(3470), 109, 0, 14, 0x595939, 1);
  if (this->m_daysLeft > 0) {
    this->DrawCenteredText(368, bannerHeight + 309, IntToString(this->m_daysLeft, 0).c_str(), 54, 0, 22, 0xFFFFFF, 1);
  }
  this->DrawCenteredText(425, bannerHeight + 318, LookupMessage(3471), 0, 0, 12, 0x595939, 1);

  {
    std::string message;
    if (this->m_blinking == 1) {
      message = FormatOwnedString(LookupMessage(3468), this->m_checkedDays + 1);
    } else {
      message = FormatOwnedString(LookupMessage(3469), this->m_checkedDays);
    }

    if (!message.empty()) {
      std::list<std::string> lines;
      this->WrapFormattedText(message, &lines, 112, 0, 12, 1);
      const int top = static_cast<int>((77 - lines.size() * 15) * 0.5 + 95);
      int line = 0;
      for (auto it = lines.begin(); it != lines.end(); ++it, ++line) {
        this->DrawCenteredText(343, bannerHeight + top + line * 12 + std::max(line - 1, 0) * 3, it->c_str(), 112, 0, 12, 0x613CA5, 1);
      }
    }
  }

  if (this->m_blinking == 1) {
    CBitmapRes* slot = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\check_attendance\\bt_slot_a.bmp")));
    switch (this->m_highlight) {
    case 0:
      slot = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\check_attendance\\bt_slot_a.bmp")));
      break;
    case 1:
      slot = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\check_attendance\\bt_slot_b.bmp")));
      break;
    case 2:
      slot = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\check_attendance\\bt_slot_press.bmp")));
      break;
    }
    this->DrawBitmap(this->m_checkedDays % 5 * 62 + 26, this->m_checkedDays / 5 * 64 + 95 + bannerHeight, slot ? &slot->image : nullptr, true);
  }

  for (int day = 1; day <= this->m_rewardCount; ++day) {
    ITEM_INFO item;
    item.SetItemId(g_checkAttendanceMgr->GetReward(day).first);
    item.quantity = g_checkAttendanceMgr->GetReward(day).second;
    g_checkAttendanceMgr->GetReward(day);

    const int x = (day - 1) % 5 * 62;
    const int y = (day - 1) / 5 * 64;
    DrawItemWithQuantity(*this, ResourceMgr::Instance(), descriptions, controls.fonts, controls.locale, x + 43, y + 109 + bannerHeight, item, 1, 0);
    this->DrawCenteredText(x + 26, y + 140 + bannerHeight, FormatOwnedString(LookupMessage(3467), day).c_str(), 58, 0, 12, 0xA44F3B, 1);
  }
}

// OFFSET: 0x007DDA90
// FUNCTION: fn_09641
void UICheckAttendanceWnd_Banner::Paint() {
  if (this->m_entries.size() != 0) {
    this->DrawBitmap(
      0, 0,
      reinterpret_cast<const BitmapImage*>(
        ResourceMgr::Instance().Load(Resolve(("유저인터페이스\\check_attendance\\" + this->m_entries[this->m_current].image).c_str()))
      ),
      false
    );
  }
}

// OFFSET: 0x007DDBB0
// FUNCTION: fn_09642
void UICheckAttendanceWnd::Slot25(int x, int y) {
  if (this->SlotAt(x, y) == this->m_checkedDays) {
    this->m_highlight = 2;
    this->Invalidate();
  }
}

// OFFSET: 0x007DDBF0
// FUNCTION: fn_09643
void UICheckAttendanceWnd::Slot31(int x, int y) {
  const int slot = this->SlotAt(x, y);
  if (slot == this->m_checkedDays) {
    this->SendMsg(nullptr, 6, 184, slot, 0, 0);
  }
}

// OFFSET: 0x007DDC30
// FUNCTION: fn_09644
void UICheckAttendanceWnd_Banner::Slot31(int x, int y) {
  const std::string url = this->m_entries[this->m_current].url;
  if (!url.empty()) {
    ShellExecuteA(nullptr, "OPEN", url.c_str(), url.c_str(), nullptr, SW_SHOW);
    return;
  }

  if (g_windowMgr.LookupWindow(this->m_entries[this->m_current].action)) {
    return;
  }

  if (this->m_current < this->m_entries.size()) {
    const AttendanceBanner& entry = this->m_entries[this->m_current];
    if (entry.action == 318) {
      g_modeMgr.gameMode->SendMsg(323, entry.argument, 0, 0, 0);
    } else {
      g_windowMgr.EnsureWindow(entry.action);
    }
  }
}

// OFFSET: 0x007DDD70
// FUNCTION: fn_09645
void UICheckAttendanceWnd::Slot28(int x, int y) {
  const int slot = this->SlotAt(x, y);
  if (slot < 0 || slot >= this->m_rewardCount) {
    return;
  }

  ITEM_INFO item;
  item.SetItemId(g_checkAttendanceMgr->GetReward(slot + 1).first);
  item.identified = 1;

  char name[260];
  std::memset(name, 0, sizeof(name));
  int capacity = sizeof(name);
  ItemDescriptions descriptions{g_itemDescriptions};
  descriptions.Name(item, name, capacity, 0);
  if (std::strlen(name) != 0) {
    int tipX = 0;
    int tipY = -20;
    this->ToScreen(tipX, tipY);
    reinterpret_cast<ModeTooltip*>(g_modeMgr.gameMode)->Show(name, tipX + x, tipY + y, 0xFFFFFFFFu, 0, 1);
  }
}

// OFFSET: 0x007DDEF0
// FUNCTION: fn_09646
void UICheckAttendanceWnd_Banner::ResetCursor(int, int) {
  this->m_context->cursor.Set(2, this->m_context->cursorTick);
}

// OFFSET: 0x007DDF00
// FUNCTION: fn_09647
void UICheckAttendanceWnd::Slot19() {
  if (this->m_blinking != 0) {
    const int highlight = timeGetTime() / 100 & 1;
    if (this->m_highlight != 2 && this->m_highlight != highlight) {
      this->m_highlight = highlight;
      this->Invalidate();
    }
  }
}

// OFFSET: 0x007DDF50
// FUNCTION: fn_09648
void UICheckAttendanceWnd_Banner::Slot19() {
  if (this->m_lastSwitch + 5000 < timeGetTime()) {
    const unsigned int next = this->m_current + 1 < this->m_entries.size() ? this->m_current + 1 : 0;
    this->SendMsg(this->m_buttons[next], 6, 215, 0, 0, 0);
  }
}

// OFFSET: 0x007DDFC0
// FUNCTION: fn_09649
void UICheckAttendanceWnd::Slot32(int x, int y) {
  const int slot = this->SlotAt(x, y);
  if (slot < 0 || slot >= this->m_rewardCount) {
    return;
  }

  UIWindow* window = g_windowMgr.EnsureWindow(12);
  if (!window) {
    return;
  }

  ITEM_INFO item;
  item.SetItemId(g_checkAttendanceMgr->GetReward(slot + 1).first);
  item.identified = 1;
  window->SendMsg(nullptr, 24, reinterpret_cast<std::intptr_t>(&item), 0, 0, 0);
}

// OFFSET: 0x007DE0C0
// FUNCTION: fn_09650
void UICheckAttendanceWnd_Banner::CreateButtons() {
  if (this->m_entries.size() > 1) {
    int index = 0;
    for (auto it = this->m_entries.begin(); it != this->m_entries.end(); ++it) {
      UIBmpRadioBtn* button = new UIBmpRadioBtn(*this->m_context, *this->m_controls);
      button->SetBitmap("유저인터페이스\\check_attendance\\btn_ad_off.bmp", 0);
      button->SetBitmap("유저인터페이스\\check_attendance\\btn_ad_off.bmp", 1);
      button->SetBitmap("유저인터페이스\\check_attendance\\btn_ad_on.bmp", 2);
      button->Move(this->geometry.width + (index - static_cast<int>(this->m_entries.size())) * 12 - 12, 41);
      button->SendMsg(this, 13, index == static_cast<int>(this->m_current), 0, 0, 0);
      this->AddChild(button);
      this->m_buttons.push_back(button);
      ++index;
    }
    this->Invalidate();
  }
}

// OFFSET: 0x007DE250
// FUNCTION: fn_09651
int UICheckAttendanceWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  if (message == 6) {
    if (argument1 == 184) {
      if (this->m_blinking) {
        CheckAttendanceRequestPacket packet;
        packet.id = 0xAEF;
        GetRagConnection().Send(g_packetLengths.FixedLength(packet.id), &packet);
      }
    } else if (argument1 == 201) {
      g_windowMgr.CloseWindow(328);
    }
  } else if (message == 34) {
    if (g_luaScript->Call("set_checkAttendance_banner", "") == true) {
      this->m_banner->CreateButtons();
    }
    if (this->m_banner->m_entries.size() == 0) {
      this->m_banner->Show(0);
      this->Resize(488, 413);
      this->m_notice->Move(170, 376);
    } else {
      this->m_banner->Show(1);
      this->Resize(488, 455);
      this->m_notice->Move(170, 418);
    }
    this->m_checkedDays = argument1 / 10;
    this->m_blinking = argument1 == this->m_checkedDays * 10;
    this->m_blinking = this->m_checkedDays < this->m_rewardCount && this->m_blinking;
    this->Invalidate();
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}

// OFFSET: 0x007DE460
// FUNCTION: fn_09652
int UICheckAttendanceWnd_Banner::SendMsg(
  UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
) {
  if (message == 6 && argument1 == 215) {
    for (unsigned int index = 0; index < this->m_buttons.size(); ++index) {
      UIWindow* button = this->m_buttons[index];
      if (sender == button) {
        button->SendMsg(this, 13, 1, 0, 0, 0);
        this->m_current = index;
      } else {
        button->SendMsg(this, 13, 0, 0, 0, 0);
      }
    }
    this->m_lastSwitch = timeGetTime();
    this->Invalidate();
  }
  return UIWindow::SendMsg(sender, message, argument1, argument2, argument3, argument4);
}
