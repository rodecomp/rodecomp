#include "Utility/Threading/ConcurrencyResourceManager.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

// GLOBAL: gl_24423
Concurrency::details::ResourceManager* g_resourceState;

// GLOBAL: gl_24422
LogContext* g_logContext;

// GLOBAL: gl_26229
char g_versionLogMessage[28] = "\x01\x4A\x43\x15\x7C\xB8\x4B\xCE\xA0\xD1\x01\xF6\xA0\x64\x21\x3A\xC4\xBC\xC6\x43\xEB\x73\x5B\xCC\x56\xB7\xBF";

// GLOBAL: gl_26236
char g_resourceLogFormat[23] = "\x01\xB6\x87\xDC\xED\x72\xDB\x36\x4B\xD2\x12\xA6\xFF\x92\x7F\x5E\x8E\x42\xCB\xCE\x26\x26";

// GLOBAL: gl_26237
char g_logLineFormat[31] = "\x01\xC8\xBD\x6A\xAC\xE4\x87\x3C\x7B\xD9\x1D\xF2\x1D\x9E\x00\x98\x03\xA8\xA7\x35\x59\x47\xF1\xD8\x01\xBC\x28\x13\x54\x4E";

// GLOBAL: gl_26238
char g_protectLogFormat[73] = "\x01\x0D\x8C\x99\x93\xE7\x64\x91\xCF\x0E\x2E\x29\x00\xC9\x54\x65\x25\x71\x4E\xEF\x15\xFD\xEE\x95\x7D\x25\xD0\x0D\xA4\x0E\xEB\x49\xCC\x0B\xEC\xAD\x68\x4C\x0C\xB9\xD8\x4C\xC3\x6F\x26\x35\x25\x62\x72\x31\x49\x19\x9C\x69\x70\x14\xEC\x60\x06\xCD\x25\x4C\xC8\xC5\x8A\x90\x6F\x36\x6A\x09\x91\x20";

// GLOBAL: gl_24424
char* g_logTextSlots[20];

// GLOBAL: gl_26233
unsigned int g_logTextSlotCount;

// GLOBAL: gl_26234
DWORD g_logTextThreadIds[20];

// OFFSET: 0x00E87CE0
// FUNCTION: fn_25441
__declspec(noinline) Concurrency::details::ResourceManager::ResourceManager() {
  this->timer = timeSetEvent(10000, 0, fptc, 0, TIME_PERIODIC);

  TimerLock* lock = new TimerLock;
  if (lock == nullptr) {
    this->lock = nullptr;
    return;
  }
  this->lock = lock;
}

// OFFSET: 0x00E87DB0
// FUNCTION: fn_25442
__declspec(noinline) Concurrency::details::Resource::Resource(int mode) {
  this->mode = mode;
  srand(GetTickCount());
  this->seed = rand();
  if (this->seed == 0) {
    this->seed = 1;
  }

  this->regionSize = this->mode == 0 ? 2 : this->seed % 20 + 10;
  this->size = this->regionSize * 4;
  this->allocation = static_cast<unsigned int*>(VirtualAlloc(nullptr, this->size, MEM_COMMIT, PAGE_READONLY));
  if (this->allocation != nullptr) {
    VirtualQuery(this->allocation, &this->regionInfo, sizeof(this->regionInfo));
    this->pages = this->regionInfo.RegionSize / this->size;
    this->lock = new TimerLock;
  }

  if (g_logContext != nullptr) {
    sub_E916F0(&g_logContext->writer, sub_E939D0(g_resourceLogFormat), this->allocation, this->size, this->mode);
  }
  this->initialized = 0;
}

// OFFSET: 0x00E88720
// FUNCTION: fn_25450
void Concurrency::details::Resource::sub_E88720() {
  ScopedTimerLock guard(this->lock);

  const unsigned int base = reinterpret_cast<unsigned int>(this->regionInfo.AllocationBase);
  if (this->mode != 0) {
    if (this->initialized == 0 || this->allocation == nullptr) {
      return;
    }
    if (this->allocation[this->seed % this->regionSize] == (base / this->seed ^ this->allocation[(this->seed + 8) % this->regionSize])) {
      return;
    }
    if (!this->sub_E90560()) {
      return;
    }
    this->allocation[this->seed % this->regionSize] = base / this->seed ^ this->allocation[(this->seed + 8) % this->regionSize];
  } else {
    if (this->initialized == 0 || this->allocation == nullptr) {
      return;
    }
    if (this->allocation[0] == (base / this->seed ^ this->allocation[1])) {
      return;
    }
    if (!this->sub_E90560()) {
      return;
    }
    this->allocation[0] = base / this->seed ^ this->allocation[1];
  }
  this->sub_E8BAF0();
}

