#ifndef RAGNAROK_SOURCE_SECURITY_NPROTECTGAMEMON_H
#define RAGNAROK_SOURCE_SECURITY_NPROTECTGAMEMON_H

void CloseNPGameMon();
void PreInitNPGameMonA(const char* name);
int InitNPGameMon();
void SetCallbackToGameMon(int(__stdcall* callback)(int, const int*));

#endif
