#ifndef RAGNAROK_SOURCE_FILE_FILECREATION_H
#define RAGNAROK_SOURCE_FILE_FILECREATION_H

#include <string>

bool CreateDirectoryPath(std::string path);
std::string CreateUniqueFileName(const std::string& filename);

#endif
