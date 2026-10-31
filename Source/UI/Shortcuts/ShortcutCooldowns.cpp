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
#include "UI/Window/UIRoMapWnd.h"
#include "UI/Window/UIJobListWnd.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIWindowMgr.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketLengths.h"
#include "Utility/Text/FormatString.h"
#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "Contents/Party/PartyBookingMgr.h"
#include "Render/Text/FontCache.h"
#include "Render/Text/TextContext.h"
#include "Client/Mode/CLoginMode.h"
#include "Client/Mode/CMode.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Render/DeviceContext/DCSurface.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/Texture.h"
#include "Resource/PaletteResource.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "Script/LuaScript.h"
#include "UI/Window/UISelCharForUServerWnd.h"
#include "UI/Window/UISerachStoreInfoWnd.h"
#include "UI/Window/WindowBorder.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Text/ColoredText.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/MessageTable.h"
#include "Utility/Text/GroupedInteger.h"

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

  struct SearchStoreEntry {
    /* 0000 */ unsigned char unknown_0000[8];
    /* 0008 */ char storeName[80];
    /* 0058 */ int price;
    /* 005C */ std::uint16_t quantity;
    /* 0060 */ ITEM_INFO item;
  };

  static_assert(sizeof(SearchStoreEntry) == sizeof(SessionEntry5BD4));
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

// OFFSET: 0x0092DE20
// FUNCTION: fn_12674
void UIRoMapWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  this->DrawMapNames();
  this->DrawRecordBorders();
  this->DrawFieldLabels();
  this->DrawHoverTooltip();
  this->DrawSelectedMapPreview();
  this->FillSurface(0xFFFF00FF, reinterpret_cast<const RECT*>(&this->m_kafraLeft));
  this->DrawTitleBar(this->m_context->fonts, this->m_context->locale, true, "", 0);
  this->FillRegion(0, 16, this->geometry.width, 2, 0xF7DFC6);
}

// OFFSET: 0x0092DE90
// FUNCTION: fn_12675
void UISeekPartyListWnd::Paint() {
  const TextLocale locale = {};
  this->DrawTitleBar(g_fontCache, locale, true, "", 0);
  if (this->m_minimized) {
    return;
  }

  unsigned red;
  unsigned green;
  unsigned blue;
  SkinPaletteChannels(ResourceMgr::Instance(), 2, 2, red, green, blue);
  this->FillRegion(0, 17, this->geometry.width, this->geometry.height - 38, RGB(red, green, blue));

  if (g_friendRows.size() != 0) {
    int drawn = 0;
    int index = 0;
    int y = 124;
    for (auto it = g_friendRows.begin(); it != g_friendRows.end(); ++it, ++index) {
      if (index < this->m_firstRow * this->m_columnCount) {
        continue;
      }
      if (drawn >= 5) {
        break;
      }

      const PartyBookingEntry entry = it->second;
      const int row = index + 1;
      this->FillRegion(20, y, 510, 40, 0xCECECE);
      if (it->second.isEnd) {
        this->FillRegion(21, y + 1, 508, (row + 1 != static_cast<int>(g_friendRows.size())) + 38, 0xFFFFFF);
        auto* more = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve("유저인터페이스\\basic_interface\\more_info.bmp")));
        this->DrawBitmap(22, y + 2, &more->image, false);
      } else {
        this->FillRegion(21, y + 1, 508, (row != static_cast<int>(g_friendRows.size())) + 38, 0xFFFFFF);
        if (this->m_hoverIndex == index) {
          this->FillRegion(20, y, 510, 40, 0xEF9C73);
        }

        const int textY = y + 3;
        this->DrawTextAt(g_fontCache, locale, 22, textY, entry.time.c_str(), 0, 0, 12, 0, 0, 0);
        const char* charName =
          this->ClipTextToWidth(entry.charName.c_str(), 130, 0, 12, 0);
        this->DrawTextAt(g_fontCache, locale, 76, textY, charName, 0, 0, 12, 0, 0, 0);
        this->DrawTextAt(g_fontCache, locale, 215, textY, entry.level.c_str(), 0, 0, 12, 0, 0, 0);
        this->DrawTextAt(g_fontCache, locale, 280, textY, entry.location.c_str(), 0, 0, 12, 0, 0, 0);
        this->DrawTextAt(g_fontCache, locale, 22, y + 23, entry.jobs.c_str(), 0, 0, 12, 0, 0, 0);
      }

      y += 40;
      ++drawn;
    }

    this->UpdateScrollBar();
  }

  this->DrawButtonBar(2, 0, 1);
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

