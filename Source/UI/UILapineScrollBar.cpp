#include "UI/UIIngredientInfoWnd.h"
#include "UI/UILapineScrollBar.h"

#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"

// OFFSET: 0x005FF810
// FUNCTION: fn_04149
void UIIngredientInfoWnd::Slot10(int argument1, int argument2, std::intptr_t argument3) {
  this->parent->Slot10(argument1, argument2, argument3);
}

// OFFSET: 0x005FF910
// FUNCTION: fn_04152
void UIIngredientInfoWnd::Move(int x, int y) {
  UIWindow::Move(x, y);
}

// OFFSET: 0x00600270
// FUNCTION: fn_04158
void UILapineScrollBar::Paint() {
  const auto load = [](const char* name) {
    return static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(name)));
  };
  this->ClearSurface(0xFFFF00FF);
  if (this->maximum == 0) {
    return;
  }

  if (this->vertical == 1) {
    CBitmapRes* top = load("유저인터페이스\\lapine\\scrollbar_bg_top.bmp");
    CBitmapRes* bottom = load("유저인터페이스\\lapine\\scrollbar_bg_btm.bmp");
    CBitmapRes* middle = load("유저인터페이스\\lapine\\scrollbar_bg_mid.bmp");
    this->DrawBitmap(0, 0, &top->image, true);

    const int interior = this->geometry.height - bottom->image.height - top->image.height;
    const int count = interior / middle->image.height + (interior % middle->image.height != 0);
    for (int index = 0; index < count; ++index) {
      this->DrawBitmap(0, middle->image.height * index + top->image.height, &middle->image, true);
    }
    this->DrawBitmap(0, this->geometry.height - bottom->image.height, &bottom->image, true);

    const int travel = this->geometry.height - bottom->image.height - top->image.height;
    const int thumbLength = this->visible * travel / this->total;
    const int thumbOffset = this->position * travel / this->total;

    const char* thumbTop;
    const char* thumbBottom;
    const char* thumbMiddle;
    if (this->state == 0) {
      thumbTop = "유저인터페이스\\lapine\\scrollbar_thumb_out_top.bmp";
      thumbBottom = "유저인터페이스\\lapine\\scrollbar_thumb_out_btm.bmp";
      thumbMiddle = "유저인터페이스\\lapine\\scrollbar_thumb_out_mid.bmp";
    } else if (this->state == 3) {
      thumbTop = "유저인터페이스\\lapine\\scrollbar_thumb_over_top.bmp";
      thumbBottom = "유저인터페이스\\lapine\\scrollbar_thumb_over_btm.bmp";
      thumbMiddle = "유저인터페이스\\lapine\\scrollbar_thumb_over_mid.bmp";
    } else if (this->state == 6) {
      thumbTop = "유저인터페이스\\lapine\\scrollbar_thumb_press_top.bmp";
      thumbBottom = "유저인터페이스\\lapine\\scrollbar_thumb_press_btm.bmp";
      thumbMiddle = "유저인터페이스\\lapine\\scrollbar_thumb_press_mid.bmp";
    } else {
      this->minimumThumb = 0;
      this->parent->Invalidate();
      return;
    }

    CBitmapRes* capTop = load(thumbTop);
    CBitmapRes* capBottom = load(thumbBottom);
    CBitmapRes* fill = load(thumbMiddle);
    const int span = thumbLength - capBottom->image.height - capTop->image.height;
    const int pieces = span / fill->image.height + (span % fill->image.height != 0);
    for (int index = 0; index < pieces; ++index) {
      this->DrawBitmap(0, fill->image.height * index + thumbOffset + capTop->image.height + top->image.height, &fill->image, true);
    }
    this->DrawBitmap(0, top->image.height + thumbOffset, &capTop->image, true);
    this->DrawBitmap(0, thumbOffset - capBottom->image.height + thumbLength + top->image.height, &capBottom->image, true);
  }

  this->minimumThumb = 0;
  this->parent->Invalidate();
}

// OFFSET: 0x00600260
// FUNCTION: fn_04157
void UILapineBox_IngredientListWnd::Paint() {
  this->ClearSurface(0xFFF7F7);
}
