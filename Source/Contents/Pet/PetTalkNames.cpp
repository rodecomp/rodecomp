#include "Contents/Jobs/JobId.h"
#include "Client/Session/Session.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"
#include "Utility/Cipher/CryptName.h"

#include <cstddef>
#include <cstdint>

namespace {

// INLINED
template <std::uint64_t Key, std::size_t Length>
const char* DecryptName(const char (&source)[Length]) {
  thread_local EncryptedName<Length, Key> name(source);
  return name.Text();
}

}

// OFFSET: 0x00E0B320
// FUNCTION: fn_23814
const char* CSession::GetPetTalkName(int job) {
  switch (job) {
  case JT_PORING: {
    const auto source = CRYPT_NAME("poring", 0x0F41EBBDA32D1B17ULL);
    return DecryptName<0x0F41EBBDA32D1B17ULL>(source.data);
  }
  case JT_ROCKER: {
    const auto source = CRYPT_NAME("rocker", 0xFBD599E117C547C1ULL);
    return DecryptName<0xFBD599E117C547C1ULL>(source.data);
  }
  case JT_DROPS: {
    const auto source = CRYPT_NAME("drops", 0x176F138D5DD7F365ULL);
    return DecryptName<0x176F138D5DD7F365ULL>(source.data);
  }
  case JT_POPORING: {
    const auto source = CRYPT_NAME("poporing", 0x452FEF0D7FC95565ULL);
    return DecryptName<0x452FEF0D7FC95565ULL>(source.data);
  }
  case JT_LUNATIC: {
    const auto source = CRYPT_NAME("lunatic", 0x9FED7DA189490D5DULL);
    return DecryptName<0x9FED7DA189490D5DULL>(source.data);
  }
  case JT_PICKY: {
    const auto source = CRYPT_NAME("picky", 0x839B27DF8F5967B3ULL);
    return DecryptName<0x839B27DF8F5967B3ULL>(source.data);
  }
  case JT_CHONCHON: {
    const auto source = CRYPT_NAME("chonchon", 0xF3CB6F7711A71B6DULL);
    return DecryptName<0xF3CB6F7711A71B6DULL>(source.data);
  }
  case JT_STEEL_CHONCHON: {
    const auto source = CRYPT_NAME("steel_chonchon", 0x7941D76335CFA14FULL);
    return DecryptName<0x7941D76335CFA14FULL>(source.data);
  }
  case JT_HUNTER_FLY: {
    const auto source = CRYPT_NAME("hunter_fly", 0x7321C3814F8799E1ULL);
    return DecryptName<0x7321C3814F8799E1ULL>(source.data);
  }
  case JT_DESERT_WOLF_B: {
    const auto source = CRYPT_NAME("desert_wolf_babe", 0x7DE349D717D7F123ULL);
    return DecryptName<0x7DE349D717D7F123ULL>(source.data);
  }
  case JT_SPORE: {
    const auto source = CRYPT_NAME("spore", 0x0F15B16B5BAFD57FULL);
    return DecryptName<0x0F15B16B5BAFD57FULL>(source.data);
  }
  case JT_POISON_SPORE: {
    const auto source = CRYPT_NAME("poison_spore", 0x793F370985ED674FULL);
    return DecryptName<0x793F370985ED674FULL>(source.data);
  }
  case JT_PECOPECO: {
    const auto source = CRYPT_NAME("pecopeco", 0xE55723712993051DULL);
    return DecryptName<0xE55723712993051DULL>(source.data);
  }
  case JT_SMOKIE: {
    const auto source = CRYPT_NAME("smokie", 0x7115D9F973F913C9ULL);
    return DecryptName<0x7115D9F973F913C9ULL>(source.data);
  }
  case JT_YOYO: {
    const auto source = CRYPT_NAME("yoyo", 0xCB1D253B7FE1F563ULL);
    return DecryptName<0xCB1D253B7FE1F563ULL>(source.data);
  }
  case JT_ORK_WARRIOR: {
    const auto source = CRYPT_NAME("ork_warrior", 0x6FE153231D879777ULL);
    return DecryptName<0x6FE153231D879777ULL>(source.data);
  }
  case JT_MUNAK: {
    const auto source = CRYPT_NAME("munak", 0x39A39F87DD5F535BULL);
    return DecryptName<0x39A39F87DD5F535BULL>(source.data);
  }
  case JT_DOKEBI: {
    const auto source = CRYPT_NAME("dokebi", 0xEF6D1D893F2B2DC5ULL);
    return DecryptName<0xEF6D1D893F2B2DC5ULL>(source.data);
  }
  case JT_SOHEE: {
    const auto source = CRYPT_NAME("sohee", 0x234977DDB54BC71DULL);
    return DecryptName<0x234977DDB54BC71DULL>(source.data);
  }
  case JT_ISIS: {
    const auto source = CRYPT_NAME("isis", 0xA12DAB392545D72FULL);
    return DecryptName<0xA12DAB392545D72FULL>(source.data);
  }
  case JT_PETIT: {
    const auto source = CRYPT_NAME("petit", 0x6BA1C1132F033B75ULL);
    return DecryptName<0x6BA1C1132F033B75ULL>(source.data);
  }
  case JT_DEVIRUCHI: {
    const auto source = CRYPT_NAME("deviruchi", 0x2FEB8739E5975BD1ULL);
    return DecryptName<0x2FEB8739E5975BD1ULL>(source.data);
  }
  case JT_BAPHOMET_: {
    const auto source = CRYPT_NAME("baphomet_j", 0x33133B977BCD03CDULL);
    return DecryptName<0x33133B977BCD03CDULL>(source.data);
  }
  case JT_SAVAGE_BABE: {
    const auto source = CRYPT_NAME("savage_babe", 0x8D1D0FF1AF2B13F7ULL);
    return DecryptName<0x8D1D0FF1AF2B13F7ULL>(source.data);
  }
  case JT_BON_GUN: {
    const auto source = CRYPT_NAME("bon_gun", 0xB39FA7917BCFBB8DULL);
    return DecryptName<0xB39FA7917BCFBB8DULL>(source.data);
  }
  case JT_ECLIPSE_P: {
    const auto source = CRYPT_NAME("ECLIPSE_P", 0x2DCB7567394DC1ABULL);
    return DecryptName<0x2DCB7567394DC1ABULL>(source.data);
  }
  case JT_ZHERLTHSH: {
    const auto source = CRYPT_NAME("zherlthsh", 0x4F637DC51307B759ULL);
    return DecryptName<0x4F637DC51307B759ULL>(source.data);
  }
  case JT_ALICE: {
    const auto source = CRYPT_NAME("alice", 0xE52DF30DCBAF7125ULL);
    return DecryptName<0xE52DF30DCBAF7125ULL>(source.data);
  }
  case JT_EVENT_RICECAKE: {
    const auto source = CRYPT_NAME("event_ricecake", 0xCFF3899D95AF355FULL);
    return DecryptName<0xCFF3899D95AF355FULL>(source.data);
  }
  case JT_GOBLINE_XMAS: {
    const auto source = CRYPT_NAME("gobline_xmas", 0x9171997D6963C9F7ULL);
    return DecryptName<0x9171997D6963C9F7ULL>(source.data);
  }
  case JT_CHUNG_E:
  case JT_CHUNG_E_: {
    const auto source = CRYPT_NAME("CHUNG_E", 0x4F99DF03C3EF31B7ULL);
    return DecryptName<0x4F99DF03C3EF31B7ULL>(source.data);
  }
  case JT_P_CHUNG_E: {
    const auto source = CRYPT_NAME("P_CHUNG_E", 0x9949BFD527532943ULL);
    return DecryptName<0x9949BFD527532943ULL>(source.data);
  }
  case JT_GOBLIN_1: {
    const auto source = CRYPT_NAME("goblin_1", 0xCD8F077123FB7D6FULL);
    return DecryptName<0xCD8F077123FB7D6FULL>(source.data);
  }
  case JT_GOBLIN_2: {
    const auto source = CRYPT_NAME("goblin_2", 0x13E33FA1A591AB67ULL);
    return DecryptName<0x13E33FA1A591AB67ULL>(source.data);
  }
  case JT_GOBLIN_4: {
    const auto source = CRYPT_NAME("goblin_4", 0x17A9E5BF39AD4737ULL);
    return DecryptName<0x17A9E5BF39AD4737ULL>(source.data);
  }
  case JT_DELETER_: {
    const auto source = CRYPT_NAME("deleter_", 0x03772763F96D9DA5ULL);
    return DecryptName<0x03772763F96D9DA5ULL>(source.data);
  }
  case JT_DIABOLIC: {
    const auto source = CRYPT_NAME("diabolic", 0x0B7109397F6B1387ULL);
    return DecryptName<0x0B7109397F6B1387ULL>(source.data);
  }
  case JT_WANDER_MAN: {
    const auto source = CRYPT_NAME("wander_man", 0x23BBD58F8B5FF5C9ULL);
    return DecryptName<0x23BBD58F8B5FF5C9ULL>(source.data);
  }
  case JT_BACSOJIN_: {
    const auto source = CRYPT_NAME("BACSOJIN_", 0xC517FB932D95BB6DULL);
    return DecryptName<0xC517FB932D95BB6DULL>(source.data);
  }
  case JT_CIVIL_SERVANT: {
    const auto source = CRYPT_NAME("CIVIL_SERVANT", 0x75ADC57D87EFBD8FULL);
    return DecryptName<0x75ADC57D87EFBD8FULL>(source.data);
  }
  case JT_LEAF_CAT: {
    const auto source = CRYPT_NAME("LEAF_CAT", 0x2BAD156DE3E1F9D9ULL);
    return DecryptName<0x2BAD156DE3E1F9D9ULL>(source.data);
  }
  case JT_LOLI_RURI: {
    const auto source = CRYPT_NAME("LOLI_RURI", 0xF161095D0DE18F95ULL);
    return DecryptName<0xF161095D0DE18F95ULL>(source.data);
  }
  case JT_MARIONETTE: {
    const auto source = CRYPT_NAME("MARIONETTE", 0xEDC74F6D13DF71EFULL);
    return DecryptName<0xEDC74F6D13DF71EFULL>(source.data);
  }
  case JT_SHINOBI: {
    const auto source = CRYPT_NAME("SHINOBI", 0x758F1D0B4F7705B9ULL);
    return DecryptName<0x758F1D0B4F7705B9ULL>(source.data);
  }
  case JT_WHISPER: {
    const auto source = CRYPT_NAME("WHISPER", 0x891B232D09451965ULL);
    return DecryptName<0x891B232D09451965ULL>(source.data);
  }
  case JT_DULLAHAN: {
    const auto source = CRYPT_NAME("DULLAHAN", 0x637B959D55771F69ULL);
    return DecryptName<0x637B959D55771F69ULL>(source.data);
  }
  case JT_GOBLIN_LEADER: {
    const auto source = CRYPT_NAME("GOBLIN_LEADER", 0xD1413DBB01CD8B51ULL);
    return DecryptName<0xD1413DBB01CD8B51ULL>(source.data);
  }
  case JT_GOLEM: {
    const auto source = CRYPT_NAME("GOLEM", 0xB7FD8F670FC7B96BULL);
    return DecryptName<0xB7FD8F670FC7B96BULL>(source.data);
  }
  case JT_IMP: {
    const auto source = CRYPT_NAME("IMP", 0xE7978FB5A94DD7FDULL);
    return DecryptName<0xE7978FB5A94DD7FDULL>(source.data);
  }
  case JT_INCUBUS: {
    const auto source = CRYPT_NAME("INCUBUS", 0x4941FD9F21BBB9FDULL);
    return DecryptName<0x4941FD9F21BBB9FDULL>(source.data);
  }
  case JT_MEDUSA: {
    const auto source = CRYPT_NAME("MEDUSA", 0x35E5656B5DEDCD75ULL);
    return DecryptName<0x35E5656B5DEDCD75ULL>(source.data);
  }
  case JT_MIYABI_NINGYO: {
    const auto source = CRYPT_NAME("MIYABI_NINGYO", 0x2F8B0F37118D157FULL);
    return DecryptName<0x2F8B0F37118D157FULL>(source.data);
  }
  case JT_NIGHTMARE_TERROR: {
    const auto source = CRYPT_NAME("NIGHTMARE_TERROR", 0xAD075B8DB599F1CDULL);
    return DecryptName<0xAD075B8DB599F1CDULL>(source.data);
  }
  case JT_STONE_SHOOTER: {
    const auto source = CRYPT_NAME("STONE_SHOOTER", 0x2FE993D1DDE1F197ULL);
    return DecryptName<0x2FE993D1DDE1F197ULL>(source.data);
  }
  case JT_SUCCUBUS: {
    const auto source = CRYPT_NAME("SUCCUBUS", 0x39FF7FB50DEB998FULL);
    return DecryptName<0x39FF7FB50DEB998FULL>(source.data);
  }
  case JT_WICKED_NYMPH: {
    const auto source = CRYPT_NAME("WICKED_NYMPH", 0x83EFBFBB37EDBDBDULL);
    return DecryptName<0x83EFBFBB37EDBDBDULL>(source.data);
  }
  case JT_J_TAINI: {
    const auto source = CRYPT_NAME("J_TAINI", 0xBF2B4B31397FF5A7ULL);
    return DecryptName<0xBF2B4B31397FF5A7ULL>(source.data);
  }
  case JT_XMAS_LUNATIC: {
    const auto source = CRYPT_NAME("XMAS_LUNATIC", 0xDD3913D5D7F97BE1ULL);
    return DecryptName<0xDD3913D5D7F97BE1ULL>(source.data);
  }
  case JT_TIKBALANG: {
    const auto source = CRYPT_NAME("TIKBALANG", 0x050505679903579BULL);
    return DecryptName<0x050505679903579BULL>(source.data);
  }
  case JT_MARIN: {
    const auto source = CRYPT_NAME("MARIN", 0xF14F79170939C7F1ULL);
    return DecryptName<0xF14F79170939C7F1ULL>(source.data);
  }
  case JT_DOMOVOI: {
    const auto source = CRYPT_NAME("DOMOVOI", 0xA5FD29D7AB6DC59FULL);
    return DecryptName<0xA5FD29D7AB6DC59FULL>(source.data);
  }
  case JT_LITTLE_PORING: {
    const auto source = CRYPT_NAME("LITTLE_PORING", 0xD97D5DBDBB2F2709ULL);
    return DecryptName<0xD97D5DBDBB2F2709ULL>(source.data);
  }
  case JT_MALE_PORING: {
    const auto source = CRYPT_NAME("male_poring", 0xED796785D1DF137FULL);
    return DecryptName<0xED796785D1DF137FULL>(source.data);
  }
  case JT_FEMALE_PORING: {
    const auto source = CRYPT_NAME("female_poring", 0x9B0F234DEFDF7593ULL);
    return DecryptName<0x9B0F234DEFDF7593ULL>(source.data);
  }
  case JT_BLUE_UNICORN: {
    const auto source = CRYPT_NAME("BLUE_UNICORN", 0xD98F5345098D630DULL);
    return DecryptName<0xD98F5345098D630DULL>(source.data);
  }
  case JT_MASTERING: {
    const auto source = CRYPT_NAME("mastering", 0x7F2DFFC1ABE91F2BULL);
    return DecryptName<0x7F2DFFC1ABE91F2BULL>(source.data);
  }
  case JT_SAVAGE: {
    const auto source = CRYPT_NAME("savage", 0xFD69A9B3F311818DULL);
    return DecryptName<0xFD69A9B3F311818DULL>(source.data);
  }
  case JT_GRAND_PECO: {
    const auto source = CRYPT_NAME("grand_peco", 0x4B755DB19953E311ULL);
    return DecryptName<0x4B755DB19953E311ULL>(source.data);
  }
  case JT_HIGH_ORC: {
    const auto source = CRYPT_NAME("high_orc", 0xBD2BA1C11B01E73BULL);
    return DecryptName<0xBD2BA1C11B01E73BULL>(source.data);
  }
  case JT_LITTLE_ISIS: {
    const auto source = CRYPT_NAME("LITTLE_ISIS", 0xED15938D333D2DB9ULL);
    return DecryptName<0xED15938D333D2DB9ULL>(source.data);
  }
  case JT_ANGELING: {
    const auto source = CRYPT_NAME("angeling", 0xC9712D676B5B3943ULL);
    return DecryptName<0xC9712D676B5B3943ULL>(source.data);
  }
  case JT_DR_EGGRING: {
    const auto source = CRYPT_NAME("dr_eggring", 0x9569C9C7E70909AFULL);
    return DecryptName<0x9569C9C7E70909AFULL>(source.data);
  }
  case JT_CHOCO: {
    const auto source = CRYPT_NAME("choco", 0xB19D1FA9116F39D3ULL);
    return DecryptName<0xB19D1FA9116F39D3ULL>(source.data);
  }
  case JT_AM_MUT: {
    const auto source = CRYPT_NAME("am_mut", 0x4191BB5531FD3999ULL);
    return DecryptName<0x4191BB5531FD3999ULL>(source.data);
  }
  case JT_HYEGUN: {
    const auto source = CRYPT_NAME("hyegun", 0xF715514F5961B75BULL);
    return DecryptName<0xF715514F5961B75BULL>(source.data);
  }
  case JT_DR_LUNATIC: {
    const auto source = CRYPT_NAME("dr_lunatic", 0xD9452D81E5CD99C1ULL);
    return DecryptName<0xD9452D81E5CD99C1ULL>(source.data);
  }
  case JT_NINE_TAIL: {
    const auto source = CRYPT_NAME("nine_tail", 0xADADEB4DF7813B55ULL);
    return DecryptName<0xADADEB4DF7813B55ULL>(source.data);
  }
  case JT_CAT_O_NINE_TAIL: {
    const auto source = CRYPT_NAME("cat_o_nine_tail", 0xB195637DAFCB0D59ULL);
    return DecryptName<0xB195637DAFCB0D59ULL>(source.data);
  }
  case JT_DIABOLIC2: {
    const auto source = CRYPT_NAME("diabolic2", 0xD7A749EB27D5BF6DULL);
    return DecryptName<0xD7A749EB27D5BF6DULL>(source.data);
  }
  case JT_DELETER_2: {
    const auto source = CRYPT_NAME("deleter_2", 0x659975C121557D3DULL);
    return DecryptName<0x659975C121557D3DULL>(source.data);
  }
  case JT_TEDDY_BEAR: {
    const auto source = CRYPT_NAME("teddy_bear", 0xBFF5C517A1A77F59ULL);
    return DecryptName<0xBFF5C517A1A77F59ULL>(source.data);
  }
  case JT_GREMLIN: {
    const auto source = CRYPT_NAME("gremlin", 0xAD6153019F93A527ULL);
    return DecryptName<0xAD6153019F93A527ULL>(source.data);
  }
  case JT_SCATLETON: {
    const auto source = CRYPT_NAME("scatleton", 0x85E1F5A7A5755B7FULL);
    return DecryptName<0x85E1F5A7A5755B7FULL>(source.data);
  }
  case JT_MUMMY: {
    const auto source = CRYPT_NAME("mummy", 0x0F61499B4973EB5DULL);
    return DecryptName<0x0F61499B4973EB5DULL>(source.data);
  }
  case JT_WILOW: {
    const auto source = CRYPT_NAME("wilow", 0xB127A19D775BF751ULL);
    return DecryptName<0xB127A19D775BF751ULL>(source.data);
  }
  case JT_ROWEEN: {
    const auto source = CRYPT_NAME("roween", 0xA31F69E7EFED01C9ULL);
    return DecryptName<0xA31F69E7EFED01C9ULL>(source.data);
  }
  case JT_HODREMLIN: {
    const auto source = CRYPT_NAME("hodremlin", 0xB7A96F8701BB4B6BULL);
    return DecryptName<0xB7A96F8701BB4B6BULL>(source.data);
  }
  case JT_METALLER: {
    const auto source = CRYPT_NAME("metaller", 0x3789496BE3E1D141ULL);
    return DecryptName<0x3789496BE3E1D141ULL>(source.data);
  }
  case JT_ANCIENT_MUMMY: {
    const auto source = CRYPT_NAME("ancient_mummy", 0x0F97E779A933D917ULL);
    return DecryptName<0x0F97E779A933D917ULL>(source.data);
  }
  case JT_XM_TEDDY_BEAR: {
    const auto source = CRYPT_NAME("xm_teddy_bear", 0xD1056FF94D4D2773ULL);
    return DecryptName<0xD1056FF94D4D2773ULL>(source.data);
  }
  case JT_SWEETS_DROPS: {
    const auto source = CRYPT_NAME("sweets_drops", 0x4331A17F33095FEBULL);
    return DecryptName<0x4331A17F33095FEBULL>(source.data);
  }
  case JT_MOONLIGHT: {
    const auto source = CRYPT_NAME("moonlight", 0x454D31872BB3EDCBULL);
    return DecryptName<0x454D31872BB3EDCBULL>(source.data);
  }
  case JT_PHREEONI: {
    const auto source = CRYPT_NAME("phreeoni", 0x81F58DED5D470D01ULL);
    return DecryptName<0x81F58DED5D470D01ULL>(source.data);
  }
  case JT_SKELION: {
    const auto source = CRYPT_NAME("skelion", 0x5DE7712FAB613F75ULL);
    return DecryptName<0x5DE7712FAB613F75ULL>(source.data);
  }
  case JT_POURING: {
    const auto source = CRYPT_NAME("pouring", 0x41CFCB831F6B6B91ULL);
    return DecryptName<0x41CFCB831F6B6B91ULL>(source.data);
  }
  case JT_MOONLIGHT2: {
    const auto source = CRYPT_NAME("moonlight", 0x49D33529F1556333ULL);
    return DecryptName<0x49D33529F1556333ULL>(source.data);
  }
  case JT_PHREEONI2: {
    const auto source = CRYPT_NAME("phreeoni", 0x792FFD1325418FB5ULL);
    return DecryptName<0x792FFD1325418FB5ULL>(source.data);
  }
  case JT_BACSOJIN2: {
    const auto source = CRYPT_NAME("bacsojin2", 0xF15F8785CDE369BFULL);
    return DecryptName<0xF15F8785CDE369BFULL>(source.data);
  }
  case JT_ORK_HERO2: {
    const auto source = CRYPT_NAME("ORK_HERO2", 0xE33F2FF779878559ULL);
    return DecryptName<0xE33F2FF779878559ULL>(source.data);
  }
  case JT_ALIZA: {
    const auto source = CRYPT_NAME("ALIZA", 0x1767DDE18D779D8BULL);
    return DecryptName<0x1767DDE18D779D8BULL>(source.data);
  }
  case JT_ALICEL: {
    const auto source = CRYPT_NAME("ALICEL", 0xC1BB8111EDD183E1ULL);
    return DecryptName<0xC1BB8111EDD183E1ULL>(source.data);
  }
  case JT_ALIOT: {
    const auto source = CRYPT_NAME("ALIOT", 0xC5B1BBCD9BC9C997ULL);
    return DecryptName<0xC5B1BBCD9BC9C997ULL>(source.data);
  }
  case 20338:
  case JT_NIGHTMARE_TERROR_H: {
    const auto source = CRYPT_NAME("nightmare_terror_mj", 0x01CB4F1DF9D3F931ULL);
    return DecryptName<0x01CB4F1DF9D3F931ULL>(source.data);
  }
  case JT_WANDER_MAN_H: {
    const auto source = CRYPT_NAME("wander_man_mj", 0x0B8745519B1F95D7ULL);
    return DecryptName<0x0B8745519B1F95D7ULL>(source.data);
  }
  case JT_GLOOMUNDERNIGHT2: {
    const auto source = CRYPT_NAME("gloomundernight2", 0xC70319B5EB61598FULL);
    return DecryptName<0xC70319B5EB61598FULL>(source.data);
  }
  case JT_EP17_2_CHILD_ADMIN1: {
    const auto source = CRYPT_NAME("child_admin_b", 0x271B45771F83DD9FULL);
    return DecryptName<0x271B45771F83DD9FULL>(source.data);
  }
  case JT_EP17_2_CHILD_ADMIN2: {
    const auto source = CRYPT_NAME("child_admin_a", 0x8783C33941E15117ULL);
    return DecryptName<0x8783C33941E15117ULL>(source.data);
  }
  case JT_WANDERING_DUCK: {
    const auto source = CRYPT_NAME("WANDERING_DUCK", 0xD5A78DB38D8DAFABULL);
    return DecryptName<0xD5A78DB38D8DAFABULL>(source.data);
  }
  case JT_KIEL_D_01_2: {
    const auto source = CRYPT_NAME("pet_kiel", 0x0399459DAF454F0FULL);
    return DecryptName<0x0399459DAF454F0FULL>(source.data);
  }
  case 21291: {
    const auto source = CRYPT_NAME("capoo_tw", 0xFFABE3D5D1BBEF73ULL);
    return DecryptName<0xFFABE3D5D1BBEF73ULL>(source.data);
  }
  case JT_DESERT_WOLF: {
    const auto source = CRYPT_NAME("DESERT_WOLF", 0xA597A7E7492B39D5ULL);
    return DecryptName<0xA597A7E7492B39D5ULL>(source.data);
  }
  case JT_DARK_PRIEST: {
    const auto source = CRYPT_NAME("DARK_PRIEST", 0x51BB39B763BBBD8DULL);
    return DecryptName<0x51BB39B763BBBD8DULL>(source.data);
  }
  case JT_FIRE_GOLEM: {
    const auto source = CRYPT_NAME("FIRE_GOLEM", 0x87E3D1C50D572DEFULL);
    return DecryptName<0x87E3D1C50D572DEFULL>(source.data);
  }
  case JT_ILL_BAPHOMET_J: {
    const auto source = CRYPT_NAME("ILL_BAPHOMET_J", 0x3FA3AFA7035DE3FFULL);
    return DecryptName<0x3FA3AFA7035DE3FFULL>(source.data);
  }
  case JT_BAPHOMET2: {
    const auto source = CRYPT_NAME("BAPHOMET2", 0xDB27CB557D038759ULL);
    return DecryptName<0xDB27CB557D038759ULL>(source.data);
  }
  case JT_DARK_ILLUSION2: {
    const auto source = CRYPT_NAME("DARK_ILLUSION2", 0x7D5D0F110B6B8371ULL);
    return DecryptName<0x7D5D0F110B6B8371ULL>(source.data);
  }
  case JT_WILD_ROSE3: {
    const auto source = CRYPT_NAME("WILD_ROSE3", 0x114901C7E37F6D49ULL);
    return DecryptName<0x114901C7E37F6D49ULL>(source.data);
  }
  case JT_BLUEMOON_LOLI_RURI2: {
    const auto source = CRYPT_NAME("BLUEMOON_LOLI_RURI2", 0xDF8117FF49CFF551ULL);
    return DecryptName<0xDF8117FF49CFF551ULL>(source.data);
  }
  case JT_FARMILIAR2: {
    const auto source = CRYPT_NAME("FARMILIAR2", 0xADC51BF9C98B413DULL);
    return DecryptName<0xADC51BF9C98B413DULL>(source.data);
  }
  case JT_FARMILIAR: {
    const auto source = CRYPT_NAME("FARMILIAR", 0x674D9F3975573D41ULL);
    return DecryptName<0x674D9F3975573D41ULL>(source.data);
  }
  case JT_CWHITEKNIGHT: {
    const auto source = CRYPT_NAME("CWHITEKNIGHT", 0x6BDF334F2BFDAD51ULL);
    return DecryptName<0x6BDF334F2BFDAD51ULL>(source.data);
  }
  case JT_KNIGHT_OF_ABYSS2: {
    const auto source = CRYPT_NAME("KNIGHT_OF_ABYSS2", 0x136F2F9915D1DBCFULL);
    return DecryptName<0x136F2F9915D1DBCFULL>(source.data);
  }
  case JT_GH_WHITEKNIGHT: {
    const auto source = CRYPT_NAME("GH_WHITEKNIGHT", 0x0B23D10F2F49D72BULL);
    return DecryptName<0x0B23D10F2F49D72BULL>(source.data);
  }
  case JT_EDDGA3: {
    const auto source = CRYPT_NAME("EDDGA3", 0xB3E307A5F753D5E3ULL);
    return DecryptName<0xB3E307A5F753D5E3ULL>(source.data);
  }
  case 23009: {
    const auto source = CRYPT_NAME("PITAYA_R_PET", 0x192D654D29337BE5ULL);
    return DecryptName<0x192D654D29337BE5ULL>(source.data);
  }
  case JT_LARVA_VIOLET: {
    const auto source = CRYPT_NAME("LARVA_VIOLET", 0xB7AFBD2FD9FD7501ULL);
    return DecryptName<0xB7AFBD2FD9FD7501ULL>(source.data);
  }
  case JT_EP20_COPO: {
    const auto source = CRYPT_NAME("ep20_copo", 0x79E915E9AF6F55DDULL);
    return DecryptName<0x79E915E9AF6F55DDULL>(source.data);
  }
  case JT_EP20_ICEWIND1: {
    const auto source = CRYPT_NAME("ep20_icewind1", 0x6B3301E1A3079125ULL);
    return DecryptName<0x6B3301E1A3079125ULL>(source.data);
  }
  case JT_EP20_ICEWIND2: {
    const auto source = CRYPT_NAME("ep20_icewind2", 0x2F2701A7E56D3D89ULL);
    return DecryptName<0x2F2701A7E56D3D89ULL>(source.data);
  }
  case JT_DARK_LORD2: {
    const auto source = CRYPT_NAME("DARK_LORD2", 0xC39FB559A17D3963ULL);
    return DecryptName<0xC39FB559A17D3963ULL>(source.data);
  }
  case 23215: {
    const auto source = CRYPT_NAME("PITAYA_B_PET", 0x71B7691737C5A1AFULL);
    return DecryptName<0x71B7691737C5A1AFULL>(source.data);
  }
  case 23216: {
    const auto source = CRYPT_NAME("HOSANGTO_PET", 0x07112F85A39F4FABULL);
    return DecryptName<0x07112F85A39F4FABULL>(source.data);
  }
  case 23233: {
    const auto source = CRYPT_NAME("Usagi_PET", 0x45E3F103CBB5F157ULL);
    return DecryptName<0x45E3F103CBB5F157ULL>(source.data);
  }
  case 23234: {
    const auto source = CRYPT_NAME("Piske_PET", 0x936F3D2FFDFD87F5ULL);
    return DecryptName<0x936F3D2FFDFD87F5ULL>(source.data);
  }
  case JT_VR_CHAO: {
    const auto source = CRYPT_NAME("VR_CHAO", 0x2D33791F67AD47DFULL);
    return DecryptName<0x2D33791F67AD47DFULL>(source.data);
  }
  case JT_EP21_ICESEAHORSE: {
    const auto source = CRYPT_NAME("ep21_iceseahorse", 0xA7471985F93F9BDFULL);
    return DecryptName<0xA7471985F93F9BDFULL>(source.data);
  }
  case JT_CLB_SS_FC: {
    const auto source = CRYPT_NAME("CLB_SS_FC", 0x5FFD7FABB5D1EF57ULL);
    return DecryptName<0x5FFD7FABB5D1EF57ULL>(source.data);
  }
  case JT_CLB_SS_TZ: {
    const auto source = CRYPT_NAME("CLB_SS_TZ", 0xD3E5516F89F161D1ULL);
    return DecryptName<0xD3E5516F89F161D1ULL>(source.data);
  }
  case JT_HORNET: {
    const auto source = CRYPT_NAME("Hornet", 0xF535E561137F138DULL);
    return DecryptName<0xF535E561137F138DULL>(source.data);
  }
  case JT_GIANT_HONET2: {
    const auto source = CRYPT_NAME("GIANT_HONET", 0x899D11BB83695B0DULL);
    return DecryptName<0x899D11BB83695B0DULL>(source.data);
  }
  case JT_MISTRESS3: {
    const auto source = CRYPT_NAME("MISTRESS2", 0x2B8725D7E95325F5ULL);
    return DecryptName<0x2B8725D7E95325F5ULL>(source.data);
  }
  case JT_CLOCK: {
    const auto source = CRYPT_NAME("Clock", 0x7989830DE73B9DBDULL);
    return DecryptName<0x7989830DE73B9DBDULL>(source.data);
  }
  case JT_C_TOWER_MANAGER2: {
    const auto source = CRYPT_NAME("C_Tower_Manager", 0xBDADDD87C5A50B6BULL);
    return DecryptName<0xBDADDD87C5A50B6BULL>(source.data);
  }
  case JT_SKEGGIOLD: {
    const auto source = CRYPT_NAME("SKEGGIOLD", 0xC7FB6DF18577D34BULL);
    return DecryptName<0xC7FB6DF18577D34BULL>(source.data);
  }
  case JT_ANGELGOLT3: {
    const auto source = CRYPT_NAME("ANGELGOLT", 0xBD3F430159B551FDULL);
    return DecryptName<0xBD3F430159B551FDULL>(source.data);
  }
  case 23434: {
    const auto source = CRYPT_NAME("DOUGHKUN", 0x2B477F9B5B39B301ULL);
    return DecryptName<0x2B477F9B5B39B301ULL>(source.data);
  }
  case 23435: {
    const auto source = CRYPT_NAME("FUMI_HAUSU", 0x5B832361BDA77197ULL);
    return DecryptName<0x5B832361BDA77197ULL>(source.data);
  }
  case JT_TIMEHOLDER3: {
    const auto source = CRYPT_NAME("TIMEHOLDER2", 0x0DF5B33D9F21EB79ULL);
    return DecryptName<0x0DF5B33D9F21EB79ULL>(source.data);
  }
  default: {
    const auto source = CRYPT_NAME("poring", 0x01E7898D9D235DDFULL);
    return DecryptName<0x01E7898D9D235DDFULL>(source.data);
  }
  }
}
