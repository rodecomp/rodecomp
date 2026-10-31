#ifndef RAGNAROK_SOURCE_RESOURCE_RMARESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_RMARESOURCE_H

#include "Resource/Resource.h"

#include <cstddef>
#include <string>
#include <vector>

struct RmaAction {
  /* 0000 */ int startFrame = -1;
  /* 0004 */ int endFrame = -1;
  /* 0008 */ int delay = 4;
  /* 000C */ std::vector<int> frames;
};

class CRmaRes : public CRes {
public:
  CRmaRes();
  ~CRmaRes() override;
  CRmaRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;

  /* 0110 */ std::vector<RmaAction> actions;
  /* 011C */ std::vector<std::string> models;
  /* 0128 */ std::vector<int> actionModels;
  /* 0134 */ float interiorZoom = 1.0f;
  /* 0138 */ float shadowZoom = 1.0f;
  /* 013C */ int interiorOffsetX = 0;
  /* 0140 */ int interiorOffsetY = 0;
  /* 0144 */ int interiorFrameStart = 0;
  /* 0148 */ int interiorFrameEnd = 0;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RmaAction) == 0x18);
static_assert(offsetof(CRmaRes, actions) == 0x110);
static_assert(offsetof(CRmaRes, interiorZoom) == 0x134);
static_assert(sizeof(CRmaRes) == 0x14C);
#endif

#endif
