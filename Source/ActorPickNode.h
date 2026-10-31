#ifndef RAGNAROK_SOURCE_ACTORPICKNODE_H
#define RAGNAROK_SOURCE_ACTORPICKNODE_H

#include <cstddef>
#include <cstdint>
#include <list>

struct ActorPickInfo {
  // INLINED
  ActorPickInfo() : left(0.0f), top(0.0f), depth(0.0f), right(0.0f), bottom(0.0f), depthOffset(0.0f) {
  }

  /* 0000 */ float left;
  /* 0004 */ float top;
  /* 0008 */ float depth;
  /* 000C */ float right;
  /* 0010 */ float bottom;
  /* 0014 */ float depthOffset;
  /* 0018 */ std::uint32_t accountId;
  /* 001C */ int job;
  /* 0020 */ int kind;
  /* 0024 */ int unknown_0024;
};

struct ActorPickBounds {
  /* 0000 */ int left;
  /* 0004 */ int top;
  /* 0008 */ int right;
  /* 000C */ int bottom;
};

class CActorPickNode {
public:
  // INLINED
  CActorPickNode() : children{} {
  }

  virtual ~CActorPickNode();
  void AddPickInfo(ActorPickInfo* info);
  void Clear();
  ActorPickInfo* Pick(float x, float y);
  void MakeQuardTree(int depth, ActorPickBounds bounds);

  /* 0004 */ int left;
  /* 0008 */ int top;
  /* 000C */ int right;
  /* 0010 */ int bottom;
  /* 0014 */ CActorPickNode* children[4];
  /* 0024 */ std::list<ActorPickInfo*> pickInfos;
};

extern CActorPickNode g_actorPickNode;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ActorPickInfo) == 0x28);
static_assert(offsetof(ActorPickInfo, depth) == 0x08);
static_assert(offsetof(ActorPickInfo, right) == 0x0C);
static_assert(offsetof(ActorPickInfo, bottom) == 0x10);
static_assert(offsetof(ActorPickInfo, depthOffset) == 0x14);
static_assert(sizeof(ActorPickBounds) == 0x10);
static_assert(offsetof(CActorPickNode, children) == 0x14);
static_assert(offsetof(CActorPickNode, pickInfos) == 0x24);
static_assert(sizeof(CActorPickNode) == 0x2C);
#endif

#endif
