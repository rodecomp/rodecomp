#ifndef RAGNAROK_SOURCE_FILE_COMMONOBJECT_H
#define RAGNAROK_SOURCE_FILE_COMMONOBJECT_H

#include <vector>

class ResourceBytes;

// RTTI CCommonObject. Token pointers borrow the
// retained file allocations until Clear/destruction, not the input file view.
class CCommonObject {
public:
  CCommonObject() = default;
  virtual ~CCommonObject();
  void Clear();
  bool LoadTokens(std::vector<char*>& tokens, const char* name, int mode, ResourceBytes& file);

private:
  /* 0004 */ std::vector<char*> m_buffers;
  CCommonObject(const CCommonObject&) = delete;
  CCommonObject& operator=(const CCommonObject&) = delete;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCommonObject) == 16);
#endif

#endif
