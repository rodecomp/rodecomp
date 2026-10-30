#ifndef RAGNAROK_SOURCE_CLIENT_MODE_CGAMEMODE_H
#define RAGNAROK_SOURCE_CLIENT_MODE_CGAMEMODE_H

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeDragState.h"
#include "Contents/ActorNameCard.h"

#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <list>
#include <map>
#include <string>
#include <unordered_set>
#include <vector>

class CView;
class CCorpse;
struct ActorNameAllPacket;
struct GameOptionEntry;
struct CWorld;

class CGameMode : public CMode {
public:
  /* 0058 */ unsigned char unknown_0058[0x10];
  /* 0068 */ char mapName[16];
  /* 0078 */ unsigned char unknown_0078[0x54];
  /* 00CC */ CWorld* scene;
  /* 00D0 */ CView* camera;
  /* 00D4 */ unsigned char unknown_00D4[0x8C];
  /* 0160 */ CActorNameCardHolder nameCard;
  /* 0194 */ std::map<std::uint32_t, int> unknown_0194;
  /* 019C */ std::map<std::uint32_t, std::pair<int, int>> unknown_019C;
  /* 01A4 */ std::list<int> unknown_01A4;
  /* 01AC */ std::map<std::uint32_t, std::uint32_t> unknown_01AC;
  /* 01B4 */ unsigned char unknown_01B4[0xE0];
  /* 0294 */ char unknown_0294[0x20];
  /* 02B4 */ UIWindow* itemWindow;
  /* 02B8 */ unsigned char unknown_02B8[0x28];
  /* 02E0 */ int unknown_02E0;
  /* 02E4 */ unsigned char unknown_02E4[0x08];
  /* 02EC */ std::string unknown_02EC;
  /* 0304 */ unsigned char unknown_0304[0x04];
  /* 0308 */ ModeDragState drag;
  /* 03C8 */ unsigned char unknown_03C8[0x1FC];
  /* 05C4 */ int unknown_05C4;
  /* 05C8 */ int unknown_05C8;
  /* 05CC */ unsigned char unknown_05CC[0x04];
  /* 05D0 */ UIWindow* unknown_05D0;
  /* 05D4 */ unsigned char unknown_05D4[0x54];
  /* 0628 */ std::unordered_set<int> unknown_0628;
  /* 0648 */ unsigned char unknown_0648[0x28];

