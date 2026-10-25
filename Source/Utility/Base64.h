#ifndef RAGNAROK_SOURCE_UTILITY_BASE64_H
#define RAGNAROK_SOURCE_UTILITY_BASE64_H

#include <string>

std::string InsertLinebreaks(std::string text, unsigned int distance);
std::string DecodeBase64(std::string encoded, bool removeLinebreaks = false);

#endif
