#include "Client/Input/Language.h"
#include "Render/Text/TextContext.h"
#include "UI/Edit/EditTextState.h"
#include "UI/Edit/EditViewport.h"

#include <algorithm>
#include <cstdint>
#include <cstring>

// OFFSET: 0x00862FB0
// FUNCTION: fn_10905
void EditViewport::Synchronize() {
  const int previousCursor = this->cursor;
  this->input.CopyDisplayText(this->text);
  this->input.Selection(this->anchor, this->cursor);
  this->state.Trim();
  const auto length = static_cast<std::uint32_t>(this->text.size());
  bool clamped = false;

  if (static_cast<std::uint32_t>(this->cursor) > length || static_cast<std::uint32_t>(this->anchor) > length) {
    this->anchor = static_cast<int>(std::min(static_cast<std::uint32_t>(this->anchor), length));
    this->cursor = static_cast<int>(std::min(static_cast<std::uint32_t>(this->cursor), length));
    this->input.Select(this->anchor, this->cursor);
    clamped = true;
  }

  if (this->cursor == 0) {
    this->firstVisible = 0;
    return;
  }

  const auto selection =
    static_cast<std::uint32_t>(std::max(this->input.cursor, this->input.anchor)) - static_cast<std::uint32_t>(std::min(this->input.cursor, this->input.anchor));

  if (selection >= length) {
    this->firstVisible = 0;
  }

  TextContext context(this->surface);
  context.SetFont(0, 12, 0, 0);
  const auto measure = [&](const char* text, unsigned bytes) { return context.Measure(text, static_cast<int>(bytes ? bytes : std::strlen(text))).cx; };
  const int available = this->width - 7;

  if (this->cursor > previousCursor || clamped) {
    const int before = this->firstVisible == 0 ? 0 : measure(this->text.c_str(), static_cast<unsigned>(this->firstVisible));
    const int position = measure(this->text.c_str(), static_cast<unsigned>(this->cursor)) + 3 - before;

    if (position > available) {
      const char* end = this->text.c_str() + this->text.size();
      const char* next = CharNextExA(static_cast<WORD>(this->codePage), this->text.c_str(), 0);

      while (measure(next, 0) + 3 > available && next < end) {
        next = CharNextExA(static_cast<WORD>(this->codePage), next, 0);
      }

      const auto tail = static_cast<std::uint32_t>(std::strlen(next));
      const int candidate = this->firstVisible + this->cursor - previousCursor;
      this->firstVisible = static_cast<int>(std::min(static_cast<std::uint32_t>(this->cursor) - tail, static_cast<std::uint32_t>(candidate)));
    }
  } else if (this->cursor < previousCursor) {
    if (measure(this->text.c_str(), static_cast<unsigned>(this->cursor)) + 3 < available && this->firstVisible > this->cursor) {
      this->firstVisible += this->cursor - previousCursor;
    }

    this->firstVisible = std::max(this->firstVisible, 0);
  }
}
