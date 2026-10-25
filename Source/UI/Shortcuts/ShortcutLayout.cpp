#include "UI/Shortcuts/ShortcutLayout.h"
#include "UI/Buttons/UIBitmapButton.h"

void ShortcutLayout::PlaceRowButtons() {
  this->decrease.Move(this->window.geometry.width - this->decrease.geometry.width - 2, this->grid.rows * 33 - 28);
  this->increase.Move(this->window.geometry.width - this->increase.geometry.width - 2, this->grid.rows * 33 - 13);
  this->decrease.Enable(this->grid.rows == 1 ? 0 : 1);
  this->increase.Enable(1);
  this->increase.SetDisabled(this->grid.rows == 4 ? 1 : 0);
  this->increase.Invalidate();
}

// OFFSET: 0x0093B450
// FUNCTION: fn_12760
void ShortcutLayout::ResizeRows() {
  const int height = (this->itemSlotActive ? 43 : 10) + this->grid.rows * 33;
  if (height == this->window.geometry.height) {
    return;
  }

  this->window.Resize(this->window.geometry.width, height);

  this->decrease.Move(this->window.geometry.width - this->decrease.geometry.width - 2, this->grid.rows * 33 - 28);
  this->increase.Move(this->window.geometry.width - this->increase.geometry.width - 2, this->grid.rows * 33 - 13);
  this->showItems.Move(247, height - 10);
  this->hideItems.Move(247, height - 10);
  this->decrease.Enable(this->grid.rows == 1 ? 0 : 1);
  this->increase.Enable(1);
  this->increase.SetDisabled(this->grid.rows == 4 ? 1 : 0);
  this->increase.Invalidate();
  this->window.Slot11();
}

// OFFSET: 0x0093B450
// FUNCTION: fn_12760
void ShortcutLayout::Restore(NormalItemSlotWindow* value) {
  this->saved = value;
  if (!value->IsValid(this->screenWidth, this->screenHeight, this->itemSlotActive)) {
    value->Reset();
  }

  const int height = value->height;
  this->window.Resize(this->window.geometry.width, height);
  const int extent = this->itemSlotActive ? height - 43 : height;
  this->showItems.Move(247, height - 10);
  this->hideItems.Move(247, height - 10);
  this->showItems.Enable(this->itemSlotActive ? 0 : 1);
  this->showItems.SetDisabled(this->itemSlotActive ? 1 : 0);
  this->showItems.Invalidate();
  this->hideItems.Enable(this->itemSlotActive ? 1 : 0);
  this->hideItems.SetDisabled(this->itemSlotActive ? 0 : 1);
  this->hideItems.Invalidate();
  this->grid.rows = extent / 33;
  this->PlaceRowButtons();
  this->window.SendMsg(nullptr, 23, 0, 0, 0, 0);
  this->window.Move(value->x, value->y);
}

// OFFSET: 0x00940FE0
// FUNCTION: fn_12787
void ShortcutLayout::Persist() {
  if (!this->saved) {
    return;
  }

  this->saved->x = this->window.geometry.x;
  this->saved->y = this->window.geometry.y;
  this->saved->width = this->window.geometry.width;
  this->saved->height = this->window.geometry.height;
}
