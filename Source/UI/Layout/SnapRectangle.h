#ifndef RAGNAROK_SOURCE_UI_LAYOUT_SNAPRECTANGLE_H
#define RAGNAROK_SOURCE_UI_LAYOUT_SNAPRECTANGLE_H

#include <cstddef>
#include <cstdint>

struct SnapRectangle {
  struct Edge {
    /* 0000 */ std::int32_t position;
    /* 0004 */ std::int32_t begin;
    /* 0008 */ std::int32_t end;
  };
  /* 0000 */ Edge left;
  /* 000C */ Edge right;
  /* 0018 */ Edge top;
  /* 0024 */ Edge bottom;

  void Set(int x, int y, int width, int height);
  void ReverseEdges();
  void Snap(const SnapRectangle& other, int& dx, int& dy) const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(SnapRectangle) == 48);
static_assert(sizeof(SnapRectangle::Edge) == 12);
static_assert(offsetof(SnapRectangle::Edge, position) == 0x0);
static_assert(offsetof(SnapRectangle::Edge, begin) == 0x4);
static_assert(offsetof(SnapRectangle::Edge, end) == 0x8);
static_assert(offsetof(SnapRectangle, left) == 0x0);
static_assert(offsetof(SnapRectangle, right) == 0xC);
static_assert(offsetof(SnapRectangle, top) == 0x18);
static_assert(offsetof(SnapRectangle, bottom) == 0x24);
#endif

#endif
