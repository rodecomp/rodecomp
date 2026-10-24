#include "Client/ClientOptions.h"
#include "Contents/AutomaticSkills.h"
#include "UI/ShortcutActivation.h"
#include "UI/UIWindow.h"

#include <bit>

// OFFSET: 0x00930900
// FUNCTION: fn_12689
void ActivateShortcut(
  UIWindow& window,
  ShortcutGrid& grid,
  int x,
  int y,
  bool itemSlotActive,
  unsigned char& currentRow,
  const NormalItemSlot* const* slots,
  AutomaticSkills& automatic,
  ClientOptions& options
) {
  const int column = grid.HitTest(x, y, itemSlotActive, currentRow);
  if (column == -1) {
    return;
  }
  const unsigned index = 9u * currentRow + static_cast<unsigned>(column);
  const int limit = std::bit_cast<int>(9u * static_cast<unsigned>(grid.rows));
  if (limit <= std::bit_cast<int>(index)) {
    return;
  }
  const NormalItemSlot& slot = *slots[index];
  if (slot.unknown_0000 != 0 && automatic.Contains(static_cast<std::uint16_t>(slot.itemId))) {
    options.Set(
      AutomaticSkills::CancelOption(std::bit_cast<std::int16_t>(static_cast<std::uint16_t>(slot.itemId))), 1, 0
    );
  } else {
    automatic.DisableAll();
  }
  window.SendMsg(nullptr, 41, column, currentRow, 0, 0);
}
