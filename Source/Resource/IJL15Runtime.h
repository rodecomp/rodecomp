#ifndef RAGNAROK_SOURCE_RESOURCE_IJL15RUNTIME_H
#define RAGNAROK_SOURCE_RESOURCE_IJL15RUNTIME_H

#include <cstddef>
#include <windows.h>

// Public IJL1.5 core-property ABI. Only the documented core fields used by
// Decoder-private JPEG_PROPERTIES stays opaque.
// Original zeroes0x4E68 bytes before ijlInit. Supplied runtime is x86 only.
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

// Host-side DLL binding, not an original application class. Original IJL calls
// are imports; provenance belongs to their reconstructed application callers.
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

#if defined(_WIN32) && !defined(_WIN64)
static_assert(offsetof(IJL15Properties, jpegBytes) == 36);
static_assert(offsetof(IJL15Properties, jpegWidth) == 44);
static_assert(sizeof(IJL15Properties) == 20072);
#endif

#endif
