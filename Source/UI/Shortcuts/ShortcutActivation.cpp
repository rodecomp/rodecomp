#include "Client/ClientOptions.h"
#include "Contents/Skills/AutomaticSkills.h"
#include "UI/Shortcuts/ShortcutActivation.h"
#include "UI/Window/UIWindow.h"

// OFFSET: 0x00930900
// FUNCTION: fn_12689
void ActivateShortcut(
  UIWindow& window, ShortcutGrid& grid, int x, int y, bool itemSlotActive, unsigned char& currentRow, const NormalItemSlot* const* slots,
  AutomaticSkills& automatic, ClientOptions& options
) {
  const int column = grid.HitTest(x, y, itemSlotActive, currentRow);
  if (column == -1) {
    return;
  }

  const unsigned index = 9u * currentRow + static_cast<unsigned>(column);
  const int limit = 9 * grid.rows;
  if (limit <= static_cast<int>(index)) {
    return;
  }

  const NormalItemSlot& slot = *slots[index];
  if (slot.unknown_0000 != 0 && automatic.Contains(static_cast<std::uint16_t>(slot.itemId))) {
    options.Set(AutomaticSkills::CancelOption(static_cast<std::int16_t>(slot.itemId)), 1, 0);
  } else {
    automatic.DisableAll();
  }

  window.SendMsg(nullptr, 41, column, currentRow, 0, 0);
}
