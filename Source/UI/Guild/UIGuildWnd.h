#ifndef RAGNAROK_SOURCE_UI_GUILD_UIGUILDWND_H
#define RAGNAROK_SOURCE_UI_GUILD_UIGUILDWND_H

#include "UI/Window/UIFrameWnd.h"

#include "Client/Input/Mouse.h"
#include "Contents/ForbiddenWords.h"
#include "Contents/GuildInfo.h"
#include "Contents/PlayerNameDictionary.h"
#include "Contents/Skills/SkillInfo.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Renderer.h"
#include "Render/Textures/SpriteAtlas.h"
#include "Render/Textures/SpritePixels.h"
#include "Render/Textures/Texture.h"
#include "Resource/PaletteResource.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"
#include "UI/Party/UIMiniPartyWnd.h"
#include "UI/Text/UITextViewer.h"
#include "UI/Window/WindowBorder.h"

#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <list>
#include <map>
#include <string>
#include <vector>

class UIBitmapButton;
class UIButton;
class UICheckBox;
class UIEditCtrl;
class UIListBoxAgit;
class UIListBoxAgitMenu;
class UIScrollBar;
class UIStaticText;
struct UIButtonContext;
struct UIListBoxContext;
struct GuildWndInfo;
struct ForbiddenWords;

bool CompareWideText(const char* left, const char* right);

struct GuildPositionEntry {
  /* 0000 */ int id;
  /* 0004 */ int unknown_0004;
  /* 0008 */ std::string name;
  /* 0020 */ int canInvite;
  /* 0024 */ int canExpel;
  /* 0028 */ int tax;
  /* 002C */ int canStorage;
};

struct GuildWndNameEntry {
  /* 0000 */ std::string name;
  /* 0018 */ int id;
};

struct GuildMemberManageEntry {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
};

class UIGuildWnd : public UIFrameWnd {
public:
  UIGuildWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot11() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  virtual int Tab() = 0;

protected:
  void CreateTabs();
  std::string StripNamelessTag(const std::string& name);

  /* 00B4 */ GuildWndInfo* m_saved;
  /* 00B8 */ UIStaticText* m_tabs[6];
  UIButtonContext* m_controls;
};

class UIGuildBanishedMemberWnd : public UIGuildWnd {
public:
  UIGuildBanishedMemberWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  int Tab() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void UpdateScroll();

protected:
  /* 00D0 */ UIStaticText* m_names[11];
  /* 00FC */ UIStaticText* m_reasons[11];
  /* 0128 */ UIScrollBar* m_scrollBar;
  /* 012C */ int m_scrollTop;
  /* 0130 */ int m_hoverRow;
  /* 0134 */ int m_columns;
  /* 0138 */ int m_visibleRows;
  /* 013C */ int m_scrollable;
  ForbiddenWords* m_forbiddenWords;
};

class UIGuildInfoManageWnd : public UIGuildWnd {
public:
  UIGuildInfoManageWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  int Tab() override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;

protected:
  void RefreshAgitList();
  void HideAgitMenu();

  /* 00D0 */ UIBitmapButton* m_editButton;
  /* 00D4 */ UIBitmapButton* m_disbandButton;
  /* 00D8 */ std::list<std::string> unknown_00D8;
  /* 00E0 */ std::vector<GuildWndNameEntry> unknown_00E0;
  /* 00EC */ int unknown_00EC;
  /* 00F0 */ int unknown_00F0;
  /* 00F4 */ std::uint32_t m_guildId;
  /* 00F8 */ int m_allyRow;
  /* 00FC */ int m_hostileRow;
  /* 0100 */ UIListBoxAgit* m_agitList;
  /* 0104 */ UIListBoxAgitMenu* m_agitMenu;
  UIListBoxContext* m_listContext;
};

class UIGuildMemberManageWnd : public UIGuildWnd {
public:
  UIGuildMemberManageWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  int Tab() override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void UpdateScroll();

protected:
  bool DrawMember(GuildInfo::Member& member, int& index, int row);
  bool GetMember(int index, GuildInfo::Member& member);
  void SortMembers(int column);

