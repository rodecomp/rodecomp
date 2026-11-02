#ifndef RAGNAROK_SOURCE_CLIENT_INPUT_LANGUAGE_H
#define RAGNAROK_SOURCE_CLIENT_INPUT_LANGUAGE_H

#include <cstdint>
#include <deque>
#include <string>
#include <vector>
#include <windows.h>

class UIWindow;
#include "UI/Window/WindowManagerWindows.h"

class TextInputSuggestions {
public:
  virtual void Update(const char* text) = 0;

protected:
  ~TextInputSuggestions() = default;
};

struct CLanguage {
  std::string& text;
  int& cursor;
  int& anchor;
  std::string& composition;
  std::string& candidate;
  int& compositionState;
  HWND& window;
  const bool& suggestionsEnabled;
  UIWindow* const& focused;
  UIWindow* const& chatInput;
  const bool& chatOpen;
  TextInputSuggestions& suggestions;

  void ResetInput();
  void DeleteSelection();
  void Insert(const char* value);
  void Select(int anchor, int cursor);
  void CopyDisplayText(std::string& output) const;
  void Selection(int& anchor, int& cursor) const;

  void SelectionRange(int& begin, int& end) const;

  void RefreshComposition(unsigned codePage);

  void CopySelection(bool password);
  void Paste(UIWindow* blockedFocus, WindowManagerWindows& windows);

  void MoveBack(unsigned codePage);

  void IncreaseSelection(unsigned codePage);
  void SetFlag8C(unsigned char value);
  void AdvanceCursor(int delta);
  void RetreatCursor(int delta);

  void Character(unsigned value, LPARAM unused, unsigned codePage);

  void KeyDown(
    unsigned key, LPARAM unused, unsigned codePage, bool password, UIWindow* blockedPasteFocus, WindowManagerWindows& windows, UIWindow* const& chatArrows
  );
};

// VTABLE: vt_01604
struct CLanguageBase {
  virtual ~CLanguageBase() = default;

  /* 0004 */ std::deque<unsigned int> unknown_0004;
  /* 0018 */ void* candidateList;
  /* 001C */ int candidateCount;
  /* 0020 */ int pageSize;
  /* 0024 */ int selection;
  /* 0028 */ std::vector<unsigned char> candidateStorage;
  /* 0034 */ std::string text;
  /* 004C */ int unknown_004C;
  /* 0050 */ int cursor;
  /* 0054 */ int anchor;
  /* 0058 */ std::string composition;
  /* 0070 */ std::string candidate;
  /* 0088 */ unsigned char unknown_0088[6];
  /* 008E */ std::uint16_t language;
  /* 0090 */ unsigned codePage;
  /* 0094 */ int unknown_0094;
  /* 0098 */ int unknown_0098;
  /* 009C */ std::string servicePrefix;
  /* 00B4 */ std::uint16_t alternateLanguage;
  /* 00B6 */ unsigned char unknown_00B6[2];

  CLanguageBase();
};

// VTABLE: vt_01607
struct CLanguageArabic : CLanguageBase {
  /* 00B8 */ bool lexiconLoaded;

  CLanguageArabic();
};

// VTABLE: vt_01606
struct CLanguageTH : CLanguageBase {
  /* 00B8 */ bool lexiconLoaded;

  CLanguageTH();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CLanguageBase, unknown_0004) == 0x0004);
static_assert(offsetof(CLanguageBase, candidateList) == 0x0018);
static_assert(offsetof(CLanguageBase, candidateStorage) == 0x0028);
static_assert(offsetof(CLanguageBase, text) == 0x0034);
static_assert(offsetof(CLanguageBase, cursor) == 0x0050);
static_assert(offsetof(CLanguageBase, anchor) == 0x0054);
static_assert(offsetof(CLanguageBase, composition) == 0x0058);
static_assert(offsetof(CLanguageBase, candidate) == 0x0070);
static_assert(offsetof(CLanguageBase, language) == 0x008E);
static_assert(offsetof(CLanguageBase, codePage) == 0x0090);
static_assert(offsetof(CLanguageBase, servicePrefix) == 0x009C);
static_assert(offsetof(CLanguageBase, alternateLanguage) == 0x00B4);
static_assert(sizeof(CLanguageBase) == 0x00B8);
static_assert(sizeof(CLanguageArabic) == 0x00BC);
static_assert(sizeof(CLanguageTH) == 0x00BC);
#endif

void __stdcall SubACDD10();
void SuggestChatCommand(const char* text);
int SubACFCE0();

#endif
