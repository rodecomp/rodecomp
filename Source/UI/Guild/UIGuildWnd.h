#ifndef RAGNAROK_SOURCE_UI_GUILD_UIGUILDWND_H
#define RAGNAROK_SOURCE_UI_GUILD_UIGUILDWND_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>

class UIButton;
struct UIButtonContext;

class UIGuildWnd : public UIFrameWnd {
public:
  void Paint() override;
  void Slot25(int x, int y) override;
  virtual int Tab() = 0;

protected:
  UIButtonContext* m_controls;
};

class UIGuildBanishedMemberWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Slot35(int delta) override;

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x74];
  /* 0128 */ UIWindow* m_scrollBar;
  /* 012C */ unsigned char unknown_012C[0xC];
  /* 0138 */ int m_visibleRows;
  /* 013C */ int m_scrollable;
};

class UIGuildInfoManageWnd : public UIGuildWnd {
public:
  int Tab() override;
};

class UIGuildMemberManageWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Slot19() override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x94];
  /* 0148 */ UIWindow* m_scrollBar;
  /* 014C */ UIButton* m_button;
  /* 0150 */ unsigned char unknown_0150[0xC];
  /* 015C */ int m_scrollTop;
  /* 0160 */ int m_hoverRow;
  /* 0164 */ int m_columns;
  /* 0168 */ int m_visibleRows;
  /* 016C */ int unknown_016C;
  /* 0170 */ int m_scrollable;
  /* 0174 */ int m_rowSpacing;
  /* 0178 */ int unknown_0178;
  /* 017C */ unsigned int m_rowCount;
  /* 0180 */ unsigned char unknown_0180[0x18];
  /* 0198 */ std::uint32_t m_buttonTime;
};

class UIGuildNoticeWnd : public UIGuildWnd {
public:
  void Slot25(int x, int y) override;
};

class UIGuildPositionManageWnd : public UIGuildWnd {
public:
  void Slot19() override;
  void Slot35(int delta) override;

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x150];
  /* 0204 */ UIWindow* m_scrollBar;
  /* 0208 */ UIButton* m_button;
  /* 020C */ unsigned char unknown_020C[0xC];
  /* 0218 */ int m_visibleRows;
  /* 021C */ int m_scrollable;
  /* 0220 */ unsigned char unknown_0220[0x10];
  /* 0230 */ std::uint32_t m_buttonTime;
};

class UIGuildSkillWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x20];
  /* 00D4 */ UIWindow* m_scrollBar;
  /* 00D8 */ unsigned char unknown_00D8[0x68];
  /* 0140 */ int m_visibleRows;
  /* 0144 */ int m_scrollable;
};

class UIGuildTotalInfoWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Slot35(int delta) override;

protected:
  /* 00B4 */ unsigned char unknown_00B4[0x4C];
  /* 0100 */ UIWindow* m_scrollBar;
  /* 0104 */ unsigned char unknown_0104[0xC];
  /* 0110 */ int m_visibleRows;
  /* 0114 */ int m_scrollable;
};

#endif