// OFFSET: 0x0093A640
// FUNCTION: fn_12757
int UISeekPartyWnd::SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) {
  switch (message) {
  case 6:
    switch (argument1) {
    case 201: {
      std::int16_t packetType = 0x806;
      GetRagConnection().Send(reinterpret_cast<const PacketLengths&>(GetRagConnection()).FixedLength(packetType), &packetType);
      g_windowMgr.CloseWindow(170);
      break;
    }
    case 202:
      if (this->m_minimized) {
        this->Resize(this->geometry.width, this->unknown_01E0);
      } else {
        this->unknown_01E0 = this->geometry.height;
        this->Resize(this->geometry.width, 44);
      }
      this->m_minimized = !this->m_minimized;
      this->m_cancelButton->Move(this->geometry.width - 46, this->geometry.height - 24);
      this->m_rewriteButton->Move(this->geometry.width - 92, this->geometry.height - 24);
      this->Invalidate();
      break;
    case 284:
      g_windowMgr.EnsureWindow(170);
      if (auto* jobs = static_cast<UIJobListWnd*>(g_windowMgr.LookupWindow(170))) {
        jobs->RefreshJobs();
      }
      break;
    }
    break;
  case 34:
    this->m_saved = reinterpret_cast<SeekPartyWndInfo*>(argument1);
    if (!this->m_saved->IsValid()) {
      this->m_saved->Reset();
    }
    this->Move(this->m_saved->x, this->m_saved->y);
    this->SendMsg(nullptr, 14, 3, this->m_saved->width, this->m_saved->height, 0);
    this->SendMsg(nullptr, 23, 0, 0, 0, 0);
    return 0;
  }

  return UIFrameWnd::SendMsg(sender, message, argument1, argument2, argument3, argument4);
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

// OFFSET: 0x00D42590
// FUNCTION: fn_21828
void ShortcutCooldowns::LoadRecords(const void* packet) {
  const auto* bytes = static_cast<const unsigned char*>(packet);
  std::int16_t size;
  std::memcpy(&size, bytes + 2, sizeof(size));
  const int count = (size - 2) / 6;

  for (int index = 0; index < count; ++index) {
    const unsigned char* source = bytes + 4 + index * 6;
    ShortcutCooldownRecord value;
    std::uint16_t id;
    std::uint32_t remaining;
    std::memcpy(&id, source, sizeof(id));
    std::memcpy(&remaining, source + 2, sizeof(remaining));
    value.id = id;
    value.end = timeGetTime() + remaining;
    this->records.push_back(value);
  }
}

// GLOBAL: gl_24794
int g_charViewOffsetX;

// GLOBAL: gl_24795
int g_charViewOffsetY;

// OFFSET: 0x0092E570
// FUNCTION: fn_12678
void UISelCharForUServerWnd::Paint() {
  const char* background =
    g_charViewCount > 12 ? "유저인터페이스\\login_interface\\selCharForIntergration2.bmp" : "유저인터페이스\\login_interface\\selCharForIntergration.bmp";
  this->DrawBitmap(0, 0, &static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(background)))->image, false);

  for (int index = 0; index < g_charViewCount; ++index) {
    if (this->m_slotEnabled[index] != 0) {
      continue;
    }

    CharSlotInfo& slot = this->m_slots[index];
    this->DrawRobeSprite(slot.job, g_session.SyncUnknown0E98(), slot.robe, slot.x, slot.y, false, 0, 0);

    SpriteAtlasRecord record{};
    for (int layer = 0; layer < 5; ++layer) {
      if (slot.actionNames[layer].size() == 0 || slot.spriteNames[layer].size() == 0) {
        continue;
      }

      ResourceMgr::Instance().Load(slot.imfName.c_str());
      auto* act = static_cast<CActRes*>(ResourceMgr::Instance().Load(slot.actionNames[layer].c_str()));
      auto* sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(slot.spriteNames[layer].c_str()));
      const ActionFrame* frame = &act->Frame(slot.action, slot.frame);

      int offsetX = 0;
      int offsetY = 0;
      if (g_session.IsDoramJob(slot.job) == true) {
        const auto* info = reinterpret_cast<const CharacterInfo*>(static_cast<CMode*>(g_charViewMode)->SendMsg(8, index, 0, 0, 0));
        int x = 0;
        int y = 0;
        int item = 0;
        if (layer == 4) {
          item = info->accessory;
        } else if (layer == 3) {
          item = info->accessory2;
        } else if (layer == 2) {
          item = info->accessory3;
        }

        int turn = 0;
        if (slot.frame == 2) {
          turn = -1;
        } else if (slot.frame == 1) {
          turn = 1;
        }

        int direction = turn - slot.unknown_0148 + slot.action;
        if (direction < 0) {
          direction = 7;
        }
        if (direction > 7) {
          direction = 0;
        }

        g_luaScript->Call("OffsetItemPos_GetOffsetForDoram", "ddd>dd", item, direction, g_session.SyncUnknown0E98(), &x, &y);
        offsetX = x;
        offsetY = y;
      }

      if (layer == 1) {
        const ActionFrame* body = &static_cast<CActRes*>(ResourceMgr::Instance().Load(slot.actionNames[0].c_str()))->Frame(slot.action, slot.frame);
        if (frame->anchorCount != 0 && body->anchorCount != 0) {
          const ActionAnchor* head = frame->anchors.data();
          const ActionAnchor* base = body->anchors.data();
          if (head->unknown_0008 == base->unknown_0008) {
            offsetX += base->x - head->x;
            g_charViewOffsetX = offsetX;
            offsetY += base->y - head->y;
            g_charViewOffsetY = offsetY;
          }
        }
      } else if (layer == 2 || layer == 3 || layer == 4) {
        const ActionFrame* head = &static_cast<CActRes*>(ResourceMgr::Instance().Load(slot.actionNames[1].c_str()))->Frame(slot.action, slot.frame);
        int syncedFrame = 0;
        const ActionFrame* synced = g_session.GetSyncedFrame(
          static_cast<CActRes*>(ResourceMgr::Instance().Load(slot.actionNames[1].c_str())),
          static_cast<CActRes*>(ResourceMgr::Instance().Load(slot.actionNames[layer].c_str())), slot.action, slot.frame, this->m_startTime, &syncedFrame
        );
        if (synced && syncedFrame != slot.frame) {
          frame = synced;
        }

        if (frame->anchorCount != 0 && head->anchorCount != 0) {
          const ActionAnchor* anchor = frame->anchors.data();
          const ActionAnchor* base = head->anchors.data();
          if (anchor->unknown_0008 == base->unknown_0008) {
            offsetX += base->x - anchor->x;
            offsetY += base->y - anchor->y;
          }
        }

        offsetX += g_charViewOffsetX;
        offsetY += g_charViewOffsetY;
        if (layer == 3 && (this->unknown_1974[index] == 0xB5 || this->unknown_1974[index] == 0xC5)) {
          offsetY -= 7;
        }
      }

      const std::uint32_t* palette = sprite->palette;
      if (layer == 0) {
        if (slot.bodyPalette != 0) {
          palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(slot.bodyPaletteName.c_str()))->palette;
        }
      } else if (layer == 1) {
        if (slot.headPalette != 0) {
          palette = static_cast<CPaletteRes*>(ResourceMgr::Instance().Load(slot.headPaletteName.c_str()))->palette;
        }
      }

      const SpriteFrameCell& cell = frame->Cell(layer < 2 ? layer : 0);
      if (cell.image == -1) {
        continue;
      }

      const SpriteRleImage* image = sprite->images[cell.imageType][cell.image];
      CTexture* texture;
      if (cell.imageType == 0) {
        texture = g_renderer->atlas.Find(image, palette, record);
        if (!texture) {
          texture = g_renderer->atlas.Insert(*GetRenderSystemHolder()->system, image, palette, record, g_serviceType, g_resourceReuseDelay);
        }
      } else {
        texture = static_cast<CTexture*>(const_cast<void*>(image->unknown_0008));
      }

      this->geometry.surface->DrawSurface(
        cell.x + offsetX + slot.x, cell.y + offsetY + slot.y, texture, static_cast<int>(record.u0 * 256.0f), static_cast<int>(record.v0 * 256.0f),
        image->width, image->height, cell.mirrored, image->unknown_0004 + 1, image->unknown_0006 + 1, 0, 0
      );
    }
  }
}

