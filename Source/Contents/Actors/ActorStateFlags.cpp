#include "Contents/Actors/ActorStateFlags.h"

// OFFSET: 0x00DF96E0
// FUNCTION: fn_23522
bool HasFalcon(std::uint32_t flags) {
  return (flags >> 4) & 1;
}

// OFFSET: 0x00DF96F0
// FUNCTION: fn_23523
bool HasFalconChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 4) ^ (current >> 4)) & 1;
}

// OFFSET: 0x00DF9710
// FUNCTION: fn_23524
bool IsHiding(std::uint32_t flags) {
  return (flags >> 1) & 1;
}

// OFFSET: 0x00DF9740
// FUNCTION: fn_23526
bool IsRidingPeco(std::uint32_t flags) {
  return (flags >> 5) & 1;
}

// OFFSET: 0x00DF9770
// FUNCTION: fn_23528
bool IsRidingBrownDragon(std::uint32_t flags) {
  return (flags >> 23) & 1;
}

// OFFSET: 0x00DF97A0
// FUNCTION: fn_23530
bool IsRidingGrayDragon(std::uint32_t flags) {
  return (flags >> 24) & 1;
}

// OFFSET: 0x00DF97D0
// FUNCTION: fn_23532
bool IsRidingBlueDragon(std::uint32_t flags) {
  return (flags >> 25) & 1;
}

// OFFSET: 0x00DF9800
// FUNCTION: fn_23534
bool IsRidingRedDragon(std::uint32_t flags) {
  return (flags >> 26) & 1;
}

// OFFSET: 0x00DF9830
// FUNCTION: fn_23536
bool IsRidingGreenDragon(std::uint32_t flags) {
  return (flags >> 19) & 1;
}

// OFFSET: 0x00DF9870
// FUNCTION: fn_23539
bool IsHanbokDress(std::uint32_t flags) {
  return (flags >> 27) & 1;
}

// OFFSET: 0x00DF98E0
// FUNCTION: fn_23543
bool IsWeddingDress(std::uint32_t flags) {
  return (flags >> 12) & 1;
}

// OFFSET: 0x00DF9900
// FUNCTION: fn_23545
bool IsOktoberfestDress(std::uint32_t flags) {
  return (flags >> 28) & 1;
}

// OFFSET: 0x00DF99A0
// FUNCTION: fn_23551
bool IsXmasDress(std::uint32_t flags) {
  return (flags >> 16) & 1;
}

// OFFSET: 0x00DF9A00
// FUNCTION: fn_23556
bool IsStarUnion(std::uint32_t flags) {
  return (flags >> 15) & 1;
}

// OFFSET: 0x00DF9A30
// FUNCTION: fn_23558
bool IsSummerDress(std::uint32_t flags) {
  return (flags >> 18) & 1;
}

// OFFSET: 0x00DF9A70
// FUNCTION: fn_23559
bool IsRidingWarg(std::uint32_t flags) {
  return (flags >> 21) & 1;
}

// OFFSET: 0x00DF9720
// FUNCTION: fn_23525
bool IsHidingChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 1) ^ (current >> 1)) & 1;
}

// OFFSET: 0x00DF9750
// FUNCTION: fn_23527
bool IsRidingPecoChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 5) ^ (current >> 5)) & 1;
}

// OFFSET: 0x00DF9780
// FUNCTION: fn_23529
bool IsRidingBrownDragonChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 23) ^ (current >> 23)) & 1;
}

// OFFSET: 0x00DF97B0
// FUNCTION: fn_23531
bool IsRidingGrayDragonChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 24) ^ (current >> 24)) & 1;
}

// OFFSET: 0x00DF97E0
// FUNCTION: fn_23533
bool IsRidingBlueDragonChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 25) ^ (current >> 25)) & 1;
}

// OFFSET: 0x00DF9810
// FUNCTION: fn_23535
bool IsRidingRedDragonChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 26) ^ (current >> 26)) & 1;
}

// OFFSET: 0x00DF9840
// FUNCTION: fn_23537
bool IsRidingGreenDragonChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 19) ^ (current >> 19)) & 1;
}

// OFFSET: 0x00DF9860
// FUNCTION: fn_23538
bool IsChaseWalking(std::uint32_t flags) {
  return (flags >> 14) & 1;
}

// OFFSET: 0x00DF9880
// FUNCTION: fn_23540
bool IsCloaking(std::uint32_t flags) {
  return (flags >> 2) & 1;
}

// OFFSET: 0x00DF9890
// FUNCTION: fn_23541
bool IsCloakingChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 2) ^ (current >> 2)) & 1;
}

// OFFSET: 0x00DF98B0
// FUNCTION: fn_23542
bool IsRidingMadogear(std::uint32_t flags) {
  return (flags >> 22) & 1;
}

// OFFSET: 0x00DF98F0
// FUNCTION: fn_23544
bool StateFlagsClear(std::uint32_t flags) {
  return flags == 0;
}

// OFFSET: 0x00DF9910
// FUNCTION: fn_23546
bool IsOrcish(std::uint32_t flags) {
  return (flags >> 11) & 1;
}

// OFFSET: 0x00DF9940
// FUNCTION: fn_23547
bool HasWarg(std::uint32_t flags) {
  return (flags >> 20) & 1;
}

// OFFSET: 0x00DF9950
// FUNCTION: fn_23548
bool HasWargChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 20) ^ (current >> 20)) & 1;
}

// OFFSET: 0x00DF9970
// FUNCTION: fn_23549
bool IsRuwachActive(std::uint32_t flags) {
  return (flags >> 13) & 1;
}

// OFFSET: 0x00DF9980
// FUNCTION: fn_23550
bool IsRuwachActiveChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 13) ^ (current >> 13)) & 1;
}

// OFFSET: 0x00DF99B0
// FUNCTION: fn_23552
bool IsSightActive(std::uint32_t flags) {
  return flags & 1;
}

// OFFSET: 0x00DF99C0
// FUNCTION: fn_23553
bool IsSightActiveChanged(std::uint32_t previous, std::uint32_t current) {
  return (previous ^ current) & 1;
}

// OFFSET: 0x00DF99D0
// FUNCTION: fn_23554
bool IsInvisible(std::uint32_t flags) {
  return (flags >> 6) & 1;
}

// OFFSET: 0x00DF99E0
// FUNCTION: fn_23555
bool IsInvisibleChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 6) ^ (current >> 6)) & 1;
}

// OFFSET: 0x00DF9A10
// FUNCTION: fn_23557
bool IsStarUnionChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 15) ^ (current >> 15)) & 1;
}

// OFFSET: 0x00DF9A80
// FUNCTION: fn_23560
bool IsRidingWargChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 21) ^ (current >> 21)) & 1;
}

// OFFSET: 0x00DF98C0
// FUNCTION: fn_46704
bool IsRidingMadogearChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 22) ^ (current >> 22)) & 1;
}

// OFFSET: 0x00DF9920
// FUNCTION: fn_46705
bool IsOrcishChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 11) ^ (current >> 11)) & 1;
}

// OFFSET: 0x00DF9A40
// FUNCTION: fn_46706
bool IsTransformed(std::uint32_t flags) {
  return (flags >> 17) & 1;
}

// OFFSET: 0x00DF9A50
// FUNCTION: fn_46707
bool IsTransformedChanged(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 17) ^ (current >> 17)) & 1;
}
