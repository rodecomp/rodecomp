#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLLEVELCONFIG_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLLEVELCONFIG_H

#include <json/value.h>

// Original storage belongs to CPlayerSkillBundle's vector at +0x64 and the
// client's homunculus vector at +0x71C. These operations do not allocate or
// invent their enclosing owners; the caller supplies established storage.
void LoadUseSkillLevels(Json::Value& data, int* levels);
void LoadHomunculusSkillLevels(Json::Value& data, int* levels);

#endif
