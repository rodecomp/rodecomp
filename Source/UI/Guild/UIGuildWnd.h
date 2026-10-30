#ifndef RAGNAROK_SOURCE_UI_GUILD_UIGUILDWND_H
#define RAGNAROK_SOURCE_UI_GUILD_UIGUILDWND_H

#include "UI/Window/UIFrameWnd.h"

#include "Contents/Skills/SkillInfo.h"

#include <cstdint>
#include <list>
#include <map>
#include <string>

class UIButton;
struct UIButtonContext;
struct GuildWndInfo;

int CompareWideText(const char* left, const char* right);

struct GuildPositionEntry {
  /* 0000 */ int id;
  /* 0004 */ int unknown_0004;
  /* 0008 */ std::string name;
  /* 0020 */ int unknown_0020;
  /* 0024 */ int unknown_0024;
  /* 0028 */ int unknown_0028;
  /* 002C */ int unknown_002C;
};

class UIGuildWnd : public UIFrameWnd {
public:
  UIGuildWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot11() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  virtual int Tab() = 0;

protected:
  /* 00B4 */ GuildWndInfo* m_saved;
  /* 00B8 */ int unknown_00B8;
  /* 00BC */ int unknown_00BC;
  /* 00C0 */ int unknown_00C0;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ int unknown_00C8;
  /* 00CC */ int unknown_00CC;
  UIButtonContext* m_controls;
};

class UIGuildBanishedMemberWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Slot35(int delta) override;

protected:
  /* 00D0 */ unsigned char unknown_00D0[0x58];
  /* 0128 */ UIWindow* m_scrollBar;
  /* 012C */ unsigned char unknown_012C[0xC];
  /* 0138 */ int m_visibleRows;
  /* 013C */ int m_scrollable;
};

class UIGuildInfoManageWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Slot19() override;

protected:
  /* 00D0 */ unsigned char unknown_00D0[0x24];
  /* 00F4 */ std::uint32_t m_guildId;
  /* 00F8 */ unsigned char unknown_00F8[0x10];
};

class UIGuildMemberManageWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Slot19() override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;

protected:
  /* 00D0 */ unsigned char unknown_00D0[0x78];
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
  UIGuildPositionManageWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot19() override;
  void Slot35(int delta) override;

protected:
  /* 00D0 */ int unknown_00D0[11];
  /* 00FC */ int unknown_00FC[11];
  /* 0128 */ int unknown_0128[11];
  /* 0154 */ int unknown_0154[11];
  /* 0180 */ int unknown_0180[11];
  /* 01AC */ int unknown_01AC[11];
  /* 01D8 */ int unknown_01D8[11];
  /* 0204 */ UIWindow* m_scrollBar;
  /* 0208 */ UIButton* m_button;
  /* 020C */ unsigned char unknown_020C[0xC];
  /* 0218 */ int m_visibleRows;
  /* 021C */ int m_scrollable;
  /* 0220 */ std::list<GuildPositionEntry> m_positions;
  /* 0228 */ std::map<int, GuildPositionEntry> m_positionMap;
  /* 0230 */ std::uint32_t m_buttonTime;
};

class UIGuildSkillWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;

protected:
  int SkillIndexAt(int x, int y);
  CSkillInfo SkillAt(int index);
  CSkillInfo VisibleSkillAt(int row);

  /* 00D0 */ unsigned char unknown_00D0[0x04];
  /* 00D4 */ UIWindow* m_scrollBar;
  /* 00D8 */ unsigned char unknown_00D8[0x5C];
  /* 0134 */ int m_hoverIndex;
  /* 0138 */ int m_scrollTop;
  /* 013C */ int m_columns;
  /* 0140 */ int m_visibleRows;
  /* 0144 */ int m_scrollable;
  /* 0148 */ std::list<CSkillInfo> m_skills;
  /* 0150 */ unsigned char unknown_0150[0x0C];
};

class UIGuildTotalInfoWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Slot35(int delta) override;

protected:
  /* 00D0 */ unsigned char unknown_00D0[0x30];
  /* 0100 */ UIWindow* m_scrollBar;
  /* 0104 */ unsigned char unknown_0104[0xC];
  /* 0110 */ int m_visibleRows;
  /* 0114 */ int m_scrollable;
};

#endif
