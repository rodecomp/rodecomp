#pragma once

#include <algorithm>
#include <functional>
#include <memory>
#include <random>
#include <unordered_map>

#include "Archive/GPak.h"
#include "Archive/GPakEx.h"
#include "Archive/PakCrypter.h"
#include "Utility/AES.h"
#include <set>
#include <vector>
#include "Archive/GPakUpdate.h"

using PakDecryptCallable = bool (*)(std::string_view, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<const std::uint8_t>, std::span<std::uint8_t>);
using PakDecryptImpl = std::_Func_impl_no_alloc<
  PakDecryptCallable, bool, std::string_view, std::span<const std::uint8_t, 60>, std::span<const std::uint8_t, 16>, std::span<const std::uint8_t>, std::span<std::uint8_t>>;
using PakCrypterNode = std::_List_node<PakCrypterMap::value_type, void*>;
using PakCipherAliasMap = std::unordered_map<std::uint16_t, std::uint16_t>;
using PakAuxiliaryRandom = std::_Rng_from_urng<unsigned int, std::ranlux24>;
using PakRanluxBase = std::_Swc_base<unsigned int, 10, 24, std::_Swc_traits<unsigned int, 16777216, 24>>;
using PakAuxiliaryCipherGostDecryption = GPakEx::CipherModeFinal<
  GPakEx::GOST, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::DECRYPTION, GPakEx::GOST::Dec>, GPakEx::CBC_Decryption>;
using PakAuxiliaryCipherGostEncryption = GPakEx::CipherModeFinal<
  GPakEx::GOST, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::GOST::Enc>, GPakEx::CFB_Decryption>;
using PakAuxiliaryCipherThreeWay = GPakEx::CipherModeFinal<
  GPakEx::ThreeWay, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::ENCRYPTION, GPakEx::ThreeWay::Enc>, GPakEx::CFB_Decryption>;
using PakAuxiliaryCipherIdea = GPakEx::CipherModeFinal<
  GPakEx::IDEA, GPakEx::DECRYPTION, GPakEx::BlockCipherFinal<GPakEx::DECRYPTION, GPakEx::IDEA::Base>, GPakEx::CBC_Decryption>;

