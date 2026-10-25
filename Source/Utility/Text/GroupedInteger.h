#ifndef RAGNAROK_SOURCE_UTILITY_TEXT_GROUPEDINTEGER_H
#define RAGNAROK_SOURCE_UTILITY_TEXT_GROUPEDINTEGER_H

#include <cstdint>

char* FormatGroupedInteger(int value, char* output, int capacity);
char* FormatGroupedInteger(std::int64_t value, char* output, int capacity);

const char* ReverseFindChar(const char* text, char target);

#endif
