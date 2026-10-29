#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_NORMALITEMSLOTS_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_NORMALITEMSLOTS_H

#include <cstddef>
#include <cstdint>

namespace Json {
  class Value;
}

#pragma pack(push, 1)
struct NormalItemSlot {
  /* 0000 */ std::uint8_t unknown_0000;
  /* 0001 */ std::uint32_t itemId;
  /* 0005 */ std::int16_t unknown_0005;
};
#pragma pack(pop)
static_assert(sizeof(NormalItemSlot) == 7);
static_assert(offsetof(NormalItemSlot, itemId) == 1);
static_assert(offsetof(NormalItemSlot, unknown_0005) == 5);

struct NormalItemSlotWindow {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
  /* 0010 */ int show;
  void Reset();
  bool IsValid(int screenWidth, int screenHeight, bool active) const;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(NormalItemSlotWindow, x) == 0x0000);
static_assert(offsetof(NormalItemSlotWindow, y) == 0x0004);
static_assert(offsetof(NormalItemSlotWindow, width) == 0x0008);
static_assert(offsetof(NormalItemSlotWindow, height) == 0x000C);
static_assert(offsetof(NormalItemSlotWindow, show) == 0x0010);
#endif

struct WindowPlacement {
  /* 0000 */ int x;
  /* 0004 */ int y;
  /* 0008 */ int width;
  /* 000C */ int height;
};

struct SkillListWndInfo : WindowPlacement {
  int IsValid() const;
};

struct StatusWndInfo : WindowPlacement {
  int IsValid() const;
};

struct SubChatHisWndInfo : WindowPlacement {
  int IsValid() const;
};

struct AdvenPartyBoardWndInfo : WindowPlacement {
  int IsValid() const;
};

struct ApplyForPartyWndInfo : WindowPlacement {
  int IsValid() const;
};

struct RegisterPartyWndInfo : WindowPlacement {
  int IsValid() const;
};

struct WhisperWndInfo : WindowPlacement {
  int IsValid() const;
};

struct MacroReporterWndInfo : WindowPlacement {
  bool IsValid() const;
};

struct PreviewEquipWndInfo : WindowPlacement {
  bool IsValid() const;
};

struct AchTracingWndInfo : WindowPlacement {
  void Reset();
};

struct BasicInfoWndInfo : WindowPlacement {
  /* 0010 */ unsigned char unknown_0010;
  /* 0011 */ unsigned char unknown_0011;
  /* 0012 */ unsigned char unknown_0012;
  /* 0013 */ unsigned char unknown_0013;

  void Reset();
};

struct CashEmotionListWndInfo : WindowPlacement {
  void Reset();
};

struct ChatRoomWndInfo : WindowPlacement {
  void Reset();
};

struct ChooseWndInfo : WindowPlacement {
  void Reset();
};

struct ClanWndInfo : WindowPlacement {
  void Reset();
};

struct CombinedCardItemCollectionWndInfo : WindowPlacement {
  void Reset();
};

struct UnusedWndInfoA : WindowPlacement {
  void Reset();
};

struct EmotionWndInfo : WindowPlacement {
  void Reset();
};

struct EquipWndInfo : WindowPlacement {
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;

  void Reset();
};

struct ExchangeAcceptWndInfo : WindowPlacement {
  void Reset();
};

struct FriendOptionWndInfo : WindowPlacement {
  void Reset();
  int IsValid() const;
};

struct GuildHelperWndInfo : WindowPlacement {
  void Reset();
};

struct GuildWndInfo : WindowPlacement {
  /* 0010 */ int unknown_0010;

  void Reset();
};

struct HomunInfoWndInfo : WindowPlacement {
  void Reset();
};

struct ItemCollectionWndInfo : WindowPlacement {
  void Reset();
};

struct ItemCompositionWndInfo : WindowPlacement {
  void Reset();
};