// OFFSET: 0x00E8BAF0
// FUNCTION: fn_25483
void Concurrency::details::Resource::sub_E8BAF0() {
  DWORD oldProtect;
  this->isProtected = VirtualProtect(this->regionInfo.AllocationBase, this->size, PAGE_READONLY, &oldProtect);
}

// OFFSET: 0x00E8CE30
// FUNCTION: fn_25496
void CALLBACK fptc(UINT, UINT, DWORD_PTR, DWORD_PTR, DWORD_PTR) {
  ScopedTimerLock guard(g_resourceState->lock);
  for (auto it = g_resourceState->resources.begin(); it != g_resourceState->resources.end(); ++it) {
    Concurrency::details::Resource* resource = it->second;
    ScopedTimerLock resourceGuard(resource->lock);
    resource->sub_E88720();
    resource->sub_E8CF10();
  }
}

// OFFSET: 0x00E8CF10
// FUNCTION: fn_25497
void Concurrency::details::Resource::sub_E8CF10() {
  if (this->allocation == nullptr) {
    return;
  }

  unsigned int* target;
  do {
    target = reinterpret_cast<unsigned int*>(static_cast<unsigned char*>(this->regionInfo.AllocationBase) + this->size * (rand() % this->pages));
  } while (this->allocation == target);

  ScopedTimerLock guard(this->lock);
  if (this->sub_E90560()) {
    if (target != nullptr) {
      memcpy(target, this->allocation, this->size);
      this->allocation = target;
    }
    this->sub_E8BAF0();
  }
}

// OFFSET: 0x00E903F0
// FUNCTION: fn_25503
void Concurrency::details::ResourceManager::sub_E903F0(unsigned int key, Resource* resource) {
  ScopedTimerLock guard(this->lock);
  this->resources[key] = resource;
}

// OFFSET: 0x00E90560
// FUNCTION: fn_25505
BOOL Concurrency::details::Resource::sub_E90560() {
  if (this->allocation == nullptr) {
    return FALSE;
  }

  DWORD oldProtect;
  BOOL result = VirtualProtect(this->regionInfo.AllocationBase, this->size, PAGE_READWRITE, &oldProtect);
  if (result && oldProtect != PAGE_READONLY && this->isProtected) {
    if (g_logContext != nullptr) {
      sub_E916F0(&g_logContext->writer, sub_E939D0(g_protectLogFormat), result, oldProtect, this->isProtected, this->allocation[0], this->allocation[1]);
    }
    result = FALSE;
  }
  return result;
}

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
      sub_E916F0(&g_logContext->writer, sub_E939D0(g_versionLogMessage));
    }
    return 0;
  }

  g_resourceState->sub_E903F0(key, resource);
  return 1;
}

// OFFSET: 0x00E916F0
// FUNCTION: fn_25523
bool sub_E916F0(LogWriter* writer, const char* format, ...) {
  char message[4096];

  if (writer->file == INVALID_HANDLE_VALUE) {
    return false;
  }

  if (writer->lineLimit != 0 && *format != '!') {
    if (writer->lineCount > writer->lineLimit) {
      return false;
    }
    ++writer->lineCount;
  }

  ScopedTimerLock guard(&writer->lock);

  va_list arguments;
  va_start(arguments, format);
  vsprintf(message, format, arguments);
  va_end(arguments);

  GetLocalTime(&writer->time);
  sprintf(writer->buffer, sub_E93D70(g_logLineFormat), writer->time.wHour, writer->time.wMinute, writer->time.wSecond, writer->time.wMilliseconds,
          message);
  DWORD length = static_cast<DWORD>(strlen(writer->buffer));

  if (writer->xorKey != 0) {
    for (int index = 0; index < static_cast<int>(length); ++index) {
      writer->xorKey += 2;
      writer->buffer[index] ^= static_cast<char>(writer->xorKey + 0x43);
    }
  } else {
    sub_E97C60(&writer->cipher, length, reinterpret_cast<unsigned char*>(writer->buffer), reinterpret_cast<unsigned char*>(writer->buffer));
  }

  DWORD written;
  WriteFile(writer->file, writer->buffer, length, &written, nullptr);
  return true;
}

