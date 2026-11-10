#include "Contents/Tables/TableMng.h"

#include <windows.h>
#include <cstdio>
#include <cstring>
#include <utility>
#include <vector>

#include "BasicExcel.hpp"

namespace YExcel {
// OFFSET: 0x00C83CD0
// FUNCTION: fn_20498
BasicExcel::BasicExcel() {
}
}

using YExcel::BasicExcel;
using YExcel::BasicExcelCell;
using YExcel::BasicExcelWorksheet;

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

  BasicExcelWorksheet* sheet = excel->GetWorksheet(0u);
  if (sheet == nullptr) {
    return false;
  }

  int totalRows = sheet->GetTotalRows();
  for (int row = 3; row < totalRows; ++row) {
    EntryQueueInfo* info = new EntryQueueInfo;
    if (info == nullptr) {
      continue;
    }

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
    info->minLevel = cell->GetInteger();

    cell = sheet->Cell(row, 4);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->maxLevel = cell->GetInteger();

    cell = sheet->Cell(row, 5);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->queueFlagA = static_cast<unsigned char>(cell->GetInteger());

    cell = sheet->Cell(row, 6);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->queueFlagB = static_cast<unsigned char>(cell->GetInteger());

    cell = sheet->Cell(row, 7);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->queueFlagC = static_cast<unsigned char>(cell->GetInteger());

    cell = sheet->Cell(row, 8);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->queueFlagD = static_cast<unsigned char>(cell->GetInteger());

    cell = sheet->Cell(row, 9);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->limitType = static_cast<unsigned char>(cell->GetInteger());

    cell = sheet->Cell(row, 10);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->limitValue = cell->GetInteger();

    cell = sheet->Cell(row, 11);
    if (cell == nullptr) {
      goto fail_row;
    }
    info->limitValueEnd = cell->GetInteger();

    cell = sheet->Cell(row, 12);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->detailsText, sizeof(info->detailsText), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->detailsText, sizeof(info->detailsText), nullptr, nullptr);
    }

    cell = sheet->Cell(row, 13);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->rulesText, sizeof(info->rulesText), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->rulesText, sizeof(info->rulesText), nullptr, nullptr);
    }

    cell = sheet->Cell(row, 14);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->rewardText, sizeof(info->rewardText), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->rewardText, sizeof(info->rewardText), nullptr, nullptr);
    }

    cell = sheet->Cell(row, 15);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->extraText, sizeof(info->extraText), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->extraText, sizeof(info->extraText), nullptr, nullptr);
    }

    cell = sheet->Cell(row, 16);
    if (cell == nullptr) {
      goto fail_row;
    }
    if (cell->Type() == 3) {
      strcpy_s(info->bitmapName, sizeof(info->bitmapName), cell->GetString());
    } else if (cell->Type() == 4) {
      wchar_t wbuf[260];
      memset(wbuf, 0, sizeof(wbuf));
      wcscpy_s(wbuf, 260, cell->GetWString());
      WideCharToMultiByte(CP_ACP, 0, wbuf, -1, info->bitmapName, sizeof(info->bitmapName), nullptr, nullptr);
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
    EntryQueueInfo* info = new EntryQueueInfo;
    if (info == nullptr) {
      continue;
    }

    ReadFile(file, &info->id, 4, &bytesRead, nullptr);
    ReadFile(file, info->name, 0x104, &bytesRead, nullptr);
    ReadFile(file, info->description, 0x104, &bytesRead, nullptr);
    ReadFile(file, &info->minLevel, 4, &bytesRead, nullptr);
    ReadFile(file, &info->maxLevel, 4, &bytesRead, nullptr);
    ReadFile(file, &info->queueFlagA, 1, &bytesRead, nullptr);
    ReadFile(file, &info->queueFlagB, 1, &bytesRead, nullptr);
    ReadFile(file, &info->queueFlagC, 1, &bytesRead, nullptr);
    ReadFile(file, &info->queueFlagD, 1, &bytesRead, nullptr);
    ReadFile(file, &info->limitType, 1, &bytesRead, nullptr);
    ReadFile(file, &info->limitValue, 4, &bytesRead, nullptr);
    ReadFile(file, &info->limitValueEnd, 4, &bytesRead, nullptr);
    ReadFile(file, info->detailsText, 0x104, &bytesRead, nullptr);
    ReadFile(file, info->rulesText, 0x104, &bytesRead, nullptr);
    ReadFile(file, info->rewardText, 0x104, &bytesRead, nullptr);
    ReadFile(file, info->extraText, 0x104, &bytesRead, nullptr);
    ReadFile(file, info->bitmapName, 0x104, &bytesRead, nullptr);
    this->m_entryQueue.insert(std::pair<unsigned int, EntryQueueInfo*>(info->id, info));
  }

  CloseHandle(file);
  return true;
}

