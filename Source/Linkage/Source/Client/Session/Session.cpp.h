#pragma once

#include "Client/Session/Session.h"
#include "Client/Mode/ModeMessage.h"
#include "Contents/Actors/ActorEnums.h"
#include "UI/Window/UIEnums.h"
#include "UI/Window/WindowId.h"
#include "Contents/CombatTypes.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Client/Mode/CGameMode.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Effects/Core/EffectActor.h"
#include "Contents/Actors/Resources/EquipmentResourceMgr.h"
#include "Contents/Guild/GuildInfo.h"
#include "Contents/Guild/GuildMgr.h"
#include "Client/Mode/ModeMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Contents/Skills/SkillId.h"
#include "Contents/World/World.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/Pc.h"
#include "Contents/Effects/Core/RagEffect.h"
#include "Contents/Skills/ClientSkills.h"
#include "UI/Login/UINewSelectCharWnd.h"
#include "Contents/Messenger/ClientMessenger.h"
#include "File/File.h"
#include "Network/Connection/Connection.h"
#include "Network/Packets/PacketId.h"
#include "Network/TextEncoding.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "UI/Status/UIBasicInfoWnd.h"
#include "UI/Window/UIWindowMgr.h"
#include "UI/Window/UIWindow.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/Configuration/ServiceConfiguration.h"
#include "Client/Application/ErrorDialog.h"
#include "Contents/Tables/MessageTable.h"
#include "Contents/Actors/PlayerNameDictionary.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/XorBytes.h"
#include "Script/LuaScript.h"
#include "Contents/Map/MapPropertyMgr.h"
#include "Contents/Tables/ForbiddenWords.h"
#include "Contents/Items/ItemNaming.h"
#include "Contents/Skills/SkillInfo.h"
#include "Client/Application/ApplicationWindow.h"
#include "Contents/CashEmotion/CashEmotionMgr.h"
#include "Contents/Jobs/JobInheritance.h"
#include "Contents/Jobs/JobNames.h"
#include "Contents/Tables/SimpleStringMgr.h"
#include "File/CommonObject.h"
#include "Contents/World/WorldViewData.h"
#include "UI/Navigation/UIRoMapWnd.h"
#include "Contents/Achievement/AchievementMgr.h"
#include "Contents/Events/CheckAttendanceMgr.h"
#include "Contents/Items/BarterMarketItemMgr.h"
#include "Contents/Items/ChangeMaterialMgr.h"
#include "Contents/Items/ItemInfoMgr.h"
#include "Contents/RuneSystem/RuneSystem.h"
#include "Contents/Map/MapInfoMgr.h"
#include "Contents/Pet/PetEvolutionMgr.h"
#include "Contents/Map/PrivateAirplaneMgr.h"
#include "Contents/Events/SpecialPopupMgr.h"
#include "Resource/MSGSTRINGID.h"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <initializer_list>
#include <mbstring.h>
#include <list>
#include <string>
#include <vector>
#include <windows.h>
#include <timeapi.h>
#include "UI/GameSettings/TalkType.h"
#include "Contents/Configuration/ServiceEnums.h"
#include "Contents/Effects/Core/EffectTypes.h"
#include "Contents/Actors/Resources/BodyResources.h"
#include "Contents/Messenger/EquipmentPropertiesMgr.h"
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"
#include "Contents/Messenger/MessengerWindows.h"
#include "UI/Items/UIEquipWnd.h"
#include "UI/Items/UIItemStoreWnd.h"
#include "UI/Shop/UIMerchantItemWnd.h"
#include "UI/Shop/UIMerchantMirrorItemWnd.h"
#include "UI/Skill/UINewSkillListWnd.h"
#include "UI/Status/UIHomunInfoWnd.h"
#include "UI/Status/UIMerInfoWnd.h"

template __declspec(noinline) int std::_Traits_compare<std::char_traits<char>>(const char* const, const std::size_t, const char* const, const std::size_t) noexcept;
using SessionShopTree = std::_Tree<std::_Tmap_traits<
  int, SessionShop, std::less<int>, std::allocator<std::pair<const int, SessionShop>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, SessionShop>, void*>*>
