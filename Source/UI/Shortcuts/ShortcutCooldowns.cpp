#include "Client/Session.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SkinPalette.h"
#include "Resource/TextureNames.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/SkinMgr.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Shortcuts/UIQuickSlotWnd.h"
#include "UI/Window/UIRebirthWnd.h"
#include "UI/Window/UISeekPartyMBWnd.h"
#include "UI/Window/UISeekPartyWnd.h"
#include "UI/Window/UISeekPartyListWnd.h"
#include "Utility/Text/FormatString.h"
#include "UI/Shortcuts/ShortcutCooldowns.h"

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

namespace {
  // OFFSET: 0x00E1DCA0
  // FUNCTION: fn_23875
  bool IsFirstRange(unsigned id) {
    return id - 8000u <= 60;
  }

  // OFFSET: 0x00E1DD20
  // FUNCTION: fn_23878
  bool IsOtherRange(unsigned id) {
    return id - 8200u <= 41 || id - 8400u <= 57;
  }
}

// OFFSET: 0x00E1DCE0
// FUNCTION: fn_23876
int CSession::IsSkillIdInRange6018(unsigned id) const {
  return id - 6018u <= 34;
}

// OFFSET: 0x00E1DD00
// FUNCTION: fn_23877
int CSession::IsSkillIdInRange6001(unsigned id) const {
  return id - 6001u <= 51;
}

// OFFSET: 0x00D42070
// FUNCTION: fn_21827
void ShortcutCooldowns::LoadSnapshot(
  const void* packet, const std::list<ShortcutCooldownIcon>& mainIcons, const std::list<ShortcutCooldownIcon>& alternateIcons, int replayMode,
  const std::uint32_t& serverTick
) {
  this->records.clear();
  const auto* bytes = static_cast<const unsigned char*>(packet);
  std::int16_t size;
  std::memcpy(&size, bytes + 2, sizeof(size));
  const int count = (size - 2) / 10;

  for (int index = 0; index < count; ++index) {
    const unsigned char* source = bytes + 4 + index * 10;
    std::uint16_t id;
    std::uint32_t duration;
    std::uint32_t remaining;
    std::memcpy(&id, source, sizeof(id));
    std::memcpy(&duration, source + 2, sizeof(duration));
    std::memcpy(&remaining, source + 6, sizeof(remaining));
    const std::uint32_t now = replayMode == 1 ? serverTick : timeGetTime();
    ShortcutCooldownRecord value{id, now + remaining, duration, {}, {}};

    for (const ShortcutCooldownIcon& icon : mainIcons) {
      if (icon.id == id) {
        value.main.emplace_back(icon.slot, icon.x, icon.y, duration, now, duration - remaining, 0);
      }
    }

    for (const ShortcutCooldownIcon& icon : alternateIcons) {
      if (icon.id == id) {
        value.alternate.emplace_back(icon.slot, icon.x, icon.y, duration, now, duration - remaining, 0);
      }
    }

    this->records.push_back(value);
  }
}

