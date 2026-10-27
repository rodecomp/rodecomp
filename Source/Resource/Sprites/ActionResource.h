#ifndef RAGNAROK_SOURCE_RESOURCE_SPRITES_ACTIONRESOURCE_H
#define RAGNAROK_SOURCE_RESOURCE_SPRITES_ACTIONRESOURCE_H

#include "Render/Textures/SpritePixels.h"
#include "Resource/Resource.h"

#include <cstddef>
#include <string>
#include <vector>

class ResourceBytes;
class ResourceMgr;

struct ActionAnchor {
  /* 0000 */ std::int32_t x;
  /* 0004 */ std::int32_t y;
  /* 0008 */ std::int32_t unknown_0008;
};

struct ActionFrame {
  /* 0000 */ std::int32_t unknown_0000[8]{};
  /* 0020 */ std::vector<SpriteFrameCell> cells;
  /* 002C */ int cellCount = 0;
  /* 0030 */ int sound = 0;
  /* 0034 */ std::vector<ActionAnchor> anchors;
  /* 0040 */ int anchorCount = 0;

  const SpriteFrameCell& Cell(unsigned index) const;
};

struct ActionResourceContext {
  ResourceBytes& file;
  ResourceMgr& resources;
  CRenderSystemBase*& renderSystem;
};

class CActRes : public CRes {
public:
  explicit CActRes(ActionResourceContext& context);
  ~CActRes() override;
  CActRes* Create() override;
  bool Load(const char* name) override;
  void Unload() override;
  const std::vector<ActionFrame>& Animation(unsigned index) const;
  const ActionFrame& Frame(unsigned animation, unsigned frame) const;
  unsigned FrameCount(unsigned animation) const;
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
static_assert(offsetof(ActionAnchor, y) == 0x0004);
static_assert(offsetof(ActionAnchor, unknown_0008) == 0x0008);
static_assert(offsetof(ActionFrame, cells) == 0x0020);
static_assert(offsetof(ActionFrame, cellCount) == 0x002C);
static_assert(offsetof(ActionFrame, sound) == 0x0030);
static_assert(offsetof(ActionFrame, anchors) == 0x0034);
static_assert(offsetof(ActionFrame, anchorCount) == 0x0040);
static_assert(offsetof(CActRes, animations) == 0x0110);
static_assert(offsetof(CActRes, maximumCells) == 0x011C);
static_assert(offsetof(CActRes, sounds) == 0x0120);
static_assert(offsetof(CActRes, speeds) == 0x012C);
#endif

#endif
