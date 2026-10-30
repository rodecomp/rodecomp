#include "Contents/Items/RuneSystem.h"
#include "Contents/MessageTable.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/RuneSystem/UIRuneSystemWnd.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x0081BD10

// OFFSET: 0x0081BE90
// FUNCTION: fn_10195
void UIEquipRuneSetWnd::Slot26(int, int) {
  this->parent->SendMsg(this, 0xAB, 0x158, 0, 0, 0);
}

// OFFSET: 0x0081BFD0
// FUNCTION: fn_10197
void UIEquipRuneSetWnd::Slot25(int, int) {
  this->parent->SendMsg(this, 0xAB, 0xC2, -1, 0, 0);
}

// OFFSET: 0x0081C000

// OFFSET: 0x0081C080

// OFFSET: 0x0081C0E0
// FUNCTION: fn_10200
void UIRuneSystemWnd::Slot25(int, int) {
}

// OFFSET: 0x0081C0F0

// OFFSET: 0x0081C190

// OFFSET: 0x0081C230
// FUNCTION: fn_10203
void UIDecomlist::Slot28(int, int) {
}

// OFFSET: 0x0081C3D0
// FUNCTION: fn_10205
void UIRuneList::Slot28(int, int) {
}

// OFFSET: 0x0081CBF0

// OFFSET: 0x0081CC00

// OFFSET: 0x0081CF10

// OFFSET: 0x0081D5F0

// OFFSET: 0x0081DA20