// OFFSET: 0x00D41990
// FUNCTION: fn_21826
void ShortcutCooldowns::ApplyStart(
  const void* packet, const std::list<ShortcutCooldownIcon>& mainIcons, const std::list<ShortcutCooldownIcon>& alternateIcons, int replayMode,
  const std::uint32_t& serverTick
) {
  const auto* bytes = static_cast<const unsigned char*>(packet);
  std::uint16_t id;
  std::uint32_t duration;
  std::memcpy(&id, bytes + 2, sizeof(id));
  std::memcpy(&duration, bytes + 4, sizeof(duration));
  const std::uint32_t now = replayMode == 1 ? serverTick : timeGetTime();
  ShortcutCooldownRecord value{id, now + duration, duration, {}, {}};
  const auto find = [&]() {
    return std::find_if(this->records.begin(), this->records.end(), [&](const ShortcutCooldownRecord& record) { return record.id == id; });
  };

  for (const ShortcutCooldownIcon& icon : mainIcons) {
    if (icon.id != id) {
      continue;
    }

    const auto entry = find();
    if (entry == this->records.end()) {
      value.main.push_back(CooldownState(icon.slot, icon.x, icon.y, duration, now, 0, 0));
      this->records.push_back(value);
    } else {
      const auto slot = std::find_if(entry->main.begin(), entry->main.end(), [&](const CooldownState& state) { return state.slot == icon.slot; });
      if (slot == entry->main.end()) {
        entry->main.push_back(CooldownState(icon.slot, icon.x, icon.y, duration, now, 0, 0));
      } else {
        this->records.erase(entry);

        value.main.push_back(CooldownState(icon.slot, icon.x, icon.y, duration, now, 0, 0));
        this->records.push_back(value);
      }
    }
  }

  for (const ShortcutCooldownIcon& icon : alternateIcons) {
    if (icon.id == id && find() == this->records.end()) {
      value.alternate.push_back(CooldownState(icon.slot, icon.x, icon.y, duration, now, 0, 0));
      this->records.push_back(value);
    }
  }
}

// INLINED
bool ShortcutCooldowns::RotateRows(int amount, int bank, int& rotation0, int& rotation1, const CooldownClock& clock) {
  const int rows = amount < 4 ? amount : amount % 4;
  if (rows <= 0) {
    return false;
  }

  if (bank == 0 || bank == 1) {
    int& rotation = bank == 0 ? rotation0 : rotation1;
    rotation += rows;
    if (rotation >= 4) {
      rotation -= 4;
    }
  }

  const std::uint32_t now = clock.Now();

  for (auto entry = this->records.begin(); entry != this->records.end();) {
    if (entry->end <= now) {
      entry = this->records.erase(entry);
      continue;
    }

    for (CooldownState& state : entry->main) {
      state.slot -= 9 * rows;
      state.y -= 33 * rows;
      if (state.slot < 0) {
        state.slot += 36;
        state.y += 132;
      }
    }

    ++entry;
  }

  return true;
}

// INLINED
void ShortcutCooldowns::PaintMain(
  const std::list<ShortcutCooldownIcon>& icons, int rows, int x, int y, CooldownDrawing& drawing, const CooldownClock& clock, RenderCommandPool& pool,
  CRenderSystemBase& system
) {
  const int limit = rows * 33 + 4;
  this->Paint(icons, &ShortcutCooldownRecord::main, limit, x, y, drawing, clock, pool, system);
}

// INLINED
void ShortcutCooldowns::PaintAlternate(
  const std::list<ShortcutCooldownIcon>& icons, int x, int y, CooldownDrawing& drawing, const CooldownClock& clock, RenderCommandPool& pool,
  CRenderSystemBase& system
) {
  this->Paint(icons, &ShortcutCooldownRecord::alternate, 0, x, y, drawing, clock, pool, system);
}

// INLINED
void ShortcutCooldowns::Paint(
  const std::list<ShortcutCooldownIcon>& icons, std::list<CooldownState> ShortcutCooldownRecord::* values, int limit, int x, int y, CooldownDrawing& drawing,
  const CooldownClock& clock, RenderCommandPool& pool, CRenderSystemBase& system
) {
  const std::uint32_t now = clock.Now();

  for (auto entry = this->records.begin(); entry != this->records.end();) {
    if (entry->end <= now) {
      entry = this->records.erase(entry);
      continue;
    }

    std::list<CooldownState>& states = (*entry).*values;

    for (const ShortcutCooldownIcon& icon : icons) {
      if (entry->id == icon.id || (entry->id == 577 && !IsFirstRange(icon.id)) || (entry->id == 9999 && (IsFirstRange(icon.id) || IsOtherRange(icon.id)))) {
        const auto previous = std::find_if(states.begin(), states.end(), [&](const CooldownState& state) { return state.slot == icon.slot; });
        if (previous == states.end()) {
          states.emplace_back(icon.slot, icon.x, icon.y, entry->duration, entry->end - entry->duration, 0, 0);
        }
      }
    }

    for (CooldownState& state : states) {
      drawing.Draw(0, 0, 12, 12, state, limit, x, y, clock, pool, system);
    }

    ++entry;
  }
}