  /* 00D0 */ UIStaticText* m_rowTexts[4][6];
  /* 0130 */ UIBitmapButton* m_rowButtons[6];
  /* 0148 */ UIScrollBar* m_scrollBar;
  /* 014C */ UIButton* m_button;
  /* 0150 */ std::vector<GuildWndNameEntry> unknown_0150;
  /* 015C */ int m_scrollTop;
  /* 0160 */ int m_hoverRow;
  /* 0164 */ int m_columns;
  /* 0168 */ int m_visibleRows;
  /* 016C */ int unknown_016C;
  /* 0170 */ int m_scrollable;
  /* 0174 */ int m_rowSpacing;
  /* 0178 */ std::list<GuildInfo::Member> m_members;
  /* 0180 */ std::map<int, GuildMemberManageEntry> unknown_0180;
  /* 0188 */ int m_sortColumn;
  /* 018C */ std::map<int, int> m_sortOrders;
  /* 0194 */ UICheckBox* m_onlineCheck;
  /* 0198 */ std::uint32_t m_buttonTime;
};

class UIGuildNoticeWnd : public UIGuildWnd {
public:
  UIGuildNoticeWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Paint() override;
  void Slot25(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;

protected:
  /* 00D0 */ UIEditCtrl* m_titleEdit;
  /* 00D4 */ UIEditCtrl* m_bodyEdit;
  ForbiddenWords* m_forbiddenWords;
};

class UIGuildPositionManageWnd : public UIGuildWnd {
public:
  UIGuildPositionManageWnd(UIWindowContext& windows, UIFrameContext& frames, UIButtonContext& controls);
  void Slot19() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;
  void Invalidate() override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void UpdateScroll();

protected:
  int PositionIndexAt(int x, int y);
  GuildPositionEntry& PositionAt(int index, int* found);

  /* 00D0 */ UICheckBox* unknown_00D0[11];
  /* 00FC */ UICheckBox* unknown_00FC[11];
  /* 0128 */ UICheckBox* unknown_0128[11];
  /* 0154 */ UIEditCtrl* unknown_0154[11];
  /* 0180 */ UIEditCtrl* unknown_0180[11];
  /* 01AC */ UIStaticText* unknown_01AC[11];
  /* 01D8 */ UIStaticText* unknown_01D8[11];
  /* 0204 */ UIScrollBar* m_scrollBar;
  /* 0208 */ UIButton* m_button;
  /* 020C */ int m_scrollTop;
  /* 0210 */ int m_hoverIndex;
  /* 0214 */ int m_columns;
  /* 0218 */ int m_visibleRows;
  /* 021C */ int m_scrollable;
  /* 0220 */ std::list<GuildPositionEntry> m_positions;
  /* 0228 */ std::map<int, GuildPositionEntry> m_positionMap;
  /* 0230 */ std::uint32_t m_buttonTime;
};

class UIGuildSkillWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void UpdateScroll();

protected:
  int SkillIndexAt(int x, int y);
  CSkillInfo SkillAt(int index);
  CSkillInfo VisibleSkillAt(int row);
  void UpdateSkillButtons();

  /* 00D0 */ unsigned char unknown_00D0[0x04];
  /* 00D4 */ UIScrollBar* m_scrollBar;
  /* 00D8 */ unsigned char unknown_00D8[0x08];
  /* 00E0 */ UIWindow* unknown_00E0[7];
  /* 00FC */ UIWindow* unknown_00FC[7];
  /* 0118 */ UIWindow* m_levelUpButtons[7];
  /* 0134 */ int m_hoverIndex;
  /* 0138 */ int m_scrollTop;
  /* 013C */ int m_columns;
  /* 0140 */ int m_visibleRows;
  /* 0144 */ int m_scrollable;
  /* 0148 */ std::list<CSkillInfo> m_skills;
  /* 0150 */ std::vector<MiniPartyInfoEntry> m_menuItems;
};

class UIGuildTotalInfoWnd : public UIGuildWnd {
public:
  int Tab() override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot25(int x, int y) override;
  void Slot35(int delta) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void UpdateScroll();

protected:
  /* 00D0 */ UIStaticText* m_rows[12];
  /* 0100 */ UIScrollBar* m_scrollBar;
  /* 0104 */ int m_scrollTop;
  /* 0108 */ int m_hoverRow;
  /* 010C */ int m_columns;
  /* 0110 */ int m_visibleRows;
  /* 0114 */ int m_scrollable;
};

#endif
