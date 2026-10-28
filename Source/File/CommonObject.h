#ifndef RAGNAROK_SOURCE_FILE_COMMONOBJECT_H
#define RAGNAROK_SOURCE_FILE_COMMONOBJECT_H

#include <string>
#include <vector>

class CFile;
class ResourceBytes;

class CCommonObject {
public:
  // INLINED
  CCommonObject() {
    this->Clear();
  }
  virtual ~CCommonObject();
  void Clear();
  bool LoadTokens(std::vector<char*>& tokens, const char* name, int mode, ResourceBytes& file);
  bool LoadCsv(const char* name, std::vector<std::string> (*transform)(const std::vector<std::string>&), void (*insert)(std::vector<const char*>&));

private:
  bool OpenCsv(const char* name, CFile& file);
  std::vector<std::string> ReadCsvRow(CFile& file, char separator);

  /* 0004 */ std::vector<char*> m_buffers;
  CCommonObject(const CCommonObject&) = delete;
  CCommonObject& operator=(const CCommonObject&) = delete;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CCommonObject) == 16);
#endif

#endif
