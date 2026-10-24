#ifndef RAGNAROK_SOURCE_UI_WINDOWGEOMETRY_H
#define RAGNAROK_SOURCE_UI_WINDOWGEOMETRY_H

#include <cstdint>

class CDC;

// Contiguous UIWindow fields+20..+67. Parent/children, manager registration and
// virtual dispatch remain in the enclosing window; no replacement owner.
struct WindowGeometry {
  /* 0014 */ int width;
  /* 0018 */ int height;
  /* 001C */ int x;
  /* 0020 */ int y;
  /* 0024 */ CDC* surface;
  /* 0028 */ int visible;
  /* 002C */ int id;
  /* 0030 */ std::uint32_t unknown_0030;
  /* 0034 */ std::uint32_t unknown_0034;
  /* 0038 */ std::uint32_t targetAlpha;
  /* 003C */ std::uint32_t alphaStart;
  /* 0040 */ std::uint32_t currentAlpha;

  WindowGeometry();
  void Move(int x, int y);
  void Show(int visible);
  void SetId(int id);
  std::uint32_t Color();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(WindowGeometry) == 48);
#endif

#endif
