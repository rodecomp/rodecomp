#ifndef RAGNAROK_SOURCE_FILE_RESOURCEPATH_H
#define RAGNAROK_SOURCE_FILE_RESOURCEPATH_H

#include <cstdint>
#include <string>

std::string MakeResourcePath(std::string filename);
bool ResourceExists(const char* filename, int looseOnly);

#endif
