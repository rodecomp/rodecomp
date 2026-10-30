#ifndef RAGNAROK_SOURCE_UI_WINDOW_FUNCDRAWWNDCHAR_H
#define RAGNAROK_SOURCE_UI_WINDOW_FUNCDRAWWNDCHAR_H

#include <map>
#include <string>
#include <utility>

class CActRes;
class CSprRes;

// VTABLE: vt_00803
class FuncDrawWNDChar {
public:
  virtual ~FuncDrawWNDChar();

  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
  /* 0010 */ unsigned char unknown_0010;
  /* 0012 */ short unknown_0012;
  /* 0014 */ int unknown_0014;
  /* 0018 */ short unknown_0018;
  /* 001A */ short unknown_001A;
  /* 001C */ short unknown_001C;
  /* 001E */ short unknown_001E;
  /* 0020 */ short unknown_0020;
  /* 0024 */ int unknown_0024;
  /* 0028 */ unsigned char unknown_0028;
  /* 002C */ int unknown_002C;
  /* 0030 */ int unknown_0030;
  /* 0034 */ int unknown_0034;
  /* 0038 */ int unknown_0038;
  /* 003C */ int unknown_003C;
  /* 0040 */ int unknown_0040;
  /* 0044 */ unsigned char unknown_0044;
  /* 0045 */ unsigned char unknown_0045;
  /* 0048 */ std::map<int, std::pair<CSprRes*, CActRes*>> m_resources;
  /* 0050 */ std::string unknown_0050;
  /* 0068 */ std::string unknown_0068;
  /* 0080 */ std::map<int, int> unknown_0080;
  /* 0088 */ unsigned char unknown_0088;
  /* 008C */ int unknown_008C;
  /* 0090 */ int unknown_0090;
  /* 0094 */ unsigned char unknown_0094;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(FuncDrawWNDChar) == 152);
#endif

#endif