struct ItemStoreFindWndInfo : WindowPlacement {
  void Reset();
};

struct ItemIdentifyWndInfo : WindowPlacement {
  void Reset();
};

struct ItemParamChangeDisplayWndInfo : WindowPlacement {
  void Reset();
};

struct ItemPurchaseWndInfo : WindowPlacement {
  void Reset();
};

struct ItemSellWndInfo : WindowPlacement {
  void Reset();
};

struct ItemShopWndInfo : WindowPlacement {
  void Reset();
};

struct ItemStoreFindListInfo : WindowPlacement {
  /* 0010 */ int unknown_0010;

  void Reset();
};

struct ItemStoreFindSlotInfo : WindowPlacement {
  struct Position {
    /* 0000 */ int x;
    /* 0004 */ int y;
  };

  void Reset(int index);
};

struct ItemWndInfo : WindowPlacement {
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018;
  /* 001C */ int unknown_001C;
  /* 0020 */ int unknown_0020;
  /* 0024 */ int unknown_0024;

  void Reset();
};

struct EntryQueueWndInfo : WindowPlacement {
  void Reset();
};

struct MiniMapWndInfo : WindowPlacement {
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018;
  /* 001C */ int unknown_001C;
  /* 0020 */ int unknown_0020;

  void Reset();
};

struct MerInfoWndInfo : WindowPlacement {
  void Reset();
};

struct MerchantItemMyShopWndInfo : WindowPlacement {
  void Reset();
};

struct MerchantItemPurchaseWndInfo : WindowPlacement {
  void Reset();
};

struct MerchantItemShopWndInfo : WindowPlacement {
  void Reset(bool wide);
};

struct MerchantItemWndInfo : WindowPlacement {
  void Reset();
};

struct MerchantMirrorItemWndInfo : WindowPlacement {
  void Reset();
};

struct MerchantShopMakeWndInfo : WindowPlacement {
  void Reset();
};

struct MessengerGroupWndInfo : WindowPlacement {
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018;

  void Reset();
};

struct NCashWndInfo : WindowPlacement {
  void Reset();
};

struct UnusedWndInfoB : WindowPlacement {
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;

  void Reset();
};

struct SeekPartyWndInfo : WindowPlacement {
  void Reset();
};

struct SeekPartyListWndInfo : WindowPlacement {
  void Reset();
};

struct SeekPartyMBWndInfo : WindowPlacement {
  void Reset();
};

struct JobListWndInfo : WindowPlacement {
  void Reset();
};

struct PartySettingWndInfo : WindowPlacement {
  void Reset();
};

struct PetInfoWndInfo : WindowPlacement {
  void Reset();
};

struct QuestDisplayInfo {
  /* 0000 */ int unknown_0000;

  void Reset();
};

struct QuickSlotWndInfo : WindowPlacement {
  void Reset();
};

struct ReplayRECControlWndInfo : WindowPlacement {
  void Reset();
};

struct SayDialogWndInfo : WindowPlacement {
  void Reset();
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(WindowPlacement, height) == 0x000C);
static_assert(sizeof(WindowPlacement) == 0x10);
static_assert(sizeof(BasicInfoWndInfo) == 0x14);
static_assert(sizeof(EquipWndInfo) == 0x18);
static_assert(sizeof(ItemStoreFindListInfo) == 0x14);
static_assert(sizeof(ItemWndInfo) == 0x28);
static_assert(sizeof(MiniMapWndInfo) == 0x24);
static_assert(sizeof(MessengerGroupWndInfo) == 0x1C);
static_assert(sizeof(GuildWndInfo) == 0x14);
static_assert(sizeof(UnusedWndInfoB) == 0x18);
#endif

extern int g_defaultPlacementWidth;
extern int g_defaultPlacementHeight;

void ExportNormalItemSlots(Json::Value& data, const NormalItemSlot* slots, const NormalItemSlotWindow& window, bool active);

#endif
