#ifndef RAGNAROK_SOURCE_UI_UIINGREDIENTINFOWND_H
#define RAGNAROK_SOURCE_UI_UIINGREDIENTINFOWND_H

#include "Contents/Items/ItemInfo.h"
#include "UI/Window/UIFrameWnd.h"

#include <string>

struct UIButtonContext;
class UILapineBox_IngredientListWnd;

class UIIngredientInfoWnd : public UIFrameWnd {
  friend class UILapineBox_IngredientListWnd;

public:
  // INLINED
  UIIngredientInfoWnd(UIWindowContext& windows, UIFrameContext& frames)
      : UIFrameWnd(windows, frames, 0), selected(0), insufficient(0), quantity(0), m_controls(nullptr) {}
  void Move(int x, int y) override;
  void Slot10(int argument1, int argument2, std::intptr_t argument3) override;
  void Paint() override;

  /* 00B8 */ ITEM_INFO item;
  /* 01B0 */ unsigned char selected;
  /* 01B1 */ unsigned char insufficient;
  /* 01B4 */ std::string tooltip;
  /* 01CC */ int quantity;

protected:
  UIButtonContext* m_controls;
};

#endif
