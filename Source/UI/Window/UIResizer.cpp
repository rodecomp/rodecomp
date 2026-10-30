#include "Client/Mode/ModeCursor.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x0085D650

// OFFSET: 0x0085F470
// FUNCTION: fn_10862
void UIResizer::Slot28(int, int) {
}

// OFFSET: 0x0085FA20
// FUNCTION: fn_10879
void UIResizer::ResetCursor(int x, int y) {
  if (g_windowMgr.Capture() == this || (x >= 0 && x < this->geometry.width && y >= 0 && y <= this->geometry.height)) {
    this->m_context->cursor.Set(2, this->m_context->cursorTick);
  } else if (g_windowMgr.Capture() != this) {
    this->m_context->cursor.Set(0, this->m_context->cursorTick);
  }
}

// OFFSET: 0x0085EFD0
// FUNCTION: fn_10853
void UIResizer::Slot31(int, int) {
  if (g_windowMgr.Capture() != this) {
    return;
  }

  if (this->parent) {
    this->parent->Slot36();
  }

  g_windowMgr.ReleaseCapture();
}
