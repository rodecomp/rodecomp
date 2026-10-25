#ifndef RAGNAROK_SOURCE_RESOURCE_AUDIO_MILESRUNTIME_H
#define RAGNAROK_SOURCE_RESOURCE_AUDIO_MILESRUNTIME_H

#include <cstdint>
#include <system_error>
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

inline MilesRuntime& MilesRuntime::Get() {
  static MilesRuntime instance;
  return instance;
}

inline MilesRuntime::MilesRuntime() : m_module(LoadLibraryA("Mss32.dll")) {
  if (!this->m_module) {
    throw std::system_error(GetLastError(), std::system_category(), "Mss32.dll");
  }

  this->startup = reinterpret_cast<decltype(this->startup)>(GetProcAddress(this->m_module, "_AIL_startup@0"));
  this->shutdown = reinterpret_cast<decltype(this->shutdown)>(GetProcAddress(this->m_module, "_AIL_shutdown@0"));
  this->setRedistDirectory = reinterpret_cast<decltype(this->setRedistDirectory)>(GetProcAddress(this->m_module, "_AIL_set_redist_directory@4"));
  this->setPreference = reinterpret_cast<decltype(this->setPreference)>(GetProcAddress(this->m_module, "_AIL_set_preference@8"));
  this->openDigitalDriver = reinterpret_cast<decltype(this->openDigitalDriver)>(GetProcAddress(this->m_module, "_AIL_open_digital_driver@16"));
  this->closeDigitalDriver = reinterpret_cast<decltype(this->closeDigitalDriver)>(GetProcAddress(this->m_module, "_AIL_close_digital_driver@4"));
  this->allocateSample = reinterpret_cast<decltype(this->allocateSample)>(GetProcAddress(this->m_module, "_AIL_allocate_sample_handle@4"));
  this->releaseSample = reinterpret_cast<decltype(this->releaseSample)>(GetProcAddress(this->m_module, "_AIL_release_sample_handle@4"));
  this->initSample = reinterpret_cast<decltype(this->initSample)>(GetProcAddress(this->m_module, "_AIL_init_sample@4"));
  this->setSampleFile = reinterpret_cast<decltype(this->setSampleFile)>(GetProcAddress(this->m_module, "_AIL_set_sample_file@12"));
  this->sampleStatus = reinterpret_cast<decltype(this->sampleStatus)>(GetProcAddress(this->m_module, "_AIL_sample_status@4"));
  this->setSampleVolume = reinterpret_cast<decltype(this->setSampleVolume)>(GetProcAddress(this->m_module, "_AIL_set_sample_volume@8"));
  this->startSample = reinterpret_cast<decltype(this->startSample)>(GetProcAddress(this->m_module, "_AIL_start_sample@4"));
  this->endSample = reinterpret_cast<decltype(this->endSample)>(GetProcAddress(this->m_module, "_AIL_end_sample@4"));

  this->openStream = reinterpret_cast<decltype(this->openStream)>(GetProcAddress(this->m_module, "_AIL_open_stream@12"));
  this->closeStream = reinterpret_cast<decltype(this->closeStream)>(GetProcAddress(this->m_module, "_AIL_close_stream@4"));
  this->streamStatus = reinterpret_cast<decltype(this->streamStatus)>(GetProcAddress(this->m_module, "_AIL_stream_status@4"));
  this->setStreamLoopCount = reinterpret_cast<decltype(this->setStreamLoopCount)>(GetProcAddress(this->m_module, "_AIL_set_stream_loop_count@8"));
  this->streamVolume = reinterpret_cast<decltype(this->streamVolume)>(GetProcAddress(this->m_module, "_AIL_stream_volume@4"));
  this->setStreamVolume = reinterpret_cast<decltype(this->setStreamVolume)>(GetProcAddress(this->m_module, "_AIL_set_stream_volume@8"));
  this->pauseStream = reinterpret_cast<decltype(this->pauseStream)>(GetProcAddress(this->m_module, "_AIL_pause_stream@8"));
  this->startStream = reinterpret_cast<decltype(this->startStream)>(GetProcAddress(this->m_module, "_AIL_start_stream@4"));

  this->enumerate3DProviders = reinterpret_cast<decltype(this->enumerate3DProviders)>(GetProcAddress(this->m_module, "_AIL_enumerate_3D_providers@12"));
  this->open3DProvider = reinterpret_cast<decltype(this->open3DProvider)>(GetProcAddress(this->m_module, "_AIL_open_3D_provider@4"));
  this->close3DProvider = reinterpret_cast<decltype(this->close3DProvider)>(GetProcAddress(this->m_module, "_AIL_close_3D_provider@4"));
  this->speakerType = reinterpret_cast<decltype(this->speakerType)>(GetProcAddress(this->m_module, "_AIL_3D_speaker_type@4"));
  this->setSpeakerType = reinterpret_cast<decltype(this->setSpeakerType)>(GetProcAddress(this->m_module, "_AIL_set_3D_speaker_type@8"));
  this->open3DListener = reinterpret_cast<decltype(this->open3DListener)>(GetProcAddress(this->m_module, "_AIL_open_3D_listener@4"));
  this->close3DListener = reinterpret_cast<decltype(this->close3DListener)>(GetProcAddress(this->m_module, "_AIL_close_3D_listener@4"));
  this->allocate3DSample = reinterpret_cast<decltype(this->allocate3DSample)>(GetProcAddress(this->m_module, "_AIL_allocate_3D_sample_handle@4"));
  this->release3DSample = reinterpret_cast<decltype(this->release3DSample)>(GetProcAddress(this->m_module, "_AIL_release_3D_sample_handle@4"));
  this->set3DSampleFile = reinterpret_cast<decltype(this->set3DSampleFile)>(GetProcAddress(this->m_module, "_AIL_set_3D_sample_file@8"));
  this->sample3DStatus = reinterpret_cast<decltype(this->sample3DStatus)>(GetProcAddress(this->m_module, "_AIL_3D_sample_status@4"));
  this->set3DSampleVolume = reinterpret_cast<decltype(this->set3DSampleVolume)>(GetProcAddress(this->m_module, "_AIL_set_3D_sample_volume@8"));
  this->start3DSample = reinterpret_cast<decltype(this->start3DSample)>(GetProcAddress(this->m_module, "_AIL_start_3D_sample@4"));
  this->end3DSample = reinterpret_cast<decltype(this->end3DSample)>(GetProcAddress(this->m_module, "_AIL_end_3D_sample@4"));
  this->roomType = reinterpret_cast<decltype(this->roomType)>(GetProcAddress(this->m_module, "_AIL_3D_room_type@4"));
  this->setRoomType = reinterpret_cast<decltype(this->setRoomType)>(GetProcAddress(this->m_module, "_AIL_set_3D_room_type@8"));
  this->set3DSampleDistances = reinterpret_cast<decltype(this->set3DSampleDistances)>(GetProcAddress(this->m_module, "_AIL_set_3D_sample_distances@12"));
  this->set3DSampleEffectsLevel =
    reinterpret_cast<decltype(this->set3DSampleEffectsLevel)>(GetProcAddress(this->m_module, "_AIL_set_3D_sample_effects_level@8"));
  this->set3DPosition = reinterpret_cast<decltype(this->set3DPosition)>(GetProcAddress(this->m_module, "_AIL_set_3D_position@16"));
  this->set3DOrientation = reinterpret_cast<decltype(this->set3DOrientation)>(GetProcAddress(this->m_module, "_AIL_set_3D_orientation@28"));

  this->fileType = reinterpret_cast<decltype(this->fileType)>(GetProcAddress(this->m_module, "_AIL_file_type@8"));
  this->wavInfo = reinterpret_cast<decltype(this->wavInfo)>(GetProcAddress(this->m_module, "_AIL_WAV_info@8"));
  this->decompressADPCM = reinterpret_cast<decltype(this->decompressADPCM)>(GetProcAddress(this->m_module, "_AIL_decompress_ADPCM@12"));
  this->decompressASI = reinterpret_cast<decltype(this->decompressASI)>(GetProcAddress(this->m_module, "_AIL_decompress_ASI@24"));
  this->freeLocked = reinterpret_cast<decltype(this->freeLocked)>(GetProcAddress(this->m_module, "_AIL_mem_free_lock@4"));

  if (!this->startup || !this->shutdown || !this->setRedistDirectory || !this->setPreference || !this->openDigitalDriver || !this->closeDigitalDriver ||
      !this->allocateSample || !this->releaseSample || !this->initSample || !this->setSampleFile || !this->sampleStatus || !this->setSampleVolume ||
      !this->startSample || !this->endSample || !this->openStream || !this->closeStream || !this->streamStatus || !this->setStreamLoopCount ||
      !this->streamVolume || !this->setStreamVolume || !this->pauseStream || !this->startStream || !this->enumerate3DProviders || !this->open3DProvider ||
      !this->close3DProvider || !this->speakerType || !this->setSpeakerType || !this->open3DListener || !this->close3DListener || !this->allocate3DSample ||
      !this->release3DSample || !this->set3DSampleFile || !this->sample3DStatus || !this->set3DSampleVolume || !this->start3DSample || !this->end3DSample ||
      !this->roomType || !this->setRoomType || !this->set3DSampleDistances || !this->set3DSampleEffectsLevel || !this->set3DPosition ||
      !this->set3DOrientation || !this->fileType || !this->wavInfo || !this->decompressADPCM || !this->decompressASI || !this->freeLocked) {
    FreeLibrary(this->m_module);
    throw std::system_error(ERROR_PROC_NOT_FOUND, std::system_category(), "Mss32 exports");
  }
}

inline MilesRuntime::~MilesRuntime() {
  FreeLibrary(this->m_module);
}

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(MilesSoundInfo) == 36);
#endif

#endif
