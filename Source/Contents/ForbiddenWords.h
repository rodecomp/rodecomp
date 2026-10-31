#ifndef RAGNAROK_SOURCE_CONTENTS_FORBIDDENWORDS_H
#define RAGNAROK_SOURCE_CONTENTS_FORBIDDENWORDS_H

#include <string>
#include <vector>

class CCommonObject;
class ResourceBytes;

struct ForbiddenWords {
  /* 0000 */ std::vector<const char*> words;

  bool Load(const char* name, CCommonObject& storage, ResourceBytes& file);
  bool Allows(const char* text) const;
  bool Contains(const char* text) const;
  void Mask(char* text, std::string* value = nullptr) const;
};

static_assert(sizeof(ForbiddenWords) == 0xC);

extern ForbiddenWords g_forbiddenWords;

#endif
