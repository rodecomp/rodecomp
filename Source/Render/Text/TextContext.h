#ifndef RAGNAROK_SOURCE_RENDER_TEXT_TEXTCONTEXT_H
#define RAGNAROK_SOURCE_RENDER_TEXT_TEXTCONTEXT_H

#include <windows.h>

class CDC;

struct TextLocale {
  signed char language;
  int service;
  const unsigned short* serverLanguage;
  bool inlineCharset;
};

class TextContext {
public:
  explicit TextContext(CDC& surface);
  ~TextContext();
  void SetFont(int family, int height, unsigned char bold, unsigned char italic);
  void SetColor(COLORREF color);
  void SetBackgroundColor(COLORREF color);
  void SetBackgroundMode(int mode);
  void SetBackground(COLORREF color, int mode);
  SIZE Measure(const char* text, int bytes);
  void Draw(int x, int y, const char* text, int bytes);
  static unsigned CodePage(unsigned charset);
  static unsigned Marker(const char* text, unsigned bytes);

private:
  /* 0000 */ HDC m_dc;
  /* 0004 */ HGDIOBJ m_previous;
  /* 0008 */ CDC* m_surface;
  /* 000C */ int m_family;
  /* 0010 */ int m_height;
  /* 0014 */ unsigned m_charset;
  /* 0018 */ unsigned char unknown_0018[5];
  /* 001D */ unsigned char m_bold;
  /* 001E */ unsigned char m_italic;
  SIZE MeasurePart(const char* text, int bytes, unsigned charset);
  void DrawPart(int x, int y, const char* text, int bytes, unsigned charset);
  void DrawPair(int x, int y, const char* text, int bytes);
};

#endif
