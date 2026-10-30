#include "Resource/ResourcePreload.h"
#include "Resource/Resource.h"
#include "Resource/ResourceMgr.h"

// GLOBAL: gl_25068
int g_spriteHalfWidth;

// GLOBAL: gl_25069
int g_spriteHalfHeight;

// OFFSET: 0x00DFCDA0
// FUNCTION: fn_23602
void PreloadCommonSprites() {
  CRes* shadowAction = ResourceMgr::Instance().Load("shadow.act");
  CRes* shadowSprite = ResourceMgr::Instance().Load("shadow.spr");
  if (shadowAction) {
    shadowAction->Lock();
  }
  if (shadowSprite) {
    shadowSprite->Lock();
  }

  const int halfWidth = g_spriteHalfWidth;
  const int halfHeight = g_spriteHalfHeight;
  g_spriteHalfWidth = 0;
  g_spriteHalfHeight = 0;

  CRes* cursorAction = ResourceMgr::Instance().Load("cursors.act");
  CRes* cursorSprite = ResourceMgr::Instance().Load("cursors.spr");
  if (cursorAction) {
    cursorAction->Lock();
  }
  if (cursorSprite) {
    cursorSprite->Lock();
  }

  CRes* numberAction = ResourceMgr::Instance().Load("이팩트\\숫자.act");
  CRes* numberSprite = ResourceMgr::Instance().Load("이팩트\\숫자.spr");
  if (numberAction) {
    numberAction->Lock();
  }
  if (numberSprite) {
    numberSprite->Lock();
  }

  g_spriteHalfWidth = halfWidth;
  g_spriteHalfHeight = halfHeight;
}
