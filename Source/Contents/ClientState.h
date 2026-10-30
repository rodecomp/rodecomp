#ifndef RAGNAROK_SOURCE_CONTENTS_CLIENTSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_CLIENTSTATE_H

#include <vector>

extern bool g_homunculusAutoFeed;
extern bool g_petAutoFeed;

void RequestBasicInfoRefresh();
void __stdcall sub_E489C0(bool enabled);
void AppendBytes(std::vector<unsigned char>& output, const std::vector<unsigned char>& bytes);

#endif
