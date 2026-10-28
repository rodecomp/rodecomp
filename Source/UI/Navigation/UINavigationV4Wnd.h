#ifndef RAGNAROK_SOURCE_UI_NAVIGATION_UINAVIGATIONV4WND_H
#define RAGNAROK_SOURCE_UI_NAVIGATION_UINAVIGATIONV4WND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

struct NavigationWindowInfo {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ unsigned char expanded;
};

class UINavigationV4Wnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot19() override;
  bool ClearRouteTexts();
  void LayoutWithoutDetails();
  void LayoutWithDetails();

  /* 00B4 */ UIWindow* unknown_00B4;
  /* 00B8 */ UIWindow* unknown_00B8;
  /* 00BC */ UIWindow* unknown_00BC;
  /* 00C0 */ UIWindow* unknown_00C0;
  /* 00C4 */ UIWindow* unknown_00C4;
  /* 00C8 */ UIWindow* unknown_00C8;
  /* 00CC */ UIWindow* unknown_00CC;
  /* 00D0 */ UIWindow* unknown_00D0;
  /* 00D4 */ UIWindow* unknown_00D4;
  /* 00D8 */ UIWindow* unknown_00D8;
  /* 00DC */ UIWindow* unknown_00DC;
  /* 00E0 */ UIWindow* unknown_00E0;
  /* 00E4 */ UIWindow* unknown_00E4;
  /* 00E8 */ UIWindow* unknown_00E8;
  /* 00EC */ UIWindow* unknown_00EC;
  /* 00F0 */ UIWindow* unknown_00F0;
  /* 00F4 */ UIWindow* unknown_00F4;
  /* 00F8 */ UIWindow* unknown_00F8;
  /* 00FC */ UIWindow* unknown_00FC;
  /* 0100 */ UIWindow* unknown_0100;
  /* 0104 */ std::vector<std::string> unknown_0104;
  /* 0110 */ NavigationWindowInfo* info;
};

class UINavigationroadiconWnd : public UIFrameWnd {
public:
  void Slot31(int x, int y) override;

  /* 00B4 */ UIWindow* icon;
};

extern int g_navigationTab;

#endif
