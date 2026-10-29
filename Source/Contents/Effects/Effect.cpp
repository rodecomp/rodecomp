#include "Contents/Effects/Effect.h"

#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/World/WorldScene.h"

// OFFSET: 0x00B4DAE0
// FUNCTION: fn_18979
CEffect::CEffect(int value)
    : id(value), position{}, sourcePosition{}, actor(0), sourceActor(0), frame(0), unknown_002C(60), unknown_0030(1), outsideRange(0), range(130),
      unknown_00B0(-1), retained(0), forceRender(0), unknown_00C4(0), unknown_00C8(0) {
}

// OFFSET: 0x00B4DD10
// OFFSET: 0x00B4DDD0
// FUNCTION: fn_18981
CEffect::~CEffect() = default;

// OFFSET: 0x005A2150
// FUNCTION: fn_03087
void CEffect::Close() {
}

// OFFSET: 0x00B4DE40
// FUNCTION: fn_18984
void CEffect::AddEvent(int kind, const char* text, int frame, int argument) {
  EffectEvent event{kind, frame, argument, text ? text : ""};
  this->events.push_back(event);
}

// OFFSET: 0x00B4DF90
// FUNCTION: fn_18985
bool CEffect::RetireIfInactive() {
  if (!this->Slot06()) {
    g_effectMgr->Retire(this, 0);
    return true;
  }

  return false;
}

// OFFSET: 0x00B4E150
// FUNCTION: fn_18987
bool CEffect::Slot06() {
  if (this->actor != 0 && !FindRenderObject(this->actor)) {
    return false;
  }

  return true;
}

// OFFSET: 0x00B4E170
// FUNCTION: fn_18988
void CEffect::SetEventFrame(int kind, int frame) {
  for (EffectEvent& event : this->events) {
    if (event.kind == kind) {
      event.frame = frame;
      return;
    }
  }
}

// OFFSET: 0x00B4E390
// FUNCTION: fn_18991
void CEffect::SetActor(std::uint32_t aid) {
  this->actor = aid;
}

// OFFSET: 0x00B4E3A0
// FUNCTION: fn_18992
void CEffect::SetSourceActor(std::uint32_t aid) {
  if (CRenderObject* source = FindRenderObject(aid)) {
    this->sourceActor = aid;
    this->sourcePosition[0] = source->render.position.x;
    this->sourcePosition[1] = source->render.position.y;
    this->sourcePosition[2] = source->render.position.z;
  }
}

// OFFSET: 0x00B249A0
// FUNCTION: fn_18773
void EffectRetirement::Clear() {
  for (auto effect = this->effects.begin(); effect != this->effects.end(); ++effect) {
    (*effect)->Close();
    delete *effect;
  }

  this->effects.clear();
}
