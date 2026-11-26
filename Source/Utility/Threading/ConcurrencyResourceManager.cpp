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
  /* 0000 */ void* lock;
  /* 0004 */ void* unknown_0004;
  /* 0008 */ void* allocation;
  /* 000C */ unsigned int size;
  /* 0010 */ unsigned int pages;
  /* 0014 */ void* unknown_0014;
  /* 0018 */ unsigned int initialized;
  /* 001C */ unsigned int seed;
  /* 0020 */ int mode;
  /* 0024 */ unsigned int regionSize;
  /* 0028 */ MEMORY_BASIC_INFORMATION regionInfo;

  Resource(int mode);
};

class ResourceManager {
public:
  /* 0000 */ TimerLock* lock;
  /* 0004 */ TimerList* list;
  /* 0008 */ void* unknown_0008;
  /* 000C */ MMRESULT timer;

  ResourceManager();
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
    g_resourceState = new Concurrency::details::ResourceManager;
    if (g_resourceState == nullptr) {
      return 0;
    }
  }

  Concurrency::details::Resource* resource = new Concurrency::details::Resource(mode);
  if (resource == nullptr) {
    if (g_logContext != nullptr) {
      char* message = sub_E939D0(g_versionLogMessage);
      sub_E916F0(g_logContext + 0x24, message);
    }
    return 0;
  }

  static_cast<Concurrency::details::ResourceManager*>(g_resourceState)->sub_E903F0(key, resource);
  return 1;
}
