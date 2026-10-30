#include "Contents/FlyingShipRouteInfo.h"

// OFFSET: 0x009105F0
// FUNCTION: fn_12503
CFlyingShipRouteInfo::CFlyingShipRouteInfo()
    : unknown_0010(0), unknown_0014(0), unknown_0018(0), unknown_001C(0), unknown_0020(0), unknown_0024(0), unknown_0028(1), unknown_002C(0), unknown_0030(0),
      unknown_0034(0) {
}

// OFFSET: 0x00910A70
// FUNCTION: fn_12507
FlyingShipRouteEntry::FlyingShipRouteEntry(FlyingShipRouteEntry&& other) noexcept = default;
