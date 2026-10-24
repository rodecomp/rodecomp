#ifndef RAGNAROK_SOURCE_UI_WINDOWFRAMEUPDATE_H
#define RAGNAROK_SOURCE_UI_WINDOWFRAMEUPDATE_H

#include <list>
#include <map>

class UIWindow;
struct BrowserRegistry;

// Borrowed UIWindowMgr rendered list, browser registry and overlay map. Calls
// retain original live traversal: callbacks may append entries, and browser
// polling may empty the registry. No snapshot or implicit lifetime extension.
struct WindowFrameUpdate {
  std::list<UIWindow*>& rendered;
  BrowserRegistry& browsers;
  std::map<int, UIWindow*>& overlays;

  void Update();
};

#endif
