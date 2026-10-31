#include "Contents/Effects/EZ2STREffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Effects/EffectLists.h"
#include "Contents/Effects/EffectMemoryMgr.h"
#include "Contents/Effects/EffectTypes.h"
#include "Contents/Skills/ClientSkills.h"
#include "Contents/Sound/SoundMgr.h"
#include "Contents/World/World.h"
#include "Render/Renderer.h"
#include "Render/ScreenRectangle.h"
#include "Render/TextureQuad.h"
#include "Script/LuaScript.h"
#include "Utility/Text/FormatString.h"

#include <string>

// OFFSET: 0x00B51810
// FUNCTION: fn_19030
CEZ2STRScreenEffect::CEZ2STRScreenEffect()
    : CEffect(-1), particle(nullptr), name("빨간포션.str"), fadePulse(0), overlayColor(0), followCamera(0), cameraAngle(0.0), jobScaled(0) {
}

// OFFSET: 0x00B51D60
// FUNCTION: fn_19036
float GetHatEffectOffsetX(int id, const char* name) {
  float offset = 0.0f;
  float result = 0.0f;

  switch (id) {
  case 0x40B:
  case 0x40C:
    offset = 5.0f;
    result = offset;
    break;
  case 0x43E:
    offset = 8.0f;
    result = offset;
    break;
  case 0x4C3:
    offset = 0.5f;
    result = offset;
    break;
  case 0x4CD:
  case 0x4CE:
  case 0x543:
  case 0x544:
  case 0x545:
  case 0x546:
  case 0x5CE:
  case 0x5CF:
    offset = 1.0f;
    result = offset;
    break;
  case 0x4F2:
  case 0x4F5:
    offset = -0.5f;
    result = offset;
    break;
  case 0x547:
  case 0x548:
  case 0x549:
  case 0x54A:
  case 0x5D0:
  case 0x5D1:
    offset = -1.0f;
    result = offset;
    break;
  }

  std::string text(name);
  if (text.find("strangelights") != std::string::npos) {
    offset = 5.0f;
    result = offset;
  } else if (text.find("superstar") != std::string::npos) {
    offset = 5.0f;
    result = offset;
  } else if (text.find("ljosalfar") != std::string::npos) {
    offset = 8.0f;
    result = offset;
  }

  if (id >= 0x9B5) {
    double position = 0.0;
    g_luaScript->Call("GetHatEfPosX", "d>l", id - 0x9B5, &position);
    result = static_cast<float>(offset + position);
  }

  return result;
}

