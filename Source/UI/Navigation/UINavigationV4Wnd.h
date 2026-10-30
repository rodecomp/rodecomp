#ifndef RAGNAROK_SOURCE_UI_NAVIGATION_UINAVIGATIONV4WND_H
#define RAGNAROK_SOURCE_UI_NAVIGATION_UINAVIGATIONV4WND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class CNavi_Object;
class CTexture;
class UIListBox_NaviSearch;
struct UIListBoxContext;
class UIStaticText;
struct UIButtonContext;

struct NavigationWindowInfo {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ unsigned char expanded;

  void Clamp();
};

class UINavigationV4Wnd : public UIFrameWnd {
public:
  void Slot11() override;
  void Slot19() override;
  void Slot31(int x, int y) override;
  bool ClearRouteTexts();
  void LayoutWithoutDetails();
  void LayoutWithDetails();
  void RefreshLayout();
  void SelectRouteNode(int node);
  bool IsListShown() const;
  void DrawMapTexture(int x, int y, int size);
  std::string ObjectName(int type, void* object);
  std::string ObjectLabel(int type, void* object);
  std::string MobName(CNavi_Object* mob);
  void SelectRoute(int route);
  void FillRouteGroups();
  void InsertNavigationLink(std::string name, int x, int y);

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
  /* 00E8 */ UIStaticText* unknown_00E8;
  /* 00EC */ UIListBox_NaviSearch* unknown_00EC;
  /* 00F0 */ UIListBox_NaviSearch* unknown_00F0;
  /* 00F4 */ UIWindow* unknown_00F4;
  /* 00F8 */ UIWindow* unknown_00F8;
  /* 00FC */ UIWindow* unknown_00FC;
  /* 0100 */ UIWindow* unknown_0100;
  /* 0104 */ std::vector<std::string> unknown_0104;
  /* 0110 */ NavigationWindowInfo* info;
};

class UIStaticBitmap;

class UINavigationroadiconWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIStaticBitmap* icon;

protected:
  UIButtonContext* m_controls;
};

class UINavigationRuideWnd : public UIFrameWnd {
public:
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void RefreshRoute();

protected:
  UIButtonContext* m_controls;

public:
  /* 00B4 */ UIListBox_NaviSearch* routeList;

protected:
  UIListBoxContext* m_listContext;
};

extern int g_navigationTab;
extern CTexture* g_navigationMapTexture;

#endif
