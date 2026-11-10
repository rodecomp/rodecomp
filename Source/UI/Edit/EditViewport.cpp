#include "Client/Input/InputCharset.h"
#include "Client/Input/Language.h"
#include "Network/TextEncoding.h"
#include "UI/Edit/UIEditCtrl.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <cstring>

// OFFSET: 0x00862FB0
// FUNCTION: fn_10905
void UIEditCtrl::Synchronize() {
  const int previousCursor = this->cursor;
  g_serverLanguage->CopyDisplayText(this->text);
  g_serverLanguage->Selection(this->anchor, this->cursor);
  this->Trim();
  bool clamped = false;

  if (static_cast<std::uint32_t>(this->cursor) > this->text.size() || static_cast<std::uint32_t>(this->anchor) > this->text.size()) {
    this->anchor = static_cast<int>(std::min(static_cast<std::uint32_t>(this->anchor), this->text.size()));
    this->cursor = static_cast<int>(std::min(static_cast<std::uint32_t>(this->cursor), this->text.size()));
    g_serverLanguage->Select(this->anchor, this->cursor);
    clamped = true;
  }

  if (this->cursor == 0) {
    this->firstVisible = 0;
    return;
  }

  std::string visibleText;
  const auto selection = static_cast<std::uint32_t>(std::abs(g_serverLanguage->cursor - g_serverLanguage->anchor));

  if (selection >= this->text.size()) {
    this->firstVisible = 0;
  }

  const auto measure = [&](const char* text, unsigned bytes) { return this->TextWidth(text, bytes, 0, 12, 0, 0); };

  if (this->cursor > previousCursor || clamped) {
    int before = 0;
    if (this->firstVisible != 0) {
      before = measure(this->text.c_str(), static_cast<unsigned>(this->firstVisible));
    }
    const int position = measure(this->text.c_str(), static_cast<unsigned>(this->cursor)) + 3 - before;

    if (position > this->geometry.width - 7) {
      const char* end = this->text.c_str() + this->text.size();
      const char* next = CharNextExA(static_cast<WORD>(g_textCodePage), this->text.c_str(), 0);

      while (measure(next, 0) + 3 > this->geometry.width - 7 && next < end) {
        next = CharNextExA(static_cast<WORD>(g_textCodePage), next, 0);
      }

      const auto tail = static_cast<std::uint32_t>(std::strlen(next));
      const int candidate = this->firstVisible + this->cursor - previousCursor;
      this->firstVisible = static_cast<int>(std::min(static_cast<std::uint32_t>(candidate), static_cast<std::uint32_t>(this->cursor) - tail));
    }
  } else if (this->cursor < previousCursor) {
    if (measure(this->text.c_str(), static_cast<unsigned>(this->cursor)) + 3 < this->geometry.width - 7 && this->firstVisible > this->cursor) {
      this->firstVisible += this->cursor - previousCursor;
    }

    this->firstVisible = std::max(this->firstVisible, 0);
  }
}
