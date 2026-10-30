#ifndef RAGNAROK_SOURCE_UI_UIREFININGWND_H
#define RAGNAROK_SOURCE_UI_UIREFININGWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Text/UIStaticText.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstdint>
#include <string>
#include <vector>

#pragma pack(push, 1)
struct RefiningMaterial {
  /* 0000 */ std::uint32_t itemId;
  /* 0004 */ unsigned char unknown_0004;
  /* 0005 */ std::uint32_t unknown_0005;
};
#pragma pack(pop)

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RefiningMaterial) == 9);
#endif

class UIRefining_BtnAgain : public UIBitmapButton {
public:
  // INLINED
  UIRefining_BtnAgain(UIWindowContext& windows, UIButtonContext& controls) : UIBitmapButton(windows, controls), cost(0) {}
  void Paint() override;
  void Enable(unsigned char value) override;
  void UpdateBitmaps();

  /* 0120 */ int cost;
  /* 0124 */ unsigned char success;
  /* 0125 */ unsigned char unknown_0125;
};

class UIRefining_BtnRefining : public UIBitmapButton {
public:
  // INLINED
  UIRefining_BtnRefining(UIWindowContext& windows, UIButtonContext& controls) : UIBitmapButton(windows, controls), cost(0) {}
  void Paint() override;
  void Enable(unsigned char value) override;

  /* 0120 */ int cost;
};

class UIRefining_BtnReturn : public UIBitmapButton {
public:
  // INLINED
  UIRefining_BtnReturn(UIWindowContext& windows, UIButtonContext& controls) : UIBitmapButton(windows, controls), highlighted(1) {}
  void Paint() override;
  void UpdateBitmaps();

  /* 0120 */ unsigned char highlighted;
};

class UIRefining_Notice : public UIStaticText {
public:
  // INLINED
  UIRefining_Notice(UIWindowContext& windows, UIButtonContext& controls)
      : UIStaticText(windows, controls), savedHighlight(0), expireTick(0), autoShown(0), highlight(0) {}
  void Slot19() override;
  void Paint() override;
  void SetNotice(const char* value, int duration, unsigned char highlight);

  /* 00D8 */ std::string savedText;
  /* 00F0 */ unsigned char savedHighlight;
  /* 00F4 */ std::uint32_t expireTick;
  /* 00F8 */ unsigned char autoShown;
  /* 00F9 */ unsigned char highlight;
};

class UIRefiningWnd : public UIFrameWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Slot25(int x, int y) override;
  void Slot26(int x, int y) override;
  void Slot28(int x, int y) override;
  void Slot31(int x, int y) override;
  void Slot32(int x, int y) override;
  void Slot33(int x, int y) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t argument1, std::intptr_t argument2, std::intptr_t argument3, std::intptr_t argument4) override;
  int SlotAt(int x, int y);
  void Reset();
  std::uint32_t SelectedMaterialValue();
  void ApplyResult();
  void SetState(int state, int result);

  /* 00B4 */ UIRefining_BtnRefining* refiningButton;
  /* 00B8 */ UIRefining_BtnReturn* returnButton;
  /* 00BC */ UIRefining_BtnAgain* againButton;
  /* 00C0 */ UIButton* unknown_00C0;
  /* 00C4 */ UIRefining_Notice* notice;
  /* 00C8 */ int state;
  /* 00CC */ int result;
  /* 00D0 */ int frameInterval;
  /* 00D4 */ std::uint32_t stateTick;
  /* 00D8 */ int frame;
  /* 00DC */ int selectedMaterial;
  /* 00E0 */ std::vector<RefiningMaterial> materials;
  /* 00EC */ unsigned char catalystEnabled;
  /* 00F0 */ int catalystItemId;
  /* 00F4 */ int catalystCount;
  /* 00F8 */ ITEM_INFO item;
  /* 01F0 */ unsigned char unknown_01F0;
  /* 01F4 */ std::vector<RefiningMaterial> nextMaterials;
  /* 0200 */ int nextCatalystCount;
  /* 0208 */ ITEM_INFO nextItem;

protected:
  UIButtonContext* m_controls;
};

#endif
