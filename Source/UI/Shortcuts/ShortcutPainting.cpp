#include "Render/Text/TextContext.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "UI/Shop/ItemDrawing.h"
#include "UI/Shortcuts/ShortcutPainting.h"
#include "UI/Window/UIWindow.h"

#include <cstdio>
#include <cstring>

namespace {
  constexpr char prefix[] = "유저인터페이스";

  // INLINED
  CBitmapRes* Load(ResourceMgr& resources, const char* tail) {
    char path[200];
    std::sprintf(path, "%s%s", prefix, tail);
    return static_cast<CBitmapRes*>(resources.Load(path));
  }

  // INLINED
  void Bitmap(UIWindow& window, CBitmapRes* image, int x, int y, bool transparent) {
    window.DrawBitmap(x, y, image ? &image->image : nullptr, transparent);
  }

  // OFFSET: 0x00A86200
  void Count(UIWindow& window, FontCache& fonts, const TextLocale& locale, int right, int y, int value) {
    char text[16];
    std::sprintf(text, "%d", value);
    const int bytes = static_cast<int>(std::strlen(text));
    TextContext context(*window.geometry.surface, fonts, locale);
    context.SetFont(0, 11, 0, 0);
    const int left = (right - context.Measure(text, bytes).cx);
    context.SetColor(0xFFFFFF);
    context.Draw((left - 1), y, text, bytes);
    context.Draw((left + 1), y, text, bytes);
    context.Draw(left, (y - 1), text, bytes);
    context.Draw(left, (y + 1), text, bytes);
    context.SetColor(0);
    context.Draw(left, y, text, bytes);
  }
}

// OFFSET: 0x0092F8E0
// FUNCTION: fn_12680
void ShortcutPainting::Paint(UIWindow& window, const ShortcutGrid& grid, const NormalItemSlot* const* slots, const NormalItemSlot* const* itemSlots) {
  CBitmapRes* frame = Load(this->resources, "\\Shortcut\\shortcut_frame.bmp");

  for (int row = 0; row < grid.rows; ++row) {
    Bitmap(window, frame, 0, (row * 33), false);
  }

  if (this->itemSlotActive) {
    Bitmap(window, Load(this->resources, "\\Shortcut\\shortcut_frame_item.bmp"), 0, grid.rows > 0 ? (grid.rows * 33) : 0, false);
  }

  if (grid.row != -1 && grid.hovered != -1) {
    CBitmapRes* chip = Load(this->resources, "\\colorchip.bmp");
    std::uint32_t color = 0;
    if (chip) {
      const std::uint32_t pixel = chip->image.width > 10 && chip->image.height > 10 ? chip->image.pixels[10 + 10 * chip->image.width] : 0x00FF0000u;
      color = ((pixel & 255) << 16) | (pixel & 0xFF00) | ((pixel >> 16) & 255);
    }

    window.FillRegion(((grid.hovered * 29) + 16), 5 + 33 * static_cast<unsigned>(this->currentRow), 24, 24, color);
  }

  this->icons.clear();

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
      if (slot.unknown_0000 != 0) {
        CSkillInfo skill = this->skills.Lookup(slot.itemId);
        if (!skill.MatchesShortcut(&slot)) {
          continue;
        }

        char path[200];

        std::sprintf(path, "%s\\item\\%s.bmp", prefix, skill.name);
        CBitmapRes* image = static_cast<CBitmapRes*>(this->resources.Load(path));
        if (!image) {
          image = Load(this->resources, "\\item\\am_berserkpitcher.bmp");
        }

        this->icons.push_back({skill.id, index, x, y});
        Bitmap(window, image, x, y, true);
        int level = slot.unknown_0005;

        if ((skill.id == 6579 || skill.id == 6580) && skill.level < level) {
          level = skill.level;
        }

        Count(window, this->fonts, this->locale, x + 27, (y + 17), level);
      } else {
        ItemInfo item = LookupClientItem(this->inventory, static_cast<int>(slot.itemId), 1);
        if (item.unknown_0000[1] == 0) {
          continue;
        }

        DrawItemIcon(window, this->resources, this->descriptions, x, y, item, static_cast<unsigned char>(item.unknown_0060));
        if (item.quantity != 0) {
          Count(window, this->fonts, this->locale, x + 27, (y + 17), item.quantity);
        }

        if (item.unknown_0088 > 0) {
          char tail[96];
          std::sprintf(tail, "\\grade_enchant\\grade_icon%d.bmp", item.unknown_0088);
          Bitmap(window, Load(this->resources, tail), x, (y + 12), true);
        }
      }
    }
  }

  if (this->itemSlotActive) {
    const int y = ((grid.rows * 33) + 4);

    for (int column = 0; column < 9; ++column) {
      const NormalItemSlot& slot = *itemSlots[column];
      if (slot.itemId == 0) {
        continue;
      }

      ItemInfo item = LookupClientItem(this->inventory, static_cast<int>(slot.itemId), 1);
      if (item.unknown_0000[1] == 0) {
        continue;
      }

      const int x = 16 + 29 * column;
      DrawItemIcon(window, this->resources, this->descriptions, x, y, item, static_cast<unsigned char>(item.unknown_0060));
      if (item.quantity != 0) {
        Count(window, this->fonts, this->locale, x + 27, (y + 17), item.quantity);
      }
    }
  }

  for (int row = 0; row < grid.rows; ++row) {
    char text[24];
    std::sprintf(text, "%d", (row + 1));
    window.DrawTextAt(this->fonts, this->locale, 6, ((row * 33) + 13), text, 0, 0, 11, 0x2F4A4A, 0, 0);
  }

  this->cooldowns.PaintMain(this->icons, grid.rows, window.geometry.x, window.geometry.y, this->drawing, this->clock, this->commands, this->system);
}
