#include "Contents/TableMng.h"

#include <windows.h>
#include <cstdio>
#include <cstring>
#include <utility>

namespace {
class BasicExcelCell {
public:
  int Type() {
    return reinterpret_cast<int(__thiscall*)(BasicExcelCell*)>(0x00C998F0)(this);
  }

  int GetInteger() {
    return reinterpret_cast<int(__thiscall*)(BasicExcelCell*)>(0x00C90B40)(this);
  }

  double GetDouble() {
    return reinterpret_cast<double(__thiscall*)(BasicExcelCell*)>(0x00C909D0)(this);
  }

  const char* GetString() {
    return reinterpret_cast<const char*(__thiscall*)(BasicExcelCell*)>(0x00C90CE0)(this);
  }

  const wchar_t* GetWString() {
    return reinterpret_cast<const wchar_t*(__thiscall*)(BasicExcelCell*)>(0x00C90E40)(this);
  }

  void SetInteger(int val) {
    reinterpret_cast<void(__thiscall*)(BasicExcelCell*, int)>(0x00C993F0)(this, val);
  }

  void SetIntegerAlt(int val) {
    reinterpret_cast<void(__thiscall*)(BasicExcelCell*, int)>(0x00C990A0)(this, val);
  }

  void SetDouble(double val) {
    reinterpret_cast<void(__thiscall*)(BasicExcelCell*, double)>(0x00C991E0)(this, val);
  }

  void SetString(const char* str) {
    reinterpret_cast<void(__thiscall*)(BasicExcelCell*, const char*)>(0x00C99200)(this, str);
  }

  void SetWString(const wchar_t* str) {
    reinterpret_cast<void(__thiscall*)(BasicExcelCell*, const wchar_t*)>(0x00C99610)(this, str);
  }

  void SetWStringAlt(const wchar_t* str) {
    reinterpret_cast<void(__thiscall*)(BasicExcelCell*, const wchar_t*)>(0x00C992E0)(this, str);
  }
};

class BasicExcelWorksheet {
public:
  int GetTotalRows() {
    return reinterpret_cast<int(__thiscall*)(BasicExcelWorksheet*)>(0x00C90DB0)(this);
  }

  int GetTotalCols() {
    return reinterpret_cast<int(__thiscall*)(BasicExcelWorksheet*)>(0x00C90DA0)(this);
  }

  BasicExcelCell* Cell(size_t row, size_t col) {
    return reinterpret_cast<BasicExcelCell*(__thiscall*)(BasicExcelWorksheet*, size_t, size_t)>(0x00C8D120)(this, row, col);
  }
};

class BasicExcel {
public:
  BasicExcel() {
    reinterpret_cast<void*(__thiscall*)(BasicExcel*)>(0x00C83CD0)(this);
  }

  ~BasicExcel() {
    reinterpret_cast<void(__thiscall*)(BasicExcel*)>(0x00C88ED0)(this);
  }

  bool Load(const char* filename) {
    return reinterpret_cast<bool(__thiscall*)(BasicExcel*, const char*)>(0x00C915E0)(this, filename);
  }

  bool SaveAs(const char* filename) {
    return reinterpret_cast<bool(__thiscall*)(BasicExcel*, const char*)>(0x00C98CE0)(this, filename);
  }

  void New(size_t sheets) {
    reinterpret_cast<void(__thiscall*)(BasicExcel*, size_t)>(0x00C92530)(this, sheets);
  }

  BasicExcelWorksheet* GetWorksheet(size_t sheetIndex) {
    return reinterpret_cast<BasicExcelWorksheet*(__thiscall*)(BasicExcel*, size_t)>(0x00C90EE0)(this, sheetIndex);
  }

private:
  unsigned char m_data[0x5A8];
};
static_assert(sizeof(BasicExcel) == 0x5A8);
}

// OFFSET: 0x00CA3400
// FUNCTION: fn_20925
CTableMng::CTableMng() {
  this->m_entryQueue.clear();
}

// OFFSET: 0x00CA35B0
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

