#include "Contents/JobNames.h"
#include "Contents/RenderObjectState.h"
#include "Resource/ActionResource.h"
#include "Resource/ResourceMgr.h"
#include "Resource/SpriteResource.h"

#include <cstdio>
#include <mmsystem.h>

// OFFSET: 0x00CA8730
// FUNCTION: fn_20991
RenderObjectState::RenderObjectState(ResourceMgr& resources, JobNames& names) : sprite(nullptr), action(nullptr) {
  this->Reset(resources, names);
}

// OFFSET: 0x00CA8F90
// FUNCTION: fn_20998
RenderObjectState::~RenderObjectState() {
  if (this->sprite) {
    this->sprite->Release();
  }

  if (this->action) {
    this->action->Release();
  }
}

// OFFSET: 0x00CAF990
// FUNCTION: fn_21086
void RenderObjectState::Reset(ResourceMgr& resources, JobNames& names) {
  this->position[0] = 0;
  this->position[1] = 0;
  this->position[2] = 0;
  this->animation = 0;
  this->previousAnimation = 0;
  this->unknown_0040 = 0;
  this->unknown_003C = 0;
  this->unknown_0044 = 0;
  this->angle = 0;
  this->scale = 1;
  this->unknown_0054 = 1;
  this->unknown_006C = 0;
  this->speed = 1;
  this->unknown_0070 = 0;
  this->visible = 1;
  this->enabled = 1;
  this->color = 0xFFFFFFFFu;
  this->unknown_00A2 = 0;
  this->unknown_00A4 = 0;
  this->unknown_009C[1] = 0;
  this->unknown_009C[2] = 0;
  this->unknown_0078 = 0;
  this->unknown_007C = 0;
  this->started = timeGetTime();
  this->unknown_009C[3] = 0;
  this->unknown_009C[0] = 0;
  this->unknown_00A3 = 0;
  this->overrideColor = 0xFFFF0000u;
  this->unknown_0080 = 0;
  this->unknown_0048 = 0;
  this->unknown_005C = 1;
  this->unknown_0060 = 1;
  this->unknown_0064 = 1;
  this->unknown_00A8 = 0;

  char spriteName[128];
  char actionName[128];
  std::sprintf(spriteName, "NPC\\%s.spr", names.Lookup(46, -1));
  std::sprintf(actionName, "NPC\\%s.act", names.Lookup(46, -1));

  if (this->sprite) {
    this->sprite->Release();
  }

  this->sprite = static_cast<CSprRes*>(resources.Load(spriteName));

  if (this->sprite) {
    ++this->sprite->references;
  }

  if (this->action) {
    this->action->Release();
  }

  this->action = static_cast<CActRes*>(resources.Load(actionName));

  if (this->action) {
    ++this->action->references;
  }

  this->unknown_0090 = 0;
  this->unknown_00F4 = 0;
  this->unknown_00F6 = 0;
  this->unknown_00F8 = 0;
  this->unknown_00FA = 0;
  this->unknown_00FC = 0;
  this->unknown_0100 = 0;
  this->accountId = 0;
  this->unknown_0114 = 0;
  this->unknown_00C8 = 0;
  this->unknown_00D8 = 0;
  this->unknown_00DC = 0;
  this->unknown_00C2 = 10;
  this->unknown_00EC = 1;
  this->flags = 0;
  this->unknown_00F0 = 0;
  this->unknown_00F2 = 0;
  this->unknown_00B4 = 1;
  this->instant.Clear();
  this->unknown_0124 = 0;
  this->unknown_0128 = 0;
  this->unknown_012C = 0;
}
