#ifndef RAGNAROK_SOURCE_FILE_FILEENUMERATION_H
#define RAGNAROK_SOURCE_FILE_FILEENUMERATION_H

#include <list>
#include <string>
#include <vector>

void EnumerateFileNames(std::list<std::string>& names, const char* directory, const char* pattern);
void EnumerateFileNames(std::vector<std::string>& names, const char* directory, const char* pattern);

#endif
