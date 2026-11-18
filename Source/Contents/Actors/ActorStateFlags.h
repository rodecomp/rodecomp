#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_ACTORSTATEFLAGS_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_ACTORSTATEFLAGS_H

#include <cstdint>

bool IsHiding(std::uint32_t flags);
bool IsRidingPeco(std::uint32_t flags);
bool IsWeddingDress(std::uint32_t flags);
bool IsStarUnion(std::uint32_t flags);
bool IsXmasDress(std::uint32_t flags);
bool IsSummerDress(std::uint32_t flags);
bool IsRidingGreenDragon(std::uint32_t flags);
bool IsRidingWarg(std::uint32_t flags);
bool IsRidingBrownDragon(std::uint32_t flags);
bool IsRidingGrayDragon(std::uint32_t flags);
bool IsRidingBlueDragon(std::uint32_t flags);
bool IsRidingRedDragon(std::uint32_t flags);
bool IsHanbokDress(std::uint32_t flags);
bool IsOktoberfestDress(std::uint32_t flags);
bool IsSightActive(std::uint32_t flags);
bool IsSightActiveChanged(std::uint32_t previous, std::uint32_t current);
bool IsHidingChanged(std::uint32_t previous, std::uint32_t current);
bool IsCloaking(std::uint32_t flags);
bool IsCloakingChanged(std::uint32_t previous, std::uint32_t current);
bool HasFalcon(std::uint32_t flags);
bool HasFalconChanged(std::uint32_t previous, std::uint32_t current);
bool IsRidingPecoChanged(std::uint32_t previous, std::uint32_t current);
bool IsInvisible(std::uint32_t flags);
bool IsInvisibleChanged(std::uint32_t previous, std::uint32_t current);
bool IsOrcish(std::uint32_t flags);
bool IsRuwachActive(std::uint32_t flags);
bool IsRuwachActiveChanged(std::uint32_t previous, std::uint32_t current);
bool IsChaseWalking(std::uint32_t flags);
bool IsStarUnionChanged(std::uint32_t previous, std::uint32_t current);
bool IsRidingGreenDragonChanged(std::uint32_t previous, std::uint32_t current);
bool HasWarg(std::uint32_t flags);
bool HasWargChanged(std::uint32_t previous, std::uint32_t current);
bool IsRidingWargChanged(std::uint32_t previous, std::uint32_t current);
bool IsRidingMadogear(std::uint32_t flags);
bool IsRidingBrownDragonChanged(std::uint32_t previous, std::uint32_t current);
bool IsRidingGrayDragonChanged(std::uint32_t previous, std::uint32_t current);
bool IsRidingBlueDragonChanged(std::uint32_t previous, std::uint32_t current);
bool IsRidingRedDragonChanged(std::uint32_t previous, std::uint32_t current);
bool StateFlagsClear(std::uint32_t flags);

#endif