// OFFSET: 0x00CA4330
// FUNCTION: fn_20935
void CTableMng::Clear() {
  for (auto it = this->m_entryQueue.begin(), end = this->m_entryQueue.end(); it != end; ++it) {
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
  std::map<unsigned int, EntryQueueInfo*>::iterator it = this->m_entryQueue.find(id);
  if (it != this->m_entryQueue.end()) {
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
  if (excel != nullptr) {
    if (excel->Load("Data\\Table\\EntryQueue.xls")) {
      BasicExcelWorksheet* sheet = excel->GetWorksheet(0u);
      if (sheet != nullptr) {
        BasicExcel* excel2 = new BasicExcel();
        if (excel2 != nullptr) {
          excel2->New(1);
          BasicExcelWorksheet* sheet2 = excel2->GetWorksheet(0u);
          if (sheet2 != nullptr) {
            int cols = sheet->GetTotalCols();
            for (int row = 0; row < 3; ++row) {
              for (int col = 0; col < cols; ++col) {
                BasicExcelCell* c1 = sheet->Cell(row, col);
                BasicExcelCell* c2 = sheet2->Cell(row, col);
                if (c1 != nullptr && c2 != nullptr) {
                  if (c1->Type() == 3) {
                    c2->SetString(c1->GetString());
                  } else if (c1->Type() == 4) {
                    c2->Set(c1->GetWString());
                  } else if (c1->Type() == 1) {
                    c2->Set(c1->GetInteger());
                  } else if (c1->Type() == 2) {
                    c2->SetDouble(c1->GetDouble());
                  }
                }
              }
            }

            int row = 3;
            for (auto it = this->m_entryQueue.begin(); it != this->m_entryQueue.end(); ++it, ++row) {
              BasicExcelCell* cell = sheet2->Cell(row, 0);
              if (cell != nullptr) {
                cell->SetInteger(it->second->id);
              }

              cell = sheet2->Cell(row, 1);
              if (cell != nullptr) {
                wchar_t wbuf[260];
                memset(wbuf, 0, sizeof(wbuf));
                MultiByteToWideChar(CP_ACP, 0, it->second->name, -1, wbuf, 260);
                cell->SetWString(wbuf);
              }

              cell = sheet2->Cell(row, 2);
              if (cell != nullptr) {
                wchar_t wbuf[260];
                memset(wbuf, 0, sizeof(wbuf));
                MultiByteToWideChar(CP_ACP, 0, it->second->description, -1, wbuf, 260);
                cell->SetWString(wbuf);
              }

              cell = sheet2->Cell(row, 3);
              if (cell != nullptr) {
                cell->SetInteger(it->second->minLevel);
              }

              cell = sheet2->Cell(row, 4);
              if (cell != nullptr) {
                cell->SetInteger(it->second->maxLevel);
              }

              cell = sheet2->Cell(row, 5);
              if (cell != nullptr) {
                cell->SetInteger(it->second->queueFlagA);
              }

              cell = sheet2->Cell(row, 6);
              if (cell != nullptr) {
                cell->SetInteger(it->second->queueFlagB);
              }

              cell = sheet2->Cell(row, 7);
              if (cell != nullptr) {
                cell->SetInteger(it->second->queueFlagC);
              }

              cell = sheet2->Cell(row, 8);
              if (cell != nullptr) {
                cell->SetInteger(it->second->queueFlagD);
              }

              cell = sheet2->Cell(row, 9);
              if (cell != nullptr) {
                cell->SetInteger(it->second->limitType);
              }

              cell = sheet2->Cell(row, 10);
              if (cell != nullptr) {
                cell->SetInteger(it->second->limitValue);
              }

              cell = sheet2->Cell(row, 11);
              if (cell != nullptr) {
                cell->SetInteger(it->second->limitValueEnd);
              }

              cell = sheet2->Cell(row, 12);
              if (cell != nullptr) {
                wchar_t wbuf[260];
                memset(wbuf, 0, sizeof(wbuf));
                MultiByteToWideChar(CP_ACP, 0, it->second->detailsText, -1, wbuf, 260);
                cell->SetWString(wbuf);
              }

              cell = sheet2->Cell(row, 13);
              if (cell != nullptr) {
                wchar_t wbuf[260];
                memset(wbuf, 0, sizeof(wbuf));
                MultiByteToWideChar(CP_ACP, 0, it->second->rulesText, -1, wbuf, 260);
                cell->SetWString(wbuf);
              }

              cell = sheet2->Cell(row, 14);
              if (cell != nullptr) {
                wchar_t wbuf[260];
                memset(wbuf, 0, sizeof(wbuf));
                MultiByteToWideChar(CP_ACP, 0, it->second->rewardText, -1, wbuf, 260);
                cell->SetWString(wbuf);
              }

              cell = sheet2->Cell(row, 15);
              if (cell != nullptr) {
                wchar_t wbuf[260];
                memset(wbuf, 0, sizeof(wbuf));
                MultiByteToWideChar(CP_ACP, 0, it->second->extraText, -1, wbuf, 260);
                cell->SetWString(wbuf);
              }

              cell = sheet2->Cell(row, 16);
              if (cell != nullptr) {
                wchar_t wbuf[260];
                memset(wbuf, 0, sizeof(wbuf));
                MultiByteToWideChar(CP_ACP, 0, it->second->bitmapName, -1, wbuf, 260);
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
          }
        }
      }
    }
  }
  return false;
}
