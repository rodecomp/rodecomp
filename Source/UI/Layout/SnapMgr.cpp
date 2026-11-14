#include "UI/Layout/SnapMgr.h"
#include "UI/Layout/SnapObject.h"
#include "UI/Window/UIWindow.h"
#include "Render/Renderer.h"
#include "UI/Window/UIWindowMgr.h"

#include <cstdlib>

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

template __declspec(noinline) void std::list<SnapObject*>::push_back(SnapObject* const&);

namespace {
// INLINED
__forceinline void RemoveLink(SnapObject& self, SnapObject* object) {
  if (!object) {
    return;
  }

  for (int side = 0; side < 4; ++side) {
    for (auto node = self.edges[side].begin(); node != self.edges[side].end(); ++node) {
      if (*node == object) {
        self.edges[side].erase(node);
        break;
      }
    }
  }
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
  for (auto object = this->objects.begin(); object != this->objects.end(); ++object) {
    (*object)->visited = 0;
  }
}

// OFFSET: 0x007E25C0
// FUNCTION: fn_09700
void CSnapMgr::SetRoot(UIWindow* window) {
  if (!g_snapEnabled || this->Contains(window) || this->root) {
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
  if (!g_snapEnabled || (this->root && this->root->window == window) || this->Contains(window)) {
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
  if (!this->root) {
    return false;
  }

  return this->root->window == window;
}

// OFFSET: 0x007E2D80
// FUNCTION: fn_09713
bool CSnapMgr::Contains(const UIWindow* window) const {
  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    if (window == (*entry)->window) {
      return true;
    }
  }

  return false;
}

// OFFSET: 0x007E2BA0
// FUNCTION: fn_09709
bool CSnapMgr::Connected(UIWindow* from, UIWindow* to) {
  this->ResetVisited();
  SnapObject* first = nullptr;
  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    if ((*entry)->window == from) {
      first = *entry;
      break;
    }
  }
  if (!first) {
    return false;
  }

  SnapObject* second = nullptr;
  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    if ((*entry)->window == to) {
      second = *entry;
      break;
    }
  }
  if (!second) {
    return false;
  }

  return first->Connected(second);
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
    RemoveLink(*object, existing);
    RemoveLink(*existing, object);

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
  if (!this->Contains(window)) {
    return;
  }

  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    this->UpdateLinks(*entry);
  }
}

// OFFSET: 0x007E2720
// FUNCTION: fn_09702
void CSnapMgr::CaptureOrigins(UIWindow* window) {
  if (!this->root || !window || window != this->root->window) {
    return;
  }

  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    UIWindow* member = (*entry)->window;
    if (member && this->ConnectedToRoot(member)) {
      member->StoreMoveOrigin();
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
  if (window == this->root->window) {
    for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
      UIWindow* member = (*entry)->window;
      if (!this->ConnectedToRoot(member)) {
        continue;
      }

      const int savedY = member->geometry.y;
      member->MoveFromOrigin(dx, dy);
      int x, y;
      g_windowMgr.ConstrainSnap(member, x, y, 1);

      member->geometry.x = savedY;
      if (std::abs(resultX) < std::abs(x)) {
        resultX = x;
      }

      if (std::abs(resultY) < std::abs(y)) {
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
  if (!this->root || window != this->root->window) {
    return;
  }

  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    UIWindow* member = (*entry)->window;
    if (this->ConnectedToRoot(member)) {
      member->MoveFromOrigin(dx, dy);
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
    int up = 0;
    object->AccumulateHeight(up, 1);
    int down = 0;
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

  SnapObject* removed = nullptr;
  for (auto entry = this->objects.begin(); entry != this->objects.end(); ++entry) {
    if ((*entry)->window == window) {
      removed = *entry;
      delete removed;
      this->objects.erase(entry);
      break;
    }
  }

  if (removed) {
    for (auto remaining = this->objects.begin(); remaining != this->objects.end(); ++remaining) {
      (*remaining)->Remove(removed);
    }
  }
}
