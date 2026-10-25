#ifndef RAGNAROK_SOURCE_RESOURCE_BITMAP_IJL15RUNTIME_H
#define RAGNAROK_SOURCE_RESOURCE_BITMAP_IJL15RUNTIME_H

#include <cstddef>
#include <system_error>
#include <windows.h>

struct IJL15Properties {
  int useProperties;
  unsigned char* dibBytes;
  int dibWidth;
  int dibHeight;
  int dibPadding;
  int dibChannels;
  int dibColor;
  int dibSubsampling;
  const char* jpegFile;
  unsigned char* jpegBytes;
  int jpegSize;
  int jpegWidth;
  int jpegHeight;
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

inline IJL15Runtime::~IJL15Runtime() {
  FreeLibrary(this->m_module);
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(IJL15Properties, jpegBytes) == 36);
static_assert(offsetof(IJL15Properties, jpegWidth) == 44);
static_assert(sizeof(IJL15Properties) == 20072);
#endif

#endif
