#include "UI/Items/UIIllustWnd.h"

#include "Client/Mode/CMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Buttons/UIBitmapButton.h"
#include "UI/Buttons/UIButton.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x0096CBD0
// FUNCTION: fn_13181
void UIIllustWnd::Slot15(int, int) {}

// OFFSET: 0x0096CBE0
// FUNCTION: fn_13182
void UIMultiIllustWnd::Slot15(int, int) {}

// OFFSET: 0x0096CBF0
// FUNCTION: fn_13183
void UIIllustWnd::Slot18() {
  if (this->m_bitmapName.size() != 0) {
    CRes* bitmap = ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str()));
    ResourceMgr::Instance().Remove(bitmap);
  }
}

// OFFSET: 0x0096CC40
// FUNCTION: fn_13184
void UIMultiIllustWnd::Slot18() {
  if (this->m_bitmapName.size() != 0) {
    CRes* bitmap = ResourceMgr::Instance().Load(Resolve(this->m_bitmapName.c_str()));
    ResourceMgr::Instance().Remove(bitmap);
  }
}

// OFFSET: 0x0096CC90

// OFFSET: 0x0096CD00

// OFFSET: 0x0096CD70
// FUNCTION: fn_13187
void UIMultiIllustWnd::Slot25(int, int) {}

// OFFSET: 0x0096CD80

// OFFSET: 0x0096CDA0

// OFFSET: 0x0096CDC0

// OFFSET: 0x0096D010

// OFFSET: 0x0096D2C0

// OFFSET: 0x0096D2E0
