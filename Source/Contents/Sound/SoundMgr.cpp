#include "Contents/Configuration/ServiceOptions.h"
#include "UI/GameSettings/TalkType.h"
#include "Contents/Sound/SoundConfiguration.h"
#include "Contents/Sound/SoundMgr.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Audio/WaveResource.h"
#include "Utility/Math/AffineMatrix.h"

#include <cstdlib>
#include <cstring>
#include <new>
#include <process.h>

extern "C" __declspec(dllimport) unsigned int __stdcall AIL_stream_status(void* stream);
extern "C" __declspec(dllimport) int __stdcall AIL_3D_room_type(void* provider);
extern "C" __declspec(dllimport) void __stdcall AIL_end_sample(void* sample);
extern "C" __declspec(dllimport) void __stdcall AIL_end_3D_sample(void* sample);
extern "C" __declspec(dllimport) void __stdcall AIL_close_stream(void* stream);
extern "C" __declspec(dllimport) void* __stdcall AIL_open_stream(void* driver, const char* name, int flags);
extern "C" __declspec(dllimport) void __stdcall AIL_set_stream_volume(void* stream, int volume);
extern "C" __declspec(dllimport) void __stdcall AIL_set_stream_loop_count(void* stream, int count);
extern "C" __declspec(dllimport) void __stdcall AIL_start_stream(void* stream);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_room_type(void* provider, int roomType);
extern "C" __declspec(dllimport) unsigned int __stdcall AIL_3D_sample_status(void* sample);
extern "C" __declspec(dllimport) int __stdcall AIL_set_3D_sample_file(void* sample, const void* data);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_sample_effects_level(void* sample, float level);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_position(void* sample, float x, float y, float z);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_orientation(void* sample, float x, float y, float z, float upX, float upY, float upZ);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_sample_volume(void* sample, int volume);
extern "C" __declspec(dllimport) void __stdcall AIL_set_3D_sample_distances(void* sample, float maximum, float minimum);
extern "C" __declspec(dllimport) void __stdcall AIL_start_3D_sample(void* sample);
extern "C" __declspec(dllimport) void __stdcall AIL_release_3D_sample_handle(void* sample);
extern "C" __declspec(dllimport) void __stdcall AIL_close_3D_listener(void* listener);
extern "C" __declspec(dllimport) void __stdcall AIL_close_3D_provider(MilesProvider provider);
extern "C" __declspec(dllimport) void __stdcall AIL_release_sample_handle(void* sample);
extern "C" __declspec(dllimport) void __stdcall AIL_close_digital_driver(void* driver);
extern "C" __declspec(dllimport) void __stdcall AIL_shutdown();
extern "C" __declspec(dllimport) void __stdcall AIL_pause_stream(void* stream, int paused);
extern "C" __declspec(dllimport) int __stdcall AIL_stream_volume(void* stream);

// STATIC: 0x0138BE7C
// GLOBAL: gl_24457
CSoundMgr* g_soundMgr = nullptr;

namespace {
  // OFFSET: 0x00401090
  // FUNCTION: fn_45443
  void ShutdownMilesAtExit() {
    MilesRuntime::Get().shutdown();
  }
}

// OFFSET: 0x00638A90
// FUNCTION: fn_45421
CSoundMgr::CSoundMgr() {
  if (this->Initialize()) {
    this->initialized = 1;
  }
}

// OFFSET: 0x00638C40
// FUNCTION: fn_45422
CSoundMgr::~CSoundMgr() {
  this->Shutdown();
}

// OFFSET: 0x00638E80
// FUNCTION: fn_04747
void CSoundMgr::FadeIn() {
  if (this->threadId != 0) {
    PostThreadMessageA(this->threadId, WM_USER, 0, 0);
    this->threadId = 0;
  }

  if (this->fadeThread != INVALID_HANDLE_VALUE) {
    WaitForSingleObject(this->fadeThread, 30);
    CloseHandle(this->fadeThread);
    this->fadeThread = INVALID_HANDLE_VALUE;
  }

  this->fadeThread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, &CSoundMgr::FadeThread, reinterpret_cast<void*>(1), 0, &this->threadId));
}

