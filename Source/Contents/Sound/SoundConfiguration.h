#ifndef RAGNAROK_SOURCE_CONTENTS_SOUND_SOUNDCONFIGURATION_H
#define RAGNAROK_SOURCE_CONTENTS_SOUND_SOUNDCONFIGURATION_H

#include <windows.h>

enum SoundMode : int {
  SOUND_MODE_DISABLED = 0,
  SOUND_MODE_2D = 1,
  SOUND_MODE_3D = 2,
};

enum SoundDigitalRate : int {
  SOUND_RATE_22050 = 0,
  SOUND_RATE_11025 = 1,
  SOUND_RATE_8000 = 2,
};

enum SoundSampleCount : int {
  SOUND_SAMPLES_48 = 0,
  SOUND_SAMPLES_32 = 1,
  SOUND_SAMPLES_16 = 2,
};

enum SoundDigitalBits : int {
  SOUND_BITS_16 = 0,
  SOUND_BITS_8 = 1,
};

struct SoundConfiguration {
  /* 0173A924 */ int mode = SOUND_MODE_DISABLED;
  /* 0173A928 */ int speakerType = 0;
  /* 0173A92C */ int digitalRateType = SOUND_RATE_22050;
  /* 0173A930 */ int numSampleType = 0;
  /* 0173A934 */ int digitalBitsType = 0;
  /* 0173A938 */ int preference15 = 0;
  /* 0173A93C */ char providerName[1024] = {};

  void LoadRegistry(const char* baseKey);
};

extern SoundConfiguration g_soundConfiguration;

int ReadSetup();

#endif
