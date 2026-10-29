#include "Contents/Actors/ActorStateFlags.h"

// OFFSET: 0x00DF9710
// FUNCTION: fn_23524
bool StateFlag01(std::uint32_t flags) {
  return (flags >> 1) & 1;
}

// OFFSET: 0x00DF9740
// FUNCTION: fn_23526
bool StateFlag05(std::uint32_t flags) {
  return (flags >> 5) & 1;
}

// OFFSET: 0x00DF9770
// FUNCTION: fn_23528
bool StateFlag23(std::uint32_t flags) {
  return (flags >> 23) & 1;
}

// OFFSET: 0x00DF97A0
// FUNCTION: fn_23530
bool StateFlag24(std::uint32_t flags) {
  return (flags >> 24) & 1;
}

// OFFSET: 0x00DF97D0
// FUNCTION: fn_23532
bool StateFlag25(std::uint32_t flags) {
  return (flags >> 25) & 1;
}

// OFFSET: 0x00DF9800
// FUNCTION: fn_23534
bool StateFlag26(std::uint32_t flags) {
  return (flags >> 26) & 1;
}

// OFFSET: 0x00DF9830
// FUNCTION: fn_23536
bool StateFlag19(std::uint32_t flags) {
  return (flags >> 19) & 1;
}

// OFFSET: 0x00DF9870
// FUNCTION: fn_23539
bool StateFlag27(std::uint32_t flags) {
  return (flags >> 27) & 1;
}

// OFFSET: 0x00DF98E0
// FUNCTION: fn_23543
bool StateFlag12(std::uint32_t flags) {
  return (flags >> 12) & 1;
}

// OFFSET: 0x00DF9900
// FUNCTION: fn_23545
bool StateFlag28(std::uint32_t flags) {
  return (flags >> 28) & 1;
}

// OFFSET: 0x00DF99A0
// FUNCTION: fn_23551
bool StateFlag16(std::uint32_t flags) {
  return (flags >> 16) & 1;
}

// OFFSET: 0x00DF9A00
// FUNCTION: fn_23556
bool StateFlag15(std::uint32_t flags) {
  return (flags >> 15) & 1;
}

// OFFSET: 0x00DF9A30
// FUNCTION: fn_23558
bool StateFlag18(std::uint32_t flags) {
  return (flags >> 18) & 1;
}

// OFFSET: 0x00DF9A70
// FUNCTION: fn_23559
bool StateFlag21(std::uint32_t flags) {
  return (flags >> 21) & 1;
}

// OFFSET: 0x00DF9720
// FUNCTION: fn_23525
bool StateFlag01Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 1) ^ (current >> 1)) & 1;
}

// OFFSET: 0x00DF9750
// FUNCTION: fn_23527
bool StateFlag05Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 5) ^ (current >> 5)) & 1;
}

// OFFSET: 0x00DF9780
// FUNCTION: fn_23529
bool StateFlag23Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 23) ^ (current >> 23)) & 1;
}

// OFFSET: 0x00DF97B0
// FUNCTION: fn_23531
bool StateFlag24Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 24) ^ (current >> 24)) & 1;
}

// OFFSET: 0x00DF97E0
// FUNCTION: fn_23533
bool StateFlag25Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 25) ^ (current >> 25)) & 1;
}

// OFFSET: 0x00DF9810
// FUNCTION: fn_23535
bool StateFlag26Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 26) ^ (current >> 26)) & 1;
}

// OFFSET: 0x00DF9840
// FUNCTION: fn_23537
bool StateFlag19Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 19) ^ (current >> 19)) & 1;
}

// OFFSET: 0x00DF9860
// FUNCTION: fn_23538
bool StateFlag14(std::uint32_t flags) {
  return (flags >> 14) & 1;
}

// OFFSET: 0x00DF9880
// FUNCTION: fn_23540
bool StateFlag02(std::uint32_t flags) {
  return (flags >> 2) & 1;
}

// OFFSET: 0x00DF9890
// FUNCTION: fn_23541
bool StateFlag02Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 2) ^ (current >> 2)) & 1;
}

// OFFSET: 0x00DF98B0
// FUNCTION: fn_23542
bool StateFlag22(std::uint32_t flags) {
  return (flags >> 22) & 1;
}

// OFFSET: 0x00DF98F0
// FUNCTION: fn_23544
bool StateFlagsClear(std::uint32_t flags) {
  return flags == 0;
}

// OFFSET: 0x00DF9910
// FUNCTION: fn_23546
bool StateFlag11(std::uint32_t flags) {
  return (flags >> 11) & 1;
}

// OFFSET: 0x00DF9940
// FUNCTION: fn_23547
bool StateFlag20(std::uint32_t flags) {
  return (flags >> 20) & 1;
}

// OFFSET: 0x00DF9950
// FUNCTION: fn_23548
bool StateFlag20Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 20) ^ (current >> 20)) & 1;
}

// OFFSET: 0x00DF9970
// FUNCTION: fn_23549
bool StateFlag13(std::uint32_t flags) {
  return (flags >> 13) & 1;
}

// OFFSET: 0x00DF9980
// FUNCTION: fn_23550
bool StateFlag13Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 13) ^ (current >> 13)) & 1;
}

// OFFSET: 0x00DF99B0
// FUNCTION: fn_23552
bool StateFlag00(std::uint32_t flags) {
  return flags & 1;
}

// OFFSET: 0x00DF99C0
// FUNCTION: fn_23553
bool StateFlag00Changed(std::uint32_t previous, std::uint32_t current) {
  return (previous ^ current) & 1;
}

// OFFSET: 0x00DF99D0
// FUNCTION: fn_23554
bool StateFlag06(std::uint32_t flags) {
  return (flags >> 6) & 1;
}

// OFFSET: 0x00DF99E0
// FUNCTION: fn_23555
bool StateFlag06Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 6) ^ (current >> 6)) & 1;
}

// OFFSET: 0x00DF9A10
// FUNCTION: fn_23557
bool StateFlag15Changed(std::uint32_t previous, std::uint32_t current) {
  return ((previous >> 15) ^ (current >> 15)) & 1;
}