// OFFSET: 0x00B52060
// FUNCTION: fn_19037
int GetHatEffectOffsetY(int id, CGameActor* actor) {
  bool ignored = false;
  if (id >= 0x9B5) {
    g_luaScript->Call("IsIgnoredRidingState_Include_AttachedToHead", "d>b", id - 0x9B5, &ignored);
  } else {
    ignored = id == 0x719;
  }

  int offset = 0;
  if (actor) {
    if (ignored == true) {
      offset = 18;
    } else {
      offset = static_cast<int>(actor->GetNameHeight());
    }
  }

  switch (id) {
  case 0x40B:
  case 0x45F:
  case 0x7D6:
    offset += 5;
    break;
  case 0x42C:
  case 0x43B:
  case 0x442:
  case 0x463:
  case 0x46E:
  case 0x470:
  case 0x472:
  case 0x473:
  case 0x955:
    offset -= 8;
    break;
  case 0x43C:
  case 0x7E2:
    offset -= 7;
    break;
  case 0x43D:
  case 0x441:
  case 0x443:
  case 0x447:
  case 0x4ED:
  case 0x552:
  case 0x75E:
  case 0x805:
  case 0x853:
    offset -= 5;
    break;
  case 0x44A:
  case 0x44E:
  case 0x44F:
  case 0x451:
  case 0x453:
  case 0x454:
  case 0x455:
  case 0x45C:
  case 0x765:
  case 0x766:
    offset -= 8;
    break;
  case 0x464:
  case 0x467:
    offset += 8;
    break;
  case 0x425:
  case 0x46D:
  case 0x476:
  case 0x4C2:
  case 0x4C5:
  case 0x63F:
  case 0x6C9:
  case 0x6CC:
  case 0x6D0:
  case 0x6F8:
  case 0x722:
  case 0x726:
  case 0x752:
  case 0x756:
  case 0x75A:
  case 0x77A:
  case 0x7B2:
  case 0x802:
  case 0x859:
  case 0x86C:
  case 0x870:
    offset -= 6;
    break;
  case 0x475:
  case 0x4C3:
  case 0x522:
  case 0x5C5:
  case 0x6D5:
  case 0x6DF:
  case 0x6E3:
  case 0x6EF:
  case 0x6F4:
  case 0x707:
  case 0x72E:
  case 0x742:
  case 0x74A:
  case 0x753:
  case 0x757:
  case 0x75B:
  case 0x76A:
  case 0x771:
  case 0x7AF:
  case 0x7B6:
  case 0x7CA:
  case 0x7D5:
  case 0x7EB:
  case 0x800:
  case 0x843:
  case 0x889:
  case 0x900:
  case 0x904:
  case 0x91E:
  case 0x921:
  case 0x923:
  case 0x924:
  case 0x926:
    offset -= 4;
    break;
  case 0x4C4:
  case 0x4D7:
  case 0x4DB:
    offset -= 10;
    break;
  case 0x4CC:
    offset -= 16;
    break;
  case 0x4CD:
  case 0x4CE:
  case 0x4D6:
  case 0x545:
  case 0x546:
  case 0x547:
  case 0x548:
  case 0x5CF:
  case 0x5D0:
  case 0x64B:
  case 0x64C:
  case 0x64D:
  case 0x64E:
  case 0x64F:
  case 0x650:
    offset -= 6;
    break;
  case 0x4EC:
  case 0x747:
  case 0x755:
  case 0x7A1:
  case 0x7E9:
  case 0x810:
  case 0x854:
  case 0x888:
  case 0x966:
  case 0x967:
  case 0x968:
    offset -= 2;
    break;
  case 0x4EE:
  case 0x4EF:
  case 0x4F0:
  case 0x4F1:
    offset -= 10;
    break;
  case 0x4F2:
  case 0x4F3:
  case 0x4F4:
  case 0x4F6:
  case 0x4F7:
  case 0x4F8:
  case 0x4F9:
  case 0x4FD:
  case 0x63B:
  case 0x63C:
  case 0x63D:
  case 0x63E:
  case 0x686:
  case 0x687:
  case 0x688:
  case 0x689:
  case 0x68A:
  case 0x6AE:
  case 0x6AF:
  case 0x6B0:
  case 0x6B1:
  case 0x6B2:
  case 0x6B3:
  case 0x6B4:
  case 0x6B5:
  case 0x6B6:
  case 0x6B7:
  case 0x6B8:
  case 0x6B9:
  case 0x6BA:
  case 0x6BB:
  case 0x6BC:
  case 0x6BD:
  case 0x6BE:
  case 0x6BF:
  case 0x6C0:
  case 0x6C1:
  case 0x6C2:
  case 0x6C3:
  case 0x6C4:
  case 0x6C5:
  case 0x6C6:
  case 0x6C7:
  case 0x6C8:
  case 0x6CA:
  case 0x6CB:
  case 0x6CD:
  case 0x6CE:
  case 0x6CF:
  case 0x6D1:
  case 0x6D2:
  case 0x6D3:
  case 0x6D4:
  case 0x6D9:
  case 0x6DA:
  case 0x6DD:
  case 0x6DE:
  case 0x6E1:
  case 0x6E2:
  case 0x6E9:
  case 0x6EA:
  case 0x6EB:
  case 0x6EC:
  case 0x6ED:
  case 0x6EE:
  case 0x6F0:
  case 0x6F1:
  case 0x6F2:
  case 0x6F3:
  case 0x701:
  case 0x702:
  case 0x703:
  case 0x704:
  case 0x705:
  case 0x709:
  case 0x70A:
  case 0x70B:
  case 0x70D:
  case 0x70E:
  case 0x70F:
  case 0x748:
  case 0x749:
  case 0x758:
  case 0x759:
  case 0x762:
  case 0x763:
  case 0x764:
  case 0x767:
  case 0x768:
  case 0x76B:
  case 0x76C:
  case 0x7B1:
  case 0x7B3:
  case 0x7C8:
  case 0x7C9:
  case 0x7D3:
  case 0x7D4:
  case 0x7DA:
  case 0x7DC:
  case 0x7DD:
  case 0x7E3:
  case 0x7E4:
  case 0x7E5:
  case 0x7E6:
  case 0x7E7:
  case 0x7E8:
  case 0x7EC:
  case 0x7ED:
  case 0x7EE:
  case 0x7EF:
  case 0x7F0:
  case 0x7F1:
  case 0x7F2:
  case 0x7F3:
  case 0x7F4:
  case 0x7F5:
  case 0x7F6:
  case 0x7F7:
  case 0x7F8:
  case 0x7F9:
  case 0x7FA:
  case 0x7FB:
  case 0x7FC:
  case 0x7FD:
  case 0x807:
  case 0x808:
  case 0x809:
  case 0x80A:
  case 0x80B:
  case 0x80C:
  case 0x80D:
  case 0x80E:
  case 0x80F:
  case 0x811:
  case 0x812:
  case 0x813:
  case 0x814:
  case 0x815:
  case 0x816:
  case 0x81B:
  case 0x81C:
  case 0x829:
  case 0x82A:
  case 0x832:
  case 0x833:
  case 0x834:
  case 0x835:
  case 0x836:
  case 0x837:
  case 0x838:
  case 0x839:
  case 0x83A:
  case 0x83B:
  case 0x83C:
  case 0x844:
  case 0x845:
  case 0x846:
  case 0x847:
  case 0x848:
  case 0x849:
  case 0x84A:
  case 0x84B:
  case 0x84C:
  case 0x84D:
  case 0x855:
  case 0x856:
  case 0x857:
  case 0x858:
  case 0x85A:
  case 0x85B:
  case 0x85C:
  case 0x85D:
  case 0x85E:
  case 0x85F:
  case 0x860:
  case 0x861:
  case 0x862:
  case 0x863:
  case 0x864:
  case 0x865:
  case 0x866:
  case 0x867:
  case 0x868:
  case 0x869:
  case 0x86D:
  case 0x86E:
  case 0x86F:
  case 0x871:
  case 0x872:
  case 0x873:
  case 0x874:
  case 0x876:
  case 0x877:
  case 0x878:
  case 0x879:
  case 0x87A:
  case 0x87B:
  case 0x87C:
  case 0x87D:
  case 0x87E:
  case 0x87F:
  case 0x880:
  case 0x881:
  case 0x882:
  case 0x883:
  case 0x884:
  case 0x885:
  case 0x886:
  case 0x88A:
  case 0x88B:
  case 0x88C:
  case 0x88D:
  case 0x88E:
  case 0x88F:
  case 0x890:
  case 0x891:
  case 0x892:
  case 0x894:
  case 0x895:
  case 0x896:
  case 0x897:
  case 0x898:
  case 0x899:
  case 0x89A:
  case 0x8BC:
  case 0x8BD:
  case 0x8BE:
  case 0x8BF:
  case 0x8C1:
  case 0x8C2:
  case 0x8C3:
  case 0x8C4:
  case 0x8C5:
  case 0x8C6:
  case 0x8C7:
  case 0x8C8:
  case 0x8CA:
  case 0x8CB:
  case 0x8CC:
  case 0x8CD:
  case 0x8D2:
  case 0x8D3:
  case 0x8D4:
  case 0x8D5:
  case 0x8D6:
  case 0x8D7:
  case 0x8D8:
  case 0x8D9:
  case 0x8DA:
  case 0x8DB:
  case 0x8DC:
  case 0x8DD:
  case 0x8DE:
  case 0x8DF:
  case 0x8E1:
  case 0x8E2:
  case 0x8E8:
    offset -= 2;
    break;
  case 0x543:
  case 0x544:
  case 0x549:
  case 0x54A:
  case 0x5CE:
  case 0x5D1:
  case 0x736:
  case 0x737:
  case 0x738:
  case 0x77F:
  case 0x782:
  case 0x79E:
  case 0x7A0:
  case 0x7A2:
  case 0x7A3:
  case 0x7D9:
  case 0x7DB:
  case 0x7E0:
  case 0x7E1:
  case 0x827:
  case 0x828:
  case 0x82B:
  case 0x82C:
  case 0x82D:
  case 0x913:
  case 0x914:
  case 0x915:
  case 0x916:
  case 0x917:
  case 0x918:
  case 0x919:
  case 0x91A:
  case 0x91B:
  case 0x91C:
  case 0x91D:
  case 0x91F:
  case 0x920:
  case 0x943:
  case 0x944:
  case 0x948:
  case 0x949:
  case 0x94A:
  case 0x94B:
  case 0x94C:
  case 0x94D:
  case 0x94F:
  case 0x950:
  case 0x951:
  case 0x952:
  case 0x953:
  case 0x954:
    offset -= 4;
    break;
  case 0x639:
  case 0x657:
  case 0x676:
  case 0x774:
    offset -= 3;
    break;
  case 0x672:
  case 0x673:
  case 0x6A7:
  case 0x6A8:
  case 0x6A9:
  case 0x71A:
  case 0x71B:
  case 0x72F:
  case 0x730:
  case 0x732:
  case 0x89C:
  case 0x89D:
  case 0x89F:
  case 0x8A0:
  case 0x8A1:
  case 0x8A2:
  case 0x8A3:
  case 0x8A4:
  case 0x8A5:
  case 0x8A7:
  case 0x8A8:
  case 0x8A9:
  case 0x8AA:
  case 0x8AB:
  case 0x8AC:
  case 0x8AD:
  case 0x8AF:
  case 0x8B0:
  case 0x8B1:
  case 0x8B2:
  case 0x8B3:
  case 0x8B4:
  case 0x8B5:
  case 0x8B7:
  case 0x8B8:
  case 0x8B9:
  case 0x8BA:
  case 0x8BB:
    offset -= 3;
    break;
  case 0x6F5:
  case 0x6F6:
  case 0x6F7:
  case 0x739:
  case 0x73A:
  case 0x73C:
  case 0x73D:
  case 0x7AC:
  case 0x7AD:
  case 0x7D0:
  case 0x7D1:
  case 0x7D2:
  case 0x851:
  case 0x852:
    --offset;
    break;
  case 0x788:
    ++offset;
    break;
  case 0x743:
    offset += 4;
    break;
  case 0x746:
  case 0x75F:
  case 0x7B4:
  case 0x7C3:
  case 0x7CF:
    --offset;
    break;
  case 0x803:
  case 0x806:
  case 0x81E:
  case 0x821:
  case 0x83E:
  case 0x83F:
    offset -= 5;
    break;
  case 0x7DE:
  case 0x7DF:
    ++offset;
    break;
  case 0x94E:
    offset += 16;
    break;
  }

  if (id >= 0x9B5) {
    int position = 0;
    g_luaScript->Call("GetHatEfPos", "d>d", id - 0x9B5, &position);
    offset += position;
  }

  return offset - 7;
}

