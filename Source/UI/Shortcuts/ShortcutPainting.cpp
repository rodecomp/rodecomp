#include "Client/Session/Session.h"
#include "Contents/Items/ItemId.h"
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

#include <algorithm>
#include <cstdio>
#include <cstring>

namespace {
  // INLINED
  CBitmapRes* Load(const char* path) {
    return static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(path)));
  }

  // INLINED
  void DrawGradeIcon(UIWindow& window, int x, int y, const ITEM_INFO& item) {
    window.DrawBitmap(
      x, y, static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(FormatOwnedString("유저인터페이스\\grade_enchant\\grade_icon%d.bmp", item.grade).c_str()))), true
    );
  }

  // INLINED
  void DrawRowNumber(UIWindow& window, int row) {
    window.DrawTextAt(6, row * 33 + 13, FormatOwnedString("%d", row + 1).c_str(), 0, 0, 11, 0x2F4A4A, 0, 0);
  }
}

// OFFSET: 0x00918940
// FUNCTION: fn_45758
__forceinline void UIShortCutWnd::DrawShortcutItem(int x, int y, const ITEM_INFO& item, unsigned char identified) {
  this->DrawItemIcon(x, y, item, identified);
  if (item.quantity != 0) {
    char text[16];
    std::sprintf(text, "%d", item.quantity);
    this->DrawRightOutlinedText(x + 27, y + 17, text, 0, 0, 0xFFFFFF, 0, 11, 0);
  }
}

// OFFSET: 0x0092F8E0
// FUNCTION: fn_12680
void UIShortCutWnd::Paint() {
  UIWindow& window = *this;
  const ShortcutGrid& grid = this->m_grid;
  CBitmapRes* frame = Load("유저인터페이스\\Shortcut\\shortcut_frame.bmp");

  int frameRow = 0;
  for (; frameRow < grid.rows; ++frameRow) {
    window.DrawBitmap(0, frameRow * 33, frame, false);
  }

  if (g_session.ItemSlotActive()) {
    window.DrawBitmap(0, frameRow * 33, Load("유저인터페이스\\Shortcut\\shortcut_frame_item.bmp"), false);
  }

  if (grid.row != -1 && grid.hovered != -1) {
    const int x = ((grid.hovered * 29) + 16);
    const int page = g_session.m_hotkeyPage;
    const SkinPaletteCell cell{2, 2};
    window.FillRegion(x, page * 33 + 5, 24, 24, g_skinInfo.PaletteColor(cell, g_skinInfo.Selected()));
  }

  g_session.m_mainCooldownIcons.clear();

  for (int row = 0; row < grid.rows; ++row) {
    for (int column = 0; column < 9; ++column) {
      const int index = ((row * 9) + column);
      if (index >= 36) {
        continue;
      }

      const int y = ((row * 33) + 4);

      const unsigned itemId = this->m_slots[index]->itemId;
      if (itemId == ITEMID_NONE) {
        continue;
      }

      const int x = 16 + 29 * column;
      if (this->m_slots[index]->enabled != 0) {
        CSkillInfo skill = g_session.LookupSkill(itemId);
        if (!this->MatchesShortcut(skill, this->m_slots[index])) {
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
        window.DrawBitmap(x, y, image, true);
        int level = this->m_slots[index]->value;

        if (IsIndexedSkill(skill.id) && skill.level < level) {
          level = skill.level;
        }

        std::sprintf(path, "%d", level);
        window.DrawRightOutlinedText(x + 27, y + 17, path, 0, 0, 0xFFFFFF, 0, 11, 0);
      } else {
        ITEM_INFO item = g_session.LookupClientItem(static_cast<int>(itemId), 1);
        if (item.identity[1] > 0) {
          this->DrawShortcutItem(x, y, item, item.identified);

          if (0 < item.grade) {
            DrawGradeIcon(window, x, y + 12, item);
          }
        }
      }
    }
  }

  if (g_session.ItemSlotActive()) {
    for (int column = 0; column < 9; ++column) {
      const unsigned itemId = this->m_itemSlots[column]->itemId;
      if (itemId == ITEMID_NONE) {
        continue;
      }

      ITEM_INFO item = g_session.LookupClientItem(static_cast<int>(itemId), 1);
      if (item.identity[1] > 0) {
        const int x = 16 + 29 * column;
        const int y = ((grid.rows * 33) + 4);
        this->DrawShortcutItem(x, y, item, item.identified);
      }
    }
  }

  for (int row = 0; row < grid.rows; ++row) {
    DrawRowNumber(window, row);
  }

  const std::uint32_t now = CooldownNow();

  for (auto entry = g_session.m_requestTimers.begin(); entry != g_session.m_requestTimers.end();) {
    if (entry->end > now) {
      for (auto icon = g_session.m_mainCooldownIcons.begin(); icon != g_session.m_mainCooldownIcons.end(); ++icon) {
        if (entry->id == icon->id || (entry->id == 577 && !g_session.IsHomunculusSkill(icon->id))) {
          const auto previous = std::find_if(entry->main.begin(), entry->main.end(), [&](const CooldownState& state) { return state.slot == icon->slot; });
          if (previous == entry->main.end()) {
            entry->main.push_back(CooldownState(icon->slot, icon->x, icon->y, entry->duration, entry->end - entry->duration, 0, 0));
          }
        }

        if ((g_session.IsHomunculusSkill(icon->id) || g_session.IsMercenarySkill(icon->id)) && entry->id == 9999) {
          const auto previous = std::find_if(entry->main.begin(), entry->main.end(), [&](const CooldownState& state) { return state.slot == icon->slot; });
          if (previous == entry->main.end()) {
            entry->main.push_back(CooldownState(icon->slot, icon->x, icon->y, entry->duration, entry->end - entry->duration, 0, 0));
          }
        }
      }

      for (auto state = entry->main.begin(); state != entry->main.end(); ++state) {
        g_cooldownOwner->Draw(0, 0, 12, 12, *state, grid.rows * 33 + 4, window.geometry.x, window.geometry.y);
      }

      ++entry;
    } else {
      entry = g_session.m_requestTimers.erase(entry);
    }
  }
}
