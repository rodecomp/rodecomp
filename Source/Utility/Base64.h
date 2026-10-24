#ifndef RAGNAROK_SOURCE_UTILITY_BASE64_H
#define RAGNAROK_SOURCE_UTILITY_BASE64_H

#include <string>

// Recovered decoder resembles cpp-base64, but original invalid-character
// passthrough and unchecked indexing differ from current upstream.
std::string InsertLinebreaks(std::string text, unsigned int distance);
std::string DecodeBase64(std::string encoded, bool removeLinebreaks = false);

#endif
