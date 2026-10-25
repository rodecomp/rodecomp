#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Sound/SoundConfiguration.h"
#include "Contents/Sound/SoundMgr.h"
#include "Resource/ResourceMgr.h"
#include "Resource/Audio/WaveResource.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Math/ScalarConversion.h"

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <new>
#include <process.h>

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
CSoundMgr::CSoundMgr(SoundMgrContext& context) : m_context(&context) {
  if (this->Initialize()) {
    this->initialized = 1;
  }
}

// OFFSET: 0x00638C40
// OFFSET: 0x00638D50
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

  if (this->stream) {
    MilesRuntime::Get().closeStream(this->stream);
    this->stream = nullptr;
  }

  if (this->secondaryStream) {
    MilesRuntime::Get().closeStream(this->secondaryStream);
    this->secondaryStream = nullptr;
  }

  this->secondaryEnabled = 0;
}

// OFFSET: 0x00639070
// FUNCTION: fn_04750
bool CSoundMgr::Initialize() {
  std::memset(this->samples, 0, sizeof(this->samples));
  std::memset(this->samples3D, 0, sizeof(this->samples3D));

  for (int i = 0; i < 64; ++i) {
    this->providerNames[i] = nullptr;
    this->providers[i] = 0;
  }

  SoundConfiguration& configuration = this->m_context->configuration;

  if (configuration.mode == 0) {
    return true;
  }

  if (configuration.numSampleType == 0) {
    this->sampleCount = 48;
    this->sample3DCount = 48;
  } else if (configuration.numSampleType == 1) {
    this->sampleCount = 32;
    this->sample3DCount = 32;
  } else {
    this->sampleCount = 16;
    this->sample3DCount = 16;
  }

  MilesRuntime& miles = MilesRuntime::Get();
  miles.setRedistDirectory(".\\");

  std::atexit(&ShutdownMilesAtExit);
  miles.startup();

  this->channels = configuration.mode == 1 ? 1 : 2;

  if (configuration.digitalRateType == 0) {
    this->rate = 44100;
  } else if (configuration.digitalRateType == 1) {
    this->rate = 11025;
  } else {
    this->rate = 8000;
  }

  this->bits = configuration.digitalBitsType == 0 ? 16 : 8;
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
bool CSoundMgr::Create(SoundMgrContext& context) {
  if (g_soundMgr) {
    return false;
  }

  g_soundMgr = new (std::nothrow) CSoundMgr(context);

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
    MilesRuntime& miles = MilesRuntime::Get();
    miles.pauseStream(this->stream, 1);
    miles.streamVolume(this->stream);
  }

  this->FadeIn();
  this->RestartSecondary();
  this->fadePending = 0;
}

