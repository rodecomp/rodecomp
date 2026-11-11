#include "Utility/Threading/NThread.h"

#include <process.h>
#include <utility>

extern template std::string::basic_string(const std::string&);

namespace {
  struct ThreadNameInfo {
    DWORD type;
    const char* name;
    DWORD threadId;
    DWORD flags;
  };

  // OFFSET: 0x00737510
  // FUNCTION: fn_07647
  void SetDebuggerThreadName(unsigned id, const char* name) {
    ThreadNameInfo info;
    info.type = 0x1000;
    info.name = name;
    info.threadId = id;
    info.flags = 0;
    __try {
      RaiseException(0x406D1388, 0, sizeof(info) / sizeof(ULONG_PTR), reinterpret_cast<ULONG_PTR*>(&info));
    } __except (EXCEPTION_EXECUTE_HANDLER) {
    }
  }
}

// OFFSET: 0x00737200
// FUNCTION: fn_07643
NThread::NThread(std::string name) : m_state(0), m_name(name) {
  this->m_handle = nullptr;
  this->m_threadId = 0;
  this->padding_0008 = true;
}

// OFFSET: 0x00737290
// FUNCTION: fn_07644
NThread::~NThread() {
  this->Stop();
}

// OFFSET: 0x007375C0
// FUNCTION: fn_07648
unsigned __stdcall NThread::Entry(void* parameter) {
  NThread* thread = static_cast<NThread*>(parameter);
  unsigned result = thread->Run();
  thread->m_state = 3;

  return result;
}

// OFFSET: 0x00737420
// FUNCTION: fn_45543
bool NThread::Start() {
  if (this->m_handle || this->m_state) {
    return false;
  }

  this->m_handle = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, Entry, this, CREATE_SUSPENDED, &this->m_threadId));
  if (!this->m_handle) {
    return false;
  }

  ResumeThread(this->m_handle);
  this->m_state = 1;
  SetDebuggerThreadName(this->m_threadId, this->m_name.c_str());

  return true;
}

// OFFSET: 0x00737480
// FUNCTION: fn_45509
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
