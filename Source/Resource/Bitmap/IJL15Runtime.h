#ifndef RAGNAROK_SOURCE_RESOURCE_BITMAP_IJL15RUNTIME_H
#define RAGNAROK_SOURCE_RESOURCE_BITMAP_IJL15RUNTIME_H

#include <cstddef>

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
  unsigned char privateProperties[19988];
};

extern "C" {
int __stdcall ijlInit(IJL15Properties* properties);
int __stdcall ijlRead(IJL15Properties* properties, int operation);
int __stdcall ijlWrite(IJL15Properties* properties, int operation);
int __stdcall ijlFree(IJL15Properties* properties);
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
