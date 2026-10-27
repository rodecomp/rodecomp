#ifndef RAGNAROK_SOURCE_RESOURCE_BITMAP_IJL15RUNTIME_H
#define RAGNAROK_SOURCE_RESOURCE_BITMAP_IJL15RUNTIME_H

#include <cstddef>
#include <system_error>
#include <windows.h>

struct IJL15Properties {
  int useProperties;
  /* 0004 */ unsigned char* dibBytes;
  /* 0008 */ int dibWidth;
  /* 000C */ int dibHeight;
  /* 0010 */ int dibPadding;
  /* 0014 */ int dibChannels;
  /* 0018 */ int dibColor;
  int dibSubsampling;
  /* 0020 */ const char* jpegFile;
  /* 0024 */ unsigned char* jpegBytes;
  /* 0028 */ int jpegSize;
  /* 002C */ int jpegWidth;
  /* 0030 */ int jpegHeight;
  int jpegChannels;
  int jpegColor;
  int jpegSubsampling;
  int thumbnailWidth;
  int thumbnailHeight;
  int conversion;
  int upsampling;
  int quality;
  alignas(8) unsigned char privateProperties[19984];
};

class IJL15Runtime {
public:
  IJL15Runtime();
  ~IJL15Runtime();
  int(__stdcall* initialize)(IJL15Properties*);
  int(__stdcall* read)(IJL15Properties*, int operation);
  int(__stdcall* write)(IJL15Properties*, int operation);
  int(__stdcall* release)(IJL15Properties*);

private:
  HMODULE m_module;
  IJL15Runtime(const IJL15Runtime&) = delete;
  IJL15Runtime& operator=(const IJL15Runtime&) = delete;
};

// INLINED
inline IJL15Runtime::IJL15Runtime() : m_module(LoadLibraryA("ijl15.dll")) {
  if (!this->m_module) {
    throw std::system_error(GetLastError(), std::system_category(), "ijl15.dll");
  }

  this->initialize = reinterpret_cast<decltype(this->initialize)>(GetProcAddress(this->m_module, "ijlInit"));
  this->read = reinterpret_cast<decltype(this->read)>(GetProcAddress(this->m_module, "ijlRead"));
  this->write = reinterpret_cast<decltype(this->write)>(GetProcAddress(this->m_module, "ijlWrite"));
  this->release = reinterpret_cast<decltype(this->release)>(GetProcAddress(this->m_module, "ijlFree"));

  if (!this->initialize || !this->read || !this->write || !this->release) {
    FreeLibrary(this->m_module);
    throw std::system_error(ERROR_PROC_NOT_FOUND, std::system_category(), "ijl15 exports");
  }
}

// INLINED
inline IJL15Runtime::~IJL15Runtime() {
  FreeLibrary(this->m_module);
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(IJL15Properties, dibBytes) == 0x0004);
static_assert(offsetof(IJL15Properties, dibWidth) == 0x0008);
static_assert(offsetof(IJL15Properties, dibHeight) == 0x000C);
static_assert(offsetof(IJL15Properties, dibPadding) == 0x0010);
static_assert(offsetof(IJL15Properties, dibChannels) == 0x0014);
static_assert(offsetof(IJL15Properties, dibColor) == 0x0018);
static_assert(offsetof(IJL15Properties, jpegFile) == 0x0020);
static_assert(offsetof(IJL15Properties, jpegSize) == 0x0028);
static_assert(offsetof(IJL15Properties, jpegHeight) == 0x0030);
static_assert(offsetof(IJL15Properties, jpegBytes) == 36);
static_assert(offsetof(IJL15Properties, jpegWidth) == 44);
static_assert(sizeof(IJL15Properties) == 20072);
#endif

#endif
