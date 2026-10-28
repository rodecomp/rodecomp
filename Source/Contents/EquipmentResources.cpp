#include "Client/Session.h"

#include <string>

// OFFSET: 0x00E0A2C0
// FUNCTION: fn_23794
std::string CSession::GetAccessoryName(int id) {
  if (id < 0 || static_cast<unsigned int>(id) >= this->m_accessoryNames.size()) {
    return "_병아리모자";
  }
  return this->m_accessoryNames[id];
}
