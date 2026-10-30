#ifndef RAGNAROK_SOURCE_UI_EDIT_UIEDITBOXEX_H
#define RAGNAROK_SOURCE_UI_EDIT_UIEDITBOXEX_H

#include "UI/Edit/UIEditCtrl.h"
#include "UI/Window/UIControl.h"

#include <chrono>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

struct EditLine {
  /* 0000 */ std::string text;
  /* 0018 */ int unknown_0018;
  /* 001C */ int selectionBegin;
  /* 0020 */ int selectionEnd;
};

class CUIEditCtrlEx : public UIEditCtrl3 {
public:
  void Paint() override;
  void Invalidate() override;
  void SelectAll() override;
  virtual void UpdateScrollBar();

  void RefreshPlaceholder();
  int LeadingBytesFitting(const std::string& text, int width);
  int TrailingOffsetFitting(const std::string& text, int width);

  /* 0188 */ std::uint32_t normalColor;
  /* 018C */ int unknown_018C;
  /* 0190 */ unsigned char unknown_0190;
  /* 0194 */ std::string placeholder;
  /* 01AC */ std::uint32_t placeholderColor;
  /* 01B0 */ int unknown_01B0;
  /* 01B4 */ int unknown_01B4;
  /* 01B8 */ int verticalPadding;
  /* 01BC */ std::vector<EditLine> lines;
  /* 01C8 */ unsigned char selectedAll;
  /* 01CC */ int column;
  /* 01D0 */ int line;
  /* 01D4 */ int unknown_01D4;
  /* 01D8 */ int unknown_01D8;
  /* 01DC */ unsigned char unknown_01DC;
  /* 01E0 */ std::chrono::system_clock::time_point caretStart;
  /* 01E8 */ int scrollMaximum;
  /* 01EC */ int visibleLines;
  /* 01F0 */ int unknown_01F0;
  /* 01F4 */ int scrollPosition;
  /* 01F8 */ int unknown_01F8;
};

class CUIEditBoxEx : public CUIControl {
public:
  bool Slot05() override;
  void Paint() override;

  void ResetSelection();
  void SetCursor(std::size_t position);
  int Length();

  /* 0080 */ std::uint32_t backgroundColor;
  /* 0084 */ std::uint32_t borderColor;
  /* 0088 */ int borderWidth;
  /* 008C */ int unknown_008C[4];
  /* 009C */ CUIEditCtrlEx* edit;
};

#endif
