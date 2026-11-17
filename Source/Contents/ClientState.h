#ifndef RAGNAROK_SOURCE_CONTENTS_CLIENTSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_CLIENTSTATE_H

#include <vector>


void __stdcall SetPetInfoAlert(bool enabled);
void AppendBytes(std::vector<unsigned char>& output, const std::vector<unsigned char>& bytes);

#endif
