#ifndef RAGNAROK_SOURCE_UI_EDIT_EDITVIEWPORT_H
#define RAGNAROK_SOURCE_UI_EDIT_EDITVIEWPORT_H

#include <string>
#include <windows.h>

class CDC;
struct CLanguageBase;
struct EditTextState;

struct EditViewport {
  std::string& text;
  int& anchor;
  int& cursor;
  int& firstVisible;
  const int& width;
  const UINT& codePage;
  EditTextState& state;
  CDC& surface;

  void Synchronize();
};

#endif
