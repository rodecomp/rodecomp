#ifndef RAGNAROK_SOURCE_CONTENTS_PETEVOLUTIONMGR_H
#define RAGNAROK_SOURCE_CONTENTS_PETEVOLUTIONMGR_H

#include <map>
#include <string>
#include <utility>
#include <vector>

struct lua_State;

struct PetEvolutionRecipe {
  /* 0000 */ int target;
  /* 0004 */ std::vector<std::pair<int, int>> materials;
};

// VTABLE: vt_00355
class CPetEvolutionMgr {
public:
  // INLINED
  CPetEvolutionMgr() : m_isValid(true) {
  }
  // INLINED
  virtual ~CPetEvolutionMgr() {
    this->m_recipes.clear();
    this->unknown_0010.clear();
    this->m_autoFeeding.clear();
  }

  static bool Create();
  static void Destroy();
  static int L_InsertEvolutionRecipeLGU(lua_State* state);
  static int L_InsertPetAutoFeeding(lua_State* state);

  std::pair<bool, std::vector<int>> GetEvolutionTargets(int petEgg);
  std::pair<bool, std::vector<std::pair<int, int>>> GetMaterials(int petEgg, int target);
  bool Load();
  std::pair<bool, std::string> InsertPetAutoFeeding(int petEgg);
  std::pair<bool, std::string> InsertRecipe(int petEgg, int target, int material, int count);
  bool IsAutoFeeding(int petEgg);

  /* 0004 */ bool m_isValid;
  /* 0008 */ std::map<int, std::vector<PetEvolutionRecipe>> m_recipes;
  /* 0010 */ std::map<int, std::string> unknown_0010;
  /* 0018 */ std::vector<int> m_autoFeeding;
};

#if defined(_WIN32) && !defined(_WIN64)
static_assert(sizeof(PetEvolutionRecipe) == 0x10);
static_assert(sizeof(CPetEvolutionMgr) == 0x24);
#endif

extern CPetEvolutionMgr* g_petEvolutionMgr;

#endif
