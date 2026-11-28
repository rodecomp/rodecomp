#include "Client/Application/ErrorDialog.h"
#include "Resource/Resource.h"

#include <cstdio>
#include <cstring>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00AECDE0
// FUNCTION: fn_18144
ResourceName::ResourceName(const char* name) {
  this->Assign(name);
}

// FOLDED: fn_18149
void ResourceName::Assign(const char* name) {
  std::strcpy(this->text, name);
  _strlwr(this->text);
  const std::size_t length = std::strlen(this->text);
  std::uint32_t value = 5381;

  for (std::size_t i = 0; i < length; ++i) {
    value = 33 * value + static_cast<std::int8_t>(this->text[i]);
  }

  this->hash = value;
}

// FOLDED: fn_18148
bool ResourceName::operator<(const ResourceName& other) const {
  if (this->hash != other.hash) {
    return this->hash < other.hash;
  }

  return std::strcmp(this->text, other.text) < 0;
}

// OFFSET: 0x00AEDA10
// FUNCTION: fn_18165
CRes::CRes() : references(0) {
}

// OFFSET: 0x00745260
// FUNCTION: fn_07756
bool CRes::LoadFromBuffer(int, int, int) {
  return false;
}

// OFFSET: 0x00745270
// FUNCTION: fn_07757
void CRes::OnProcess() {
}

// OFFSET: 0x00745280
// FUNCTION: fn_07758
bool CRes::IsTransient() {
  return false;
}

// OFFSET: 0x00747540
// FUNCTION: fn_07789
void CRes::Unload() {
}

// OFFSET: 0x00AEFCF0
// FUNCTION: fn_18185
void CRes::MissingFile(const char* name) {
  char text[256];
  std::sprintf(text, "ResourceError : Can't find file %s\n", name);
  ShowErrorDialog(text, 0);
}

// OFFSET: 0x00AEFAD0
// FUNCTION: fn_18183
std::uint32_t CRes::Lock() {
  return ++this->references;
}

// OFFSET: 0x00AF0BE0
// FUNCTION: fn_18194
std::uint32_t CRes::Unlock() {
  this->lastUse = timeGetTime();
  return --this->references;
}

// OFFSET: 0x00AEF390
// FUNCTION: fn_18178
const char* CRes::GetName() {
  return this->name.text;
}
