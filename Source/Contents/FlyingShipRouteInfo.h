#ifndef RAGNAROK_SOURCE_CONTENTS_FLYINGSHIPROUTEINFO_H
#define RAGNAROK_SOURCE_CONTENTS_FLYINGSHIPROUTEINFO_H

#include <cstddef>
#include <string>
#include <vector>

class UIRoMapWnd;
struct WorldViewRecord;

struct FlyingShipRouteEntry {
  /* 0000 */ std::string name;
  /* 0018 */ int areaBounds[4];
  /* 0028 */ int areaFlags;
  /* 002C */ float screenPosition[2];
  /* 0034 */ float width;
  /* 0038 */ float height;

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
  __declspec(noinline) bool UpdateRoute(WorldViewRecord* record);
  void Update(UIRoMapWnd* wnd, float elapsed);

  /* 0004 */ std::vector<FlyingShipRouteEntry> entries;
  /* 0010 */ int unknown_0010;
  /* 0014 */ int unknown_0014;
  /* 0018 */ int segmentIndex;
  /* 001C */ float segmentProgress;
  /* 0020 */ float segmentLength;
  /* 0024 */ float travelledDistance;
  /* 0028 */ unsigned char routeReset;
  /* 002C */ int routeType;
  /* 0030 */ int lineColor;
  union {
    /* 0034 */ short routeState;
    struct {
      /* 0034 */ unsigned char m_moving;
      /* 0035 */ unsigned char m_routeBuilt;
    };
  };
  /* 0038 */ std::string lineTextureName;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(FlyingShipRouteEntry) == 0x3C);
static_assert(offsetof(FlyingShipRouteEntry, areaBounds) == 0x18);
static_assert(offsetof(FlyingShipRouteEntry, height) == 0x38);
static_assert(sizeof(CFlyingShipRouteInfo) == 0x50);
static_assert(offsetof(CFlyingShipRouteInfo, entries) == 0x04);
static_assert(offsetof(CFlyingShipRouteInfo, routeReset) == 0x28);
static_assert(offsetof(CFlyingShipRouteInfo, routeState) == 0x34);
static_assert(offsetof(CFlyingShipRouteInfo, lineTextureName) == 0x38);
#endif

#endif
