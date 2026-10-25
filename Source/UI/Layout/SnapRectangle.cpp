#include "UI/Layout/SnapRectangle.h"

#include <utility>

namespace {
  bool Overlaps(int a, int b, int c, int d) {
    return (c >= a && c <= b) || (d >= a && d <= b) || (a >= c && a <= d) || (b >= c && b <= d);
  }

  bool Near(int value, int edge) {
    return value >= (edge - 10) && value <= (edge + 10);
  }
}

// OFFSET: 0x009B5250
// FUNCTION: fn_13839
void SnapRectangle::Set(int x, int y, int width, int height) {
  const int right = (x + width);
  const int bottom = (y + height);
  this->left = {x, y, bottom};
  this->right = {right, y, bottom};
  this->top = {y, x, right};
  this->bottom = {bottom, x, right};
}

// OFFSET: 0x009B5200
// FUNCTION: fn_13838
void SnapRectangle::ReverseEdges() {
  std::swap(this->left, this->right);
  std::swap(this->top, this->bottom);
}

// OFFSET: 0x009B4F70
// FUNCTION: fn_13836
void SnapRectangle::Snap(const SnapRectangle& other, int& dx, int& dy) const {
  dx = 0;
  dy = 0;
  if (Overlaps(this->left.begin, this->left.end, other.right.begin, other.right.end)) {
    int edge = this->left.position;
    int value = other.right.position;
    bool close = Near(value, edge);

    if (!close) {
      edge = this->right.position;
      value = other.left.position;
      close = Near(value, edge);
    }

    if (close) {
      dx = (edge - value);
      edge = this->top.position;
      value = other.top.position;
      close = Near(value, edge);

      if (!close) {
        edge = this->bottom.position;
        value = other.bottom.position;
        close = Near(value, edge);
      }

      if (close) {
        dy = (edge - value);
      }
    }
  }

  if (Overlaps(this->top.begin, this->top.end, other.bottom.begin, other.bottom.end)) {
    int edge = this->top.position;
    int value = other.bottom.position;
    bool close = Near(value, edge);

    if (!close) {
      edge = this->bottom.position;
      value = other.top.position;
      close = Near(value, edge);
    }

    if (close) {
      dy = (edge - value);
      edge = this->left.position;
      value = other.left.position;
      close = Near(value, edge);

      if (!close) {
        edge = this->right.position;
        value = other.right.position;
        close = Near(value, edge);
      }

      if (close) {
        dx = (edge - value);
      }
    }
  }
}
