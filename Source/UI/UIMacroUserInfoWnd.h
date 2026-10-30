#ifndef RAGNAROK_SOURCE_UI_UIMACROUSERINFOWND_H
#define RAGNAROK_SOURCE_UI_UIMACROUSERINFOWND_H

#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <utility>
#include <vector>

class UIButton;
class UIScrollBar;
struct UIButtonContext;

class UIMacroUserInfoWnd : public UIFrameWnd {
public:
  bool Slot08() override;
  void Slot15(int width, int height) override;
  void Paint() override;

  /* 00B4 */ std::string m_name;
  /* 00CC */ std::string m_message;
  UIButtonContext* m_controls;
};

struct MacroReporterEntry {
  /* 0000 */ std::string name;
  /* 0018 */ int id;
  /* 001C */ short unknown_001C;
};

class UIMacroReporterWnd : public UIFrameWnd {
public:
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;

  void UpdateScrollBar();
  bool ValidateTarget(unsigned char notify);

  /* 00B4 */ std::vector<std::pair<UIButton*, UIButton*>> m_rows;
  /* 00C0 */ UIScrollBar* m_scrollBar;
  /* 00C4 */ bool m_scrollable;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int m_firstRow;
  /* 00D0 */ int m_visibleRows;
  /* 00D4 */ UIWindow* unknown_00D4;
  /* 00D8 */ UIButton* m_reportButton;
  /* 00DC */ UIWindow* m_tabs[4];
  /* 00EC */ int m_mode;
  /* 00F0 */ UIWindow* m_edit;
  /* 00F4 */ int unknown_00F4;
  /* 00F8 */ int unknown_00F8;
  /* 00FC */ void* unknown_00FC;
  /* 0100 */ std::vector<MacroReporterEntry> m_entries;
  /* 010C */ bool unknown_010C;
  /* 0110 */ std::vector<UIButton*> m_removedRows;
  /* 011C */ UIWindow* m_fields[10];
  /* 0144 */ std::string m_names[10];
  /* 0234 */ std::string m_target;
  /* 024C */ bool m_namesValid;
  /* 024D */ bool m_targetValid;
  /* 0250 */ int m_nameCount;
  UIButtonContext* m_controls;
};

#endif
