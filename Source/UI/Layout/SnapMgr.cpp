#include "UI/Layout/SnapMgr.h"
#include "UI/Layout/SnapObject.h"
#include "UI/Window/UIWindow.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindowMgr.h"

// STATIC: 0x01334698
// GLOBAL: gl_21916
const int g_snapOppositeSide[4] = {2, 3, 0, 1};
// STATIC: 0x013346A8
// GLOBAL: gl_21918
bool g_snapEnabled = true;
// STATIC: 0x013346A4
// GLOBAL: gl_21917
int g_snapDirection = 1;
// GLOBAL: gl_24655
CSnapMgr g_snapMgr;

namespace {
  // INLINED
  int Magnitude(int value) {
    return value < 0 ? (0 - value) : value;
  }
}

// OFFSET: 0x007E2DB0
// FUNCTION: fn_09714
bool SnapObject::ContainsUpward(SnapObject* target) {
  if (this == target) {
    return true;
  }

  for (auto next = this->edges[1].begin(); next != this->edges[1].end(); ++next) {
    if ((*next)->ContainsUpward(target)) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x007E21D0
// FUNCTION: fn_09694
CSnapMgr::CSnapMgr() : root(nullptr) {
}

// OFFSET: 0x007E23D0
// FUNCTION: fn_09697
CSnapMgr::~CSnapMgr() {
  this->Clear();
}

// INLINED
SnapObject* CSnapMgr::Find(const UIWindow* window) const {
  for (SnapObject* object : this->objects) {
    if (object->window == window) {
      return object;
    }
  }

  return nullptr;
}

// INLINED
void CSnapMgr::ResetVisited() {
  for (SnapObject* object : this->objects) {
    object->visited = 0;
  }
}

// OFFSET: 0x007E25C0
// FUNCTION: fn_09700
void CSnapMgr::SetRoot(UIWindow* window) {
  if (!g_snapEnabled || this->Find(window) || this->root) {
    return;
  }

  SnapObject* object = new SnapObject;
  object->window = window;
  this->root = object;
  this->objects.push_back(object);
}

// OFFSET: 0x007E2670
// FUNCTION: fn_09701
void CSnapMgr::Add(UIWindow* window) {
  if (!g_snapEnabled || (this->root && this->root->window == window) || this->Find(window)) {
    return;
  }

  SnapObject* object = new SnapObject;
  object->window = window;
  this->UpdateLinks(object);
  this->objects.push_back(object);
}

// OFFSET: 0x007E2D20
// FUNCTION: fn_09712
bool CSnapMgr::IsRoot(const UIWindow* window) const {
  return this->root && this->root->window == window;
}

// OFFSET: 0x007E2D80
// FUNCTION: fn_09713
bool CSnapMgr::Contains(const UIWindow* window) const {
  return this->Find(window) != nullptr;
}

// OFFSET: 0x007E2BA0
// FUNCTION: fn_09709
bool CSnapMgr::Connected(UIWindow* from, UIWindow* to) {
  this->ResetVisited();
  SnapObject* first = this->Find(from);
  if (!first) {
    return false;
  }

  SnapObject* second = this->Find(to);
  return second && first->Connected(second);
}

// OFFSET: 0x007E2F60
// FUNCTION: fn_09715
bool CSnapMgr::ConnectedToRoot(UIWindow* window) {
  if (!this->root || this->root->window == window) {
    return false;
  }

  this->ResetVisited();
  SnapObject* object = this->Find(window);
  return object && object->Connected(this->root);
}

// OFFSET: 0x007E34C0
// FUNCTION: fn_09719
void CSnapMgr::UpdateLinks(SnapObject* object) {
  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    SnapObject* existing = *entry;
    const int side = object->AdjacentSide(*existing);
    object->Remove(existing);
    existing->Remove(object);

    if (side != -1) {
      object->edges[side].push_back(existing);
      const int opposite = g_snapOppositeSide[side];
      if (opposite != -1) {
        existing->edges[opposite].push_back(object);
      }
    }
  }
}

// OFFSET: 0x007E29C0
// FUNCTION: fn_09705
void CSnapMgr::Refresh(UIWindow* window) {
  if (!this->Find(window)) {
    return;
  }

  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    this->UpdateLinks(*entry);
  }
}

// OFFSET: 0x007E2720
// FUNCTION: fn_09702
void CSnapMgr::CaptureOrigins(UIWindow* window) {
  if (!this->root || !window || this->root->window != window) {
    return;
  }

  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    if ((*entry)->window && this->ConnectedToRoot((*entry)->window)) {
      (*entry)->window->Slot12();
    }
  }
}

