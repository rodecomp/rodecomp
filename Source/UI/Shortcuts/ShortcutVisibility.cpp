#include "UI/Shortcuts/ShortcutVisibility.h"

// OFFSET: 0x00AAA310
// FUNCTION: fn_17269
bool WindowVisibility::Show(int id, int visible) {
  switch (id) {
  case 1: {
    if (this->restricted && !this->replayAllowsBasicInfo) {
      return false;
    }

    this->basicInfoVisible = visible;

    if (!this->basicInfo) {
      this->windows.EnsureWindow(1);
    }

    this->basicInfo->Show(visible);
    UIWindow* window = this->basicInfo;

    if (visible == 0) {
      window->Slot11();
      return true;
    }

    this->ordering.Promote(window);
    return true;
  }

  case 15:
    this->windows.EnsureWindow(15);

    if (this->window15->geometry.visible != visible) {
      this->window15->Show(visible);
    }

    if (visible != 0) {
      this->ordering.Promote(this->window15);
    }

    return false;

  case 36:
    return this->shortcut.Show(visible);

  case 46: {
    this->window46Visible = visible;

    if (!this->window46) {
      if (visible == 0) {
        return true;
      }

      this->windows.EnsureWindow(46);

      if (!this->window46) {
        return true;
      }
    }

    this->window46->Show(visible);
    UIWindow* window = this->window46;

    if (visible == 0) {
      window->Slot11();
      return true;
    }

    this->ordering.Promote(window);
    return true;
  }

  case 273:
    this->windows.LookupWindow(273)->Show(visible);
    return true;

  case 276:
    this->window276Visible = visible;

    if (!this->window276) {
      if (visible == 0) {
        return true;
      }

      this->windows.EnsureWindow(276);

      if (!this->window276) {
        return true;
      }
    }

    this->window276->Show(visible);
    return true;
  }

  return false;
}
