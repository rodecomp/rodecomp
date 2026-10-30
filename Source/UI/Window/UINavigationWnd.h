#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>

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
  void RefreshHeaderTexts(int mode);
  void RefreshWorldMapMarks();
  void RefreshWorldMapSelection();
  void RefreshWorldMapLevels();

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x8];
  /* 00BC */ std::string m_title;
  /* 00D4 */ std::string m_subtitle;
  /* 00EC */ unsigned char unknown_00EC[0x8];
  /* 00F4 */ UIWindow* m_routePanel;
  /* 00F8 */ UIWindow* m_searchPanel;
  /* 00FC */ unsigned char unknown_00FC[0x8];
  /* 0104 */ UIWindow* m_routeFooter;
  /* 0108 */ UIWindow* m_searchFooter;
  /* 010C */ unsigned char unknown_010C[0x18];
  /* 0124 */ NavigationWndInfo* m_saved;
};

#endif
