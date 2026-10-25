#ifndef RAGNAROK_SOURCE_RESOURCE_AUDIO_MILESRUNTIME_H
#define RAGNAROK_SOURCE_RESOURCE_AUDIO_MILESRUNTIME_H

#include <cstdint>
#include <windows.h>

using MilesProvider = std::uint32_t;
using MilesEnumeration = std::uint32_t;

struct MilesSoundInfo {
  int format;
  const void* data;
  std::uint32_t dataLength;
  std::uint32_t rate;
  int bits;
  int channels;
  std::uint32_t samples;
  std::uint32_t blockSize;
  const void* initial;
};

class MilesRuntime {
public:
  static MilesRuntime& Get();
  ~MilesRuntime();
  MilesRuntime(const MilesRuntime&) = delete;
  MilesRuntime& operator=(const MilesRuntime&) = delete;

  int(__stdcall* startup)();
  void(__stdcall* shutdown)();
  const char*(__stdcall* setRedistDirectory)(const char*);
  int(__stdcall* setPreference)(unsigned, int);
  void*(__stdcall* openDigitalDriver)(unsigned, int, int, unsigned);
  void(__stdcall* closeDigitalDriver)(void*);
  void*(__stdcall* allocateSample)(void*);
  void(__stdcall* releaseSample)(void*);
  void(__stdcall* initSample)(void*);
  int(__stdcall* setSampleFile)(void*, const void*, int);
  unsigned(__stdcall* sampleStatus)(void*);
  void(__stdcall* setSampleVolume)(void*, int);
  void(__stdcall* startSample)(void*);
  void(__stdcall* endSample)(void*);

  void*(__stdcall* openStream)(void*, const char*, int);
  void(__stdcall* closeStream)(void*);
  unsigned(__stdcall* streamStatus)(void*);
  void(__stdcall* setStreamLoopCount)(void*, int);
  int(__stdcall* streamVolume)(void*);
  void(__stdcall* setStreamVolume)(void*, int);
  void(__stdcall* pauseStream)(void*, int);
  void(__stdcall* startStream)(void*);

  int(__stdcall* enumerate3DProviders)(MilesEnumeration*, MilesProvider*, const char**);
  int(__stdcall* open3DProvider)(MilesProvider);
  void(__stdcall* close3DProvider)(MilesProvider);
  int(__stdcall* speakerType)(MilesProvider);
  void(__stdcall* setSpeakerType)(MilesProvider, int);
  void*(__stdcall* open3DListener)(MilesProvider);
  void(__stdcall* close3DListener)(void*);
  void*(__stdcall* allocate3DSample)(MilesProvider);
  void(__stdcall* release3DSample)(void*);
  int(__stdcall* set3DSampleFile)(void*, const void*);
  unsigned(__stdcall* sample3DStatus)(void*);
  void(__stdcall* set3DSampleVolume)(void*, int);
  void(__stdcall* start3DSample)(void*);
  void(__stdcall* end3DSample)(void*);
  int(__stdcall* roomType)(MilesProvider);
  void(__stdcall* setRoomType)(MilesProvider, int);
  void(__stdcall* set3DSampleDistances)(void*, float, float);
  void(__stdcall* set3DSampleEffectsLevel)(void*, float);
  void(__stdcall* set3DPosition)(void*, float, float, float);
  void(__stdcall* set3DOrientation)(void*, float, float, float, float, float, float);

  int(__stdcall* fileType)(const void*, unsigned);
  int(__stdcall* wavInfo)(const void*, MilesSoundInfo*);
  int(__stdcall* decompressADPCM)(const MilesSoundInfo*, void**, unsigned*);
  int(__stdcall* decompressASI)(const void*, unsigned, const char*, void**, unsigned*, void*);
  void(__stdcall* freeLocked)(void*);

private:
  MilesRuntime();
  HMODULE m_module;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MilesSoundInfo) == 36);
#endif

#endif
