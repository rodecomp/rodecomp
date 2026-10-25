#ifndef RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLLEVELCONFIG_H
#define RAGNAROK_SOURCE_CONTENTS_SKILLS_SKILLLEVELCONFIG_H

#include <json/value.h>

void LoadUseSkillLevels(Json::Value& data, int* levels);
void LoadHomunculusSkillLevels(Json::Value& data, int* levels);

#endif
