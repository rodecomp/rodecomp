#include "Security/ThemidaMgr.h"

#include "Archive/GPak.h"

#include <atomic>
#include <new>
#include <random>
#include <windows.h>

// GLOBAL: gl_24891
CThemidaMgr* g_themidaMgr;

// GLOBAL: gl_26182
std::atomic<bool> s_themidaThreadStop;
// GLOBAL: gl_26185
std::atomic<bool> s_themidaThreadStarted;
// GLOBAL: gl_26183
ThemidaKeyEncoderA s_themidaKeyA;
// GLOBAL: gl_26184
ThemidaKeyEncoderB s_themidaThreadHandle;

// OFFSET: 0x0076F150
// FUNCTION: fn_46732
// VIRTUALIZED: emulated 0x0076F16A
static DWORD WINAPI ThemidaWatchThread(void*) {
  s_themidaKeyA.Generate(reinterpret_cast<const unsigned char*>(&ThemidaKeyMagic));
  s_themidaThreadStarted = true;
  s_themidaThreadStarted.notify_one();

  std::mt19937 engine(std::random_device{}());
  std::uniform_int_distribution<int> interval(500, 1500);
  for (;;) {
    Sleep(interval(engine));
    if (s_themidaThreadStop) {
      break;
    }
  }

  return 1;
}

// OFFSET: 0x0076EC00
// FUNCTION: fn_08235
CThemidaMgr::CThemidaMgr() : m_valid(true) {
}

// OFFSET: 0x0076EC10
// FUNCTION: fn_08236
CThemidaMgr::~CThemidaMgr() {
}

// OFFSET: 0x0076EC20
// FUNCTION: fn_08237
unsigned char* ThemidaKeyEncoderA::Generate(const unsigned char* input) {
  PakUniformRandom random(std::random_device{}());
  FillRandomBytes(reinterpret_cast<char*>(this->m_key), reinterpret_cast<char*>(this->m_key) + 5, random);
  for (int index = 0; index < 4; ++index) {
    this->m_key[index + 5] = input[index] ^ this->m_key[index + 1] ^ this->m_key[index];
  }
  return this->m_key;
}

// OFFSET: 0x0076ECC0
// FUNCTION: fn_08238
unsigned char* ThemidaKeyEncoderB::Generate(const unsigned char* input) {
  PakUniformRandom random(std::random_device{}());
  FillRandomBytes(reinterpret_cast<char*>(this->m_key), reinterpret_cast<char*>(this->m_key) + 5, random);
  for (int index = 0; index < 4; ++index) {
    this->m_key[index + 5] = input[index] ^ this->m_key[index + 1] ^ this->m_key[index];
  }
  return this->m_key;
}

// OFFSET: 0x0076EDE0
// FUNCTION: fn_08240
bool CThemidaMgr::Create() {
  if (g_themidaMgr == nullptr) {
    g_themidaMgr = new (std::nothrow) CThemidaMgr();
    if (g_themidaMgr != nullptr) {
      if (!g_themidaMgr->m_valid) {
        delete g_themidaMgr;
        g_themidaMgr = nullptr;
        return false;
      }
      return true;
    }
  }
  return false;
}

// OFFSET: 0x0076EE90
// FUNCTION: fn_08241
void CThemidaMgr::Destroy() {
  if (g_themidaMgr != nullptr) {
    delete g_themidaMgr;
    g_themidaMgr = nullptr;
  }
}

// OFFSET: 0x0076EEB0
// FUNCTION: fn_08242
void CThemidaMgr::Check() {
  unsigned int value = 0;
  s_themidaKeyA.Generate(reinterpret_cast<const unsigned char*>(&value));

  value = reinterpret_cast<unsigned int>(CreateThread(nullptr, 0, &ThemidaWatchThread, nullptr, 0, nullptr));
  s_themidaThreadHandle.Generate(reinterpret_cast<const unsigned char*>(&value));
  s_themidaThreadStarted.wait(false);
  s_themidaThreadStarted = false;
}

// OFFSET: 0x0076F050
// FUNCTION: fn_08243
void CThemidaMgr::StopThread() {
  s_themidaThreadStop.exchange(true);
  unsigned int handle = s_themidaThreadHandle.Decode();
  if (handle != 0) {
    WaitForSingleObject(reinterpret_cast<HANDLE>(handle), INFINITE);
    CloseHandle(reinterpret_cast<HANDLE>(s_themidaThreadHandle.Decode()));
    unsigned int zero = 0;
    s_themidaThreadHandle.Generate(reinterpret_cast<const unsigned char*>(&zero));
  }

  unsigned int zero = 0;
  s_themidaKeyA.Generate(reinterpret_cast<const unsigned char*>(&zero));
}
