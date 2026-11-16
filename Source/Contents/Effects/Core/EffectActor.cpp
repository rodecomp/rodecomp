#include "Contents/Actors/ActorEnums.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Effects/Core/RagEffect.h"

// OFFSET: 0x00CA8C40
// FUNCTION: fn_20993
EffectActor::~EffectActor() {
  if (!this->effectList.empty()) {
    for (std::list<CRagEffect*>::iterator it = this->effectList.begin(); it != this->effectList.end(); ++it) {
      if (*it) {
        CRagEffect* effect = *it;
        effect->SendMsg(this, AM_MASTER_DEAD, 0, 0, 0, 0, 0, 0);
      }
    }
    this->effectList.erase(this->effectList.begin(), this->effectList.end());
  }
  this->firstEffect = nullptr;
  this->secondEffect = nullptr;
}

// OFFSET: 0x00CAF4B0
// FUNCTION: fn_21083
void EffectActor::ResetEffectState() {
  this->launchEffectId = -1;
  this->launchSkillLevel = 0;
  this->effectLaunched = 0;
  this->firstEffect = nullptr;
  this->secondEffect = nullptr;
  this->render.translucentOther[0] = 0;
}
