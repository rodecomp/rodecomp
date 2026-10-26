#include "File/FileCreation.h"
#include "Utility/Text/FormatString.h"

#include <cstdlib>
#include <direct.h>
#include <fstream>
#include <io.h>
#include <share.h>
#include <windows.h>

// FUNCTION: fn_18255
bool CreateDirectoryPath(std::string path) {
  if (path.empty()) {
    return false;
  }

  std::string::size_type separator = path.find('\\');
  std::string::size_type end = separator;
  std::string prefix;
  std::string remaining(path);
  while (prefix.size() != path.size()) {
    prefix = path.substr(0, end);
    remaining = remaining.substr(separator + 1);
    separator = remaining.find('\\');
    end = separator == std::string::npos ? path.size() : end + separator + 1;
    if (_access(prefix.c_str(), 0) != 0 && _mkdir(prefix.c_str()) != 0) {
      return false;
    }
  }
  return true;
}

// FUNCTION: fn_18256
std::string CreateUniqueFileName(const std::string& filename) {
  char drive[MAX_PATH]{};
  char directory[MAX_PATH]{};
  char name[MAX_PATH]{};
  char extension[MAX_PATH]{};
  _splitpath_s(filename.c_str(), drive, sizeof(drive), directory, sizeof(directory), name, sizeof(name), extension, sizeof(extension));
  std::string parent = FormatOwnedString("%s%s", drive, directory);
  CreateDirectoryPath(parent);

  int index = 0;
  std::string result(filename);
  while (_access(result.c_str(), 0) == 0) {
    result = FormatOwnedString("%s%s%s(%d)%s", drive, directory, name, ++index, extension);
  }
  std::ofstream output(result.c_str(), std::ios::out, _SH_DENYNO);
  output.close();
  return result;
}