// OFFSET: 0x00B56210
// FUNCTION: fn_19041
bool CEZ2STRScreenEffect::IsAttachedToHead() const {
  switch (this->id) {
  case 0x425:
  case 0x42C:
  case 0x442:
  case 0x446:
  case 0x44A:
  case 0x44E:
  case 0x44F:
  case 0x451:
  case 0x453:
  case 0x454:
  case 0x455:
  case 0x45C:
  case 0x45F:
  case 0x463:
  case 0x464:
  case 0x467:
  case 0x46D:
  case 0x46E:
  case 0x470:
  case 0x472:
  case 0x473:
  case 0x475:
  case 0x476:
  case 0x4C2:
  case 0x4C3:
  case 0x4C4:
  case 0x4C5:
  case 0x4CC:
  case 0x4CD:
  case 0x4CE:
  case 0x4D6:
  case 0x4D7:
  case 0x4DB:
  case 0x4EA:
  case 0x4EB:
  case 0x4EC:
  case 0x4ED:
  case 0x4EE:
  case 0x4EF:
  case 0x4F0:
  case 0x4F1:
  case 0x4F2:
  case 0x4F3:
  case 0x4F4:
  case 0x4F5:
  case 0x4F6:
  case 0x4F7:
  case 0x4F8:
  case 0x4F9:
  case 0x4FA:
  case 0x4FB:
  case 0x4FD:
  case 0x522:
  case 0x543:
  case 0x544:
  case 0x545:
  case 0x546:
  case 0x547:
  case 0x548:
  case 0x549:
  case 0x54A:
  case 0x552:
  case 0x590:
  case 0x591:
  case 0x592:
  case 0x593:
  case 0x5C5:
  case 0x5CE:
  case 0x5CF:
  case 0x5D0:
  case 0x5D1:
  case 0x62D:
  case 0x62E:
  case 0x62F:
  case 0x631:
  case 0x632:
  case 0x633:
  case 0x634:
  case 0x635:
  case 0x636:
  case 0x637:
  case 0x638:
  case 0x639:
  case 0x63B:
  case 0x63C:
  case 0x63D:
  case 0x63E:
  case 0x63F:
  case 0x64B:
  case 0x64C:
  case 0x64D:
  case 0x64E:
  case 0x64F:
  case 0x650:
  case 0x657:
  case 0x658:
  case 0x659:
  case 0x65A:
  case 0x65B:
  case 0x65C:
  case 0x65D:
  case 0x65E:
  case 0x65F:
  case 0x660:
  case 0x663:
  case 0x664:
  case 0x665:
  case 0x666:
  case 0x667:
  case 0x668:
  case 0x669:
  case 0x66A:
  case 0x66B:
  case 0x66C:
  case 0x66D:
  case 0x66E:
  case 0x66F:
  case 0x670:
  case 0x671:
  case 0x672:
  case 0x673:
  case 0x674:
  case 0x675:
  case 0x676:
  case 0x686:
  case 0x687:
  case 0x688:
  case 0x689:
  case 0x68A:
  case 0x6A2:
  case 0x6A3:
  case 0x6A4:
  case 0x6A5:
  case 0x6A6:
  case 0x6A7:
  case 0x6A8:
  case 0x6A9:
  case 0x6AC:
  case 0x6AE:
  case 0x6AF:
  case 0x6B0:
  case 0x6B1:
  case 0x6B2:
  case 0x6B3:
  case 0x6B4:
  case 0x6B5:
  case 0x6B6:
  case 0x6B7:
  case 0x6B8:
  case 0x6B9:
  case 0x6BA:
  case 0x6BB:
  case 0x6BC:
  case 0x6BD:
  case 0x6BE:
  case 0x6BF:
  case 0x6C0:
  case 0x6C1:
  case 0x6C2:
  case 0x6C3:
  case 0x6C4:
  case 0x6C5:
  case 0x6C6:
  case 0x6C7:
  case 0x6C8:
  case 0x6C9:
  case 0x6CA:
  case 0x6CB:
  case 0x6CC:
  case 0x6CD:
  case 0x6CE:
  case 0x6CF:
  case 0x6D0:
  case 0x6D1:
  case 0x6D2:
  case 0x6D3:
  case 0x6D4:
  case 0x6D5:
  case 0x6D6:
  case 0x6D7:
  case 0x6D9:
  case 0x6DA:
  case 0x6DD:
  case 0x6DE:
  case 0x6DF:
  case 0x6E1:
  case 0x6E2:
  case 0x6E3:
  case 0x6E9:
  case 0x6EA:
  case 0x6EB:
  case 0x6EC:
  case 0x6ED:
  case 0x6EE:
  case 0x6EF:
  case 0x6F0:
  case 0x6F1:
  case 0x6F2:
  case 0x6F3:
  case 0x6F4:
  case 0x6F5:
  case 0x6F6:
  case 0x6F7:
  case 0x6F8:
  case 0x6F9:
  case 0x6FA:
  case 0x6FB:
  case 0x6FC:
  case 0x6FD:
  case 0x6FE:
  case 0x6FF:
  case 0x700:
  case 0x701:
  case 0x702:
  case 0x703:
  case 0x704:
  case 0x705:
  case 0x707:
  case 0x709:
  case 0x70A:
  case 0x70B:
  case 0x70D:
  case 0x70E:
  case 0x70F:
  case 0x710:
  case 0x711:
  case 0x712:
  case 0x714:
  case 0x716:
  case 0x718:
  case 0x719:
  case 0x71A:
  case 0x71B:
  case 0x71C:
  case 0x71D:
  case 0x71E:
  case 0x71F:
  case 0x720:
  case 0x721:
  case 0x722:
  case 0x723:
  case 0x724:
  case 0x726:
  case 0x727:
  case 0x728:
  case 0x729:
  case 0x72A:
  case 0x72B:
  case 0x72C:
  case 0x72E:
  case 0x72F:
  case 0x730:
  case 0x732:
  case 0x736:
  case 0x737:
  case 0x738:
  case 0x739:
  case 0x73A:
  case 0x73C:
  case 0x73D:
  case 0x73E:
  case 0x73F:
  case 0x740:
  case 0x741:
  case 0x742:
  case 0x743:
  case 0x744:
  case 0x745:
  case 0x746:
  case 0x747:
  case 0x748:
  case 0x749:
  case 0x74A:
  case 0x751:
  case 0x752:
  case 0x753:
  case 0x755:
  case 0x756:
  case 0x757:
  case 0x758:
  case 0x759:
  case 0x75A:
  case 0x75B:
  case 0x75C:
  case 0x75D:
  case 0x75E:
  case 0x75F:
  case 0x760:
  case 0x761:
  case 0x762:
  case 0x763:
  case 0x764:
  case 0x765:
  case 0x766:
  case 0x767:
  case 0x768:
  case 0x76A:
  case 0x76B:
  case 0x76C:
  case 0x76D:
  case 0x76F:
  case 0x770:
  case 0x771:
  case 0x772:
  case 0x773:
  case 0x774:
  case 0x775:
  case 0x77A:
  case 0x77B:
  case 0x77C:
  case 0x77D:
  case 0x77F:
  case 0x782:
  case 0x784:
  case 0x785:
  case 0x786:
  case 0x787:
  case 0x788:
  case 0x789:
  case 0x78A:
  case 0x78B:
  case 0x78E:
  case 0x78F:
  case 0x790:
  case 0x791:
  case 0x793:
  case 0x794:
  case 0x795:
  case 0x796:
  case 0x797:
  case 0x798:
  case 0x799:
  case 0x79A:
  case 0x79B:
  case 0x79C:
  case 0x79D:
  case 0x79E:
  case 0x7A0:
  case 0x7A1:
  case 0x7A2:
  case 0x7A3:
  case 0x7AC:
  case 0x7AD:
  case 0x7AE:
  case 0x7AF:
  case 0x7B1:
  case 0x7B2:
  case 0x7B3:
  case 0x7B4:
  case 0x7B6:
  case 0x7B7:
  case 0x7B8:
  case 0x7B9:
  case 0x7BA:
  case 0x7BB:
  case 0x7BC:
  case 0x7BD:
  case 0x7BE:
  case 0x7BF:
  case 0x7C0:
  case 0x7C1:
  case 0x7C3:
  case 0x7C4:
  case 0x7C5:
  case 0x7C6:
  case 0x7C7:
  case 0x7C8:
  case 0x7C9:
  case 0x7CA:
  case 0x7CB:
  case 0x7CC:
  case 0x7CD:
  case 0x7CF:
  case 0x7D0:
  case 0x7D1:
  case 0x7D2:
  case 0x7D3:
  case 0x7D4:
  case 0x7D5:
  case 0x7D6:
  case 0x7D7:
  case 0x7D8:
  case 0x7D9:
  case 0x7DA:
  case 0x7DB:
  case 0x7DC:
  case 0x7DD:
  case 0x7DE:
  case 0x7DF:
  case 0x7E0:
  case 0x7E1:
  case 0x7E2:
  case 0x7E3:
  case 0x7E4:
  case 0x7E5:
  case 0x7E6:
  case 0x7E7:
  case 0x7E8:
  case 0x7E9:
  case 0x7EB:
  case 0x7EC:
  case 0x7ED:
  case 0x7EE:
  case 0x7EF:
  case 0x7F0:
  case 0x7F1:
  case 0x7F2:
  case 0x7F3:
  case 0x7F4:
  case 0x7F5:
  case 0x7F6:
  case 0x7F7:
  case 0x7F8:
  case 0x7F9:
  case 0x7FA:
  case 0x7FB:
  case 0x7FC:
  case 0x7FD:
  case 0x7FE:
  case 0x7FF:
  case 0x800:
  case 0x801:
  case 0x802:
  case 0x803:
  case 0x804:
  case 0x805:
  case 0x806:
  case 0x807:
  case 0x808:
  case 0x809:
  case 0x80A:
  case 0x80B:
  case 0x80C:
  case 0x80D:
  case 0x80E:
  case 0x80F:
  case 0x810:
  case 0x811:
  case 0x812:
  case 0x813:
  case 0x814:
  case 0x815:
  case 0x816:
  case 0x817:
  case 0x818:
  case 0x819:
  case 0x81B:
  case 0x81C:
  case 0x81E:
  case 0x81F:
  case 0x821:
  case 0x823:
  case 0x824:
  case 0x825:
  case 0x826:
  case 0x827:
  case 0x828:
  case 0x829:
  case 0x82A:
  case 0x82B:
  case 0x82C:
  case 0x82D:
  case 0x82E:
  case 0x82F:
  case 0x830:
  case 0x831:
  case 0x832:
  case 0x833:
  case 0x834:
  case 0x835:
  case 0x836:
  case 0x837:
  case 0x838:
  case 0x839:
  case 0x83A:
  case 0x83B:
  case 0x83C:
  case 0x83E:
  case 0x83F:
  case 0x840:
  case 0x841:
  case 0x843:
  case 0x844:
  case 0x845:
  case 0x846:
  case 0x847:
  case 0x848:
  case 0x849:
  case 0x84A:
  case 0x84B:
  case 0x84C:
  case 0x84D:
  case 0x84E:
  case 0x84F:
  case 0x850:
  case 0x851:
  case 0x852:
  case 0x853:
  case 0x854:
  case 0x855:
  case 0x856:
  case 0x857:
  case 0x858:
  case 0x859:
  case 0x85A:
  case 0x85B:
  case 0x85C:
  case 0x85D:
  case 0x85E:
  case 0x85F:
  case 0x860:
  case 0x861:
  case 0x862:
  case 0x863:
  case 0x864:
  case 0x865:
  case 0x866:
  case 0x867:
  case 0x868:
  case 0x869:
  case 0x86B:
  case 0x86C:
  case 0x86D:
  case 0x86E:
  case 0x86F:
  case 0x870:
  case 0x871:
  case 0x872:
  case 0x873:
  case 0x874:
  case 0x876:
  case 0x877:
  case 0x878:
  case 0x879:
  case 0x87A:
  case 0x87B:
  case 0x87C:
  case 0x87D:
  case 0x87E:
  case 0x87F:
  case 0x880:
  case 0x881:
  case 0x882:
  case 0x883:
  case 0x884:
  case 0x885:
  case 0x886:
  case 0x888:
  case 0x889:
  case 0x88A:
  case 0x88B:
  case 0x88C:
  case 0x88D:
  case 0x88E:
  case 0x88F:
  case 0x890:
  case 0x891:
  case 0x892:
  case 0x894:
  case 0x895:
  case 0x896:
  case 0x897:
  case 0x898:
  case 0x899:
  case 0x89A:
  case 0x89C:
  case 0x89D:
  case 0x89F:
  case 0x8A0:
  case 0x8A1:
  case 0x8A2:
  case 0x8A3:
  case 0x8A4:
  case 0x8A5:
  case 0x8A7:
  case 0x8A8:
  case 0x8A9:
  case 0x8AA:
  case 0x8AB:
  case 0x8AC:
  case 0x8AD:
  case 0x8AF:
  case 0x8B0:
  case 0x8B1:
  case 0x8B2:
  case 0x8B3:
  case 0x8B4:
  case 0x8B5:
  case 0x8B7:
  case 0x8B8:
  case 0x8B9:
  case 0x8BA:
  case 0x8BB:
  case 0x8BC:
  case 0x8BD:
  case 0x8BE:
  case 0x8BF:
  case 0x8C1:
  case 0x8C2:
  case 0x8C3:
  case 0x8C4:
  case 0x8C5:
  case 0x8C6:
  case 0x8C7:
  case 0x8C8:
  case 0x8CA:
  case 0x8CB:
  case 0x8CC:
  case 0x8CD:
  case 0x8CE:
  case 0x8D0:
  case 0x8D1:
  case 0x8D2:
  case 0x8D3:
  case 0x8D4:
  case 0x8D5:
  case 0x8D6:
  case 0x8D7:
  case 0x8D8:
  case 0x8D9:
  case 0x8DA:
  case 0x8DB:
  case 0x8DC:
  case 0x8DD:
  case 0x8DE:
  case 0x8DF:
  case 0x8E1:
  case 0x8E2:
  case 0x8E7:
  case 0x8E8:
  case 0x900:
  case 0x904:
  case 0x910:
  case 0x911:
  case 0x912:
  case 0x913:
  case 0x914:
  case 0x915:
  case 0x916:
  case 0x917:
  case 0x918:
  case 0x919:
  case 0x91A:
  case 0x91B:
  case 0x91C:
  case 0x91D:
  case 0x91E:
  case 0x91F:
  case 0x920:
  case 0x921:
  case 0x923:
  case 0x924:
  case 0x926:
  case 0x943:
  case 0x944:
  case 0x948:
  case 0x949:
  case 0x94A:
  case 0x94B:
  case 0x94C:
  case 0x94D:
  case 0x94E:
  case 0x94F:
  case 0x950:
  case 0x951:
  case 0x952:
  case 0x953:
  case 0x954:
  case 0x955:
  case 0x95B:
  case 0x95C:
  case 0x95D:
  case 0x95E:
  case 0x95F:
  case 0x963:
  case 0x965:
  case 0x966:
  case 0x967:
  case 0x968:
  case 0x969:
  case 0x96A:
  case 0x975:
  case 0x976:
    return true;
  default:
    return false;
  }
}

