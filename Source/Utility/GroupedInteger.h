#ifndef RAGNAROK_SOURCE_UTILITY_GROUPEDINTEGER_H
#define RAGNAROK_SOURCE_UTILITY_GROUPEDINTEGER_H

#include <cstdint>

// Original formatting preserves its trailing-character truncation policy.
// output must have capacity bytes; nonpositive capacity leaves it untouched.
char* FormatGroupedInteger(int value, char* output, int capacity);
char* FormatGroupedInteger(std::int64_t value, char* output, int capacity);

const char* ReverseFindChar(const char* text, char target);

#endif