// OFFSET: 0x007E2A10
// FUNCTION: fn_09706
void CSnapMgr::ConstrainGroup(UIWindow* window, int& dx, int& dy) {
  if (!this->root) {
    return;
  }

  int resultX = 0, resultY = 0;
  if (this->root->window == window) {
    for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
      UIWindow* member = (*entry)->window;
      if (!this->ConnectedToRoot(member)) {
        continue;
      }

      const int savedY = member->geometry.y;
      member->Slot13(dx, dy);
      int x, y;
      g_windowMgr.ConstrainSnap(member, x, y, 1);

      member->geometry.x = savedY;
      if (Magnitude(resultX) < Magnitude(x)) {
        resultX = x;
      }

      if (Magnitude(resultY) < Magnitude(y)) {
        resultY = y;
      }
    }
  }

  dx = resultX;
  dy = resultY;
}

// OFFSET: 0x007E3390
// FUNCTION: fn_09718
void CSnapMgr::MoveConnected(UIWindow* window, int dx, int dy) {
  if (!this->root || this->root->window != window) {
    return;
  }

  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    UIWindow* member = (*entry)->window;
    if (this->ConnectedToRoot(member)) {
      member->Slot13(dx, dy);
      g_windowMgr.StoreSnapRectangle(member);
    }
  }
}

// OFFSET: 0x007E31C0
// FUNCTION: fn_09717
void SnapObject::MoveBranch(int dx, int dy, int side) {
  if (side == -1 || this->visited == 1) {
    return;
  }

  this->visited = 1;
  if (this->window) {
    this->window->geometry.x = (this->window->geometry.x + dx);
    this->window->geometry.y = (this->window->geometry.y + dy);
    g_windowMgr.StoreSnapRectangle(this->window);
  }

  for (auto next = this->edges[side].begin(); next != this->edges[side].end(); ++next) {
    (*next)->MoveBranch(dx, dy, side);
  }
}

// OFFSET: 0x007E2FD0
// FUNCTION: fn_09716
void CSnapMgr::MoveEdge(UIWindow* window, int dx, int dy, int side) {
  this->ResetVisited();
  SnapObject* object = this->Find(window);
  if (!object) {
    return;
  }

  if (side != -1 && object->visited != 1) {
    object->visited = 1;
    int up = 0, down = 0;
    object->AccumulateHeight(up, 1);
    const int y = object->window->geometry.y;
    const int height = object->window->geometry.height;
    object->AccumulateHeight(down, 3);
    const int candidateBottom = ((object->window->geometry.y - dy) + down);
    const int screenHeight = g_renderer->viewport.height;

    if (((y + height) - up) != 0 && (!g_snapMgr.root || g_snapMgr.root != object || candidateBottom == screenHeight)) {
      bool moveSelf = side != 3;
      if (side == 3) {
        bool boundaryAncestor = false;
        if (g_snapMgr.root != object) {
          for (SnapObject* next : g_snapMgr.root->edges[1]) {
            if (next == object) {
              boundaryAncestor = true;
              break;
            }

            for (auto above = next->edges[1].begin(); above != next->edges[1].end(); ++above) {
              if ((*above)->ContainsUpward(object)) {
                boundaryAncestor = true;
                break;
              }
            }

            if (boundaryAncestor) {
              break;
            }
          }
        }

        if (boundaryAncestor || candidateBottom == screenHeight) {
          side = g_snapDirection;
          dy = (0 - dy);
          moveSelf = true;
        }
      }

      if (moveSelf && side == 1 && object->window) {
        object->window->geometry.x = (object->window->geometry.x + dx);
        object->window->geometry.y = (object->window->geometry.y + dy);
        g_windowMgr.StoreSnapRectangle(object->window);
      }
    }

    for (auto next = object->edges[side].begin(); next != object->edges[side].end(); ++next) {
      (*next)->MoveBranch(dx, dy, side);
    }
  }

  this->Refresh(window);
}

// OFFSET: 0x007E3600
// FUNCTION: fn_09720
void CSnapMgr::Clear() {
  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    delete *entry;
  }

  this->objects.clear();
  this->root = nullptr;
}

// OFFSET: 0x007E3670
// FUNCTION: fn_09721
void CSnapMgr::Remove(UIWindow* window) {
  if (this->root && this->root->window == window) {
    this->root = nullptr;
  }

  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    SnapObject* object = *entry;
    if (object->window != window) {
      continue;
    }

    delete object;
    this->objects.erase(entry);

    for (auto remaining = this->objects.begin(); remaining != this->objects.end(); ++remaining) {
      (*remaining)->Remove(object);
    }

    return;
  }
}
