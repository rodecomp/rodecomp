#ifndef RAGNAROK_SOURCE_CONTENTS_SOUND_SOUNDMGR_H
#define RAGNAROK_SOURCE_CONTENTS_SOUND_SOUNDMGR_H

#include "Resource/Audio/MilesRuntime.h"

#include <cstddef>
#include <cstdint>
#include <string>
#include <windows.h>

class ResourceMgr;
struct ServiceOptions;
struct SoundConfiguration;

struct SoundMgrContext {
  SoundConfiguration& configuration;
  const ServiceOptions& serviceOptions;
  ResourceMgr& resources;
};

class CSoundMgr {
public:
  explicit CSoundMgr(SoundMgrContext& context);
  virtual ~CSoundMgr();

  static bool Create(SoundMgrContext& context);
  static void Destroy();

  void FadeIn();
  void FadeOut();
  void StopStreams();
  bool Initialize();
  void Update();
  void Shutdown();
  void EndSamples();

  static int Volume(const CSoundMgr* sound);
  static void Pause(CSoundMgr* sound, int paused);
  static void SetVolume(CSoundMgr* sound, int volume);

  bool IsComplete() const;
  bool IsStopped() const;
  void Unused(int, int);
  bool PlayData3D(const void* data, const float& x, const float& y, const float& z, int maximumDistance, int minimumDistance, float scale);
  void SetDefaultName(std::string name);
  bool PlayData2D(const void* data, const float& x, const float& y, const float& z, int maximumDistance, int minimumDistance, float scale);
  void PlayStream(const char* name, int loopCount, bool fade);
  void PlayWave(const char* name, float x, float y, float z, int maximumDistance, int minimumDistance, float scale, int unused);
  void RestartSecondary();
  void SetSampleVolume(int volume);
  void Set3DVolume(int volume);
  void SetTargetVolume(int volume);
  void SetListenerYaw(float yaw);
  void SetRoom(int room);

  /* 0004 */ HANDLE fadeThread = INVALID_HANDLE_VALUE;
  /* 0008 */ unsigned threadId = 0;
  /* 000C */ std::uint8_t initialized = 0;
  /* 000D */ std::uint8_t unknown_000D[3];
  /* 0010 */ int unknown = 0;
  /* 0014 */ int secondaryEnabled = 0;
  /* 0018 */ void* driver = nullptr;
  /* 001C */ void* stream = nullptr;
  /* 0020 */ void* samples[48];
  /* 00E0 */ int targetVolume = 100;
  /* 00E4 */ int streamVolume = 100;
  /* 00E8 */ int sampleVolume = 100;
  /* 00EC */ int unknown250 = 250;
  /* 00F0 */ int unknown40 = 40;
  /* 00F4 */ int rate = 0;
  /* 00F8 */ int bits = 0;
  /* 00FC */ int channels = 0;
  /* 0100 */ int sampleCount = 0;
  /* 0104 */ std::string currentName;
  /* 011C */ std::string defaultName;
  /* 0134 */ int volume3D = 100;
  /* 0138 */ const char* providerNames[64];
  /* 0238 */ MilesProvider providers[64];
  /* 0338 */ MilesProvider provider = 0;
  /* 033C */ MilesEnumeration enumerator = 0;
  /* 0340 */ void* samples3D[48];
  /* 0400 */ void* listener = nullptr;
  /* 0404 */ int roomSupport = 0;
  /* 0408 */ int effectsEnabled = 0;
  /* 040C */ int sample3DCount = 0;
  /* 0410 */ void* secondaryStream = nullptr;
  /* 0414 */ std::uint8_t fadePending = 0;
  /* 0415 */ std::uint8_t reopenPending = 0;
  /* 0416 */ std::uint8_t unknown_0416[2];
  /* 0418 */ DWORD fadeExit = 1;

private:
  static unsigned __stdcall FadeThread(void* direction);

  SoundMgrContext* m_context;
};

extern CSoundMgr* g_soundMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(std::string) == 0x18);
static_assert(offsetof(CSoundMgr, fadeThread) == 0x004);
static_assert(offsetof(CSoundMgr, threadId) == 0x008);
static_assert(offsetof(CSoundMgr, initialized) == 0x00C);
static_assert(offsetof(CSoundMgr, unknown_000D) == 0x000D);
static_assert(offsetof(CSoundMgr, unknown) == 0x010);
static_assert(offsetof(CSoundMgr, secondaryEnabled) == 0x014);
static_assert(offsetof(CSoundMgr, driver) == 0x018);
static_assert(offsetof(CSoundMgr, stream) == 0x01C);
static_assert(offsetof(CSoundMgr, samples) == 0x020);
static_assert(offsetof(CSoundMgr, targetVolume) == 0x0E0);
static_assert(offsetof(CSoundMgr, streamVolume) == 0x0E4);
static_assert(offsetof(CSoundMgr, sampleVolume) == 0x0E8);
static_assert(offsetof(CSoundMgr, unknown250) == 0x0EC);
static_assert(offsetof(CSoundMgr, unknown40) == 0x0F0);
static_assert(offsetof(CSoundMgr, rate) == 0x0F4);
static_assert(offsetof(CSoundMgr, bits) == 0x0F8);
static_assert(offsetof(CSoundMgr, channels) == 0x0FC);
static_assert(offsetof(CSoundMgr, sampleCount) == 0x100);
static_assert(offsetof(CSoundMgr, currentName) == 0x104);
static_assert(offsetof(CSoundMgr, defaultName) == 0x11C);
static_assert(offsetof(CSoundMgr, volume3D) == 0x134);
static_assert(offsetof(CSoundMgr, providerNames) == 0x138);
static_assert(offsetof(CSoundMgr, providers) == 0x238);
static_assert(offsetof(CSoundMgr, provider) == 0x338);
static_assert(offsetof(CSoundMgr, enumerator) == 0x33C);
static_assert(offsetof(CSoundMgr, samples3D) == 0x340);
static_assert(offsetof(CSoundMgr, listener) == 0x400);
static_assert(offsetof(CSoundMgr, roomSupport) == 0x404);
static_assert(offsetof(CSoundMgr, effectsEnabled) == 0x408);
static_assert(offsetof(CSoundMgr, sample3DCount) == 0x40C);
static_assert(offsetof(CSoundMgr, secondaryStream) == 0x410);
static_assert(offsetof(CSoundMgr, fadePending) == 0x414);
static_assert(offsetof(CSoundMgr, reopenPending) == 0x415);
static_assert(offsetof(CSoundMgr, unknown_0416) == 0x0416);
static_assert(offsetof(CSoundMgr, fadeExit) == 0x418);
static_assert(sizeof(CSoundMgr) == 0x420);
#endif

#endif
