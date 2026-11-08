#include "Client/Input/InputCharset.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Client/Application/ApplicationWindow.h"
#include "Client/Input/Language.h"
#include "Contents/CashEmotionMgr.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Network/TextEncoding.h"
#include "Utility/Lexicon/LexiconBoundary.h"
#include "Utility/Text/LanguageCharset.h"
#include "Utility/Text/TextTokens.h"
#include "UI/Chat/UINewChatWnd.h"
#include "UI/Lists/UIMemberListBox.h"
#include "UI/UICaptchaPreviewWnd.h"
#include "UI/UIRoundBoxTooltipWnd.h"
#include "UI/UIMacroDetectorWnd.h"
#include "UI/Window/UINewMakeCharWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "UI/Window/WindowManagerWindows.h"

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <format>
#include <imm.h>

// OFFSET: 0x00ACF2A0
// FUNCTION: fn_17794
void CLanguageBase::ResetInput() {
  HIMC context = ImmGetContext(hWnd);

  if (context) {
    ImmNotifyIME(context, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
    ImmReleaseContext(hWnd, context);
  }

  this->text.clear();
  this->cursor = static_cast<int>(this->text.size());
  this->anchor = static_cast<int>(this->text.size());
  this->composition.clear();
  this->candidate.clear();
  this->candidateCount = 0;
}

// OFFSET: 0x00ACD700
// FUNCTION: fn_17767
void CLanguageBase::DeleteSelection() {
  if (this->cursor == this->anchor) {
    return;
  }

  const int begin = std::min(this->cursor, this->anchor);
  const int length = std::max(this->cursor, this->anchor) - begin;
  this->text.erase(static_cast<std::size_t>(begin), length);
  this->cursor = begin;
  this->anchor = begin;

  if (g_windowMgr.inputSuggestionsEnabled && ChatWindow() && ChatWindow()->m_chatEdit && g_windowMgr.GetHoverWindow() == ChatWindow()->m_chatEdit) {
    g_cashEmotionMgr->ProcessChatCommand(this->text.c_str());
  }
}

// OFFSET: 0x00ACD360
// FUNCTION: fn_17760
void CLanguageBase::Insert(const char* value) {
  this->DeleteSelection();
  this->text.insert(static_cast<std::size_t>(this->cursor), value, std::strlen(value));
  this->anchor = this->cursor + static_cast<int>(std::strlen(value));
  this->cursor = this->anchor;

  SuggestChatCommand(this->text.c_str());
}



// OFFSET: 0x00ACF590
// FUNCTION: fn_17797
void CLanguageBase::Select(int anchor, int cursor) {
  this->anchor = anchor;
  this->cursor = cursor;

  if (anchor < 0) {
    this->anchor = 0;
    anchor = 0;
  }

  if (static_cast<unsigned>(anchor) > this->text.size()) {
    this->anchor = static_cast<int>(this->text.size());
  }

  if (cursor < 0) {
    this->cursor = 0;
    return;
  }

  this->cursor = static_cast<unsigned>(cursor) > this->text.size() ? static_cast<int>(this->text.size()) : cursor;
}

// OFFSET: 0x00ACE3C0
// FUNCTION: fn_17782
void CLanguageBase::CopyDisplayText(std::string& output) const {
  if (&output != &this->text) {
    output = this->text;
  }

  if (!this->candidate.empty()) {
    output.insert(static_cast<std::size_t>(this->cursor), this->candidate.c_str(), std::strlen(this->candidate.c_str()));
  }
}

// OFFSET: 0x00ACE420
// FUNCTION: fn_17783
void CLanguageBase::Selection(int& anchor, int& cursor) const {
  anchor = this->anchor;
  cursor = this->cursor;
}

// OFFSET: 0x00ACDD10
// FUNCTION: fn_17774
void __stdcall SubACDD10() {
}

// OFFSET: 0x00ACD5C0
// FUNCTION: fn_17765
void CLanguageBase::CopySelection() {
  const int length = std::abs(this->cursor - this->anchor);
  if (!length || !OpenClipboard(hWnd)) {
    return;
  }

  HGLOBAL memory = GlobalAlloc(GMEM_DDESHARE, static_cast<std::uint32_t>(std::abs(this->cursor - this->anchor) + 1));
  if (memory) {
    auto* data = static_cast<char*>(GlobalLock(memory));
    if (data) {
      int begin = this->anchor;
      int end = this->cursor;
      if (begin >= end) {
        begin = this->cursor;
        end = this->anchor;
      }

      if (!this->password) {
        std::memcpy(data, this->text.c_str() + begin, end - begin);
      } else {
        std::memset(data, '*', static_cast<std::uint32_t>(this->text.size()) - static_cast<std::uint32_t>(begin));
      }

      data[std::abs(this->cursor - this->anchor)] = 0;
      GlobalUnWire(memory);
      EmptyClipboard();
      SetClipboardData(CF_TEXT, memory);
    }
  }

  CloseClipboard();
}

// OFFSET: 0x00ACD540
// FUNCTION: fn_17764
void CLanguageBase::Paste() {
  if (g_windowMgr.window_4F1FC == g_windowMgr.GetHoverWindow() || g_windowMgr.LookupWindow(WID_NEWMAKECHARWND) || g_windowMgr.LookupWindow(WID_CHANGENAMEWND)) {
    return;
  }

  if (!OpenClipboard(hWnd)) {
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
void CLanguageBase::MoveBack() {
  if (this->cursor != 0) {
    if (this->cursor < 0) {
      this->cursor = 0;
    } else {
      this->cursor = static_cast<int>(
        CharPrevExA(static_cast<WORD>(this->codePage), this->text.c_str(), this->text.c_str() + this->cursor, 0) - this->text.c_str()
      );
    }
  }
}
// OFFSET: 0x00ACE440
// FUNCTION: fn_45441
void CLanguageBase::SelectionRange(int& begin, int& end) const {
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
void CLanguageBase::SetPassword(unsigned char value) {
  this->password = value;
}
// OFFSET: 0x00ACE4C0
// FUNCTION: fn_17787
void CLanguageBase::MoveForward() {
  const unsigned length = static_cast<unsigned>(this->text.size());
  const unsigned position = static_cast<unsigned>(this->cursor);
  if (position == length) {
    return;
  }
  if (position > length) {
    this->cursor = static_cast<int>(length);
    return;
  }
  this->cursor = static_cast<int>(CharNextExA(static_cast<WORD>(this->codePage), this->text.c_str() + position, 0) - this->text.c_str());
}
// OFFSET: 0x00ACE5E0
// FUNCTION: fn_17789
void CLanguageBase::AdvanceCursor(int delta) {
  const unsigned length = static_cast<unsigned>(this->text.size());
  if (static_cast<unsigned>(this->cursor) == length) {
    return;
  }

  if (static_cast<unsigned>(this->cursor) > length) {
    this->cursor = static_cast<int>(length);
  } else if (static_cast<unsigned>(this->cursor) + delta <= length) {
    this->cursor += delta;
  }

  if (!HIBYTE(GetKeyState(VK_SHIFT))) {
    this->anchor = this->cursor;
  }
}

// OFFSET: 0x00ACE620
// FUNCTION: fn_17790
void CLanguageBase::RetreatCursor(int delta) {
  if (this->cursor == 0) {
    return;
  }

  if (this->cursor < 0) {
    this->cursor = 0;
  } else if (this->cursor - delta >= 0) {
    this->cursor -= delta;
  }

  if (!HIBYTE(GetKeyState(VK_SHIFT))) {
    this->anchor = this->cursor;
  }
}

// OFFSET: 0x00ACE660
// FUNCTION: fn_17791
void CLanguageBase::Character(unsigned value, LPARAM) {
  switch (value) {
  case VK_BACK:
    if (this->cursor == this->anchor) {
      this->MoveBack();
    }

    this->DeleteSelection();
    return;
  case VK_TAB:
  case '\n':
  case VK_RETURN:
  case VK_ESCAPE:
    return;
  }

  if (value <= 31) {
    return;
  }

  this->DeleteSelection();
  const char* bytes = reinterpret_cast<const char*>(&value);
  this->text.insert(static_cast<std::size_t>(this->cursor), bytes, std::strlen(bytes));
  this->anchor = this->cursor + static_cast<int>(std::strlen(bytes));
  this->cursor = this->anchor;

  SuggestChatCommand(this->text.c_str());
}

// OFFSET: 0x00ACE880
// FUNCTION: fn_17792
void CLanguageBase::KeyDown(unsigned key, LPARAM) {
  if (static_cast<unsigned char>(static_cast<unsigned short>(GetKeyState(VK_SHIFT)) >> 8)) {
    switch (key) {
    case VK_END:
      this->cursor = static_cast<int>(this->text.size());
      break;
    case VK_HOME:
      this->cursor = 0;
      break;
    case VK_LEFT:
      this->MoveBack();
      break;
    case VK_RIGHT:
      this->MoveForward();
      break;
    case VK_INSERT:
      this->Paste();
      break;
    case VK_DELETE:
      this->CopySelection();
      this->DeleteSelection();
      break;
    }

    return;
  }

  if (static_cast<unsigned char>(static_cast<unsigned short>(GetKeyState(VK_CONTROL)) >> 8)) {
    switch (key) {
    case 'A':
      this->anchor = 0;
      this->cursor = static_cast<int>(this->text.size());
      if (this->cursor < 0) {
        this->cursor = 0;
      }
      break;
    case 'C':
      this->CopySelection();
      break;
    case 'V':
      this->Paste();
      break;
    case 'X':
      this->CopySelection();
      this->DeleteSelection();
      break;
    }

    return;
  }

  switch (key) {
  case VK_END:
    this->cursor = static_cast<int>(this->text.size());
    this->anchor = this->cursor;
    break;
  case VK_HOME:
    this->cursor = 0;
    this->anchor = 0;
    break;
  case VK_LEFT:
    if (ChatWindow() && ChatWindow()->SuggestionsVisible() && ChatWindow()->m_suggestions) {
      ChatWindow()->m_suggestions->SendMsg(nullptr, UM_CASH_EMOTION_KEY, VK_LEFT, 0, 0, 0);
      return;
    }

    this->MoveBack();
    this->anchor = this->cursor;
    break;
  case VK_RIGHT:
    if (ChatWindow() && ChatWindow()->SuggestionsVisible() && ChatWindow()->m_suggestions) {
      ChatWindow()->m_suggestions->SendMsg(nullptr, UM_CASH_EMOTION_KEY, VK_RIGHT, 0, 0, 0);
      return;
    }

    this->MoveForward();
    this->anchor = this->cursor;
    break;
  case VK_DELETE:
    if (this->cursor == this->anchor) {
      this->MoveForward();
    }

    this->DeleteSelection();
    break;
  }
}

// OFFSET: 0x00ACEEB0
// FUNCTION: fn_45450
void SuggestChatCommand(const char* text) {
  if (text && g_windowMgr.inputSuggestionsEnabled && ChatWindow() && ChatWindow()->m_chatEdit &&
      g_windowMgr.GetHoverWindow() == ChatWindow()->m_chatEdit) {
    g_cashEmotionMgr->ProcessChatCommand(text);
  }
}

// OFFSET: 0x00ACFCE0
// FUNCTION: fn_17806
int SubACFCE0() {
  return 0;
}

// OFFSET: 0x007D9960
// FUNCTION: fn_09607
UIHairBitmap::UIHairBitmap() : UIBitmapButton() {
}

// OFFSET: 0x007ECEC0
// FUNCTION: fn_09804
UICaptchaPreviewWnd::UICaptchaPreviewWnd() : UIFrameWnd(0), m_image(nullptr), m_imageData(nullptr), m_imageSize(0), m_imageReceived(0) {
  std::memset(this->m_captchaKey, 0, sizeof(this->m_captchaKey));
}

// OFFSET: 0x0086C7B0
// FUNCTION: fn_11030
UIChatHisBox2::UIChatHisBox2()
    : UIChatHisBox(), backgroundRed(255), backgroundGreen(255), backgroundBlue(255) {
}

// OFFSET: 0x0086C830
// FUNCTION: fn_11031
UIChatHisBox3::UIChatHisBox3() : UIChatHisBox2(), autoScroll(1), fontHeight(12) {
}

// OFFSET: 0x00999FF0
// FUNCTION: fn_13536
UIColoringTextViewer::UIColoringTextViewer() : UIListBox(), color(0), lineLimit(0) {
}

// OFFSET: 0x007ECF50
// FUNCTION: fn_09805
UIMacroDetectorWnd::UIMacroDetectorWnd()
    : UIFrameWnd(0), m_button(nullptr), m_imageData(nullptr), m_imageSize(0), m_imageReceived(0), remainingAttempts(0), totalDuration(0),
      startTick(0), messageDuration(0), m_lastAnswer(), message(), lastCountdownTick(0), messageTick(3000), m_image(nullptr), m_editCtrl(nullptr) {
  std::memset(this->m_captchaKey, 0, sizeof(this->m_captchaKey));
  g_windowMgr.CloseWindow(WID_ESCOPTIONWND);
  g_windowMgr.CloseWindow(WID_CHOOSEWARPWND);
}
// OFFSET: 0x0099A060
// FUNCTION: fn_13537
UIRoundBoxTooltipWnd::UIRoundBoxTooltipWnd() : UIFrameWnd(0), viewer(nullptr) {
}

// OFFSET: 0x00ACC7F0
// FUNCTION: fn_17750
CLanguageBase::CLanguageBase()
    : unknown_0004(), candidateList(nullptr), candidateCount(0), pageSize(0), selection(0),
      candidateStorage(), text(), cursor(0), anchor(0), composition(), candidate(),
      language(static_cast<std::uint16_t>(reinterpret_cast<std::uintptr_t>(GetKeyboardLayout(0)))), unknown_0094(0),
      unknown_0098(0), servicePrefix() {
  this->codePage = g_inlineCharset ? LanguageCodePage(this->language) : g_textCodePage;

  switch (static_cast<std::uint16_t>(GetUserDefaultLangID() & 0x3FF)) {
  case LANG_GERMAN:
    this->alternateLanguage = 7;
    this->servicePrefix = "german\\";
    break;
  case LANG_ITALIAN:
    this->alternateLanguage = 16;
    this->servicePrefix = "italian\\";
    break;
  case LANG_PORTUGUESE:
    this->alternateLanguage = 22;
    this->servicePrefix = "portuguese\\";
    break;
  case LANG_TURKISH:
    this->alternateLanguage = 31;
    this->servicePrefix = "turkish\\";
    break;
  default:
    this->alternateLanguage = 9;
    this->servicePrefix = "english\\";
    break;
  }

  if (g_serviceType == 15) {
    this->codePage = 0x4EA;
  }
}

// OFFSET: 0x00ACCCD0
// FUNCTION: fn_17751
CLanguageArabic::CLanguageArabic() : lexiconLoaded(false) {
}

// GLOBAL: gl_25168
LexiconBoundary g_lexiconBoundary;

// OFFSET: 0x00ACCF00
// FUNCTION: fn_17753
CLanguageTH::CLanguageTH() : lexiconLoaded(g_lexiconBoundary.Load()) {
}

// OFFSET: 0x007D99C0
// FUNCTION: fn_09608
UIHairStyleButton::UIHairStyleButton(int style, int sex) : UIBitmapButton(), m_style(style), m_sex(sex), m_race(0), m_preview(nullptr), m_selected(false) {
}

// OFFSET: 0x00ACD140
// FUNCTION: fn_17755
CLanguageBase::~CLanguageBase() = default;

// OFFSET: 0x00ACD200
// FUNCTION: fn_17757
CLanguageArabic::~CLanguageArabic() = default;

// OFFSET: 0x00ACD300
// FUNCTION: fn_17759
CLanguageTH::~CLanguageTH() = default;

// OFFSET: 0x00ACDD20
// FUNCTION: fn_17775
void CLanguageBase::RefreshComposition() {
  HIMC context = ImmGetContext(hWnd);
  if (!context) {
    this->candidate.clear();
    return;
  }

  WCHAR wide[256];
  char converted[256];
  const auto bytes = static_cast<std::uint32_t>(ImmGetCompositionStringW(context, GCS_COMPSTR, wide, sizeof(wide)));
  const unsigned size = WideCharToMultiByte(this->codePage, 0, wide, bytes >> 1, converted, sizeof(converted), nullptr, nullptr);
  ImmReleaseContext(hWnd, context);
  this->candidate.assign(converted, size);

  char combined[256];
  *std::format_to_n(combined, sizeof(combined) - 1, "{}{}", this->text, this->candidate).out = 0;

  SuggestChatCommand(combined);
}
