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
  UIMacroUserInfoWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  bool Slot08() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  int SendMsg(
    UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
  ) override;

  /* 00B4 */ std::string m_name;
  /* 00CC */ std::string m_message;
  UIButtonContext* m_controls;
};

struct MacroReporterEntry {
  MacroReporterEntry() = default;
  MacroReporterEntry(std::string name, int id, short unknown_001C);

  /* 0000 */ std::string name;
  /* 0018 */ int id;
  /* 001C */ short unknown_001C;
};

struct MacroReporterState {
  /* 0000 */ int width;
  /* 0004 */ int height;
  /* 0008 */ int x;
  /* 000C */ int y;
  /* 0010 */ unsigned char unknown_0010;
  /* 0011 */ bool unknown_0011;
  /* 0014 */ std::vector<MacroReporterEntry> entries;
};

class UIMacroReporterWnd : public UIFrameWnd {
public:
  UIMacroReporterWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot11() override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(
    UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
  ) override;

  void UpdateScrollBar();
  bool ValidateTarget(unsigned char notify);
  bool ValidateNames(unsigned char notify);
  void sub_7EF2C0(MacroReporterEntry* entry);
  void sub_7EF2E0(int aid);
  void sub_7EF430(int aid, std::string name);
  std::string sub_7EF680(int maxWidth, const char* text, int font, int height, unsigned char bold);

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
  /* 00FC */ MacroReporterState* m_state;
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
