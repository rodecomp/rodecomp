#include "Client/Session.h"
#include "Contents/Actors/RenderObjectState.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

// INLINED
RenderObjectState::RenderObjectState() : sprite(nullptr), action(nullptr) {
  this->Reset();
}

// INLINED
RenderObjectState::~RenderObjectState() {
  if (this->sprite) {
    this->sprite->Unlock();
  }

  if (this->action) {
    this->action->Unlock();
  }
}

// OFFSET: 0x00CAF990
// FUNCTION: fn_21086
void RenderObjectState::Reset() {
  this->position.x = 0;
  this->position.y = 0;
  this->position.z = 0;
  this->animation = 0;
  this->previousAnimation = 0;
  this->lastSoundAnimation = 0;
  this->curMotion = 0;
  this->lastSoundMotion = 0;
  this->angle = 0;
  this->scale = 1;
  this->shadowScale = 1;
  this->motionType = 0;
  this->speed = 1;
  this->state = 0;
  this->visible = 1;
  this->enabled = 1;
  this->color = 0xFFFFFFFFu;
  this->flatDepth = 0;
  this->visibilityState = 0;
  this->isMotionFinished = 0;
  this->isMotionFreezed = 0;
  this->spriteOffsetX = 0;
  this->spriteAngle = 0;
  this->started = timeGetTime();
  this->isSingleColor = 0;
  this->isLieOnGround = 0;
  this->colorLocked = 0;
  this->overrideColor = 0xFFFF0000u;
  this->depthLayer = 0;
  this->bodyPalette = 0;
  this->distanceScale = 1;
  this->motionFinishThreshold = 1;
  this->motionSpeedScale = 1;
  this->isCharacter = 0;

  char spriteName[128];
  char actionName[128];
  std::sprintf(spriteName, "NPC\\%s.spr", g_session.GetJobName(46, -1));
  std::sprintf(actionName, "NPC\\%s.act", g_session.GetJobName(46, -1));

  if (this->sprite) {
    this->sprite->Unlock();
  }

  this->sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName));

  if (this->sprite) {
    this->sprite->Lock();
  }

  if (this->action) {
    this->action->Unlock();
  }

  this->action = static_cast<CActRes*>(ResourceMgr::Instance().Load(actionName));

  if (this->action) {
    this->action->Lock();
  }

  this->savedTintColor = 0;
  this->effectPhase = 0;
  this->tintPhase = 0;
  this->hoverAngle = 0;
  this->hoverHeight = 0;
  this->fontSequenceState = 0;
  this->unknown_0100 = 0;
  this->accountId = 0;
  this->specialMotionLocked = 0;
  this->frameSequenceActive = 0;
  this->animationSequenceActive = 0;
  this->timedSequenceActive = 0;
  this->sequenceInterval = 10;
  this->effectPhaseState = 1;
  this->flags = 0;
  this->pulsePhaseState = 0;
  this->outlineAlpha = 0;
  this->useGroundLighting = 1;
  this->instant.Clear();
  this->blendFlags = 0;
  this->hoverOffsetLow = 0;
  this->hoverOffsetHigh = 0;
}
