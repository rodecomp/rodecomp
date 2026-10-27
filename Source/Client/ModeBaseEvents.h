#ifndef RAGNAROK_SOURCE_CLIENT_MODEBASEEVENTS_H
#define RAGNAROK_SOURCE_CLIENT_MODEBASEEVENTS_H

#include <cstdint>
#include <string>
#include <windows.h>

class CAsyncWorkMgr;
class CRenderSystemBase;
class LuaScript;
struct EmotionHotkeys;
struct CharacterConfigSave;
struct AgencyRequestContext;
struct StopRecruitingContext;

struct ModeBaseEvents {
  int& transitionRequested;
  std::string& transitionName;
  EmotionHotkeys& emotions;
  LuaScript& script;
  const std::uint32_t& persistenceAccountId;
  CRenderSystemBase*& renderSystem;
  const char* token16;
  CAsyncWorkMgr& work;
  CharacterConfigSave& characterSave;
  AgencyRequestContext& agency;
  StopRecruitingContext& stopRecruiting;
  HWND& window;

  int Send(int event, const char* text, int argument2, int argument3, int argument4);
};

#endif
