#include "Client/Input/InputCharset.h"
#include "Client/Input/Language.h"
#include "Network/TextEncoding.h"
#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstring>


// OFFSET: 0x00868450
// FUNCTION: fn_10959
void UIEditCtrl::Trim() {
  const auto length = this->text.size();

  if (static_cast<int>(length) <= this->maximum) {
    return;
  }

  const auto codePage = static_cast<WORD>(g_textCodePage);
  const char* start = this->text.c_str();
  const char* next = CharNextExA(codePage, start, 0);

  while (length - std::strlen(next) <= static_cast<unsigned>(this->maximum)) {
    next = CharNextExA(codePage, next, 0);
  }

  const char* previous = CharPrevExA(codePage, start, next, 0);
  this->text.resize(length - std::strlen(previous));

  while (this->text.size() < static_cast<unsigned>(this->maximum)) {
    this->text += " ";
  }
}

// OFFSET: 0x0085A590
// FUNCTION: fn_10778
void UIEditCtrl::SelectAll() {
  if (g_windowMgr.GetHoverWindow() == this) {
    g_serverLanguage->Select(0, static_cast<int>(g_serverLanguage->text.size()));
  } else {
    this->anchor = 0;
    this->cursor = static_cast<int>(this->text.size());
  }
}

