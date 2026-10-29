#ifndef RAGNAROK_SOURCE_CLIENT_TIMER_H
#define RAGNAROK_SOURCE_CLIENT_TIMER_H

extern int g_frameskip;

void ResetTimer();
void InitTimer(unsigned long freq);
int GetSkipFrameCount();

#endif
