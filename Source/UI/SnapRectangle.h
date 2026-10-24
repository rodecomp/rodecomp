#ifndef RAGNAROK_SOURCE_UI_SNAPRECTANGLE_H
#define RAGNAROK_SOURCE_UI_SNAPRECTANGLE_H

#include <cstdint>

// Four axis-aligned edge records in original order: left, right, top, bottom.
// Vertical records store(x,top,bottom); horizontal records store(y,left,right).
struct SnapRectangle {
  struct Edge {
    std::int32_t position;
    std::int32_t begin;
    std::int32_t end;
  };
  Edge left;
  Edge right;
  Edge top;
  Edge bottom;

  void Set(int x, int y, int width, int height);
  void ReverseEdges();
  void Snap(const SnapRectangle& other, int& dx, int& dy) const;
};

static_assert(sizeof(SnapRectangle) == 48);

#endif