// OFFSET: 0x00638EF0
// FUNCTION: fn_04748
void CSoundMgr::FadeOut() {
  if (this->threadId != 0) {
    PostThreadMessageA(this->threadId, WM_USER, 0, 0);
    this->threadId = 0;
  }

  if (this->fadeThread != INVALID_HANDLE_VALUE) {
    WaitForSingleObject(this->fadeThread, 30);
    CloseHandle(this->fadeThread);
    this->fadeThread = INVALID_HANDLE_VALUE;
  }

  this->fadeThread = reinterpret_cast<HANDLE>(_beginthreadex(nullptr, 0, &CSoundMgr::FadeThread, nullptr, 0, &this->threadId));
}

// OFFSET: 0x00638F60
// FUNCTION: fn_04749
void CSoundMgr::StopStreams() {
  this->currentName = std::string();
  MilesRuntime& miles = MilesRuntime::Get();

  if (this->stream) {
    miles.closeStream(this->stream);
    this->stream = nullptr;
  }

  if (this->secondaryStream) {
    miles.closeStream(this->secondaryStream);
    this->secondaryStream = nullptr;
  }

  this->secondaryEnabled = 0;
}

// OFFSET: 0x00639070
// FUNCTION: fn_04750
bool CSoundMgr::Initialize() {
  for (int i = 0; i < 48; ++i) {
    this->samples[i] = nullptr;
  }

  for (int i = 0; i < 48; ++i) {
    this->samples3D[i] = nullptr;
  }

  for (int i = 0; i < 64; ++i) {
    this->providerNames[i] = nullptr;
    this->providers[i] = 0;
  }

  SoundConfiguration& configuration = g_soundConfiguration;

  if (configuration.mode == 0) {
    return true;
  }

  switch (configuration.numSampleType) {
  case 0:
    this->sampleCount = 48;
    this->sample3DCount = 48;
    break;
  case 1:
    this->sampleCount = 32;
    this->sample3DCount = 32;
    break;
  default:
    this->sampleCount = 16;
    this->sample3DCount = 16;
    break;
  }

  MilesRuntime& miles = MilesRuntime::Get();
  miles.setRedistDirectory(".\\");

  std::atexit(&ShutdownMilesAtExit);
  miles.startup();

  this->channels = configuration.mode == 1 ? 1 : 2;

  switch (configuration.digitalRateType) {
  case 0:
    this->rate = 44100;
    break;
  case 1:
    this->rate = 11025;
    break;
  case 2:
    this->rate = 8000;
    break;
  default:
    this->rate = 8000;
    break;
  }

  switch (configuration.digitalBitsType) {
  case 0:
    this->bits = 16;
    break;
  case 1:
    this->bits = 8;
    break;
  default:
    this->bits = 8;
    break;
  }

  miles.setPreference(15, configuration.preference15);
  this->driver = miles.openDigitalDriver(this->rate, this->bits, this->channels, 1);

  if (!this->driver) {
    return false;
  }

  this->StopStreams();

  if (configuration.mode == 1) {
    for (int i = 0; i < this->sampleCount; ++i) {
      this->samples[i] = miles.allocateSample(this->driver);

      if (!this->samples[i]) {
        break;
      }
    }

    return true;
  }

  if (configuration.mode != 2) {
    return true;
  }

  strcpy_s(configuration.providerName, sizeof(configuration.providerName), "Miles Fast 2D Positional Audio");

  int index = 0;

  while (miles.enumerate3DProviders(&this->enumerator, &this->providers[index], &this->providerNames[index])) {
    if (std::strcmp(this->providerNames[index], configuration.providerName) == 0) {
      this->provider = this->providers[index];
      break;
    }

    ++index;
  }

  if (!this->provider) {
    return true;
  }

  if (miles.open3DProvider(this->provider) != 0) {
    return false;
  }

  if (miles.speakerType(this->provider) != -1) {
    miles.setSpeakerType(this->provider, configuration.speakerType);
  }

  this->listener = miles.open3DListener(this->provider);

  if (!this->listener) {
    return false;
  }

  for (int i = 0; i < this->sample3DCount; ++i) {
    this->samples3D[i] = miles.allocate3DSample(this->provider);

    if (!this->samples3D[i]) {
      break;
    }
  }

  return true;
}

