#ifndef RAGNAROK_SOURCE_UTILITY_GUIDPARSE_H
#define RAGNAROK_SOURCE_UTILITY_GUIDPARSE_H

#include <cstddef>
#include <windows.h>

// Returns 1 for eleven conversions, -1 for input exhaustion, otherwise zero.
// Failed parsing may partially overwrite output; trailing text is not rejected.
int ParseGuid(const char* text, std::size_t bytes, GUID& output, bool braced);

#endif
