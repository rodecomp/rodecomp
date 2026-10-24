#include "Client/TextInputBuffer.h"
#include "UI/EditTextState.h"

#include <cstring>

// OFFSET: 0x008680D0
// FUNCTION: fn_10952
void EditTextState::SetText(const char* value) {
  this->text.assign(value, std::strlen(value));
  this->Trim();

  if (this->focused) {
    this->input.CancelComposition();
    this->input.CancelComposition();
    this->input.Insert(this->text.c_str());
  }

  this->SelectAll();
}

// OFFSET: 0x00868450
// FUNCTION: fn_10959
void EditTextState::Trim() {
  const auto length = this->text.size();

  if (static_cast<int>(length) <= this->maximum) {
    return;
  }

  const auto codePage = static_cast<WORD>(this->codePage);
  const char* start = this->text.c_str();
  const char* next = CharNextExA(codePage, start, 0);

  while (length - std::strlen(next) <= static_cast<unsigned>(this->maximum)) {
    next = CharNextExA(codePage, next, 0);
  }

  const char* previous = CharPrevExA(codePage, start, next, 0);
  this->text.resize(length - std::strlen(previous));

  while (this->text.size() < static_cast<unsigned>(this->maximum)) {
    this->text.push_back(' ');
  }
}

// OFFSET: 0x0085A590
// FUNCTION: fn_10778
void EditTextState::SelectAll() {
  if (this->focused) {
    this->input.Select(0, static_cast<int>(this->input.text.size()));
  } else {
    this->anchor = 0;
    this->cursor = static_cast<int>(this->text.size());
  }
}

// OFFSET: 0x00858D50
// FUNCTION: fn_10768
int EditTextState::Length() const {
  return static_cast<int>(this->focused ? this->input.text.size() : this->text.size());
}
