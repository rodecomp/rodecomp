#ifndef RAGNAROK_SOURCE_UTILITY_FORMATSTRING_H
#define RAGNAROK_SOURCE_UTILITY_FORMATSTRING_H

#include <cstddef>

const char* FormatString(const char* format, ...);
int StringFormat(char* buffer, std::size_t sizeInBytes, std::size_t count, const char* format, ...);

#endif
