#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Buttons/UIRadioBtn.h"

// OFFSET: 0x0085D470

// OFFSET: 0x0085ECB0
// FUNCTION: fn_10844
void UIRadioBtn::Slot25(int, int) {
  if (this->parent && this->enabled != 0) {
    this->parent->SendMsg(this, 6, this->geometry.id, this->value, 0, 0);
  }
}
