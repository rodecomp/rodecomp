#include "Contents/Effects/SpriteParticle.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/Sprites/SpriteResource.h"

// OFFSET: 0x00B40E30
// FUNCTION: fn_18864
void CSpriteParticle::UpdateFrame() {
  if (!this->action || this->loopMode == 3) {
    return;
  }

  const long long frameCount = static_cast<int>(this->action->FrameCount(this->animation));
  const long long delay = static_cast<long long>(this->frameDelay);
  if (delay == 0 || (this->unknown_00E8 >= delay && this->unknown_00E8 % delay == 0)) {
    ++this->frame;
  }

  switch (this->loopMode) {
  case 0:
    if (this->frame >= frameCount) {
      this->frame = 0;
    }
    break;
  case 1:
    if (this->frame >= frameCount) {
      this->life = -1.0f;
    }
    break;
  case 2:
    if (this->frame >= frameCount) {
      this->frame = static_cast<int>(frameCount) - 1;
    }
    break;
  }
}

// OFFSET: 0x00B41D60
// FUNCTION: fn_18866
void CSpriteParticle::SetAction(const char* name) {
  if (this->action) {
    this->action->Unlock();
  }

  this->action = static_cast<CActRes*>(ResourceMgr::Instance().Load(name));
  if (this->action) {
    this->action->Lock();
  }
}

// OFFSET: 0x00B41DA0
// FUNCTION: fn_18867
void CSpriteParticle::SetSprite(const char* name) {
  if (this->sprite) {
    this->sprite->Unlock();
  }

  this->sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(name));
  if (this->sprite) {
    this->sprite->Lock();
  }
}
