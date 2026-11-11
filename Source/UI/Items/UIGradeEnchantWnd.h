#ifndef RAGNAROK_SOURCE_UI_ITEMS_UIGRADEENCHANTWND_H
#define RAGNAROK_SOURCE_UI_ITEMS_UIGRADEENCHANTWND_H

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
  /* 000C */ std::int32_t downgradeAmount;
  /* 0010 */ unsigned char breakOnFailure;
};
#pragma pack(pop)

struct GradeEnchantInfo {
  /* 0000 */ std::int32_t blessingItemId;
  /* 0004 */ std::int32_t blessingStepAmount;
  /* 0008 */ std::int32_t maxBlessingSteps;
  /* 000C */ std::int32_t bonusPerStep;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(GradeEnchantMaterial) == 17);
static_assert(sizeof(GradeEnchantInfo) == 16);
#endif

class UIGradeEnchant_BtnStart : public UIBitmapButton {
public:
  UIGradeEnchant_BtnStart();
  void Paint() override;
  void Enable(unsigned char value) override;
};

class UIGradeEnchant_BtnEnhancedUp : public UIBitmapButton {
public:
  UIGradeEnchant_BtnEnhancedUp();
  void Paint() override;
  void OnMouseHover(int x, int y) override;
  void Enable(unsigned char value) override;

  /* 0120 */ int tooltipValue;
  /* 0124 */ unsigned char tooltipEnabled;
};

class UIGradeEnchant_BtnEnhancedDown : public UIBitmapButton {
public:
  UIGradeEnchant_BtnEnhancedDown();
  void Paint() override;
  void Enable(unsigned char value) override;
};

class UIGradeEnchantWnd : public UIFrameWnd {
public:
  UIGradeEnchantWnd();
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnCreate(int width, int height) override;
  void OnProcess() override;
  void Paint() override;
  void OnLBtnDown(int x, int y) override;
  void OnLBtnDblClk(int x, int y) override;
  void OnMouseHover(int x, int y) override;
  void OnLBtnUp(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void OnRBtnUp(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  void ResetItem();
  int SelectedMaterialZeny();
  int SuccessRate();
  void UpdateEnchantMessages();
  void SetEnchantInfo(
    short index, int value, int materialCount, GradeEnchantInfo info, int enchantOption, int enchantOptionCount, const GradeEnchantMaterial* materials
  );
  void SetResult(int state, int result);

  /* 00B4 */ UIGradeEnchant_BtnStart* startButton;
  /* 00B8 */ UIGradeEnchant_BtnEnhancedUp* enhancedUpButton;
  /* 00BC */ UIGradeEnchant_BtnEnhancedDown* enhancedDownButton;
  /* 00C0 */ int state;
  /* 00C4 */ int result;
  /* 00C8 */ std::uint32_t itemFrameInterval;
  /* 00CC */ std::uint32_t wheelFrameInterval;
  /* 00D0 */ std::uint32_t stateTick;
  /* 00D4 */ std::uint32_t wheelTick;
  /* 00D8 */ int itemFrame;
  /* 00DC */ int wheelFrame;
  /* 00E0 */ std::uint32_t resultTick;
  /* 00E4 */ int selectedMaterial;
  /* 00E8 */ int hoverMaterial;
  /* 00EC */ std::vector<GradeEnchantMaterial> materials;
  /* 00F8 */ GradeEnchantInfo info;
  /* 0108 */ int enchantOption;
  /* 010C */ int enchantOptionCount;
  /* 0110 */ unsigned char protect;
  /* 0114 */ int blessingAmount;
  /* 0118 */ int baseSuccessRate;
  /* 011C */ std::vector<int> messageIds;
  /* 0128 */ ITEM_INFO item;
  /* 0220 */ unsigned char requestPending;
};

#endif
