#include "UI/Layout/SnapRectangle.h"

#include "Utility/Text/TextTokens.h"

#include <cstdlib>
#include <cstring>
#include <utility>

namespace {
  // INLINED
  bool Overlaps(int a, int b, int c, int d) {
    return (c >= a && c <= b) || (d >= a && d <= b) || (a >= c && a <= d) || (b >= c && b <= d);
  }

  // INLINED
  bool Near(int value, int edge) {
    return value >= (edge - 10) && value <= (edge + 10);
  }
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

