#include "UI/Window/UISerachStoreInfoWnd.h"

// OFFSET: 0x009121F0
// FUNCTION: fn_12527
UISerachStoreInfoWnd::UISerachStoreInfoWnd()
    : UIFrameWnd(*reinterpret_cast<UIWindowContext*>(nullptr), *reinterpret_cast<UIFrameContext*>(nullptr), 0), unknown_00DC(1), unknown_00E0(-1),
      unknown_00E4(0), unknown_00E8(25), unknown_00EC(0), unknown_00ED(1) {
  this->unknown_00F0.clear();
}
