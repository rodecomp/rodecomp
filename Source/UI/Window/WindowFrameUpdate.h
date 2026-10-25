#ifndef RAGNAROK_SOURCE_UI_WINDOW_WINDOWFRAMEUPDATE_H
#define RAGNAROK_SOURCE_UI_WINDOW_WINDOWFRAMEUPDATE_H

#include <list>
#include <map>

class UIWindow;
struct BrowserRegistry;

struct WindowFrameUpdate {
  std::list<UIWindow*>& rendered;
  BrowserRegistry& browsers;
  std::map<int, UIWindow*>& overlays;

  void Update();
};

#endif
