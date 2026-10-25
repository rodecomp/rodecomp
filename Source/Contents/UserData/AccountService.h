#ifndef RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTSERVICE_H
#define RAGNAROK_SOURCE_CONTENTS_USERDATA_ACCOUNTSERVICE_H

#include <string>

class LuaScript;

std::string ResolveAccountServiceBase(LuaScript& settings, LuaScript& tables, const char* host, const char* port, const char* overrideKey);

#endif
