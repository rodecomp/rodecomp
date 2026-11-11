#include "Contents/Jobs/JobId.h"
#include "Client/Session/Session.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Cipher/LocalEncryptedName.h"

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
    const char source[] = {'\x67', '\x74', '\x5F', '\xCA', '\xD3', '\x8C', '\x41'};
    return DecryptName<0x0F41EBBDA32D1B17ULL>(source);
  }
  case JT_ROCKER: {
    const char source[] = {'\xB3', '\x28', '\xA6', '\x7C', '\x84', '\xEB', '\xD5'};
    return DecryptName<0xFBD599E117C547C1ULL>(source);
  }
  case JT_DROPS: {
    const char source[] = {'\x01', '\x81', '\xB8', '\x2D', '\xFE', '\x13'};
    return DecryptName<0x176F138D5DD7F365ULL>(source);
  }
  case JT_POPORING: {
    const char source[] = {'\x15', '\x3A', '\xB9', '\x10', '\x7F', '\x86', '\x41', '\x22', '\x65'};
    return DecryptName<0x452FEF0D7FC95565ULL>(source);
  }
  case JT_LUNATIC: {
    const char source[] = {'\x31', '\x78', '\x27', '\xE8', '\xD5', '\x14', '\x8E', '\x9F'};
    return DecryptName<0x9FED7DA189490D5DULL>(source);
  }
  case JT_PICKY: {
    const char source[] = {'\xC3', '\x0E', '\x3A', '\xE4', '\xA6', '\x27'};
    return DecryptName<0x839B27DF8F5967B3ULL>(source);
  }
  case JT_CHONCHON: {
    const char source[] = {'\x0E', '\x73', '\xC8', '\x7F', '\x14', '\x07', '\xA4', '\x9D', '\x6D'};
    return DecryptName<0xF3CB6F7711A71B6DULL>(source);
  }
  case JT_STEEL_CHONCHON: {
    const char source[] = {'\x3C', '\xD5', '\xAA', '\x50', '\x0F', '\x88', '\x22', '\x11', '\x20', '\xCF', '\xAC', '\x5D', '\x0C', '\xB9', '\x41'};
    return DecryptName<0x7941D76335CFA14FULL>(source);
  }
  case JT_HUNTER_FLY: {
    const char source[] = {'\x89', '\xEC', '\xE9', '\x3B', '\xE4', '\xB1', '\x7E', '\x15', '\x8D', '\xE0', '\x87'};
    return DecryptName<0x7321C3814F8799E1ULL>(source);
  }
  case JT_DESERT_WOLF_B: {
    const char source[] = {'\x47', '\x94', '\xA4', '\x72', '\xA5', '\x3D', '\xBC', '\x0A', '\x4C', '\x9D', '\xB1', '\x48', '\xB5', '\x28', '\x81', '\x18', '\x23'};
    return DecryptName<0x7DE349D717D7F123ULL>(source);
  }
  case JT_SPORE: {
    const char source[] = {'\x0C', '\xA5', '\xC0', '\x29', '\x0E', '\xB1'};
    return DecryptName<0x0F15B16B5BAFD57FULL>(source);
  }
  case JT_POISON_SPORE: {
    const char source[] = {'\x3F', '\x08', '\x84', '\xF6', '\x66', '\x59', '\x60', '\x0A', '\x3F', '\x08', '\x9F', '\xE0', '\x09'};
    return DecryptName<0x793F370985ED674FULL>(source);
  }
  case JT_PECOPECO: {
    const char source[] = {'\x6D', '\x60', '\xF0', '\x46', '\x01', '\x46', '\x34', '\x8A', '\x1D'};
    return DecryptName<0xE55723712993051DULL>(source);
  }
  case JT_SMOKIE: {
    const char source[] = {'\xBA', '\x7E', '\x96', '\x18', '\x90', '\xBC', '\x15'};
    return DecryptName<0x7115D9F973F913C9ULL>(source);
  }
  case JT_YOYO: {
    const char source[] = {'\x1A', '\x9A', '\x98', '\x10', '\x3B'};
    return DecryptName<0xCB1D253B7FE1F563ULL>(source);
  }
  case JT_ORK_WARRIOR: {
    const char source[] = {'\x18', '\xE5', '\xEC', '\x42', '\x54', '\x32', '\x93', '\x1D', '\x1E', '\xF8', '\xF5', '\x1D'};
    return DecryptName<0x6FE153231D879777ULL>(source);
  }
  case JT_MUNAK: {
    const char source[] = {'\x36', '\x26', '\x31', '\xBC', '\xEC', '\x9F'};
    return DecryptName<0x39A39F87DD5F535BULL>(source);
  }
  case JT_DOKEBI: {
    const char source[] = {'\xA1', '\x42', '\x40', '\x5A', '\xEB', '\x74', '\x6D'};
    return DecryptName<0xEF6D1D893F2B2DC5ULL>(source);
  }
  case JT_SOHEE: {
    const char source[] = {'\x6E', '\xA8', '\x23', '\xD0', '\xB8', '\x77'};
    return DecryptName<0x234977DDB54BC71DULL>(source);
  }
  case JT_ISIS: {
    const char source[] = {'\x46', '\xA4', '\x2C', '\x56', '\x39'};
    return DecryptName<0xA12DAB392545D72FULL>(source);
  }
  case JT_PETIT: {
    const char source[] = {'\x05', '\x5E', '\x77', '\x46', '\x67', '\xC1'};
    return DecryptName<0x6BA1C1132F033B75ULL>(source);
  }
  case JT_DEVIRUCHI: {
    const char source[] = {'\xB5', '\x3E', '\xE1', '\x8C', '\x4B', '\xF2', '\x88', '\x47', '\xB8', '\x5B'};
    return DecryptName<0x2FEB8739E5975BD1ULL>(source);
  }
  case JT_BAPHOMET_: {
    const char source[] = {'\xAF', '\x62', '\xBD', '\x13', '\xF8', '\x56', '\x76', '\x47', '\x92', '\x69', '\xCD'};
    return DecryptName<0x33133B977BCD03CDULL>(source);
  }
  case JT_SAVAGE_BABE: {
    const char source[] = {'\x84', '\x72', '\x5D', '\xCE', '\x96', '\x6A', '\x42', '\xEF', '\x96', '\x71', '\x4E', '\xAF'};
    return DecryptName<0x8D1D0FF1AF2B13F7ULL>(source);
  }
  case JT_BON_GUN: {
    const char source[] = {'\xEF', '\xD4', '\xA1', '\x24', '\xF6', '\xD2', '\xF1', '\xB3'};
    return DecryptName<0xB39FA7917BCFBB8DULL>(source);
  }
  case JT_ECLIPSE_P: {
    const char source[] = {'\xEE', '\x82', '\x01', '\x70', '\x37', '\x26', '\x8E', '\x72', '\xFB', '\xC1'};
    return DecryptName<0x2DCB7567394DC1ABULL>(source);
  }
  case JT_ZHERLTHSH: {
    const char source[] = {'\x23', '\xDF', '\x62', '\x61', '\xA9', '\x09', '\x0B', '\x3C', '\x31', '\xB7'};
    return DecryptName<0x4F637DC51307B759ULL>(source);
  }
  case JT_ALICE: {
    const char source[] = {'\x44', '\x1D', '\xC6', '\xA8', '\x68', '\xF3'};
    return DecryptName<0xE52DF30DCBAF7125ULL>(source);
  }
  case JT_EVENT_RICECAKE: {
    const char source[] = {'\x3A', '\x43', '\xCA', '\xFB', '\xE9', '\xD6', '\x81', '\xA6', '\x3C', '\x50', '\xCC', '\xF4', '\xF6', '\xEC', '\xF3'};
    return DecryptName<0xCFF3899D95AF355FULL>(source);
  }
  case JT_GOBLINE_XMAS: {
    const char source[] = {'\x90', '\xA6', '\x01', '\x05', '\x14', '\xF7', '\x14', '\xCE', '\x8F', '\xA4', '\x02', '\x1A', '\x7D'};
    return DecryptName<0x9171997D6963C9F7ULL>(source);
  }
  case JT_CHUNG_E:
  case JT_CHUNG_E_: {
    const char source[] = {'\xF4', '\x79', '\xBA', '\x8D', '\x44', '\x80', '\xDC', '\x4F'};
    return DecryptName<0x4F99DF03C3EF31B7ULL>(source);
  }
  case JT_P_CHUNG_E: {
    const char source[] = {'\x13', '\x76', '\x10', '\x6F', '\x80', '\xF1', '\x0E', '\xC6', '\x06', '\x29'};
    return DecryptName<0x9949BFD527532943ULL>(source);
  }
  case JT_GOBLIN_1: {
    const char source[] = {'\x08', '\x12', '\x99', '\x4F', '\x18', '\x69', '\xD0', '\xFC', '\x6F'};
    return DecryptName<0xCD8F077123FB7D6FULL>(source);
  }
  case JT_GOBLIN_2: {
    const char source[] = {'\x00', '\xC4', '\xF3', '\xC9', '\xC8', '\x51', '\xBC', '\x21', '\x67'};
    return DecryptName<0x13E33FA1A591AB67ULL>(source);
  }
  case JT_GOBLIN_4: {
    const char source[] = {'\x50', '\x28', '\xCF', '\x55', '\xD6', '\x8B', '\xF6', '\x23', '\x37'};
    return DecryptName<0x17A9E5BF39AD4737ULL>(source);
  }
  case JT_DELETER_: {
    const char source[] = {'\xC1', '\xF8', '\x01', '\x9C', '\x17', '\x42', '\x05', '\x5C', '\xA5'};
    return DecryptName<0x03772763F96D9DA5ULL>(source);
  }
  case JT_DIABOLIC: {
    const char source[] = {'\xE3', '\x7A', '\x0A', '\x1D', '\x56', '\x65', '\x18', '\x68', '\x87'};
    return DecryptName<0x0B7109397F6B1387ULL>(source);
  }
  case JT_WANDER_MAN: {
    const char source[] = {'\xBE', '\x94', '\x31', '\xEF', '\xEA', '\xA7', '\xE4', '\x4E', '\xA8', '\x9B', '\x5F'};
    return DecryptName<0x23BBD58F8B5FF5C9ULL>(source);
  }
  case JT_BACSOJIN_: {
    const char source[] = {'\x2F', '\xFA', '\xD6', '\x7E', '\xDC', '\xB1', '\x5E', '\x8B', '\x32', '\xBB'};
    return DecryptName<0xC517FB932D95BB6DULL>(source);
  }
  case JT_CIVIL_SERVANT: {
    const char source[] = {'\xCC', '\xF4', '\xB9', '\xCE', '\x31', '\x9A', '\xFE', '\x30', '\xDD', '\xEB', '\xAE', '\xC9', '\x29', '\xC5'};
    return DecryptName<0x75ADC57D87EFBD8FULL>(source);
  }
  case JT_LEAF_CAT: {
    const char source[] = {'\x95', '\xBC', '\xA0', '\xA5', '\x32', '\x56', '\xEC', '\x7F', '\xD9'};
    return DecryptName<0x2BAD156DE3E1F9D9ULL>(source);
  }
  case JT_LOLI_RURI: {
    const char source[] = {'\xD9', '\xC0', '\xAD', '\x44', '\x02', '\x5B', '\x34', '\xA3', '\xDC', '\x8F'};
    return DecryptName<0xF161095D0DE18F95ULL>(source);
  }
  case JT_MARIONETTE: {
    const char source[] = {'\xA2', '\x30', '\x8D', '\x5A', '\x22', '\x01', '\x82', '\xB9', '\xBB', '\x34', '\xDF'};
    return DecryptName<0xEDC74F6D13DF71EFULL>(source);
  }
  case JT_SHINOBI: {
    const char source[] = {'\xEA', '\x4D', '\x3E', '\x01', '\x44', '\x5F', '\xC6', '\x75'};
    return DecryptName<0x758F1D0B4F7705B9ULL>(source);
  }
  case JT_WHISPER: {
    const char source[] = {'\x32', '\x51', '\x0C', '\x5A', '\x7D', '\x66', '\x49', '\x89'};
    return DecryptName<0x891B232D09451965ULL>(source);
  }
  case JT_DULLAHAN: {
    const char source[] = {'\x2D', '\x4A', '\x3B', '\x19', '\xDC', '\xDD', '\x3A', '\x2D', '\x69'};
    return DecryptName<0x637B959D55771F69ULL>(source);
  }
  case JT_GOBLIN_LEADER: {
    const char source[] = {'\x16', '\xC4', '\x8F', '\x4D', '\xF2', '\x73', '\x1E', '\x9D', '\x14', '\xCA', '\x89', '\x44', '\xE9', '\x3D'};
    return DecryptName<0xD1413DBB01CD8B51ULL>(source);
  }
  case JT_GOLEM: {
    const char source[] = {'\x2C', '\xF6', '\x8B', '\x4A', '\x2A', '\x8F'};
    return DecryptName<0xB7FD8F670FC7B96BULL>(source);
  }
  case JT_IMP: {
    const char source[] = {'\xB4', '\x9A', '\x1D', '\xA9'};
    return DecryptName<0xE7978FB5A94DD7FDULL>(source);
  }
  case JT_INCUBUS: {
    const char source[] = {'\xB4', '\xF7', '\xF8', '\x74', '\xDD', '\xA8', '\x12', '\x49'};
    return DecryptName<0x4941FD9F21BBB9FDULL>(source);
  }
  case JT_MEDUSA: {
    const char source[] = {'\x38', '\x88', '\xA9', '\x08', '\x38', '\x24', '\xE5'};
    return DecryptName<0x35E5656B5DEDCD75ULL>(source);
  }
  case JT_MIYABI_NINGYO: {
    const char source[] = {'\x32', '\x5C', '\xD4', '\x50', '\x75', '\x46', '\xD4', '\x61', '\x36', '\x5B', '\xCA', '\x48', '\x78', '\x0F'};
    return DecryptName<0x2F8B0F37118D157FULL>(source);
  }
  case JT_NIGHTMARE_TERROR: {
    const char source[] = {'\x83', '\xB8', '\xDE', '\xFD', '\xD9', '\x16', '\x46', '\xFF', '\x88', '\xAE', '\xCD', '\xF0', '\xDF', '\x09', '\x48', '\xFF', '\xCD'};
    return DecryptName<0xAD075B8DB599F1CDULL>(source);
  }
  case JT_STONE_SHOOTER: {
    const char source[] = {'\xC4', '\xA5', '\xAE', '\x93', '\x94', '\xCC', '\xBA', '\x67', '\xD8', '\xBE', '\xB5', '\x98', '\x83', '\x93'};
    return DecryptName<0x2FE993D1DDE1F197ULL>(source);
  }
  case JT_SUCCUBUS: {
    const char source[] = {'\xDC', '\xCC', '\xA8', '\x4E', '\xE0', '\x3D', '\xAA', '\x6A', '\x8F'};
    return DecryptName<0x39FF7FB50DEB998FULL>(source);
  }
  case JT_WICKED_NYMPH: {
    const char source[] = {'\xEA', '\xF4', '\xAE', '\x7C', '\xFE', '\xFB', '\xB0', '\xCD', '\xE4', '\xF0', '\xBD', '\x7F', '\xBB'};
    return DecryptName<0x83EFBFBB37EDBDBDULL>(source);
  }
  case JT_J_TAINI: {
    const char source[] = {'\xED', '\xAA', '\x2B', '\x78', '\x78', '\x05', '\x62', '\xBF'};
    return DecryptName<0xBF2B4B31397FF5A7ULL>(source);
  }
  case JT_XMAS_LUNATIC: {
    const char source[] = {'\xB9', '\x36', '\xB8', '\x84', '\x8A', '\x5F', '\x6C', '\x93', '\xA0', '\x2F', '\xB0', '\x94', '\xD5'};
    return DecryptName<0xDD3913D5D7F97BE1ULL>(source);
  }
  case JT_TIKBALANG: {
    const char source[] = {'\xCF', '\x1E', '\x48', '\xDB', '\x26', '\x49', '\x44', '\x4B', '\xDC', '\x57'};
    return DecryptName<0x050505679903579BULL>(source);
  }
  case JT_MARIN: {
    const char source[] = {'\xBC', '\x86', '\x6B', '\x40', '\x59', '\x79'};
    return DecryptName<0xF14F79170939C7F1ULL>(source);
  }
  case JT_DOMOVOI: {
    const char source[] = {'\xDB', '\x8A', '\x20', '\xE4', '\x81', '\x66', '\xB4', '\xA5'};
    return DecryptName<0xA5FD29D7AB6DC59FULL>(source);
  }
  case JT_LITTLE_PORING: {
    const char source[] = {'\x45', '\x6E', '\x7B', '\xEF', '\xF1', '\x18', '\x22', '\x89', '\x46', '\x75', '\x66', '\xF5', '\xFA', '\x5D'};
    return DecryptName<0xD97D5DBDBB2F2709ULL>(source);
  }
  case JT_MALE_PORING: {
    const char source[] = {'\x12', '\x72', '\xB3', '\xB4', '\xDA', '\x17', '\x16', '\x9F', '\x16', '\x7D', '\xB8', '\xD1'};
    return DecryptName<0xED796785D1DF137FULL>(source);
  }
  case JT_FEMALE_PORING: {
    const char source[] = {'\xF5', '\x10', '\xB2', '\x8E', '\x21', '\x46', '\x50', '\xEB', '\xFC', '\x07', '\xB6', '\x81', '\x2A', '\x23'};
    return DecryptName<0x9B0F234DEFDF7593ULL>(source);
  }
  case JT_BLUE_UNICORN: {
    const char source[] = {'\x4F', '\x2F', '\xD8', '\x4C', '\x1A', '\x06', '\xC1', '\x90', '\x4E', '\x2C', '\xDF', '\x47', '\x45'};
    return DecryptName<0xD98F5345098D630DULL>(source);
  }
  case JT_MASTERING: {
    const char source[] = {'\x46', '\x7E', '\x9A', '\xDF', '\xA4', '\x8D', '\x44', '\x11', '\x4C', '\x1F'};
    return DecryptName<0x7F2DFFC1ABE91F2BULL>(source);
  }
  case JT_SAVAGE: {
    const char source[] = {'\xFE', '\xE0', '\x67', '\x92', '\xD4', '\xCC', '\x69'};
    return DecryptName<0xFD69A9B3F311818DULL>(source);
  }
  case JT_GRAND_PECO: {
    const char source[] = {'\x76', '\x91', '\x32', '\xF7', '\xD5', '\x02', '\x05', '\x2E', '\x72', '\x8C', '\x53'};
    return DecryptName<0x4B755DB19953E311ULL>(source);
  }
  case JT_HIGH_ORC: {
    const char source[] = {'\x53', '\x8E', '\x66', '\x73', '\x9E', '\xCE', '\x59', '\xDE', '\x3B'};
    return DecryptName<0xBD2BA1C11B01E73BULL>(source);
  }
  case JT_LITTLE_ISIS: {
    const char source[] = {'\xF5', '\x64', '\x69', '\x67', '\xC1', '\xD6', '\x4A', '\xA4', '\xEA', '\x64', '\x6E', '\x33'};
    return DecryptName<0xED15938D333D2DB9ULL>(source);
  }
  case JT_ANGELING: {
    const char source[] = {'\x22', '\x57', '\x3C', '\x0E', '\x0B', '\x44', '\x1F', '\xAE', '\x43'};
    return DecryptName<0xC9712D676B5B3943ULL>(source);
  }
  case JT_DR_EGGRING: {
    const char source[] = {'\xCB', '\x7B', '\x56', '\x82', '\xA0', '\xAE', '\x1B', '\xFC', '\xC1', '\x6E', '\x09'};
    return DecryptName<0x9569C9C7E70909AFULL>(source);
  }
  case JT_CHOCO: {
    const char source[] = {'\xB0', '\x51', '\x00', '\x72', '\xC6', '\x1F'};
    return DecryptName<0xB19D1FA9116F39D3ULL>(source);
  }
  case JT_AM_MUT: {
    const char source[] = {'\xF8', '\x54', '\xA2', '\x5C', '\x20', '\xCF', '\x91'};
    return DecryptName<0x4191BB5531FD3999ULL>(source);
  }
  case JT_HYEGUN: {
    const char source[] = {'\x33', '\xCE', '\x04', '\x3E', '\x3A', '\x3F', '\x15'};
    return DecryptName<0xF715514F5961B75BULL>(source);
  }
  case JT_DR_LUNATIC: {
    const char source[] = {'\xA5', '\xEB', '\x92', '\x89', '\xF4', '\x43', '\x24', '\xAD', '\xA8', '\xFA', '\xCD'};
    return DecryptName<0xD9452D81E5CD99C1ULL>(source);
  }
  case JT_NINE_TAIL: {
    const char source[] = {'\x3B', '\x52', '\xEF', '\x92', '\x12', '\x9F', '\xCC', '\xC4', '\x39', '\x3B'};
    return DecryptName<0xADADEB4DF7813B55ULL>(source);
  }
  case JT_CAT_O_NINE_TAIL: {
    const char source[] = {'\x3A', '\x6C', '\xBF', '\xF0', '\x12', '\x3C', '\xFB', '\xD8', '\x37', '\x68', '\x94', '\xDB', '\x1C', '\x0A', '\xF9', '\xB1'};
    return DecryptName<0xB195637DAFCB0D59ULL>(source);
  }
  case JT_DIABOLIC2: {
    const char source[] = {'\x09', '\xD6', '\xB4', '\x45', '\x84', '\x25', '\xCE', '\xB4', '\x5F', '\xBF'};
    return DecryptName<0xD7A749EB27D5BF6DULL>(source);
  }
  case JT_DELETER_2: {
    const char source[] = {'\x59', '\x18', '\x39', '\x44', '\xB5', '\x10', '\xEB', '\x3A', '\x0F', '\x7D'};
    return DecryptName<0x659975C121557D3DULL>(source);
  }
  case JT_TEDDY_BEAR: {
    const char source[] = {'\x2D', '\x1A', '\xC3', '\xC5', '\x6E', '\x9A', '\x97', '\xDA', '\x38', '\x0D', '\xA7'};
    return DecryptName<0xBFF5C517A1A77F59ULL>(source);
  }
  case JT_GREMLIN: {
    const char source[] = {'\x40', '\xD7', '\xF6', '\xF2', '\x6D', '\x3A', '\x0F', '\xAD'};
    return DecryptName<0xAD6153019F93A527ULL>(source);
  }
  case JT_SCATLETON: {
    const char source[] = {'\x0C', '\x38', '\x14', '\xD1', '\xCB', '\x90', '\x95', '\xEA', '\x11', '\x5B'};
    return DecryptName<0x85E1F5A7A5755B7FULL>(source);
  }
  case JT_MUMMY: {
    const char source[] = {'\x30', '\x9E', '\x1E', '\x24', '\xE2', '\x49'};
    return DecryptName<0x0F61499B4973EB5DULL>(source);
  }
  case JT_WILOW: {
    const char source[] = {'\x26', '\x9E', '\x37', '\x18', '\xEA', '\xA1'};
    return DecryptName<0xB127A19D775BF751ULL>(source);
  }
  case JT_ROWEEN: {
    const char source[] = {'\xBB', '\x6E', '\x9A', '\x8A', '\x82', '\x07', '\x1F'};
    return DecryptName<0xA31F69E7EFED01C9ULL>(source);
  }
  case JT_HODREMLIN: {
    const char source[] = {'\x03', '\x24', '\xDF', '\x73', '\xE2', '\x02', '\xC5', '\xDE', '\x05', '\x4B'};
    return DecryptName<0xB7A96F8701BB4B6BULL>(source);
  }
  case JT_METALLER: {
    const char source[] = {'\x2C', '\xB4', '\x95', '\x82', '\x07', '\x25', '\xEC', '\x45', '\x41'};
    return DecryptName<0x3789496BE3E1D141ULL>(source);
  }
  case JT_ANCIENT_MUMMY: {
    const char source[] = {'\x76', '\xB7', '\x50', '\xC0', '\x1C', '\x89', '\xE3', '\x50', '\x7A', '\xAC', '\x5E', '\xC4', '\x00', '\xE7'};
    return DecryptName<0x0F97E779A933D917ULL>(source);
  }
  case JT_XM_TEDDY_BEAR: {
    const char source[] = {'\x0B', '\x4A', '\x12', '\x39', '\x9C', '\x0B', '\x61', '\xA8', '\x2C', '\x45', '\x28', '\x2C', '\x8B', '\x6F'};
    return DecryptName<0xD1056FF94D4D2773ULL>(source);
  }
  case JT_SWEETS_DROPS: {
    const char source[] = {'\x98', '\x28', '\x6C', '\x56', '\x0B', '\xD2', '\x6E', '\x27', '\x99', '\x30', '\x79', '\x40', '\x7F'};
    return DecryptName<0x4331A17F33095FEBULL>(source);
  }
  case JT_MOONLIGHT: {
    const char source[] = {'\xA6', '\x82', '\xDC', '\x45', '\xEB', '\x58', '\x2A', '\x2D', '\xBF', '\xED'};
    return DecryptName<0x454D31872BB3EDCBULL>(source);
  }
  case JT_PHREEONI: {
    const char source[] = {'\x71', '\x65', '\x35', '\x38', '\x88', '\xE2', '\x9B', '\xE8', '\x01'};
    return DecryptName<0x81F58DED5D470D01ULL>(source);
  }
  case JT_SKELION: {
    const char source[] = {'\x06', '\x54', '\x04', '\xC7', '\x46', '\x1E', '\x89', '\x5D'};
    return DecryptName<0x5DE7712FAB613F75ULL>(source);
  }
  case JT_POURING: {
    const char source[] = {'\xE1', '\x04', '\x1E', '\x6D', '\xEA', '\xA5', '\xA8', '\x41'};
    return DecryptName<0x41CFCB831F6B6B91ULL>(source);
  }
  case JT_MOONLIGHT2: {
    const char source[] = {'\x5E', '\x0C', '\x3A', '\x9F', '\x45', '\x5C', '\xB4', '\x21', '\x47', '\x63'};
    return DecryptName<0x49D33529F1556333ULL>(source);
  }
  case JT_PHREEONI2: {
    const char source[] = {'\xC5', '\xE7', '\x33', '\x40', '\x76', '\x92', '\x41', '\x10', '\xB5'};
    return DecryptName<0x792FFD1325418FB5ULL>(source);
  }
  case JT_BACSOJIN2: {
    const char source[] = {'\xDD', '\x08', '\x80', '\xBE', '\xEA', '\xED', '\x36', '\x9F', '\x8D', '\x69'};
    return DecryptName<0xF15F8785CDE369BFULL>(source);
  }
  case JT_ORK_HERO2: {
    const char source[] = {'\x16', '\xD7', '\xCC', '\x26', '\xBF', '\x6A', '\x6D', '\xAC', '\x6B', '\x85'};
    return DecryptName<0xE33F2FF779878559ULL>(source);
  }
  case JT_ALIZA: {
    const char source[] = {'\xCA', '\xD1', '\x3E', '\xD7', '\xA0', '\xDD'};
    return DecryptName<0x1767DDE18D779D8BULL>(source);
  }
  case JT_ALICEL: {
    const char source[] = {'\xA0', '\xCF', '\x98', '\xAE', '\x54', '\xCD', '\xBB'};
    return DecryptName<0xC1BB8111EDD183E1ULL>(source);
  }
  case JT_ALIOT: {
    const char source[] = {'\xD6', '\x85', '\x80', '\xD4', '\x99', '\xBB'};
    return DecryptName<0xC5B1BBCD9BC9C997ULL>(source);
  }
  case 20338:
  case JT_NIGHTMARE_TERROR_H: {
    const char source[] = {'\x5F', '\x90', '\xB4', '\x91', '\x69', '\x22', '\xAA', '\x73', '\x54', '\xA6', '\xA7', '\x9C', '\x6F', '\x3D', '\xA4', '\x73', '\x6E', '\x94', '\xB9', '\xF9'};
    return DecryptName<0x01CB4F1DF9D3F931ULL>(source);
  }
  case JT_WANDER_MAN_H: {
    const char source[] = {'\xA0', '\xF4', '\x71', '\xFF', '\x34', '\x37', '\xD8', '\x66', '\xB6', '\xFB', '\x40', '\xF6', '\x3B', '\x45'};
    return DecryptName<0x0B8745519B1F95D7ULL>(source);
  }
  case JT_GLOOMUNDERNIGHT2: {
    const char source[] = {'\xE8', '\x35', '\x0E', '\x84', '\xD8', '\x6C', '\x6D', '\xA3', '\xEA', '\x2B', '\x0F', '\x82', '\xD2', '\x71', '\x77', '\xF5', '\x8F'};
    return DecryptName<0xC70319B5EB61598FULL>(source);
  }
  case JT_EP17_2_CHILD_ADMIN1: {
    const char source[] = {'\xFC', '\xB5', '\xEA', '\x73', '\x13', '\x1A', '\x7A', '\x43', '\xF2', '\xB4', '\xED', '\x40', '\x15', '\x45'};
    return DecryptName<0x271B45771F83DD9FULL>(source);
  }
  case JT_EP17_2_CHILD_ADMIN2: {
    const char source[] = {'\x74', '\x39', '\x88', '\x2D', '\x5D', '\x9C', '\xE2', '\xE3', '\x7A', '\x38', '\x8F', '\x1E', '\x58', '\xC3'};
    return DecryptName<0x8783C33941E15117ULL>(source);
  }
  case JT_WANDERING_DUCK: {
    const char source[] = {'\xFC', '\xEE', '\xC3', '\xC9', '\xF6', '\xDF', '\xEE', '\x9B', '\xEC', '\xF0', '\xC9', '\xD8', '\xF0', '\xC6', '\xA7'};
    return DecryptName<0xD5A78DB38D8DAFABULL>(source);
  }
  case JT_KIEL_D_01_2: {
    const char source[] = {'\x7F', '\x2A', '\x31', '\xF0', '\xF6', '\x2C', '\xFC', '\x6F', '\x0F'};
    return DecryptName<0x0399459DAF454F0FULL>(source);
  }
  case 21291: {
    const char source[] = {'\x10', '\x8E', '\xCB', '\xBE', '\xBA', '\xBC', '\xDF', '\x88', '\x73'};
    return DecryptName<0xFFABE3D5D1BBEF73ULL>(source);
  }
  case JT_DESERT_WOLF: {
    const char source[] = {'\x91', '\x7C', '\x78', '\x0C', '\xB5', '\xF3', '\xC8', '\xF2', '\x9A', '\x75', '\x6D', '\x49'};
    return DecryptName<0xA597A7E7492B39D5ULL>(source);
  }
  case JT_DARK_PRIEST: {
    const char source[] = {'\xC9', '\xFC', '\xE9', '\x28', '\xE8', '\x69', '\xE9', '\x18', '\xC8', '\xEE', '\xEF', '\x63'};
    return DecryptName<0x51BB39B763BBBD8DULL>(source);
  }
  case JT_FIRE_GOLEM: {
    const char source[] = {'\xA9', '\x64', '\x05', '\x48', '\x9A', '\x96', '\xAC', '\xCB', '\xAA', '\x60', '\x57'};
    return DecryptName<0x87E3D1C50D572DEFULL>(source);
  }
  case JT_ILL_BAPHOMET_J: {
    const char source[] = {'\xB6', '\xAF', '\x11', '\x5C', '\xE5', '\xEE', '\xF3', '\x77', '\xB0', '\xAE', '\x18', '\x57', '\xF8', '\xE5', '\xA3'};
    return DecryptName<0x3FA3AFA7035DE3FFULL>(source);
  }
  case JT_BAPHOMET2: {
    const char source[] = {'\x1B', '\xC6', '\x53', '\x35', '\x1A', '\x86', '\x62', '\x8F', '\x6B', '\x87'};
    return DecryptName<0xDB27CB557D038759ULL>(source);
  }
  case JT_DARK_ILLUSION2: {
    const char source[] = {'\x35', '\xC2', '\x39', '\x40', '\x4E', '\x46', '\x11', '\x31', '\x24', '\xD0', '\x22', '\x44', '\x5F', '\x3D', '\x5D'};
    return DecryptName<0x7D5D0F110B6B8371ULL>(source);
  }
  case JT_WILD_ROSE3: {
    const char source[] = {'\x1E', '\x24', '\x33', '\xA7', '\x98', '\x53', '\x06', '\x42', '\x0C', '\x5E', '\x7F'};
    return DecryptName<0x114901C7E37F6D49ULL>(source);
  }
  case JT_BLUEMOON_LOLI_RURI2: {
    const char source[] = {'\x13', '\xB9', '\x9A', '\x0C', '\xB2', '\x58', '\xCE', '\x91', '\x0E', '\xB9', '\x80', '\x05', '\xB6', '\x48', '\xD3', '\x8A', '\x03', '\xBC', '\xFD', '\x49'};
    return DecryptName<0xDF8117FF49CFF551ULL>(source);
  }
  case JT_FARMILIAR2: {
    const char source[] = {'\x7B', '\x00', '\xD9', '\x84', '\xB0', '\x57', '\x8C', '\xEC', '\x6F', '\x73', '\x8B'};
    return DecryptName<0xADC51BF9C98B413DULL>(source);
  }
  case JT_FARMILIAR: {
    const char source[] = {'\x07', '\x7C', '\x05', '\x38', '\x70', '\xD3', '\x04', '\x26', '\x13', '\x3D'};
    return DecryptName<0x674D9F3975573D41ULL>(source);
  }
  case JT_CWHITEKNIGHT: {
    const char source[] = {'\x12', '\xFA', '\xB5', '\x62', '\x1B', '\x76', '\x94', '\x25', '\x18', '\xEA', '\xB5', '\x7F', '\x4F'};
    return DecryptName<0x6BDF334F2BFDAD51ULL>(source);
  }
  case JT_KNIGHT_OF_ABYSS2: {
    const char source[] = {'\x84', '\x95', '\x98', '\x52', '\xD1', '\x7B', '\x30', '\x5C', '\x89', '\x84', '\x90', '\x57', '\xC0', '\x7C', '\x3C', '\x21', '\xCF'};
    return DecryptName<0x136F2F9915D1DBCFULL>(source);
  }
  case JT_GH_WHITEKNIGHT: {
    const char source[] = {'\x6C', '\x9F', '\x16', '\x78', '\x47', '\x98', '\x77', '\x4E', '\x60', '\x99', '\x00', '\x68', '\x47', '\x85', '\x23'};
    return DecryptName<0x0B23D10F2F49D72BULL>(source);
  }
  case JT_EDDGA3: {
    const char source[] = {'\xA6', '\x91', '\x17', '\xB0', '\xE4', '\x34', '\xE3'};
    return DecryptName<0xB3E307A5F753D5E3ULL>(source);
  }
  case 23009: {
    const char source[] = {'\xB5', '\x32', '\x67', '\x68', '\x14', '\x24', '\x72', '\x4B', '\xBA', '\x2B', '\x76', '\x7D', '\x4D'};
    return DecryptName<0x192D654D29337BE5ULL>(source);
  }
  case JT_LARVA_VIOLET: {
    const char source[] = {'\x4D', '\x34', '\xAF', '\x8F', '\x6E', '\xE2', '\xF9', '\xFE', '\x4E', '\x39', '\xB8', '\x8D', '\x2F'};
    return DecryptName<0xB7AFBD2FD9FD7501ULL>(source);
  }
  case JT_EP20_COPO: {
    const char source[] = {'\xB8', '\x25', '\x5D', '\x9F', '\xB6', '\x76', '\x86', '\x09', '\xB2', '\x55'};
    return DecryptName<0x79E915E9AF6F55DDULL>(source);
  }
  case JT_EP20_ICEWIND1: {
    const char source[] = {'\x40', '\xE1', '\x35', '\x93', '\xBE', '\x68', '\x50', '\x0E', '\x52', '\xF8', '\x69', '\xC7', '\xD0', '\x01'};
    return DecryptName<0x6B3301E1A3079125ULL>(source);
  }
  case JT_EP20_ICEWIND2: {
    const char source[] = {'\xEC', '\x4D', '\x5F', '\xD5', '\xF8', '\x68', '\x44', '\x4A', '\xFE', '\x54', '\x03', '\x81', '\x95', '\x01'};
    return DecryptName<0x2F2701A7E56D3D89ULL>(source);
  }
  case JT_DARK_LORD2: {
    const char source[] = {'\x27', '\x78', '\x2F', '\xEA', '\x06', '\xF9', '\xD0', '\x91', '\x27', '\x0B', '\x7D'};
    return DecryptName<0xC39FB559A17D3963ULL>(source);
  }
  case 23215: {
    const char source[] = {'\xFF', '\xE8', '\x91', '\x76', '\x4E', '\x28', '\xE8', '\x33', '\xF0', '\xF1', '\x80', '\x63', '\x17'};
    return DecryptName<0x71B7691737C5A1AFULL>(source);
  }
  case 23216: {
    const char source[] = {'\xE3', '\x00', '\xCC', '\xE2', '\xCB', '\x68', '\x45', '\x48', '\xF4', '\x1F', '\xDA', '\xF7', '\x85'};
    return DecryptName<0x07112F85A39F4FABULL>(source);
  }
  case 23233: {
    const char source[] = {'\x02', '\x82', '\xD4', '\xAC', '\x6A', '\xAE', '\xB3', '\x00', '\x03', '\xF1'};
    return DecryptName<0x45E3F103CBB5F157ULL>(source);
  }
  case 23234: {
    const char source[] = {'\xA5', '\xEE', '\x8E', '\x96', '\x4A', '\x62', '\x3F', '\xD6', '\xA1', '\x87'};
    return DecryptName<0x936F3D2FFDFD87F5ULL>(source);
  }
  case JT_VR_CHAO: {
    const char source[] = {'\x89', '\x15', '\xF2', '\x24', '\x57', '\x38', '\x7C', '\x2D'};
    return DecryptName<0x2D33791F67AD47DFULL>(source);
  }
  case JT_EP21_ICESEAHORSE: {
    const char source[] = {'\xBA', '\xEB', '\x0D', '\xC8', '\xDA', '\x70', '\x24', '\xC2', '\xAC', '\xFE', '\x5E', '\x91', '\xEA', '\x6B', '\x34', '\xC2', '\xDF'};
    return DecryptName<0xA7471985F93F9BDFULL>(source);
  }
  case JT_CLB_SS_FC: {
    const char source[] = {'\x14', '\xA3', '\x93', '\xEA', '\xF8', '\x2C', '\xA2', '\x19', '\x14', '\xEF'};
    return DecryptName<0x5FFD7FABB5D1EF57ULL>(source);
  }
  case JT_CLB_SS_TZ: {
    const char source[] = {'\x92', '\x2D', '\xB3', '\xD6', '\x3C', '\x02', '\xBA', '\x87', '\x8B', '\x61'};
    return DecryptName<0xD3E5516F89F161D1ULL>(source);
  }
  case JT_HORNET: {
    const char source[] = {'\xC5', '\x7C', '\x0D', '\x7D', '\x04', '\x91', '\x35'};
    return DecryptName<0xF535E561137F138DULL>(source);
  }
  case JT_GIANT_HONET2: {
    const char source[] = {'\x4A', '\x12', '\x28', '\xCD', '\xEF', '\x4E', '\xD5', '\xC6', '\x43', '\x1E', '\x3D', '\x83'};
    return DecryptName<0x899D11BB83695B0DULL>(source);
  }
  case JT_MISTRESS3: {
    const char source[] = {'\xB8', '\x6C', '\x00', '\xBD', '\x85', '\x60', '\xD4', '\x78', '\xC7', '\x25'};
    return DecryptName<0x2B8725D7E95325F5ULL>(source);
  }
  case JT_CLOCK: {
    const char source[] = {'\xFE', '\xF1', '\x54', '\x84', '\x66', '\x83'};
    return DecryptName<0x7989830DE73B9DBDULL>(source);
  }
  case JT_C_TOWER_MANAGER2: {
    const char source[] = {'\x28', '\x54', '\xF1', '\xAA', '\xF0', '\xB8', '\xDF', '\xE2', '\x26', '\x6A', '\xCB', '\xA4', '\xE0', '\xB8', '\xDF', '\xBD'};
    return DecryptName<0xBDADDD87C5A50B6BULL>(source);
  }
  case JT_SKEGGIOLD: {
    const char source[] = {'\x18', '\x98', '\x32', '\xC2', '\xB6', '\x24', '\xB4', '\x8B', '\x0F', '\xD3'};
    return DecryptName<0xC7FB6DF18577D34BULL>(source);
  }
  case JT_ANGELGOLT3: {
    const char source[] = {'\xBC', '\x1F', '\xF2', '\x1C', '\x4D', '\x04', '\x70', '\xF1', '\xA9', '\x51'};
    return DecryptName<0xBD3F430159B551FDULL>(source);
  }
  case 23434: {
    const char source[] = {'\x45', '\xFC', '\x6C', '\x1C', '\xD3', '\x34', '\x12', '\x65', '\x01'};
    return DecryptName<0x2B477F9B5B39B301ULL>(source);
  }
  case 23435: {
    const char source[] = {'\xD1', '\x24', '\xEA', '\xF4', '\x3E', '\x6B', '\xC2', '\x0E', '\xC4', '\x24', '\xA7'};
    return DecryptName<0x5B832361BDA77197ULL>(source);
  }
  case JT_TIMEHOLDER3: {
    const char source[] = {'\x2D', '\xA2', '\x6C', '\xDA', '\x75', '\xFC', '\xB9', '\x49', '\x3C', '\xB9', '\x13', '\x9F'};
    return DecryptName<0x0DF5B33D9F21EB79ULL>(source);
  }
  default: {
    const char source[] = {'\xAF', '\x32', '\x51', '\xF4', '\xE3', '\xEE', '\xE7'};
    return DecryptName<0x01E7898D9D235DDFULL>(source);
  }
  }
}
