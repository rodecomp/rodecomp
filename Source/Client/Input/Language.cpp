#include "Client/Input/Language.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <imm.h>

// OFFSET: 0x00ACF2A0
// FUNCTION: fn_17794
void CLanguage::ResetInput() {
  HIMC context = ImmGetContext(this->window);

  if (context) {
    ImmNotifyIME(context, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
    ImmReleaseContext(this->window, context);
  }

  this->text.clear();
  this->cursor = 0;
  this->anchor = 0;
  this->composition.clear();
  this->candidate.clear();
  this->compositionState = 0;
}

// OFFSET: 0x00ACD700
// FUNCTION: fn_17767
void CLanguage::DeleteSelection() {
  if (this->cursor == this->anchor) {
    return;
  }

  const int begin = std::min(this->cursor, this->anchor);
  const int length = std::max(this->cursor, this->anchor) - begin;
  this->text.erase(static_cast<std::size_t>(begin), length);
  this->cursor = begin;
  this->anchor = begin;

  if (this->suggestionsEnabled && this->chatOpen && this->chatInput && this->focused == this->chatInput) {
    this->suggestions.Update(this->text.c_str());
  }
}

// OFFSET: 0x00ACD360
// FUNCTION: fn_17760
void CLanguage::Insert(const char* value) {
  this->DeleteSelection();
  this->text.insert(static_cast<std::size_t>(this->cursor), value, std::strlen(value));
  this->anchor = this->cursor + static_cast<int>(std::strlen(value));
  this->cursor = this->anchor;

  if (this->suggestionsEnabled && this->chatOpen && this->chatInput && this->focused == this->chatInput) {
    this->suggestions.Update(this->text.c_str());
  }
}

// OFFSET: 0x00ACF590
// FUNCTION: fn_17797
void CLanguage::Select(int anchor, int cursor) {
  this->anchor = anchor;
  this->cursor = cursor;
  this->anchor = std::clamp(this->anchor, 0, static_cast<int>(this->text.size()));
  this->cursor = std::clamp(this->cursor, 0, static_cast<int>(this->text.size()));
}

// OFFSET: 0x00ACE3C0
// FUNCTION: fn_17782
void CLanguage::CopyDisplayText(std::string& output) const {
  if (&output != &this->text) {
    output = this->text;
  }

  if (!this->candidate.empty()) {
    output.insert(static_cast<std::size_t>(this->cursor), this->candidate.c_str(), std::strlen(this->candidate.c_str()));
  }
}

// OFFSET: 0x00ACE420
// FUNCTION: fn_17783
void CLanguage::Selection(int& anchor, int& cursor) const {
  anchor = this->anchor;
  cursor = this->cursor;
}

// OFFSET: 0x00ACDD10
// FUNCTION: fn_17774
void __stdcall SubACDD10() {
}

// OFFSET: 0x00ACDD20
// FUNCTION: fn_17775
void CLanguage::RefreshComposition(unsigned codePage) {
  HIMC context = ImmGetContext(this->window);
  if (!context) {
    this->candidate.clear();
    return;
  }

  WCHAR wide[256];
  char converted[256];
  const auto bytes = static_cast<std::uint32_t>(ImmGetCompositionStringW(context, GCS_COMPSTR, wide, sizeof(wide)));
  const unsigned size = WideCharToMultiByte(codePage, 0, wide, bytes >> 1, converted, sizeof(converted), nullptr, nullptr);
  ImmReleaseContext(this->window, context);
  this->candidate.assign(converted, size);

  char combined[256];
  const std::size_t committed = std::min<std::size_t>(this->text.size(), 255);
  std::memcpy(combined, this->text.data(), committed);
  const std::size_t pending = std::min<std::size_t>(this->candidate.size(), 255 - committed);
  std::memcpy(combined + committed, this->candidate.data(), pending);
  combined[committed + pending] = 0;

  if (this->suggestionsEnabled && this->chatOpen && this->chatInput && this->focused == this->chatInput) {
    this->suggestions.Update(combined);
  }
}

// OFFSET: 0x00ACD5C0
// FUNCTION: fn_17765
void CLanguage::CopySelection(bool password) {
  const int length = std::max(this->cursor, this->anchor) - std::min(this->cursor, this->anchor);
  if (!length || !OpenClipboard(this->window)) {
    return;
  }

  HGLOBAL memory = GlobalAlloc(GMEM_DDESHARE, static_cast<std::uint32_t>(length + 1));
  if (memory) {
    auto* data = static_cast<char*>(GlobalLock(memory));
    if (data) {
      const int begin = std::min(this->cursor, this->anchor);
      if (password) {
        std::memset(data, '*', static_cast<std::uint32_t>(this->text.size()) - static_cast<std::uint32_t>(begin));
      } else {
        std::memcpy(data, this->text.data() + begin, length);
      }

      data[length] = 0;
      GlobalUnWire(memory);
      EmptyClipboard();
      SetClipboardData(CF_TEXT, memory);
    }
  }

  CloseClipboard();
}

// OFFSET: 0x00ACD540
// FUNCTION: fn_17764
void CLanguage::Paste(UIWindow* blockedFocus, WindowManagerWindows& windows) {
  if (this->focused == blockedFocus || windows.LookupWindow(278) || windows.LookupWindow(128)) {
    return;
  }

  if (!OpenClipboard(this->window)) {
    return;
  }

  HANDLE memory = GetClipboardData(CF_TEXT);
  if (memory) {
    const char* data = static_cast<const char*>(GlobalLock(memory));
    if (data) {
      this->Insert(data);
      GlobalUnWire(memory);
    }
  }

  CloseClipboard();
}

// OFFSET: 0x00ACD6B0
// FUNCTION: fn_17766
void CLanguage::MoveBack(unsigned codePage) {
  if (this->cursor < 0) {
    this->cursor = 0;
  } else if (this->cursor != 0) {
    const char* begin = this->text.c_str();
    this->cursor = static_cast<int>(CharPrevExA(static_cast<WORD>(codePage), begin, begin + this->cursor, 0) - begin);
  }
}

// OFFSET: 0x00ACE440
// FUNCTION: fn_45441
void CLanguage::SelectionRange(int& begin, int& end) const {
  if (this->anchor < this->cursor) {
    begin = this->anchor;
    end = this->cursor;
  } else {
    begin = this->cursor;
    end = this->anchor;
  }
}

// OFFSET: 0x00ACE4B0
// FUNCTION: fn_17786
void CLanguage::SetFlag8C(unsigned char value) {
  reinterpret_cast<unsigned char*>(this)[0x8C] = value;
}
// OFFSET: 0x00ACE4C0
// FUNCTION: fn_17787
void CLanguage::IncreaseSelection(unsigned codePage) {
  const auto length = static_cast<std::uint32_t>(this->text.size());
  const auto position = static_cast<std::uint32_t>(this->cursor);
  if (position > length) {
    this->cursor = static_cast<int>(length);
  } else if (position < length) {
    const char* begin = this->text.c_str();
    this->cursor = static_cast<int>(CharNextExA(static_cast<WORD>(codePage), begin + position, 0) - begin);
  }
}
// OFFSET: 0x00ACE5E0
// FUNCTION: fn_17789
void CLanguage::AdvanceCursor(int delta) {
  const unsigned length = static_cast<unsigned>(this->text.size());
  unsigned cursor = static_cast<unsigned>(this->cursor);
  if (cursor == length) {
    return;
  }
  if (cursor > length) {
    this->cursor = static_cast<int>(length);
  } else {
    unsigned next = cursor + static_cast<unsigned>(delta);
    if (next <= length) {
      this->cursor = static_cast<int>(next);
    }
  }
  if ((static_cast<unsigned short>(GetKeyState(VK_SHIFT)) >> 8) == 0) {
    this->anchor = this->cursor;
  }
}

// OFFSET: 0x00ACE620
// FUNCTION: fn_17790
void CLanguage::RetreatCursor(int delta) {
  if (this->cursor == 0) {
    return;
  }
  if (this->cursor < 0) {
    this->cursor = 0;
  } else {
    int prev = this->cursor - delta;
    if (prev >= 0) {
      this->cursor = prev;
    }
  }
  if ((static_cast<unsigned short>(GetKeyState(VK_SHIFT)) >> 8) == 0) {
    this->anchor = this->cursor;
  }
}

// OFFSET: 0x00ACE660
// FUNCTION: fn_17791
void CLanguage::Character(unsigned value, LPARAM, unsigned codePage) {
  if (value == 8) {
    if (this->cursor == this->anchor) {
      this->MoveBack(codePage);
    }

    this->DeleteSelection();
    return;
  }

  if (value <= 31) {
    return;
  }

  char bytes[sizeof(value)];
  std::memcpy(bytes, &value, sizeof(value));
  this->Insert(bytes);
}

// OFFSET: 0x00ACE880
// FUNCTION: fn_17792
void CLanguage::KeyDown(
  unsigned key, LPARAM, unsigned codePage, bool password, UIWindow* blockedPasteFocus, WindowManagerWindows& windows, UIWindow* const& chatArrows
) {
  const auto cut = [&]() {
    this->CopySelection(password);
    this->DeleteSelection();
  };

  if ((static_cast<unsigned short>(GetKeyState(VK_SHIFT)) >> 8) != 0) {
    switch (key) {
    case VK_LEFT:
      this->MoveBack(codePage);
      break;
    case VK_RIGHT:
      this->IncreaseSelection(codePage);
      break;
    case VK_HOME:
      this->cursor = 0;
      break;
    case VK_END:
      this->cursor = static_cast<int>(this->text.size());
      break;
    case VK_INSERT:
      this->Paste(blockedPasteFocus, windows);
      break;
    case VK_DELETE:
      cut();
      break;
    }

    return;
  }

  if ((static_cast<unsigned short>(GetKeyState(VK_CONTROL)) >> 8) != 0) {
    switch (key) {
    case 'X':
      cut();
      break;
    case 'C':
      this->CopySelection(password);
      break;
    case 'V':
      this->Paste(blockedPasteFocus, windows);
      break;
    case 'A':
      this->anchor = 0;
      this->cursor = std::max(static_cast<int>(this->text.size()), 0);
      break;
    }

    return;
  }

  switch (key) {
  case VK_DELETE:
    if (this->cursor == this->anchor) {
      this->IncreaseSelection(codePage);
    }
    this->DeleteSelection();
    break;
  case VK_LEFT:
  case VK_RIGHT:
    if (chatArrows && chatArrows->geometry.visible != 0) {
      chatArrows->SendMsg(nullptr, 176, key, 0, 0, 0);
    } else {
      if (key == VK_LEFT) {
        this->MoveBack(codePage);
      } else {
        this->IncreaseSelection(codePage);
      }

      this->anchor = this->cursor;
    }
    break;
  case VK_HOME:
    this->cursor = 0;
    this->anchor = 0;
    break;
  case VK_END:
    this->cursor = static_cast<int>(this->text.size());
    this->anchor = this->cursor;
    break;
  }
}
