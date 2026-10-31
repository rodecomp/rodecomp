#ifndef RAGNAROK_SOURCE_UI_GUILD_UIGUILD_STORAGE_LOG_H
#define RAGNAROK_SOURCE_UI_GUILD_UIGUILD_STORAGE_LOG_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>
#include <vector>

class UIStaticText;
class UITabControl;

struct GuildStorageLogEntry {
  /* 0000 */ int unknown_0000;
  /* 0004 */ int unknown_0004;
  /* 0008 */ unsigned char unknown_0008;
  /* 0009 */ char unknown_0009[24];
  /* 0021 */ char unknown_0021[27];
  /* 003C */ std::string unknown_003C;
  /* 0054 */ unsigned char unknown_0054[0x1C];
  /* 0070 */ unsigned char unknown_0070;
  /* 0071 */ unsigned char unknown_0071[3];
};

extern std::vector<GuildStorageLogEntry> g_guildStorageLogs[3];

class UIGuild_Storage_Log : public UIFrameWnd {
public:
  UIGuild_Storage_Log(UIWindowContext& windows, UIFrameContext& frames);
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00B4 */ UIStaticText* m_dateTexts[10];
  /* 00DC */ UIStaticText* m_nameTexts[10];
  /* 0104 */ UIStaticText* m_itemTexts[10];
  /* 012C */ UIStaticText* m_countTexts[10];
  /* 0154 */ int m_category;
  /* 0158 */ UITabControl* unknown_0158;
  /* 015C */ unsigned m_page;
  /* 0160 */ unsigned m_pageCounts[3];
  /* 016C */ unsigned m_offset;
  /* 0170 */ int m_state;
  /* 0174 */ int unknown_0174;
  /* 0178 */ std::uint32_t unknown_0178[3];
  /* 0184 */ std::uint32_t* unknown_0184;
  /* 0188 */ std::uint32_t* unknown_0188;
  /* 018C */ std::uint32_t* unknown_018C;

  void FillRows();
};

#endif
