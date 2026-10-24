#ifndef RAGNAROK_SOURCE_RENDER_TEXTCONTEXT_H
#define RAGNAROK_SOURCE_RENDER_TEXTCONTEXT_H

#include <cstdint>
#include <windows.h>

class CDC;
class FontCache;

// Original locale/service globals and server language field remain borrowed.
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
  void SetBackground(COLORREF color, int mode);
  SIZE Measure(const char* text, int bytes);
  void Draw(int x, int y, const char* text, int bytes);
  static unsigned CodePage(unsigned charset);
  static unsigned Marker(const char* text, unsigned bytes, bool enabled);

private:
  HDC m_dc;
  HGDIOBJ m_previous;
  CDC* m_surface;
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