// OFFSET: 0x00CA36A0
// FUNCTION: fn_20931
bool CTableMng::ReadEntryQueue() {
  this->Clear();
  BasicExcel* excel = new BasicExcel();
  if (excel != nullptr) {
    if (this->ReadEntryQueueBinary()) {
      delete excel;
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00CA3750
// FUNCTION: fn_20932
bool CTableMng::ReadEntryQueueFromXls() {
  this->Clear();
  BasicExcel* excel = new BasicExcel();
  if (excel != nullptr) {
    if (this->ReadEntryQueueXls(excel)) {
      delete excel;
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00CA3800
// FUNCTION: fn_20933
bool CTableMng::ReadEntryQueueXls(void* excelPtr) {
  if (excelPtr == nullptr) {
    return false;
  }

  BasicExcel* excel = static_cast<BasicExcel*>(excelPtr);
  if (!excel->Load("Data\\Table\\EntryQueue.xls")) {
    MessageBoxA(nullptr, "Data\\Table\\EntryQueue.xls Load Fail", "Load Failed", 0x30);
    return false;
  }

  BasicExcelWorksheet* sheet = excel->GetWorksheet(0);
  if (sheet == nullptr) {
    return false;
  }

  int totalRows = sheet->GetTotalRows();
  for (int row = 3; row < totalRows; ++row) {
    EntryQueueInfo* info = new EntryQueueInfo();
    if (info == nullptr) {
      continue;
    }
    memset(info, 0, sizeof(EntryQueueInfo));

    BasicExcelCell* cell = sheet->Cell(row, 0);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->id = cell->GetInteger();
    if (info->id == 0) {
      delete info;
      continue;
    }

    cell = sheet->Cell(row, 1);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->name, sizeof(info->name), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->name, sizeof(info->name), nullptr, nullptr);
    }

    cell = sheet->Cell(row, 2);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->description, sizeof(info->description), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->description, sizeof(info->description), nullptr, nullptr);
    }

    cell = sheet->Cell(row, 3);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->value020C = cell->GetInteger();

    cell = sheet->Cell(row, 4);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->value0210 = cell->GetInteger();

    cell = sheet->Cell(row, 5);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->flag0214 = static_cast<unsigned char>(cell->GetInteger());

    cell = sheet->Cell(row, 6);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->flag0215 = static_cast<unsigned char>(cell->GetInteger());

    cell = sheet->Cell(row, 7);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->flag0216 = static_cast<unsigned char>(cell->GetInteger());

    cell = sheet->Cell(row, 8);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->flag0217 = static_cast<unsigned char>(cell->GetInteger());

    cell = sheet->Cell(row, 9);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->flag0218 = static_cast<unsigned char>(cell->GetInteger());

    cell = sheet->Cell(row, 10);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->value021C = cell->GetInteger();

    cell = sheet->Cell(row, 11);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->value0220 = cell->GetInteger();

    cell = sheet->Cell(row, 12);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->text0224, sizeof(info->text0224), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->text0224, sizeof(info->text0224), nullptr, nullptr);
    }

    cell = sheet->Cell(row, 13);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->text0328, sizeof(info->text0328), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->text0328, sizeof(info->text0328), nullptr, nullptr);
    }

    cell = sheet->Cell(row, 14);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->text042C, sizeof(info->text042C), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->text042C, sizeof(info->text042C), nullptr, nullptr);
    }

    cell = sheet->Cell(row, 15);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->text0530, sizeof(info->text0530), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->text0530, sizeof(info->text0530), nullptr, nullptr);
    }

    cell = sheet->Cell(row, 16);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->text0634, sizeof(info->text0634), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->text0634, sizeof(info->text0634), nullptr, nullptr);
    }

    if (this->m_entryQueue.find(info->id) != this->m_entryQueue.end()) {
      char buf[260];
      memset(buf, 0, sizeof(buf));
      sprintf_s(buf, sizeof(buf), "Row [%d]", row + 1);
      MessageBoxA(nullptr, "EntryQueue Map Key is already Exist!!!", buf, 0);
      goto fail_row;
    }

    this->m_entryQueue.insert(std::pair<unsigned int, EntryQueueInfo*>(info->id, info));
    continue;

fail_row:
    delete info;
    MessageBoxA(nullptr, "Data\\Table\\EntryQueue.xls Load Fail", "Load Failed", 0x30);
  }

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

