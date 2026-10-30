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
  /* 0008 */ std::int32_t zeny;
  /* 000C */ unsigned char unknown_000C[5];
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
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void ResetItem();
  int SelectedMaterialZeny();
  int SuccessRate();
  void SetEnchantInfo(
    short index, int value, int materialCount, GradeEnchantInfo info, int value0108, int value010C, const GradeEnchantMaterial* materials
  );
  void SetResult(int state, int result);

  /* 00B8 */ UIGradeEnchant_BtnEnhancedUp* enhancedUpButton;
  /* 00C0 */ int state;
  /* 00C4 */ int result;
  /* 00C8 */ std::uint32_t unknown_00C8;
  /* 00CC */ std::uint32_t unknown_00CC;
  /* 00D0 */ std::uint32_t stateTick;
  /* 00D4 */ std::uint32_t unknown_00D4;
  /* 00D8 */ int unknown_00D8;
  /* 00DC */ int unknown_00DC;
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
  /* 011C */ std::vector<int> unknown_011C;
  /* 0128 */ ITEM_INFO item;
  /* 0220 */ unsigned char unknown_0220;
  /* 00B4 */ UIGradeEnchant_BtnStart* startButton;

protected:
  UIButtonContext* m_controls;
};

#endif
