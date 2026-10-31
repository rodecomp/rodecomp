#ifndef RAGNAROK_SOURCE_CONTENTS_REPUTEMGRLOAD_INL
#define RAGNAROK_SOURCE_CONTENTS_REPUTEMGRLOAD_INL

#include "Contents/ReputeMgr.h"
#include "File/File.h"
#include "Network/TextEncoding.h"
#include "Utility/JsonCppLoader.h"

#include <algorithm>
#include <cstring>
#include <utility>

// OFFSET: 0x0050AE30

// OFFSET: 0x0050B830

// OFFSET: 0x0060D060

// OFFSET: 0x0060D440

// OFFSET: 0x0060DC70
// FUNCTION: fn_04294
bool CReputeMgr::sub_60DC70(std::map<std::uint64_t, ReputeGroup>& groups) {
  CFile file;
  if (!file.Open("ContentData\\Repute\\ReputeGroupData.bson", 0, true, false)) {
    return false;
  }

  const auto* data = static_cast<const unsigned char*>(file.GetData());
  std::vector<unsigned char> buffer(data, data + file.GetSize());

  JsonCppLoader loader;
  if (!loader.sub_4F3C70(buffer, true)) {
    return false;
  }

  std::map<int, ReputeGroupRecord> records;
  if (!loader.sub_50AE30(&records)) {
    return false;
  }

  groups.clear();
  std::for_each(records.begin(), records.end(), GroupInserter{this, &groups});
  return true;
}

// OFFSET: 0x0060E090
// FUNCTION: fn_04295
bool CReputeMgr::sub_60E090(std::map<std::uint64_t, ReputeInfo>& infos) {
  CFile file;
  if (!file.Open("ContentData\\Repute\\ReputeInfoData.bson", 0, true, false)) {
    return false;
  }

  const auto* data = static_cast<const unsigned char*>(file.GetData());
  std::vector<unsigned char> buffer(data, data + file.GetSize());

  JsonCppLoader loader;
  if (!loader.sub_4F3C70(buffer, true)) {
    return false;
  }

  std::map<int, ReputeInfoRecord> records;
  if (!loader.sub_50B830(&records)) {
    return false;
  }

  infos.clear();
  std::for_each(records.begin(), records.end(), InfoInserter{this, &infos});
  return true;
}

#endif
