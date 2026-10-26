#include "File/DirectoryRemoval.h"

#include <cstring>
#include <direct.h>
#include <io.h>
#include <list>
#include <string>
#include <sys/stat.h>
#include <windows.h>

// FUNCTION: fn_18245
void RemoveDirectoryTree(const char* path) {
  std::list<std::string> files;
  std::list<std::string> directories;
  WIN32_FIND_DATAA found{};
  char filename[2048];
  std::strcpy(filename, path);
  std::strcat(filename, "\\*.*");
  HANDLE search = FindFirstFileA(filename, &found);
  if (search == INVALID_HANDLE_VALUE) {
    FindClose(search);
  } else {
    do {
      if (found.cFileName[0] == '.' && (found.cFileName[1] == 0 || (found.cFileName[1] == '.' && found.cFileName[2] == 0))) {
        continue;
      }
      if ((found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
        directories.push_back(std::string(found.cFileName));
      } else {
        files.push_back(std::string(found.cFileName));
      }
    } while (FindNextFileA(search, &found));
    FindClose(search);

    std::strcpy(filename, path);
    std::strcat(filename, "\\");
    for (const std::string& file : files) {
      _chmod((std::string(filename) + file).c_str(), _S_IWRITE);
      _unlink((std::string(filename) + file).c_str());
    }
    for (const std::string& directory : directories) {
      RemoveDirectoryTree((std::string(filename) + directory).c_str());
    }
  }
  _rmdir(path);
}
