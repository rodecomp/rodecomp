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

// OFFSET: 0x00DE5BD0
// FUNCTION: fn_23494
void CSession::InitJobHitWaveNames() {
  std::vector<std::string>& jobWaves = reinterpret_cast<std::vector<std::string>&>(this->unknown_525C[0x0C]);
  jobWaves.clear();
  jobWaves.resize(28053, "player_clothes.wav");
  jobWaves[0] = "player_clothes.wav";
  jobWaves[24] = "player_wooden_male.wav";
  jobWaves[25] = "player_wooden_male.wav";
  jobWaves[5] = "player_clothes.wav";
  jobWaves[4] = "player_clothes.wav";
  jobWaves[2] = "player_clothes.wav";
  jobWaves[10] = "player_clothes.wav";
  jobWaves[8] = "player_clothes.wav";
  jobWaves[9] = "player_clothes.wav";
  jobWaves[3] = "player_wooden_male.wav";
  jobWaves[6] = "player_wooden_male.wav";
  jobWaves[11] = "player_wooden_male.wav";
  jobWaves[12] = "player_wooden_male.wav";
  jobWaves[1] = "player_metal.wav";
  jobWaves[7] = "player_metal.wav";
  jobWaves[16] = "player_clothes.wav";
  jobWaves[19] = "player_wooden_male.wav";
  jobWaves[20] = "player_wooden_male.wav";
  jobWaves[17] = "player_wooden_male.wav";
  jobWaves[14] = "player_metal.wav";
  jobWaves[15] = "player_metal.wav";
  jobWaves[18] = "player_clothes.wav";
  jobWaves[23] = "player_clothes.wav";
  jobWaves[51] = "player_clothes.wav";
  jobWaves[56] = "player_clothes.wav";
  jobWaves[55] = "player_clothes.wav";
  jobWaves[53] = "player_clothes.wav";
  jobWaves[61] = "player_clothes.wav";
  jobWaves[108] = "player_clothes.wav";
  jobWaves[114] = "player_clothes.wav";
  jobWaves[59] = "player_clothes.wav";
  jobWaves[107] = "player_clothes.wav";
  jobWaves[113] = "player_clothes.wav";
  jobWaves[60] = "player_clothes.wav";
  jobWaves[105] = "player_clothes.wav";
  jobWaves[111] = "player_clothes.wav";
  jobWaves[54] = "player_wooden_male.wav";
  jobWaves[57] = "player_wooden_male.wav";
  jobWaves[62] = "player_wooden_male.wav";
  jobWaves[106] = "player_wooden_male.wav";
  jobWaves[112] = "player_wooden_male.wav";
  jobWaves[63] = "player_wooden_male.wav";
  jobWaves[109] = "player_wooden_male.wav";
  jobWaves[115] = "player_wooden_male.wav";
  jobWaves[52] = "player_metal.wav";
  jobWaves[58] = "player_metal.wav";
  jobWaves[104] = "player_metal.wav";
  jobWaves[110] = "player_metal.wav";
  jobWaves[67] = "player_clothes.wav";
  jobWaves[117] = "player_clothes.wav";
  jobWaves[124] = "player_clothes.wav";
  jobWaves[70] = "player_wooden_male.wav";
  jobWaves[71] = "player_wooden_male.wav";
  jobWaves[118] = "player_wooden_male.wav";
  jobWaves[119] = "player_wooden_male.wav";
  jobWaves[125] = "player_wooden_male.wav";
  jobWaves[126] = "player_wooden_male.wav";
  jobWaves[68] = "player_wooden_male.wav";
  jobWaves[122] = "player_wooden_male.wav";
  jobWaves[129] = "player_wooden_male.wav";
  jobWaves[65] = "player_metal.wav";
  jobWaves[116] = "player_metal.wav";
  jobWaves[123] = "player_metal.wav";
  jobWaves[66] = "player_metal.wav";
  jobWaves[120] = "player_metal.wav";
  jobWaves[127] = "player_metal.wav";
  jobWaves[69] = "player_clothes.wav";
  jobWaves[121] = "player_clothes.wav";
  jobWaves[128] = "player_clothes.wav";
  jobWaves[73] = "player_clothes.wav";
  jobWaves[78] = "player_clothes.wav";
  jobWaves[77] = "player_clothes.wav";
  jobWaves[75] = "player_clothes.wav";
  jobWaves[83] = "player_clothes.wav";
  jobWaves[81] = "player_clothes.wav";
  jobWaves[82] = "player_clothes.wav";
  jobWaves[76] = "player_wooden_male.wav";
  jobWaves[79] = "player_wooden_male.wav";
  jobWaves[84] = "player_wooden_male.wav";
  jobWaves[85] = "player_wooden_male.wav";
  jobWaves[74] = "player_metal.wav";
  jobWaves[80] = "player_metal.wav";
  jobWaves[100] = "player_wooden_male.wav";
  jobWaves[101] = "player_metal.wav";
  jobWaves[102] = "player_clothes.wav";
  jobWaves[89] = "player_clothes.wav";
  jobWaves[92] = "player_wooden_male.wav";
  jobWaves[93] = "player_wooden_male.wav";
  jobWaves[90] = "player_wooden_male.wav";
  jobWaves[87] = "player_metal.wav";
  jobWaves[88] = "player_metal.wav";
  jobWaves[91] = "player_clothes.wav";
  jobWaves[95] = "player_clothes.wav";
  jobWaves[96] = "player_wooden_male.wav";
  jobWaves[97] = "player_metal.wav";
  jobWaves[98] = "player_metal.wav";
  jobWaves[99] = "player_clothes.wav";
  jobWaves[240] = "player_clothes.wav";
  jobWaves[241] = "player_clothes.wav";
  jobWaves[261] = "player_wooden_male.wav";
  jobWaves[262] = "player_wooden_male.wav";
  jobWaves[272] = "player_wooden_male.wav";
  jobWaves[273] = "player_wooden_male.wav";
  jobWaves[274] = "player_wooden_male.wav";
  jobWaves[275] = "player_wooden_male.wav";
  jobWaves[276] = "player_metal.wav";
  jobWaves[288] = "player_metal.wav";
  jobWaves[277] = "player_clothes.wav";
  jobWaves[278] = "player_wooden_male.wav";
  jobWaves[279] = "player_wooden_male.wav";
  jobWaves[289] = "player_metal.wav";
  jobWaves[290] = "player_clothes.wav";
  jobWaves[291] = "player_metal.wav";
  jobWaves[292] = "player_clothes.wav";
  jobWaves[302] = "player_metal.wav";
  jobWaves[304] = "player_wooden_male.wav";
  jobWaves[308] = "player_metal.wav";
  jobWaves[310] = "player_wooden_male.wav";
  jobWaves[307] = "player_wooden_male.wav";
  jobWaves[313] = "player_wooden_male.wav";
  jobWaves[314] = "player_wooden_male.wav";
  jobWaves[305] = "player_metal.wav";
  jobWaves[311] = "player_clothes.wav";
  jobWaves[306] = "player_clothes.wav";
  jobWaves[312] = "player_metal.wav";
  jobWaves[309] = "player_clothes.wav";
  jobWaves[303] = "player_clothes.wav";
  jobWaves[329] = "player_clothes.wav";
  jobWaves[302] = "player_metal.wav";
  jobWaves[330] = "player_metal.wav";
  jobWaves[308] = "player_metal.wav";
  jobWaves[331] = "player_metal.wav";
  jobWaves[352] = "player_metal.wav";
  jobWaves[366] = "player_metal.wav";
  jobWaves[353] = "player_clothes.wav";
  jobWaves[354] = "player_wooden_male.wav";
  jobWaves[355] = "player_wooden_male.wav";
  jobWaves[356] = "player_wooden_male.wav";
  jobWaves[357] = "player_clothes.wav";
  jobWaves[358] = "player_clothes.wav";
}
