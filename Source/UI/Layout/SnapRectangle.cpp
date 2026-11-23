#include "UI/Layout/SnapRectangle.h"

#include "Utility/Text/TextTokens.h"

#include <cstdlib>
#include <cstring>
#include <utility>

namespace {
  // INLINED
  bool SnapLeadingEdge(int edge, int value, int& offset) {
    if (value < edge - 10 || value > edge + 10) {
      return false;
    }
    offset = edge - value;
    return true;
  }

  // INLINED
  bool SnapTrailingEdge(int edge, int value, int& offset) {
    if (value > edge + 10 || value < edge - 10) {
      return false;
    }
    offset = edge - value;
    return true;
  }
}

// OFFSET: 0x009B50C0
// FUNCTION: fn_45773
__forceinline bool SnapRectangle::Overlaps(int firstBegin, int firstEnd, int secondBegin, int secondEnd) const {
  return (secondBegin >= firstBegin && secondBegin <= firstEnd) || (secondEnd >= firstBegin && secondEnd <= firstEnd) ||
         (firstBegin >= secondBegin && firstBegin <= secondEnd) || (firstEnd >= secondBegin && firstEnd <= secondEnd);
}

// OFFSET: 0x009B5250
// FUNCTION: fn_13839
void SnapRectangle::Set(int x, int y, int width, int height) {
  this->left.position = x;
  this->left.begin = y;
  this->left.end = y + height;
  this->right.position = x + width;
  this->right.begin = y;
  this->right.end = y + height;
  this->top.position = y;
  this->top.begin = x;
  this->top.end = x + width;
  this->bottom.position = y + height;
  this->bottom.begin = x;
  this->bottom.end = x + width;
}

// OFFSET: 0x009B5200
// FUNCTION: fn_13838
void SnapRectangle::ReverseEdges() {
  Edge edge = this->left;
  this->left = this->right;
  this->right = edge;
  edge = this->top;
  this->top = this->bottom;
  this->bottom = edge;
}

// OFFSET: 0x009B4F70
// FUNCTION: fn_13836
void SnapRectangle::Snap(const SnapRectangle& other, int& dx, int& dy) const {
  dx = 0;
  dy = 0;
  if (this->Overlaps(this->left.begin, this->left.end, other.right.begin, other.right.end)) {
    if (SnapLeadingEdge(this->left.position, other.right.position, dx) || SnapTrailingEdge(this->right.position, other.left.position, dx)) {
      if (!SnapLeadingEdge(this->top.position, other.top.position, dy)) {
        SnapTrailingEdge(this->bottom.position, other.bottom.position, dy);
      }
    }
  }

  if (this->Overlaps(this->top.begin, this->top.end, other.bottom.begin, other.bottom.end)) {
    if (SnapLeadingEdge(this->top.position, other.bottom.position, dy) || SnapTrailingEdge(this->bottom.position, other.top.position, dy)) {
      if (SnapLeadingEdge(this->left.position, other.left.position, dx)) {
        return;
      }
      const int right = this->right.position;
      const int otherRight = other.right.position;
      if (otherRight <= right + 10 && otherRight >= right - 10) {
        dx = right - otherRight;
      }
    }
  }
}

