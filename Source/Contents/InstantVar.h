#ifndef RAGNAROK_SOURCE_CONTENTS_INSTANTVAR_H
#define RAGNAROK_SOURCE_CONTENTS_INSTANTVAR_H

#include <cstdint>
#include <map>

// RTTI CInstantVar. Embedded by CRenderObject at +0x118
// and client at +0x7FA8. Initialization is inlined in their constructors.
class CInstantVar {
public:
  CInstantVar() = default;
  virtual ~CInstantVar();
  void Set(int key, std::int64_t value);
  std::int64_t Get(int key) const;
  void Clear();

private:
  // Original signed-key ordered tree, 32-byte x86 nodes containing an aligned
  // 64-bit mapped value. A missing read does not insert a zero-valued node.
  std::map<int, std::int64_t> m_values;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CInstantVar) == 12);
#endif

#endif
