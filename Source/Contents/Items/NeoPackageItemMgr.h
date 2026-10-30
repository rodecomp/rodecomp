#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_NEOPACKAGEITEMMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_NEOPACKAGEITEMMGR_H

#include "Utility/Singleton.h"

#include <cstddef>
#include <list>
#include <map>
#include <new>
#include <optional>
#include <set>
#include <string>

struct lua_State;

namespace NEO_PACKAGEITEM {

struct PackageEntry {
  // INLINED
  PackageEntry(int itemId, const std::string& name, int probability) : itemId(itemId), name(name), probability(probability) {
  }

  /* 0000 */ int itemId;
  /* 0004 */ std::string name;
  /* 001C */ int probability;
};

struct PackageGroup {
  /* 0000 */ int totalProbability;
  /* 0004 */ std::list<PackageEntry> items;
};

class CNeoPackageItemMgr : public Singleton<CNeoPackageItemMgr> {
public:
  static int LuaInsertPackage(lua_State* state);
  static bool LoadScript();

  CNeoPackageItemMgr();

  void Init();
  void InsertPackage(int packageId, int groupId, int itemId, const std::string& name, int probability);
  std::optional<std::map<int, PackageGroup>*> FindPackage(int itemId);
  bool HasPackage(int itemId);

  /* 0004 */ std::map<int, std::map<int, PackageGroup>> m_packages;
};

class CPaidPackageItemMgr : public Singleton<CPaidPackageItemMgr> {
public:
  static int LuaAddItemSold(lua_State* L);
  static bool LoadScript();

  CPaidPackageItemMgr();

  void Init();
  void AddItem(int itemId);
  bool IsPaidItem(int itemId);

  /* 0004 */ std::set<int> m_items;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PackageEntry) == 0x20);
static_assert(sizeof(PackageGroup) == 0xC);
static_assert(sizeof(CNeoPackageItemMgr) == 0xC);
static_assert(sizeof(CPaidPackageItemMgr) == 0xC);
#endif

}

template <>
NEO_PACKAGEITEM::CNeoPackageItemMgr* Singleton<NEO_PACKAGEITEM::CNeoPackageItemMgr>::s_instance;

template <>
NEO_PACKAGEITEM::CPaidPackageItemMgr* Singleton<NEO_PACKAGEITEM::CPaidPackageItemMgr>::s_instance;

// INLINED
template <>
inline NEO_PACKAGEITEM::CNeoPackageItemMgr* Singleton<NEO_PACKAGEITEM::CNeoPackageItemMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) NEO_PACKAGEITEM::CNeoPackageItemMgr;
  }

  return s_instance;
}

// INLINED
template <>
inline NEO_PACKAGEITEM::CPaidPackageItemMgr* Singleton<NEO_PACKAGEITEM::CPaidPackageItemMgr>::GetInstance() {
  if (!s_instance) {
    s_instance = new (std::nothrow) NEO_PACKAGEITEM::CPaidPackageItemMgr;
  }

  return s_instance;
}

#endif
