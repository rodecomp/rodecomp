#ifndef RAGNAROK_SOURCE_UI_WINDOW_UIHOTKEYWND_H
#define RAGNAROK_SOURCE_UI_WINDOW_UIHOTKEYWND_H

#include "Contents/Hotkeys/HotkeyInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <map>

class UIBalloonText;
class UIButton;
class UITabControl;
class UIWindow;

class UIHotKeyWnd : public UIFrameWnd {
public:
  struct HotkeyEntry {
    int index;
    HotkeyInfo info;
  };

  UIHotKeyWnd();
  void OnCreate(int width, int height) override;
  void OnLBtnDown(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  int RowFromPoint(int x, int y);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void UpdatePageButtons();
  void SaveHotkeys();
  void SetHotkey(int tab, int index, HotkeyInfo info);
  HotkeyEntry GetHotkey(int tab, int index, bool resolve);
  void OnMouseHover(int x, int y) override;
  void ReloadHotkeys();
  bool AssignSelectedHotkey(int mode);
  int CheckHotkeyConflict();

  /* 00B4 */ int m_selectedRow;
  /* 00B8 */ int m_rowsPerPage;
  /* 00BC */ int m_titleHeight;
  /* 00C0 */ int m_buttonBarHeight;
  /* 00C4 */ int m_tabHeight;
  /* 00C8 */ int m_horizontalMargin;
  /* 00CC */ int m_verticalMargin;
  /* 00D0 */ int m_rowHeight;
  /* 00D4 */ int m_keyColumnWidth;
  /* 00D8 */ int m_descriptionColumnWidth;
  /* 00DC */ std::map<int, HotkeyInfo> hotkeys[4];
  /* 00FC */ UIBalloonText* m_balloon;
  /* 0100 */ UITabControl* m_tabs;
  /* 0104 */ int m_tab;
  /* 0108 */ UIWindow* m_optionCheckBox;
  /* 010C */ int m_page;
  /* 0110 */ int m_pageCount;
  /* 0114 */ unsigned char m_paged;
  /* 0115 */ unsigned char unknown_0115[0x3];
  /* 0118 */ UIButton* m_prevButton;
  /* 011C */ UIButton* m_nextButton;
};

#endif
