#ifndef RAGNAROK_SOURCE_UI_MACRO_UIMACROUSERINFOWND_H
#define RAGNAROK_SOURCE_UI_MACRO_UIMACROUSERINFOWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>
#include <utility>
#include <vector>

class UIBitmapButton;
class UIButton;
class UIScrollBar;
class UITextButton;

class UIMacroUserInfoWnd : public UIFrameWnd {
public:
  UIMacroUserInfoWnd();
  bool IsTransmitMouseInput() override;
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(
    UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
  ) override;

  /* 00B4 */ std::string m_name;
  /* 00CC */ std::string m_message;
};

struct MacroReporterEntry {
  MacroReporterEntry() = default;
  MacroReporterEntry(std::string name, int id, short padding_001C);

  /* 0000 */ std::string name;
  /* 0018 */ int id;
  /* 001C */ short reportValue;
};

struct MacroReporterState : MacroReporterWndInfo {
  /* 0010 */ unsigned char padding_0010;
  /* 0011 */ bool activeOnly;
  /* 0014 */ std::vector<MacroReporterEntry> entries;
};

class UIMacroReporterWnd : public UIFrameWnd {
public:
  UIMacroReporterWnd();
  void StoreInfo() override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnWheel(int delta) override;
  int SendMsg(
    UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4
  ) override;

  void UpdateScrollBar();
  bool ValidateTarget(unsigned char notify);
  bool ValidateNames(unsigned char notify);
  void AddPickedActor(MacroReporterEntry* entry);
  void AddActorByAID(int aid);
  void AddActorRow(int aid, std::string name);
  std::string EllipsizeText(int maxWidth, const char* text, int font, int height, unsigned char bold);
  void SendMacroUserInfoPacket(std::uint16_t first, std::uint16_t second, std::uint8_t third);

  /* 00B4 */ std::vector<std::pair<UIBitmapButton*, UITextButton*>> m_rows;
  /* 00C0 */ UIScrollBar* m_scrollBar;
  /* 00C4 */ bool m_scrollable;
  /* 00C8 */ int scrollBarWidth;
  /* 00CC */ int m_firstRow;
  /* 00D0 */ int m_visibleRows;
  /* 00D4 */ UIWindow* activeCheckBox;
  /* 00D8 */ UIButton* m_reportButton;
  /* 00DC */ UIWindow* m_tabs[4];
  /* 00EC */ int m_mode;
  /* 00F0 */ UIWindow* m_edit;
  /* 00F4 */ int reportCount;
  /* 00F8 */ int maxReportCount;
  /* 00FC */ MacroReporterState* m_state;
  /* 0100 */ std::vector<MacroReporterEntry> m_entries;
  /* 010C */ bool activeOnly;
  /* 0110 */ std::vector<UIButton*> m_removedRows;
  /* 011C */ UIWindow* m_fields[10];
  /* 0144 */ std::string m_names[10];
  /* 0234 */ std::string m_target;
  /* 024C */ bool m_namesValid;
  /* 024D */ bool m_targetValid;
  /* 0250 */ int m_nameCount;
};

#endif
