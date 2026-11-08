#ifndef RAGNAROK_SOURCE_UI_GUILD_UIGUILD_STORAGE_LOG_H
#define RAGNAROK_SOURCE_UI_GUILD_UIGUILD_STORAGE_LOG_H

#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>
#include <vector>

class UIStaticText;
class UITabControl;

struct GuildStorageLogEntry {
  /* 0000 */ int logId = 0;
  /* 0004 */ int quantity = 0;
  /* 0008 */ unsigned char operation = 0;
  /* 0009 */ char characterName[24] = {};
  /* 0021 */ char timestamp[24] = {};
  /* 003C */ std::string itemName;
  /* 0054 */ unsigned char identified = 0;
  /* 0058 */ int itemType = 0;
  /* 005C */ int refineLevel = 0;
  /* 0060 */ unsigned char cards[16] = {};
  /* 0070 */ unsigned char category = 0;
  /* 0072 */ std::int16_t grade = 0;
};

extern std::vector<GuildStorageLogEntry> g_guildStorageLogs[3];

class UIGuild_Storage_Log : public UIFrameWnd {
public:
  UIGuild_Storage_Log();
  void OnCreate(int width, int height) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

protected:
  /* 00B4 */ UIStaticText* m_dateTexts[10];
  /* 00DC */ UIStaticText* m_nameTexts[10];
  /* 0104 */ UIStaticText* m_itemTexts[10];
  /* 012C */ UIStaticText* m_countTexts[10];
  /* 0154 */ int m_category;
  /* 0158 */ UITabControl* m_tabs;
  /* 015C */ unsigned m_page;
  /* 0160 */ unsigned m_pageCounts[3];
  /* 016C */ unsigned m_offset;
  /* 0170 */ int m_state;
  /* 0174 */ int m_logTextHeight;
  /* 0178 */ std::uint32_t m_categoryCounts[3];
  /* 0184 */ std::uint32_t* m_rowFlagsBegin;
  /* 0188 */ std::uint32_t* m_rowFlagsEnd;
  /* 018C */ std::uint32_t* m_rowFlagsCapacity;

  void FillLogRows(int index);
  void FillRows();
};

#endif