// OFFSET: 0x00CA4430
// FUNCTION: fn_20938
bool CTableMng::WriteEntryQueueXls() {
  BasicExcel* excel = new BasicExcel();
  if (excel == nullptr) {
    return false;
  }

  if (!excel->Load("Data\\Table\\EntryQueue.xls")) {
    delete excel;
    return false;
  }

  BasicExcelWorksheet* sheet = excel->GetWorksheet(0);
  if (sheet == nullptr) {
    delete excel;
    return false;
  }

  BasicExcel* excel2 = new BasicExcel();
  if (excel2 == nullptr) {
    delete excel;
    return false;
  }

  excel2->New(1);
  BasicExcelWorksheet* sheet2 = excel2->GetWorksheet(0);
  if (sheet2 == nullptr) {
    delete excel2;
    delete excel;
    return false;
  }

  int cols = sheet->GetTotalCols();
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < cols; ++col) {
      BasicExcelCell* c1 = sheet->Cell(row, col);
      BasicExcelCell* c2 = sheet2->Cell(row, col);
      if (c1 != nullptr && c2 != nullptr) {
        int t = c1->Type();
        if (t == 3) {
          c2->SetString(c1->GetString());
        } else if (t == 4) {
          c2->SetWStringAlt(c1->GetWString());
        } else if (t == 1) {
          c2->SetIntegerAlt(c1->GetInteger());
        } else if (t == 2) {
          c2->SetDouble(c1->GetDouble());
        }
      }
    }
  }

  int row = 3;
  wchar_t wbuf[260];
  for (auto it = this->m_entryQueue.begin(); it != this->m_entryQueue.end(); ++it, ++row) {
    EntryQueueInfo* info = it->second;
    BasicExcelCell* cell = sheet2->Cell(row, 0);
    if (cell != nullptr) {
      cell->SetInteger(info->id);
    }

    cell = sheet2->Cell(row, 1);
    if (cell != nullptr) {
      memset(wbuf, 0, sizeof(wbuf));
      MultiByteToWideChar(CP_ACP, 0, info->name, -1, wbuf, 260);
      cell->SetWString(wbuf);
    }

    cell = sheet2->Cell(row, 2);
    if (cell != nullptr) {
      memset(wbuf, 0, sizeof(wbuf));
      MultiByteToWideChar(CP_ACP, 0, info->description, -1, wbuf, 260);
      cell->SetWString(wbuf);
    }

    cell = sheet2->Cell(row, 3);
    if (cell != nullptr) {
      cell->SetInteger(info->value020C);
    }

    cell = sheet2->Cell(row, 4);
    if (cell != nullptr) {
      cell->SetInteger(info->value0210);
    }

    cell = sheet2->Cell(row, 5);
    if (cell != nullptr) {
      cell->SetInteger(info->flag0214);
    }

    cell = sheet2->Cell(row, 6);
    if (cell != nullptr) {
      cell->SetInteger(info->flag0215);
    }

    cell = sheet2->Cell(row, 7);
    if (cell != nullptr) {
      cell->SetInteger(info->flag0216);
    }

    cell = sheet2->Cell(row, 8);
    if (cell != nullptr) {
      cell->SetInteger(info->flag0217);
    }

    cell = sheet2->Cell(row, 9);
    if (cell != nullptr) {
      cell->SetInteger(info->flag0218);
    }

    cell = sheet2->Cell(row, 10);
    if (cell != nullptr) {
      cell->SetInteger(info->value021C);
    }

    cell = sheet2->Cell(row, 11);
    if (cell != nullptr) {
      cell->SetInteger(info->value0220);
    }

    cell = sheet2->Cell(row, 12);
    if (cell != nullptr) {
      memset(wbuf, 0, sizeof(wbuf));
      MultiByteToWideChar(CP_ACP, 0, info->text0224, -1, wbuf, 260);
      cell->SetWString(wbuf);
    }

    cell = sheet2->Cell(row, 13);
    if (cell != nullptr) {
      memset(wbuf, 0, sizeof(wbuf));
      MultiByteToWideChar(CP_ACP, 0, info->text0328, -1, wbuf, 260);
      cell->SetWString(wbuf);
    }

    cell = sheet2->Cell(row, 14);
    if (cell != nullptr) {
      memset(wbuf, 0, sizeof(wbuf));
      MultiByteToWideChar(CP_ACP, 0, info->text042C, -1, wbuf, 260);
      cell->SetWString(wbuf);
    }

    cell = sheet2->Cell(row, 15);
    if (cell != nullptr) {
      memset(wbuf, 0, sizeof(wbuf));
      MultiByteToWideChar(CP_ACP, 0, info->text0530, -1, wbuf, 260);
      cell->SetWString(wbuf);
    }

    cell = sheet2->Cell(row, 16);
    if (cell != nullptr) {
      memset(wbuf, 0, sizeof(wbuf));
      MultiByteToWideChar(CP_ACP, 0, info->text0634, -1, wbuf, 260);
      cell->SetWString(wbuf);
    }
  }

  delete excel;
  if (DeleteFileA("Data\\Table\\EntryQueue.xls")) {
    if (excel2->SaveAs("Data\\Table\\EntryQueue.xls")) {
      delete excel2;
      return true;
    }
  }

  delete excel2;
  return false;
}
