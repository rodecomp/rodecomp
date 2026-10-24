#include "UI/ChatSuggestions.h"
#include "UI/InputFocus.h"
#include "UI/ItemTagRetirement.h"
#include "UI/UIEditCtrl.h"
#include "UI/UIWindow.h"
#include "UI/WindowCoordinates.h"
#include "UI/WindowManagerWindows.h"

// OFFSET: 0x0091E7E0
// FUNCTION: fn_12628
bool ChatSuggestions::Visible() const {
  return this->window && this->window->geometry.visible != 0;
}

// OFFSET: 0x0093D270
// FUNCTION: fn_12767
void ChatSuggestions::Show(std::uint8_t visible) {
  UIWindow* child = this->window;
  if (!child) {
    return;
  }

  child->Show(visible);
  if (visible != 0) {
    this->chat.RemoveChild(child, 0);
    if (!this->chat.FindUid(child->Uid())) {
      this->chat.AddChild(child);
    }
  }
}

// OFFSET: 0x00933980
// FUNCTION: fn_12732
void ChatInputLayout::Apply() {
  if (this->open != 0) {
    const int base = this->chat.geometry.height - this->inputHeight;
    (*this->children[0])->Move(6, base + 4);
    (*this->children[1])->Move(110, base + 4);
    UIWindow* child = *this->children[2];
    child->Move(3, this->chat.geometry.height - child->geometry.height);
    (*this->children[3])->Move(97, base + 3);
    (*this->children[4])->Move(586, base + 7);
    (*this->children[5])->Move(572, base + 7);
  } else {
    for (UIWindow** child : this->children) {
      (*child)->Move(-100, -100);
    }
  }

  this->chat.Invalidate();
}

// OFFSET: 0x009337F0
// FUNCTION: fn_12730
void ChatHeaderLayout::Apply() {
  if (this->enabled == 0 && this->mode != 0) {
    (*this->buttons[0])->Move(-100, -100);
    (*this->buttons[1])->Move(-100, -100);
    (*this->buttons[2])->Move(-100, -100);
    UIWindow* button = *this->buttons[3];
    button->Move(this->width - button->geometry.width - 2, 4);
    return;
  }

  if (this->mode == 0) {
    for (UIWindow** button : this->buttons) {
      (*button)->Move(-100, -100);
    }
    return;
  }

  UIWindow* button = *this->buttons[0];
  int x = this->width - button->geometry.width - 2;
  button->Move(x, 4);

  for (unsigned index = 1; index < this->buttons.size(); ++index) {
    button = *this->buttons[index];
    x -= (button->geometry.width + 2);
    button->Move(x, 4);
  }
}

// OFFSET: 0x0093D2C0
// FUNCTION: fn_12768
void ChatRowLayout::Resize(int count) {
  if (this->popup26) {
    this->windows.CloseWindow(26);
  }

  UIWindow& chat = this->input.chat;
  if (this->capture == &chat) {
    return;
  }

  const int rowCount = count > 15 ? 0 : count < 0 ? 15 : count;
  chat.geometry.unknown_0030 = static_cast<unsigned>(rowCount);
  chat.Invalidate();
  const int previousHeight = chat.geometry.height;
  const int height =
    rowCount == 0 ? this->input.inputHeight : this->input.inputHeight + (rowCount + 3) * 14 + this->headerHeight;
  chat.Resize(chat.geometry.width, height);
  chat.Move(chat.geometry.x, chat.geometry.y - height + previousHeight);

  if (rowCount != 0) {
    for (std::size_t index = 0; index < this->rows.size(); ++index) {
      UIWindow* row = this->rows[index];
      row->Resize(chat.geometry.width - 8, height - this->headerHeight - this->input.inputHeight - 4);
      this->rows[index]->Invalidate();
      row = this->rows[index];
      if (index == static_cast<unsigned>(this->selected)) {
        row->Move(5, this->headerHeight + 2);
      } else {
        row->Move(-100 - chat.geometry.width, -100);
      }
    }

    chat.Invalidate();
  } else {
    for (std::size_t index = 0; index < this->rows.size(); ++index) {
      this->rows[index]->Move(-100 - chat.geometry.width, -100);
    }
  }

  (*this->input.children[0])->Move(6, chat.geometry.height - this->input.inputHeight + 4);
  (*this->input.children[1])->Move(110, chat.geometry.height - this->input.inputHeight + 4);
  UIWindow* child = *this->input.children[2];
  child->Move(3, chat.geometry.height - child->geometry.height);

  if (rowCount != 0) {
    (*this->header.buttons[2])->Move(this->headerCellWidth * 2 + 4, 4);
    (*this->header.buttons[3])->Move(this->headerCellWidth * 2 + 19, 4);
    this->headerChild->Move(3, 0);
  } else {
    (*this->header.buttons[2])->Move(-100, -100);
    (*this->header.buttons[3])->Move(-100, -100);
    this->headerChild->Move(-100, -100);
  }

  // These three positions are snapshotted together before the callbacks.
  const int base = chat.geometry.height - this->input.inputHeight;
  const int firstY = base + 3;
  const int lastY = base + 7;
  (*this->input.children[3])->Move(97, firstY);
  (*this->input.children[4])->Move(586, lastY);
  (*this->input.children[5])->Move(572, lastY);

  if (this->suggestions) {
    UIWindow* edit = *this->input.children[1];
    this->suggestions->Move(edit->geometry.x, edit->geometry.y - this->suggestions->geometry.height);
  }

  this->header.Apply();
  this->input.Apply();
}

// OFFSET: 0x00916170
// FUNCTION: fn_12582
void ChatInputFocus::Toggle() {
  if (this->open != 0 && this->alternate != 0) {
    if (*this->secondary->Text() == '\0') {
      this->open = 0;
      this->layout.Apply();
      SetPressedWindowRoot(this->pressedRoot, nullptr);
    }
  } else {
    this->open = 1;
    this->layout.Apply();
    this->focus.Set(this->primary);
    this->primary->SetText("");

    if (this->alternate == 0) {
      this->rows.Resize(static_cast<int>(this->layout.chat.geometry.unknown_0030));
    }

    SetPressedWindowRoot(this->pressedRoot, this->primary);
  }

  if (this->tags) {
    this->tags->Clear();
  }
}

// OFFSET: 0x00AA4730
// FUNCTION: fn_17222
// Outside-click suggestion branch of mouse message dispatch.
void ChatInputFocus::DismissOutside(ChatSuggestions& suggestions, UIWindow* companion, int button, int x, int y) {
  if (button != 1 || !suggestions.Visible()) {
    return;
  }

  int left;
  int top;
  int right;
  int bottom;
  WindowScreenBounds(suggestions.window, left, top, right, bottom);
  const bool outside = x < left || x > right || y < top || y > bottom;
  WindowScreenBounds(companion, left, top, right, bottom);
  const bool outsideCompanion = x < left || x > right || y < top || y > bottom;

  if (outside && outsideCompanion) {
    suggestions.Show(0);
    if (this->secondary) {
      this->secondary->SetText("");
    }
    this->Toggle();
  }
}
