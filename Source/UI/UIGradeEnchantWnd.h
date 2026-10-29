#ifndef RAGNAROK_SOURCE_UI_UIGRADEENCHANTWND_H
#define RAGNAROK_SOURCE_UI_UIGRADEENCHANTWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <vector>

#pragma pack(push, 1)
struct GradeEnchantMaterial {
  /* 0000 */ std::uint32_t itemId;
  /* 0004 */ std::int32_t amount;
  /* 0008 */ unsigned char unknown_0008[9];
};
#pragma pack(pop)

struct GradeEnchantInfo {
  /* 0000 */ std::int32_t unknown_0000;
  /* 0004 */ std::int32_t unknown_0004;
  /* 0008 */ std::int32_t unknown_0008;
  /* 000C */ std::int32_t unknown_000C;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GradeEnchantMaterial) == 17);
static_assert(sizeof(GradeEnchantInfo) == 16);
#endif

class UIGradeEnchant_BtnStart : public UIBitmapButton {
public:
  void Paint() override;
  void Enable(unsigned char value) override;
};

class UIGradeEnchant_BtnEnhancedUp : public UIBitmapButton {
public:
  void Paint() override;
  void Slot28(int x, int y) override;
  void Enable(unsigned char value) override;

  /* 0120 */ int tooltipValue;
  /* 0124 */ unsigned char tooltipEnabled;
};

class UIGradeEnchant_BtnEnhancedDown : public UIBitmapButton {
public:
  void Paint() override;
  void Enable(unsigned char value) override;
};

class UIGradeEnchantWnd : public UIFrameWnd {
public:
  void Slot25(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void SetEnchantInfo(
    short index, int value, int materialCount, GradeEnchantInfo info, int value0108, int value010C, const GradeEnchantMaterial* materials
  );
  void SetResult(int state, int result);

  /* 00B8 */ UIGradeEnchant_BtnEnhancedUp* enhancedUpButton;
  /* 00C0 */ int state;
  /* 00C4 */ int result;
  /* 00D0 */ std::uint32_t stateTick;
  /* 00D8 */ int unknown_00D8;
  /* 00E0 */ std::uint32_t unknown_00E0;
  /* 00E4 */ int selectedMaterial;
  /* 00E8 */ int hoverMaterial;
  /* 00EC */ std::vector<GradeEnchantMaterial> materials;
  /* 00F8 */ GradeEnchantInfo info;
  /* 0108 */ int unknown_0108;
  /* 010C */ int unknown_010C;
  /* 0110 */ unsigned char unknown_0110;
  /* 0114 */ int unknown_0114;
  /* 0118 */ int unknown_0118;
  /* 0128 */ ITEM_INFO item;
  /* 0220 */ unsigned char unknown_0220;

protected:
  UIButtonContext* m_controls;
};

#endif