// OFFSET: 0x0092EBE0
// FUNCTION: fn_12679
void UISerachStoreInfoWnd::Paint() {
  UIButtonContext& controls = *this->m_controls;
  const bool empty = std::string(this->m_edits[1]->Text()).size() == 0;
  this->m_findButton->SetDisabled(empty);
  this->m_findButton->Invalidate();
  this->m_edits[0]->disabled = empty;
  this->m_edits[0]->Invalidate();
  this->m_edits[2]->disabled = empty;
  this->m_edits[2]->Invalidate();
  this->m_edits[3]->disabled = empty;
  this->m_edits[3]->Invalidate();

  this->DrawPanelBackground(0, 0, "");
  this->DrawTitleBar(controls.fonts, controls.locale, true, LookupMessage(0x6F4), 0);
  DrawOutlinedText(*this, 5, 20, "Type", 0, 0x7F3CFF, 0xFFFFFF, 0, 11, 1, 0);
  DrawOutlinedText(*this, 98, 20, "Item", 0, 0x7F3CFF, 0xFFFFFF, 0, 11, 1, 0);
  DrawOutlinedText(*this, 185, 20, "Card", 0, 0x7F3CFF, 0xFFFFFF, 0, 11, 1, 0);
  DrawOutlinedText(*this, 273, 20, "Zeny", 0, 0x7F3CFF, 0xFFFFFF, 0, 11, 1, 0);
  this->DrawTextAt(controls.fonts, controls.locale, 353, 38, "~", 0, 0, 12, 0, 0, 0);

  const std::string count = FormatOwnedString(LookupMessage(0x70A), g_session.unknown_5BE0);
  this->DrawRightAlignedText(this->geometry.width - 60, 60, count.c_str(), 0, 0, 12, 0, 0);

  this->FillRegion(5, 80, this->geometry.width - 10, 20, 0xDFFFFE);
  DrawRoundedBorder(*this, 5, 80, this->geometry.width - 10, this->unknown_00E8 * 10 + 21, 0xC0C0C0);
  DrawRectangleBorder(*this, 50, 80, 110, 21, 0xC0C0C0);
  DrawRectangleBorder(*this, 330, 80, 30, 21, 0xC0C0C0);
  this->FillRegion(5, 100, this->geometry.width - 10, 1, 0xC0C0C0);

  this->DrawTextAt(
    controls.fonts, controls.locale, (110 - this->TextWidth(controls.fonts, controls.locale, LookupMessage(0x6F5), 0, 0, 12, 0, 0)) / 2 + 50, 85,
    LookupMessage(0x6F5), 0, 0, 12, 0, 0, 0
  );
  this->DrawTextAt(
    controls.fonts, controls.locale, (170 - this->TextWidth(controls.fonts, controls.locale, LookupMessage(0x424), 0, 0, 12, 0, 0)) / 2 + 160, 85,
    LookupMessage(0x424), 0, 0, 12, 0, 0, 0
  );
  this->DrawTextAt(
    controls.fonts, controls.locale, (30 - this->TextWidth(controls.fonts, controls.locale, LookupMessage(0x6F6), 0, 0, 12, 0, 0)) / 2 + 330, 85,
    LookupMessage(0x6F6), 0, 0, 12, 0, 0, 0
  );
  this->DrawTextAt(
    controls.fonts, controls.locale, (75 - this->TextWidth(controls.fonts, controls.locale, LookupMessage(0x6F7), 0, 0, 12, 0, 0)) / 2 + 360, 85,
    LookupMessage(0x6F7), 0, 0, 12, 0, 0, 0
  );

  if (this->unknown_00F0.size() != 0) {
    this->DrawTextAt(controls.fonts, controls.locale, 5, this->geometry.height - 20, this->unknown_00F0.c_str(), 0, 0, 12, 0xFF, 0, 0);
  }

  if (static_cast<unsigned int>(this->unknown_00DC * 10 - 10) >= g_session.unknown_5BD4.size()) {
    return;
  }

  const SearchStoreEntry* end = reinterpret_cast<const SearchStoreEntry*>(g_session.unknown_5BD4.data() + g_session.unknown_5BD4.size());
  const SearchStoreEntry* entry = reinterpret_cast<const SearchStoreEntry*>(g_session.unknown_5BD4.data()) + (this->unknown_00DC - 1) * 10;
  ItemDescriptions descriptions{g_itemDescriptions};
  for (int row = 0; entry != end; ++entry) {
    if (row == 10) {
      break;
    }

    if (this->unknown_00E0 == row) {
      this->FillRegion(5, this->unknown_00E8 * row + 100, this->geometry.width - 10, this->unknown_00E8, 0xFBDFCA);
    }
    DrawRectangleBorder(*this, 50, this->unknown_00E8 * row + 100, 110, this->unknown_00E8 + 1, 0xC0C0C0);
    DrawRectangleBorder(*this, 330, this->unknown_00E8 * row + 100, 30, this->unknown_00E8 + 1, 0xC0C0C0);

    const int next = row + 1;
    if (next < 10) {
      this->FillRegion(5, this->unknown_00E8 * next + 100, this->geometry.width - 10, 1, 0xC0C0C0);
    }

    const int top = this->unknown_00E8 * row + 100;
    const int y = top + 7;
    DrawItemIcon(*this, controls.resources, descriptions, 13, top, entry->item, 0);
    if (entry->item.unknown_0088 > 0) {
      this->DrawBitmap(
        13, y + 5,
        &static_cast<CBitmapRes*>(
          ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", entry->item.unknown_0088).c_str()))
        )->image,
        true
      );
    }

    std::string storeName = entry->storeName;
    const std::size_t lineEnd = storeName.find('\r');
    if (lineEnd != std::string::npos) {
      storeName = storeName.substr(0, lineEnd);
    }
    this->DrawTextAt(controls.fonts, controls.locale, 55, y, this->ClipTextToWidth(storeName.c_str(), 100, 0, 12, 0), 0, 0, 12, 0, 0, 0);
    this->DrawTextAt(controls.fonts, controls.locale, 165, y, this->ClipTextToWidth(entry->item.DisplayName(0).c_str(), 160, 0, 12, 0), 0, 0, 12, 0, 0, 0);

    char text[100];
    std::sprintf(text, "%d", entry->quantity);
    this->DrawTextAt(
      controls.fonts, controls.locale, (30 - this->TextWidth(controls.fonts, controls.locale, text, 0, 0, 12, 0, 0)) / 2 + 330, y, text, 0, 0, 12, 0, 0, 0
    );

    const std::string price = FormatGroupedInteger(entry->price, text, 100) + std::string(" Z");
    this->DrawAmount(controls.fonts, controls.locale, entry->price, price, 435 - this->TextWidth(controls.fonts, controls.locale, price.c_str(), 0, 0, 12, 0, 0), y);

    row = next;
  }

  char page[12];
  std::sprintf(page, " %d ", this->unknown_00DC);
  this->DrawTextAt(
    controls.fonts, controls.locale, (this->geometry.width - this->TextWidth(controls.fonts, controls.locale, page, 0, 0, 12, 0, 0)) / 2,
    this->geometry.height - 20, page, 0, 0, 12, 0, 0, 0
  );
}
