#include "Client/ModeEvents.h"
#include "UI/ShortcutUse.h"
#include "UI/UIButton.h"

#include <bit>
#include <cstdio>
#include <cstdlib>

// Message 41 branch.
// OFFSET: 0x0093B450
// FUNCTION: fn_12760
void ShortcutUse::Use(const NormalItemSlot& slot) {
  if (this->options.Get(5) > 0 || slot.itemId == 0) {
    return;
  }

  if (slot.unknown_0000 != 0) {
    CSkillInfo skill = this->skills.Lookup(slot.itemId);
    if (skill.MatchesShortcut(&slot)) {
      int level = std::bit_cast<std::int16_t>(slot.unknown_0005);
      if ((skill.id == 6579 || skill.id == 6580) && skill.level < level) {
        level = skill.level;
      }

      this->mode->SendModeMessage(113, reinterpret_cast<std::intptr_t>(&skill), level, 0, 0);
    }

    return;
  }

  ItemInfo item = LookupClientItem(this->inventory, std::bit_cast<int>(slot.itemId), 1);
  if (item.unknown_0000[1] == 0) {
    return;
  }

  ModeEvents* mode = this->mode;

  const auto use = [&]() { mode->SendModeMessage(27, item.unknown_0000[1], 0, 0, 0); };

  switch (item.unknown_0000[0]) {
  case 0:
  case 1:
  case 2:
    if (
      std::atoi(item.itemId.c_str()) != 12802 ||
      this->dialogs.Confirm(this->naming.Message(2373), 2, 0, 0, "Message", 280, 120) != 188
    ) {
      use();
    }

    break;

  case 10:
  case 16:
  case 17:
  case 19:
    if (item.unknown_0000[3] == 0x8000) {
      mode->SendModeMessage(87, item.unknown_0000[1], item.unknown_0000[2], 0, 0);
    }

    break;

  case 18:
    if (this->itemConfirmationState >= 0) {
      const std::string name = this->naming.DisplayName(item, 0);
      char text[4096];

      _snprintf_s(text, sizeof(text), _TRUNCATE, this->naming.Message(1226), name.c_str());

      if (this->dialogs.Confirm(text, 2, 0, 0, "Message", 280, 120) == 187) {
        use();
      }
    }

    break;

  default:
    if (item.unknown_0000[2] != 0 && item.unknown_0000[3] == 0 && item.identified != 0) {
      mode->SendModeMessage(19, item.unknown_0000[1], item.unknown_0000[2], 0, 0);
    }

    break;
  }
}