// OFFSET: 0x00B51A60
// FUNCTION: fn_19032
bool CEZ2STRScreenEffect::ApplyHeadOffset(CEZ2STRParticle* particle) {
  if (this->unknown_00C8 != 1 && this->IsAttachedToHead() != true) {
    return false;
  }

  CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
  if (!actor) {
    return false;
  }

  float offsetX = GetHatEffectOffsetX(this->id, particle->m_name.c_str());
  float offsetY = static_cast<float>(GetHatEffectOffsetY(this->id, actor));
  if (this->jobScaled == 1 && g_session.IsDoramJob(actor->GetEffectiveJob(-1)) == true) {
    offsetY -= 3.0f;
  }

  float factor = GetSkillFactor(actor->GetEffectiveJob(-1));
  if (((actor->render.flags >> 4) & 1) != 0 && this->fadePulse == 1) {
    float scale = 1.0f;
    actor->ScaleByFadePulse(scale);
    factor = scale * factor;
  }

  float shiftX = offsetX * factor;
  float shiftY = factor * offsetY;
  if (shiftX < 0.0f && shiftY < 0.0f) {
    return false;
  }

  vector3d position = particle->position;
  const matrix& view = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view;
  const matrix& inverse = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.inverse;

  float x = position.y * view.values[3] + position.x * view.values[0] + position.z * view.values[6] + view.values[9] + shiftX;
  float y = position.y * view.values[4] + position.x * view.values[1] + position.z * view.values[7] + view.values[10] - shiftY;
  float z = position.y * view.values[5] + position.x * view.values[2] + position.z * view.values[8] + view.values[11];

  float worldX = y * inverse.values[3] + x * inverse.values[0] + z * inverse.values[6] + inverse.values[9];
  float worldY = y * inverse.values[4] + x * inverse.values[1] + z * inverse.values[7] + inverse.values[10];
  float worldZ = y * inverse.values[5] + x * inverse.values[2] + z * inverse.values[8] + inverse.values[11];
  particle->position.x = worldX;
  particle->position.y = worldY;
  particle->position.z = worldZ;
  return true;
}

