#include "Utility/Hash/GGHashMgr.h"

#include "Client/Session/Session.h"
#include "Contents/Tables/MessageTable.h"
#include "Resource/MSGSTRINGID.h"
#include "Security/NProtectGameGuardMgr.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Threading/NSyncObject.h"

#include <new>
#include <string>
#include <vector>

// GLOBAL: gl_24843
CGGHashMgr* g_ggHashMgr = nullptr;

// STRING: st_02588
static const char g_ggHashMgrSource[] =
    "C:\\repositories\\RagnarokClient\\RagnarokClient\\Security Solution\\GGHash\\CGGHashMgr.cpp";

// STRING: st_02589
static const char g_ggHashErrorFormat[] = "%s ( %d )";

// OFFSET: 0x0076B590
// FUNCTION: fn_08188
CGGHashMgr::CGGHashMgr() {
  this->m_hash = nullptr;
  this->m_initialized = false;
  InitializeCriticalSection(&this->m_lock);

  this->m_hash = new (std::nothrow) CGGHash;
  if (this->m_hash) {
    bool valid;
    {
      std::string filename("RagHash.dat");
      if (!this->m_hash->Load(filename.c_str())) {
        return;
      }

      if (this->m_hash->GetSize() < 2) {
        return;
      }

      valid = this->m_hash->Check() == 1;
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
  NSyncObject lock(&this->m_lock);
  if (!g_session.skyMapNames.empty()) {
    for (std::vector<std::string>::iterator it = g_session.skyMapNames.begin(); it != g_session.skyMapNames.end(); ++it) {
      const char* filename = it->c_str();
      NSyncObject verifyLock(&this->m_lock);
      if (!this->m_hash->VerifyFile(filename)) {
        const DWORD error = this->m_hash->GetError();
        CNProtectGameGuardMgr* manager = g_nProtectGameGuardMgr;
        manager->SetError(g_ggHashMgrSource, 0x8C,
                          std::string(FormatString(g_ggHashErrorFormat, LookupMessage(MSI_CHEAT_HASHFILE), error)));
      }
    }
    g_session.skyMapNames.clear();
  }
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
