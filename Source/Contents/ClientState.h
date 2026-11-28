#ifndef RAGNAROK_SOURCE_CONTENTS_CLIENTSTATE_H
#define RAGNAROK_SOURCE_CONTENTS_CLIENTSTATE_H

#include <vector>


void __stdcall SetPetInfoAlert(bool enabled);

// OFFSET: 0x00E4A180
// FUNCTION: fn_24249
inline void AppendBytes(std::vector<unsigned char>& output, const std::vector<unsigned char>& bytes) {
  output.insert(output.end(), bytes.begin(), bytes.end());
}

#endif
