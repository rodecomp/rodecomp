#include "Client/Timer.h"

#include <windows.h>
#include <mmsystem.h>

// GLOBAL: gl_24746
int g_frameskip = 1;

// GLOBAL: gl_24744
static unsigned long frameTime;

// GLOBAL: gl_24745
static unsigned long frameFreq;

// OFFSET: 0x00C7F530
// FUNCTION: fn_20431
int GetSkipFrameCount() {
  int skipCount = (timeGetTime() - frameTime) / frameFreq;
  frameTime += frameFreq * skipCount;

  if (g_frameskip) {
    return skipCount;
  }

  return 1;
}

// OFFSET: 0x00C7F570
// FUNCTION: fn_20432
void InitTimer(unsigned long freq) {
  frameTime = timeGetTime();
  frameFreq = 1000 / freq;
}

// OFFSET: 0x00C7F590
// FUNCTION: fn_20433
void ResetTimer() {
  frameTime = timeGetTime();
}
