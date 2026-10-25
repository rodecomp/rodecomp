#ifndef RAGNAROK_SOURCE_UTILITY_ASYNC_NOBJECT_H
#define RAGNAROK_SOURCE_UTILITY_ASYNC_NOBJECT_H

#include <cstdint>

class NObject {
public:
  NObject();
  virtual ~NObject();

protected:
  /* 0004 */ std::uint32_t m_objectId;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(NObject) == 8);
#endif

#endif
