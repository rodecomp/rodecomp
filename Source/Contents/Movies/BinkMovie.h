#ifndef RAGNAROK_SOURCE_CONTENTS_MOVIES_BINKMOVIE_H
#define RAGNAROK_SOURCE_CONTENTS_MOVIES_BINKMOVIE_H

#include "Resource/BinkRuntime.h"

class CFile;
class CTexture;

class BinkMovie {
public:
  BinkMovie();
  ~BinkMovie();
  bool Open(const char* name, int loop, bool memory);
  void Close();
  bool Update();
  bool CopyTo(CTexture* texture);
  // INLINED
  BinkHandle* Handle() const {
    return this->m_handle;
  }
  int State() const {
    return this->m_state;
  }

private:
  /* 0000 */ BinkHandle* m_handle;
  /* 0004 */ int m_state;
  /* 0008 */ int m_loop;
  /* 000C */ int unknown_000C;
  /* 0010 */ CFile* m_file;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(BinkMovie) == 0x0014);
#endif

#endif
