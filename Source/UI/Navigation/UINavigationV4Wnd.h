#ifndef RAGNAROK_SOURCE_UI_NAVIGATION_UINAVIGATIONV4WND_H
#define RAGNAROK_SOURCE_UI_NAVIGATION_UINAVIGATIONV4WND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

enum NavigationButtonId : int {
  NAVI_BUTTON_DATA = 191,
  NAVI_BUTTON_VIEW_CHANGE = 429,
  NAVI_BUTTON_FIND = 430,
  NAVI_SEARCH_LIST = 441,
  NAVI_BUTTON_ROAD_LIST = 442,
  NAVI_BUTTON_PRIVATE_AIRPLANE = 499,
  NAVI_SEARCH_EDIT = 516,
};

class CBitmapRes;
class CNavi_Object;
class CTexture;
class UIListBox_NaviSearch;
class UIStaticText;

class UIBitmapButton;
class UICheckBox;
class UIComboBox_BmpWnd;
class UIBitmapEditCtrl;

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
  /* 00B8 */ UIBitmapButton* expandedCloseButton;
  /* 00BC */ UIWindow* minimizeButton;
  /* 00C0 */ UIBitmapButton* helpButton;
  /* 00C4 */ UIBitmapButton* maximizeButton;
  /* 00C8 */ UIBitmapButton* roadIconButton;
  /* 00CC */ UIBitmapButton* dataButton;
  /* 00D0 */ UIBitmapButton* backButton;
  /* 00D4 */ UICheckBox* minimizeCheck;
  /* 00D8 */ UIBitmapButton* roadListButton;
  /* 00DC */ UIComboBox_BmpWnd* searchType;
  /* 00E0 */ UIBitmapEditCtrl* searchEdit;
  /* 00E4 */ UIBitmapButton* searchButton;
  /* 00E8 */ UIStaticText* summaryLabel;
  /* 00EC */ UIListBox_NaviSearch* roadList;
  /* 00F0 */ UIListBox_NaviSearch* searchList;
  /* 00F4 */ UICheckBox* filterCheck;
  /* 00F8 */ UIBitmapButton* shareButton;
  /* 00FC */ UIBitmapButton* findButton;
  /* 0100 */ UIBitmapButton* moveButton;
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
