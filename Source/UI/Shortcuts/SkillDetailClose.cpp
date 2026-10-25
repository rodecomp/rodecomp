#include "UI/Window/FrameFocus.h"
#include "UI/Shortcuts/SkillDetailClose.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowRetirement.h"

// OFFSET: 0x00A8CF20
bool SkillDetailClose::Close() {
  if (this->window) {
    this->saved[46] = {this->window->geometry.x, this->window->geometry.y, this->window->geometry.width, this->window->geometry.height};
  }

  if (!this->modalStack.empty() && this->modalStack.back() != 0) {
    return false;
  }

  if (this->window == this->reference_4F1D4) {
    this->reference_4F1D4 = nullptr;
  }

  bool removed = false;

  if (this->window) {
    this->retirement.Schedule(this->window);
    this->window = nullptr;
    this->visible = 0;
    removed = true;
  }

  this->windowsById.erase(46);

  if (removed) {
    this->focus.RemoveOrder(46);
  }

  return removed;
}
