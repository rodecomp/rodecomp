#ifndef RAGNAROK_SOURCE_UI_EDITTEXTSTATE_H
#define RAGNAROK_SOURCE_UI_EDITTEXTSTATE_H

#include <string>
#include <windows.h>

struct TextInputBuffer;

// Noncontiguous UIEditCtrl fields. UIEditCtrl's actual virtual SetText also
// invalidates its parent; that ownership is retained by the concrete control.
struct EditTextState {
  std::string& text;  // +0xD8
  int& anchor;        // +0x7C
  int& cursor;        // +0x80
  const int& maximum; // +0x88
  const bool& focused;
  TextInputBuffer& input;
  const UINT& codePage;

  void SetText(const char* value);
  void Trim();
  void SelectAll();
  int Length() const;
};

#endif
