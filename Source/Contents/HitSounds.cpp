#include "Client/Session.h"
#include "Contents/Jobs/JobInheritance.h"

#include <cstdlib>
#include <string>
#include <vector>

// OFFSET: 0x00DDE3D0
// FUNCTION: fn_23395
const char* CSession::GetHitWaveName(int job) {
  if (!((job >= 31 && job < 1000) || job < 0 || (job > 1000 && job < 3999) || (job >= 20021 && job <= 31999))) {
    const std::vector<std::string>& jobWaves = reinterpret_cast<const std::vector<std::string>&>(this->unknown_525C[0x0C]);
    if (job > 3950) {
      return jobWaves.begin()[job - 3950].c_str();
    }
    return jobWaves.begin()[job].c_str();
  }

  const int weapon = GetRealWeaponId(-1);
  if (weapon == -1) {
    switch (std::rand() % 4) {
    case 0:
      return "_enemy_hit_normal1.wav";
    case 1:
      return "_enemy_hit_normal2.wav";
    case 2:
      return "_enemy_hit_normal3.wav";
    case 3:
      return "_enemy_hit_normal4.wav";
    }
  }

  const std::vector<std::string>& weaponWaves = reinterpret_cast<const std::vector<std::string>&>(this->unknown_525C[0x18]);
  if (weaponWaves.empty() || weaponWaves.size() <= static_cast<unsigned int>(weapon)) {
    return nullptr;
  }

  if (this->GetDisplayJob() == 4046 || this->GetDisplayJob() == 4225) {
    return weaponWaves[weapon].c_str();
  }

  if (weapon == 0) {
    switch (std::rand() % 4) {
    case 0:
      return "_hit_fist1.wav";
    case 1:
      return "_hit_fist2.wav";
    case 2:
      return "_hit_fist3.wav";
    case 3:
      return "_hit_fist4.wav";
    }
  }

  return weaponWaves[weapon].c_str();
}