// OFFSET: 0x00B51CC0
// FUNCTION: fn_19033
void CEZ2STRScreenEffect::Close() {
  FreeParticle(this->particle);
}

// OFFSET: 0x00B51CD0
// FUNCTION: fn_19034
vector3d CEZ2STRScreenEffect::Slot25() {
  if (CEZ2STRParticle* particle = this->particle) {
    return particle->m_windowOffset;
  }

  return vector3d{};
}

// OFFSET: 0x00B51D20
// FUNCTION: fn_19035
bool CEZ2STRScreenEffect::Slot33() {
  if (this->id != 0x76C) {
    return CEffect::Slot33();
  }

  CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
  if (!actor) {
    return false;
  }

  if (actor->unknown_0304 != 0) {
    return actor->EffectVisible();
  }

  return true;
}

// OFFSET: 0x00B56790
// FUNCTION: fn_19042
bool CEZ2STRScreenEffect::Slot06() {
  if (this->unknown_00C8 == 1) {
    return this->ActorExists();
  }

  switch (this->id) {
  case 0x8D:
  case 0x3E6:
  case 0x45F:
  case 0x470:
  case 0x473:
  case 0x4C5:
  case 0x4EE:
  case 0x4EF:
  case 0x4F0:
  case 0x4F1:
  case 0x514:
  case 0x515:
  case 0x516:
  case 0x517:
  case 0x518:
  case 0x519:
  case 0x51A:
  case 0x51B:
  case 0x51C:
  case 0x51D:
  case 0x552:
  case 0x5BB:
  case 0x5BC:
  case 0x5BD:
  case 0x5BE:
  case 0x5BF:
  case 0x5C0:
  case 0x5C1:
  case 0x5C2:
  case 0x5C3:
  case 0x5C4:
  case 0x63F:
  case 0x64B:
  case 0x64C:
  case 0x64D:
  case 0x64E:
  case 0x64F:
  case 0x650:
  case 0x676:
  case 0x6AE:
  case 0x6BE:
  case 0x6BF:
  case 0x6CD:
  case 0x6D7:
  case 0x6EB:
  case 0x6EC:
  case 0x6F0:
  case 0x6F1:
  case 0x701:
  case 0x702:
  case 0x703:
  case 0x719:
  case 0x72F:
  case 0x752:
  case 0x756:
  case 0x75A:
  case 0x765:
  case 0x766:
  case 0x76C:
  case 0x77F:
  case 0x7AA:
  case 0x7B7:
  case 0x7BA:
  case 0x7BD:
  case 0x7BE:
  case 0x7C1:
  case 0x7C7:
  case 0x7D2:
  case 0x7EF:
  case 0x7F9:
  case 0x7FA:
  case 0x803:
  case 0x804:
  case 0x807:
  case 0x808:
  case 0x80C:
  case 0x81E:
  case 0x81F:
  case 0x84E:
  case 0x851:
  case 0x852:
  case 0x876:
  case 0x881:
  case 0x886:
  case 0x8A2:
  case 0x8A3:
  case 0x8AA:
  case 0x8AB:
  case 0x8B2:
  case 0x8B3:
  case 0x8BA:
  case 0x8BB:
  case 0x8DC:
  case 0x8DD:
  case 0x8DE:
  case 0x8E1:
  case 0x8E8:
  case 0x8FE:
  case 0x948:
  case 0x949:
  case 0x94A:
  case 0x94B:
  case 0x94C:
  case 0x94D:
  case 0x94F:
  case 0x950:
  case 0x951:
  case 0x952:
  case 0x953:
  case 0x954:
  case 0x955:
  case 0x963:
    return this->ActorExists();
  }

  CEZ2STRParticle* particle = this->particle;
  if (particle->life == 0.0f) {
    return true;
  }

  return particle->life * 60.0f > static_cast<float>(particle->unknown_00E8);
}

