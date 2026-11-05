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
  std::map<int, std::int64_t>& Values();
  const std::map<int, std::int64_t>& Values() const;

private:
  friend class CSession;
  /* 0004 */ std::map<int, std::int64_t> m_values;
};

// INLINED
inline std::map<int, std::int64_t>& CInstantVar::Values() {
  return this->m_values;
}

// INLINED
inline const std::map<int, std::int64_t>& CInstantVar::Values() const {
  return this->m_values;
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CInstantVar) == 12);
#endif

#endif
