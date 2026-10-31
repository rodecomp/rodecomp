#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIWINDOWFACTORY_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIWINDOWFACTORY_H

class UIWindow;
class UIWindowMgr;
struct UIWindowContext;
struct UIFrameContext;
struct UIEntryQueueContext;
struct UIListBoxContext;
struct QuickSlotPainting;
struct ScriptOptions;

struct UIWindowFactory {
  UIWindowMgr& windows;
  UIWindowContext& windowContext;
  UIFrameContext& frames;
  UIEntryQueueContext& entryQueue;
  UIListBoxContext& listBox;
  QuickSlotPainting& quickSlots;
  ScriptOptions& options;

  UIWindow* CreateFromLayout(int id);
};

extern bool g_searchStoreInfoCreated;

#endif
