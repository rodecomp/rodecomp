#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_ACTORSTATEFLAGS_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_ACTORSTATEFLAGS_H

#include <cstdint>

bool StateFlag01(std::uint32_t flags);
bool StateFlag05(std::uint32_t flags);
bool StateFlag12(std::uint32_t flags);
bool StateFlag15(std::uint32_t flags);
bool StateFlag16(std::uint32_t flags);
bool StateFlag18(std::uint32_t flags);
bool StateFlag19(std::uint32_t flags);
bool StateFlag21(std::uint32_t flags);
bool StateFlag23(std::uint32_t flags);
bool StateFlag24(std::uint32_t flags);
bool StateFlag25(std::uint32_t flags);
bool StateFlag26(std::uint32_t flags);
bool StateFlag27(std::uint32_t flags);
bool StateFlag28(std::uint32_t flags);
bool StateFlag00(std::uint32_t flags);
bool StateFlag00Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag01Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag02(std::uint32_t flags);
bool StateFlag02Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag04(std::uint32_t flags);
bool StateFlag04Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag05Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag06(std::uint32_t flags);
bool StateFlag06Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag11(std::uint32_t flags);
bool StateFlag13(std::uint32_t flags);
bool StateFlag13Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag14(std::uint32_t flags);
bool StateFlag15Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag19Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag20(std::uint32_t flags);
bool StateFlag20Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag21Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag22(std::uint32_t flags);
bool StateFlag23Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag24Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag25Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlag26Changed(std::uint32_t previous, std::uint32_t current);
bool StateFlagsClear(std::uint32_t flags);

#endif
