#ifndef RAGNAROK_SOURCE_UI_NAVIGATION_UINAVIGATIONV4WND_H
#define RAGNAROK_SOURCE_UI_NAVIGATION_UINAVIGATIONV4WND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class CBitmapRes;
class CNavi_Object;
class CTexture;
class UIListBox_NaviSearch;
class UIStaticText;

class UINavigationV4Wnd : public UIFrameWnd {
public:
  UINavigationV4Wnd();
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  bool ClearRouteTexts();
  void LayoutWithoutDetails();
  void LayoutWithDetails();
  void RefreshLayout();
  __declspec(noinline) void SelectRouteNode(int node);
  bool IsListShown() const;
  void DrawMapTexture(int x, int y, int size);
  std::string ObjectName(int type, void* object);
  std::string ObjectLabel(int type, void* object);
  std::string MobName(CNavi_Object* mob);
  void SelectRoute(int route);
  void FillRouteGroups();
  bool LoadRoute(int index);
  void SetRoadIcon(int slot);
  void InsertNavigationLink(std::string name, int x, int y);
  void DrawTargetSprite(int x, int y, int width, int height, const CBitmapRes* background);

  /* 00B4 */ UIWindow* closeButton;
  /* 00B8 */ UIWindow* expandedCloseButton;
  /* 00BC */ UIWindow* minimizeButton;
  /* 00C0 */ UIWindow* helpButton;
  /* 00C4 */ UIWindow* maximizeButton;
  /* 00C8 */ UIWindow* roadIconButton;
  /* 00CC */ UIWindow* dataButton;
  /* 00D0 */ UIWindow* backButton;
  /* 00D4 */ UIWindow* minimizeCheck;
  /* 00D8 */ UIWindow* roadListButton;
  /* 00DC */ UIWindow* searchType;
  /* 00E0 */ UIWindow* searchEdit;
  /* 00E4 */ UIWindow* searchButton;
  /* 00E8 */ UIStaticText* summaryLabel;
  /* 00EC */ UIListBox_NaviSearch* roadList;
  /* 00F0 */ UIListBox_NaviSearch* searchList;
  /* 00F4 */ UIWindow* filterCheck;
  /* 00F8 */ UIWindow* shareButton;
  /* 00FC */ UIWindow* findButton;
  /* 0100 */ UIWindow* moveButton;
  /* 0104 */ std::vector<std::string> routeFiles;
  /* 0110 */ NavigationWindowInfo* info;
  /* 0114 */ int padding_0114;
  /* 0118 */ int noticeUntil;
  /* 011C */ int animationDuration;
  /* 0120 */ unsigned char padding_0120[4];
};

class UIStaticBitmap;

class UINavigationroadiconWnd : public UIFrameWnd {
public:
  UINavigationroadiconWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00B4 */ UIStaticBitmap* icon;
};

class UINavigationRuideWnd : public UIFrameWnd {
public:
  UINavigationRuideWnd();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  void RefreshRoute();

protected:

public:
  /* 00B4 */ UIListBox_NaviSearch* routeList;
};


#endif
