#ifndef RAGNAROK_SOURCE_CONTENTS_EVENTS_ROULETTEMGR_H
#define RAGNAROK_SOURCE_CONTENTS_EVENTS_ROULETTEMGR_H

#include "Contents/Items/ItemInfo.h"
#include "Render/Primitives/RenderPrimitive.h"

#include <cstddef>
#include <cstdint>
#include <map>

struct RouletteSlot {
  /* 0000 */ short row;
  /* 0002 */ short column;
  /* 0004 */ int itemId;
  /* 0008 */ short count;

  // INLINED
  bool operator<(const RouletteSlot& other) const {
    return this->row < other.row && this->column < other.column;
  }
};

struct RouletteVertex : RenderVertex {
  // INLINED
  __forceinline RouletteVertex() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
    this->rhw = 0;
    this->diffuse = 0;
    this->specular = 0xFF000000;
    this->u = 0;
    this->v = 0;
  }
};

struct RouletteQuad {
  /* 0000 */ RouletteVertex vertices[6];

  // INLINED
  __forceinline RouletteQuad() {}
};

struct RouletteQuads {
  /* 0000 */ RouletteQuad current;
  /* 00C0 */ RouletteQuad start;
  /* 0180 */ RouletteQuad target;

  // INLINED
  __forceinline RouletteQuads() {}
};

class RouletteMgr {
public:
  static bool Create();
  static void Destroy();

  /* 0000 */ unsigned char m_valid;
  /* 0004 */ int m_serial;
  /* 0008 */ int m_frame;
  /* 000C */ int m_cycle;
  /* 0010 */ unsigned char m_spinning;
  /* 0014 */ int m_stage;
  /* 0018 */ int resetState;
  /* 001C */ RouletteQuads m_quads;
  /* 025C */ unsigned long m_lastTick;
  /* 0260 */ unsigned long m_interval;
  /* 0264 */ int m_index;
  /* 0268 */ int m_additionItemId;
  /* 026C */ int m_goldPoint;
  /* 0270 */ int m_silverPoint;
  /* 0274 */ int m_bronzePoint;
  /* 0278 */ unsigned char m_resultShown;
  /* 0279 */ unsigned char m_bonus;
  /* 027C */ std::multimap<RouletteSlot, ITEM_INFO> m_items;
  /* 0288 */ ITEM_INFO m_resultItem;
  /* 0380 */ ITEM_INFO m_additionItem;

  __declspec(noinline) RouletteMgr();
  void AddItem(RouletteSlot slot);

  // INLINED
  void SetPoints(int gold, int silver, int bronze) {
    this->m_goldPoint = gold;
    this->m_silverPoint = silver;
    this->m_bronzePoint = bronze;
  }
  void Reset();
  void Draw();
  void Update();
  void SetAdditionItem(int itemId);
  void StartSpin(int x, int y, int originX, int originY);
  void SetQuad(int x, int y, int originX, int originY);
  void SetResult(unsigned short index, unsigned char shown, unsigned char bonus);
};

extern RouletteMgr* g_rouletteMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(RouletteSlot) == 12);
static_assert(sizeof(RouletteQuads) == 0x240);
static_assert(offsetof(RouletteMgr, m_quads) == 0x1C);
static_assert(offsetof(RouletteMgr, m_lastTick) == 0x25C);
static_assert(offsetof(RouletteMgr, m_resultShown) == 0x278);
static_assert(offsetof(RouletteMgr, m_items) == 0x27C);
static_assert(offsetof(RouletteMgr, m_resultItem) == 0x288);
static_assert(offsetof(RouletteMgr, m_additionItem) == 0x380);
static_assert(sizeof(RouletteMgr) == 0x478);
#endif

#endif
