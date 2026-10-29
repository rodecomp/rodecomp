#include "Utility/GGHashMgr.h"

#include <new>
#include <string>

// GLOBAL: gl_24843
CGGHashMgr* g_ggHashMgr = nullptr;

// OFFSET: 0x0076B590
// FUNCTION: fn_08188
CGGHashMgr::CGGHashMgr() : m_initialized(false), m_hash(nullptr) {
  InitializeCriticalSection(&this->m_lock);

  this->m_hash = new (std::nothrow) CGGHash;
  if (this->m_hash) {
    bool valid;
    {
      std::string filename("RagHash.dat");
      valid = this->m_hash->Load(filename.c_str()) && this->m_hash->GetSize() >= 2 && this->m_hash->Check() == 1;
    }

    if (valid) {
      this->m_initialized = true;
    }
  }
}

// OFFSET: 0x0076B7C0
// FUNCTION: fn_08189
CGGHashMgr::~CGGHashMgr() {
  if (this->m_hash) {
    delete this->m_hash;
    this->m_hash = nullptr;
  }

  DeleteCriticalSection(&this->m_lock);
}

// OFFSET: 0x0076B840
// FUNCTION: fn_08190
int CGGHashMgr::IsValid() {
  return 1;
}

// OFFSET: 0x0076B870
// FUNCTION: fn_08191
void CGGHashMgr::VerifyFile(const char* filename) {
}

// OFFSET: 0x0076B970
// FUNCTION: fn_08193
void CGGHashMgr::VerifyBuffer(const char* name, const unsigned char* buf, unsigned int size) {
}

// OFFSET: 0x0076BB50
// FUNCTION: fn_08195
void CGGHashMgr::VerifyPendingFiles() {
}

// OFFSET: 0x0076BE20
// FUNCTION: fn_08196
int CGGHashMgr::Create() {
  return 1;
}

// OFFSET: 0x0076BEE0
// FUNCTION: fn_08198
void CGGHashMgr::Destroy() {
  if (g_ggHashMgr) {
    delete g_ggHashMgr;
    g_ggHashMgr = nullptr;
  }
}
