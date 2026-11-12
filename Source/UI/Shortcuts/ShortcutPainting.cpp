#include "Client/Session/Session.h"
#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Bitmap/SkinPalette.h"
#include "Resource/Bitmap/TextureNames.h"
#include "UI/Shop/ItemDrawing.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Skills/PlayerSkillBundle.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Items/ItemInfo.h"
#include "Contents/Skin/SkinMgr.h"
#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Shortcuts/ShortcutCooldowns.h"
#include "UI/Shortcuts/ShortcutGrid.h"
#include "UI/Shortcuts/UIShortCutWnd.h"
#include "UI/Cooldowns/CooldownOwner.h"
#include "UI/Window/UIWindow.h"
#include "Utility/Text/FormatString.h"

#include <cstdio>
#include <cstring>

namespace {
  // INLINED
  CBitmapRes* Load(const char* path) {
    return static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
  }

  // INLINED
  void Bitmap(UIWindow& window, CBitmapRes* image, int x, int y, bool transparent) {
    window.DrawBitmap(x, y, image ? &image->image : nullptr, transparent);
  }

  // INLINED
  __forceinline void Count(UIWindow& window, int right, int y, int value) {
    char text[16];
    std::sprintf(text, "%d", value);
    window.DrawRightOutlinedText(right, y, text, 0, 0, 0xFFFFFF, 0, 11, 0);
  }
}

// OFFSET: 0x00A86200
// FUNCTION: fn_17019
void UIWindow::DrawRightOutlinedText(
  int right, int y, const char* text, int bytes, std::uint32_t color, std::uint32_t outline, int family, int height,
  unsigned char bold
) {
  if (bytes == 0) {
    bytes = static_cast<int>(std::strlen(text));
  }

  TextContext context(*this->geometry.surface);
  context.SetFont(family, height, bold, 0);
  const int left = right - context.Measure(text, static_cast<int>(std::strlen(text))).cx;
  context.SetColor(outline);
  context.Draw(left - 1, y, text, bytes);
  context.Draw(left + 1, y, text, bytes);
  context.Draw(left, y - 1, text, bytes);
  context.Draw(left, y + 1, text, bytes);
  context.SetColor(color);
  context.Draw(left, y, text, bytes);
}

// OFFSET: 0x0092F8E0
// FUNCTION: fn_12680
void UIShortCutWnd::Paint() {
  UIWindow& window = *this;
  const ShortcutGrid& grid = this->m_grid;
  const NormalItemSlot* const* slots = this->m_slots;
  const NormalItemSlot* const* itemSlots = this->m_itemSlots;
  ResourceMgr& resources = ResourceMgr::Instance();
  CBitmapRes* frame = Load("유저인터페이스\\Shortcut\\shortcut_frame.bmp");

  for (int row = 0; row < grid.rows; ++row) {
    Bitmap(window, frame, 0, (row * 33), false);
  }

  if (g_session.ItemSlotActive()) {
    Bitmap(window, Load("유저인터페이스\\Shortcut\\shortcut_frame_item.bmp"), 0, grid.rows > 0 ? (grid.rows * 33) : 0, false);
  }

  if (grid.row != -1 && grid.hovered != -1) {
    const SkinPaletteCell cell{2, 2};
    window.FillRegion(((grid.hovered * 29) + 16), 5 + 33 * static_cast<unsigned>(g_session.m_hotkeyPage), 24, 24,
                      SkinPaletteColor(resources, cell, g_skinInfo.Selected()));
  }

  g_session.m_mainCooldownIcons.clear();

  for (int row = 0; row < grid.rows; ++row) {
    const int y = ((row * 33) + 4);

    for (int column = 0; column < 9; ++column) {
      const int index = ((row * 9) + column);
      if (index >= 36) {
        continue;
      }

      const NormalItemSlot& slot = *slots[index];
      if (slot.itemId == 0) {
        continue;
      }

      const int x = 16 + 29 * column;
      if (slot.enabled != 0) {
        CSkillInfo skill = g_session.LookupSkill(slot.itemId);
        if (!skill.MatchesShortcut(&slot)) {
          continue;
        }

        char path[1024];
        g_session.ItemBitmapPath(skill.name, path);
        CBitmapRes* image = Load(path);
        if (!image) {
          std::sprintf(path, "유저인터페이스\\item\\am_berserkpitcher.bmp");
          image = Load(path);
        }

        g_session.m_mainCooldownIcons.push_back({skill.id, index, x, y});
        Bitmap(window, image, x, y, true);
        int level = slot.value;

        if (IsIndexedSkill(skill.id) && skill.level < level) {
          level = skill.level;
        }

        Count(window, x + 27, (y + 17), level);
      } else {
        ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(slot.itemId), 1);
        if (item.identity[1] == 0) {
          continue;
        }

        DrawItemIcon(window, resources, x, y, item, item.identified);
        if (item.quantity != 0) {
          Count(window, x + 27, (y + 17), item.quantity);
        }

        if (0 < item.grade) {
          Bitmap(window, Load(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.grade).c_str()), x, (y + 12), true);
        }
      }
    }
  }

  if (g_session.ItemSlotActive()) {
    const int y = ((grid.rows * 33) + 4);

    for (int column = 0; column < 9; ++column) {
      const NormalItemSlot& slot = *itemSlots[column];
      if (slot.itemId == 0) {
        continue;
      }

      ITEM_INFO item = LookupClientItem(g_session.m_inventoryItems, static_cast<int>(slot.itemId), 1);
      if (item.identity[1] == 0) {
        continue;
      }

      const int x = 16 + 29 * column;
      DrawItemIcon(window, resources, x, y, item, item.identified);
      if (item.quantity != 0) {
        Count(window, x + 27, (y + 17), item.quantity);
      }
    }
  }

  for (int row = 0; row < grid.rows; ++row) {
    char text[24];
    std::sprintf(text, "%d", (row + 1));
    window.DrawTextAt(6, ((row * 33) + 13), text, 0, 0, 11, 0x2F4A4A, 0, 0);
  }

  PaintMainCooldowns(g_session.m_requestTimers, g_session.m_mainCooldownIcons, grid.rows, window.geometry.x, window.geometry.y, g_cooldownOwner->Drawing());
}
