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

  UIHotKeyWnd(UIWindowContext& windows, UIFrameContext& frames);
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;
  int RowFromPoint(int x, int y);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void UpdatePageButtons();
  void SaveHotkeys();
  void SetHotkey(int tab, int index, HotkeyInfo info);
  HotkeyEntry GetHotkey(int tab, int index, bool resolve);
  void ReloadHotkeys();
  int AssignSelectedHotkey(int mode);
  int CheckHotkeyConflict();

  /* 00B4 */ int m_selectedRow;
  /* 00B8 */ int m_rowsPerPage;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int unknown_00CC;
  /* 00D0 */ int unknown_00D0;
  /* 00D4 */ int unknown_00D4;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ std::map<int, HotkeyInfo> hotkeys[4];
  /* 00FC */ UIBalloonText* m_balloon;
  /* 0100 */ UITabControl* unknown_0100;
  /* 0104 */ int m_tab;
  /* 0108 */ UIWindow* unknown_0108;
  /* 010C */ int m_page;
  /* 0110 */ int m_pageCount;
  /* 0114 */ unsigned char unknown_0114;
  /* 0115 */ unsigned char unknown_0115[0x3];
  /* 0118 */ UIButton* m_prevButton;
  /* 011C */ UIButton* m_nextButton;
};

#endif
