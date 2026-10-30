#include "Client/Mode/ModeCursor.h"
#include "Resource/Bitmap/BitmapResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/TextureNames.h"
#include "UI/Chat/UIChatRoomTitle.h"

// OFFSET: 0x0085C690

// OFFSET: 0x0085F980
// FUNCTION: fn_10876
void UIChatRoomTitle::ResetCursor(int x, int y) {
  if (x >= 0 && x < this->geometry.width && y >= 0 && y <= this->geometry.height) {
    this->m_context->cursor.Set(2, this->m_context->cursorTick);
  } else {
    this->m_context->cursor.Set(0, this->m_context->cursorTick);
  }
}
