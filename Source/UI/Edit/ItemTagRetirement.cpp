#include "UI/Window/InputFocus.h"
#include "UI/Edit/ItemTagRetirement.h"
#include "UI/Window/UIWindow.h"

// INLINED
FocusedItemTagRemoval::FocusedItemTagRemoval(InputFocus& focus) : m_focus(focus) {
}

// OFFSET: 0x00896EB0
// FUNCTION: fn_11519
void FocusedItemTagRemoval::Remove(UIWindow* child) {
  if (this->m_focus.window) {
    this->m_focus.window->RemoveChild(child, 1);
  }
}

// OFFSET: 0x0087A2F0
// FUNCTION: fn_11231
void ItemTagRetirement::Clear() {
  for (auto entry = this->records.begin(); entry != this->records.end();) {
    this->removal.Remove(entry->child);
    entry = this->records.erase(entry);
  }
}
