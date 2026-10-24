#ifndef RAGNAROK_SOURCE_CLIENT_MOUSE_H
#define RAGNAROK_SOURCE_CLIENT_MOUSE_H

#include <cstdint>
#include <windows.h>

// RTTI CMouse. Defaults below are the original
// static-data initializer, not evidence of a separate allocating constructor.
class CMouse {
public:
  virtual ~CMouse();
  void Update();
  void Reset();
  int Center();
  void SetX(int value);
  void SetY(int value);
  // Message position comes from GetCursorPos/mainWindow, not lParam or source.
  // The original resets the global mouse on first message; pass that owner
  // separately so non-global instances retain the same distinction.
  void Message(CMouse& global, HWND mainWindow, HWND source, UINT message, WPARAM keys, LPARAM unused);

  /* 0004 */ int x = 0;
  /* 0008 */ int y = 0;
  /* 000C */ int deltaX = 0;
  /* 0010 */ int deltaY = 0;
  /* 0014 */ int buttons[2] = {};
  /* 001C */ int wheel = 0;
  /* 0020 */ int pendingX = 0;
  /* 0024 */ int pendingY = 0;
  /* 0028 */ int pendingWheel = 0;
  /* 002C */ std::uint8_t previousButtons[2] = {};
  /* 002E */ std::uint8_t pendingButtons[2] = {};
  /* 0030 */ std::uint32_t pressTicks[2] = {};
  /* 0038 */ std::uint32_t doubleClickTime = 500;
  /* 003C */ std::uint8_t active = 1;
  std::uint8_t unknown_003D[3] = {};
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CMouse) == 64);
#endif

#endif