// OFFSET: 0x0092DDE0
// FUNCTION: fn_12673
void UIRebirthWnd::Paint() {
  auto* background = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\win_msgbox1.bmp")));
  this->DrawBitmap(0, 0, &background->image, false);
}

// OFFSET: 0x0092E230
// FUNCTION: fn_12676
void UISeekPartyMBWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, true, "", 0);
  if (this->m_minimized) {
    return;
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));

  const RECT outer = {50, 95, 310, 293};
  const RECT inner = {51, 96, 309, 292};
  this->FillSurface(0, &outer);
  this->FillSurface(0xF0F0F0, &inner);
  this->DrawButtonBar(2, 0, 1);
}

// OFFSET: 0x0092E300
// FUNCTION: fn_12677
void UISeekPartyWnd::Paint() {
  auto& controls = *this->m_controls;
  this->DrawTitleBar(controls.fonts, controls.locale, true, "", 0);

  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));

  if (!this->m_minimized) {
    std::string name;
    const char* names[] = {"bibibic", "bibibic", "달기린", "달기린", "boya", "boya"};
    name = FormatOwnedString(
      "유저인터페이스\\basic_interface\\seekparty\\type_0%d\\seekparty_%s_0%d_%d.bmp", this->m_type, names[this->m_type], this->m_type, this->m_level
    );
    if (ResourceMgr::Instance().Exists(name.c_str())) {
      auto* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name.c_str())));
      this->DrawBitmap(1, 18, &bitmap->image, false);
    }
  }

  this->DrawButtonBar(2, 0, 1);

  char text[10] = {};
  const unsigned seconds = this->m_elapsed / 1000;
  std::sprintf(text, "%02d:%02d:%02d", seconds / 60 / 60 % 60, seconds / 60 % 60, seconds % 60);
  this->DrawTextAt(controls.fonts, controls.locale, 10, this->geometry.height - 20, text, 0, 0, 12, 0, 0, 0);
}

// OFFSET: 0x0092D660
// FUNCTION: fn_12671
void UIQuickSlotWnd::Paint() {
  QuickSlotPainting& painting = *this->m_painting;
  this->DrawPanelBackground(0, 0, "유저인터페이스\\basic_interface\\quickslot.bmp");
  if (this->m_selected != -1) {
    const SkinPaletteCell cell = {2, 2};
    this->FillRegion(this->m_selected * 29 + 5, 5, 24, 24, SkinPaletteColor(ResourceMgr::Instance(), cell, g_skinInfo.Selected()));
  }

  painting.icons.clear();

  for (int index = 0; index < 2; ++index) {
    const QuickSlotEntry& entry = this->m_entries[index];
    if (entry.id == 0) {
      continue;
    }

    const int x = index * 29 + 5;
    if (entry.skill) {
      CSkillInfo skill = painting.skills.Lookup(entry.id);
      if (!skill.available) {
        continue;
      }

      char text[1024];
      std::sprintf(text, "유저인터페이스\\item\\%s.bmp", skill.name);
      auto* image = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(text)));
      painting.icons.push_back({skill.id, index, x, 5});
      this->DrawBitmap(x, 5, &image->image, true);
      if (entry.count != 0) {
        std::sprintf(text, "%d", entry.count);
        this->DrawTextAt(painting.fonts, painting.locale, x + 15, 20, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(painting.fonts, painting.locale, x + 13, 20, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(painting.fonts, painting.locale, x + 14, 21, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(painting.fonts, painting.locale, x + 14, 19, text, 0, 0, 11, 0xFFFFFF, 0, 0);
        this->DrawTextAt(painting.fonts, painting.locale, x + 14, 20, text, 0, 0, 11, 0, 0, 0);
      }
    } else {
      ITEM_INFO item = LookupClientItem(painting.inventory, static_cast<int>(entry.id), 1);
      if (item.unknown_0000[1] == 0) {
        continue;
      }

      DrawItemWithQuantity(*this, ResourceMgr::Instance(), painting.descriptions, painting.fonts, painting.locale, x, 5, item, item.identified, 1);
      if (item.unknown_0088 > 0) {
        const std::string name = FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.unknown_0088);
        auto* grade = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name.c_str())));
        this->DrawBitmap(x, 17, &grade->image, true);
      }
    }
  }

  painting.cooldowns.PaintAlternate(painting.icons, this->geometry.x, this->geometry.y, painting.drawing, painting.clock, painting.commands, painting.system);
}

