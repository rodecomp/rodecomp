#include "UI/Layout/SnapObject.h"
#include "UI/Window/UIWindow.h"

namespace {
  // INLINED
  bool Overlaps(int a, int b, int c, int d) {
    return (c >= a && c <= b) || (d >= a && d <= b) || (a >= c && a <= d) || (b >= c && b <= d);
  }
}

// OFFSET: 0x007E2240
// FUNCTION: fn_09695
SnapObject::SnapObject() : window(nullptr), visited(0) {
}

// OFFSET: 0x007E2550
// OFFSET: 0x007E2360
SnapObject::~SnapObject() = default;

// OFFSET: 0x007E2770
// FUNCTION: fn_09703
int SnapObject::AdjacentSide(const SnapObject& other) const {
  const WindowGeometry& a = this->window->geometry;
  const WindowGeometry& b = other.window->geometry;
  const int right = (a.x + a.width);
  const int bottom = (a.y + a.height);
  const int otherRight = (b.x + b.width);
  const int otherBottom = (b.y + b.height);

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

// OFFSET: 0x007E2B00
// FUNCTION: fn_09707
int SnapObject::GetHeight() const {
  return this->window->geometry.height;
}

// OFFSET: 0x007E2B10
// FUNCTION: fn_09708
void SnapObject::AccumulateHeight(int& height, int side) const {
  const SnapObject* current = this;

  for (;;) {
    height = (height + current->window->geometry.height);
    const SnapObject* next = nullptr;

    for (SnapObject* candidate : current->edges[side]) {
      next = candidate->window->geometry.height > 0 ? candidate : nullptr;
    }

    if (!next) {
      return;
    }

    current = next;
  }
}
