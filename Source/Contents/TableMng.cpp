#include "Contents/TableMng.h"

#include <windows.h>

#include <utility>

// OFFSET: 0x00CA3400
// FUNCTION: fn_20925
CTableMng::CTableMng() {
  this->m_entryQueue.clear();
}

CTableMng::~CTableMng() {
  this->Clear();
}

// OFFSET: 0x00CA3630
// FUNCTION: fn_20928
void CTableMng::ResetTables() {
  this->m_entryQueue.clear();
}

// OFFSET: 0x00CA3640
// FUNCTION: fn_20929
EntryQueueInfo* CTableMng::GetEntryQueueInfo(unsigned int id) {
  auto it = this->m_entryQueue.find(id);
  if (it == this->m_entryQueue.end()) {
    return nullptr;
  }
  return it->second;
}

// OFFSET: 0x00CA3690
// FUNCTION: fn_20930
bool CTableMng::Init() {
  return true;
}

// OFFSET: 0x00CA3FD0
// FUNCTION: fn_20934
bool CTableMng::ReadEntryQueueBinary() {
  HANDLE file = CreateFileA("Data\\Table\\EntryQueue.bex", GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (file == INVALID_HANDLE_VALUE) {
    MessageBoxA(nullptr, "Data\\Table\\EntryQueue.bex", "Load Failed", MB_ICONEXCLAMATION);
    return false;
  }

  unsigned int count = 0;
  DWORD bytesRead = 0;
  ReadFile(file, &count, 4, &bytesRead, nullptr);
  for (unsigned int i = 0; i < count; ++i) {
    EntryQueueInfo* info = new EntryQueueInfo();
    if (info == nullptr) {
      continue;
    }

    ReadFile(file, &info->id, 4, &bytesRead, nullptr);
    ReadFile(file, info->name, 0x104, &bytesRead, nullptr);
    ReadFile(file, info->description, 0x104, &bytesRead, nullptr);
    ReadFile(file, &info->value020C, 4, &bytesRead, nullptr);
    ReadFile(file, &info->value0210, 4, &bytesRead, nullptr);
    ReadFile(file, &info->flag0214, 1, &bytesRead, nullptr);
    ReadFile(file, &info->flag0215, 1, &bytesRead, nullptr);
    ReadFile(file, &info->flag0216, 1, &bytesRead, nullptr);
    ReadFile(file, &info->flag0217, 1, &bytesRead, nullptr);
    ReadFile(file, &info->flag0218, 1, &bytesRead, nullptr);
    ReadFile(file, &info->value021C, 4, &bytesRead, nullptr);
    ReadFile(file, &info->value0220, 4, &bytesRead, nullptr);
    ReadFile(file, info->text0224, 0x104, &bytesRead, nullptr);
    ReadFile(file, info->text0328, 0x104, &bytesRead, nullptr);
    ReadFile(file, info->text042C, 0x104, &bytesRead, nullptr);
    ReadFile(file, info->text0530, 0x104, &bytesRead, nullptr);
    ReadFile(file, info->text0634, 0x104, &bytesRead, nullptr);
    this->m_entryQueue.insert(std::pair<unsigned int, EntryQueueInfo*>(info->id, info));
  }

  CloseHandle(file);
  return true;
}

// OFFSET: 0x00CA4330
// FUNCTION: fn_20935
void CTableMng::Clear() {
  for (auto it = this->m_entryQueue.begin(); it != this->m_entryQueue.end(); ++it) {
    if (it->second != nullptr) {
      delete it->second;
      it->second = nullptr;
    }
  }
  this->m_entryQueue.clear();
}

// OFFSET: 0x00CA43B0
// FUNCTION: fn_20936
bool CTableMng::AddEntryQueueInfo(unsigned int id, EntryQueueInfo* info) {
  if (this->m_entryQueue.find(id) != this->m_entryQueue.end()) {
    return false;
  }
  this->m_entryQueue.insert(std::pair<unsigned int, EntryQueueInfo*>(id, info));
  return true;
}

// OFFSET: 0x00CA4410
// FUNCTION: fn_20937
void CTableMng::Release() {
  this->Clear();
}
