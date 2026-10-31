#include "Client/Application/ErrorDialog.h"
#include "Resource/Resource.h"

#include <cstdio>
#include <windows.h>
#include <timeapi.h>

// OFFSET: 0x00AECDE0
// FUNCTION: fn_18144
ResourceName::ResourceName(const char* name) {
  this->Assign(name);
}

// INLINED
void ResourceName::Assign(const char* name) {
  myGPack::AssignNameKey(this->hash, this->text, name);
}

// OFFSET: 0x00AEDA10
// FUNCTION: fn_18165
CRes::CRes() : references(0) {
}

// OFFSET: 0x007467C0
// FUNCTION: fn_07777
CRes::~CRes() {
}

// OFFSET: 0x00745260
// FUNCTION: fn_07756
bool CRes::Slot01(int, int, int) {
  return false;
}

// OFFSET: 0x00745270
// FUNCTION: fn_07757
void CRes::Slot02() {
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
void CRes::MissingFile(CRenderSystemBase* system, const char* name) {
  char text[256];
  std::sprintf(text, "ResourceError : Can't find file %s\n", name);
  ShowErrorDialog(system, text, 0);
}

// OFFSET: 0x00AEFAD0
// FUNCTION: fn_18183
std::uint32_t CRes::Lock() {
  return ++this->references;
}

// OFFSET: 0x00AF0BE0
// FUNCTION: fn_18194
void CRes::Unlock() {
  this->lastUse = timeGetTime();
  --this->references;
}

// OFFSET: 0x00AEF390
// FUNCTION: fn_18178
const char* CRes::GetName() {
  return this->name.text;
}
