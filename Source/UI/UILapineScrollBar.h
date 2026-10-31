#ifndef RAGNAROK_SOURCE_UI_UILAPINESCROLLBAR_H
#define RAGNAROK_SOURCE_UI_UILAPINESCROLLBAR_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Lists/UIScrollBar.h"
#include "UI/UIIngredientInfoWnd.h"
#include "UI/UIScrollWnd.h"
#include "UI/Window/UIFrameWnd.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

class UIBitmapButton;
struct UIButtonContext;
class UILapineUpgradeBox_IngredientInfoWnd;
class UILapineBoxWnd;

class UILapineScrollBar : public UIScrollBar {
public:
  // INLINED
  explicit UILapineScrollBar(UIWindowContext& windows) : UIScrollBar(windows) {
    this->arrowLength = 0;
  }
  void Paint() override;
};

class UILapineUpgradeBox_IngredientListWnd : public UIScrollWnd {
public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Paint() override;
  void Slot35(int argument) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  UILapineUpgradeBox_IngredientInfoWnd* FindSelected();
  bool AddIngredient(std::uint32_t identity);
  bool CanUseIngredient(std::uint32_t identity);

  /* 00B0 */ std::vector<ITEM_INFO> materials;
  /* 00BC */ std::vector<UILapineUpgradeBox_IngredientInfoWnd*> items;
  /* 00C8 */ int minOptionCount;
  /* 00CC */ int minRefine;
  /* 00D0 */ int maxRefine;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIScrollWnd, scrollBar) == 0x90);
static_assert(offsetof(UIScrollWnd, wheelScroll) == 0xA5);
static_assert(offsetof(UILapineUpgradeBox_IngredientListWnd, materials) == 0xB0);
static_assert(offsetof(UILapineUpgradeBox_IngredientListWnd, items) == 0xBC);
static_assert(offsetof(UILapineUpgradeBox_IngredientListWnd, maxRefine) == 0xD0);
#endif

class UILapineBox_IngredientListWnd : public UIScrollWnd {
  friend class UILapineBoxWnd;

public:
  UILapineBox_IngredientListWnd(UIWindowContext& windows, int width, int height);
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot35(int argument) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  unsigned char AddIngredient(std::uint32_t id);

  /* 00B0 */ std::vector<ITEM_INFO> materials;
  /* 00BC */ std::vector<UIIngredientInfoWnd*> items;
  /* 00C8 */ int minRefine;
  /* 00CC */ int maxRefine;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UILapineBox_IngredientListWnd, materials) == 0xB0);
static_assert(offsetof(UILapineBox_IngredientListWnd, items) == 0xBC);
static_assert(offsetof(UILapineBox_IngredientListWnd, minRefine) == 0xC8);
static_assert(offsetof(UILapineBox_IngredientListWnd, maxRefine) == 0xCC);
#endif

class UILapineBoxWnd : public UIFrameWnd {
  friend class UILapineBox_IngredientListWnd;

public:
  void Slot10(int x, int y, std::intptr_t value) override;
  void Slot15(int width, int height) override;
  void Slot19() override;
  void Slot28(int x, int y) override;
  void Paint() override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;
  int AddIngredient(std::uint32_t id, int quantity);
  void DropMaterial(int x, int y);
  void DragIngredient(int x, int y);
  void NotifyWindow(int unused, int alsoUnused);
  void NotifyWindowAt(int x, int y);
  void SetIngredient(int id);
  void AddMaterial(int id, int quantity);
  __declspec(noinline) void ReadMaterial(ITEM_INFO* out, int x, int y);
  __declspec(noinline) void MaterialPosition(int* out, int index);
  void SetWaiting(unsigned char waiting);
  // INLINED
  UIFrameContext& FrameContext() {
    return *this->m_frameContext;
  }

  /* 00B4 */ UIBitmapButton* cancelButton;
  /* 00B8 */ UIBitmapButton* makeButton;
  /* 00BC */ UIBitmapButton* moveButton;
  /* 00C0 */ UILapineBox_IngredientListWnd* ingredientList;
  /* 00C4 */ int unknown_00C4;
  /* 00C8 */ std::string title;
  /* 00E0 */ std::vector<ITEM_INFO> items;
  /* 00EC */ int maxItems;
  /* 00F0 */ std::string description;
  /* 0108 */ std::string notice;
  /* 0120 */ std::uint32_t noticeTick;
  /* 0124 */ std::uint32_t noticeDuration;
  /* 0128 */ unsigned char waiting;

protected:
  UIButtonContext* m_controls;
};

#endif
