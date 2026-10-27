#ifndef RAGNAROK_SOURCE_RENDER_TEXT_TEXTCONTEXT_H
#define RAGNAROK_SOURCE_RENDER_TEXT_TEXTCONTEXT_H

#include <windows.h>

class CDC;
class FontCache;

struct TextLocale {
  signed char language;
  int service;
  const unsigned short* serverLanguage;
  bool inlineCharset;
};

class TextContext {
public:
  TextContext(CDC& surface, FontCache& fonts, const TextLocale& locale);
  ~TextContext();
  void SetFont(int family, int height, unsigned char bold, unsigned char italic);
  void SetColor(COLORREF color);
  void SetBackgroundColor(COLORREF color);
  void SetBackgroundMode(int mode);
  void SetBackground(COLORREF color, int mode);
  SIZE Measure(const char* text, int bytes);
  void Draw(int x, int y, const char* text, int bytes);
  static unsigned CodePage(unsigned charset);
  static unsigned Marker(const char* text, unsigned bytes, bool enabled);

private:
  /* 0000 */ HDC m_dc;
  /* 0004 */ HGDIOBJ m_previous;
  /* 0008 */ CDC* m_surface;
  FontCache* m_fonts;
  const TextLocale* m_locale;
  int m_family;
  int m_height;
  unsigned m_charset;
  unsigned char m_bold;
  unsigned char m_italic;
  SIZE MeasurePart(const char* text, int bytes, unsigned charset);
  void DrawPart(int x, int y, const char* text, int bytes, unsigned charset);
  void DrawPair(int x, int y, const char* text, int bytes);
};

#endif
