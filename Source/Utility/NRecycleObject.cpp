#include "Utility/NRecycleObject.h"

// OFFSET: 0x005B9E50
// FUNCTION: fn_03440
NRecycleObject::NRecycleObject(std::string poolName) : m_poolName(std::move(poolName)) {
}

// OFFSET: 0x005BA560
// FUNCTION: fn_03444
NRecycleObject::~NRecycleObject() = default;

// OFFSET: 0x005782C0
// FUNCTION: fn_02551
NRecycleObjectPoolInter::~NRecycleObjectPoolInter() = default;
