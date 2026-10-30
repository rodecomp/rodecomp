#include "Security/ReassemblyPacketMgr.h"

#include <new>

// GLOBAL: gl_24984
CReassemblyPacketImpl* g_packetReassembler;

// GLOBAL: gl_24985
CReassemblyPacketMgr* g_reassemblyPacketMgr;

// OFFSET: 0x00B154C0
// FUNCTION: fn_18417
void CReassemblyPacketImpl::Attach() {
}

// OFFSET: 0x00B154D0
// FUNCTION: fn_18418
int CReassemblyPacketImpl::IsReady() {
  return 1;
}

// OFFSET: 0x00B154E0
// FUNCTION: fn_18419
CReassemblyPacketImpl::CReassemblyPacketImpl() {
  this->unknown_0004 = 0;
  this->unknown_0008 = 0;
  this->unknown_000C = 0;
  this->m_valid = true;
}

// OFFSET: 0x00B15500
// FUNCTION: fn_18420
void CReassemblyPacketImpl::Detach() {
}

// OFFSET: 0x00B15510
// FUNCTION: fn_18421
bool CReassemblyPacketImpl::Create() {
  if (g_packetReassembler == nullptr) {
    g_packetReassembler = new (std::nothrow) CReassemblyPacketImpl();
    if (g_packetReassembler != nullptr) {
      if (g_packetReassembler->m_valid) {
        return true;
      }
      delete g_packetReassembler;
      g_packetReassembler = nullptr;
    }
  }
  return false;
}

// OFFSET: 0x00B155D0
// FUNCTION: fn_18422
void CReassemblyPacketImpl::Destroy() {
  if (g_packetReassembler != nullptr) {
    delete g_packetReassembler;
    g_packetReassembler = nullptr;
  }
}

// OFFSET: 0x00B155F0
// FUNCTION: fn_18423
int CReassemblyPacketImpl::Notify(int) {
  return 0;
}

// OFFSET: 0x00B156E0
// FUNCTION: fn_18424
bool CReassemblyPacketMgr::CreateInstance() {
  if (g_reassemblyPacketMgr == nullptr) {
    g_reassemblyPacketMgr = new (std::nothrow) CReassemblyPacketMgr();
    if (g_reassemblyPacketMgr != nullptr) {
      if (g_reassemblyPacketMgr->m_valid) {
        return true;
      }
      delete g_reassemblyPacketMgr;
      g_reassemblyPacketMgr = nullptr;
    }
  }
  return false;
}

// OFFSET: 0x00B157A0
// FUNCTION: fn_18425
void CReassemblyPacketMgr::Release() {
  CReassemblyPacketImpl::Destroy();
}

// OFFSET: 0x00B157B0
// FUNCTION: fn_18426
void CReassemblyPacketMgr::DestroyInstance() {
  if (g_reassemblyPacketMgr != nullptr) {
    delete g_reassemblyPacketMgr;
    g_reassemblyPacketMgr = nullptr;
  }
}
