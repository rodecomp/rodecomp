#ifndef RAGNAROK_SOURCE_UTILITY_GUIDPARSE_H
#define RAGNAROK_SOURCE_UTILITY_GUIDPARSE_H

#include <cstddef>
#include <windows.h>

int ParseGuid(const char* text, std::size_t bytes, GUID& output, bool braced);

#endif