// OFFSET: 0x00B65EE0
// FUNCTION: fn_19044
void CEZ2STRScreenEffect::Slot03() {
  if (this->followCamera == 1 && this->particle) {
    double yaw = static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->camera.yaw;
    if (yaw != this->cameraAngle) {
      CEZ2STRParticle* particle = this->particle;
      double angle = this->cameraAngle - yaw + particle->m_angle;
      if (angle < 0.0) {
        angle += 360.0;
      } else if (angle > 360.0) {
        angle -= 360.0;
      }
      particle->m_angle = static_cast<float>(angle);
      this->cameraAngle = yaw;
    }
  }

  this->particle->position.x = this->position[0];
  this->particle->position.y = this->position[1];
  this->particle->position.z = this->position[2];

  if (CEZ2STRParticle* particle = this->particle) {
    if (this->ApplyHeadOffset(particle) || this->id == 0x584) {
      particle->Process(0.0f, 0.0f, 0.0f);
    } else {
      particle->Process(0.0f, -80.0f, 0.0f);
    }
  }

  ++this->particle->unknown_00E8;
  if (this->id == 0x851) {
    int frame = this->particle->unknown_00E8 - 30;
    if (frame >= 0 && frame % 64 == 0) {
      CRenderObject* player = reinterpret_cast<CRenderObject*>(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->scene->player);
      g_soundMgr->PlayWave(
        "effect\\ABC_ABYSS_SQUARE.wav", player->render.position.x - this->position[0], 0.0f, player->render.position.z - this->position[2], 250, 40, 1.0f, 0
      );
    }
  }

  CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
  if (!actor) {
    return;
  }

  if (this->jobScaled == 1 && actor->Slot55() <= 0) {
    float factor = GetSkillFactor(actor->GetEffectiveJob(-1));
    if (this->fadePulse == 1) {
      actor->ScaleByFadePulse(factor);
    }
    CEZ2STRParticle* particle = this->particle;
    float extent[2];
    const float* shift = actor->GetExtent(extent);
    particle->m_shiftX = factor * shift[0];
    particle->m_shiftY = factor * shift[1];
  }

  this->particle->m_lift = *reinterpret_cast<const long long*>(&actor->render.unknown_0128);
}

