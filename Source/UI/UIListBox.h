#ifndef RAGNAROK_SOURCE_UI_UILISTBOX_H
#define RAGNAROK_SOURCE_UI_UILISTBOX_H

#include "UI/UIWindow.h"

#include <string>
#include <vector>

struct FrameFocus;
class ResourceMgr;
class UIScrollBar;
class WindowManagerWindows;

struct UIListBoxContext {
  FrameFocus& capture;
  ResourceMgr& resources;
  FontCache& fonts;
  const TextLocale& locale;
  const unsigned short& codePage;
  WindowManagerWindows& windows;
};

// RTTI UIListBox. Children own both scrollbars; this
// vector owns the line strings. Original x86 size 212, source size 220.
class UIListBox : public UIWindow {
public:
  UIListBox(UIWindowContext& windows, UIListBoxContext& list);
  ~UIListBox() override;
  void Resize(int width, int height) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot35(int value) override;
  int SendMsg(
    UIWindow* sender,
    int message,
    std::intptr_t argument1,
    std::intptr_t argument2,
    std::intptr_t argument3,
    std::intptr_t argument4
  ) override;

  virtual void Configure(int x, int y, int width, int height, unsigned char transparent); // 53
  virtual void AddLine(const char* text);                                                 // 54
  virtual void Clear();                                                                   // 55
  virtual void Refresh();                                                                 // 56
  int LineAt(int x, int y) const;

  /* 007C */ int backgroundRed;
  /* 0080 */ int backgroundGreen;
  /* 0084 */ int backgroundBlue;
  /* 0088 */ std::vector<std::string> lines;
  /* 0094 */ int selected;
  /* 0098 */ int topLine;
  /* 009C */ int horizontalOffset;
  /* 00A0 */ int contentWidth;
  /* 00A4 */ int verticalOverflow;
  /* 00A8 */ int bottomMargin;
  /* 00AC */ UIScrollBar* verticalBar;
  /* 00B0 */ UIScrollBar* horizontalBar;
  /* 00B4 */ int transparent;
  /* 00B8 */ int lineHeight;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ std::uint32_t selectionColor;
  /* 00C4 */ unsigned char selectable;
  /* 00C8 */ std::uint32_t textColor;
  /* 00CC */ unsigned char centerVertically;
  /* 00D0 */ int textInset;

private:
  UIListBoxContext* m_listContext;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(UIListBox) == 220);
#endif

#endif
