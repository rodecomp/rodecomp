#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

class UIButton;
class UIListBoxNavi;

struct NavigationWndInfo {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ bool minimized;
};

extern bool g_navigationMinimized;
extern int g_navigationMode;
extern UIWindow* g_navigationWindow;
extern bool g_navigationHidden;

class UINavigationWnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot31(int x, int y) override;
  void LayoutTabs();
  void ApplyMinimized();
  void RefreshHeaderTexts(int mode);
  void RefreshWorldMapMarks();
  void RefreshWorldMapSelection();
  void RefreshWorldMapLevels();
  void RefreshRouteList();

protected:
  /* 00B4 */ UIWindow* m_closeButton;
  /* 00B8 */ UIButton* m_minimizeButton;
  /* 00BC */ std::string m_title;
  /* 00D4 */ std::string m_subtitle;
  /* 00EC */ UIWindow* unknown_00EC;
  /* 00F0 */ UIWindow* m_titleBar;
  /* 00F4 */ UIListBoxNavi* m_routePanel;
  /* 00F8 */ UIListBoxNavi* m_searchPanel;
  /* 00FC */ UIWindow* unknown_00FC;
  /* 0100 */ UIWindow* unknown_0100;
  /* 0104 */ UIWindow* m_routeFooter;
  /* 0108 */ UIWindow* m_searchFooter;
  /* 010C */ UIWindow* unknown_010C;
  /* 0110 */ UIWindow* unknown_0110;
  /* 0114 */ UIWindow* unknown_0114;
  /* 0118 */ UIWindow* unknown_0118;
  /* 011C */ UIWindow* unknown_011C;
  /* 0120 */ UIWindow* unknown_0120;
  /* 0124 */ NavigationWndInfo* m_saved;
  /* 0128 */ UIWindow* unknown_0128;
};

#endif
