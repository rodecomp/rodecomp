#include "UI/UIItemReformWnd.h"

#include "Contents/Items/ItemNaming.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/WindowBorder.h"
#include "Utility/Text/FormatString.h"

#include <algorithm>
#include <cstdint>
#include <string>

// OFFSET: 0x005FCD10

// OFFSET: 0x005FD600

// OFFSET: 0x005FD770

// OFFSET: 0x005FD7E0

// OFFSET: 0x005FDA60

// OFFSET: 0x005FDB00
// FUNCTION: fn_04118
void UIItemReformWnd_DetailBtn::Slot47() {
  if (this->parent) {
    this->parent->SendMsg(this, 167, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005FDBE0

// OFFSET: 0x005FDC50
// FUNCTION: fn_04121
void UIItemReformWnd_DetailBtn::Slot48() {
  if (this->parent) {
    this->parent->SendMsg(this, 168, 0, 0, 0, 0);
  }
}

// OFFSET: 0x005FDDA0
