#include "Client/ClientOptions.h"
#include "UI/ShortcutControlEvents.h"
#include "UI/ShortcutCooldowns.h"
#include "UI/ShortcutVisibility.h"
#include "UI/UIBitmapButton.h"
#include "UI/UICheckBox.h"
#include "UI/UIWindow.h"

#include <bit>

// OFFSET: 0x0093B450
// FUNCTION: fn_12760
void ShortcutControlEvents::Click(int id) {
  switch (id) {
  case 201:
    this->visibility.Show(0);
    return;

  case 216:
    if (this->window.geometry.visible == 0) {
      this->visibility.Show(1);
      return;
    }

    this->grid.rows = std::bit_cast<int>(static_cast<unsigned>(this->grid.rows) + 1u);

    if (this->grid.rows > 4) {
      this->grid.rows = 1;
      this->visibility.Show(0);
    }

    this->window.SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;

  case 217:
    if (this->grid.rows > 1) {
      --this->grid.rows;
    }

    this->window.SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;

  case 549: {
    const bool checked = this->options.Get(10) == 0;
    this->options.Set(10, checked ? 1 : 0, 0);
    this->controls.bank->geometry.unknown_0030 = checked ? 1 : 0;
    this->controls.bank->Invalidate();

    for (ShortcutCooldownRecord& record : this->cooldowns.records) {
      record.main.clear();
    }

    this->window.SendMsg(nullptr, 23, 0, 0, 0, 0);
    return;
  }

  case 588:
  case 589: {
    this->itemSlotActive = id == 588;
    const bool active = this->itemSlotActive;

    this->controls.showItems->Enable(active ? 0 : 1);
    this->controls.showItems->SetDisabled(active ? 1 : 0);
    this->controls.showItems->Invalidate();

    this->controls.hideItems->Enable(active ? 1 : 0);
    this->controls.hideItems->SetDisabled(active ? 0 : 1);
    this->controls.hideItems->Invalidate();

    this->window.SendMsg(nullptr, 14, 0, 0, 0, 0);
    return;
  }

  default:
    return;
  }
}
