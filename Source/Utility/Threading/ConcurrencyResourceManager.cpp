#include <cstdint>
#include <new>
#include <windows.h>
#include <mmiscapi2.h>

// GLOBAL: gl_24423
void* g_resourceState;

// GLOBAL: gl_24422
unsigned char* g_logContext;

// GLOBAL: gl_26229
char g_versionLogMessage[1];

struct TimerList {
  /* 0000 */ TimerList* next;
  /* 0004 */ TimerList* previous;
  /* 0008 */ TimerList* head;
  /* 000C */ std::uint16_t flags;
};

struct TimerLock : CRITICAL_SECTION {
  /* 0018 */ std::uint32_t initialized;
};

namespace Concurrency {
namespace details {

class Resource {
public:
  void* sub_E87DB0(int mode);
};

class ResourceManager {
public:
  /* 0000 */ TimerLock* lock;
  /* 0004 */ TimerList* list;
  /* 0008 */ void* unknown_0008;
  /* 000C */ MMRESULT timer;

  __declspec(noinline) ResourceManager* RetrieveSystemVersionInformation();
  void sub_E903F0(int key, void* result);
};

}
}

void CALLBACK fptc(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR);

char* sub_E939D0(const char* message);
void sub_E916F0(void* writer, const char* message);

// OFFSET: 0x00E87CE0

// OFFSET: 0x00E90470
// FUNCTION: fn_25504
int sub_E90470(int key, int mode) {
  if (g_resourceState == nullptr) {
    void* memory = ::operator new(0x10);
    Concurrency::details::ResourceManager* state = nullptr;
    if (memory != nullptr) {
      state = static_cast<Concurrency::details::ResourceManager*>(memory)->RetrieveSystemVersionInformation();
    }
    g_resourceState = state;
    if (state == nullptr) {
      return 0;
    }
  }

  void* memory = ::operator new(0x44);
  void* result = nullptr;
  if (memory != nullptr) {
    result = static_cast<Concurrency::details::Resource*>(memory)->sub_E87DB0(mode);
  }

  if (result == nullptr) {
    if (g_logContext != nullptr) {
      char* message = sub_E939D0(g_versionLogMessage);
      sub_E916F0(g_logContext + 0x24, message);
    }
    return 0;
  }

  static_cast<Concurrency::details::ResourceManager*>(g_resourceState)->sub_E903F0(key, result);
  return 1;
}
