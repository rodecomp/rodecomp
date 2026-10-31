#ifndef RAGNAROK_SOURCE_UI_GUILD_UIGUILD_STORAGE_LOG_H
#define RAGNAROK_SOURCE_UI_GUILD_UIGUILD_STORAGE_LOG_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <vector>

class UIStaticText;

struct GuildStorageLogEntry {
  /* 0000 */ unsigned char unknown_0000[0x74];
};

extern std::vector<GuildStorageLogEntry> g_guildStorageLogs[3];

class UIGuild_Storage_Log : public UIFrameWnd {
public:
  UIGuild_Storage_Log(UIWindowContext& windows, UIFrameContext& frames);
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00B4 */ UIStaticText* m_dateTexts[10];
  /* 00DC */ UIStaticText* m_nameTexts[10];
  /* 0104 */ UIStaticText* m_itemTexts[10];
  /* 012C */ UIStaticText* m_countTexts[10];
  /* 0154 */ int m_category;
  /* 0158 */ std::uint32_t unknown_0158;
  /* 015C */ unsigned m_page;
  /* 0160 */ unsigned m_pageCounts[3];
  /* 016C */ unsigned m_offset;
  /* 0170 */ int m_state;
  /* 0174 */ int unknown_0174;
  /* 0178 */ std::uint32_t unknown_0178[3];
  /* 0184 */ int unknown_0184;
  /* 0188 */ int unknown_0188;
  /* 018C */ int unknown_018C;
};

#endif
