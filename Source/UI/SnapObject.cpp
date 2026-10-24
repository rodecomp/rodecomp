#include "UI/SnapObject.h"
#include "UI/UIWindow.h"

#include <bit>

namespace {
  int Add(int a, int b) {
    return std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(a) + static_cast<std::uint32_t>(b));
  }
  bool Overlaps(int a, int b, int c, int d) {
    return (c >= a && c <= b) || (d >= a && d <= b) || (a >= c && a <= d) || (b >= c && b <= d);
  }
}

// OFFSET: 0x007E2240
// FUNCTION: fn_09695
SnapObject::SnapObject() : window(nullptr), visited(0) {
}
// OFFSET: 0x007E2550
// FUNCTION: fn_09699
// OFFSET: 0x007E2360
// FUNCTION: fn_09696
SnapObject::~SnapObject() = default;

// OFFSET: 0x007E2770
// FUNCTION: fn_09703
int SnapObject::AdjacentSide(const SnapObject& other) const {
  const WindowGeometry& a = this->window->geometry;
  const WindowGeometry& b = other.window->geometry;
  const int right = Add(a.x, a.width);
  const int bottom = Add(a.y, a.height);
  const int otherRight = Add(b.x, b.width);
  const int otherBottom = Add(b.y, b.height);
  if (a.x == otherRight && Overlaps(a.y, bottom, b.y, otherBottom)) {
    return 0;
  }
  if (b.x == right && Overlaps(a.y, bottom, b.y, otherBottom)) {
    return 2;
  }
  if (a.y == otherBottom && Overlaps(a.x, right, b.x, otherRight)) {
    return 1;
  }
  if (b.y == bottom && Overlaps(a.x, right, b.x, otherRight)) {
    return 3;
  }
  return -1;
}

// OFFSET: 0x007E2960
// FUNCTION: fn_09704
void SnapObject::Remove(SnapObject* object) {
  if (!object) {
    return;
  }
  for (auto& edge : this->edges) {
    for (auto node = edge.begin(); node != edge.end(); ++node) {
      if (*node == object) {
        edge.erase(node);
        break;
      }
    }
  }
}

// OFFSET: 0x007E2C20
// FUNCTION: fn_09710
bool SnapObject::Contains(SnapObject* target) {
  if (this->visited == 1) {
    return false;
  }
  if (target == this) {
    return true;
  }
  this->visited = 1;
  for (auto& edge : this->edges) {
    for (SnapObject* node : edge) {
      if (node->Contains(target)) {
        return true;
      }
    }
  }
  return false;
}

// OFFSET: 0x007E2C90
// FUNCTION: fn_09711
bool SnapObject::Connected(SnapObject* target) {
  if (this->visited == 1 || target == this) {
    return false;
  }
  this->visited = 1;
  for (auto& edge : this->edges) {
    for (SnapObject* node : edge) {
      if (node->Contains(target)) {
        return true;
      }
    }
  }
  return false;
}

// OFFSET: 0x007E2B10
// FUNCTION: fn_09708
void SnapObject::AccumulateHeight(int& height, int side) const {
  const SnapObject* current = this;
  for (;;) {
    height = Add(height, current->window->geometry.height);
    const SnapObject* next = nullptr;
    for (SnapObject* candidate : current->edges[side]) {
      // Original selection is overwritten even by a nonpositive final node.
      next = candidate->window->geometry.height > 0 ? candidate : nullptr;
    }
    if (!next) {
      return;
    }
    current = next;
  }
}