SessionShopTree::_Find_lower_bound<int>(const int&) const;
using NameToIdTree = std::_Tree<std::_Tmap_traits<
  std::string, int, std::less<std::string>, std::allocator<std::pair<const std::string, int>>, false>>;
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::pair<const std::string, int>, void*>*>
NameToIdTree::_Find_lower_bound<std::string>(const std::string&) const;
using ShopEntryTree = std::_Tree<std::_Tmap_traits<
  int, std::pair<int, int>, std::less<int>, std::allocator<std::pair<const int, std::pair<int, int>>>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::pair<const int, std::pair<int, int>>, void*>*>
ShopEntryTree::_Find_lower_bound<int>(const int&) const;
using ActorIdTree = std::_Tree<std::_Tset_traits<
  std::uint32_t, std::less<std::uint32_t>, std::allocator<std::uint32_t>, false>>;
extern template std::_Tree_find_result<std::_Tree_node<std::uint32_t, void*>*>
ActorIdTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;
extern template void std::vector<std::string>::resize(size_type, const std::string&);
extern template void std::vector<unsigned char>::_Tidy() noexcept;
extern template std::string& std::string::operator=(std::string&&) noexcept;
using PendingActorNameTree = std::_Tree<std::_Tset_traits<
  std::uint32_t, std::less<std::uint32_t>, std::allocator<std::uint32_t>, false>>;
template __declspec(noinline) std::_Tree_find_result<std::_Tree_node<std::uint32_t, void*>*>
PendingActorNameTree::_Find_lower_bound<std::uint32_t>(const std::uint32_t&) const;
template __declspec(noinline) void std::vector<SearchStoreResult>::_Destroy(SearchStoreResult*, SearchStoreResult*);
template __forceinline void std::_Tree<std::_Tmap_traits<
  int, SessionRecord6144, std::less<int>, std::allocator<std::pair<const int, SessionRecord6144>>, false>>::clear() noexcept;
