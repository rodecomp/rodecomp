#ifndef RAGNAROK_SOURCE_UI_MINIPARTYMANAGER_H
#define RAGNAROK_SOURCE_UI_MINIPARTYMANAGER_H

class UIWindowMgr;
struct UIWindowContext;
struct UIFrameContext;

struct MiniPartyWindows {
  UIWindowMgr& windows;
  UIWindowContext& windowContext;
  UIFrameContext& frames;

  void Rebuild();
};

#endif
