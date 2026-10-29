#ifndef RAGNAROK_SOURCE_CONTENTS_ITEMS_CHANGEMATERIALMGR_H
#define RAGNAROK_SOURCE_CONTENTS_ITEMS_CHANGEMATERIALMGR_H

#include <map>
#include <string>
#include <vector>

struct lua_State;

struct ChangeMaterialEntry {
  /* 0000 */ int itemId;
  /* 0004 */ int count;

  // INLINED
  bool operator<(const ChangeMaterialEntry& other) const {
    return this->itemId < other.itemId;
  }
};

struct ChangeMaterialProduct {
  /* 0000 */ int itemId;
  /* 0004 */ int count;
  /* 0008 */ int rate;
};

struct ChangeMaterialRecipe {
  // INLINED
  ChangeMaterialRecipe(std::vector<ChangeMaterialEntry> materials, std::vector<ChangeMaterialProduct> products, int type)
      : materials(materials), products(products), type(type) {
  }

  /* 0000 */ std::vector<ChangeMaterialEntry> materials;
  /* 000C */ std::vector<ChangeMaterialProduct> products;
  /* 0018 */ int type;
};

struct ChangeMaterialResult {
  // INLINED
  ChangeMaterialResult(bool success, const char* message) : success(success), message(message) {
  }

  /* 0000 */ bool success;
  /* 0004 */ std::string message;
};

struct MaterialKey {
  explicit MaterialKey(const std::vector<ChangeMaterialEntry>& materials);

  /* 0000 */ std::vector<int> itemIds;
};

struct MaterialKeyLess {
  bool operator()(const MaterialKey& left, const MaterialKey& right) const;
};

class ChangeMaterialRecipeList {
public:
  ChangeMaterialResult Add(std::vector<ChangeMaterialEntry> materials, std::vector<ChangeMaterialProduct> products, int type);

  /* 0000 */ std::vector<ChangeMaterialRecipe> m_recipes;
};

class CChangeMaterialMgr {
public:
  // INLINED
  CChangeMaterialMgr() : m_isValid(true) {
  }

  static bool CreateInstance();
  static void DestroyInstance();
  static int LuaInsertRecipe(lua_State* L);

  bool Load();
  ChangeMaterialResult InsertRecipe(std::vector<ChangeMaterialEntry> materials, const std::vector<ChangeMaterialProduct>& products, int type);

  /* 0000 */ std::map<MaterialKey, ChangeMaterialRecipeList, MaterialKeyLess> m_library;
  /* 0008 */ bool m_isValid;
};

std::string ChangeMaterialFormat(const char* format, ...);

extern CChangeMaterialMgr* g_changeMaterialMgr;

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(ChangeMaterialRecipe) == 0x1C);
static_assert(sizeof(ChangeMaterialResult) == 0x1C);
static_assert(sizeof(CChangeMaterialMgr) == 0xC);
#endif

#endif
