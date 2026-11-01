#ifndef RAGNAROK_SOURCE_CONTENTS_FORBIDDENWORDS_H
#define RAGNAROK_SOURCE_CONTENTS_FORBIDDENWORDS_H

#include <string>
#include <vector>

struct ForbiddenWords {
  /* 0000 */ std::vector<const char*> words;

  ~ForbiddenWords();
  bool Load(const char* name);
  bool Allows(const char* text) const;
  bool Contains(const char* text) const;
  void Mask(char* text, std::string* value = nullptr) const;
};

static_assert(sizeof(ForbiddenWords) == 0xC);

extern ForbiddenWords g_forbiddenWords;

#endif
