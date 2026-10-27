#ifndef RAGNAROK_SOURCE_CONTENTS_MOVIES_BINKMOVIE_H
#define RAGNAROK_SOURCE_CONTENTS_MOVIES_BINKMOVIE_H

#include "Resource/BinkRuntime.h"

#include <vector>

class ResourceBytes;
class CRenderSystemBase;
class CTexture;

struct BinkMovieContext {
  BinkRuntime& runtime;
  ResourceBytes& file;
  CRenderSystemBase*& renderSystem;
};

class BinkMovie {
public:
  explicit BinkMovie(BinkMovieContext& context);
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
  std::vector<unsigned char> m_bytes;
  BinkMovieContext* m_context;
};

#endif
