#include "Utility/NThread.h"

#include <process.h>
#include <utility>

namespace {

  // OFFSET: 0x00737510
  // Original debugger naming exception; this is not compiler array-unwind code.
  void SetDebuggerThreadName(unsigned id, const char* name) {
    ULONG_PTR arguments[4] = {0x1000, reinterpret_cast<ULONG_PTR>(name), id, 0};
    __try {
      RaiseException(0x406D1388, 0, 4, arguments);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
    }
  }

}

// OFFSET: 0x00737200
// FUNCTION: fn_07643
NThread::NThread(std::string name)
    : m_threadId(0), unknown_0008(true), m_state(0), m_handle(nullptr), m_name(std::move(name)) {
}

// OFFSET: 0x00737290
// FUNCTION: fn_07644
// OFFSET: 0x00737350
// FUNCTION: fn_07645
NThread::~NThread() {
  // The original installs NThread's own vtable before this sequence. A live
  // worker here therefore reaches its pure virtual stop slot, not an override.
  this->Stop();
}

// OFFSET: 0x007375C0
unsigned __stdcall NThread::Entry(void* parameter) {
  NThread* thread = static_cast<NThread*>(parameter);
  unsigned result = thread->Run();
  thread->m_state = 3;
  return result;
}

// ORIGINAL_SITE: 0x00737420 in fn_07585 (fragment)
bool NThread::Start() {
  if (this->m_handle || this->m_state) {
    return false;
  }
  this->m_handle =
    reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, Entry, this, CREATE_SUSPENDED, &this->m_threadId));
  if (!this->m_handle) {
    return false;
  }
  ResumeThread(this->m_handle);
  this->m_state = 1;
  SetDebuggerThreadName(this->m_threadId, this->m_name.c_str());
  return true;
}

// ORIGINAL_SITE: 0x00737480 in fn_07586 (fragment)
void NThread::Stop() {
  switch (this->m_state) {
  case 2:
    if (ResumeThread(this->m_handle) != static_cast<DWORD>(-1)) {
      this->m_state = 1;
    }
    [[fallthrough]];
  case 1:
    this->RequestStop();
    WaitForSingleObject(this->m_handle, 3000);
    [[fallthrough]];
  case 3:
    if (this->m_handle) {
      CloseHandle(this->m_handle);
      this->m_handle = nullptr;
    }
    this->m_state = 0;
    break;
  default:
    break;
  }
}

// OFFSET: 0x007374F0
// FUNCTION: fn_07646
void NThread::Resume() {
  if (this->m_state == 2 && ResumeThread(this->m_handle) != static_cast<DWORD>(-1)) {
    this->m_state = 1;
  }
}