// OFFSET: 0x00924A50
// FUNCTION: fn_12646
void UIQuickSlotWnd::Slot15(int, int) {}

// OFFSET: 0x00930610
// FUNCTION: fn_12686
void UIQuickSlotWnd::Slot26(int x, int y) {
  if (g_session.GetOption(5) > 0 || static_cast<unsigned>(y) > 23) {
    return;
  }

  if (x > 58 && x < this->geometry.width) {
    return;
  }

  const int slot = (x - 4) / 29;
  if (x > slot * 29 + 30 || slot == -1) {
    return;
  }

  AutomaticSkills& automatic = this->m_painting->automatic;
  const QuickSlotEntry& entry = this->m_entries[slot];
  if (entry.skill && automatic.Contains(static_cast<std::uint16_t>(entry.id))) {
    g_session.SetOption(AutomaticSkills::CancelOption(static_cast<std::int16_t>(entry.id)), 1, 0);
  } else {
    automatic.DisableAll();
  }

  this->SendMsg(nullptr, 41, slot, 0, 0, 0);
}

// OFFSET: 0x00932E80
// FUNCTION: fn_12718
void UIQuickSlotWnd::Slot19() {
  this->Invalidate();
}

// OFFSET: 0x0091C4D0
// FUNCTION: fn_12614
int UIQuickSlotWnd::SlotX(int slot) {
  for (int index = 0; index < 2; ++index) {
    if (index == slot) {
      return index * 29 + 5;
    }
  }

  return -1;
}

// OFFSET: 0x0091C500
// FUNCTION: fn_12615
int UIQuickSlotWnd::SlotAt(int x, int y) {
  if (y < 0 || y >= 24 || (x > 58 && x < this->geometry.width)) {
    return -1;
  }

  const int index = (x - 4) / 29;
  if (x > index * 29 + 30) {
    return -1;
  }

  return index;
}

// OFFSET: 0x00932E90
// FUNCTION: fn_12719
void UISeekPartyWnd::Slot19() {
  int frameCounts[6] = {5, 5, 17, 11, 11, 11};
  const std::uint32_t now = GetTickCount();
  this->m_frameElapsed = now - this->m_frameTick;
  if (this->m_frameElapsed > this->m_frameDuration) {
    this->m_level = this->m_level < frameCounts[this->m_type] ? this->m_level + 1 : 0;
    this->m_elapsed += this->m_frameDuration;
    this->m_frameTick = now;
    this->Invalidate();
  }
}

// OFFSET: 0x00940EE0
// FUNCTION: fn_12783
void UIQuickSlotWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00940F60
// FUNCTION: fn_12785
void UISeekPartyMBWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00940FA0
// FUNCTION: fn_12786
void UISeekPartyWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
  }
}

// OFFSET: 0x00940F20
// FUNCTION: fn_12784
void UISeekPartyListWnd::Slot11() {
  if (this->m_saved) {
    this->m_saved->width = this->geometry.width;
    this->m_saved->height = this->geometry.height;
    this->m_saved->x = this->geometry.x;
    this->m_saved->y = this->geometry.y;
  }
}
