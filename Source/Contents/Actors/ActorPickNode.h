#ifndef RAGNAROK_SOURCE_CONTENTS_ACTORS_ACTORPICKNODE_H
#define RAGNAROK_SOURCE_CONTENTS_ACTORS_ACTORPICKNODE_H

#include <cstddef>
#include <cstdint>
#include <list>

struct ActorPickPoint {
  // INLINED
  ActorPickPoint() : x(0.0f), y(0.0f), z(0.0f) {
  }

  /* 0000 */ float x;
  /* 0004 */ float y;
  /* 0008 */ float z;
};

struct ActorPickInfo {
  bool Contains(float x, float y) const;

  /* 0000 */ ActorPickPoint corners[2];
  /* 0018 */ std::uint32_t accountId;
  /* 001C */ int job;
  /* 0020 */ int kind;
  /* 0024 */ int healthState;
};

struct ActorPickBounds {
  /* 0000 */ int left;
  /* 0004 */ int top;
  /* 0008 */ int right;
  /* 000C */ int bottom;
};

class CActorPickNode {
public:
  CActorPickNode();
  virtual ~CActorPickNode();
  void AddPickInfo(ActorPickInfo* info);
  __declspec(noinline) void Clear();
  ActorPickInfo* Pick(float x, float y);
  void MakeQuardTree(int depth, ActorPickBounds bounds);
  int QuadrantAt(int x, int y) const;
  ActorPickBounds ChildBounds(ActorPickBounds bounds, int quadrant) const;

  /* 0004 */ ActorPickBounds bounds;
  /* 0014 */ CActorPickNode* children[4];
  /* 0024 */ std::list<ActorPickInfo*> pickInfos;
};

extern CActorPickNode g_actorPickNode;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ActorPickInfo) == 0x28);
static_assert(sizeof(ActorPickBounds) == 0x10);
static_assert(offsetof(CActorPickNode, children) == 0x14);
static_assert(offsetof(CActorPickNode, pickInfos) == 0x24);
static_assert(sizeof(CActorPickNode) == 0x2C);
#endif

#endif
