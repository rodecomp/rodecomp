#ifndef RAGNAROK_SOURCE_UI_EDIT_EDITVIEWPORT_H
#define RAGNAROK_SOURCE_UI_EDIT_EDITVIEWPORT_H

#include <string>
#include <windows.h>

class CDC;
class FontCache;
struct TextLocale;
struct CLanguage;
struct EditTextState;

struct EditViewport {
  std::string& text;
  int& anchor;
  int& cursor;
  int& firstVisible;
  const int& width;
  const UINT& codePage;
  CLanguage& input;
  EditTextState& state;
  CDC& surface;
  FontCache& fonts;
  const TextLocale& locale;

  void Synchronize();
};

#endif
