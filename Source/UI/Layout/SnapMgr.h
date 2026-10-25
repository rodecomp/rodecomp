#ifndef RAGNAROK_SOURCE_UI_LAYOUT_SNAPMGR_H
#define RAGNAROK_SOURCE_UI_LAYOUT_SNAPMGR_H

#include <list>

class SnapObject;
class UIWindow;
struct WindowSnapping;

class CSnapMgr {
public:
  CSnapMgr();
  virtual ~CSnapMgr();
  void SetRoot(UIWindow* window);
  void Add(UIWindow* window);
  void Remove(UIWindow* window);
  void Clear();
  bool IsRoot(const UIWindow* window) const;
  bool Contains(const UIWindow* window) const;
  bool Connected(UIWindow* from, UIWindow* to);
  bool ConnectedToRoot(UIWindow* window);
  void UpdateLinks(SnapObject* object);
  void Refresh(UIWindow* window);
  void CaptureOrigins(UIWindow* window);
  void ConstrainGroup(UIWindow* window, int& dx, int& dy, WindowSnapping& snapping);
  void MoveConnected(UIWindow* window, int dx, int dy, WindowSnapping& snapping);
  void MoveEdge(UIWindow* window, int dx, int dy, int side, WindowSnapping& snapping);

  /* 0004 */ SnapObject* root;
  /* 0008 */ std::list<SnapObject*> objects;

private:
  void ResetVisited();
  SnapObject* Find(const UIWindow* window) const;
  static void MoveBranch(SnapObject* object, int dx, int dy, int side, WindowSnapping& snapping);
  CSnapMgr(const CSnapMgr&) = delete;
  CSnapMgr& operator=(const CSnapMgr&) = delete;
};

// STATIC: 0x01334698 / 0x013346A8
extern const int g_snapOppositeSide[4];
extern bool g_snapEnabled;
// STATIC: 0x013346A4 / 0x013346B0
extern int g_snapDirection;
extern SnapObject* g_snapBoundary;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CSnapMgr) == 16);
#endif

#endif
