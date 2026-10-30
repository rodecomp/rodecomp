#ifndef RAGNAROK_SOURCE_ACTORPICKNODE_H
#define RAGNAROK_SOURCE_ACTORPICKNODE_H

#include <cstddef>
#include <list>

struct ActorPickInfo {
  /* 0000 */ float left;
  /* 0004 */ float top;
  /* 0008 */ unsigned char unknown_0008[0x20];
};

class CActorPickNode {
public:
  void AddPickInfo(ActorPickInfo* info);

  /* 0000 */ void* vtable;
  /* 0004 */ int left;
  /* 0008 */ int top;
  /* 000C */ int right;
  /* 0010 */ int bottom;
  /* 0014 */ CActorPickNode* children[4];
  /* 0024 */ std::list<ActorPickInfo*> pickInfos;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ActorPickInfo) == 0x28);
static_assert(offsetof(CActorPickNode, children) == 0x14);
static_assert(offsetof(CActorPickNode, pickInfos) == 0x24);
static_assert(sizeof(CActorPickNode) == 0x2C);
#endif

#endif
