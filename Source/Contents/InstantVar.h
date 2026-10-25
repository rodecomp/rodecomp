#ifndef RAGNAROK_SOURCE_CONTENTS_INSTANTVAR_H
#define RAGNAROK_SOURCE_CONTENTS_INSTANTVAR_H

#include <cstdint>
#include <map>

class CInstantVar {
public:
  CInstantVar() = default;
  virtual ~CInstantVar();
  void Set(int key, std::int64_t value);
  std::int64_t Get(int key) const;
  void Clear();

private:
  std::map<int, std::int64_t> m_values;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CInstantVar) == 12);
#endif

#endif
