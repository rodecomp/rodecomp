#ifndef RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTMIRRORITEMWND_H
#define RAGNAROK_SOURCE_UI_SHOP_UIMERCHANTMIRRORITEMWND_H

#include "Contents/UserData/NormalItemSlots.h"
#include "UI/Item/UIItemBaseWnd.h"

#include <cstddef>
#include <cstdint>

class UIMerchantMirrorItemWnd : public UIItemBaseWnd {
public:
  std::vector<unsigned char> GetReplayData() override;
  UIMerchantMirrorItemWnd(int value);
  void OnMouseHover(int x, int y) override;
  void OnRBtnDown(int x, int y) override;
  void StoreInfo() override;
  void DragAndDrop(int x, int y, std::intptr_t value) override;
  void OnLBtnDown(int x, int y) override;
  void OnCreate(int width, int height) override;
  int SendMsg(UIWindow* sender, int message, std::intptr_t a, std::intptr_t b, std::intptr_t c, std::intptr_t d) override;

  /* 00F0 */ MerchantMirrorItemWndInfo* saved;
  /* 00F4 */ int mode;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(UIMerchantMirrorItemWnd, saved) == 0xF0);
#endif

#endif