  std::string MapName();
  void WorldCell(float x, float z, int& column, int& row) const;
  void SubcellPosition(int column, int row, int subColumn, int subRow, float& x, float& z) const;
  void PlayerCell(int* column, int* row) const;
  bool CanFindPath(int sx, int sy, int cellX, int cellY, int dx, int dy);
  bool IsAllowedPair(int kind, int value);
  bool IsWithinRange(const int& x1, const int& y1, const int& x2, const int& y2, int range);
  bool IsRegisterCell(int x, int y);
  bool HasUnknown01A4(int value);
  void ReceiveActorNameAll(const ActorNameAllPacket* packet);
  void ReceiveColoredNotice(const unsigned char* packet);
  void ReceiveActorText(const unsigned char* packet);
  void ReceiveRentalItemTime(const unsigned char* packet);
  void ReceiveActorSequenceDeadline(const unsigned char* packet);
  void ReceiveActorSignal(const unsigned char* packet);
  void IgnorePacket(const unsigned char* packet);
  void ReceiveHuntingList(const unsigned char* packet);
  void ReceiveEquipAck(const unsigned char* packet);
  void ReceivePetAct(unsigned char* packet);
  void PlayBgm(unsigned char type, std::string name);
  void ReceiveNpcBgm(const unsigned char* packet);
  void IgnoreRemovedPacket1(const unsigned char* packet);
  void IgnoreRemovedPacket2(const unsigned char* packet);
  void ReceiveBillingInfo(const unsigned char* packet);
  bool NotifyItemResult(int result);
  void NotifyItemBounds();
  CCorpse* SpawnCorpse(float x, float y, float z, const char* action, const char* sprite, int animation, int motion, float angle);
  std::uint32_t FindUnknown01AC(std::uint32_t key);
  std::string ActorName(std::uint32_t id);
  std::uint32_t ActorNameColor(int job, std::uint32_t aid);
  const CActorNameCard& LookupNameCard(std::uint32_t id);
  const char* DragItemName();
  void PushUnknown01A4(int value);
  bool IsOverWeight(bool notify);
  void CompleteUnknown0194(std::uint32_t id, const std::string& text);
  std::string GetActiveSkillEffectTimeText(int skillId, int* activeSkillId);
  bool IsActiveSkillEffectId(std::uint32_t id);
  bool IsExcludedSkillEffectId(std::uint32_t id);
  int SubD07740(int id, int defaultResult);
  bool AppendGuildEmblem(std::vector<char>& output, std::vector<std::string>& names, int guildId, int unused);
  void ReceiveResultNotice(int result);
  void RefreshWindow16(const unsigned char* packet);
  void ReceiveWindow154Close(std::int16_t value);
  void MoveWindow16(int x, int y);
  void ResizeWindow16(int width, int height);
  void ReceiveNotice1950(const unsigned char* packet);
  void ReceiveActorPairFreeze(const unsigned char* packet);
  void ReceiveEffectRemove(const unsigned char* packet);
  void ReceiveActorEffect347(const unsigned char* packet);
  void ReceiveUnknown0294Text(const unsigned char* packet);
  void ReceiveToggle128(int enabled);
  void PlaceWindow16(unsigned int xPercent, unsigned int yPercent);
  void ReceiveUnknown7FBC(const unsigned char* packet);
  void ReceiveActorMessage144(const unsigned char* packet);
  void StopActorMove(std::uint32_t aid, std::int16_t x, std::int16_t y, unsigned char flag);
  void NotifyResult4138(int result);
  void ReceiveGuildUnknown0114List(const unsigned char* packet);
  void ReceiveClanLeave(const unsigned char* packet);
  void ReceiveClanInfo(const unsigned char* packet);
  void ReceiveGameOptions(int count, const GameOptionEntry* entries);
  void ReceiveActorClanName(const unsigned char* packet);
  void ReceiveClanMemberCount(const unsigned char* packet);
  void ReceiveGuildEmblemVersion(std::uint32_t aid, int guildId, int emblemVersion);
  void ReceiveRuneActivation(const unsigned char* packet);
  void ReceiveRuneSetGrades(const unsigned char* packet);
  void ReceiveRuneRewardResult(int result, int tagId, std::int64_t id, int step);
  void ReceiveRuneRewardSteps(const unsigned char* packet);
  void NotifyWindow4F258(int result);
  void ReceiveNamedResult(std::uint32_t aid, std::string name, int result);
  void ReceiveBasicInfoResult(unsigned char result);
  void ReceiveUnknown0454Result(const unsigned char* packet);
  std::string MakeSimulatorFolder(const std::string& folder);
  std::FILE* OpenPackageSimulatorFile(const std::string& folder, int itemId, std::string& itemName);
  std::FILE* OpenSimulatorFile(const std::string& folder, const std::string& name);
  void ReceiveRandomUpgradeSimulation(const unsigned char* packet);
  void ReceiveEnchantSimulation(const unsigned char* packet);
  void ReceivePackageSimulation(const unsigned char* packet);
  void ReceiveWindow154Result(std::int16_t value);
  void UpdateUnknown0628(int id, bool add);
  void IgnoreRemovedPacket3(const unsigned char* packet);
  void IgnoreRemovedPacket4(const unsigned char* packet);
  void IgnoreRemovedPacket5(const unsigned char* packet);
  void ReceiveItem1738Add(const unsigned char* packet);
  void ReceiveItem1738AddOptions(const unsigned char* packet);
  void ReceiveItem1738AddGrade(const unsigned char* packet);
  void ReceiveItem1740Add(const unsigned char* packet);
  void ReceiveItem1740AddOptions(const unsigned char* packet);
  void ReceiveItem1740AddGrade(const unsigned char* packet);
  void ReceiveUnknown05C4Result(const unsigned char* packet);
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(CGameMode, mapName) == 0x68);
static_assert(offsetof(CGameMode, unknown_0194) == 0x194);
static_assert(offsetof(CGameMode, unknown_019C) == 0x19C);
static_assert(offsetof(CGameMode, unknown_01A4) == 0x1A4);
static_assert(offsetof(CGameMode, unknown_01AC) == 0x1AC);
static_assert(offsetof(CGameMode, scene) == 0xCC);
static_assert(offsetof(CGameMode, camera) == 0xD0);
static_assert(offsetof(CGameMode, nameCard) == 0x160);
static_assert(offsetof(CGameMode, unknown_0294) == 0x294);
static_assert(offsetof(CGameMode, drag) == 0x308);
static_assert(offsetof(CGameMode, unknown_05C4) == 0x5C4);
static_assert(offsetof(CGameMode, unknown_05D0) == 0x5D0);
static_assert(offsetof(CGameMode, unknown_0628) == 0x628);
static_assert(sizeof(CGameMode) == 0x670);
#endif

#endif
