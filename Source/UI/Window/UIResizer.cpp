#include "Client/Mode/ModeCursor.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Window/UIResizer.h"
#include "UI/Window/UIWindowMgr.h"

// OFFSET: 0x0085D650
// FUNCTION: fn_10815
void UIResizer::Paint() {
  if (!this->bitmapName.empty()) {
    CBitmapRes* bitmap = static_cast<CBitmapRes*>(ResourceMgr::Instance().Load(Resolve(this->bitmapName.c_str())));
    this->DrawBitmap(0, 0, bitmap ? &bitmap->image : nullptr, false);
    return;
  }

  this->ClearSurface(this->backgroundColor);
  this->FillRegion(0, 0, this->geometry.width, 2, this->lineColor);
}

// OFFSET: 0x0085ECE0

// OFFSET: 0x0085F470
// FUNCTION: fn_10862
void UIResizer::Slot28(int, int) {
}

// OFFSET: 0x0085F6D0

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

// OFFSET: 0x008697F0
// FUNCTION: fn_10971
void UIResizer::SetDirection(int value) {
  this->direction = value;
}
