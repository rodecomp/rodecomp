#include "Client/Session/Session.h"
#include "Contents/Actors/RenderObject.h"
#include "Resource/Sprites/ActionResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Sprites/SpriteResource.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00CAF990
// FUNCTION: fn_21086
void CRenderObject::Reset() {
  this->render.position.x = 0;
  this->render.position.y = 0;
  this->render.position.z = 0;
  this->render.animation = 0;
  this->render.previousAnimation = 0;
  this->render.lastSoundAnimation = 0;
  this->render.curMotion = 0;
  this->render.lastSoundMotion = 0;
  this->render.angle = 0;
  this->render.scale = 1;
  this->render.shadowScale = 1;
  this->render.motionType = 0;
  this->render.speed = 1;
  this->render.state = 0;
  this->render.visible = 1;
  this->render.enabled = 1;
  this->render.color = 0xFFFFFFFFu;
  this->render.flatDepth = 0;
  this->render.shadowVisible = 0;
  this->render.trackBounds = 0;
  this->render.isMotionFinished = 0;
  this->render.isMotionFreezed = 0;
  this->render.spriteOffsetX = 0;
  this->render.spriteAngle = 0;
  this->render.started = timeGetTime();
  this->render.isSingleColor = 0;
  this->render.isLieOnGround = 0;
  this->render.colorLocked = 0;
  this->render.overrideColor = 0xFFFF0000u;
  this->render.depthLayer = 0;
  this->render.bodyPalette = 0;
  this->render.distanceScale = 1;
  this->render.motionFinishThreshold = 1;
  this->render.motionSpeedScale = 1;
  this->render.isCharacter = 0;

  char spriteName[128];
  char actionName[128];
  std::sprintf(spriteName, "NPC\\%s.spr", g_session.GetJobName(46, -1));
  std::sprintf(actionName, "NPC\\%s.act", g_session.GetJobName(46, -1));

  if (this->render.sprite) {
    this->render.sprite->Unlock();
  }

  this->render.sprite = static_cast<CSprRes*>(ResourceMgr::Instance().Load(spriteName));

  if (this->render.sprite) {
    this->render.sprite->Lock();
  }

  if (this->render.action) {
    this->render.action->Unlock();
  }

  this->render.action = static_cast<CActRes*>(ResourceMgr::Instance().Load(actionName));

  if (this->render.action) {
    this->render.action->Lock();
  }

  this->render.savedTintColor = 0;
  this->render.effectPhase = 0;
  this->render.tintPhase = 0;
  this->render.hoverAngle = 0;
  this->render.hoverHeight = 0;
  this->render.fontSequenceState = 0;
  this->render.sequenceReset = 0;
  this->render.accountId = 0;
  this->render.specialMotionLocked = 0;
  this->render.frameSequenceActive = 0;
  this->render.animationSequenceActive = 0;
  this->render.timedSequenceActive = 0;
  this->render.sequenceInterval = 10;
  this->render.effectPhaseState = 1;
  this->render.flags = 0;
  this->render.pulsePhaseState = 0;
  this->render.outlineAlpha = 0;
  this->render.useGroundLighting = 1;
  this->render.instant.Clear();
  this->render.blendFlags = 0;
  this->render.hoverOffset = 0;
}
