#include "File/DirectoryRemoval.h"

#include <cstring>
#include <direct.h>
#include <io.h>
#include <list>
#include <string>
#include <sys/stat.h>
#include <windows.h>

template __declspec(noinline) std::string::~basic_string() noexcept;

// OFFSET: 0x00AF45E0
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
    std::string dot(".");
    std::string dotdot("..");
    do {
      if (dot != found.cFileName && dotdot != found.cFileName) {
        if ((found.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
          directories.push_back(std::string(found.cFileName));
        } else {
          files.push_back(std::string(found.cFileName));
        }
      }
    } while (FindNextFileA(search, &found));
    FindClose(search);

    std::strcpy(filename, path);
    std::strcat(filename, "\\");
    for (const std::string& file : files) {
      _chmod((filename + file).c_str(), _S_IWRITE);
      _unlink((filename + file).c_str());
    }
    for (const std::string& directory : directories) {
      RemoveDirectoryTree((filename + directory).c_str());
    }
  }
  _rmdir(path);
}
