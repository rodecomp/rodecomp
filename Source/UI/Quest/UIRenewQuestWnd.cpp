#include "UI/Quest/UIRenewQuestWnd.h"

#include "Contents/MessageTable.h"
#include "Contents/RenewQuestMgr.h"
#include "Render/DeviceContext/DeviceContext.h"
#include "Render/Textures/Texture.h"
#include "Render/Textures/TextureCache.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/UIStaticBitmap.h"
#include "Utility/Text/FormatString.h"

#include <cstddef>
#include <cstring>

// OFFSET: 0x007FB0C0
// FUNCTION: fn_09951
void Nullsub255() {}

// OFFSET: 0x007FB0D0
// FUNCTION: fn_09952
void UIRecommededQuestScrollWnd::Slot15(int, int) {
  this->scrollBarWidth = 9;
}

// OFFSET: 0x007FB0E0
// FUNCTION: fn_09953
void UIRecommededQuestScrollWnd::Paint() {
  this->ClearSurface(0xFFFF00FF);
  if (this->parent) {
    this->parent->Invalidate();
  }
}

// OFFSET: 0x007FB100
// FUNCTION: fn_09954
void UIRecommededQuestScrollWnd::Slot35(int delta) {
  if (this->wheelScroll == 1) {
    if (delta < 0) {
      this->SendMsg(this->scrollBar, 7, 15, 0, 0, 0);
      return;
    }
    this->SendMsg(this->scrollBar, 7, -15, 0, 0, 0);
  }
}

// OFFSET: 0x007FB860

// OFFSET: 0x007FBAD0
// FUNCTION: fn_09961
void UIRenewDetailQuestInfoWnd::Slot25(int x, int y) {
  if (y < 17) {
    UIFrameWnd::Slot25(x, y);
  }
}

// OFFSET: 0x007FBFD0

// OFFSET: 0x007FC870

// OFFSET: 0x007FC9B0

// OFFSET: 0x007FCD80

// OFFSET: 0x007FCDB0
