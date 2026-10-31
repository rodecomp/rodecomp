#ifndef RAGNAROK_SOURCE_CONTENTS_FLYINGSHIPROUTEINFO_H
#define RAGNAROK_SOURCE_CONTENTS_FLYINGSHIPROUTEINFO_H

#include <cstddef>
#include <string>
#include <vector>

class UIRoMapWnd;

struct FlyingShipRouteEntry {
  /* 0000 */ std::string name;
  /* 0018 */ int unknown_0018[4];
  /* 0028 */ int unknown_0028;
  /* 002C */ float unknown_002C[2];
  /* 0034 */ float unknown_0034;
  /* 0038 */ float unknown_0038;

  FlyingShipRouteEntry() = default;
  FlyingShipRouteEntry(const FlyingShipRouteEntry& other) = default;
  FlyingShipRouteEntry(FlyingShipRouteEntry&& other) noexcept;
  FlyingShipRouteEntry& operator=(const FlyingShipRouteEntry& other) = default;
  FlyingShipRouteEntry& operator=(FlyingShipRouteEntry&& other) = default;
};

class CFlyingShipRouteInfo {
public:
  CFlyingShipRouteInfo();
  virtual ~CFlyingShipRouteInfo() = default;

  bool IsRouteMap(std::string mapName);
  void Draw(UIRoMapWnd* wnd);
  void BuildRoute(const std::vector<std::string>& mapNames);

  /* 0004 */ std::vector<FlyingShipRouteEntry> entries;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int unknown_0018;
  /* 001C */ float unknown_001C;
  /* 0020 */ int unknown_0020;
  /* 0024 */ int unknown_0024;
  /* 0028 */ unsigned char unknown_0028;
  /* 002C */ int unknown_002C;
  /* 0030 */ int unknown_0030;
  /* 0034 */ short unknown_0034;
  /* 0038 */ std::string unknown_0038;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(FlyingShipRouteEntry) == 0x3C);
static_assert(offsetof(FlyingShipRouteEntry, unknown_0018) == 0x18);
static_assert(offsetof(FlyingShipRouteEntry, unknown_0038) == 0x38);
static_assert(sizeof(CFlyingShipRouteInfo) == 0x50);
static_assert(offsetof(CFlyingShipRouteInfo, entries) == 0x04);
static_assert(offsetof(CFlyingShipRouteInfo, unknown_0028) == 0x28);
static_assert(offsetof(CFlyingShipRouteInfo, unknown_0034) == 0x34);
static_assert(offsetof(CFlyingShipRouteInfo, unknown_0038) == 0x38);
#endif

#endif
