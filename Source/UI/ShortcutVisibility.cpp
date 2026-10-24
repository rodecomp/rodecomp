#include "UI/ShortcutVisibility.h"
#include "UI/UIWindow.h"
#include "UI/WindowManagerWindows.h"
#include "UI/WindowOrdering.h"

// OFFSET: 0x00AAA310
// FUNCTION: fn_17269
bool ShortcutVisibility::Show(int value) {
  if (this->restricted && !this->replayAllowsShortcut) {
    return false;
  }

  this->visible = value;
  this->windows.EnsureWindow(36);
  this->shortcut->Show(value);

  if (value != 0) {
    this->ordering.Promote(this->shortcut);
  }

  if (this->showCompanion) {
    this->windows.EnsureWindow(159);
  }

  UIWindow* companion = this->windows.LookupWindow(159);

  if (companion) {
    companion->Show(value);

    if (value != 0) {
      this->ordering.Promote(companion);
    }
  }

  return true;
}