// OFFSET: 0x00B661B0
// FUNCTION: fn_19045
void CEZ2STRScreenEffect::Slot04() {
  CEZ2STRParticle* particle = this->particle;
  if (!particle) {
    return;
  }

  switch (this->id) {
  case 0x647:
  case 0x907:
  case 0x908:
  case 0x909:
  case 0x90A:
  case 0x90B:
  case 0x90D:
  case 0x90E:
  case 0x90F:
    this->overlayColor += 0x2000000;
    break;
  case 0x95B:
  case 0x95C:
  case 0x95F:
    this->overlayColor += 0x10000000;
    break;
  default:
    goto draw;
  }

  if (this->overlayColor >= 0xAA000000u) {
    this->overlayColor = 0xAA000000u;
  }

  DrawBlendedRectangle(
    g_renderer->primitives, g_renderer->queues, 0, 0, g_renderer->viewport.width, g_renderer->viewport.height, this->overlayColor,
    g_renderer->fog.supported != 0, g_depthStart, g_depthEnd
  );

draw:
  particle->DrawScreen(static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view);
}

// OFFSET: 0x00B66460
// FUNCTION: fn_19046
void CEZ2STRScreenEffect::Slot00(int argument) {
  this->particle->submit = reinterpret_cast<ParticleSubmit>(argument);
}

// OFFSET: 0x00B66480
// FUNCTION: fn_19047
void CEZ2STRScreenEffect::Slot23(float argument) {
  if (this->particle) {
    this->particle->m_alpha = argument;
  }
}

// OFFSET: 0x00B664A0
// FUNCTION: fn_19048
void CEZ2STRScreenEffect::Slot29(int id) {
  CEffect::Slot29(id);
  switch (id) {
  case 0x3F4:
  case 0x40A:
  case 0x40B:
  case 0x40C:
  case 0x433:
  case 0x437:
  case 0x43B:
  case 0x43C:
  case 0x43D:
  case 0x43E:
  case 0x43F:
  case 0x441:
  case 0x443:
  case 0x444:
  case 0x445:
  case 0x447:
    this->unknown_00C8 = 1;
    break;
  default:
    if (id >= 0x9B5) {
      this->unknown_00C8 = 1;
    }
    break;
  }
}

// OFFSET: 0x00B66540
// FUNCTION: fn_19049
void CEZ2STRScreenEffect::Slot24(int argument1, int argument2, int argument3, int argument4, int argument5) {
  if (!this->particle) {
    return;
  }

  const vector3d offset = *reinterpret_cast<const vector3d*>(&argument1);
  this->particle->m_windowOffset = offset;
  if (static_cast<unsigned char>(argument4) == 1) {
    this->particle->m_attachToWindow = true;
    this->particle->m_windowId = argument5;
  }
}

// OFFSET: 0x00B665B0
// FUNCTION: fn_19050
void CEZ2STRScreenEffect::Slot21(int argument) {
  this->particle->flags = argument;
}

// OFFSET: 0x00B665D0
// FUNCTION: fn_19051
void CEZ2STRScreenEffect::Slot15(int argument) {
  if (this->particle) {
    this->particle->m_repeat = argument - 1;
  }
}

