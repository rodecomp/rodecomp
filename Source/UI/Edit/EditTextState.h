#ifndef RAGNAROK_SOURCE_UI_EDIT_EDITTEXTSTATE_H
#define RAGNAROK_SOURCE_UI_EDIT_EDITTEXTSTATE_H

#include <string>
#include <windows.h>

struct CLanguage;

struct EditTextState {
  std::string& text;
  int& anchor;
  int& cursor;
  const int& maximum;
  const bool& focused;
  CLanguage& input;
  const UINT& codePage;

  void SetText(const char* value);
  void Trim();
  void SelectAll();
  int Length() const;
};

#endif