// INLINED
static void DecodeText(char* text) {
  const char seed = text[1];
  text[2] ^= static_cast<char>(seed * 3 + 0x65);
  text[3] ^= static_cast<char>(seed * 9 + 0x68);
  const int length = (static_cast<unsigned char>(text[2]) << 8) | static_cast<unsigned char>(text[3]);

  char key = static_cast<char>(seed * 9 + 4);
  for (int index = 0; index < length; ++index) {
    key *= 3;
    text[index] = static_cast<char>((key + 0x65) ^ text[index + 4]);
    ++key;
  }
  text[length] = '\0';
}

// OFFSET: 0x00E939D0
// FUNCTION: fn_25555
char* sub_E939D0(char* text) {
  ScopedTimerLock guard(sub_E94520());

  if (text != nullptr && *text == 1) {
    DecodeText(text);

    unsigned int slot = 0;
    const DWORD threadId = GetCurrentThreadId();
    for (unsigned int index = 1; index < 20; ++index) {
      if (g_logTextThreadIds[index] == threadId) {
        slot = index;
        break;
      }
    }
    if (slot == 0 && g_logTextSlotCount < 19) {
      slot = ++g_logTextSlotCount;
      g_logTextThreadIds[slot] = threadId;
    }

    char* previous = g_logTextSlots[slot];
    g_logTextSlots[slot] = text;
    if (previous != nullptr && previous != text && *text != 1 && *previous != 1) {
      char plain[2048];
      lstrcpynA(plain, previous, sizeof(plain));
      const int length = lstrlenA(plain);
      if (length != 0) {
        const unsigned char seed = static_cast<unsigned char>(reinterpret_cast<std::uintptr_t>(plain));
        previous[0] = 1;
        previous[1] = static_cast<char>(seed);
        const unsigned char lengthKey = static_cast<unsigned char>(seed * 3 + 0x65);
        const unsigned char lowKey = static_cast<unsigned char>(seed * 9 + 0x68);

        int out = 4;
        int key = seed * 9 + 4;
        for (int index = 0; index < length; ++index) {
          key *= 3;
          previous[out] = static_cast<char>((key + 0x65) ^ plain[index]);
          ++key;
          ++out;
        }
        previous[out] = '\0';
        previous[2] = static_cast<char>((out - 4) >> 8 ^ lengthKey);
        previous[3] = static_cast<char>((out - 4) ^ lowKey);
      }
    }
  }
  return text;
}

// OFFSET: 0x00E93D70
// FUNCTION: fn_25557
char* sub_E93D70(char* text) {
  ScopedTimerLock guard(sub_E94520());
  if (text != nullptr && *text == 1) {
    DecodeText(text);
  }
  return text;
}

// OFFSET: 0x00E94520
// FUNCTION: fn_25561
TimerLock* sub_E94520() {
  // GLOBAL: gl_26235
  static TimerLock s_textLock;
  return &s_textLock;
}

// INLINED
static unsigned int Rc4Next(Rc4State* key, unsigned int& x, unsigned int& y) {
  x = (x + 1) & 0xFF;
  const unsigned int tx = key->data[x];
  y = (tx + y) & 0xFF;
  const unsigned int ty = key->data[y];
  key->data[x] = ty;
  key->data[y] = tx;
  return key->data[(tx + ty) & 0xFF];
}

// OFFSET: 0x00E97C60
// FUNCTION: fn_25600
void sub_E97C60(Rc4State* key, unsigned int length, const unsigned char* input, unsigned char* output) {
  unsigned int x = key->x;
  unsigned int y = key->y;

  for (unsigned int blocks = length >> 3; blocks != 0; --blocks) {
    for (int index = 0; index < 8; ++index) {
      output[index] = static_cast<unsigned char>(Rc4Next(key, x, y) ^ input[index]);
    }
    input += 8;
    output += 8;
  }

  for (unsigned int remaining = length & 7; remaining != 0; --remaining) {
    *output++ = static_cast<unsigned char>(Rc4Next(key, x, y) ^ *input++);
  }

  key->y = y;
  key->x = x;
}
