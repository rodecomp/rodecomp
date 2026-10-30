#ifndef RAGNAROK_SOURCE_SECURITY_REASSEMBLYPACKETMGR_H
#define RAGNAROK_SOURCE_SECURITY_REASSEMBLYPACKETMGR_H

#include <cstddef>

class CReassemblyPacketImpl {
public:
  CReassemblyPacketImpl();

  __declspec(noinline) static bool Create();
  __declspec(noinline) static void Destroy();

  void Attach();
  int IsReady();
  void Detach();
  int Notify(int event);

  /* 0000 */ bool m_valid;
  /* 0004 */ int unknown_0004;
  /* 0008 */ int unknown_0008;
  /* 000C */ int unknown_000C;
};

class CReassemblyPacketMgr {
public:
  CReassemblyPacketMgr() : m_valid(false) {
    if (this->Init()) {
      this->m_valid = true;
    }
  }

  ~CReassemblyPacketMgr() {
    CReassemblyPacketImpl::Destroy();
  }

  bool Init() {
    return CReassemblyPacketImpl::Create() != false;
  }

  static bool CreateInstance();
  static void DestroyInstance();

  void Release();

  /* 0000 */ bool m_valid;
};

extern CReassemblyPacketImpl* g_packetReassembler;
extern CReassemblyPacketMgr* g_reassemblyPacketMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(CReassemblyPacketImpl) == 0x10);
static_assert(offsetof(CReassemblyPacketImpl, unknown_000C) == 0xC);
static_assert(sizeof(CReassemblyPacketMgr) == 1);
#endif

#endif
