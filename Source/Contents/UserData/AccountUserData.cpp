#include "Contents/UserData/AccountUserData.h"
#include "Contents/Messenger/WhisperBlockMgr.h"
#include "Network/TextEncoding.h"

// OFFSET: 0x005BC950
// FUNCTION: fn_03466
void LoadWhisperBlockList(Json::Value& data) {
  Json::Value names = data["WhisperBlockList"];

  for (Json::UInt i = 0; i < names.size(); ++i) {
    std::string name = FromUtf8(names[i].asString(), g_textCodePage);
    g_whisperBlockMgr->Add(name);
  }
}