template __declspec(noinline) std::pair<std::string, std::vector<std::string>>::pair(std::string&, std::vector<std::string>&);
template __declspec(noinline) std::_Tree_node<std::pair<const int, float>, void*>*
std::allocator<std::_Tree_node<std::pair<const int, float>, void*>>::allocate(const std::size_t);
template __declspec(noinline) std::_Tree_node<std::pair<const int, MapPosEntry>, void*>*
std::allocator<std::_Tree_node<std::pair<const int, MapPosEntry>, void*>>::allocate(const std::size_t);
template __declspec(noinline) std::_Tree_node<std::pair<const int, RobeAlpha>, void*>*
std::allocator<std::_Tree_node<std::pair<const int, RobeAlpha>, void*>>::allocate(const std::size_t);
template __declspec(noinline) std::_Tree_node<std::pair<const std::string, int>, void*>*
std::allocator<std::_Tree_node<std::pair<const std::string, int>, void*>>::allocate(const std::size_t);
template __declspec(noinline) std::_Tree_node<std::pair<const int, SessionShop>, void*>*
std::allocator<std::_Tree_node<std::pair<const int, SessionShop>, void*>>::allocate(const std::size_t);
template __declspec(noinline) std::map<std::string, MapPosEntry>::map();
template __declspec(noinline) const char* EncryptedName<14, 0x072D2D872D259D5BULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x9BE3EF737387AD79ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xB96B77B7255F8569ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x11CB35636377DD07ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x4755C1355575FD37ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x9D8F2D37BFFD61B7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x79DF094DD9616575ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x038535BB092533B7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xFB2BC75769E777D3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xB5D7B5AF4FEDFF57ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x9B9D51AD3515F779ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xB5F3F779237315F3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x67DD5BAB1919D9D1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x91493751EF33B5FDULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x6BC977B3B5993503ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xB7A76BF9CD41F99FULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x732B390D63FD47D7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x1FDB81175D1557E3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xE3FB23537D2785BFULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x8BBF792B47E72173ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xB3CF9BEB7963DB89ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x71A509DB43792991ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xDD69EDFFA18F537DULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xFFBF43D1FFD5FF49ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xEB3D4FAB0FC183DBULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x8F3F2FA959977F9FULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xE51B29334D353B81ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x8B3FA35B5BDDDF89ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x7FE563BD3FB7F925ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x5D6B3B1DEB2397B1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x7DB1050779034DE7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0x01E7898D9D235DDFULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x0DF5B33D9F21EB79ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x9DEFC105A1EB2F71ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x6961FF056999A303ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0xBD3F430159B551FDULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xC7FB6DF18577D34BULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x5B832361BDA77197ULL>::Text();
template __declspec(noinline) const char* EncryptedName<34, 0x2B477F9B5B39B301ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x2B8725D7E95325F5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x899D11BB83695B0DULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xBDADDD87C5A50B6BULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x7989830DE73B9DBDULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xA7471985F93F9BDFULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xF535E561137F138DULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xD3E5516F89F161D1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x45E3F103CBB5F157ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x07112F85A39F4FABULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x2D33791F67AD47DFULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x936F3D2FFDFD87F5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x2F2701A7E56D3D89ULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x6B3301E1A3079125ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0x71B7691737C5A1AFULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xC39FB559A17D3963ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xFF7D83CB791339D9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xED5143E7C1B39315ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x393D916561BDC391ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x31956551B311FD95ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x8741152D8FB34B4FULL>::Text();
template __declspec(noinline) const char* EncryptedName<34, 0x955D4DF94DE9BB07ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xC399B7091589B50BULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0xB739073FE921AD6FULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x17FFC98B398FAF61ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x9B5B959B954D9333ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xAFA5617F178B6185ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x078143534999254DULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x3F5315DFE32B4FAFULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xE50B2935073DD3E9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x7D750DADBDE9C3F1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x33832D4D3FDFBDBFULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x1BF915E1F37DFDFDULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x01FF9313695DD9DFULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x6DED3DD1CF75AD9BULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x49C3759B77DBD3C1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0xD1538513DF975701ULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x91E147EBBD47C39FULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x4F1791FB1503C713ULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0xA1A55B4D1597EDD3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x0753A7E1EFA9E139ULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x7FC1015DFFCF6D19ULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0xCB3FBD4183DB8323ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x7FCD77F9C7CBD14DULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x5B8F49AF09390D1BULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x3B915119EFD523EFULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x474DC581D5E98357ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x3F557BE333F95F6DULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x8B3FEB5B8BD5C3C3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x0BEB63A5C905B3A7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x078333BB9725B7C9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x8F1B5BFD5F6175B5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x69FD1D9733D3CFC1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xF5657D1F3DE5CDC7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0x8B65CD61012B5931ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0xF7530137154D8FE5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0x61FF87ED7BF39BE1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x7D993F650D2785FFULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x2D394DBD0F332733ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0x151F77EBEBBB2147ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0xC7912587FFF31B71ULL>::Text();
template __declspec(noinline) const char* EncryptedName<34, 0x69D345DB950F336DULL>::Text();
template __declspec(noinline) const char* EncryptedName<34, 0x9B17C9EFDBB39F2FULL>::Text();
template __declspec(noinline) const char* EncryptedName<34, 0x2D3B15774149D5DDULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xD98F03A52D35636DULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x0F0F8B6B8F9D19D1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xD3755DD95F47A909ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x31D7C1EDA345E36DULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x3FAF85D995034FD7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x490FFB43A12F0DB9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x91651D81F559F971ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x73D1674B61DF7B0BULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x2FF3C5036D070127ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x17359BF953C367B3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xC52FB5FBB94FF917ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x93E7A3A1F587CDC5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x59278F63D79F3DA1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x812BE72BCD5707B7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x3375B1C7DBB5FD55ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x1D6D75EDE7CD311FULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xCBD1F3239BBBEDF7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x7DBBD74FA36F3BCBULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0x4917EB6D71B701ABULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xFD4F85BBA7E37D4FULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x51C57BB9F1DD9B0FULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xC733B7A333F9F571ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x7D61612937C1592DULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x8F4B518D839D1581ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xCBD7DDAFDF87F549ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x39E7F13755252F8FULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x3D075BD5256F9B6FULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xEB074B7FBDA3BFD5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x03014749B127914FULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x251761D7370BA5C3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x2B034DCBB1812785ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x85FB93CF2F494179ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x894FEDABAFCF45E3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x8B03E17DA967E943ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x190B8705C141D1CFULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xC98BB32777F94371ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xA5F51F3989DBB99FULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xB3D5C99B2B05FFEFULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xC71BB3CDEB499DD1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0x3939079FF3395587ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0xCF391B0F8B9D95DBULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x6967633B9D89092BULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xAFCD4F69FBDFE157ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0x85D9CFCF0FBB71ABULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0x795383DD11AF7DA7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0xD9FF511B2BB921E7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<16, 0x978B51F50DC5FD25ULL>::Text();
template __declspec(noinline) const char* EncryptedName<12, 0xC757D96143298511ULL>::Text();
template __declspec(noinline) const char* EncryptedName<16, 0xE1F181F923B759E7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<12, 0x37D76D8F495113C3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<20, 0x7BAD7791F50F5D8DULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x2F1337956DC3CFD5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<23, 0xC91F9D493F75FDA3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x1F01A13D8D915327ULL>::Text();
template __declspec(noinline) const char* EncryptedName<23, 0xF33D3D3B0BD9C53DULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0xC74BB3BB110FB10FULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x0B5F6D6B23D5AFA7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<16, 0x2B7BAB0B6971E3E9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<12, 0x8773593DB181D573ULL>::Text();
template __declspec(noinline) const char* EncryptedName<16, 0xFBDBDB13A77191BFULL>::Text();
template __declspec(noinline) const char* EncryptedName<12, 0x19BBC5910555FB97ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0xBBD983C7FD611FE3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0xA1ED494BF319772DULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x798B394FF5FF434DULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x410DD52D6731BFFDULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x07B733370F612D0FULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x113DC179F1C795C9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x31F5C32DD5C53779ULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x2F9B717365C1250FULL>::Text();
template __declspec(noinline) const char* EncryptedName<10, 0x4FE54379493FB5D5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<20, 0x656FAB972B3D21BBULL>::Text();
template __declspec(noinline) const char* EncryptedName<20, 0x33CD2BDD5B3927EDULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0xD97BB73901ED99BFULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x5D6B8FCBE3FB97A1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0xE5FDE3151BED831FULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x8971CB79C18D8B27ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x8DC5D10DAB1585BDULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x21156F4141E35DD9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x51EB99CBCB075F35ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x1B3B756FE399E1D9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x3B6D9BFD27A7C7F7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x451FD72783630709ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x73C32959FDFFA51BULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xA15B555BE9CD2B81ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xD9115915B1919533ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0xB3F1A10745C1F105ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x99157F159BB99BD9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<23, 0x7959FFBF652D07BFULL>::Text();
template __declspec(noinline) const char* EncryptedName<25, 0x01C9FBA7819FFBD7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0xABED6971354375C3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<29, 0x6BA557130BDFE997ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0xFBAD074BDB2B77A1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<25, 0x6D05D5271B5D65C3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x050DF16FC75B25B9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<25, 0x0789EBC9F5C1C59FULL>::Text();
template __declspec(noinline) const char* EncryptedName<22, 0x3D8387EF97A37DCFULL>::Text();
template __declspec(noinline) const char* EncryptedName<29, 0x07A7D947CF7323BFULL>::Text();
template __declspec(noinline) const char* EncryptedName<12, 0xD97DC927AFE911F1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<26, 0x8BA52B970DD531ADULL>::Text();
template __declspec(noinline) const char* EncryptedName<16, 0x47B7BF55EB471557ULL>::Text();
template __declspec(noinline) const char* EncryptedName<30, 0xCFB9671DEF4BD9E7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<35, 0x2F2103792383718BULL>::Text();
template __declspec(noinline) const char* EncryptedName<28, 0x999F5191852D2F6BULL>::Text();
template __declspec(noinline) const char* EncryptedName<42, 0x4349ADE96FEB27E5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<44, 0xA55B7BEF3D7377E3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<30, 0x31E3CF9D358D8705ULL>::Text();
template __declspec(noinline) const char* EncryptedName<44, 0x79A58B0F2173CF49ULL>::Text();
template __declspec(noinline) const char* EncryptedName<46, 0xF5496753BDE7C5BDULL>::Text();
template __declspec(noinline) const char* EncryptedName<26, 0x795787B7779D19DBULL>::Text();
template __declspec(noinline) const char* EncryptedName<33, 0x6B3917C939150B9BULL>::Text();
template __declspec(noinline) const char* EncryptedName<26, 0x29AB0B59651BDFD5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<33, 0xF9B1EF9F7961EFF5ULL>::Text();
template __declspec(noinline) const char* EncryptedName<24, 0xA1DFCDAD555BB16DULL>::Text();
template __declspec(noinline) const char* EncryptedName<25, 0xAFF9332FD397533FULL>::Text();
template __declspec(noinline) const char* EncryptedName<31, 0x194F8339AF753FADULL>::Text();
template __declspec(noinline) const char* EncryptedName<26, 0x7DC10333F96DE3E7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<33, 0xE9FBAB53D5A1BB17ULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x51372B6F4BA14B23ULL>::Text();
template __declspec(noinline) const char* EncryptedName<25, 0xB70319D3A78D39CDULL>::Text();
template __declspec(noinline) const char* EncryptedName<18, 0x1F79132B518FD389ULL>::Text();
template __declspec(noinline) const char* EncryptedName<25, 0xC1FD95811D1F4FA1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<24, 0x35F59F6F3DD1C965ULL>::Text();
template __declspec(noinline) const char* EncryptedName<31, 0xD7BBEB2DDB295D11ULL>::Text();
template __declspec(noinline) const char* EncryptedName<26, 0xBF0F412941076FADULL>::Text();
template __declspec(noinline) const char* EncryptedName<33, 0x8DFF0B4B67D9FDE1ULL>::Text();
template __declspec(noinline) const char* EncryptedName<30, 0xB59BADC5B365617DULL>::Text();
template __declspec(noinline) const char* EncryptedName<37, 0x77D925073D6F19B9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<12, 0x91E7A7DFA58F9B6BULL>::Text();
template __declspec(noinline) const char* EncryptedName<12, 0x2529F16B0375530DULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x91DF1F5D3B014D45ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x0D7FA94DD32361A9ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x2F371DDDADDD53CFULL>::Text();
template __declspec(noinline) const char* EncryptedName<6, 0x2F3D75DFCF678FF3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<6, 0x3BBB5F0F8F216F31ULL>::Text();
template __declspec(noinline) const char* EncryptedName<7, 0x6F9D21B96D71C1FFULL>::Text();
template __declspec(noinline) const char* EncryptedName<7, 0xED730315539785C7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<7, 0x8F61434F0F79DDD7ULL>::Text();
template __declspec(noinline) const char* EncryptedName<9, 0xA5C5D1A3B9457B53ULL>::Text();
template __declspec(noinline) const char* EncryptedName<11, 0x61F59D5D41E76519ULL>::Text();
template __declspec(noinline) const char* EncryptedName<13, 0x4B670715D543C1C3ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x7155E5EF0DFB0D03ULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x4B1D078937DB892BULL>::Text();
template __declspec(noinline) const char* EncryptedName<14, 0x5FFD7FABB5D1EF57ULL>::Text();
