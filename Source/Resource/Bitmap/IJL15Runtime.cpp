#include "Resource/Bitmap/IJL15Runtime.h"

#include <system_error>

IJL15Runtime::IJL15Runtime() : m_module(LoadLibraryA("ijl15.dll")) {
  if (!this->m_module) {
    throw std::system_error(GetLastError(), std::system_category(), "ijl15.dll");
  }

  this->initialize = reinterpret_cast<decltype(this->initialize)>(GetProcAddress(this->m_module, "ijlInit"));
  this->read = reinterpret_cast<decltype(this->read)>(GetProcAddress(this->m_module, "ijlRead"));
  this->write = reinterpret_cast<decltype(this->write)>(GetProcAddress(this->m_module, "ijlWrite"));
  this->release = reinterpret_cast<decltype(this->release)>(GetProcAddress(this->m_module, "ijlFree"));

  if (!this->initialize || !this->read || !this->write || !this->release) {
    FreeLibrary(this->m_module);
    throw std::system_error(ERROR_PROC_NOT_FOUND, std::system_category(), "ijl15 exports");
  }
}

IJL15Runtime::~IJL15Runtime() {
  FreeLibrary(this->m_module);
}
