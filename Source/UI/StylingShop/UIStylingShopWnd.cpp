#include "UI/StylingShop/UIStylingShopWnd.h"

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x00829B50
// FUNCTION: fn_10341
void UIStylingshopTabParentWnd::Slot15(int, int) {
}

// OFFSET: 0x00829B60

// OFFSET: 0x00829BE0

// OFFSET: 0x00829C20
// FUNCTION: fn_10344
void UIAccessoryShowCase::Paint() {
  this->ClearSurface(0xFFFFFFFF);
}

// OFFSET: 0x00829C30

// OFFSET: 0x00829F30

// OFFSET: 0x0082A020
// FUNCTION: fn_10350
void UISlotOfMyStyleWnd::Paint() {
}

// OFFSET: 0x0082A090
// FUNCTION: fn_10352
void UIStylingshopTabParentWnd::Paint() {
}

// OFFSET: 0x0082A0A0
// FUNCTION: fn_10353
void UISlotForSelectWnd::Slot26(int, int) {
  if (this->parent) {
    this->parent->SendMsg(nullptr, 121, 0, 0, 0, 0);
  }
}

// OFFSET: 0x0082A0C0

// OFFSET: 0x0082A0E0

// OFFSET: 0x0082A210

// OFFSET: 0x0082A330
// FUNCTION: fn_10360
void UIAccBitmapButton::Slot33(int, int) {
  this->parent->SendMsg(this, 101, this->geometry.id, this->m_argument, 0, 0);
}