// OFFSET: 0x00654D10
// FUNCTION: fn_05051
template __declspec(noinline) void std::_Hash_vec<PakCrypterMap::_Mybase::_Aliter>::_Assign_grow(const size_type, const value_type);
// OFFSET: 0x00654E00
// FUNCTION: fn_05052
template __declspec(noinline) void std::_Hash_vec<PakDecryptFunctionMap::_Mybase::_Aliter>::_Assign_grow(const size_type, const value_type);
// OFFSET: 0x00654EF0
// FUNCTION: fn_05053
template __declspec(noinline) bool PakCrypterMap::_Mybase::_Check_rehash_required_1() const noexcept;
// OFFSET: 0x00654F60
// FUNCTION: fn_05055
template __declspec(noinline) void PakDecryptImpl::_Delete_this(bool) noexcept;
// OFFSET: 0x00654FB0
// FUNCTION: fn_05056
template __declspec(noinline) bool PakDecryptImpl::_Do_call(
  std::string_view&&, std::span<const std::uint8_t, 60>&&, std::span<const std::uint8_t, 16>&&, std::span<const std::uint8_t>&&, std::span<std::uint8_t>&&
);
// OFFSET: 0x00654FF0
// FUNCTION: fn_05057
template __declspec(noinline) void PakCrypterMap::_Mybase::_Forced_rehash(size_type);
// OFFSET: 0x00655190
// FUNCTION: fn_05058
template __declspec(noinline) void PakDecryptFunctionMap::_Mybase::_Forced_rehash(size_type);
// OFFSET: 0x00655330
// FUNCTION: fn_05059
template __declspec(noinline) const void* PakDecryptImpl::_Get() const noexcept;
// OFFSET: 0x00655340
// FUNCTION: fn_05060
template __declspec(noinline) PakCrypterNode* PakCrypterMap::_Mybase::_Insert_new_node_before(const std::size_t, PakCrypterNode* const, PakCrypterNode* const) noexcept;
// OFFSET: 0x006553C0
// FUNCTION: fn_05062
template __declspec(noinline) void PakCrypterMap::_Mybase::_Rehash_for_1();
// OFFSET: 0x00655460
// FUNCTION: fn_05064
template __declspec(noinline) const type_info& PakDecryptImpl::_Target_type() const noexcept;
// OFFSET: 0x00655470
// FUNCTION: fn_05065
template __declspec(noinline) PakDecryptFunctionMap::iterator PakDecryptFunctionMap::_Mybase::erase(const_iterator, const_iterator) noexcept;
// OFFSET: 0x00655690
// FUNCTION: fn_05066
template __declspec(noinline) PakCrypterNode* std::allocator<PakCrypterNode>::allocate(const std::size_t);
// OFFSET: 0x00655D70
// FUNCTION: fn_05069
template __declspec(noinline) std::_Hash_find_last_result<PakArchiveKeyMap::_Mybase::_Nodeptr> PakArchiveKeyMap::_Mybase::_Find_last<std::uint16_t>(
  const std::uint16_t&, const std::size_t
) const;
// OFFSET: 0x00655F00
// FUNCTION: fn_05070
template __declspec(noinline) void PakRanluxBase::_Seed<unsigned int>(unsigned int&, bool, std::true_type);
// OFFSET: 0x00656370
// FUNCTION: fn_05072
template __declspec(noinline) void std::fill<PakArchiveKeyMap::_Mybase::_Unchecked_iterator*, PakArchiveKeyMap::_Mybase::_Unchecked_iterator>(
  PakArchiveKeyMap::_Mybase::_Unchecked_iterator* const, PakArchiveKeyMap::_Mybase::_Unchecked_iterator* const, const PakArchiveKeyMap::_Mybase::_Unchecked_iterator&
);
// OFFSET: 0x006564D0
// FUNCTION: fn_45535
template __declspec(noinline) std::shared_ptr<PakAuxiliaryCipherGostDecryption> std::make_shared<PakAuxiliaryCipherGostDecryption>();
// OFFSET: 0x00656590
// FUNCTION: fn_45536
template __declspec(noinline) std::shared_ptr<PakAuxiliaryCipherGostEncryption> std::make_shared<PakAuxiliaryCipherGostEncryption>();
// OFFSET: 0x00656650
// FUNCTION: fn_45537
template __declspec(noinline) std::shared_ptr<PakAuxiliaryCipherThreeWay> std::make_shared<PakAuxiliaryCipherThreeWay>();
// OFFSET: 0x00656710
// FUNCTION: fn_45538
template __declspec(noinline) std::shared_ptr<PakAuxiliaryCipherIdea> std::make_shared<PakAuxiliaryCipherIdea>();
// OFFSET: 0x006567D0
// FUNCTION: fn_45539
template __declspec(noinline) std::shared_ptr<AES> std::make_shared<AES>();
// OFFSET: 0x00657170
// FUNCTION: fn_05084
template __declspec(noinline) void std::_Hash_vec<PakCipherAliasMap::_Mybase::_Aliter>::_Tidy() noexcept;
// OFFSET: 0x00657200
// FUNCTION: fn_05085
template __declspec(noinline) void std::_Hash_vec<PakArchiveKeyMap::_Mybase::_Aliter>::_Tidy() noexcept;
// OFFSET: 0x006572E0
// FUNCTION: fn_05086
template __declspec(noinline) std::_Ref_count_obj2<PakAuxiliaryCipherGostDecryption>::~_Ref_count_obj2() noexcept;
// OFFSET: 0x006572F0
// FUNCTION: fn_05087
template __declspec(noinline) std::_Ref_count_obj2<PakAuxiliaryCipherGostEncryption>::~_Ref_count_obj2() noexcept;
// OFFSET: 0x00657300
// FUNCTION: fn_05088
template __declspec(noinline) std::_Ref_count_obj2<PakAuxiliaryCipherThreeWay>::~_Ref_count_obj2() noexcept;
// OFFSET: 0x00657310
// FUNCTION: fn_05089
template __declspec(noinline) std::_Ref_count_obj2<PakAuxiliaryCipherIdea>::~_Ref_count_obj2() noexcept;
// OFFSET: 0x00657320
// FUNCTION: fn_05090
template __declspec(noinline) std::_Ref_count_obj2<AES>::~_Ref_count_obj2() noexcept;
// OFFSET: 0x00657A50
// FUNCTION: fn_05093
template __declspec(noinline) unsigned int PakAuxiliaryRandom::operator()(unsigned int);
// OFFSET: 0x00657BB0
// FUNCTION: fn_05094
template __declspec(noinline) unsigned int PakRanluxBase::operator()();
// OFFSET: 0x00657C30
// FUNCTION: fn_05095
template __declspec(noinline) unsigned int std::linear_congruential<unsigned int, 40014, 0, 2147483563>::operator()() noexcept;
// OFFSET: 0x00659FC0
// FUNCTION: fn_05119
template __declspec(noinline) void std::_Hash_vec<PakCipherAliasMap::_Mybase::_Aliter>::_Assign_grow(const size_type, const value_type);
// OFFSET: 0x0065A110
// FUNCTION: fn_05120
template __declspec(noinline) void std::_Hash_vec<PakArchiveKeyMap::_Mybase::_Aliter>::_Assign_grow(const size_type, const value_type);
// OFFSET: 0x0065A200
// FUNCTION: fn_05121
template __declspec(noinline) void std::_Ref_count_obj2<PakAuxiliaryCipherGostDecryption>::_Delete_this() noexcept;
// OFFSET: 0x0065A210
// FUNCTION: fn_05122
template __declspec(noinline) void std::_Ref_count_obj2<PakAuxiliaryCipherGostEncryption>::_Delete_this() noexcept;
// OFFSET: 0x0065A220
// FUNCTION: fn_05123
template __declspec(noinline) void std::_Ref_count_obj2<PakAuxiliaryCipherThreeWay>::_Delete_this() noexcept;
// OFFSET: 0x0065A230
// FUNCTION: fn_05124
template __declspec(noinline) void std::_Ref_count_obj2<PakAuxiliaryCipherIdea>::_Delete_this() noexcept;
// OFFSET: 0x0065A240
// FUNCTION: fn_05125
template __declspec(noinline) void std::_Ref_count_obj2<AES>::_Delete_this() noexcept;
// OFFSET: 0x0065A250
// FUNCTION: fn_05126
template __declspec(noinline) void std::_Ref_count_obj2<PakAuxiliaryCipherGostDecryption>::_Destroy() noexcept;
// OFFSET: 0x0065A260
// FUNCTION: fn_05127
template __declspec(noinline) void std::_Ref_count_obj2<PakAuxiliaryCipherGostEncryption>::_Destroy() noexcept;
// OFFSET: 0x0065A270
// FUNCTION: fn_05128
template __declspec(noinline) void std::_Ref_count_obj2<PakAuxiliaryCipherThreeWay>::_Destroy() noexcept;
// OFFSET: 0x0065A280
// FUNCTION: fn_05129
template __declspec(noinline) void std::_Ref_count_obj2<PakAuxiliaryCipherIdea>::_Destroy() noexcept;
// OFFSET: 0x0065A290
// FUNCTION: fn_05130
template __declspec(noinline) void std::_Ref_count_obj2<AES>::_Destroy() noexcept;
// OFFSET: 0x0065A2A0
// FUNCTION: fn_05131
template __declspec(noinline) void PakArchiveKeyMap::_Mybase::_Forced_rehash(size_type);
// OFFSET: 0x0065A440
// FUNCTION: fn_05132
template __declspec(noinline) unsigned int PakAuxiliaryRandom::_Get_all_bits();
// OFFSET: 0x00658E90
// FUNCTION: fn_05113
template __declspec(noinline) PakKeyMaterial PakObfuscated<PakKeyMaterial>::Unmask() const;



