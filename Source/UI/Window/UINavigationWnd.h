#ifndef RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UINAVIGATIONWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <vector>

class UIBitmapButton;
class UIButton;
class UICheckBox;
class UIComboBoxDefWnd;
class UIEditCtrl;
class UIListBoxNavi;
class UIResizer;
struct UIButtonContext;
struct UIComboBoxHeaderContext;
struct UIEditContext;
struct UIListBoxContext;

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
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot31(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void LayoutTabs();
  void ApplyMinimized();
  void RefreshHeaderTexts(int mode);
  void RefreshWorldMapMarks();
  void RefreshWorldMapSelection();
  void RefreshWorldMapLevels();
  void RefreshRouteList();
  void FillRouteList();

protected:
  /* 00B4 */ UIWindow* m_closeButton;
  /* 00B8 */ UIButton* m_minimizeButton;
  /* 00BC */ std::string m_title;
  /* 00D4 */ std::string m_subtitle;
  /* 00EC */ UIComboBoxDefWnd* unknown_00EC;
  /* 00F0 */ UIEditCtrl* m_titleBar;
  /* 00F4 */ UIListBoxNavi* m_routePanel;
  /* 00F8 */ UIListBoxNavi* m_searchPanel;
  union {
    /* 00FC */ UIBitmapButton* m_buttons[6];
    struct {
      /* 00FC */ UIWindow* unknown_00FC;
      /* 0100 */ UIWindow* unknown_0100;
      /* 0104 */ UIWindow* m_routeFooter;
      /* 0108 */ UIWindow* m_searchFooter;
      /* 010C */ UIWindow* unknown_010C;
      /* 0110 */ UIWindow* unknown_0110;
    };
  };
  /* 0114 */ UICheckBox* unknown_0114;
  /* 0118 */ UICheckBox* unknown_0118;
  /* 011C */ UICheckBox* unknown_011C;
  /* 0120 */ UIBitmapButton* unknown_0120;
  /* 0124 */ NavigationWndInfo* m_saved;
  /* 0128 */ UIResizer* unknown_0128;
  /* 012C */ std::vector<std::string> m_history;

  UIButtonContext* m_controls;
  UIListBoxContext* m_listContext;
  UIComboBoxHeaderContext* m_comboContext;
  UIEditContext* m_editContext;
};

#endif