// OFFSET: 0x00639350
// FUNCTION: fn_04751
bool CSoundMgr::Create() {
  if (g_soundMgr) {
    return false;
  }

  g_soundMgr = new (std::nothrow) CSoundMgr();

  if (!g_soundMgr) {
    return false;
  }

  if (!g_soundMgr->initialized) {
    delete g_soundMgr;
    g_soundMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x00639570
// FUNCTION: fn_04752
void CSoundMgr::Update() {
  if (this->fadeThread != INVALID_HANDLE_VALUE) {
    GetExitCodeThread(this->fadeThread, &this->fadeExit);
  }

  if (!this->fadePending || this->fadeExit != 0) {
    return;
  }

  if (this->stream) {
    AIL_pause_stream(this->stream, 1);
    AIL_stream_volume(this->stream);
  }

  this->FadeIn();
  this->RestartSecondary();
  this->fadePending = 0;
}

// OFFSET: 0x006396B0
// FUNCTION: fn_45423
void CSoundMgr::Shutdown() {
  if (g_soundConfiguration.mode == 0) {
    return;
  }

  MilesRuntime& miles = MilesRuntime::Get();

  if (this->provider) {
    for (void*& sample : this->samples3D) {
      if (sample) {
        miles.release3DSample(sample);
        sample = nullptr;
      }
    }

    miles.close3DListener(this->listener);
    miles.close3DProvider(this->provider);
    this->provider = 0;
  }

  this->StopStreams();

  for (void*& sample : this->samples) {
    if (sample) {
      miles.releaseSample(sample);
      sample = nullptr;
    }
  }

  if (this->driver) {
    miles.closeDigitalDriver(this->driver);
    this->driver = nullptr;
  }

  miles.shutdown();
}

// OFFSET: 0x00639880
// FUNCTION: fn_04753
void CSoundMgr::Destroy() {
  if (g_soundMgr) {
    delete g_soundMgr;
    g_soundMgr = nullptr;
  }
}

// OFFSET: 0x006398A0
// FUNCTION: fn_04754
void CSoundMgr::EndSamples() {
  for (int i = 0; i < 48; ++i) {
    if (this->samples[i]) {
      AIL_end_sample(this->samples[i]);
    }
  }

  if (this->provider) {
    for (int i = 0; i < 48; ++i) {
      if (this->samples3D[i]) {
        AIL_end_3D_sample(this->samples3D[i]);
      }
    }
  }
}

// OFFSET: 0x00639910
// FUNCTION: fn_04755
unsigned __stdcall CSoundMgr::FadeThread(void* direction) {
  MSG message;

  if (direction == reinterpret_cast<void*>(1)) {
    for (int step = 1; step <= 1000; ++step) {
      if (PeekMessageA(&message, nullptr, WM_USER, WM_USER, PM_NOREMOVE)) {
        break;
      }

      CSoundMgr* sound = g_soundMgr;
      const int volume = CSoundMgr::Volume(sound);

      if (sound->targetVolume <= volume) {
        break;
      }

      const int next = sound->streamVolume + 1;
      CSoundMgr::SetVolume(sound, next);
      Sleep(10);
    }
  } else {
    for (int step = 1; step <= 1000; ++step) {
      if (PeekMessageA(&message, nullptr, WM_USER, WM_USER, PM_NOREMOVE)) {
        break;
      }

      CSoundMgr* sound = g_soundMgr;

      if (!sound || sound->streamVolume <= 0) {
        break;
      }

      const int next = sound->streamVolume - 1;
      CSoundMgr::SetVolume(sound, next);
      Sleep(10);
    }
  }

  return 0;
}

// OFFSET: 0x00639A60
// FUNCTION: fn_04756
int CSoundMgr::Volume(const CSoundMgr* sound) {
  return sound ? sound->streamVolume : 0;
}

// OFFSET: 0x00639A70
// FUNCTION: fn_04757
bool CSoundMgr::IsComplete() const {
  if (g_soundConfiguration.mode == 0 || !this->stream || !this->secondaryStream || this->fadeExit != 0) {
    return false;
  }

  if (AIL_stream_status(this->secondaryStream) == 2) {
    return true;
  }

  if (AIL_stream_status(this->stream) == 2) {
    return true;
  }

  return false;
}

// OFFSET: 0x00639AC0
// FUNCTION: fn_04758
bool CSoundMgr::IsStopped() const {
  return this->stream && AIL_stream_status(this->stream) == 8;
}

// OFFSET: 0x00639AE0
// FUNCTION: fn_04759
void CSoundMgr::Unused(int, int) {
}

// OFFSET: 0x00639AF0
// FUNCTION: fn_04760
void CSoundMgr::Pause(CSoundMgr* sound, int paused) {
  if (!sound || !sound->stream) {
    return;
  }

  MilesRuntime& miles = MilesRuntime::Get();
  miles.pauseStream(sound->stream, paused);
  miles.streamVolume(sound->stream);

  if (paused == 0) {
    sound->currentName = sound->defaultName;
  }
}

// OFFSET: 0x00639B50
// FUNCTION: fn_45424
bool CSoundMgr::PlayData3D(const void* data, const float& x, const float& y, const float& z, int maximumDistance, int minimumDistance, float scale) {
  const float scaledVolume = static_cast<float>(this->volume3D) * scale;
  const int volume = static_cast<int>(scaledVolume);

  if (!this->provider) {
    return this->PlayData2D(data, x, y, z, 250, 40, 1.0f);
  }

  MilesRuntime& miles = MilesRuntime::Get();

  for (int i = 0; i < this->sample3DCount; ++i) {
    if (!this->samples3D[i]) {
      continue;
    }

    if (miles.sample3DStatus(this->samples3D[i]) != 2) {
      continue;
    }

    if (!miles.set3DSampleFile(this->samples3D[i], data)) {
      return false;
    }

    miles.set3DSampleEffectsLevel(this->samples3D[i], this->effectsEnabled ? 1.0f : 0.0f);
    miles.set3DPosition(this->samples3D[i], x, y, z);
    miles.set3DOrientation(this->samples3D[i], -x, -y, -z, 0.0f, 1.0f, 0.0f);
    miles.set3DSampleVolume(this->samples3D[i], volume);
    miles.set3DSampleDistances(this->samples3D[i], static_cast<float>(maximumDistance), static_cast<float>(minimumDistance));
    miles.start3DSample(this->samples3D[i]);

    return true;
  }

  return false;
}

// OFFSET: 0x00639D30
// FUNCTION: fn_04761
void CSoundMgr::SetDefaultName(std::string name) {
  this->defaultName = name;

  if (g_serviceOptions.Get(TT_MUSIC_ON_OFF)) {
    this->PlayStream(nullptr, 0, false);
  }

  this->SetRoom(0);
}

// OFFSET: 0x00639DF0
// FUNCTION: fn_04762
bool CSoundMgr::PlayData2D(const void* data, const float& x, const float& y, const float& z, int maximumDistance, int minimumDistance, float scale) {
  const float zSquared = z * z;
  const float xSquared = x * x;
  const float horizontalSquared = xSquared + zSquared;
  const float horizontal = SquareRoot(horizontalSquared);
  const float horizontalResquared = horizontal * horizontal;
  const float ySquared = y * y;
  const float distanceSquared = ySquared + horizontalResquared;
  const float distance = SquareRoot(distanceSquared);

  if (distance >= static_cast<float>(maximumDistance)) {
    return false;
  }

  int volume = this->sampleVolume;
  const float minimum = static_cast<float>(minimumDistance);

  if (!(minimum >= distance)) {
    const float numerator = static_cast<float>(volume) * minimum;
    const float attenuatedVolume = numerator / distance;
    volume = static_cast<int>(attenuatedVolume);
  }

  const float scaledVolume = static_cast<float>(volume) * scale;
  volume = static_cast<int>(scaledVolume);

  MilesRuntime& miles = MilesRuntime::Get();

  for (int i = 0; i < this->sampleCount; ++i) {
    if (!this->samples[i]) {
      continue;
    }

    if (miles.sampleStatus(this->samples[i]) != 2) {
      continue;
    }

    miles.initSample(this->samples[i]);
    miles.setSampleFile(this->samples[i], data, 0);
    miles.setSampleVolume(this->samples[i], volume);
    miles.startSample(this->samples[i]);

    return true;
  }

  return false;
}

// OFFSET: 0x00639F00
// FUNCTION: fn_04763
void CSoundMgr::PlayStream(const char* name, int loopCount, bool fade) {
  if (g_soundConfiguration.mode == 0) {
    return;
  }

  if (!name) {
    name = this->defaultName.c_str();
  }

  if (this->currentName == name) {
    return;
  }

  if (fade) {
    this->FadeOut();
    this->fadePending = 1;
    this->currentName.assign(name);

    if (this->secondaryStream) {
      this->reopenPending = 1;
      return;
    }

    this->secondaryStream = AIL_open_stream(this->driver, this->currentName.c_str(), 0);
    const std::string copiedName(name);
    return;
  }

  this->StopStreams();
  this->currentName.assign(name);
  this->stream = AIL_open_stream(this->driver, this->currentName.c_str(), 0);

  {
    const std::string copiedName(name);
  }

  AIL_set_stream_volume(this->stream, this->streamVolume);
  AIL_set_stream_loop_count(this->stream, loopCount);
  AIL_start_stream(this->stream);
}

// OFFSET: 0x0063A130
// FUNCTION: fn_04764
void CSoundMgr::PlayWave(const char* name, float x, float y, float z, int maximumDistance, int minimumDistance, float scale, int unused) {
  const unsigned char enabled = g_serviceOptions.Get(TT_EFFECT_SOUND_ON_OFF);

  if (g_soundConfiguration.mode == 0 || !enabled) {
    return;
  }

  static constexpr char permittedName[] = "버튼소리.wav";

  if (this->tingOnly != 0 && std::strcmp(name, permittedName) != 0) {
    return;
  }

  CWave* wave = static_cast<CWave*>(ResourceMgr::Instance().Load(name));

  if (!wave) {
    return;
  }

  if (g_soundConfiguration.mode == 1) {
    this->PlayData2D(wave->data, x, y, z, maximumDistance, minimumDistance, scale);
  } else if (g_soundConfiguration.mode == 2) {
    this->PlayData3D(wave->data, x, y, z, maximumDistance, minimumDistance, scale);
  }
}

// OFFSET: 0x0063A3B0
// FUNCTION: fn_45425
void CSoundMgr::RestartSecondary() {
  if (!this->secondaryEnabled) {
    return;
  }

  if (this->reopenPending && this->secondaryStream) {
    AIL_close_stream(this->secondaryStream);
    this->secondaryStream = nullptr;
    this->secondaryStream = AIL_open_stream(this->driver, this->currentName.c_str(), 0);
    this->reopenPending = 0;
  }

  if (this->secondaryStream) {
    AIL_set_stream_volume(this->secondaryStream, this->streamVolume);
    AIL_set_stream_loop_count(this->secondaryStream, 1);
    AIL_start_stream(this->secondaryStream);
  }
}

// OFFSET: 0x0063A440
// FUNCTION: fn_04765
void CSoundMgr::SetSampleVolume(int volume) {
  this->sampleVolume = volume > 127 ? 127 : (volume < 0 ? 0 : volume);
}

// OFFSET: 0x0063A470
// FUNCTION: fn_04766
void CSoundMgr::Set3DVolume(int volume) {
  this->volume3D = volume > 127 ? 127 : (volume < 0 ? 0 : volume);
}

// OFFSET: 0x0063A4A0
// FUNCTION: fn_04767
void CSoundMgr::SetTargetVolume(int volume) {
  this->targetVolume = volume > 127 ? 127 : (volume < 0 ? 0 : volume);
}

// OFFSET: 0x0063A4D0
// FUNCTION: fn_04768
void CSoundMgr::SetListenerYaw(float yaw) {
  if (!this->listener) {
    return;
  }

  const float angle = 180.0f - yaw;
  const int degrees = static_cast<int>(angle);
  const float sine = ClientSine(degrees);
  const float cosine = ClientCosine(degrees);
  MilesRuntime::Get().set3DOrientation(this->listener, sine, 0.0f, -cosine, 0.0f, 1.0f, 0.0f);
}

// OFFSET: 0x0063A560
// FUNCTION: fn_45426
void CSoundMgr::SetRoom(int room) {
  if (!this->provider) {
    return;
  }

  this->roomSupport = AIL_3D_room_type(reinterpret_cast<void*>(this->provider)) != -1;

  if (this->roomSupport) {
    AIL_set_3D_room_type(reinterpret_cast<void*>(this->provider), room);
  }
}

// OFFSET: 0x0063A5A0
// FUNCTION: fn_04769
void CSoundMgr::SetVolume(CSoundMgr* sound, int volume) {
  if (!sound) {
    return;
  }

  sound->streamVolume = volume > 127 ? 127 : (volume < 0 ? 0 : volume);
  MilesRuntime& miles = MilesRuntime::Get();

  if (sound->stream) {
    miles.setStreamVolume(sound->stream, sound->streamVolume);
  }

  if (sound->secondaryStream) {
    miles.setStreamVolume(sound->secondaryStream, sound->streamVolume);
  }
}
