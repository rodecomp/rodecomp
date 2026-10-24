#ifndef RAGNAROK_SOURCE_CONTENTS_BINKMOVIE_H
#define RAGNAROK_SOURCE_CONTENTS_BINKMOVIE_H

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

// Reconstructed name for 20-byte decoder wrapper.
// Owned input snapshots replace the original retained CFile read allocation.
class BinkMovie {
public:
  explicit BinkMovie(BinkMovieContext& context);
  ~BinkMovie();
  bool Open(const char* name, int loop, bool memory);
  void Close();
  bool Update();
  bool CopyTo(CTexture* texture);
  BinkHandle* Handle() const {
    return this->m_handle;
  }
  int State() const {
    return this->m_state;
  }

private:
  BinkHandle* m_handle;
  int m_state;
  int m_loop;
  std::vector<unsigned char> m_bytes;
  BinkMovieContext* m_context;
};

#endif
