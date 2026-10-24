#ifndef RAGNAROK_SOURCE_RESOURCE_ACTIONRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_ACTIONRESOURCE_H

#include "Render/SpritePixels.h"
#include "Resource/Resource.h"

#include <string>
#include <vector>

class ResourceBytes;
class ResourceMgr;

struct ActionAnchor {
  std::int32_t x;
  std::int32_t y;
  std::int32_t unknown_0008;
};

struct ActionFrame {
  std::int32_t unknown_0000[8]{};
  std::vector<SpriteFrameCell> cells;
  int cellCount = 0;
  int sound = 0;
  std::vector<ActionAnchor> anchors;
  int anchorCount = 0;

  const SpriteFrameCell& Cell(unsigned index) const;
};

struct ActionResourceContext {
  // Must retain this load's bytes across recursive ResourceMgr::Load calls;
  // use a distinct file owner from the sprite prototype, as original CFile does.
  ResourceBytes& file;
  ResourceMgr& resources;
  CRenderSystemBase*& renderSystem;
};

// RTTI CActRes, original size 312. Frame/sound vectors
// are released by Unload; animation speeds survive until destruction/reload.
class CActRes : public CRes {
public:
  explicit CActRes(ActionResourceContext& context);
  ~CActRes() override;
  CActRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  const std::vector<ActionFrame>& Animation(unsigned index) const;
  float Speed(unsigned index) const;
  const char* Sound(unsigned index) const;

  /* 0110 */ std::vector<std::vector<ActionFrame>> animations;
  /* 011C */ int maximumCells;
  /* 0120 */ std::vector<std::string> sounds;
  /* 012C */ std::vector<float> speeds;

private:
  ActionResourceContext* m_context;
};

static_assert(sizeof(ActionAnchor) == 12);
#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ActionFrame) == 68);
static_assert(sizeof(CActRes) == 316);
#endif

#endif