// OFFSET: 0x006396B0
// FUNCTION: fn_45423
void CSoundMgr::Shutdown() {
  if (this->m_context->configuration.mode == 0) {
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
  for (void* sample : this->samples) {
    if (sample) {
      MilesRuntime::Get().endSample(sample);
    }
  }

  if (this->provider) {
    for (void* sample : this->samples3D) {
      if (sample) {
        MilesRuntime::Get().end3DSample(sample);
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
  if (this->m_context->configuration.mode == 0 || !this->stream || !this->secondaryStream || this->fadeExit != 0) {
    return false;
  }

  MilesRuntime& miles = MilesRuntime::Get();

  if (miles.streamStatus(this->secondaryStream) == 2) {
    return true;
  }

  return miles.streamStatus(this->stream) == 2;
}

// OFFSET: 0x00639AC0
// FUNCTION: fn_04758
bool CSoundMgr::IsStopped() const {
  return this->stream && MilesRuntime::Get().streamStatus(this->stream) == 8;
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
  const int volume = TruncateToInt(scaledVolume);

  if (!this->provider) {
    return this->PlayData2D(data, x, y, z, 250, 40, 1.0f);
  }

  for (int i = 0; i < this->sample3DCount; ++i) {
    if (!this->samples3D[i]) {
      continue;
    }

    MilesRuntime& miles = MilesRuntime::Get();

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

  if (this->m_context->serviceOptions.Get(7)) {
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
  const float horizontal = std::sqrt(horizontalSquared);
  const float horizontalResquared = horizontal * horizontal;
  const float ySquared = y * y;
  const float distanceSquared = ySquared + horizontalResquared;
  const float distance = std::sqrt(distanceSquared);

  if (distance >= static_cast<float>(maximumDistance)) {
    return false;
  }

  int volume = this->sampleVolume;
  const float minimum = static_cast<float>(minimumDistance);

  if (!(minimum >= distance)) {
    const float numerator = static_cast<float>(volume) * minimum;
    const float attenuatedVolume = numerator / distance;
    volume = TruncateToInt(attenuatedVolume);
  }

  const float scaledVolume = static_cast<float>(volume) * scale;
  volume = TruncateToInt(scaledVolume);

  for (int i = 0; i < this->sampleCount; ++i) {
    if (!this->samples[i]) {
      continue;
    }

    MilesRuntime& miles = MilesRuntime::Get();

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
  if (this->m_context->configuration.mode == 0) {
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

    this->secondaryStream = MilesRuntime::Get().openStream(this->driver, this->currentName.c_str(), 0);
    const std::string copiedName(name);
    return;
  }

  this->StopStreams();
  this->currentName.assign(name);
  MilesRuntime& miles = MilesRuntime::Get();
  this->stream = miles.openStream(this->driver, this->currentName.c_str(), 0);

  {
    const std::string copiedName(name);
  }

  miles.setStreamVolume(this->stream, this->streamVolume);
  miles.setStreamLoopCount(this->stream, loopCount);
  miles.startStream(this->stream);
}

// OFFSET: 0x0063A130
// FUNCTION: fn_04764
void CSoundMgr::PlayWave(const char* name, float x, float y, float z, int maximumDistance, int minimumDistance, float scale, int unused) {
  const unsigned char enabled = this->m_context->serviceOptions.Get(11);

  if (this->m_context->configuration.mode == 0 || !enabled) {
    return;
  }

  static constexpr char permittedName[] = "\xB9\xF6\xC6\xB0\xBC\xD2\xB8\xAE.wav";

  if (this->unknown != 0 && std::strcmp(name, permittedName) != 0) {
    return;
  }

  CWave* wave = static_cast<CWave*>(this->m_context->resources.Load(name));

  if (!wave) {
    return;
  }

  if (this->m_context->configuration.mode == 1) {
    this->PlayData2D(wave->data, x, y, z, maximumDistance, minimumDistance, scale);
  } else if (this->m_context->configuration.mode == 2) {
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
    MilesRuntime& miles = MilesRuntime::Get();
    miles.closeStream(this->secondaryStream);
    this->secondaryStream = nullptr;
    this->secondaryStream = miles.openStream(this->driver, this->currentName.c_str(), 0);
    this->reopenPending = 0;
  }

  if (this->secondaryStream) {
    MilesRuntime& miles = MilesRuntime::Get();
    miles.setStreamVolume(this->secondaryStream, this->streamVolume);
    miles.setStreamLoopCount(this->secondaryStream, 1);
    miles.startStream(this->secondaryStream);
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
  const int degrees = TruncateToInt(angle);
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

  MilesRuntime& miles = MilesRuntime::Get();
  this->roomSupport = miles.roomType(this->provider) != -1;

  if (this->roomSupport) {
    miles.setRoomType(this->provider, room);
  }
}

// OFFSET: 0x0063A5A0
// FUNCTION: fn_04769
void CSoundMgr::SetVolume(CSoundMgr* sound, int volume) {
  if (!sound) {
    return;
  }

  sound->streamVolume = volume > 127 ? 127 : (volume < 0 ? 0 : volume);

  if (sound->stream) {
    MilesRuntime::Get().setStreamVolume(sound->stream, sound->streamVolume);
  }

  if (sound->secondaryStream) {
    MilesRuntime::Get().setStreamVolume(sound->secondaryStream, sound->streamVolume);
  }
}