// OFFSET: 0x00AE9640
// FUNCTION: fn_18106
template __declspec(noinline) std::vector<myGPack::PakEntry>::~vector() noexcept;
// OFFSET: 0x00AECA10
// FUNCTION: fn_18138
template __declspec(noinline) myGPack::PakEntry* std::vector<myGPack::PakEntry>::_Ufill(myGPack::PakEntry*, const size_type, std::_Value_init_tag);
// OFFSET: 0x00E55780
// FUNCTION: fn_24307
template __declspec(noinline) std::set<myGPack::PakFreeBlock>::_Mybase::~_Tree() noexcept;
// OFFSET: 0x006468C0
// FUNCTION: fn_04883
template __declspec(noinline) void std::fill<unsigned int*, unsigned int>(unsigned int* const, unsigned int* const, const unsigned int&);
// OFFSET: 0x00646930
// FUNCTION: fn_04884
template __declspec(noinline) void std::fill<int*, int>(int* const, int* const, const int&);
// OFFSET: 0x006451C0
// FUNCTION: fn_04852
template __declspec(noinline) std::_Hash_find_last_result<PakCrypterMap::_Mybase::_Nodeptr> PakCrypterMap::_Mybase::_Find_last<std::uint16_t>(
  const std::uint16_t&, const std::size_t
) const;
// OFFSET: 0x00645230
// FUNCTION: fn_04853
template __declspec(noinline) std::_Hash_find_last_result<PakDecryptFunctionMap::_Mybase::_Nodeptr>
PakDecryptFunctionMap::_Mybase::_Find_last<std::uint16_t>(const std::uint16_t&, const std::size_t) const;
// OFFSET: 0x006452C0
// FUNCTION: fn_04854
template __declspec(noinline) void PakDecryptFunctionMap::_Mybase::_Node::_Free_non_head<PakDecryptFunctionMap::_Mybase::_Alnode>(
  PakDecryptFunctionMap::_Mybase::_Alnode&, PakDecryptFunctionMap::_Mybase::_Nodeptr
) noexcept;
// OFFSET: 0x0064A400
// FUNCTION: fn_04905
template __declspec(noinline) std::_Hash_vec<PakCrypterMap::_Mybase::_Aliter>::~_Hash_vec();
// OFFSET: 0x0064A490
// FUNCTION: fn_04906
template __declspec(noinline) std::_Hash_vec<PakDecryptFunctionMap::_Mybase::_Aliter>::~_Hash_vec();
// OFFSET: 0x0064A520
// FUNCTION: fn_04907
template __declspec(noinline) std::_List_node_emplace_op2<PakCrypterMap::_Mybase::_Alnode>::~_List_node_emplace_op2();
// OFFSET: 0x006453A0
// FUNCTION: fn_04856
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_AES_A<0x201, 2, 16, 32, GPakEx::IDEA, true, 0x33, 0, 2048>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_AES_A<0x201, 2, 16, 32, GPakEx::IDEA, true, 0x33, 0, 2048>*&);
// OFFSET: 0x00645510
// FUNCTION: fn_04858
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10A, GPakEx::TEA, GPakEx::OFB_Mode, 24, 32, GPakEx::IDEA, true, 0x31, 1024, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10A, GPakEx::TEA, GPakEx::OFB_Mode, 24, 32, GPakEx::IDEA, true, 0x31, 1024, 1024>*&);
// OFFSET: 0x00645680
// FUNCTION: fn_04860
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10B, GPakEx::TEA, GPakEx::CFB_Mode, 16, 32, GPakEx::IDEA, false, 0x34, 0, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10B, GPakEx::TEA, GPakEx::CFB_Mode, 16, 32, GPakEx::IDEA, false, 0x34, 0, 1024>*&);
// OFFSET: 0x006457F0
// FUNCTION: fn_04862
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10C, GPakEx::RC6, GPakEx::CFB_Mode, 28, 44, GPakEx::IDEA, false, 0x32, 1024, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10C, GPakEx::RC6, GPakEx::CFB_Mode, 28, 44, GPakEx::IDEA, false, 0x32, 1024, 1024>*&);
// OFFSET: 0x00645960
// FUNCTION: fn_04864
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10D, GPakEx::RC5, GPakEx::OFB_Mode, 0, 16, GPakEx::IDEA, true, 0x32, 2048, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10D, GPakEx::RC5, GPakEx::OFB_Mode, 0, 16, GPakEx::IDEA, true, 0x32, 2048, 1024>*&);
// OFFSET: 0x00645AD0
// FUNCTION: fn_04866
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10E, GPakEx::XTEA, GPakEx::OFB_Mode, 44, 24, GPakEx::IDEA, true, 0x31, 1024, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10E, GPakEx::XTEA, GPakEx::OFB_Mode, 44, 24, GPakEx::IDEA, true, 0x31, 1024, 1024>*&);
// OFFSET: 0x00645C40
// FUNCTION: fn_04868
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10F, GPakEx::XTEA, GPakEx::CFB_Mode, 44, 16, GPakEx::IDEA, false, 0x30, 1024, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x10F, GPakEx::XTEA, GPakEx::CFB_Mode, 44, 16, GPakEx::IDEA, false, 0x30, 1024, 1024>*&);
// OFFSET: 0x00645DB0
// FUNCTION: fn_04870
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x110, GPakEx::RC6, GPakEx::CFB_Mode, 0, 32, GPakEx::IDEA, true, 0x30, 0, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x110, GPakEx::RC6, GPakEx::CFB_Mode, 0, 32, GPakEx::IDEA, true, 0x30, 0, 1024>*&);
// OFFSET: 0x00645F20
// FUNCTION: fn_04872
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x111, GPakEx::XTEA, GPakEx::CFB_Mode, 44, 0, GPakEx::IDEA, false, 0x32, 0, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x111, GPakEx::XTEA, GPakEx::CFB_Mode, 44, 0, GPakEx::IDEA, false, 0x32, 0, 1024>*&);
// OFFSET: 0x00646090
// FUNCTION: fn_04874
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x112, GPakEx::RC6, GPakEx::OFB_Mode, 32, 16, GPakEx::IDEA, true, 0x32, 0, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x112, GPakEx::RC6, GPakEx::OFB_Mode, 32, 16, GPakEx::IDEA, true, 0x32, 0, 1024>*&);
// OFFSET: 0x00646200
// FUNCTION: fn_04876
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x113, GPakEx::RC6, GPakEx::OFB_Mode, 24, 32, GPakEx::IDEA, false, 0x32, 0, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x113, GPakEx::RC6, GPakEx::OFB_Mode, 24, 32, GPakEx::IDEA, false, 0x32, 0, 1024>*&);
// OFFSET: 0x00646370
// FUNCTION: fn_04878
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x202, GPakEx::ThreeWay, GPakEx::CBC_Mode, 16, 44, GPakEx::IDEA, false, 0x30, 1024, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x202, GPakEx::ThreeWay, GPakEx::CBC_Mode, 16, 44, GPakEx::IDEA, false, 0x30, 1024, 1024>*&);
// OFFSET: 0x006464E0
// FUNCTION: fn_04880
template __declspec(noinline) std::pair<PakCrypterMap::iterator, bool> PakCrypterMap::_Mybase::emplace<std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x203, GPakEx::ThreeWay, GPakEx::OFB_Mode, 24, 44, GPakEx::IDEA, true, 0x31, 1024, 1024>*&>(std::uint16_t&, GPakEx::Crypter_SUB_XOR_BlockCipher_A<0x203, GPakEx::ThreeWay, GPakEx::OFB_Mode, 24, 44, GPakEx::IDEA, true, 0x31, 1024, 1024>*&);
