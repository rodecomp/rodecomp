#include "Contents/PetEvolutionMgr.h"
#include "Client/Session.h"
#include "Script/NLuaState.h"
#include "Utility/Text/FormatString.h"

extern "C" {
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <new>
#include <string>
#include <utility>
#include <vector>
#include <windows.h>

// GLOBAL: gl_24877
CPetEvolutionMgr* g_petEvolutionMgr;

// OFFSET: 0x00672C20
// FUNCTION: fn_05367
bool CPetEvolutionMgr::Create() {
  if (g_petEvolutionMgr != nullptr) {
    return false;
  }

  g_petEvolutionMgr = new (std::nothrow) CPetEvolutionMgr();
  if (g_petEvolutionMgr == nullptr) {
    return false;
  }

  if (!g_petEvolutionMgr->m_isValid) {
    delete g_petEvolutionMgr;
    g_petEvolutionMgr = nullptr;
    return false;
  }

  if (!g_petEvolutionMgr->Load()) {
    if (g_petEvolutionMgr != nullptr) {
      delete g_petEvolutionMgr;
    }
    g_petEvolutionMgr = nullptr;
    return false;
  }

  return true;
}

// OFFSET: 0x00672D70
// FUNCTION: fn_05368
void CPetEvolutionMgr::Destroy() {
  if (g_petEvolutionMgr != nullptr) {
    delete g_petEvolutionMgr;
    g_petEvolutionMgr = nullptr;
  }
}

// OFFSET: 0x00672D90
// FUNCTION: fn_05369
std::pair<bool, std::vector<int>> CPetEvolutionMgr::GetEvolutionTargets(int petEgg) {
  std::vector<int> targets;
  const auto found = this->m_recipes.find(petEgg);
  if (found == this->m_recipes.end()) {
    return std::pair<bool, std::vector<int>>(false, std::vector<int>());
  }

  for (auto it = found->second.begin(); it != found->second.end(); ++it) {
    targets.push_back(it->target);
  }
  return std::pair<bool, std::vector<int>>(true, targets);
}

// OFFSET: 0x00672FC0
// FUNCTION: fn_05370
std::pair<bool, std::vector<std::pair<int, int>>> CPetEvolutionMgr::GetMaterials(int petEgg, int target) {
  std::vector<std::pair<int, int>> materials;
  const auto found = this->m_recipes.find(petEgg);
  if (found == this->m_recipes.end()) {
    return std::pair<bool, std::vector<std::pair<int, int>>>(false, std::vector<std::pair<int, int>>());
  }

  for (auto it = found->second.begin(); it != found->second.end(); ++it) {
    if (it->target == target) {
      for (auto material = it->materials.begin(); material != it->materials.end(); ++material) {
        materials.push_back(*material);
      }
      return std::pair<bool, std::vector<std::pair<int, int>>>(true, materials);
    }
  }

  return std::pair<bool, std::vector<std::pair<int, int>>>(false, std::vector<std::pair<int, int>>());
}

// OFFSET: 0x00673260
// FUNCTION: fn_05371
bool CPetEvolutionMgr::Load() {
  NLuaState lua;
  if (lua.IsValid()) {
    luaL_openlibs(lua.GetState());
    lua_register(lua.GetState(), "InsertEvolutionRecipeLGU", CPetEvolutionMgr::L_InsertEvolutionRecipeLGU);
    lua_register(lua.GetState(), "InsertPetAutoFeeding", CPetEvolutionMgr::L_InsertPetAutoFeeding);
    if (luaL_loadfile(lua.GetState(), "system\\PetEvolutionCln_true.lub") != 0 || lua_pcall(lua.GetState(), 0, 0, 0) != 0) {
      MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CPetEvolutionMgr", 0);
    } else {
      lua_getglobal(lua.GetState(), "main");
      if (lua_pcall(lua.GetState(), 0, 2, 0) != 0) {
        MessageBoxA(nullptr, lua_tostring(lua.GetState(), -1), "CPetEvolutionMgr", 0);
      } else if (!lua_isstring(lua.GetState(), -1)) {
        MessageBoxA(nullptr, "second return value is not string...", "CPetEvolutionMgr", 0);
      } else {
        if (lua_toboolean(lua.GetState(), -2)) {
          return true;
        }
        MessageBoxA(nullptr, FormatString("error : %s", lua_tostring(lua.GetState(), -1)), "CPetEvolutionMgr", 0);
      }
    }
  }

  return false;
}

// OFFSET: 0x006733E0
// FUNCTION: fn_05372
std::pair<bool, std::string> CPetEvolutionMgr::InsertPetAutoFeeding(int petEgg) {
  for (auto it = this->m_autoFeeding.begin(); it != this->m_autoFeeding.end(); ++it) {
    if (*it == petEgg) {
      return std::pair<bool, std::string>(true, "PetEggITID already exist in AutoFeedingPetList");
    }
  }

  this->m_autoFeeding.push_back(petEgg);
  return std::pair<bool, std::string>(true, "good");
}

// OFFSET: 0x006734C0
// FUNCTION: fn_05373
std::pair<bool, std::string> CPetEvolutionMgr::InsertRecipe(int petEgg, int target, int material, int count) {
  std::vector<PetEvolutionRecipe>& recipes = this->m_recipes[petEgg];
  for (auto it = recipes.begin(); it != recipes.end(); ++it) {
    if (it->target == target) {
      for (auto entry = it->materials.begin(); entry != it->materials.end(); ++entry) {
        if (entry->first == material) {
          return std::pair<bool, std::string>(false, "duplicate material ITID");
        }
      }

      it->materials.push_back(std::make_pair(material, count));
      return std::pair<bool, std::string>(true, "good");
    }
  }

  PetEvolutionRecipe recipe;
  recipe.target = target;
  recipe.materials.push_back(std::make_pair(material, count));
  recipes.push_back(std::move(recipe));
  return std::pair<bool, std::string>(true, "good");
}

// OFFSET: 0x006737C0
// FUNCTION: fn_05374
bool CPetEvolutionMgr::IsAutoFeeding(int petEgg) {
  for (auto it = this->m_autoFeeding.begin(); it != this->m_autoFeeding.end(); ++it) {
    if (*it == petEgg) {
      return true;
    }
  }
  return false;
}

// OFFSET: 0x00673C50

// OFFSET: 0x00673E90
