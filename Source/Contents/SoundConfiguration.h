#ifndef RAGNAROK_SOURCE_CONTENTS_SOUNDCONFIGURATION_H
#define RAGNAROK_SOURCE_CONTENTS_SOUNDCONFIGURATION_H

#include <windows.h>

// Shared sound configuration. Populated by the setup registry loader.
struct SoundConfiguration {
  /* 0173A924 */ int mode = 0;
  /* 0173A928 */ int speakerType = 0;
  /* 0173A92C */ int digitalRateType = 0;
  /* 0173A930 */ int numSampleType = 0;
  /* 0173A934 */ int digitalBitsType = 0;
  /* 0173A938 */ int preference15 = 0;
  /* 0173A93C */ char providerName[1024] = {};

  // Sound portion of the larger setup loader. baseKey is the application's shared setup key.
  void LoadRegistry(const char* baseKey);
};

#endif
