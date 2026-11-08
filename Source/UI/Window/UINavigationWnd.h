#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIWindowMgr;
class CActRes;
class CSprRes;
struct NaviCell;
struct NaviRouteLine;
class UIBitmapButton;
class UIButton;
class UICheckBox;
class UIComboBoxDefWnd;
class UIEditCtrl;
class UIListBoxNavi;
class UIResizer;

struct NavigationWndInfo {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ bool minimized;
};


class UINavigationWnd : public UIFrameWnd {
public:
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  void OnLBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void LayoutTabs();
  void ApplyMinimized();
  __declspec(noinline) void RefreshHeaderTexts(int mode);
  void RefreshWorldMapMarks();
  void RefreshWorldMapSelection();
  void RefreshWorldMapLevels();
  void RefreshRouteList();
  void FillRouteList();
  bool LoadRoute(int index);
  bool ClearHistory();

protected:
  /* 00B4 */ UIWindow* m_closeButton;
  /* 00B8 */ UIButton* m_minimizeButton;
  /* 00BC */ std::string m_title;
  /* 00D4 */ std::string m_subtitle;
  /* 00EC */ UIComboBoxDefWnd* m_searchType;
  /* 00F0 */ UIEditCtrl* m_titleBar;
  /* 00F4 */ UIListBoxNavi* m_routePanel;
  /* 00F8 */ UIListBoxNavi* m_searchPanel;
  union {
    /* 00FC */ UIBitmapButton* m_buttons[6];
    struct {
      /* 00FC */ UIWindow* m_searchButton;
      /* 0100 */ UIWindow* m_startButton;
      /* 0104 */ UIWindow* m_routeFooter;
      /* 0108 */ UIWindow* m_searchFooter;
      /* 010C */ UIWindow* m_systemChangeButton;
      /* 0110 */ UIWindow* m_folderButton;
    };
  };
  /* 0114 */ UICheckBox* m_allowGates;
  /* 0118 */ UICheckBox* m_includeObjects;
  /* 011C */ UICheckBox* m_allowWarps;
  /* 0120 */ UIBitmapButton* m_helpButton;
  /* 0124 */ NavigationWndInfo* m_saved;
  /* 0128 */ UIResizer* m_resizer;
  /* 012C */ std::vector<std::string> m_history;

};

#endif