// OFFSET: 0x00B665F0
// FUNCTION: fn_19052
void CEZ2STRScreenEffect::Slot14(int argument) {
  if (this->particle) {
    this->particle->m_lifeTime = argument;
  }
}

// OFFSET: 0x00B66610
// FUNCTION: fn_19053
void CEZ2STRScreenEffect::Slot17(float argument) {
  if (this->particle) {
    this->particle->m_angle = argument;
  }
}

// OFFSET: 0x00B66630
// FUNCTION: fn_19054
void CEZ2STRScreenEffect::Slot22(float argument) {
  if (this->particle) {
    this->particle->m_scale = argument;
  }
}

// OFFSET: 0x00B66650
// FUNCTION: fn_19055
void CEZ2STRScreenEffect::Slot13(int argument) {
  if (this->particle) {
    this->particle->Advance(*reinterpret_cast<float*>(&argument));
  }
}

// OFFSET: 0x00B66680
// FUNCTION: fn_19056
void CEZ2STRScreenEffect::Slot16(int argument) {
  if (this->particle) {
    this->particle->m_delay = argument;
  }
}

// OFFSET: 0x00B666A0
// FUNCTION: fn_19057
void CEZ2STRScreenEffect::Slot20(int argument) {
  this->name = reinterpret_cast<const char*>(argument);
  FreeParticle(this->particle);
  this->Slot01();
}

// OFFSET: 0x00B52970
// FUNCTION: fn_19038
int CEZ2STRScreenEffect::Slot28() {
  return this->particle->m_frame;
}

// OFFSET: 0x00B52980

// FUNCTION: fn_19039
void CEZ2STRScreenEffect::Slot01() {
  CEZ2STRParticle* particle = static_cast<CEZ2STRParticle*>(
    CreateEZ2STRParticle(reinterpret_cast<const vector3d*>(this->position), 5)
  );
  if (!particle) {
    return;
  }

  particle->m_actorId = this->actor;

  if (this->unknown_00C8 == 1) {
    this->retained = 1;
    particle->m_loop = true;
    bool adjustSize = false;
    g_luaScript->Call("IsAdjustSizeWhenShrinkState", "d>b", this->id - 0x9B5, &adjustSize);
    particle->m_followActor = adjustSize;
    bool adjustPosition = false;
    g_luaScript->Call("IsAdjustPositionWhenShrinkState", "d>b", this->id - 0x9B5, &adjustPosition);
    this->fadePulse = adjustPosition;
  }

  switch (this->id) {
  case 0x3E6:
  case 0x442:
  case 0x45F:
  case 0x470:
  case 0x473:
  case 0x4C5:
  case 0x4DC:
  case 0x4DD:
  case 0x4DE:
  case 0x4DF:
  case 0x4E0:
  case 0x4E1:
  case 0x4E2:
  case 0x4E3:
  case 0x4E4:
  case 0x4E5:
  case 0x4EE:
  case 0x4EF:
  case 0x4F0:
  case 0x4F1:
    particle->m_loop = true;
    break;

  case 0x552:
  case 0x584:
  case 0x5A6:
  case 0x607:
  case 0x63F:
  case 0x64B:
  case 0x64C:
  case 0x64D:
  case 0x64E:
  case 0x64F:
  case 0x650:
  case 0x676:
  case 0x6AE:
  case 0x6BE:
  case 0x6BF:
  case 0x6CD:
  case 0x6D7:
  case 0x6EB:
  case 0x6F1:
  case 0x701:
  case 0x702:
  case 0x703:
  case 0x712:
  case 0x719:
  case 0x72F:
  case 0x73C:
  case 0x73D:
  case 0x752:
  case 0x756:
  case 0x75A:
  case 0x765:
  case 0x766:
  case 0x76C:
  case 0x77F:
  case 0x7AA:
  case 0x7B7:
  case 0x7BA:
  case 0x7BD:
  case 0x7BE:
  case 0x7C1:
  case 0x7C7:
  case 0x7D2:
  case 0x7EF:
  case 0x7F9:
  case 0x7FA:
  case 0x803:
  case 0x804:
  case 0x807:
  case 0x808:
  case 0x80C:
  case 0x81E:
  case 0x81F:
  case 0x84E:
  case 0x851:
  case 0x852:
  case 0x876:
  case 0x881:
  case 0x886:
  case 0x8A2:
  case 0x8A3:
  case 0x8AA:
  case 0x8AB:
  case 0x8B2:
  case 0x8B3:
  case 0x8BA:
  case 0x8BB:
  case 0x8DC:
  case 0x8DD:
  case 0x8DE:
  case 0x8E1:
  case 0x8E8:
  case 0x8FE:
  case 0x948:
  case 0x949:
  case 0x94A:
  case 0x94B:
  case 0x94C:
  case 0x94D:
  case 0x94F:
  case 0x950:
  case 0x951:
  case 0x952:
  case 0x953:
  case 0x954:
  case 0x955:
  case 0x963:
    this->retained = 1;
    particle->m_loop = true;
    break;

  case 0x6EC:
    this->AddEvent(0, "effect\\AG_MYSTERY_ILLUSION.wav", 0, 0);
    this->retained = 1;
    particle->m_loop = true;
    break;

  case 0x6F0:
    this->AddEvent(0, "effect\\AG_TORNADO_STORM.wav", 0, 0);
    this->retained = 1;
    particle->m_loop = true;
    break;

  case 0x710:
    this->AddEvent(0, "effect\\CD_MEDIALE_VOTUM.wav", 0, 0);
    break;

  default:
    break;
  }

  if (this->unknown_00C4 > 0) {
    particle->m_loop = true;
  }

  InitParticleQuad(particle->vertices);
  this->particle = particle;
  this->Slot34(particle, this->id);

  CGameMode* mode = static_cast<CGameMode*>(g_modeMgr.GetGameMode());
  this->cameraAngle = static_cast<double>(mode->camera->camera.yaw);

  if (this->id >= 0x9B5) {
    g_luaScript->Call("IsAttachedToHead", "d>b", this->id - 0x9B5, &this->jobScaled);
  }
}

// OFFSET: 0x00B531E0
