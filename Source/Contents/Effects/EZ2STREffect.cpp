#include "Contents/Effects/EZ2STREffect.h"
#include "Client/Mode/CGameMode.h"
#include "Client/Mode/ModeMgr.h"
#include "Client/Session.h"
#include "Contents/Actors/GameActor.h"
#include "Contents/Actors/RenderObject.h"
#include "Contents/Camera/Camera.h"
#include "Contents/Configuration/ServiceOptions.h"
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
#include "Utility/Math/AffineMatrix.h"
#include "Utility/Text/FormatString.h"
#include <bit>
#include <cmath>
#include <cstdlib>
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

// OFFSET: 0x00B56D30
// FUNCTION: fn_19043
void CEZ2STRScreenEffect::Slot34(CEZ2STRParticle* particle, int id) {
  switch (id) {
  case 0xCC:
    particle->Open("빨간포션.str");
    break;
  case 0xCD:
    particle->Open("주홍포션.str");
    break;
  case 0xCE:
    particle->Open("노란포션.str");
    break;
  case 0xCF:
    particle->Open("하얀포션.str");
    break;
  case 0xD0:
    particle->Open("파란포션.str");
    break;
  case 0xD1:
    particle->Open("초록포션.str");
    break;
  case 0xD2:
    particle->Open("fruit.str");
    break;
  case 0xD3:
    particle->Open("fruit_.str");
    break;
  case 0x59:
    particle->Open("StormGust.str");
    break;
  case 0x3E4:
    particle->Open("jyumonjikiri.str");
    break;
  case 0x3E5:
    particle->Open("setsudan.str");
    break;
  case 0x3E6:
    particle->Open("red_cross.str");
    break;
  case 0x3E7:
    particle->Open("ez_cure.str");
    break;
  case 0x3F0:
    particle->Open("술식해방.str");
    break;
  case 0x3F1:
    particle->Open("불길한붉은달빛.str");
    break;
  case 0x3F2:
    particle->Open("초승달2.str");
    break;
  case 0x3E0:
    particle->Open("술식전개3.str");
    break;
  case 0x3E1:
    particle->Open("술식전개4.str");
    break;
  case 0x3E2:
    particle->Open("술식전개2.str");
    break;
  case 0x3E3:
    particle->Open("술식전개5.str");
    break;
  case 0x3E9:
    particle->Open("허무의그림자.str");
    break;
  case 0x3F3:
    particle->Open("흐린달빛환상2.str");
    break;
  case 0xD:
    particle->Open("SafetyWall.str");
    break;
  case 0x3F4:
    particle->Open("cloudh.str");
    break;
  case 0x40A:
    particle->Open("moonstar.str");
    break;
  case 0x437:
    particle->Open("EFST_DECORATION_OF_MUSIC\\note_1.str");
    break;
  case 0x43F:
    particle->Open("efst_happiness_star\\moonstar2.str");
    break;
  case 0x40B:
    particle->Open("strangelights.str");
    break;
  case 0x40C:
    particle->Open("superstar.str");
    break;
  case 0x426:
    particle->Open("RL_HAMER_GOD\\hogg2.str");
    break;
  case 0x427:
    particle->Open("RL_MESS_SPIRAL\\S5.str");
    break;
  case 0x428:
    particle->Open("RL_FIRE_RAIN\\fire_rain.str");
    break;
  case 0x42A:
    particle->Open("RL_BANISHING_BUSTER\\vanishing1.str");
    break;
  case 0x42B:
    particle->Open("RL_SLUGSHOT\\slug.str");
    break;
  case 0x42C:
    particle->Open("RL_d_tail\\dtdt.str");
    break;
  case 0x432:
    particle->Open("RL_EXPLOSION\\boom.str");
    break;
  case 0x425:
    particle->Open("RL_C_MAKER\\deffender.str");
    break;
  case 0x433:
    particle->Open("RL_C_MAKER\\cm.str");
    break;
  case 0x434:
    particle->Open("RL_QUICK_DRAW\\qd2.str");
    break;
  case 0x435:
    particle->Open("RL_P_ALTER\\platinum.str");
    break;
  case 0x436:
    particle->Open("RL_STORM\\S_STORM.str");
    break;
  case 0x43B:
    particle->Open("efst_flowersmoke\\flowersmoke.str");
    break;
  case 0x43C:
    particle->Open("efst_fstone\\fstone.str");
    break;
  case 0x43D:
    particle->Open("efst_qscaraba\\qscaraba.str");
    break;
  case 0x43E:
    particle->Open("efst_ljosalfar\\ljosalfar.str");
    break;
  case 0x441:
    particle->Open("efst_maple_falls\\maple_falls.str");
    break;
  case 0x442:
    particle->Open("Arrows_Effect_finish\\pol.str");
    break;
  case 0x443:
    particle->Open("efst_magical_feather\\magical_feather.str");
    break;
  case 0x444:
    particle->Open("efst_mermaid_loging\\bubblebubble.str");
    break;
  case 0x446:
    particle->Open("ach_complete\\ppring3.str");
    break;
  case 0x447:
    particle->Open("efst_time_accessory\\time_accessory.str");
    break;
  case 0x44A:
    particle->Open("su_freshshrimp\\su_freshshrimp.str");
    break;
  case 0x44E:
    particle->Open("su_tunabelly\\su_tunabelly.str");
    break;
  case 0x44D:
    particle->Open("su_sv_stemspear\\su_sv_stemspear.str");
    break;
  case 0x44F:
    particle->Open("su_scratch\\su_scratch.str");
    break;
  case 0x451:
    particle->Open("su_bite\\su_bite.str");
    break;
  case 0x453:
    particle->Open("su_scaroftarou\\su_scaroftarou.str");
    break;
  case 0x454:
    particle->Open("su_arclousedash\\su_arclousedash.str");
    break;
  case 0x455:
    particle->Open("su_pickypeck\\su_pickypeck.str");
    break;
  case 0x45C:
    particle->Open("su_bunchofshrimp\\su_bunchofshrimp.str");
    break;
  case 0x45F:
    particle->Open("bite_scar\\bite_scar.str");
    break;
  case 0x461:
    particle->Open("wide_critical_wound\\wide_critical_wound.str");
    break;
  case 0x463:
    particle->Open("su_grooming\\su_grooming.str");
    break;
  case 0x464:
    particle->Open("su_chattering\\su_chattering.str");
    break;
  case 0x465:
    particle->Open("su_nyanggrass\\su_nyanggrass.str");
    break;
  case 0x466:
    particle->Open("su_hiss\\su_hiss.str");
    break;
  case 0x467:
    particle->Open("su_powerofflock\\su_powerofflock.str");
    break;
  case 0x469:
    particle->Open("su_svg_spirit\\su_svg_spirit.str");
    break;
  case 0x8D:
    particle->Open(FormatOwnedString("Pneuma%d.str", std::max(rand() % 3, 0) + 1));
    break;
  case 0x46D:
    particle->Open("RL_fire_dance\\firedance.str");
    break;
  case 0x46E:
    particle->Open("RL_richs_coin_A\\reachs.str");
    break;
  case 0x470:
    particle->Open("RL_anti_material\\anti.str");
    break;
  case 0x471:
    particle->Open("RL_e_chain\\eternal.str");
    break;
  case 0x472:
    particle->Open("RL_heat_barrel\\htb.str");
    break;
  case 0x473:
    particle->Open("RL_h_mine\\mine.str");
    break;
  case 0x475:
    particle->Open("we_oneforever\\we_oneforever.str");
    break;
  case 0x476:
    particle->Open("we_cheerup\\we_cheerup.str");
    break;
  case 0x489:
    particle->Open("Quagmire\\Quagmire.str");
    break;
  case 0x48A:
    particle->Open("lord_of_vermillion\\lord_of_vermillion.str");
    break;
  case 0x48B:
    {
      std::string name = FormatOwnedString("MeteorStorm\\meteor%d.str", std::max(rand() % 3, 0) + 1);
      particle->Open(name.c_str());
      break;
    }
  case 0x4C2:
    particle->Open("solar_burst\\solar_burst.str");
    break;
  case 0x4C3:
    particle->Open("sj_document\\sj_document.str");
    break;
  case 0x4C4:
    particle->Open("falling_star\\falling_star.str");
    break;
  case 0x4C5:
    particle->Open("flash_kick\\flash_kick.str");
    break;
  case 0x4CC:
    particle->Open("soul_golem\\soul_golem.str");
    break;
  case 0x4CD:
    particle->Open("newmoon_kick\\newmoon_kick.str");
    break;
  case 0x4CE:
    particle->Open("fullmoon_kick\\fullmoon_kick.str");
    break;
  case 0x4D6:
    particle->Open("soul_curse\\soul_curse.str");
    break;
  case 0x4D7:
    particle->Open("prominence_kick\\prominence_kick.str");
    break;
  case 0x4DB:
    particle->Open("soul_revolve\\soul_revolve.str");
    break;
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
    particle->Open(FormatOwnedString("test_effect\\%02d\\%02d.str", id - 0x4DC, id - 0x4DC));
    break;
  case 0x4E6:
    particle->Open("new_meteostorm\\new_meteostorm\\new_meteostorm.str");
    break;
  case 0x4E7:
    particle->Open("new_meteostorm\\new_meteostorm_bottom\\new_meteostorm_bottom.str");
    break;
  case 0x4EA:
    particle->Open("new_adoramus\\new_adoramus\\new_adoramus.str");
    break;
  case 0x4EB:
    particle->Open("new_adoramus\\new_adoramus_bottom\\new_adoramus_bottom.str");
    break;
  case 0x4EC:
    particle->Open("tetravortex\\tetra_cast\\tetra_cast.str");
    break;
  case 0x4ED:
    particle->Open("tetravortex\\tetra_ball_00_core\\tetra_ball_00_core.str");
    break;
  case 0x4EE:
    particle->Open("tetravortex\\tetra_ball_01_fire\\tetra_01_fire.str");
    break;
  case 0x4EF:
    particle->Open("tetravortex\\tetra_ball_04_water\\tetra_04_water.str");
    break;
  case 0x4F0:
    particle->Open("tetravortex\\tetra_ball_02_wind\\tetra_02_wind.str");
    break;
  case 0x4F1:
    particle->Open("tetravortex\\tetra_ball_03_earth\\tetra_03_earth.str");
    break;
  case 0x4F2:
    particle->Open("tetravortex\\tetra_01_fire_explosion\\tetra_01_fire_expn.str");
    break;
  case 0x4F3:
    particle->Open("tetravortex\\tetra_04_water_explosion\\tetra_04_water_expn.str");
    break;
  case 0x4F4:
    particle->Open("tetravortex\\tetra_02_wind_explosion\\tetra_02_wind_expn.str");
    break;
  case 0x4F5:
    particle->Open("tetravortex\\tetra_03_earth_explosion\\tetra_03_earth_expn.str");
    break;
  case 0x4F6:
    particle->Open("tetravortex\\tetra_01_fire_ground\\tetra_01_fire_grd.str");
    break;
  case 0x4F7:
    particle->Open("tetravortex\\tetra_04_water_ground\\tetra_04_water_grd.str");
    break;
  case 0x4F8:
    particle->Open("tetravortex\\tetra_02_wind_ground\\tetra_02_wind_grd.str");
    break;
  case 0x4F9:
    particle->Open("tetravortex\\tetra_03_earth_ground\\tetra_03_earth_grd.str");
    break;
  case 0x4FD:
    particle->Open("tetravortex\\tetra_ball_00_core_ground\\tetra_ball_00_core_grd.str");
    break;
  case 0x4FE:
    particle->Open("tetravortex\\tetra_01_fire_sub_explosion\\tetra_01_fire_sub_expn.str");
    break;
  case 0x4FF:
    particle->Open("tetravortex\\tetra_04_water_sub_explosion\\tetra_04_water_sub_expn.str");
    break;
  case 0x500:
    particle->Open("tetravortex\\tetra_02_wind_sub_explosion\\tetra_02_wind_sub_expn.str");
    break;
  case 0x501:
    particle->Open("tetravortex\\tetra_03_earth_sub_explosion\\tetra_03_earth_sub_expn.str");
    break;
  case 0x4FA:
    particle->Open("new_crimsonrock\\new_crimsonrock\\new_crimsonrock.str");
    break;
  case 0x4FB:
    particle->Open("new_crimsonrock\\new_crimsonrock_bottom\\new_crimsonrock_bottom.str");
    break;
  case 0x512:
    particle->Open("new_crimsonrock\\new_crimsonrock_sub_explosion\\new_crimsonrock_sub_explosion.str");
    break;
  case 0x502:
    particle->Open("new_diamonddust\\new_diamonddust\\new_diamonddust.str");
    break;
  case 0x503:
    particle->Open("new_diamonddust\\new_diamonddust_bottom\\new_diamonddust_bottom.str");
    break;
  case 0x504:
    particle->Open("new_diamonddust\\new_diamonddust_cast\\new_diamonddust_cast.str");
    break;
  case 0x505:
    particle->Open("new_severerainstorm\\new_severerainstorm_cast\\new_severerainstorm_cast.str");
    break;
  case 0x507:
    particle->Open("new_secrament\\new_secrament\\new_secrament.str");
    break;
  case 0x50F:
    particle->Open("new_secrament\\new_secrament_aura\\new_secrament_aura.str");
    break;
  case 0x508:
    particle->Open("new_psychicwave\\new_psychicwave\\new_psychicwave.str");
    break;
  case 0x509:
    particle->Open("new_psychicwave\\new_psychicwave_bottom\\new_psychicwave_bottom.str");
    break;
  case 0x50A:
    particle->Open("new_psychicwave\\new_psychicwave_cast\\new_psychicwave_cast.str");
    break;
  case 0x50D:
    particle->Open("new_arrowstorm\\new_arrowstorm_00\\new_arrowstorm_00.str");
    break;
  case 0x50E:
    particle->Open("new_arrowstorm\\new_arrowstorm_01\\new_arrowstorm_01.str");
    break;
  case 0x510:
    particle->Open("new_triangleshot\\new_triangleshot\\new_triangleshot.str");
    break;
  case 0x511:
    particle->Open("new_triangleshot\\new_triangleshot_bottom\\new_triangleshot_bottom.str");
    break;
  case 0x51E:
    particle->Open("new_rampageblaster\\new_rampageblaster\\new_rampageblaster.str");
    break;
  case 0x51F:
    particle->Open("new_rampageblaster\\new_rampageblaster_bottom\\new_rampageblaster_bottom.str");
    break;
  case 0x520:
    particle->Open("new_peintbomb\\new_peintbomb\\new_peintbomb.str");
    break;
  case 0x521:
    particle->Open("new_peintbomb\\new_peintbomb_bottom\\new_peintbomb_bottom.str");
    break;
  case 0x513:
    particle->Open("new_rollingcutter\\new_rollingcutter\\new_rollingcutter.str");
    break;
  case 0x514:
    particle->Open("new_rollingcutter\\new_rollingcutter_count_01\\new_rollingcutter_count_01.str");
    break;
  case 0x515:
    particle->Open("new_rollingcutter\\new_rollingcutter_count_02\\new_rollingcutter_count_02.str");
    break;
  case 0x516:
    particle->Open("new_rollingcutter\\new_rollingcutter_count_03\\new_rollingcutter_count_03.str");
    break;
  case 0x517:
    particle->Open("new_rollingcutter\\new_rollingcutter_count_04\\new_rollingcutter_count_04.str");
    break;
  case 0x518:
    particle->Open("new_rollingcutter\\new_rollingcutter_count_05\\new_rollingcutter_count_05.str");
    break;
  case 0x519:
    particle->Open("new_rollingcutter\\new_rollingcutter_count_06\\new_rollingcutter_count_06.str");
    break;
  case 0x51A:
    particle->Open("new_rollingcutter\\new_rollingcutter_count_07\\new_rollingcutter_count_07.str");
    break;
  case 0x51B:
    particle->Open("new_rollingcutter\\new_rollingcutter_count_08\\new_rollingcutter_count_08.str");
    break;
  case 0x51C:
    particle->Open("new_rollingcutter\\new_rollingcutter_count_09\\new_rollingcutter_count_09.str");
    break;
  case 0x51D:
    particle->Open("new_rollingcutter\\new_rollingcutter_count_10\\new_rollingcutter_count_10.str");
    break;
  case 0x522:
    particle->Open("new_crossripperslasher\\new_crossripperslasher\\new_crossripperslasher.str");
    break;
  case 0x523:
    particle->Open("new_crossripperslasher\\new_crossripperslasher_clock_00\\new_crossripperslasher_clock_00.str");
    break;
  case 0x524:
    particle->Open("new_crossripperslasher\\new_crossripperslasher_clock_01\\new_crossripperslasher_clock_01.str");
    break;
  case 0x525:
    particle->Open("new_crossripperslasher\\new_crossripperslasher_clock_03\\new_crossripperslasher_clock_03.str");
    break;
  case 0x526:
    particle->Open("new_crossripperslasher\\new_crossripperslasher_clock_05\\new_crossripperslasher_clock_05.str");
    break;
  case 0x527:
    particle->Open("new_crossripperslasher\\new_crossripperslasher_clock_06\\new_crossripperslasher_clock_06.str");
    break;
  case 0x528:
    particle->Open("new_crossripperslasher\\new_crossripperslasher_clock_07\\new_crossripperslasher_clock_07.str");
    break;
  case 0x529:
    particle->Open("new_crossripperslasher\\new_crossripperslasher_clock_09\\new_crossripperslasher_clock_09.str");
    break;
  case 0x52A:
    particle->Open("new_crossripperslasher\\new_crossripperslasher_clock_11\\new_crossripperslasher_clock_11.str");
    break;
  case 0x52B:
    particle->Open("new_tigercannon\\new_tigercannon.str");
    break;
  case 0x544:
    particle->Open("new_armscannon\\new_armscannon_01_clock_down\\new_armscannon_01_clock_down.str");
    break;
  case 0x543:
    particle->Open("new_armscannon\\new_armscannon_01_clock_up\\new_armscannon_01_clock_up.str");
    break;
  case 0x546:
    particle->Open("new_armscannon\\new_armscannon_05_clock_down\\new_armscannon_05_clock_down.str");
    break;
  case 0x545:
    particle->Open("new_armscannon\\new_armscannon_05_clock_up\\new_armscannon_05_clock_up.str");
    break;
  case 0x548:
    particle->Open("new_armscannon\\new_armscannon_07_clock_down\\new_armscannon_07_clock_down.str");
    break;
  case 0x547:
    particle->Open("new_armscannon\\new_armscannon_07_clock_up\\new_armscannon_07_clock_up.str");
    break;
  case 0x54A:
    particle->Open("new_armscannon\\new_armscannon_11_clock_down\\new_armscannon_11_clock_down.str");
    break;
  case 0x549:
    particle->Open("new_armscannon\\new_armscannon_11_clock_up\\new_armscannon_11_clock_up.str");
    break;
  case 0x54B:
    particle->Open("new_armscannon\\new_armscannon_explosion\\new_armscannon_explosion.str");
    break;
  case 0x54C:
    particle->Open("new_armscannon\\new_armscannon_explosion_bottom\\new_armscannon_explosion_bottom.str");
    break;
  case 0x568:
    particle->Open("new_armscannon\\new_armscannon_cast\\new_armscannon_cast.str");
    break;
  case 0x569:
    particle->Open("new_armscannon\\new_armscannon_cast_bottom\\new_armscannon_cast_bottom.str");
    break;
  case 0x54E:
    particle->Open("new_cart_cannon\\new_cart_cannon_explosion\\new_cart_cannon_explosion.str");
    break;
  case 0x54F:
    particle->Open("new_cart_cannon\\new_cart_cannon_explosion_bottom\\new_cart_cannon_explosion_bottom.str");
    break;
  case 0x550:
    particle->Open("new_cart_cannon\\new_cart_cannon_fire\\new_cart_cannon_fire.str");
    break;
  case 0x551:
    particle->Open("new_cart_cannon\\new_cart_cannon_fire_bottom\\new_cart_cannon_fire_bottom.str");
    break;
  case 0x58E:
    particle->Open("new_cart_cannon\\new_cart_cannon_cast\\new_cart_cannon_cast.str");
    break;
  case 0x58F:
    particle->Open("new_cart_cannon\\new_cart_cannon_cast_bottom\\new_cart_cannon_cast_bottom.str");
    break;
  case 0x552:
    particle->Open("efst_qscaraba\\qscaraba.str");
    break;
  case 0x553:
    particle->Open("new_axetornado\\new_axetornado\\new_axetornado.str");
    break;
  case 0x554:
    particle->Open("new_axetornado\\new_axetornado_bottom\\new_axetornado_bottom.str");
    break;
  case 0x555:
    particle->Open("new_cart_tornado\\new_cart_tornado\\new_cart_tornado.str");
    break;
  case 0x556:
    particle->Open("new_cart_tornado\\new_cart_tornado_bottom\\new_cart_tornado_bottom.str");
    break;
  case 0x557:
    particle->Open("help_angel\\help_angel\\help_angel.str");
    break;
  case 0x558:
    particle->Open("help_angel\\help_angel_bottom\\help_angel_bottom.str");
    break;
  case 0x559:
    particle->Open("new_banishingpoint\\new_banishingpoint_target\\new_banishingpoint_target.str");
    break;
  case 0x55A:
    particle->Open("new_banishingpoint\\new_banishingpoint_target_bottom\\new_banishingpoint_target_bottom.str");
    break;
  case 0x55B:
    particle->Open("new_banishingpoint\\new_banishingpoint_cast\\new_banishingpoint_cast.str");
    break;
  case 0x55C:
    particle->Open("new_banishingpoint\\new_banishingpoint_cast_bottom\\new_banishingpoint_cast_bottom.str");
    break;
  case 0x55D:
    particle->Open("new_banishingpoint\\new_banishingpoint_01\\new_banishingpoint_01.str");
    break;
  case 0x55E:
    particle->Open("new_banishingpoint\\new_banishingpoint_05\\new_banishingpoint_05.str");
    break;
  case 0x55F:
    particle->Open("new_banishingpoint\\new_banishingpoint_07\\new_banishingpoint_07.str");
    break;
  case 0x560:
    particle->Open("new_banishingpoint\\new_banishingpoint_11\\new_banishingpoint_11.str");
    break;
  case 0x562:
    particle->Open("new_dragonbreath\\new_dragonbreath_bomb\\new_dragonbreath_bomb.str");
    break;
  case 0x563:
    particle->Open("new_dragonbreath\\new_dragonbreath_bomb_bottom\\new_dragonbreath_bomb_bottom.str");
    break;
  case 0x564:
    particle->Open("new_dragonbreath\\new_dragonbreath_01_clock\\new_dragonbreath_01_clock.str");
    break;
  case 0x565:
    particle->Open("new_dragonbreath\\new_dragonbreath_05_clock\\new_dragonbreath_05_clock.str");
    break;
  case 0x566:
    particle->Open("new_dragonbreath\\new_dragonbreath_07_clock\\new_dragonbreath_07_clock.str");
    break;
  case 0x567:
    particle->Open("new_dragonbreath\\new_dragonbreath_11_clock\\new_dragonbreath_11_clock.str");
    break;
  case 0x56A:
    particle->Open("new_dragonbreath_water\\new_dragonbreath_water_bomb\\new_dragonbreath_water_bomb.str");
    break;
  case 0x56B:
    particle->Open("new_dragonbreath_water\\new_dragonbreath_water_bomb_bottom\\new_dragonbreath_water_bomb_bottom.str");
    break;
  case 0x56C:
    particle->Open("new_dragonbreath_water\\new_dragonbreath_water_01_clock\\new_dragonbreath_water_01_clock.str");
    break;
  case 0x56D:
    particle->Open("new_dragonbreath_water\\new_dragonbreath_water_05_clock\\new_dragonbreath_water_05_clock.str");
    break;
  case 0x56E:
    particle->Open("new_dragonbreath_water\\new_dragonbreath_water_07_clock\\new_dragonbreath_water_07_clock.str");
    break;
  case 0x56F:
    particle->Open("new_dragonbreath_water\\new_dragonbreath_water_11_clock\\new_dragonbreath_water_11_clock.str");
    break;
  case 0x570:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_counter_slash\\new_counter_slash\\new_counter_slash.str");
    } else {
      particle->Open("new_counter_slash\\new_counter_slash\\new_counter_slash.str");
    }
    break;
  case 0x571:
    particle->Open("new_counter_slash\\new_counter_slash_bottom\\new_counter_slash_bottom.str");
    break;
  case 0x572:
    particle->Open("new_fatal_menace\\new_fatal_menace\\new_fatal_menace.str");
    break;
  case 0x573:
    particle->Open("new_fatal_menace\\new_fatal_menace_bottom\\new_fatal_menace_bottom.str");
    break;
  case 0x574:
    particle->Open("new_fatal_menace\\new_fatal_menace_cast\\new_fatal_menace_cast.str");
    break;
  case 0x575:
    particle->Open("new_fatal_menace\\new_fatal_menace_cast_bottom\\new_fatal_menace_cast_bottom.str");
    break;
  case 0x578:
    particle->Open("new_cannon_spear\\new_cannon_spear_12_clock\\new_cannon_spear_12_clock.str");
    break;
  case 0x579:
    particle->Open("new_cannon_spear\\new_cannon_spear_01_clock\\new_cannon_spear_01_clock.str");
    break;
  case 0x57A:
    particle->Open("new_cannon_spear\\new_cannon_spear_03_clock\\new_cannon_spear_03_clock.str");
    break;
  case 0x57B:
    particle->Open("new_cannon_spear\\new_cannon_spear_05_clock\\new_cannon_spear_05_clock.str");
    break;
  case 0x57C:
    particle->Open("new_cannon_spear\\new_cannon_spear_06_clock\\new_cannon_spear_06_clock.str");
    break;
  case 0x57D:
    particle->Open("new_cannon_spear\\new_cannon_spear_07_clock\\new_cannon_spear_07_clock.str");
    break;
  case 0x57E:
    particle->Open("new_cannon_spear\\new_cannon_spear_09_clock\\new_cannon_spear_09_clock.str");
    break;
  case 0x57F:
    particle->Open("new_cannon_spear\\new_cannon_spear_11_clock\\new_cannon_spear_11_clock.str");
    break;
  case 0x580:
    particle->Open("new_cannon_spear\\new_cannon_spear_bottom\\new_cannon_spear_bottom.str");
    break;
  case 0x582:
    particle->Open("new_hit\\new_basicattack\\new_basicattack.str");
    break;
  case 0x583:
    particle->Open("new_hit\\new_criticalattack\\new_criticalattack.str");
    break;
  case 0x584:
    particle->Open(this->name);
    break;
  case 0x585:
    particle->Open("new_comet\\new_comet\\new_comet.str");
    break;
  case 0x586:
    particle->Open("new_comet\\new_comet_bottom\\new_comet_bottom.str");
    break;
  case 0x587:
    particle->Open("new_comet\\new_comet_cast_bottom\\new_comet_cast_bottom.str");
    break;
  case 0x588:
    particle->Open("new_comet\\new_comet_sub_explosion\\new_comet_sub_explosion.str");
    break;
  case 0x589:
    particle->Open("new_comet\\new_comet_sub_explosion_bottom\\new_comet_sub_explosion_bottom.str");
    break;
  case 0x58D:
    particle->Open("new_comet\\new_comet_cast\\new_comet_cast.str");
    break;
  case 0x590:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_rayofgenesis\\new_rayofgenesis\\new_rayofgenesis.str");
    } else {
      particle->Open("new_rayofgenesis\\new_rayofgenesis\\new_rayofgenesis.str");
    }
    break;
  case 0x591:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_rayofgenesis\\new_rayofgenesis_cast\\new_rayofgenesis_cast.str");
    } else {
      particle->Open("new_rayofgenesis\\new_rayofgenesis_cast\\new_rayofgenesis_cast.str");
    }
    break;
  case 0x592:
    particle->Open("new_rayofgenesis\\new_rayofgenesis_cast_bottom\\new_rayofgenesis_cast_bottom.str");
    break;
  case 0x593:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_rayofgenesis\\new_rayofgenesis_hit\\new_rayofgenesis_hit.str");
    } else {
      particle->Open("new_rayofgenesis\\new_rayofgenesis_hit\\new_rayofgenesis_hit.str");
    }
    break;
  case 0x594:
    particle->Open("mineffect\\new_adoramus\\new_adoramus\\new_adoramus.str");
    break;
  case 0x595:
    particle->Open("mineffect\\new_adoramus\\new_adoramus_bottom\\new_adoramus_bottom.str");
    break;
  case 0x596:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_cast\\new_tetravortex_cast.str");
    break;
  case 0x597:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_ball_core\\new_tetravortex_ball_core.str");
    break;
  case 0x598:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_fire\\new_tetravortex_fire.str");
    break;
  case 0x599:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_water\\new_tetravortex_water.str");
    break;
  case 0x59A:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_wind\\new_tetravortex_wind.str");
    break;
  case 0x59B:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_earth\\new_tetravortex_earth.str");
    break;
  case 0x59C:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_fire_bottom\\new_tetravortex_fire_bottom.str");
    break;
  case 0x59D:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_water_bottom\\new_tetravortex_water_bottom.str");
    break;
  case 0x59E:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_wind_bottom\\new_tetravortex_wind_bottom.str");
    break;
  case 0x59F:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_earth_bottom\\new_tetravortex_earth_bottom.str");
    break;
  case 0x5A0:
    particle->Open("mineffect\\new_tetravortex\\new_tetravortex_ball_core_bottom\\new_tetravortex_ball_core_bottom.str");
    break;
  case 0x5A1:
    particle->Open("mineffect\\new_crimsonrock\\new_crimsonrock\\new_crimsonrock.str");
    break;
  case 0x5A2:
    particle->Open("mineffect\\new_crimsonrock\\new_crimsonrock_bottom\\new_crimsonrock_bottom.str");
    break;
  case 0x5A3:
    particle->Open("new_jackfrost\\new_jackfrost\\new_jackfrost_00.str");
    break;
  case 0x5A4:
    particle->Open("new_jackfrost\\new_jackfrost\\new_jackfrost_01.str");
    break;
  case 0x5A5:
    particle->Open("new_jackfrost\\new_jackfrost\\new_jackfrost_02.str");
    break;
  case 0x5A6:
    particle->Open("new_jackfrost\\new_jackfrost_cast\\new_jackfrost_cast.str");
    break;
  case 0x5A7:
    particle->Open("new_jackfrost\\new_jackfrost_magicrune\\new_jackfrost_frost.str");
    break;
  case 0x5AA:
    particle->Open("mineffect\\new_diamonddust\\new_diamonddust\\new_diamonddust.str");
    break;
  case 0x5AB:
    particle->Open("mineffect\\new_diamonddust\\new_diamonddust_bottom\\new_diamonddust_bottom.str");
    break;
  case 0x5AC:
    particle->Open("mineffect\\new_diamonddust\\new_diamonddust_cast\\new_diamonddust_cast.str");
    break;
  case 0x5AD:
    particle->Open("mineffect\\new_secrament\\new_secrament\\new_secrament.str");
    break;
  case 0x5AE:
    particle->Open("mineffect\\new_psychicwave\\new_psychicwave\\new_psychicwave.str");
    break;
  case 0x5AF:
    particle->Open("mineffect\\new_psychicwave\\new_psychicwave_bottom\\new_psychicwave_bottom.str");
    break;
  case 0x5B0:
    particle->Open("mineffect\\new_psychicwave\\new_psychicwave_cast\\new_psychicwave_cast.str");
    break;
  case 0x5B1:
    particle->Open("mineffect\\new_arrowstorm\\new_arrowstorm_00\\new_arrowstorm_00.str");
    break;
  case 0x5B2:
    particle->Open("mineffect\\new_arrowstorm\\new_arrowstorm_01\\new_arrowstorm_01.str");
    break;
  case 0x5B3:
    particle->Open("mineffect\\new_triangleshot\\new_triangleshot\\new_triangleshot.str");
    break;
  case 0x5B4:
    particle->Open("mineffect\\new_triangleshot\\new_triangleshot_bottom\\new_triangleshot_bottom.str");
    break;
  case 0x5B5:
    particle->Open("mineffect\\new_rampageblaster\\new_rampageblaster\\new_rampageblaster_exp.str");
    break;
  case 0x5B6:
    particle->Open("mineffect\\new_rampageblaster\\new_rampageblaster_bottom\\new_rampageblaster_bottom.str");
    break;
  case 0x5B7:
    particle->Open("mineffect\\new_peintbomb\\new_peintbomb\\new_peintbomb.str");
    break;
  case 0x5B8:
    particle->Open("mineffect\\new_peintbomb\\new_peintbomb_bottom\\new_peintbomb_bottom.str");
    break;
  case 0x5B9:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_earthgrave\\new_earthgrave\\new_earthgrave.str");
    } else {
      particle->Open("new_earthgrave\\new_earthgrave\\new_earthgrave.str");
    }
    break;
  case 0x656:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_earthgrave\\new_earthgrave_bottom\\new_earthgrave_bottom.str");
    } else {
      particle->Open("new_earthgrave\\new_earthgrave_bottom\\new_earthgrave_bottom.str");
    }
    break;
  case 0x657:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_earthgrave\\new_earthgrave_hit\\new_earthgrave_hit.str");
    } else {
      particle->Open("new_earthgrave\\new_earthgrave_hit\\new_earthgrave_hit.str");
    }
    break;
  case 0x5BA:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter\\new_rollingcutter_cast.str");
    break;
  case 0x5BB:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter_count_01\\new_rollingcutter_count_01.str");
    break;
  case 0x5BC:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter_count_02\\new_rollingcutter_count_02.str");
    break;
  case 0x5BD:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter_count_03\\new_rollingcutter_count_03.str");
    break;
  case 0x5BE:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter_count_04\\new_rollingcutter_count_04.str");
    break;
  case 0x5BF:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter_count_05\\new_rollingcutter_count_05.str");
    break;
  case 0x5C0:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter_count_06\\new_rollingcutter_count_06.str");
    break;
  case 0x5C1:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter_count_07\\new_rollingcutter_count_07.str");
    break;
  case 0x5C2:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter_count_08\\new_rollingcutter_count_08.str");
    break;
  case 0x5C3:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter_count_09\\new_rollingcutter_count_09.str");
    break;
  case 0x5C4:
    particle->Open("mineffect\\new_rollingcutter\\new_rollingcutter_count_10\\new_rollingcutter_count_10.str");
    break;
  case 0x5C5:
    particle->Open("mineffect\\new_crossripperslasher\\new_crossripperslasher\\new_crossripperslasher.str");
    break;
  case 0x5C6:
    particle->Open("mineffect\\new_crossripperslasher\\new_crossripperslasher_12\\new_crossripperslasher_12.str");
    break;
  case 0x5C7:
    particle->Open("mineffect\\new_crossripperslasher\\new_crossripperslasher_01\\new_crossripperslasher_01.str");
    break;
  case 0x5C8:
    particle->Open("mineffect\\new_crossripperslasher\\new_crossripperslasher_03\\new_crossripperslasher_03.str");
    break;
  case 0x5C9:
    particle->Open("mineffect\\new_crossripperslasher\\new_crossripperslasher_05\\new_crossripperslasher_05.str");
    break;
  case 0x5CA:
    particle->Open("mineffect\\new_crossripperslasher\\new_crossripperslasher_06\\new_crossripperslasher_06.str");
    break;
  case 0x5CB:
    particle->Open("mineffect\\new_crossripperslasher\\new_crossripperslasher_07\\new_crossripperslasher_07.str");
    break;
  case 0x5CC:
    particle->Open("mineffect\\new_crossripperslasher\\new_crossripperslasher_09\\new_crossripperslasher_09.str");
    break;
  case 0x5CD:
    particle->Open("mineffect\\new_crossripperslasher\\new_crossripperslasher_11\\new_crossripperslasher_11.str");
    break;
  case 0x5CE:
    particle->Open("mineffect\\new_armscannon\\new_armscannon_01\\new_armscannon_01.str");
    break;
  case 0x5CF:
    particle->Open("mineffect\\new_armscannon\\new_armscannon_05\\new_armscannon_05.str");
    break;
  case 0x5D0:
    particle->Open("mineffect\\new_armscannon\\new_armscannon_07\\new_armscannon_07.str");
    break;
  case 0x5D1:
    particle->Open("mineffect\\new_armscannon\\new_armscannon_11\\new_armscannon_11.str");
    break;
  case 0x5D2:
    particle->Open("mineffect\\new_armscannon\\new_armscannon_explosion\\armscannon_explosion.str");
    break;
  case 0x5D3:
    particle->Open("mineffect\\new_armscannon\\new_armscannon_explosion_bottom\\armscannon_explosion_bottom.str");
    break;
  case 0x5D4:
    particle->Open("mineffect\\new_armscannon\\new_armscannon_cast\\new_armscannon_cast.str");
    break;
  case 0x5D5:
    particle->Open("mineffect\\new_armscannon\\new_armscannon_cast_bottom\\new_armscannon_cast_bottom.str");
    break;
  case 0x5D6:
    particle->Open("mineffect\\new_axetornado\\new_axetornado\\new_axetornado.str");
    break;
  case 0x5D7:
    particle->Open("mineffect\\new_axetornado\\new_axetornado_bottom\\new_axetornado_bottom.str");
    break;
  case 0x5D8:
    particle->Open("mineffect\\new_cart_cannon\\new_cart_cannon_explosion\\armscannon_explosion.str");
    break;
  case 0x5D9:
    particle->Open("mineffect\\new_cart_cannon\\new_cart_cannon_explosion_bottom\\armscannon_explosion_bottom.str");
    break;
  case 0x5DA:
    particle->Open("mineffect\\new_cart_cannon\\new_cart_cannon_fire_bottom\\new_cartcannon_cast_bottom.str");
    break;
  case 0x5DB:
    particle->Open("mineffect\\new_cart_cannon\\new_cart_cannon_cast\\new_cart_cannon_cast.str");
    break;
  case 0x5DC:
    particle->Open("mineffect\\new_cart_cannon\\new_cart_cannon_cast_bottom\\new_cart_cannon_cast_bottom.str");
    break;
  case 0x5DD:
    particle->Open("mineffect\\new_cart_tornado\\new_cart_tornado\\new_carttornado.str");
    break;
  case 0x5DE:
    particle->Open("mineffect\\new_cart_tornado\\new_cart_tornado_bottom\\new_carttornado_bottom.str");
    break;
  case 0x5DF:
    particle->Open("mineffect\\new_banishingpoint\\new_banishingpoint_target\\new_banishingpoint_target.str");
    break;
  case 0x5E0:
    particle->Open("mineffect\\new_banishingpoint\\new_banishingpoint_target_bottom\\new_banishingpoint_target_bottom.str");
    break;
  case 0x5E1:
    particle->Open("mineffect\\new_banishingpoint\\new_banishingpoint_cast\\new_banishingpoint_cast.str");
    break;
  case 0x5E2:
    particle->Open("mineffect\\new_banishingpoint\\new_banishingpoint_cast_bottom\\new_banishingpoint_cast_combine.str");
    break;
  case 0x5E3:
    particle->Open("mineffect\\new_banishingpoint\\new_banishingpoint_01\\new_banishingpoint_01.str");
    break;
  case 0x5E4:
    particle->Open("mineffect\\new_banishingpoint\\new_banishingpoint_05\\new_banishingpoint_05.str");
    break;
  case 0x5E5:
    particle->Open("mineffect\\new_banishingpoint\\new_banishingpoint_07\\new_banishingpoint_07.str");
    break;
  case 0x5E6:
    particle->Open("mineffect\\new_banishingpoint\\new_banishingpoint_11\\new_banishingpoint_11.str");
    break;
  case 0x5E7:
    particle->Open("mineffect\\new_cannonspear\\new_cannonspear_12\\new_cannonspear_12.str");
    break;
  case 0x5E8:
    particle->Open("mineffect\\new_cannonspear\\new_cannonspear_01\\new_cannonspear_01.str");
    break;
  case 0x5E9:
    particle->Open("mineffect\\new_cannonspear\\new_cannonspear_03\\new_cannonspear_03.str");
    break;
  case 0x5EA:
    particle->Open("mineffect\\new_cannonspear\\new_cannonspear_05\\new_cannonspear_05.str");
    break;
  case 0x5EB:
    particle->Open("mineffect\\new_cannonspear\\new_cannonspear_06\\new_cannonspear_06.str");
    break;
  case 0x5EC:
    particle->Open("mineffect\\new_cannonspear\\new_cannonspear_07\\new_cannonspear_07.str");
    break;
  case 0x5ED:
    particle->Open("mineffect\\new_cannonspear\\new_cannonspear_09\\new_cannonspear_09.str");
    break;
  case 0x5EE:
    particle->Open("mineffect\\new_cannonspear\\new_cannonspear_11\\new_cannonspear_11.str");
    break;
  case 0x5EF:
    particle->Open("mineffect\\new_cannonspear\\new_cannonspear_bottom\\new_cannonspear_bottom.str");
    break;
  case 0x5F0:
    particle->Open("mineffect\\new_dragonbreath\\new_dragonbreath_fire_bomb\\new_dragonbreath_fire_bomb.str");
    break;
  case 0x5F1:
    particle->Open("mineffect\\new_dragonbreath\\new_dragonbreath_fire_01\\new_dragonbreath_fire_01.str");
    break;
  case 0x5F2:
    particle->Open("mineffect\\new_dragonbreath\\new_dragonbreath_fire_05\\new_dragonbreath_fire_05.str");
    break;
  case 0x5F3:
    particle->Open("mineffect\\new_dragonbreath\\new_dragonbreath_fire_07\\new_dragonbreath_fire_07.str");
    break;
  case 0x5F4:
    particle->Open("mineffect\\new_dragonbreath\\new_dragonbreath_fire_11\\new_dragonbreath_fire_11.str");
    break;
  case 0x5F6:
    particle->Open("mineffect\\new_dragonbreath_water\\new_dragonbreath_water_bomb\\new_dragonbreath_water_bomb.str");
    break;
  case 0x5F7:
    particle->Open("mineffect\\new_dragonbreath_water\\new_dragonbreath_water_01\\new_dragonbreath_water_01.str");
    break;
  case 0x5F8:
    particle->Open("mineffect\\new_dragonbreath_water\\new_dragonbreath_water_05\\new_dragonbreath_water_05.str");
    break;
  case 0x5F9:
    particle->Open("mineffect\\new_dragonbreath_water\\new_dragonbreath_water_07\\new_dragonbreath_water_07.str");
    break;
  case 0x5FA:
    particle->Open("mineffect\\new_dragonbreath_water\\new_dragonbreath_water_11\\new_dragonbreath_water_11.str");
    break;
  case 0x5FC:
    particle->Open("mineffect\\new_tigercannon\\new_tigercannon.str");
    break;
  case 0x5FD:
    particle->Open("mineffect\\new_severerainstorm\\new_severerainstorm_cast\\new_severerainstorm_cast.str");
    break;
  case 0x5FE:
    particle->Open("mineffect\\new_comet\\new_comet\\new_comet.str");
    break;
  case 0x5FF:
    particle->Open("mineffect\\new_comet\\new_comet_bottom\\new_comet_bottom.str");
    break;
  case 0x600:
    particle->Open("mineffect\\new_comet\\new_comet_cast_bottom\\new_comet_cast_bottom.str");
    break;
  case 0x601:
    particle->Open("mineffect\\new_comet\\new_comet_sub_explosion\\new_comet_sub_explosion.str");
    break;
  case 0x602:
    particle->Open("mineffect\\new_comet\\new_comet_sub_explosion_bottom\\new_comet_sub_explosion_bottom.str");
    break;
  case 0x603:
    particle->Open("mineffect\\new_comet\\new_comet_cast\\new_comet_cast.str");
    break;
  case 0x604:
    particle->Open("mineffect\\new_jackfrost\\new_jackfrost_ice\\new_jackfrost_00.str");
    break;
  case 0x605:
    particle->Open("mineffect\\new_jackfrost\\new_jackfrost_ice\\new_jackfrost_01.str");
    break;
  case 0x606:
    particle->Open("mineffect\\new_jackfrost\\new_jackfrost_ice\\new_jackfrost_02.str");
    break;
  case 0x607:
    particle->Open("mineffect\\new_jackfrost\\new_jackfrost_cast\\new_jackfrost_cast.str");
    break;
  case 0x608:
    particle->Open("mineffect\\new_jackfrost\\new_jackfrost_magicrune\\new_jackfrost_frost.str");
    break;
  case 0x60A:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_poisonsmoke\\new_poisonsmoke\\new_poisonsmoke_1.str");
    } else {
      particle->Open("new_poisonsmoke\\new_poisonsmoke\\new_poisonsmoke_1.str");
    }
    break;
  case 0x60B:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_poisonsmoke\\new_poisonsmoke\\new_poisonsmoke_2.str");
    } else {
      particle->Open("new_poisonsmoke\\new_poisonsmoke\\new_poisonsmoke_2.str");
    }
    break;
  case 0x60C:
    particle->Open("new_poisonsmoke\\new_poisonsmoke_cast\\new_poisonsmoke_cast.str");
    break;
  case 0x60D:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_fearbreeze\\new_fearbreeze.str");
    } else {
      particle->Open("new_fearbreeze\\new_fearbreeze.str");
    }
    break;
  case 0x60E:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_hellInferno\\new_hellInferno\\new_hellInferno.str");
    } else {
      particle->Open("new_hellInferno\\new_hellInferno\\new_hellInferno.str");
    }
    break;
  case 0x60F:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_hellInferno\\new_hellInferno_bottom\\new_hellInferno_bottom.str");
    } else {
      particle->Open("new_hellInferno\\new_hellInferno_bottom\\new_hellInferno_bottom.str");
    }
    break;
  case 0x610:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_hellInferno\\new_hellInferno_cast_bottom\\new_hellInferno_cast_bottom.str");
    } else {
      particle->Open("new_hellInferno\\new_hellInferno_cast_bottom\\new_hellInferno_cast_bottom.str");
    }
    break;
  case 0x611:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_hellInferno\\new_hellInferno_sub\\new_hellInferno_sub.str");
    } else {
      particle->Open("new_hellInferno\\new_hellInferno_sub\\new_hellInferno_sub.str");
    }
    break;
  case 0x612:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_hellInferno\\new_hellInferno_sub_bottom\\new_hellInferno_sub_bottom.str");
    } else {
      particle->Open("new_hellInferno\\new_hellInferno_sub_bottom\\new_hellInferno_sub_bottom.str");
    }
    break;
  case 0x615:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_clearence\\new_clearence\\new_clearence.str");
    } else {
      particle->Open("new_clearence\\new_clearence\\new_clearence.str");
    }
    break;
  case 0x616:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_clearence\\new_clearence_bottom\\new_clearence_bottom.str");
    } else {
      particle->Open("new_clearence\\new_clearence_bottom\\new_clearence_bottom.str");
    }
    break;
  case 0x617:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_clearence\\new_clearence_cast\\new_clearence_cast.str");
    } else {
      particle->Open("new_clearence\\new_clearence_cast\\new_clearence_cast.str");
    }
    break;
  case 0x619:
    particle->Open("new_varetyrspear\\new_varetyrspear\\new_varetyrspear.str");
    break;
  case 0x61A:
    particle->Open("new_varetyrspear\\new_varetyrspear_bottom\\new_varetyrspear_bottom.str");
    break;
  case 0x61B:
    particle->Open("new_varetyrspear\\new_varetyrspear_cast\\new_varetyrspear_cast.str");
    break;
  case 0x61C:
    particle->Open("new_varetyrspear\\new_varetyrspear_cast_bottom\\new_varetyrspear_cast_bottom.str");
    break;
  case 0x61E:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_crossimpact\\new_crossimpact\\new_crossimpact.str");
    } else {
      particle->Open("new_crossimpact\\new_crossimpact\\new_crossimpact.str");
    }
    break;
  case 0x61F:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_crossimpact\\new_crossimpact_target\\new_crossimpact_target.str");
    } else {
      particle->Open("new_crossimpact\\new_crossimpact_target\\new_crossimpact_target.str");
    }
    break;
  case 0x639:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_reverberation\\new_reverberation_hit\\new_reverberation_hit.str");
    } else {
      particle->Open("new_reverberation\\new_reverberation_hit\\new_reverberation_hit.str");
    }
    break;
  case 0x622:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_fallenempire\\new_fallenempire\\new_fallenempire.str");
    } else {
      particle->Open("new_fallenempire\\new_fallenempire\\new_fallenempire.str");
    }
    break;
  case 0x623:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_fallenempire\\new_fallenempire_bottom\\new_fallenempire_bottom.str");
    } else {
      particle->Open("new_fallenempire\\new_fallenempire_bottom\\new_fallenempire_bottom.str");
    }
    break;
  case 0x624:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_fallenempire\\new_fallenempire_hit\\new_fallenempire_hit.str");
    } else {
      particle->Open("new_fallenempire\\new_fallenempire_hit\\new_fallenempire_hit.str");
    }
    break;
  case 0x625:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_gateofhell\\new_gateofhell_cast\\new_gateofhell_cast.str");
    } else {
      particle->Open("new_gateofhell\\new_gateofhell_cast\\new_gateofhell_cast.str");
    }
    break;
  case 0x626:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_gateofhell\\new_gateofhell_target\\new_gateofhell_target.str");
    } else {
      particle->Open("new_gateofhell\\new_gateofhell_target\\new_gateofhell_target.str");
    }
    break;
  case 0x628:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_howlingoflion\\new_howlingoflion\\new_howlingoflion.str");
    } else {
      particle->Open("new_howlingoflion\\new_howlingoflion\\new_howlingoflion.str");
    }
    break;
  case 0x629:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_howlingoflion\\new_howlingoflion_bottom\\new_howlingoflion_bottom.str");
    } else {
      particle->Open("new_howlingoflion\\new_howlingoflion_bottom\\new_howlingoflion_bottom.str");
    }
    break;
  case 0x62A:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_howlingoflion\\new_howlingoflion_cast\\new_howlingoflion_cast.str");
    } else {
      particle->Open("new_howlingoflion\\new_howlingoflion_cast\\new_howlingoflion_cast.str");
    }
    break;
  case 0x62B:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_howlingoflion\\new_howlingoflion_cast_bottom\\new_howlingoflion_cast_bottom.str");
    } else {
      particle->Open("new_howlingoflion\\new_howlingoflion_cast_bottom\\new_howlingoflion_cast_bottom.str");
    }
    break;
  case 0x62C:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_howlingoflion\\new_howlingoflion_hit\\new_howlingoflion_hit.str");
    } else {
      particle->Open("new_howlingoflion\\new_howlingoflion_hit\\new_howlingoflion_hit.str");
    }
    break;
  case 0x62D:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_metalicsound\\new_metalicsound\\new_metalicsound.str");
    } else {
      particle->Open("new_metalicsound\\new_metalicsound\\new_metalicsound.str");
    }
    break;
  case 0x62E:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_metalicsound\\new_metalicsound_bottom\\new_metalicsound_bottom.str");
    } else {
      particle->Open("new_metalicsound\\new_metalicsound_bottom\\new_metalicsound_bottom.str");
    }
    break;
  case 0x62F:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_metalicsound\\new_metalicsound_hit\\new_metalicsound_hit.str");
    } else {
      particle->Open("new_metalicsound\\new_metalicsound_hit\\new_metalicsound_hit.str");
    }
    break;
  case 0x631:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_reproduce\\new_reproduce\\new_reproduce.str");
    } else {
      particle->Open("new_reproduce\\new_reproduce\\new_reproduce.str");
    }
    break;
  case 0x632:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_reproduce\\new_reproduce_bottom\\new_reproduce_bottom.str");
    } else {
      particle->Open("new_reproduce\\new_reproduce_bottom\\new_reproduce_bottom.str");
    }
    break;
  case 0x633:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_soulexpansion\\new_soulexpansion_cast\\new_soulexpansion_cast.str");
    } else {
      particle->Open("new_soulexpansion\\new_soulexpansion_cast\\new_soulexpansion_cast.str");
    }
    break;
  case 0x634:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_soulexpansion\\new_soulexpansion_cast_bottom\\new_soulexpansion_cast_bottom.str");
    } else {
      particle->Open("new_soulexpansion\\new_soulexpansion_cast_bottom\\new_soulexpansion_cast_bottom.str");
    }
    break;
  case 0x635:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_soulexpansion\\new_soulexpansion_loop\\new_soulexpansion_loop.str");
    } else {
      particle->Open("new_soulexpansion\\new_soulexpansion_loop\\new_soulexpansion_loop.str");
    }
    break;
  case 0x636:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_soulexpansion\\new_soulexpansion_loop_bottom\\new_soulexpansion_loop_bottom.str");
    } else {
      particle->Open("new_soulexpansion\\new_soulexpansion_loop_bottom\\new_soulexpansion_loop_bottom.str");
    }
    break;
  case 0x637:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_soulexpansion\\new_soulexpansion_hit\\new_soulexpansion_hit.str");
    } else {
      particle->Open("new_soulexpansion\\new_soulexpansion_hit\\new_soulexpansion_hit.str");
    }
    break;
  case 0x638:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_soulexpansion\\new_soulexpansion_hit_bottom\\new_soulexpansion_hit_bottom.str");
    } else {
      particle->Open("new_soulexpansion\\new_soulexpansion_hit_bottom\\new_soulexpansion_hit_bottom.str");
    }
    break;
  case 0x63B:
    particle->Open("new_aromaoil\\new_aromaoil\\new_aromaoil_combine.str");
    break;
  case 0x63C:
    particle->Open("new_aromaoil\\new_aromaoil_bottom\\new_aromaoil_bottom.str");
    break;
  case 0x63D:
    particle->Open("new_bathfoam\\new_bathfoam\\new_bathfoam.str");
    break;
  case 0x63E:
    particle->Open("new_bathfoam\\new_bathfoam_bottom\\new_bathfoam_bottom.str");
    break;
  case 0x63F:
    particle->Open("tetravortex\\tetra_ball_00_core\\tetra_ball_00_core.str");
    break;
  case 0x647:
    particle->Open("grade_enchant\\new_intro\\new_intro.str");
    break;
  case 0x648:
    particle->Open("grade_enchant\\new_success\\new_success.str");
    break;
  case 0x649:
    particle->Open("grade_enchant\\new_failed\\new_failed.str");
    break;
  case 0x64B:
    particle->Open("blue_wave\\ef_mon_electric.str");
    break;
  case 0x64C:
    particle->Open("blue_wave\\ef_mon_electric2.str");
    break;
  case 0x64D:
    particle->Open("blue_wave\\ef_mon_electric3.str");
    break;
  case 0x64E:
    particle->Open("red_wave\\ef_mon_explodngwave.str");
    break;
  case 0x64F:
    particle->Open("red_wave\\ef_mon_explodngwave2.str");
    break;
  case 0x650:
    particle->Open("red_wave\\ef_mon_explodngwave3.str");
    break;
  case 0x654:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_wallofthorn\\new_wallofthorn_cast\\new_wallofthorn_cast.str");
    } else {
      particle->Open("new_wallofthorn\\new_wallofthorn_cast\\new_wallofthorn_cast.str");
    }
    break;
  case 0x655:
    particle->Open("new_wallofthorn\\new_wallofthorn_cast_bottom\\new_wallofthorn_cast_bottom.str");
    break;
  case 0x658:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_ignitionbreak\\new_ignitionbreak\\new_ignitionbreak.str");
    } else {
      particle->Open("new_ignitionbreak\\new_ignitionbreak\\new_ignitionbreak.str");
    }
    break;
  case 0x659:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_ignitionbreak\\new_ignitionbreak_bottom\\new_ignitionbreak_bottom.str");
    } else {
      particle->Open("new_ignitionbreak\\new_ignitionbreak_bottom\\new_ignitionbreak_bottom.str");
    }
    break;
  case 0x65A:
    particle->Open("new_ignitionbreak\\new_ignitionbreak_cast\\new_ignitionbreak_cast.str");
    break;
  case 0x65B:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_judex\\new_judex_cast\\new_judex_cast.str");
    } else {
      particle->Open("new_judex\\new_judex_cast\\new_judex_cast.str");
    }
    break;
  case 0x65C:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_judex\\new_judex_cast_bottom\\new_judex_cast_bottom.str");
    } else {
      particle->Open("new_judex\\new_judex_cast_bottom\\new_judex_cast_bottom.str");
    }
    break;
  case 0x65D:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_judex\\new_judex_cast_loop\\new_judex_cast_loop.str");
    } else {
      particle->Open("new_judex\\new_judex_cast_loop\\new_judex_cast_loop.str");
    }
    break;
  case 0x65E:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_judex\\new_judex_cast_loop_bottom\\new_judex_cast_loop_bottom.str");
    } else {
      particle->Open("new_judex\\new_judex_cast_loop_bottom\\new_judex_cast_loop_bottom.str");
    }
    break;
  case 0x65F:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_judex\\new_judex_target\\new_judex_target.str");
    } else {
      particle->Open("new_judex\\new_judex_target\\new_judex_target.str");
    }
    break;
  case 0x660:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_judex\\new_judex_target_bottom\\new_judex_target_bottom.str");
    } else {
      particle->Open("new_judex\\new_judex_target_bottom\\new_judex_target_bottom.str");
    }
    break;
  case 0x661:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_judex\\new_judex_light\\new_judex_light.str");
    } else {
      particle->Open("new_judex\\new_judex_light\\new_judex_light.str");
    }
    break;
  case 0x662:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_judex\\new_judex_light_bottom\\new_judex_light_bottom.str");
    } else {
      particle->Open("new_judex\\new_judex_light_bottom\\new_judex_light_bottom.str");
    }
    break;
  case 0x663:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_chainlightning\\new_chainlightning_cast\\new_chainlightning_cast.str");
    } else {
      particle->Open("new_chainlightning\\new_chainlightning_cast\\new_chainlightning_cast.str");
    }
    break;
  case 0x664:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_chainlightning\\new_chainlightning_cast_bottom\\new_chainlightning_cast_bottom.str");
    } else {
      particle->Open("new_chainlightning\\new_chainlightning_cast_bottom\\new_chainlightning_cast_bottom.str");
    }
    break;
  case 0x665:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_chainlightning\\new_chainlightning_lockon\\new_chainlightning_lockon.str");
    } else {
      particle->Open("new_chainlightning\\new_chainlightning_lockon\\new_chainlightning_lockon.str");
    }
    break;
  case 0x666:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_chainlightning\\new_chainlightning_target\\new_chainlightning_target.str");
    } else {
      particle->Open("new_chainlightning\\new_chainlightning_target\\new_chainlightning_target.str");
    }
    break;
  case 0x667:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_mandragora\\new_mandragora\\new_mandragora.str");
    } else {
      particle->Open("new_mandragora\\new_mandragora\\new_mandragora.str");
    }
    break;
  case 0x668:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_mandragora\\new_mandragora_bottom\\new_mandragora_bottom.str");
    } else {
      particle->Open("new_mandragora\\new_mandragora_bottom\\new_mandragora_bottom.str");
    }
    break;
  case 0x669:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_mandragora\\new_mandragora_cast\\new_mandragora_cast.str");
    } else {
      particle->Open("new_mandragora\\new_mandragora_cast\\new_mandragora_cast.str");
    }
    break;
  case 0x66A:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_soundofdestruction\\new_soundofdestruction_cast\\new_soundofdestruction_cast.str");
    } else {
      particle->Open("new_soundofdestruction\\new_soundofdestruction_cast\\new_soundofdestruction_cast.str");
    }
    break;
  case 0x66B:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_soundofdestruction\\new_soundofdestruction_cast_bottom\\new_soundofdestruction_cast_bottom.str");
    } else {
      particle->Open("new_soundofdestruction\\new_soundofdestruction_cast_bottom\\new_soundofdestruction_cast_bottom.str");
    }
    break;
  case 0x66C:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_soundofdestruction\\new_soundofdestruction_target\\new_soundofdestruction_target.str");
    } else {
      particle->Open("new_soundofdestruction\\new_soundofdestruction_target\\new_soundofdestruction_target.str");
    }
    break;
  case 0x66D:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_aimedbolt\\new_aimedbolt_attack_01clock\\new_aimedbolt_attack_01clock.str");
    } else {
      particle->Open("new_aimedbolt\\new_aimedbolt_attack_01clock\\new_aimedbolt_attack_01clock.str");
    }
    break;
  case 0x66E:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_aimedbolt\\new_aimedbolt_attack_05clock\\new_aimedbolt_attack_05clock.str");
    } else {
      particle->Open("new_aimedbolt\\new_aimedbolt_attack_05clock\\new_aimedbolt_attack_05clock.str");
    }
    break;
  case 0x66F:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_aimedbolt\\new_aimedbolt_attack_07clock\\new_aimedbolt_attack_07clock.str");
    } else {
      particle->Open("new_aimedbolt\\new_aimedbolt_attack_07clock\\new_aimedbolt_attack_07clock.str");
    }
    break;
  case 0x670:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_aimedbolt\\new_aimedbolt_attack_11clock\\new_aimedbolt_attack_11clock.str");
    } else {
      particle->Open("new_aimedbolt\\new_aimedbolt_attack_11clock\\new_aimedbolt_attack_11clock.str");
    }
    break;
  case 0x671:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_aimedbolt\\new_aimedbolt_cast_bottom\\new_aimedbolt_cast_bottom.str");
    } else {
      particle->Open("new_aimedbolt\\new_aimedbolt_cast_bottom\\new_aimedbolt_cast_bottom.str");
    }
    break;
  case 0x672:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_aimedbolt\\new_aimedbolt_target_end\\new_aimedbolt_end.str");
    } else {
      particle->Open("new_aimedbolt\\new_aimedbolt_target_end\\new_aimedbolt_end.str");
    }
    break;
  case 0x673:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_aimedbolt\\new_aimedbolt_target_loop\\new_aimedbolt_target_loop.str");
    } else {
      particle->Open("new_aimedbolt\\new_aimedbolt_target_loop\\new_aimedbolt_target_loop.str");
    }
    break;
  case 0x674:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_unlimit\\new_unlimit\\new_unlimit.str");
    } else {
      particle->Open("new_unlimit\\new_unlimit\\new_unlimit.str");
    }
    break;
  case 0x675:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_unlimit\\new_unlimit_bottom\\new_unlimit_bottom.str");
    } else {
      particle->Open("new_unlimit\\new_unlimit_bottom\\new_unlimit_bottom.str");
    }
    break;
  case 0x676:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_epiclesis\\new_epiclesis_combine.str");
    } else {
      particle->Open("new_epiclesis\\new_epiclesis_combine\\new_epiclesis_combine.str");
    }
    break;
  case 0x67A:
    particle->Open("new_axeboomerang\\new_axeboomerang_cast\\new_axeboomerang_cast.str");
    break;
  case 0x67B:
    particle->Open("new_axeboomerang\\new_axeboomerang_cast_bottom\\new_axeboomerang_cast_bottom.str");
    break;
  case 0x67C:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_axeboomerang\\new_axeboomerang_hit\\new_axeboomerang_hit.str");
    } else {
      particle->Open("new_axeboomerang\\new_axeboomerang_hit\\new_axeboomerang_hit.str");
    }
    break;
  case 0x67D:
    particle->Load("hackandslash\\hackandslash\\hackandslash.str");
    break;
  case 0x67E:
    particle->Load("hackandslash\\hackandslash_bottom\\hackandslash_bottom.str");
    break;
  case 0x67F:
    particle->Load("hackandslash\\hackandslash_hit\\hackandslash_hit.str");
    break;
  case 0x681:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_spellfist\\new_spellfist_fire\\new_spellfist_fire.str");
    } else {
      particle->Open("new_spellfist\\new_spellfist_fire\\new_spellfist_fire.str");
    }
    break;
  case 0x682:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_spellfist\\new_spellfist_fire_bottom\\new_spellfist_fire_bottom.str");
    } else {
      particle->Open("new_spellfist\\new_spellfist_fire_bottom\\new_spellfist_fire_bottom.str");
    }
    break;
  case 0x683:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_spellfist\\new_spellfist_water\\new_spellfist_water.str");
    } else {
      particle->Open("new_spellfist\\new_spellfist_water\\new_spellfist_water.str");
    }
    break;
  case 0x684:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_spellfist\\new_spellfist_water_bottom\\new_spellfist_water_bottom.str");
    } else {
      particle->Open("new_spellfist\\new_spellfist_water_bottom\\new_spellfist_water_bottom.str");
    }
    break;
  case 0x685:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_spellfist\\new_spellfist_wind\\new_spellfist_wind.str");
    } else {
      particle->Open("new_spellfist\\new_spellfist_wind\\new_spellfist_wind.str");
    }
    break;
  case 0x686:
    particle->Load("stormslash\\stormslash\\stormslash_1.str");
    break;
  case 0x687:
    particle->Load("stormslash\\stormslash\\stormslash_2.str");
    break;
  case 0x688:
    particle->Load("stormslash\\stormslash\\stormslash_3.str");
    break;
  case 0x689:
    particle->Load("stormslash\\stormslash\\stormslash_4.str");
    break;
  case 0x68A:
    particle->Load("stormslash\\stormslash\\stormslash_5.str");
    break;
  case 0x68B:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_cartboost\\new_cartboost\\new_cartboost.str");
    } else {
      particle->Open("new_cartboost\\new_cartboost\\new_cartboost.str");
    }
    break;
  case 0x68C:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_cartboost\\new_cartboost_bottom\\new_cartboost_bottom.str");
    } else {
      particle->Open("new_cartboost\\new_cartboost_bottom\\new_cartboost_bottom.str");
    }
    break;
  case 0x68F:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_overbrand\\new_overbrand_01_clock\\new_overbrand_01_clock.str");
    } else {
      particle->Open("new_overbrand\\new_overbrand_01_clock\\new_overbrand_01_clock.str");
    }
    break;
  case 0x690:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_overbrand\\new_overbrand_03_clock\\new_overbrand_03_clock.str");
    } else {
      particle->Open("new_overbrand\\new_overbrand_03_clock\\new_overbrand_03_clock.str");
    }
    break;
  case 0x691:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_overbrand\\new_overbrand_05_clock\\new_overbrand_05_clock.str");
    } else {
      particle->Open("new_overbrand\\new_overbrand_05_clock\\new_overbrand_05_clock.str");
    }
    break;
  case 0x692:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_overbrand\\new_overbrand_06_clock\\new_overbrand_06_clock.str");
    } else {
      particle->Open("new_overbrand\\new_overbrand_06_clock\\new_overbrand_06_clock.str");
    }
    break;
  case 0x693:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_overbrand\\new_overbrand_07_clock\\new_overbrand_07_clock.str");
    } else {
      particle->Open("new_overbrand\\new_overbrand_07_clock\\new_overbrand_07_clock.str");
    }
    break;
  case 0x694:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_overbrand\\new_overbrand_09_clock\\new_overbrand_09_clock.str");
    } else {
      particle->Open("new_overbrand\\new_overbrand_09_clock\\new_overbrand_09_clock.str");
    }
    break;
  case 0x695:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_overbrand\\new_overbrand_11_clock\\new_overbrand_11_clock.str");
    } else {
      particle->Open("new_overbrand\\new_overbrand_11_clock\\new_overbrand_11_clock.str");
    }
    break;
  case 0x696:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_overbrand\\new_overbrand_12_clock\\new_overbrand_12_clock.str");
    } else {
      particle->Open("new_overbrand\\new_overbrand_12_clock\\new_overbrand_12_clock.str");
    }
    break;
  case 0x697:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_overbrand\\new_overbrand_cast\\new_overbrand_cast.str");
    } else {
      particle->Open("new_overbrand\\new_overbrand_cast\\new_overbrand_cast.str");
    }
    break;
  case 0x698:
    particle->Open("new_overbrand\\new_overbrand_cast_bottom\\new_overbrand_cast_bottom.str");
    break;
  case 0x699:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_overbrand\\new_overbrand_hit\\new_overbrand_hit.str");
    } else {
      particle->Open("new_overbrand\\new_overbrand_hit\\new_overbrand_hit.str");
    }
    break;
  case 0x69A:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_windcutter\\new_windcutter\\new_windcutter.str");
    } else {
      particle->Open("new_windcutter\\new_windcutter\\new_windcutter.str");
    }
    break;
  case 0x69B:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_windcutter\\new_windcutter_bottom_01clock\\new_windcutter_bottom_01clock.str");
    } else {
      particle->Open("new_windcutter\\new_windcutter_bottom_01clock\\new_windcutter_bottom_01clock.str");
    }
    break;
  case 0x69C:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_windcutter\\new_windcutter_bottom_05clock\\new_windcutter_bottom_05clock.str");
    } else {
      particle->Open("new_windcutter\\new_windcutter_bottom_05clock\\new_windcutter_bottom_05clock.str");
    }
    break;
  case 0x69D:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_windcutter\\new_windcutter_target\\new_windcutter_target.str");
    } else {
      particle->Open("new_windcutter\\new_windcutter_target\\new_windcutter_target.str");
    }
    break;
  case 0x69F:
    particle->Load("new_servantweapon\\new_servantweapon_hit\\new_servantweapon_hit.str");
    break;
  case 0x6A0:
    particle->Load("new_servantweapon\\new_servantweapon_hit_bottom\\new_servantweapon_hit_bottom.str");
    break;
  case 0x6A1:
    particle->Load("new_servantweapon\\new_servantweapon_shoot_bottom\\new_servantweapon_shoot_bottom.str");
    break;
  case 0x6A2:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_powerswing\\new_powerswing_01_clock\\new_powerswing_01_clock.str");
    } else {
      particle->Open("new_powerswing\\new_powerwing_01_clock\\new_powerwing_01_clock.str");
    }
    break;
  case 0x6A3:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_powerswing\\new_powerswing_05_clock\\new_powerswing_05_clock.str");
    } else {
      particle->Open("new_powerswing\\new_powerwing_05_clock\\new_powerwing_05_clock.str");
    }
    break;
  case 0x6A4:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_powerswing\\new_powerswing_07_clock\\new_powerswing_07_clock.str");
    } else {
      particle->Open("new_powerswing\\new_powerwing_07_clock\\new_powerwing_07_clock.str");
    }
    break;
  case 0x6A5:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_powerswing\\new_powerswing_11_clock\\new_powerswing_11_clock.str");
    } else {
      particle->Open("new_powerswing\\new_powerwing_11_clock\\new_powerwing_11_clock.str");
    }
    break;
  case 0x6A6:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_powerswing\\new_powerswing_bottom\\new_powerswing_bottom.str");
    } else {
      particle->Open("new_powerswing\\new_powerswing_bottom\\new_powerswing_bottom.str");
    }
    break;
  case 0x6A7:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_vulcanarm\\new_vulcanarm\\new_vulcanarm.str");
    } else {
      particle->Open("new_vulcanarm\\new_vulcanarm\\new_vulcanarm.str");
    }
    break;
  case 0x6A8:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_vulcanarm\\new_vulcanarm_bottom\\new_vulcanarm_bottom.str");
    } else {
      particle->Open("new_vulcanarm\\new_vulcanarm_bottom\\new_vulcanarm_bottom.str");
    }
    break;
  case 0x6A9:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_vulcanarm\\new_vulcanarm_shothole\\new_vulcanarm_shothole.str");
    } else {
      particle->Open("new_vulcanarm\\new_vulcanarm_shothole\\new_vulcanarm_shothole.str");
    }
    break;
  case 0x6AA:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_earthdrive\\new_earthdrive\\new_earthdrive.str");
    } else {
      particle->Open("new_earthdrive\\new_earthdrive\\new_earthdrive.str");
    }
    break;
  case 0x6AB:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_earthdrive\\new_earthdrive_bottom\\new_earthdrive_bottom.str");
    } else {
      particle->Open("new_earthdrive\\new_earthdrive_bottom\\new_earthdrive_bottom.str");
    }
    break;
  case 0x6AC:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_earthdrive\\new_earthdrive_target\\new_earthdrive_target.str");
    } else {
      particle->Open("new_earthdrive\\new_earthdrive_target\\new_earthdrive_target.str");
    }
    break;
  case 0x6AE:
    particle->Load("servant_sign\\servant_sign\\servant_sign.str");
    break;
  case 0x8E8:
    particle->Load("servant_sign\\servant_sign_bottom\\servant_sign_bottom.str");
    break;
  case 0x6AF:
    particle->Load("servant_sign\\servant_sign_down\\servant_sign_down.str");
    break;
  case 0x6B0:
    particle->Load("servant_sign\\servant_sign_down_bottom\\servant_sign_down_bottom.str");
    break;
  case 0x6B1:
    particle->Load("servant_phantom\\servant_phantom_down\\servant_phantom_down.str");
    break;
  case 0x6B2:
    particle->Load("servant_phantom\\servant_phantom_hit\\servant_phantom_hit.str");
    break;
  case 0x6B3:
    particle->Load("servant_phantom\\servant_phantom_hit_bottom\\servant_phantom_hit_bottom.str");
    break;
  case 0x6B4:
    particle->Load("servant_phantom\\servant_phantom_sub\\servant_phantom_sub.str");
    break;
  case 0x6B5:
    particle->Load("servant_phantom\\servant_phantom_sub_bottom\\servant_phantom_sub_bottom.str");
    break;
  case 0x6B6:
    particle->Load("servant_demolition\\servant_demolition_collect\\servant_demolition_collect.str");
    break;
  case 0x6B7:
    particle->Load("servant_demolition\\servant_demolition_hit\\servant_demolition_hit.str");
    break;
  case 0x6B8:
    particle->Load("servant_demolition\\servant_demolition_leftdown\\servant_demolition_leftdown.str");
    break;
  case 0x6B9:
    particle->Load("servant_demolition\\servant_demolition_rightdown\\servant_demolition_rightdown.str");
    break;
  case 0x6BA:
    particle->Load("dragonnic_ora\\dragonnic_ora\\dragonnic_ora.str");
    break;
  case 0x6BB:
    particle->Load("dragonnic_ora\\dragonnic_ora_bottom\\dragonnic_ora_bottom.str");
    break;
  case 0x6BC:
    particle->Load("dragonnic_ora\\dragonnic_ora_hit\\dragonnic_ora_hit.str");
    break;
  case 0x6BD:
    particle->Load("dragonnic_ora\\dragonnic_ora_hit_bottom\\dragonnic_ora_hit_bottom.str");
    break;
  case 0x6BE:
    particle->Load("dragonnic_ora\\dragonnic_ora_buff\\dragonnic_ora_buff.str");
    break;
  case 0x6BF:
    particle->Load("dragonnic_ora\\dragonnic_ora_buff_bottom\\dragonnic_ora_buff_bottom.str");
    break;
  case 0x6C0:
    particle->Load("madness_crusher\\madness_crusher\\madness_crusher.str");
    break;
  case 0x6C1:
    particle->Load("madness_crusher\\madness_crusher_bottom\\madness_crusher_bottom.str");
    break;
  case 0x6D6:
    particle->Load("vigor\\vigor_cast\\vigor_cast.str");
    break;
  case 0x6D7:
    particle->Load("vigor\\vigor_buff\\vigor_buff.str");
    break;
  case 0x6C2:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_autoshadowspell\\new_autoshadowspell\\new_autoshadowspell.str");
    } else {
      particle->Open("new_autoshadowspell\\new_autoshadowspell\\new_autoshadowspell.str");
    }
    break;
  case 0x6C3:
    if (g_serviceOptions.Get(229)) {
      particle->Open("mineffect\\new_autoshadowspell\\new_autoshadowspell_bottom\\new_autoshadowspell_bottom.str");
    } else {
      particle->Open("new_autoshadowspell\\new_autoshadowspell_bottom\\new_autoshadowspell_bottom.str");
    }
    break;
  case 0x6C4:
    particle->Load("destructive_hurricane\\destructive_hurricane\\destructive_hurricane.str");
    break;
  case 0x6C5:
    particle->Load("destructive_hurricane\\destructive_hurricane_bottom\\destructive_hurricane_bottom.str");
    break;
  case 0x6C6:
    particle->Load("destructive_hurricane\\destructive_hurricane_cast\\destructive_hurricane_cast.str");
    break;
  case 0x6C7:
    particle->Load("destructive_hurricane\\destructive_hurricane_cast_bottom\\destructive_hurricane_cast_bottom.str");
    break;
  case 0x6C8:
    particle->Load("destructive_hurricane\\destructive_hurricane_climax\\destructive_hurricane_climax.str");
    break;
  case 0x6C9:
    particle->Load("destructive_hurricane\\destructive_hurricane_hit\\destructive_hurricane_hit.str");
    break;
  case 0x6CA:
    particle->Load("deadly_projection\\deadly_projection_cast\\deadly_projection_cast.str");
    break;
  case 0x6CB:
    particle->Load("deadly_projection\\deadly_projection_cast_bottom\\deadly_projection_cast_bottom.str");
    break;
  case 0x6CC:
    particle->Load("deadly_projection\\deadly_projection_hit\\deadly_projection_hit.str");
    break;
  case 0x6CD:
    particle->Load("deadly_projection\\efst_deadly_defeasance\\efst_deadly_defeasance.str");
    break;
  case 0x6CE:
    particle->Load("soul_vc_strike\\soul_vc_strike_cast\\soul_vc_strike_cast.str");
    break;
  case 0x6CF:
    particle->Load("soul_vc_strike\\soul_vc_strike_cast_bottom\\soul_vc_strike_cast_bottom.str");
    break;
  case 0x6D0:
    particle->Load("soul_vc_strike\\soul_vc_strike_hit\\soul_vc_strike_hit.str");
    break;
  case 0x6D1:
    particle->Load("rockdown\\rockdown\\rockdown.str");
    break;
  case 0x6D2:
    particle->Load("rockdown\\rockdown_bottom\\rockdown_bottom.str");
    break;
  case 0x6D3:
    particle->Load("rockdown\\rockdown_cast\\rockdown_cast.str");
    break;
  case 0x6D4:
    particle->Load("rockdown\\rockdown_cast_bottom\\rockdown_cast_bottom.str");
    break;
  case 0x6D5:
    particle->Load("rockdown\\rockdown_hit\\rockdown_hit.str");
    break;
  case 0x6D9:
    particle->Load("rain_of_crystal\\rain_of_crystal_casting\\rain_of_crystal_casting.str");
    break;
  case 0x6DA:
    particle->Load("rain_of_crystal\\rain_of_crystal_casting_bottom\\rain_of_crystal_casting_bottom.str");
    break;
  case 0x6DD:
    particle->Load("crimsonarrow\\crimsonarrow_cast\\crimsonarrow_cast.str");
    break;
  case 0x6DE:
    particle->Load("crimsonarrow\\crimsonarrow_cast_bottom\\crimsonarrow_cast_bottom.str");
    break;
  case 0x6DF:
    particle->Load("crimsonarrow\\crimsonarrow_hit\\crimsonarrow_hit.str");
    break;
  case 0x6E1:
    particle->Load("violentquake\\violentquake_cast\\violentquake_cast.str");
    break;
  case 0x6E2:
    particle->Load("violentquake\\violentquake_cast_bottom\\violentquake_cast_bottom.str");
    break;
  case 0x6E3:
    particle->Load("violentquake\\violentquake_hit\\violentquake_hit.str");
    break;
  case 0x701:
    particle->Load("violentquake\\violentquake_bottom\\violentquake_bottom.str");
    break;
  case 0x6E5:
    particle->Load("new_chargingpierce\\new_chargingpierce_cast\\new_chargingpierce_cast.str");
    break;
  case 0x6E6:
    particle->Load("new_chargingpierce\\new_chargingpierce_cast_bottom\\new_chargingpierce_cast_bottom.str");
    break;
  case 0x6E7:
    particle->Load("new_chargingpierce\\new_chargingpierce_hit\\new_chargingpierce_hit.str");
    break;
  case 0x6E9:
    particle->Load("astralstrike\\astralstrike_cast\\astralstrike_cast.str");
    break;
  case 0x6EA:
    particle->Load("astralstrike\\astralstrike_cast_bottom\\astralstrike_cast_bottom.str");
    break;
  case 0x6EB:
    particle->Load("mystery_illusion\\mystery_illusion\\mystery_illusion.str");
    break;
  case 0x6EC:
    particle->Load("mystery_illusion\\mystery_illusion_bottom\\mystery_illusion_bottom.str");
    break;
  case 0x6ED:
    particle->Load("mystery_illusion\\mystery_illusion_cast\\mystery_illusion_cast.str");
    break;
  case 0x6EE:
    particle->Load("mystery_illusion\\mystery_illusion_cast_bottom\\mystery_illusion_cast_bottom.str");
    break;
  case 0x6EF:
    particle->Load("mystery_illusion\\mystery_illusion_hit\\mystery_illusion_hit.str");
    break;
  case 0x6F0:
    particle->Load("tornadostorm\\tornadostorm\\tornadostorm.str");
    break;
  case 0x6F1:
    particle->Load("tornadostorm\\tornadostorm_bottom\\tornadostorm_bottom.str");
    break;
  case 0x6F2:
    particle->Load("tornadostorm\\tornadostorm_cast\\tornadostorm_cast.str");
    break;
  case 0x6F3:
    particle->Load("tornadostorm\\tornadostorm_cast_bottom\\tornadostorm_cast_bottom.str");
    break;
  case 0x6F4:
    particle->Load("tornadostorm\\tornadostorm_hit\\tornadostorm_hit.str");
    break;
  case 0x6F5:
    particle->Load("frozen_slash\\frozen_slash\\frozen_slash.str");
    break;
  case 0x6F6:
    particle->Load("frozen_slash\\frozen_slash_casting\\frozen_slash_cast.str");
    break;
  case 0x6F7:
    particle->Load("frozen_slash\\frozen_slash_casting_bottom\\frozen_slash_cast_bottom.str");
    break;
  case 0x6F8:
    particle->Load("frozen_slash\\frozen_slash_hit\\frozen_slash_hit.str");
    break;
  case 0x6F9:
    particle->Load("frozen_slash\\frozen_slash_ice\\frozen_slash_ice_01.str");
    break;
  case 0x6FA:
    particle->Load("frozen_slash\\frozen_slash_ice\\frozen_slash_ice_02.str");
    break;
  case 0x6FB:
    particle->Load("frozen_slash\\frozen_slash_ice\\frozen_slash_ice_03.str");
    break;
  case 0x6FC:
    particle->Load("frozen_slash\\frozen_slash_ice\\frozen_slash_ice_04.str");
    break;
  case 0x6FD:
    particle->Load("frozen_slash\\frozen_slash_ice\\frozen_slash_ice_05.str");
    break;
  case 0x6FE:
    particle->Load("frozen_slash\\frozen_slash_ice\\frozen_slash_ice_06.str");
    break;
  case 0x6FF:
    particle->Load("frozen_slash\\frozen_slash_ice\\frozen_slash_ice_07.str");
    break;
  case 0x700:
    particle->Load("frozen_slash\\frozen_slash_ice\\frozen_slash_ice_08.str");
    break;
  case 0x702:
    particle->Load("allbloom\\allbloom\\allbloom.str");
    break;
  case 0x703:
    particle->Load("allbloom\\allbloom_bottom\\allbloom_bottom.str");
    break;
  case 0x704:
    particle->Load("allbloom\\allbloom_cast\\allbloom_cast.str");
    break;
  case 0x705:
    particle->Load("allbloom\\allbloom_cast_bottom\\allbloom_cast_bottom.str");
    break;
  case 0x707:
    particle->Load("allbloom\\allbloom_hit\\allbloom_hit.str");
    break;
  case 0x709:
    particle->Load("climax\\climax_light\\climax_light.str");
    break;
  case 0x70A:
    particle->Load("climax\\climax_cast\\climax_cast.str");
    break;
  case 0x70B:
    particle->Load("climax\\climax_cast_loop\\climax_cast_loop.str");
    break;
  case 0x70D:
    particle->Load("froral_flareroad\\froral_flareroad_cast\\froral_flareroad_cast.str");
    break;
  case 0x70E:
    particle->Load("froral_flareroad\\froral_flareroad_cast_bottom\\froral_flareroad_cast_bottom.str");
    break;
  case 0x70F:
    particle->Load("froral_flareroad\\froral_flareroad_hit\\froral_flareroad_hit.str");
    break;
  case 0x710:
    particle->Load("medialevotum\\medialevotum\\medialevotum.str");
    break;
  case 0x711:
    particle->Load("medialevotum\\medialevotum_cast\\medialevotum_cast.str");
    break;
  case 0x712:
    particle->Open("medialevotum\\medialevotum_loop\\medialevotum_loop.str");
    break;
  case 0x713:
    particle->Load("argutusvita\\argutusvita\\argutusvita.str");
    break;
  case 0x714:
    particle->Load("argutusvita\\argutusvita_cast\\argutusvita_cast.str");
    break;
  case 0x715:
    particle->Load("argutustelum\\argutustelum\\argutustelum.str");
    break;
  case 0x716:
    particle->Load("argutustelum\\argutustelum_cast\\argutustelum_cast.str");
    break;
  case 0x717:
    particle->Load("presensacies\\presensacies\\presensacies.str");
    break;
  case 0x718:
    particle->Load("presensacies\\presensacies_cast\\presensacies_cast.str");
    break;
  case 0x76D:
    particle->Load("new_abr_mother\\new_abr_mother_cast\\new_abr_mother_cast.str");
    break;
  case 0x76F:
    particle->Load("new_abr_mother\\new_abr_mother_01\\new_abr_mother_01.str");
    break;
  case 0x770:
    particle->Load("new_abr_mother\\new_abr_mother_02\\new_abr_mother_02.str");
    break;
  case 0x719:
    particle->Load("new_d_machine\\new_d_machine\\new_d_machine.str");
    break;
  case 0x71A:
    particle->Load("new_d_machine\\new_d_machine_cast\\new_d_machine_cast.str");
    break;
  case 0x71B:
    particle->Load("new_d_machine\\new_d_machine_cast_bottom\\new_d_machine_cast_bottom.str");
    break;
  case 0x71C:
    particle->Load("new_d_machine\\new_d_machine_start_end\\new_d_machine_start.str");
    break;
  case 0x71D:
    particle->Load("new_d_machine\\new_d_machine_start_end\\new_d_machine_end.str");
    break;
  case 0x730:
    particle->Load("a_machine\\a_machine_cast\\a_machine_cast.str");
    break;
  case 0x732:
    particle->Load("a_machine\\a_machine_hit\\a_machine_hit.str");
    break;
  case 0x795:
    particle->Load("new_axe_stomp\\new_axe_stomp\\new_axe_stomp.str");
    break;
  case 0x793:
    particle->Load("new_axe_stomp\\new_axe_stomp_hit\\new_axe_stomp_hit.str");
    break;
  case 0x794:
    particle->Load("new_axe_stomp\\new_axe_stomp_bottom\\new_axe_stomp_bottom.str");
    break;
  case 0x796:
    particle->Load("new_axe_stomp\\new_axe_stomp_12c\\new_axe_stomp_12c.str");
    break;
  case 0x797:
    particle->Load("new_axe_stomp\\new_axe_stomp_01c\\new_axe_stomp_01c.str");
    break;
  case 0x798:
    particle->Load("new_axe_stomp\\new_axe_stomp_03c\\new_axe_stomp_03.str");
    break;
  case 0x799:
    particle->Load("new_axe_stomp\\new_axe_stomp_05c\\new_axe_stomp_05c.str");
    break;
  case 0x79A:
    particle->Load("new_axe_stomp\\new_axe_stomp_06c\\new_axe_stomp_06c.str");
    break;
  case 0x79B:
    particle->Load("new_axe_stomp\\new_axe_stomp_08c\\new_axe_stomp_08c.str");
    break;
  case 0x79C:
    particle->Load("new_axe_stomp\\new_axe_stomp_09c\\new_axe_stomp_09c.str");
    break;
  case 0x79D:
    particle->Load("new_axe_stomp\\new_axe_stomp_11c\\new_axe_stomp_11c.str");
    break;
  case 0x75C:
    particle->Load("new_rush_quake\\new_rush_quake\\new_rush_quake.str");
    break;
  case 0x75D:
    particle->Load("new_rush_quake\\new_rush_quake_bottom\\new_rush_quake.str");
    break;
  case 0x75E:
    particle->Load("new_rush_quake\\new_rush_quake_hit\\new_rush_quake_hit.str");
    break;
  case 0x720:
    particle->Load("arbitrium\\arbitrium\\arbitrium.str");
    break;
  case 0x721:
    particle->Load("arbitrium\\arbitrium_bottom\\arbitrium_bottom.str");
    break;
  case 0x71E:
    particle->Open("arbitrium\\arbitrium_cast\\arbitrium_cast.str");
    break;
  case 0x71F:
    particle->Load("arbitrium\\arbitrium_cast_bottom\\arbitrium_cast_bottom.str");
    break;
  case 0x722:
    particle->Load("arbitrium\\arbitrium_hit\\arbitrium_hit.str");
    break;
  case 0x723:
    particle->Load("dancing_knife\\dancing_knife_cast\\dancing_knife_cast.str");
    break;
  case 0x724:
    particle->Load("dancing_knife\\dancing_knife_cast_bottom\\dancing_knife_cast_bottom.str");
    break;
  case 0x726:
    particle->Load("abyss_dagger\\abyss_dagger_hit\\abyss_dagger_hit.str");
    break;
  case 0x72E:
    particle->Load("eternal_slash\\eternal_slash_hit\\eternal_slash_hit.str");
    break;
  case 0x727:
    particle->Open("new_reparatio\\new_reparatio_cast\\new_reparatio_cast.str");
    break;
  case 0x728:
    particle->Load("new_reparatio\\new_reparatio_cast_bottom\\new_reparatio_cast_bottom.str");
    break;
  case 0x729:
    particle->Load("new_reparatio\\new_reparatio\\new_reparatio.str");
    break;
  case 0x72A:
    particle->Load("new_reparatio\\new_reparatio_bottom\\new_reparatio_bottom.str");
    break;
  case 0x72B:
    particle->Load("new_reparatio\\new_reparatio_target\\new_reparatio_target.str");
    break;
  case 0x72C:
    particle->Open("new_reparatio\\new_reparatio_target_bottom\\new_reparatio_target_bottom.str");
    break;
  case 0x736:
    particle->Load("effligo\\effligo\\effligo.str");
    break;
  case 0x737:
    particle->Load("effligo\\effligo_bottom\\effligo_bottom.str");
    break;
  case 0x738:
    particle->Load("effligo\\effligo_hit\\effligo_hit.str");
    break;
  case 0x739:
    particle->Load("competentia\\competentia\\competentia.str");
    break;
  case 0x73A:
    particle->Open("competentia\\competentia_bottom\\competentia_bottom.str");
    break;
  case 0x73C:
    particle->Open("competentia\\competentia_loop\\competentia_loop.str");
    break;
  case 0x73D:
    particle->Load("competentia\\competentia_loop_bottom\\competentia_loop_bottom.str");
    break;
  case 0x7A6:
    particle->Load("pneumaticusprocella\\pneumaticusprocella_cast\\pneumaticusprocella_cast.str");
    break;
  case 0x789:
    particle->Load("new_pramen\\new_pramen_bottom\\new_pramen_bottom.str");
    break;
  case 0x78A:
    particle->Load("new_pramen\\new_pramen_cast\\new_pramen_cast.str");
    break;
  case 0x78B:
    particle->Load("new_pramen\\new_pramen\\new_pramen.str");
    break;
  case 0x784:
    particle->Load("new_dilectioheal\\new_dilectioheal\\new_dilectioheal.str");
    break;
  case 0x785:
    particle->Open("new_dilectioheal\\new_dilectioheal_cast\\new_dilectioheal_cast.str");
    break;
  case 0x786:
    particle->Load("new_dilectioheal\\new_dilectioheal_cast_bottom\\new_dilectioheal_cast_bottom.str");
    break;
  case 0x787:
    particle->Load("new_dilectioheal\\new_dilectioheal_target\\new_dilectioheal_target.str");
    break;
  case 0x788:
    particle->Load("new_dilectioheal\\new_dilectioheal_target_bottom\\new_dilectioheal_target_bottom.str");
    break;
  case 0x73E:
    particle->Load("crystal_impact\\crystal_impact\\crystal_impact.str");
    break;
  case 0x73F:
    particle->Load("crystal_impact\\crystal_impact_bottom\\crystal_impact_bottom.str");
    break;
  case 0x740:
    particle->Load("crystal_impact\\crystal_impact_cast\\crystal_impact_cast.str");
    break;
  case 0x741:
    particle->Load("crystal_impact\\crystal_impact_cast_bottom\\crystal_impact_cast_bottom.str");
    break;
  case 0x742:
    particle->Load("crystal_impact\\crystal_impact_hit\\crystal_impact_hit.str");
    break;
  case 0x743:
    particle->Load("crystal_impact\\crystal_impact_buff\\crystal_impact_buff.str");
    break;
  case 0x746:
    particle->Load("enchanting_shadow\\enchanting_shadow\\new_enchanting_shadow.str");
    break;
  case 0x747:
    particle->Load("enchanting_shadow\\enchanting_shadow_bottom\\new_enchanting_shadow_bottom.str");
    break;
  case 0x782:
    particle->Load("flametrap\\flametrap_hit\\flametrap_hit.str");
    break;
  case 0x781:
    particle->Load("flametrap\\flametrap_cast\\flametrap_cast.str");
    break;
  case 0x77F:
    particle->Load("flametrap\\flametrap\\flametrap.str");
    break;
  case 0x744:
    particle->Load("religio\\religio\\religio.str");
    break;
  case 0x745:
    particle->Load("religio\\religio_cast\\religio_cast.str");
    break;
  case 0x748:
    particle->Load("fatal_shadow_crow\\fatal_shadow_crow\\fatal_shadow_crow.str");
    break;
  case 0x749:
    particle->Load("fatal_shadow_crow\\fatal_shadow_crow_bottom\\fatal_shadow_crow_bottom.str");
    break;
  case 0x74A:
    particle->Load("fatal_shadow_crow\\fatal_shadow_crow_hit\\fatal_shadow_crow_hit.str");
    break;
  case 0x74C:
    particle->Load("shadow_exceed\\shadow_exceed_cast\\shadow_exceed_cast.str");
    break;
  case 0x752:
    particle->Load("deepblindtrap\\deepblindtrap\\deepblindtrap.str");
    break;
  case 0x751:
    particle->Load("deepblindtrap\\deepblindtrap_cast\\deepblindtrap_cast.str");
    break;
  case 0x753:
    particle->Load("deepblindtrap\\deepblindtrap_hit\\deepblindtrap_hit.str");
    break;
  case 0x7A8:
    particle->Load("4wh_calumitygale\\calumitygale\\calumitygale.str");
    break;
  case 0x7A9:
    particle->Load("4wh_calumitygale\\calumitygale_cast\\calumitygale_cast.str");
    break;
  case 0x7AA:
    particle->Load("4wh_calumitygale\\calumitygale_loop\\calumitygale_loop.str");
    break;
  case 0x756:
    particle->Load("swifttrap\\swifttrap\\swifttrap.str");
    break;
  case 0x755:
    particle->Load("swifttrap\\swifttrap_cast\\swifttrap_cast.str");
    break;
  case 0x757:
    particle->Load("swifttrap\\swifttrap_hit\\swifttrap_hit.str");
    break;
  case 0x75A:
    particle->Load("solidtrap\\solidtrap\\solidtrap.str");
    break;
  case 0x758:
    particle->Load("solidtrap\\solidtrap_cast\\solidtrap_cast.str");
    break;
  case 0x759:
    particle->Load("solidtrap\\solidtrap_cast_bottom\\solidtrap_cast_bottom.str");
    break;
  case 0x75B:
    particle->Load("solidtrap\\solidtrap_hit\\solidtrap_hit.str");
    break;
  case 0x75F:
    particle->Load("benedictum\\benedictum\\benedictum.str");
    break;
  case 0x760:
    particle->Open("benedictum\\benedictum_cast\\benedictum_cast.str");
    break;
  case 0x761:
    particle->Load("benedictum\\benedictum_cast_bottom\\benedictum_cast_bottom.str");
    break;
  case 0x762:
    particle->Load("strantumtremor\\strantumtremor_cast\\strantumtremor_cast.str");
    break;
  case 0x763:
    particle->Load("strantumtremor\\strantumtremor_cast_bottom\\strantumtremor_cast_bottom.str");
    break;
  case 0x764:
    particle->Load("strantumtremor\\strantumtremor_hit\\strantumtremor_hit.str");
    break;
  case 0x765:
    particle->Load("strantumtremor\\strantumtremor\\strantumtremor.str");
    break;
  case 0x766:
    particle->Load("strantumtremor\\strantumtremor_bottom\\strantumtremor_bottom.str");
    break;
  case 0x767:
    particle->Load("impact_crater\\impact_crater\\impact_crater.str");
    break;
  case 0x768:
    particle->Load("impact_crater\\impact_crater_bottom\\impact_crater_bottom.str");
    break;
  case 0x76A:
    particle->Load("impact_crater\\impact_crater_hit\\impact_crater_hit.str");
    break;
  case 0x76B:
    particle->Load("windsign\\windsign\\windsign.str");
    break;
  case 0x76C:
    particle->Load("windsign\\windsign_loop\\windsign_loop.str");
    break;
  case 0x772:
    particle->Load("new_abr_infinity\\new_abr_infinity\\new_abr_infinity.str");
    break;
  case 0x773:
    particle->Load("new_abr_infinity\\new_abr_infinity_bottom\\new_abr_infinity_bottom.str");
    break;
  case 0x774:
    particle->Load("new_abr_infinity\\new_abr_infinity_cast\\new_abr_infinity_cast.str");
    break;
  case 0x775:
    particle->Load("new_abr_infinity\\new_abr_infinity_hit\\new_abr_infinity_hit.str");
    break;
  case 0x78E:
    particle->Load("new_battle_w\\new_battle_w\\new_battle_w.str");
    break;
  case 0x78F:
    particle->Load("new_battle_w\\new_battle_w_cast\\new_battle_w_cast.str");
    break;
  case 0x790:
    particle->Load("new_battle_w\\new_battle_w_bottom\\new_battle_w_bottom.str");
    break;
  case 0x791:
    particle->Load("new_battle_w\\new_battle_w_hit\\new_battle_w_hit.str");
    break;
  case 0x77B:
    particle->Load("new_d_cannon\\new_d_cannon\\new_d_cannon.str");
    break;
  case 0x77C:
    particle->Load("new_d_cannon\\new_d_cannon_cast\\new_d_cannon_cast.str");
    break;
  case 0x77D:
    particle->Load("new_d_cannon\\new_d_cannon_hit\\new_d_cannon_hit.str");
    break;
  case 0x771:
    particle->Load("hawkrush\\hawkrush\\hawkrush.str");
    break;
  case 0x77A:
    particle->Load("eternal_slash\\eternal_slash_hit\\eternal_slash_hit.str");
    break;
  case 0x78D:
    particle->Load("new_potent_venom\\new_potent_venom.str");
    break;
  case 0x79E:
    particle->Load("crescivebolt\\crescivebolt_cast\\crescivebolt_cast.str");
    break;
  case 0x7A0:
    particle->Load("crescivebolt\\crescivebolt_hit\\crescivebolt_hit.str");
    break;
  case 0x7A1:
    particle->Load("shadow_stab\\shadow_stab\\shadow_stab.str");
    break;
  case 0x7A2:
    particle->Load("eternal_slash\\eternal_slash_hit\\eternal_slash_hit.str");
    break;
  case 0x7A3:
    particle->Load("shadow_stab\\shadow_stab_hit2\\shadow_stab_hit2.str");
    break;
  case 0x7B7:
    particle->Load("4ig_firstfaithpower\\firstfaithpower\\firstfaithpower.str");
    break;
  case 0x7B8:
    particle->Load("4ig_firstfaithpower\\firstfaithpower_cast\\firstfaithpower_cast.str");
    break;
  case 0x7B9:
    particle->Load("4ig_firstfaithpower\\firstfaithpower_start\\firstfaithpower_start.str");
    break;
  case 0x7BA:
    particle->Load("4ig_judge\\judge\\judge.str");
    break;
  case 0x7BB:
    particle->Load("4ig_judge\\judge_cast\\judge_cast.str");
    break;
  case 0x7BC:
    particle->Load("4ig_judge\\judge_start\\judge_start.str");
    break;
  case 0x7BD:
    if (!g_serviceOptions.Get(229)) {
      particle->Open("4ig_judge\\judge_aura\\judge_aura.str");
    }
    break;
  case 0x7BE:
    particle->Load("4ig_thirdexorflame\\thirdexorflame\\thirdexorflame.str");
    break;
  case 0x7BF:
    particle->Load("4ig_thirdexorflame\\thirdexorflame_cast\\thirdexorflame_cast.str");
    break;
  case 0x7C0:
    particle->Load("4ig_thirdexorflame\\thirdexorflame_start\\thirdexorflame_start.str");
    break;
  case 0x7C1:
    particle->Load("4ig_thirdexorflame\\thirdexorflame_aura\\thirdexorflame_aura.str");
    break;
  case 0x7C4:
    particle->Load("4ig_oleumsanctum\\oleumsanctum\\oleumsanctum.str");
    break;
  case 0x7C5:
    particle->Load("4ig_oleumsanctum\\oleumsanctum_bottom\\oleumsanctum_bottom.str");
    break;
  case 0x7C6:
    particle->Load("4ig_oleumsanctum\\oleumsanctum_hit\\oleumsanctum_hit.str");
    break;
  case 0x7C7:
    particle->Load("4ig_oleumsanctum\\oleumsanctum_loop\\oleumsanctum_loop.str");
    break;
  case 0x7CB:
    particle->Load("4ig_explosionblaster\\explosionblaster\\explosionblaster.str");
    break;
  case 0x7CC:
    particle->Load("4ig_explosionblaster\\explosionblaster_bottom\\explosionblaster_bottom.str");
    break;
  case 0x7CD:
    particle->Load("4ig_explosionblaster\\explosionblaster_hit\\explosionblaster_hit.str");
    break;
  case 0x803:
    particle->Load("4ig_firstbrand\\firstbrand\\firstbrand.str");
    break;
  case 0x804:
    if (!g_serviceOptions.Get(229)) {
      particle->Open("4ig_firstbrand\\firstbrand_aura\\firstbrand_aura.str");
    }
    break;
  case 0x805:
    particle->Load("4ig_firstbrand\\firstbrand_hit\\firstbrand_hit.str");
    break;
  case 0x806:
    particle->Load("4ig_firstbrand\\firstbrand_start\\firstbrand_start.str");
    break;
  case 0x81E:
    particle->Load("4ig_secondfalth\\secondfalth\\secondfalth.str");
    break;
  case 0x81F:
    particle->Load("4ig_secondfalth\\secondfalth_aura\\secondfalth_aura.str");
    break;
  case 0x821:
    particle->Load("4ig_secondfalth\\secondfalth_start\\secondfalth_start.str");
    break;
  case 0x825:
    particle->Load("4iq_thirdflamebomb\\thirdflamebomb\\thirdflamebomb.str");
    break;
  case 0x826:
    particle->Load("4iq_thirdflamebomb\\thirdflamebomb_bottom\\thirdflamebomb_bottom.str");
    break;
  case 0x823:
    particle->Load("4iq_secondflame\\secondflame\\secondflame.str");
    break;
  case 0x824:
    particle->Load("4iq_secondflame\\secondflame_bottom\\secondflame_bottom.str");
    break;
  case 0x7AC:
    particle->Load("sincerefaith\\sincerefaith\\sincerefaith.str");
    break;
  case 0x7AD:
    if (!g_serviceOptions.Get(229)) {
      particle->Open("sincerefaith\\sincerefaith_bottom\\sincerefaith_bottom.str");
    }
    break;
  case 0x831:
    particle->Load("4iq_thirdconsecration\\thirdconsecration\\thirdconsecration.str");
    break;
  case 0x8E7:
    particle->Load("4iq_thirdconsecration\\thirdconsecration_bottom\\thirdconsecration_bottom.str");
    break;
  case 0x840:
    particle->Load("4iq_thirdpunish\\thirdpunish\\thirdpunish.str");
    break;
  case 0x841:
    particle->Load("4iq_thirdpunish\\thirdpunish_bottom\\thirdpunish_bottom.str");
    break;
  case 0x82E:
    particle->Load("4iq_massivefblaster\\massivefblaster\\massivefblaster.str");
    break;
  case 0x82F:
    particle->Load("4iq_massivefblaster\\massivefblaster_hit\\massivefblaster_hit.str");
    break;
  case 0x830:
    particle->Load("4iq_massivefblaster\\massivefblaster_hit_bottom\\massivefblaster_hit_bottom.str");
    break;
  case 0x7AE:
    particle->Load("abyss_dagger\\abyss_dagger\\abyss_dagger.str");
    break;
  case 0x7AF:
    particle->Load("abyss_dagger\\abyss_dagger_hit\\abyss_dagger_hit.str");
    break;
  case 0x84E:
    particle->Load("4abc_abyss_slayer\\abyss_slayer\\abyss_slayer.str");
    break;
  case 0x84F:
    particle->Load("4abc_abyss_slayer\\abyss_slayer_cast\\abyss_slayer_cast.str");
    break;
  case 0x850:
    particle->Load("4abc_abyss_slayer\\abyss_slayer_cast_bottom\\abyss_slayer_cast_bottom.str");
    break;
  case 0x7B1:
    particle->Load("hawkvumerang\\hawkvumerang_cast\\hawkvumerang_cast.str");
    break;
  case 0x7B2:
    particle->Load("hawkvumerang\\hawkvumerang_hit\\hawkvumerang_hit.str");
    break;
  case 0x7B3:
    particle->Load("hawkvumerang\\hawkvumerang_start\\hawkvumerang_start.str");
    break;
  case 0x7B4:
    particle->Load("galestorm\\galestorm_cast\\galestorm_cast.str");
    break;
  case 0x7B6:
    particle->Load("galestorm\\galestorm_hit\\galestorm_hit.str");
    break;
  case 0x7D6:
    particle->Load("4tr_kvasirsonata\\kvasirsonata\\kvasirsonata.str");
    break;
  case 0x7D7:
    particle->Load("4tr_kvasirsonata\\kvasirsonata_cast\\kvasirsonata_cast.str");
    break;
  case 0x7D8:
    particle->Load("4tr_kvasirsonata\\kvasirsonata_cast_bottom\\kvasirsonata_cast_bottom.str");
    break;
  case 0x7C3:
    particle->Load("powerfulfaith\\powerfulfaith\\powerfulfaith.str");
    break;
  case 0x7C8:
    particle->Load("increasing_activity\\increasing_activity\\increasing_activity.str");
    break;
  case 0x7C9:
    particle->Load("increasing_activity\\increasing_activity_cast\\increasing_activity_cast.str");
    break;
  case 0x7CA:
    particle->Load("increasing_activity\\increasing_activity_cast_bottom\\increasing_activity_cast_bottom.str");
    break;
  case 0x7CF:
    particle->Load("firmfaith\\firmfaith\\firmfaith.str");
    break;
  case 0x7D0:
    particle->Load("mysticsymphony\\mysticsymphony\\mysticsymphony.str");
    break;
  case 0x7D1:
    particle->Load("mysticsymphony\\mysticsymphony_cast\\mysticsymphony_cast.str");
    break;
  case 0x7D2:
    particle->Load("mysticsymphony\\mysticsymphony_note\\mysticsymphony_note.str");
    break;
  case 0x7D3:
    particle->Load("activity_burn\\activity_burn\\activity_burn.str");
    break;
  case 0x7D4:
    particle->Load("activity_burn\\activity_burn_cast\\activity_burn_cast.str");
    break;
  case 0x7D5:
    particle->Load("activity_burn\\activity_burn_cast_bottom\\activity_burn_cast_bottom.str");
    break;
  case 0x7DE:
    particle->Load("chain_reaction_shot\\chain_reaction_shot_cast\\chain_reaction_shot_cast.str");
    break;
  case 0x7DF:
    particle->Load("chain_reaction_shot\\chain_reaction_shot_cast_bottom\\chain_reaction_shot_cast_bottom.str");
    break;
  case 0x7E0:
    particle->Load("eternal_slash\\eternal_slash_hit\\eternal_slash_hit.str");
    break;
  case 0x7E1:
    particle->Load("chain_reaction_shot\\chain_reaction_shot_hitsub\\chain_reaction_shot_hitsub.str");
    break;
  case 0x7E2:
    particle->Load("chain_reaction_shot\\chain_reaction_shot\\chain_reaction_shot.str");
    break;
  case 0x7D9:
    particle->Load("serpens\\serpens_bottom\\serpens_bottom.str");
    break;
  case 0x7DA:
    particle->Load("serpens\\serpens_cast\\serpens_cast.str");
    break;
  case 0x7DB:
    particle->Load("serpens\\serpens_cast_bottom\\serpens_cast_bottom.str");
    break;
  case 0x7DC:
    particle->Load("serpens\\serpens_hit\\serpens_hit.str");
    break;
  case 0x7DD:
    particle->Load("serpens\\serpens_shield\\serpens_shield.str");
    break;
  case 0x8D2:
    particle->Load("serpens\\serpens_deep_poisoning\\serpens_deep_poisoning.str");
    break;
  case 0x8D3:
    particle->Load("serpens\\serpens_deep_poisoning_bottom\\serpens_deep_poisoning_bottom.str");
    break;
  case 0x7E3:
    particle->Load("diluvio\\diluvio_ice\\diluvio_ice.str");
    break;
  case 0x7E4:
    particle->Load("diluvio\\diluvio_ice_bottom\\diluvio_ice_bottom.str");
    break;
  case 0x7E5:
    particle->Load("diluvio\\diluvio_cast\\diluvio_cast.str");
    break;
  case 0x7E6:
    particle->Load("diluvio\\diluvio_cast_bottom\\diluvio_cast_bottom.str");
    break;
  case 0x7E7:
    particle->Load("diluvio\\diluvio_hit\\diluvio_hit.str");
    break;
  case 0x7E8:
    particle->Load("diluvio\\diluvio_armor\\diluvio_armor.str");
    break;
  case 0x8D4:
    particle->Load("diluvio\\diluvio_cold_force\\diluvio_cold_force.str");
    break;
  case 0x8D5:
    particle->Load("diluvio\\diluvio_cold_force _bottom\\diluvio_cold_force _bottom.str");
    break;
  case 0x7E9:
    particle->Load("spell_enchanting\\spell_enchanting\\spell_enchanting.str");
    break;
  case 0x7EB:
    particle->Load("diamond_storm\\diamond_storm_bottom\\diamond_storm_bottom.str");
    break;
  case 0x7EC:
    particle->Load("diamond_storm\\diamond_storm_cast\\diamond_storm_cast.str");
    break;
  case 0x7ED:
    particle->Load("diamond_storm\\diamond_storm_cast_bottom\\diamond_storm_cast_bottom.str");
    break;
  case 0x7EE:
    particle->Load("diamond_storm\\diamond_storm_hit\\diamond_storm_hit.str");
    break;
  case 0x7FE:
    particle->Load("roseblossom\\roseblossom_cast\\roseblossom_cast.str");
    break;
  case 0x7FF:
    particle->Load("roseblossom\\roseblossom_cast_bottom\\roseblossom_cast_bottom.str");
    break;
  case 0x800:
    particle->Load("roseblossom\\roseblossom_hit\\roseblossom_hit.str");
    break;
  case 0x801:
    particle->Load("roseblossom\\roseblossom_rose\\roseblossom_rose.str");
    break;
  case 0x802:
    particle->Load("roseblossom\\roseblossom_shot\\roseblossom_shot.str");
    break;
  case 0x7EF:
    particle->Load("venom_swamp\\venom_swamp_bottom\\venom_swamp_bottom.str");
    break;
  case 0x7F0:
    particle->Load("venom_swamp\\venom_swamp_cast\\venom_swamp_cast.str");
    break;
  case 0x7F1:
    particle->Load("venom_swamp\\venom_swamp_cast_bottom\\venom_swamp_cast_bottom.str");
    break;
  case 0x7F2:
    particle->Load("venom_swamp\\venom_swamp_hit\\venom_swamp_hit.str");
    break;
  case 0x7F3:
    particle->Load("ardor\\ardor\\ardor.str");
    break;
  case 0x7F4:
    particle->Load("ardor\\ardor_bottom\\ardor_bottom.str");
    break;
  case 0x7F5:
    particle->Load("ardor\\ardor_cast\\ardor_cast.str");
    break;
  case 0x7F6:
    particle->Load("ardor\\ardor_cast_bottom\\ardor_cast_bottom.str");
    break;
  case 0x7F7:
    particle->Load("ardor\\ardor_hit\\ardor_hit.str");
    break;
  case 0x7F8:
    particle->Load("ardor\\ardor_flamearmor\\ardor_flamearmor.str");
    break;
  case 0x8D6:
    particle->Load("ardor\\ardor_flametechnic\\ardor_flametechnic.str");
    break;
  case 0x8D7:
    particle->Load("ardor\\ardor_flametechnic_bottom\\ardor_flametechnic_bottom.str");
    break;
  case 0x7F9:
    particle->Load("lightning_land\\lightning_land\\lightning_land.str");
    break;
  case 0x7FA:
    particle->Load("lightning_land\\lightning_land_bottom\\lightning_land_bottom.str");
    break;
  case 0x7FB:
    particle->Load("lightning_land\\lightning_land_cast\\lightning_land_cast.str");
    break;
  case 0x7FC:
    particle->Load("lightning_land\\lightning_land_cast_bottom\\lightning_land_cast_bottom.str");
    break;
  case 0x7FD:
    particle->Load("lightning_land\\lightning_land_hit\\lightning_land_hit.str");
    break;
  case 0x807:
    particle->Load("conflagration\\conflagration\\conflagration.str");
    break;
  case 0x808:
    particle->Load("conflagration\\conflagration_bottom\\conflagration_bottom.str");
    break;
  case 0x809:
    particle->Load("conflagration\\conflagration_cast\\conflagration_cast.str");
    break;
  case 0x80A:
    particle->Load("conflagration\\conflagration_cast_bottom\\conflagration_cast_bottom.str");
    break;
  case 0x80B:
    particle->Load("conflagration\\conflagration_hit\\conflagration_hit.str");
    break;
  case 0x80C:
    particle->Load("soundblend\\soundblend\\soundblend.str");
    break;
  case 0x80D:
    particle->Load("soundblend\\soundblend_cast\\soundblend_cast.str");
    break;
  case 0x80E:
    particle->Load("soundblend\\soundblend_cast_bottom\\soundblend_cast_bottom.str");
    break;
  case 0x80F:
    particle->Load("soundblend\\soundblend_red\\soundblend_red.str");
    break;
  case 0x810:
    particle->Load("metalicfury\\metalicfury\\metalicfury.str");
    break;
  case 0x811:
    particle->Load("jawaii_serenade\\jawaii_serenade\\jawaii_serenade.str");
    break;
  case 0x812:
    particle->Load("jawaii_serenade\\jawaii_serenade_cast\\jawaii_serenade_cast.str");
    break;
  case 0x813:
    particle->Load("jawaii_serenade\\jawaii_serenade_cast_bottom\\jawaii_serenade_cast_bottom.str");
    break;
  case 0x814:
    particle->Load("pronmarch\\pronmarch\\pronmarch.str");
    break;
  case 0x815:
    particle->Load("pronmarch\\pronmarch_cast\\pronmarch_cast.str");
    break;
  case 0x816:
    particle->Load("pronmarch\\pronmarch_cast_bottom\\pronmarch_cast_bottom.str");
    break;
  case 0x817:
    particle->Load("rhythmshooting\\rhythmshooting_cast\\rhythmshooting_cast.str");
    break;
  case 0x818:
    particle->Load("rhythmshooting\\rhythmshooting_cast_bottom\\rhythmshooting_cast_bottom.str");
    break;
  case 0x819:
    particle->Load("rhythmshooting\\rhythmshooting_hit\\rhythmshooting_hit.str");
    break;
  case 0x81B:
    particle->Load("nipelheim\\nipelheim\\nipelheim.str");
    break;
  case 0x81C:
    particle->Load("nipelheim\\nipelheim_cast\\nipelheim_cast.str");
    break;
  case 0x827:
    particle->Load("abyss_strike\\abyss_strike\\abyss_strike.str");
    break;
  case 0x828:
    particle->Load("abyss_strike\\abyss_strike_bottom\\abyss_strike_bottom.str");
    break;
  case 0x829:
    particle->Load("abyss_strike\\abyss_strike_cast\\abyss_strike_cast.str");
    break;
  case 0x82A:
    particle->Load("abyss_strike\\abyss_strike_cast_bottom\\abyss_strike_cast_bottom.str");
    break;
  case 0x82B:
    particle->Load("abyss_strike\\abyss_strike_ground\\abyss_strike_ground.str");
    break;
  case 0x82C:
    particle->Load("abyss_strike\\abyss_strike_ground_bottom\\abyss_strike_ground_bottom.str");
    break;
  case 0x82D:
    particle->Load("abyss_strike\\abyss_strike_hit\\abyss_strike_hit.str");
    break;
  case 0x832:
    particle->Load("procella\\procella\\procella.str");
    break;
  case 0x833:
    particle->Load("procella\\procella_bottom\\procella_bottom.str");
    break;
  case 0x834:
    particle->Load("procella\\procella_cast\\procella_cast.str");
    break;
  case 0x835:
    particle->Load("procella\\procella_cast_bottom\\procella_cast_bottom.str");
    break;
  case 0x836:
    particle->Load("procella\\procella_hit\\procella_hit.str");
    break;
  case 0x837:
    particle->Load("procella\\procella_storm\\procella_storm.str");
    break;
  case 0x8D8:
    particle->Load("procella\\procella_grace_breeze\\procella_grace_breeze.str");
    break;
  case 0x8D9:
    particle->Load("procella\\procella_grace_breeze_bottom\\procella_grace_breeze_bottom.str");
    break;
  case 0x838:
    particle->Load("terremotus\\terremotus\\terremotus.str");
    break;
  case 0x839:
    particle->Load("terremotus\\terremotus_cast\\terremotus_cast.str");
    break;
  case 0x83A:
    particle->Load("terremotus\\terremotus_cast_bottom\\terremotus_cast_bottom.str");
    break;
  case 0x83B:
    particle->Load("terremotus\\terremotus_hit\\terremotus_hit.str");
    break;
  case 0x83C:
    particle->Load("terremotus\\terremotus_protect\\terremotus_protect.str");
    break;
  case 0x8DA:
    particle->Load("terremotus\\terremotus_earth_care\\terremotus_earth_care.str");
    break;
  case 0x8DB:
    particle->Load("terremotus\\terremotus_earth_care_bottom\\terremotus_earth_care_bottom.str");
    break;
  case 0x83E:
    particle->Load("from_the_abyss\\from_the_abyss_shadow\\from_the_abyss_shadow.str");
    break;
  case 0x83F:
    particle->Load("abyss_strike\\abyss_strike_hit\\abyss_strike_hit.str");
    break;
  case 0x854:
    particle->Load("deft_stab\\deft_stab\\deft_stab.str");
    break;
  case 0x851:
    particle->Load("abyss_square\\abyss_square\\abyss_square.str");
    break;
  case 0x852:
    particle->Load("abyss_square\\abyss_square_bottom\\abyss_square_bottom.str");
    break;
  case 0x853:
    particle->Load("abyss_dagger\\abyss_dagger_hit\\abyss_dagger_hit.str");
    break;
  case 0x843:
    particle->Load("frenzy_shot\\frenzy_shot_hit\\frenzy_shot_hit.str");
    break;
  case 0x844:
    particle->Load("gef_nocturn\\gef_nocturn\\gef_nocturn.str");
    break;
  case 0x845:
    particle->Load("gef_nocturn\\gef_nocturn_cast\\gef_nocturn_cast.str");
    break;
  case 0x846:
    particle->Load("gef_nocturn\\gef_nocturn_cast_bottom\\gef_nocturn_cast_bottom.str");
    break;
  case 0x847:
    particle->Load("roki_capriccio\\roki_capriccio\\roki_capriccio.str");
    break;
  case 0x848:
    particle->Load("roki_capriccio\\roki_capriccio_cast\\roki_capriccio_cast.str");
    break;
  case 0x849:
    particle->Load("roki_capriccio\\roki_capriccio_cast_bottom\\roki_capriccio_cast_bottom.str");
    break;
  case 0x84A:
    particle->Load("musical_interlude\\musical_interlude\\musical_interlude.str");
    break;
  case 0x84B:
    particle->Load("musical_interlude\\musical_interlude_cast\\musical_interlude_cast.str");
    break;
  case 0x84C:
    particle->Load("ain_rhapsody\\ain_rhapsody\\ain_rhapsody.str");
    break;
  case 0x84D:
    particle->Load("ain_rhapsody\\ain_rhapsody_cast\\ain_rhapsody_cast.str");
    break;
  case 0x855:
    particle->Load("terradrive\\terradrive\\terradrive.str");
    break;
  case 0x856:
    particle->Load("terradrive\\terradrive_bottom\\terradrive_bottom.str");
    break;
  case 0x857:
    particle->Load("terradrive\\terradrive_cast\\terradrive_cast.str");
    break;
  case 0x858:
    particle->Load("terradrive\\terradrive_cast_bottom\\terradrive_cast_bottom.str");
    break;
  case 0x859:
    particle->Load("terradrive\\terradrive_hit\\terradrive_hit.str");
    break;
  case 0x85A:
    particle->Load("elemental_buster\\elemental_buster_cast\\elemental_buster_cast.str");
    break;
  case 0x85B:
    particle->Load("elemental_buster\\elemental_buster\\elemental_buster_fire\\elemental_buster_fire.str");
    break;
  case 0x85C:
    particle->Load("elemental_buster\\elemental_buster\\elemental_buster_fire_bottom\\elemental_buster_fire_bottom.str");
    break;
  case 0x85D:
    particle->Load("elemental_buster\\elemental_buster_hit\\elemental_buster_fire_hit\\elemental_buster_fire_hit.str");
    break;
  case 0x85E:
    particle->Load("elemental_buster\\elemental_buster\\elemental_buster_land\\elemental_buster_land.str");
    break;
  case 0x85F:
    particle->Load("elemental_buster\\elemental_buster\\elemental_buster_land_bottom\\elemental_buster_land_bottom.str");
    break;
  case 0x860:
    particle->Load("elemental_buster\\elemental_buster_hit\\elemental_buster_land_hit\\elemental_buster_land_hit.str");
    break;
  case 0x861:
    particle->Load("elemental_buster\\elemental_buster\\elemental_buster_poison\\elemental_buster_poison.str");
    break;
  case 0x862:
    particle->Load("elemental_buster\\elemental_buster\\elemental_buster_poison_bottom\\elemental_buster_poison_bottom.str");
    break;
  case 0x863:
    particle->Load("elemental_buster\\elemental_buster_hit\\elemental_buster_poison_hit\\elemental_buster_poison_hit.str");
    break;
  case 0x864:
    particle->Load("elemental_buster\\elemental_buster\\elemental_buster_water\\elemental_buster_water.str");
    break;
  case 0x865:
    particle->Load("elemental_buster\\elemental_buster\\elemental_buster_water_bottom\\elemental_buster_water_bottom.str");
    break;
  case 0x866:
    particle->Load("elemental_buster\\elemental_buster_hit\\elemental_buster_water_hit\\elemental_buster_water_hit.str");
    break;
  case 0x867:
    particle->Load("elemental_buster\\elemental_buster\\elemental_buster_wind\\elemental_buster_wind.str");
    break;
  case 0x868:
    particle->Load("elemental_buster\\elemental_buster\\elemental_buster_wind_bottom\\elemental_buster_wind_bottom.str");
    break;
  case 0x869:
    particle->Load("elemental_buster\\elemental_buster_hit\\elemental_buster_wind_hit\\elemental_buster_wind_hit.str");
    break;
  case 0x86B:
    particle->Load("unlucky_rush\\unlucky_rush_cast\\unlucky_rush_cast.str");
    break;
  case 0x86C:
    particle->Load("abyss_dagger\\abyss_dagger_hit\\abyss_dagger_hit.str");
    break;
  case 0x86D:
    particle->Load("overslash\\overslash\\overslash.str");
    break;
  case 0x86E:
    particle->Load("overslash\\overslash_bottom\\overslash_bottom.str");
    break;
  case 0x86F:
    particle->Load("overslash\\overslash_cast\\overslash_cast.str");
    break;
  case 0x870:
    particle->Load("overslash\\overslash_hit\\overslash_hit.str");
    break;
  case 0x871:
    particle->Load("adv_protection\\adv_protection\\adv_protection.str");
    break;
  case 0x872:
    particle->Load("adv_protection\\adv_protection_bottom\\adv_protection_bottom.str");
    break;
  case 0x873:
    particle->Load("adv_protection\\adv_protection_cast\\adv_protection_cast.str");
    break;
  case 0x874:
    particle->Load("adv_protection\\adv_protection_cast_bottom\\adv_protection_cast_bottom.str");
    break;
  case 0x876:
    particle->Load("crossrain\\cross_rain_bottom\\cross_rain_bottom.str");
    break;
  case 0x877:
    particle->Load("crossrain\\cross_rain_cast\\cross_rain_cast.str");
    break;
  case 0x878:
    particle->Load("crossrain\\cross_rain_cast_bottom\\cross_rain_cast_bottom.str");
    break;
  case 0x879:
    particle->Load("guard_stance\\guard_stance\\guard_stance.str");
    break;
  case 0x87A:
    particle->Load("guard_stance\\guard_stance_cast\\guard_stance_cast.str");
    break;
  case 0x87B:
    particle->Load("guard_stance\\guard_stance_cast_bottom\\guard_stance_cast_bottom.str");
    break;
  case 0x87C:
    particle->Load("attack_stance\\attack_stance\\attack_stance.str");
    break;
  case 0x87D:
    particle->Load("attack_stance\\attack_stance_cast\\attack_stance_cast.str");
    break;
  case 0x87E:
    particle->Load("attack_stance\\attack_stance_cast_bottom\\attack_stance_cast_bottom.str");
    break;
  case 0x87F:
    particle->Load("grand_judgement\\grand_judgement\\grand_judgement.str");
    break;
  case 0x880:
    particle->Load("grand_judgement\\grand_judgement_bottom\\grand_judgement_bottom.str");
    break;
  case 0x881:
    particle->Load("grand_judgement\\grand_judgement_buff\\grand_judgement_buff.str");
    break;
  case 0x882:
    particle->Load("grand_judgement\\grand_judgement_cast\\grand_judgement_cast.str");
    break;
  case 0x883:
    particle->Load("grand_judgement\\grand_judgement_cast_bottom\\grand_judgement_cast_bottom.str");
    break;
  case 0x885:
    particle->Load("holy_shield\\holy_shield\\holy_shield.str");
    break;
  case 0x884:
    particle->Load("holy_shield\\holy_shield_cast\\holy_shield_cast.str");
    break;
  case 0x886:
    particle->Load("holy_shield\\holy_shield_loop\\holy_shield_loop.str");
    break;
  case 0x888:
    particle->Load("shield_shooting\\shield_shooting_cast\\shield_shooting_cast.str");
    break;
  case 0x889:
    particle->Load("shield_shooting\\shield_shooting_hit\\shield_shooting_hit.str");
    break;
  case 0x88A:
    particle->Load("the_w_protection\\the_w_protection\\the_w_protection.str");
    break;
  case 0x88B:
    particle->Load("the_w_protection\\the_w_protection_cast\\the_w_protection_cast.str");
    break;
  case 0x88C:
    particle->Load("guardianshield\\guardianshield\\guardianshield.str");
    break;
  case 0x88D:
    particle->Load("guardianshield\\guardianshield_bottom\\guardianshield_bottom.str");
    break;
  case 0x88E:
    particle->Load("guardianshield\\guardianshield_sub\\guardianshield_sub.str");
    break;
  case 0x88F:
    particle->Load("judgement_cross\\judgement_cross\\judgement_cross.str");
    break;
  case 0x890:
    particle->Load("judgement_cross\\judgement_cross_bottom\\judgement_cross_bottom.str");
    break;
  case 0x891:
    particle->Load("judgement_cross\\judgement_cross_cast\\judgement_cross_cast.str");
    break;
  case 0x892:
    particle->Load("judgement_cross\\judgement_cross_cast_bottom\\judgement_cross_cast_bottom.str");
    break;
  case 0x894:
    particle->Load("rebound_shield\\rebound_shield\\rebound_shield.str");
    break;
  case 0x895:
    particle->Load("rebound_shield\\rebound_shield_bottom\\rebound_shield_bottom.str");
    break;
  case 0x896:
    particle->Load("rebound_shield\\rebound_shield_cast\\rebound_shield_cast.str");
    break;
  case 0x897:
    particle->Load("rebound_shield\\rebound_shield_cast_bottom\\rebound_shield_cast_bottom.str");
    break;
  case 0x898:
    particle->Load("ultimate_sacrifice\\ultimatesacrifice\\ultimatesacrifice.str");
    break;
  case 0x899:
    particle->Load("ultimate_sacrifice\\ultimatesacrifice_bottom\\ultimatesacrifice_bottom.str");
    break;
  case 0x89A:
    particle->Load("ultimate_sacrifice\\ultimatesacrifice_shine\\ultimatesacrifice_shine.str");
    break;
  case 0x89C:
    particle->Load("acidified_zone_fire\\acidified_zone_fire_cast\\acidified_zone_fire_cast.str");
    break;
  case 0x89D:
    particle->Load("acidified_zone_fire\\acidified_zone_fire_cast_bottom\\acidified_zone_fire_cast_bottom.str");
    break;
  case 0x89F:
    particle->Load("acidified_zone_fire\\acidified_zone_fire_hit\\acidified_zone_fire_hit.str");
    break;
  case 0x8A0:
    particle->Load("acidified_zone_fire\\acidified_zone_fire_a_start\\acidified_zone_fire_a_start.str");
    break;
  case 0x8A1:
    particle->Load("acidified_zone_fire\\acidified_zone_fire_a_start_bottom\\acidified_zone_fire_a_start_bottom.str");
    break;
  case 0x8A2:
    particle->Load("acidified_zone_fire\\acidified_zone_fire_a_loop\\acidified_zone_fire_a_loop.str");
    break;
  case 0x8A3:
    particle->Load("acidified_zone_fire\\acidified_zone_fire_a_loop_bottom\\acidified_zone_fire_a_loop_bottom.str");
    break;
  case 0x8A4:
    particle->Load("acidified_zone_water\\acidified_zone_water_cast\\acidified_zone_water_cast.str");
    break;
  case 0x8A5:
    particle->Load("acidified_zone_water\\acidified_zone_water_cast_bottom\\acidified_zone_water_cast_bottom.str");
    break;
  case 0x8A7:
    particle->Load("acidified_zone_water\\acidified_zone_water_hit\\acidified_zone_water_hit.str");
    break;
  case 0x8A8:
    particle->Load("acidified_zone_water\\acidified_zone_water_a_start\\acidified_zone_water_a_start.str");
    break;
  case 0x8A9:
    particle->Load("acidified_zone_water\\acidified_zone_water_a_start_bottom\\acidified_zone_water_a_start_bottom.str");
    break;
  case 0x8AA:
    particle->Load("acidified_zone_water\\acidified_zone_water_a_loop\\acidified_zone_water_a_loop.str");
    break;
  case 0x8AB:
    particle->Load("acidified_zone_water\\acidified_zone_water_a_loop_bottom\\acidified_zone_water_a_loop_bottom.str");
    break;
  case 0x8AC:
    particle->Load("acidified_zone_wind\\acidified_zone_wind_cast\\acidified_zone_wind_cast.str");
    break;
  case 0x8AD:
    particle->Load("acidified_zone_wind\\acidified_zone_wind_cast_bottom\\acidified_zone_wind_cast_bottom.str");
    break;
  case 0x8AF:
    particle->Load("acidified_zone_wind\\acidified_zone_wind_hit\\acidified_zone_wind_hit.str");
    break;
  case 0x8B0:
    particle->Load("acidified_zone_wind\\acidified_zone_wind_a_start\\acidified_zone_wind_a_start.str");
    break;
  case 0x8B1:
    particle->Load("acidified_zone_wind\\acidified_zone_wind_a_start_bottom\\acidified_zone_wind_a_start_bottom.str");
    break;
  case 0x8B2:
    particle->Load("acidified_zone_wind\\acidified_zone_wind_a_loop\\acidified_zone_wind_a_loop.str");
    break;
  case 0x8B3:
    particle->Load("acidified_zone_wind\\acidified_zone_wind_a_loop_bottom\\acidified_zone_wind_a_loop_bottom.str");
    break;
  case 0x8B4:
    particle->Load("acidified_zone_ground\\acidified_zone_ground_cast\\acidified_zone_ground_cast.str");
    break;
  case 0x8B5:
    particle->Load("acidified_zone_ground\\acidified_zone_ground_cast_bottom\\acidified_zone_ground_cast_bottom.str");
    break;
  case 0x8B7:
    particle->Load("acidified_zone_ground\\acidified_zone_ground_hit\\acidified_zone_ground_hit.str");
    break;
  case 0x8B8:
    particle->Load("acidified_zone_ground\\acidified_zone_ground_a_start\\acidified_zone_ground_a_start.str");
    break;
  case 0x8B9:
    particle->Load("acidified_zone_ground\\acidified_zone_ground_a_start_bottom\\acidified_zone_ground_a_start_bottom.str");
    break;
  case 0x8BA:
    particle->Load("acidified_zone_ground\\acidified_zone_ground_a_loop\\acidified_zone_ground_a_loop.str");
    break;
  case 0x8BB:
    particle->Load("acidified_zone_ground\\acidified_zone_ground_a_loop_bottom\\acidified_zone_ground_a_loop_bottom.str");
    break;
  case 0x8BC:
    particle->Load("helltree\\helltree_cast\\helltree_cast.str");
    break;
  case 0x8BD:
    particle->Load("helltree\\helltree_cast_bottom\\helltree_cast_bottom.str");
    break;
  case 0x8BE:
    particle->Load("helltree\\hell_dusty\\hell_dusty.str");
    break;
  case 0x8BF:
    particle->Load("helltree\\hell_howling\\hell_howling.str");
    break;
  case 0x8C1:
    particle->Load("research_report\\researchreport\\researchreport.str");
    break;
  case 0x8C2:
    particle->Load("research_report\\researchreport_cast\\researchreport_cast.str");
    break;
  case 0x8C3:
    particle->Load("research_report\\researchreport_cast_bottom\\researchreport_cast_bottom.str");
    break;
  case 0x8C7:
    particle->Load("wooden_warrior\\wooden_warrior_cast\\wooden_warrior_cast.str");
    break;
  case 0x8C8:
    particle->Load("wooden_warrior\\wooden_warrior_cast_bottom\\wooden_warrior_cast_bottom.str");
    break;
  case 0x8CA:
    particle->Load("wooden_warrior\\wooden_attack\\wooden_attack\\wooden_attack.str");
    break;
  case 0x8CB:
    particle->Load("wooden_warrior\\wooden_attack\\wooden_attack_hit\\wooden_attack_hit.str");
    break;
  case 0x8CC:
    particle->Load("wooden_warrior\\throwrock\\throwrock\\throwrock.str");
    break;
  case 0x8CD:
    particle->Load("wooden_warrior\\throwrock\\throwrock_hit\\throwrock_hit.str");
    break;
  case 0x8CE:
    particle->Load("wooden_fairy\\wooden_cast\\wooden_cast.str");
    break;
  case 0x8D0:
    particle->Load("wooden_fairy\\wooden_cast_bottom\\wooden_cast_bottom.str");
    break;
  case 0x8D1:
    particle->Load("wooden_fairy\\fairy_dusty\\fairy_dusty.str");
    break;
  case 0x8C4:
    particle->Open("state_effect\\lassitude\\lassitude.str");
    break;
  case 0x8C5:
    particle->Open("state_effect\\swooning\\swooning.str");
    break;
  case 0x8C6:
    particle->Open("state_effect\\holyflame\\holyflame.str");
    break;
  case 0x8DC:
    particle->Open("state_effect\\lightningstrike\\lightningstrike.str");
    break;
  case 0x8DD:
    particle->Open("state_effect\\conflagration\\conflagration.str");
    break;
  case 0x8DE:
    particle->Open("state_effect\\frostbite\\frostbite\\frostbite.str");
    break;
  case 0x8DF:
    particle->Open("state_effect\\frostbite\\frostbite_finish\\frostbite_finish.str");
    break;
  case 0x8E2:
    particle->Open("state_effect\\deadlypoison\\deadlypoison.str");
    break;
  case 0x8E1:
    particle->Open("state_effect\\crystallization\\crystallization.str");
    break;
  case 0x8FE:
    particle->Open("NPC_KILLING_AURA\\npc_killing_aura_bottom\\killing_aura.str");
    break;
  case 0x8FF:
    particle->Open("NPC_KILLING_AURA\\npc_killing_aura_bottom\\killing_aura_end.str");
    break;
  case 0x900:
    particle->Open("NPC_KILLING_AURA\\npc_killing_aura_hit\\killing_aura_hit.str");
    break;
  case 0x904:
    particle->Open("NPC_CANE_OF_EVIL_EYE\\npc_cane_of_evil_eye_hit\\npc_cane_of_evil_eye_hit.str");
    break;
  case 0x907:
    particle->Open("ui_enchant\\ui_intro_yellow\\ui_intro_yellow.str");
    break;
  case 0x908:
    particle->Open("ui_enchant\\ui_enchant_success\\ui_enchant_success.str");
    break;
  case 0x909:
    particle->Open("ui_enchant\\ui_fail\\ui_enchant_fail.str");
    break;
  case 0x90A:
    particle->Open("ui_enchant\\ui_intro_blue\\ui_intro_blue.str");
    break;
  case 0x90B:
    particle->Open("ui_enchant\\ui_levelup_success\\ui_levelup_success.str");
    break;
  case 0x90C:
    particle->Open("ui_enchant\\ui_fail\\ui_levelup_fail.str");
    break;
  case 0x90D:
    particle->Open("ui_enchant\\ui_intro_green\\ui_intro_green.str");
    break;
  case 0x90E:
    particle->Open("ui_enchant\\ui_reset_success\\ui_reset_success.str");
    break;
  case 0x90F:
    particle->Open("ui_enchant\\ui_fail\\ui_reset_fail.str");
    break;
  case 0x95B:
    particle->Open("runesystem\\rune_active\\ui_finish.str");
    break;
  case 0x95C:
    particle->Open("runesystem\\runeset_active\\ui_activate.str");
    break;
  case 0x95D:
    particle->Open("runesystem\\runeset_upgrade_success\\rune_upgrade_succecss.str");
    break;
  case 0x95E:
    particle->Open("runesystem\\runeset_upgrade_fail\\rune_upgrade_fail.str");
    break;
  case 0x95F:
    particle->Open("runesystem\\decom_broken\\ui_broken.str");
    break;
  case 0x975:
    particle->Open("runesystem\\rune_slot_small\\rune_slot_small.str");
    break;
  case 0x976:
    particle->Open("runesystem\\rune_slot_big\\rune_slot_big.str");
    break;
  case 0x910:
    particle->Open("toxin_of_mandara\\toxin_of_mandara_bomb\\toxin_of_mandara_bomb.str");
    break;
  case 0x911:
    particle->Open("toxin_of_mandara\\toxin_of_mandara_bomb_bottom\\toxin_of_mandara_bomb_bottom.str");
    break;
  case 0x912:
    particle->Open("toxin_of_mandara\\toxin_of_mandara_shock\\toxin_of_mandara_shock.str");
    break;
  case 0x913:
    particle->Open("needle_stinger\\needle_stinger\\needle_stinger.str");
    break;
  case 0x914:
    particle->Open("needle_stinger\\needle_stinger_bottom\\needle_stinger_bottom.str");
    break;
  case 0x915:
    particle->Open("needle_stinger\\needle_stinger_hit\\needle_stinger_hit.str");
    break;
  case 0x916:
    particle->Open("absolute_zephyr\\absolute_zephyr\\absolute_zephyr.str");
    break;
  case 0x917:
    particle->Open("absolute_zephyr\\absolute_zephyr_bottom\\absolute_zephyr_bottom.str");
    break;
  case 0x918:
    particle->Open("absolute_zephyr\\absolute_zephyr_hit\\absolute_zephyr_hit.str");
    break;
  case 0x919:
    particle->Open("twister_cutter\\twister_cutter\\twister_cutter.str");
    break;
  case 0x91A:
    particle->Open("twister_cutter\\twister_cutter_hit\\twister_cutter_hit.str");
    break;
  case 0x91B:
    particle->Open("heilige_pferd\\heilige_pferd\\heilige_pferd.str");
    break;
  case 0x91C:
    particle->Open("heilige_pferd\\heilige_pferd_bottom\\heilige_pferd_bottom.str");
    break;
  case 0x91D:
    particle->Open("heilige_pferd\\heilige_pferd_cast\\heilige_pferd_cast.str");
    break;
  case 0x91E:
    particle->Open("goldene_tone\\goldene_tone\\goldene_tone.str");
    break;
  case 0x91F:
    particle->Open("glanzen_spies\\glanzen_spies\\glanzen_spies.str");
    break;
  case 0x920:
    particle->Open("glanzen_spies\\glanzen_spies_hit\\glanzen_spies_hit.str");
    break;
  case 0x921:
    particle->Open("blazing_and_furious\\blazing_and_furious\\blazing_and_furious.str");
    break;
  case 0x923:
    particle->Open("the_one_fighter_rises\\the_one_fighter_rises_bottom\\the_one_fighter_rises_bottom.str");
    break;
  case 0x924:
    particle->Open("tempering\\tempering\\tempering.str");
    break;
  case 0x926:
    particle->Open("blast_forge\\blast_forge_hit\\blast_forge_hit.str");
    break;
  case 0x943:
    particle->Open("tetravortex\\tetra_04_water_sub_explosion\\tetra_04_water_sub_expn.str");
    break;
  case 0x944:
    particle->Open("tetravortex\\tetra_03_earth_sub_explosion\\tetra_03_earth_sub_expn.str");
    break;
  case 0x948:
    particle->Open("npc_20th_crystal\\npc_20th_crystal_s.str");
    break;
  case 0x949:
    particle->Open("npc_20th_crystal\\npc_20th_crystal_m.str");
    break;
  case 0x94A:
    particle->Open("npc_20th_crystal\\npc_20th_crystal_l.str");
    break;
  case 0x94B:
  case 0x94F:
  case 0x952:
    particle->Open("npc_gate\\npc_gate_green\\npc_gate_green.str");
    break;
  case 0x94C:
  case 0x950:
  case 0x953:
    particle->Open("npc_gate\\npc_gate_purple\\npc_gate_purple.str");
    break;
  case 0x94D:
  case 0x951:
  case 0x954:
    particle->Open("npc_gate\\npc_gate_red\\npc_gate_red.str");
    break;
  case 0x94E:
    particle->Open("20th_firework\\20th_firework.str");
    break;
  case 0x955:
    particle->Open("npc_gate_skyblue\\npc_gate_skyblue\\npc_gate_skyblue.str");
    break;
  case 0x963:
    particle->Open("new_battle_w\\new_battle_w_hit\\new_battle_w_hit.str");
    break;
  case 0x965:
    particle->Open("new_pramen\\new_pramen\\new_pramen.str");
    break;
  case 0x966:
    particle->Open("the_w_protection\\the_w_protection\\the_w_protection.str");
    break;
  case 0x967:
    particle->Open("elemental_buster\\elemental_buster\\elemental_buster_fire\\elemental_buster_fire.str");
    break;
  case 0x968:
    particle->Open("destructive_hurricane\\destructive_hurricane_bottom\\destructive_hurricane_bottom.str");
    break;
  case 0x969:
    particle->Open("crystal_impact\\crystal_impact_bottom\\crystal_impact_bottom.str");
    break;
  case 0x96A:
    particle->Open("new_judex\\new_judex_target\\new_judex_target.str");
    break;
  default:
    break;
  }

  if (id >= 0x9B5) {
    const char* name = nullptr;
    g_luaScript->Call("GetHatEfResName", "d>s", id - 0x9B5, &name);
    if (name != nullptr) {
      particle->Open(name);
    }
  }
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
// FUNCTION: fn_19040
void CEZ2STRScreenEffect::Slot12() {
  switch (this->id) {
  case 0xCC:
  case 0xCD:
  case 0xCE:
  case 0xCF:
  case 0xD2:
    this->AddEvent(0, "_heal_effect.wav", 0, 0);
    break;

  case 0xD0:
  case 0xD1:
  case 0xD3:
    this->AddEvent(0, "effect\\흡기.wav", 0, 0);
    break;

  case 0x426:
    this->AddEvent(0, "effect\\RL_HAMMER_OF_GOD.wav", 0, 0);
    break;

  case 0x44E:
    this->AddEvent(0, "effect\\su_tunabelly.wav", 0, 0);
    break;

  case 0x44D:
    this->AddEvent(0, "effect\\su_stemspear.wav", 0, 0);
    break;

  case 0x44F:
    this->AddEvent(0, "effect\\su_scratch.wav", 0, 0);
    break;

  case 0x453:
    this->AddEvent(0, "effect\\su_scaroftarou.wav", 0, 0);
    break;

  case 0x455:
    this->AddEvent(0, "effect\\su_pickypeck.wav", 0, 0);
    break;

  case 0x45C:
    this->AddEvent(0, "effect\\su_brunchofshrimp.wav", 0, 0);
    break;

  case 0x463:
    this->AddEvent(0, "effect\\su_grooming.wav", 0, 0);
    break;

  case 0x464:
    this->AddEvent(0, "effect\\su_chattering.wav", 0, 0);
    break;

  case 0x466:
    this->AddEvent(0, "effect\\su_hiss.wav", 0, 0);
    break;

  case 0x467:
    this->AddEvent(0, "effect\\su_powerofflock.wav", 0, 0);
    break;

  case 0x469:
    this->AddEvent(0, "effect\\su_svg_spirit.wav", 0, 0);
    break;

  case 0x489: {
    int random = rand() % 12;
    random = random < 0 ? 0 : random;
    if (random == 0) {
      this->AddEvent(0, "effect\\wizard_quagmire.wav", 0, std::bit_cast<int>(-100.0f));
    }
    break;
  }

  case 0x48A:
    this->AddEvent(0, "effect\\wizard_meteo.wav", 0, 0);
    this->AddEvent(0, "effect\\hunter_blastmine.wav", 0, 0);
    this->AddEvent(0, "effect\\hunter_blastmine.wav", 0xa, 0);
    this->AddEvent(0, "effect\\wizard_meteo.wav", 0x14, 0);
    this->AddEvent(0, "effect\\wizard_meteo.wav", 0x32, 0);
    this->AddEvent(0, "effect\\wizard_meteo.wav", 0x50, 0);
    this->AddEvent(0, "effect\\wizard_meteo.wav", 0x64, 0);
    this->AddEvent(0, "effect\\wizard_meteo.wav", 0x82, 0);
    this->AddEvent(0, "effect\\wizard_meteo.wav", 0x8c, 0);
    this->AddEvent(0, "effect\\wizard_meteo.wav", 0xb4, 0);
    this->AddEvent(0, "effect\\wizard_meteo.wav", 0xc8, 0);
    this->AddEvent(1, nullptr, 0x32, 0);
    this->AddEvent(1, nullptr, 0x64, 0);
    this->AddEvent(1, nullptr, 0x96, 0);
    this->AddEvent(1, nullptr, 0xc8, 0);
    this->AddEvent(1, nullptr, 0xfa, 0);
    break;

  case 0x48B:
    this->AddEvent(0, "effect\\wizard_meteor.wav", 0, 0);
    this->AddEvent(1, nullptr, 0x28, 0);
    break;

  case 0x58F:
  case 0x592:
  case 0x5DC:
  case 0x610:
  case 0x6C7:
  case 0x6D4:
  case 0x6E2:
  case 0x6EE:
  case 0x6F3:
  case 0x705:
  case 0x70A:
  case 0x740:
  case 0x763:
  case 0x7E6:
  case 0x7F1:
  case 0x7FC:
  case 0x85A:
    this->AddEvent(0, "effect\\EF_BeginSpell.wav", 0, 0);
    this->particle->flags = 13;
    break;

  case 0x554:
  case 0x5D7:
    this->particle->flags = 13;
    this->AddEvent(0, "ice_titan_attack.wav", 5, 0);
    break;

  case 0x573:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\EF_Bash.wav", 0, 0);
    this->AddEvent(0, "effect\\rog_intimidate.wav", 0x23, 0);
    break;

  case 0x580:
  case 0x5EF:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\EF_hit2.wav", 0xa, 0);
    this->AddEvent(0, "effect\\EF_hit4.wav", 0x14, 0);
    break;

  case 0x583:
    this->AddEvent(0, "effect\\EF_hit2.wav", 0, 0);
    break;

  case 0x590:
    this->AddEvent(0, "effect\\mon_맹룡과강.wav", 5, 0);
    this->AddEvent(0, "effect\\mon_맹룡과강.wav", 5, 0);
    this->AddEvent(0, "effect\\mon_맹룡과강.wav", 5, 0);
    break;

  case 0x585:
  case 0x5FE:
    this->AddEvent(0, "effect\\g크레이지위드.wav", 0, 0);
    break;

  case 0x60A:
  case 0x60B:
  case 0x64B:
  case 0x64C:
  case 0x64D:
  case 0x64E:
  case 0x64F:
  case 0x650:
    this->particle->flags = 5;
    break;

  case 0x60D:
    this->AddEvent(0, "effect\\EF_Bash.wav", 0, 0);
    break;

  case 0x60E:
    this->AddEvent(0, "hell_in.wav", 0, 0);
    break;

  case 0x615:
    this->AddEvent(0, "effect\\샤프슈팅.wav", 0, 0);
    this->AddEvent(0, "effect\\흡기.wav", 0, 0);
    break;

  case 0x619:
    this->AddEvent(0, "effect\\프레셔.wav", 3, 0);
    this->AddEvent(0, "effect\\S바레티르스피어.wav", 0x16, 0);
    this->AddEvent(1, nullptr, 0x16, 0);
    break;

  case 0x61E:
    this->AddEvent(0, "effect\\EF_StoneCurse.wav", 0, 0);
    break;

  case 0x638:
    this->AddEvent(0, "effect\\매직 크래쉬.wav", 0, 0);
    this->particle->flags = 13;
    break;

  case 0x647:
  case 0x648:
  case 0x907:
  case 0x908:
  case 0x90A:
  case 0x90B:
  case 0x90D:
  case 0x90E:
  case 0x95B:
  case 0x95C:
  case 0x95D:
  case 0x95E:
  case 0x95F:
  case 0x975:
  case 0x976:
    this->particle->flags = 517;
    break;

  case 0x649:
    this->particle->flags = 517;
    this->AddEvent(1, nullptr, 0xb4, 0);
    break;

  case 0x666:
    this->AddEvent(0, "chainlight.wav", 0, 0);
    break;

  case 0x66C:
    this->AddEvent(0, "effect\\priest_lexaeterna.wav", 0, 0);
    break;

  case 0x671:
  case 0x6CB:
  case 0x6CF:
  case 0x6DA:
  case 0x6DE:
  case 0x6EA:
  case 0x6F7:
  case 0x70E:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\EF_BeginSpell.wav", 0, 0);
    break;

  case 0x676:
    this->AddEvent(0, "effect\\EF_GlassWall.wav", 0x1e, 0);
    break;

  case 0x67E:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\DK_HACKANDSLASHER.wav", 0, 0);
    break;

  case 0x69A:
    this->AddEvent(0, "effect\\wl_siennaexecrate.wav", 0, 0);
    break;

  case 0x6A6:
  case 0x75D:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\wizard_fire_pillar_b.wav", 0xf, 0);
    this->AddEvent(1, nullptr, 0x14, 0);
    break;

  case 0x6AA:
    this->AddEvent(0, "effect\\wizard_earthspike.wav", 0x14, 0);
    this->AddEvent(1, nullptr, 0x14, 0);
    break;

  case 0x6B0:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\DK_SERVANT_W_SIGN.wav", 0x28, 0);
    break;

  case 0x6B2:
    this->AddEvent(0, "effect\\DK_SERVANT_W_PHANTOM.wav", 0, 0);
    break;

  case 0x6BA:
    this->AddEvent(0, "effect\\DK_DRAGONIC_AURA1.wav", 0, 0);
    break;

  case 0x6BC:
    this->AddEvent(0, "effect\\DK_DRAGONIC_AURA2.wav", 1, 0);
    break;

  case 0x6C0:
    this->AddEvent(0, "effect\\DK_MADNESS_CRUSHER.wav", 1, 0);
    break;

  case 0x6C4:
    this->AddEvent(0, "effect\\AG_DESTRUCTIVE_HURRICANE.wav", 0, 0);
    break;

  case 0x6CC: {
    CGameActor* actor = static_cast<CGameActor*>(FindRenderObject(this->actor));
    if (actor && actor->bodyState == 0 && actor->unknown_02C4 == 0) {
      this->AddEvent(0, "effect\\AG_DEADLY_PROJECTION.wav", 0, 0);
    }
    break;
  }

  case 0x6D1:
    this->AddEvent(0, "effect\\AG_ROCK_DOWN.wav", 0, 0);
    break;

  case 0x6E7:
    this->AddEvent(0, "effect\\DK_CHARGINGPIERCE1.wav", 0, 0);
    break;

  case 0x6F5:
    this->AddEvent(0, "effect\\AG_FROZEN_SLASH.wav", 0, 0);
    break;

  case 0x6D6:
    this->AddEvent(0, "effect\\DK_VIGOR.wav", 0, 0);
    break;

  case 0x6B7:
    this->AddEvent(0, "effect\\DK_SERVANT_W_DEMOL.wav", 0, 0);
    break;

  case 0x686:
  case 0x687:
  case 0x688:
  case 0x689:
  case 0x68A:
    for (int i = 0; i <= this->id - 0x686; ++i) {
      this->AddEvent(0, FormatOwnedString("effect\\DK_STORMSLASH%d.wav", rand() % 4 + 1).c_str(), i * 15, 0);
    }
    break;

  case 0x76F:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\MT_ABR_NET_REPAIR.wav", 0, 0);
    break;

  case 0x770:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\MT_ABR_NET_SUPPORT.wav", 0, 0);
    break;

  case 0x71C:
    this->AddEvent(0, "effect\\MT_D_MACHINE.wav", 0, 0);
    this->particle->flags = 13;
    break;

  case 0x794:
    this->particle->flags = 13;
    this->AddEvent(1, nullptr, 0x14, 0);
    break;

  case 0x824:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\wizard_fire_pillar_b.wav", 0xf, 0);
    this->AddEvent(1, nullptr, 0x28, 0);
    break;

  case 0x721:
    this->AddEvent(0, "effect\\CD_ARBITRIUM.wav", 0, 0);
    this->particle->flags = 13;
    break;

  case 0x788:
    this->AddEvent(0, "effect\\CD_DILECTIO_HEAL.wav", 0, 0);
    this->particle->flags = 13;
    break;

  case 0x73A:
    this->AddEvent(0, "effect\\CD_COMPETENTIA.wav", 0, 0);
    this->particle->flags = 13;
    break;

  case 0x73E:
    this->AddEvent(0, "effect\\AG_CRYSTAL_IMPACT.wav", 0, 0);
    break;

  case 0x7A9:
  case 0x7CA:
  case 0x7D5:
  case 0x7DB:
  case 0x7F6:
  case 0x835:
  case 0x83A:
  case 0x858:
  case 0x87B:
  case 0x87E:
  case 0x883:
  case 0x8BD:
  case 0x8C8:
  case 0x8D0:
    this->particle->flags = 13;
    [[fallthrough]];

  case 0x625:
  case 0x62A:
  case 0x62D:
  case 0x633:
  case 0x654:
  case 0x65D:
  case 0x663:
  case 0x669:
  case 0x66A:
  case 0x681:
  case 0x683:
  case 0x685:
  case 0x711:
  case 0x714:
  case 0x716:
  case 0x718:
  case 0x71E:
  case 0x723:
  case 0x745:
  case 0x746:
  case 0x74C:
  case 0x758:
  case 0x760:
  case 0x785:
  case 0x79E:
  case 0x7A6:
  case 0x7B1:
  case 0x7B4:
  case 0x7B8:
  case 0x7BB:
  case 0x7BF:
  case 0x86F:
  case 0x877:
  case 0x884:
  case 0x888:
  case 0x891:
  case 0x896:
    this->AddEvent(0, "effect\\EF_BeginSpell.wav", 0, 0);
    break;

  case 0x82E:
    this->AddEvent(0, "effect\\IQ_MASSIVE_F_BLASTER.wav", 0, 0);
    this->particle->flags = 13;
    break;

  case 0x826:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\IQ_THIRD_FLAME_BOMB.wav", 0, 0);
    break;

  case 0x841:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\IQ_THIRD_PUNISH.wav", 0, 0);
    break;

  case 0x7D8:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\TR_KVASIR_SONATA.wav", 0, 0);
    break;

  case 0x765:
    this->AddEvent(0, "effect\\AG_STRANTUM_TREMOR1.wav", 0, 0);
    [[fallthrough]];

  case 0x766:
    this->particle->flags = 13;
    switch (this->unknown_00B0) {
    case 1:
      this->Slot22(0.7f);
      break;
    case 2:
    case 3:
    case 4:
    case 5:
      this->Slot22(1.25f);
      break;
    }
    break;

  case 0x7A1: {
    float fromReciprocalDepth = 0.0f;
    float toReciprocalDepth = 0.0f;
    float fromX;
    float fromY;
    float toX;
    float toY;
    g_renderer->ProjectViewport(this->position, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &fromX, &fromY, &fromReciprocalDepth);
    g_renderer->ProjectViewport(this->sourcePosition, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &toX, &toY, &toReciprocalDepth);

    const float dx = toX - fromX;
    const float dy = toY - fromY;
    double angle = static_cast<float>(std::acos(static_cast<double>(dx / SquareRoot(dy * dy + dx * dx)))) / 3.141592f * 180.0f;
    if (fromY > toY) {
      angle = 360.0 - angle;
    }

    if (this->particle) {
      this->particle->m_angle = static_cast<float>(angle);
    }
    break;
  }

  case 0x7AC:
    this->particle->flags = 13;
    [[fallthrough]];

  case 0x7C3:
  case 0x7CF:
    this->AddEvent(0, "effect\\IQ_FAITH.wav", 0, 0);
    break;

  case 0x7C8:
    this->particle->m_repeat = 2;
    this->AddEvent(0, "effect\\EM_INCREASING_ACTIVITY.wav", 0, 0);
    break;

  case 0x7E2: {
    float fromReciprocalDepth = 0.0f;
    float toReciprocalDepth = 0.0f;
    float fromX;
    float fromY;
    float toX;
    float toY;
    g_renderer->ProjectViewport(this->position, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &fromX, &fromY, &fromReciprocalDepth);
    g_renderer->ProjectViewport(this->sourcePosition, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &toX, &toY, &toReciprocalDepth);

    const float dx = toX - fromX;
    const float dy = toY - fromY;
    double angle = static_cast<float>(std::acos(static_cast<double>(-dx / SquareRoot(dy * dy + dx * dx)))) / 3.141592f * 180.0f;
    if (toY > fromY) {
      angle = 360.0 - angle;
    }

    if (this->particle) {
      this->particle->m_angle = static_cast<float>(angle);
    }
    break;
  }

  case 0x7D1:
    this->particle->flags = 13;
    [[fallthrough]];

  case 0x7D7:
  case 0x7DE:
  case 0x7FE:
  case 0x80D:
  case 0x812:
  case 0x815:
  case 0x817:
  case 0x81C:
  case 0x829:
  case 0x845:
  case 0x848:
  case 0x84B:
  case 0x84D:
  case 0x84F:
  case 0x86B:
  case 0x873:
  case 0x88B:
  case 0x89C:
  case 0x8A4:
  case 0x8AC:
  case 0x8B4:
  case 0x8C2:
    this->AddEvent(0, "effect\\ef_beginspell.wav", 0, 0);
    break;

  case 0x7D9:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\EM_SUMMON_ELEMENTAL_SERPENS_2.wav", 0, 0);
    break;

  case 0x744:
    this->AddEvent(0, "effect\\CD_RELIGIO.wav", 0, 0);
    break;

  case 0x75F:
    this->AddEvent(0, "effect\\CD_BENEDICTUM.wav", 0, 0);
    break;

  case 0x713:
  case 0x715:
    this->AddEvent(0, "effect\\CD_ARGUTUS_VITA.wav", 0, 0);
    break;

  case 0x737:
    this->AddEvent(0, "effect\\CD_EFFLIGO.wav", 0, 0);
    break;

  case 0x717:
    this->AddEvent(0, "effect\\CD_PRESENS_ACIES.wav", 0, 0);
    break;

  case 0x789:
    this->AddEvent(0, "effect\\CD_FRAMEN.wav", 0, 0);
    break;

  case 0x7E3:
    this->AddEvent(0, "effect\\EM_SUMMON_ELEMENTAL_DILUVIO_2.wav", 0, 0);
    break;

  case 0x7EB:
    this->particle->life = static_cast<float>(EffectLifetime(0x7EB)) / 60.0f;
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\EM_DIAMOND_STORM.wav", 0, 0);
    break;

  case 0x7ED:
  case 0x80A:
    this->AddEvent(0, "effect\\ef_beginspell.wav", 0, 0);
    this->particle->flags = 13;
    break;

  case 0x7EF:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\EM_VENOM_SWAMP.wav", 0, 0);
    break;

  case 0x7F3:
    this->AddEvent(0, "effect\\EM_SUMMON_ELEMENTAL_ARDOR_2.wav", 0, 0);
    break;

  case 0x7F9:
    this->AddEvent(0, "effect\\EM_LIGHTNING_LAND.wav", 0, 0);
    break;

  case 0x802: {
    float fromReciprocalDepth = 0.0f;
    float toReciprocalDepth = 0.0f;
    float fromX;
    float fromY;
    float toX;
    float toY;
    g_renderer->ProjectViewport(this->position, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &fromX, &fromY, &fromReciprocalDepth);
    g_renderer->ProjectViewport(this->sourcePosition, static_cast<CGameMode*>(g_modeMgr.GetGameMode())->camera->frame.view.values, &toX, &toY, &toReciprocalDepth);

    const float dx = toX - fromX;
    const float dy = toY - fromY;
    double angle = static_cast<float>(std::acos(static_cast<double>(-dx / SquareRoot(dy * dy + dx * dx)))) / 3.141592f * 180.0f;
    if (toY > fromY) {
      angle = 360.0 - angle;
    }

    if (this->particle) {
      this->particle->m_angle = static_cast<float>(angle);
    }

    this->AddEvent(0, "effect\\TR_ROSEBLOSSOM_SHOT.wav", 0, 0);
    break;
  }

  case 0x807:
    this->AddEvent(0, "effect\\EM_CONFLAGRATION.wav", 0, 0);
    break;

  case 0x811:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\TR_JAWAII_SERENADE.wav", 0, 0);
    break;

  case 0x832:
    this->AddEvent(0, "effect\\EM_SUMMON_ELEMENTAL_PROCELLA_2.wav", 0, 0);
    break;

  case 0x838:
    this->AddEvent(0, "effect\\EM_SUMMON_ELEMENTAL_TERREMOTUS_2.wav", 0, 0);
    break;

  case 0x83E:
    this->particle->flags = 13;
    this->AddEvent(0, "effect\\ABC_FROM_THE_ABYSS.wav", 0, 0);
    break;

  case 0x771:
    this->AddEvent(0, "effect\\WH_HAWKRUSH.wav", 0, 0);
    break;

  case 0x76B:
    this->AddEvent(0, "effect\\WH_WIND_SIGN.wav", 0, 0);
    break;

  case 0x7A8:
    this->AddEvent(0, "effect\\WH_CALAMITYGALE.wav", 0, 0);
    break;

  case 0x752:
  case 0x756:
  case 0x75A:
  case 0x77F:
    this->AddEvent(0, "effect\\WH_TRAP.wav", 0, 0);
    break;

  case 0x753:
    this->AddEvent(0, "effect\\WH_DEEP_BLIND_TRAP_HIT.wav", 0, 0);
    break;

  case 0x75B:
    this->AddEvent(0, "effect\\WH_SOLID_TRAP_HIT.wav", 0, 0);
    break;

  case 0x757:
    this->AddEvent(0, "effect\\WH_SWIFT_TRAP_HIT.wav", 0, 0);
    break;

  case 0x782:
    this->AddEvent(0, "effect\\WH_FLAMETRAP_HIT.wav", 0, 0);
    break;

  case 0x855:
    this->AddEvent(0, "effect\\EM_TERRA_DRIVE.wav", 0, 0);
    break;

  case 0x7A2:
    this->AddEvent(0, "effect\\SHC_SHADOW_STAB.wav", 0, 0);
    break;

  case 0x77A:
    this->AddEvent(0, "effect\\SHC_ETERNAL_SLASH.wav", 0, 0);
    break;

  case 0x76A:
    this->AddEvent(0, "effect\\SHC_IMPACT_CRATER.wav", 0, 0);
    break;

  case 0x748:
    this->AddEvent(0, "effect\\SHC_FATAL_SHADOW_CROW.wav", 0, 0);
    break;

  case 0x78D:
    this->AddEvent(0, "effect\\SHC_POTENT_VENOM.wav", 0, 0);
    break;

  case 0x795:
    this->AddEvent(0, "effect\\MT_AXE_STOMP.wav", 0, 0);
    break;

  case 0x75E:
    this->AddEvent(0, "effect\\MT_RUSH_QUAKE.wav", 0, 0);
    break;

  case 0x8CC:
    this->particle->m_angle = static_cast<float>(ScreenAngle(*reinterpret_cast<const vector3d*>(this->position), *reinterpret_cast<const vector3d*>(this->sourcePosition)) + 180.0);
    this->AddEvent(0, "effect\\BO_THROWROCK.wav", 0, 0);
    break;

  case 0x8CA:
    this->particle->m_angle = static_cast<float>(ScreenAngle(*reinterpret_cast<const vector3d*>(this->position), *reinterpret_cast<const vector3d*>(this->sourcePosition)) + 180.0);
    this->AddEvent(0, "effect\\BO_WOODEN_ATTACK.wav", 0, 0);
    break;

  case 0x801:
    this->AddEvent(0, "effect\\TR_ROSEBLOSSOM_ROSE.wav", 0, 0);
    break;

  case 0x80C:
    this->AddEvent(0, "effect\\TR_SOUNDBLEND_SHOT.wav", 0, 0);
    break;

  case 0x80F:
    this->AddEvent(0, "effect\\TR_SOUNDBLEND_HIT.wav", 0, 0);
    break;

  case 0x810:
    this->AddEvent(0, "effect\\TR_METALIC_FURY.wav", 0, 0);
    break;

  case 0x844:
    this->AddEvent(0, "effect\\TR_GEF_NOCTURN.wav", 0, 0);
    break;

  case 0x847:
    this->AddEvent(0, "effect\\TR_ROKI_CAPRICCIO.wav", 0, 0);
    break;

  case 0x84C:
    this->AddEvent(0, "effect\\TR_AIN_RHAPSODY.wav", 0, 0);
    break;

  case 0x84A:
    this->AddEvent(0, "effect\\TR_MUSICAL_INTERLUDE.wav", 0, 0);
    break;

  case 0x81B:
    this->AddEvent(0, "effect\\TR_NIPELHEIM_REQUIEM.wav", 0, 0);
    break;

  case 0x814:
    this->AddEvent(0, "effect\\TR_PRON_MARCH.wav", 0, 0);
    break;

  case 0x7D0:
    this->AddEvent(0, "effect\\TR_MYSTIC_SYMPHONY.wav", 0, 0);
    break;

  case 0x7C4:
    this->AddEvent(0, "effect\\IQ_OLEUM_SANCTUM.wav", 0x32, 0);
    break;

  case 0x7CB:
    this->AddEvent(0, "effect\\IQ_EXPOSION_BLASTER.wav", 0, 0);
    break;

  case 0x7B9:
    this->AddEvent(0, "effect\\IQ_FIRST_FAITH_POWER.wav", 0, 0);
    break;

  case 0x7BC:
    this->AddEvent(0, "effect\\IQ_JUDGE.wav", 0, 0);
    break;

  case 0x7C0:
    this->AddEvent(0, "effect\\IQ_THIRD_EXOR_FLAME.wav", 0, 0);
    break;

  case 0x806:
    this->AddEvent(0, "effect\\IQ_FIRST_BRAND.wav", 0, 0);
    break;

  case 0x81E:
    this->AddEvent(0, "effect\\IQ_SECOND_FAITH.wav", 0x14, 0);
    break;

  case 0x823:
    this->AddEvent(0, "effect\\IQ_SECOND_FLAME.wav", 0, 0);
    break;

  case 0x831:
    this->AddEvent(0, "effect\\IQ_THIRD_CONSECRATION.wav", 0, 0);
    break;

  case 0x88A:
    this->AddEvent(0, "apocalips_attack.wav", 0, 0);
    break;

  case 0x871:
    this->AddEvent(0, "effect\\BO_ADVANCE_PROTECTION.wav", 0, 0);
    break;

  case 0x8D1:
    this->AddEvent(0, "effect\\BO_FAIRY_DUSTY.wav", 0, 0);
    break;

  case 0x8C1:
    this->AddEvent(0, "effect\\BO_RESEARCHREPORT.wav", 0, 0);
    break;

  case 0x8BF:
    this->AddEvent(0, "effect\\BO_HELL_HOWLING.wav", 0, 0);
    break;

  case 0x7AE:
    this->AddEvent(0, "effect\\ABC_ABYSS_DAGGER.wav", 0, 0);
    break;

  case 0x827:
    this->AddEvent(0, "effect\\ABC_ABYSS_STRIKE.wav", 0, 0);
    break;

  case 0x854:
    this->AddEvent(0, "effect\\ABC_DEFT_STAB1.wav", 0, 0);
    break;

  case 0x7E9:
    this->AddEvent(0, "effect\\EM_SPELL_ENCHANTING.wav", 0, 0);
    break;

  case 0x7D3:
    this->AddEvent(0, "effect\\EM_ACTIVITY_BURN.wav", 0, 0);
    break;

  case 0x85B:
    this->AddEvent(0, "effect\\EM_ELEMETAL_BUSTER_FIRE.wav", 0, 0);
    break;

  case 0x864:
    this->AddEvent(0, "effect\\EM_ELEMETAL_BUSTER_WATER.wav", 0, 0);
    break;

  case 0x861:
    this->AddEvent(0, "effect\\EM_ELEMETAL_BUSTER_POISON.wav", 0, 0);
    break;

  case 0x85E:
    this->AddEvent(0, "effect\\EM_ELEMETAL_BUSTER_GROUND.wav", 0, 0);
    break;

  case 0x867:
    this->AddEvent(0, "effect\\EM_ELEMETAL_BUSTER_WIND.wav", 0, 0);
    break;

  case 0x8D6:
    this->AddEvent(0, "effect\\EF_FireArrow1.wav", 0, 0);
    this->AddEvent(1, nullptr, 0, 0);
    break;

  case 0x8D4:
    this->AddEvent(0, "effect\\EF_IceArrow1.wav", 0, 0);
    this->AddEvent(1, nullptr, 0, 0);
    break;

  case 0x8D8:
    this->AddEvent(0, "effect\\EF_LightBolt3.wav", 0, 0);
    this->AddEvent(1, nullptr, 0, 0);
    break;

  case 0x8DA:
    this->AddEvent(0, "effect\\wizard_earthspike.wav", 0, 0);
    this->AddEvent(1, nullptr, 0, 0);
    break;

  case 0x8D2:
    this->AddEvent(0, "effect\\ef_detoxication.wav", 0, 0);
    this->AddEvent(1, nullptr, 0, 0);
    break;

  case 0x879:
  case 0x87C:
    this->AddEvent(0, "effect\\IG_GUARD_STANCE.wav", 0, 0);
    break;

  case 0x88E:
    this->AddEvent(0, "effect\\IG_GUARDIAN_SHIELD.wav", 0, 0);
    break;

  case 0x894:
    this->AddEvent(0, "effect\\IG_REBOUND_SHIELD.wav", 0, 0);
    break;

  case 0x898:
    this->AddEvent(0, "effect\\IG_ULTIMATE_SACRIFICE.wav", 0, 0);
    break;

  case 0x885:
    this->AddEvent(0, "effect\\IG_HOLY_SHIELD1.wav", 0, 0);
    break;

  case 0x886:
    this->AddEvent(0, "effect\\IG_HOLY_SHIELD2.wav", 0x22, 0);
    break;

  case 0x87F:
    this->AddEvent(0, "effect\\IG_GRAND_JUDGEMENT.wav", 0, 0);
    break;

  case 0x86D:
    this->AddEvent(0, "effect\\IG_OVERSLASH.wav", 0, 0);
    break;

  case 0x909:
  case 0x90C:
  case 0x90F:
    this->particle->flags = 0x205;
    this->AddEvent(1, nullptr, 0, 0);
    break;

  case 0x943:
    this->AddEvent(0, "effect\\drop_effect_1.wav", 0, 0);
    break;

  case 0x944:
    this->AddEvent(0, "effect\\drop_effect_2.wav", 0, 0);
    break;

  case 0x94E:
    this->AddEvent(0, "effect\\pokjuk_jap.wav", 0, 0);
    break;

  case 0x94F:
  case 0x950:
  case 0x951:
    this->particle->m_scale = 0.6f;
    break;

  case 0x952:
  case 0x953:
  case 0x954:
    this->particle->m_scale = 0.35f;
    break;

  case 0x4E7:
  case 0x4EB:
  case 0x4F6:
  case 0x4F7:
  case 0x4F8:
  case 0x4F9:
  case 0x4FB:
  case 0x4FD:
  case 0x503:
  case 0x504:
  case 0x507:
  case 0x509:
  case 0x50A:
  case 0x511:
  case 0x51F:
  case 0x521:
  case 0x544:
  case 0x54A:
  case 0x54C:
  case 0x54F:
  case 0x551:
  case 0x556:
  case 0x558:
  case 0x55A:
  case 0x55C:
  case 0x563:
  case 0x569:
  case 0x56B:
  case 0x571:
  case 0x575:
  case 0x586:
  case 0x587:
  case 0x589:
  case 0x595:
  case 0x59C:
  case 0x59D:
  case 0x59E:
  case 0x59F:
  case 0x5A0:
  case 0x5A2:
  case 0x5AB:
  case 0x5AC:
  case 0x5AD:
  case 0x5AF:
  case 0x5B0:
  case 0x5B4:
  case 0x5B6:
  case 0x5B8:
  case 0x5CE:
  case 0x5D1:
  case 0x5D3:
  case 0x5D5:
  case 0x5D9:
  case 0x5DA:
  case 0x5DE:
  case 0x5E0:
  case 0x5E2:
  case 0x5FF:
  case 0x600:
  case 0x602:
  case 0x60C:
  case 0x60F:
  case 0x612:
  case 0x616:
  case 0x61A:
  case 0x61C:
  case 0x623:
  case 0x629:
  case 0x62B:
  case 0x62E:
  case 0x632:
  case 0x634:
  case 0x636:
  case 0x63C:
  case 0x63E:
  case 0x655:
  case 0x656:
  case 0x659:
  case 0x65C:
  case 0x65E:
  case 0x660:
  case 0x662:
  case 0x664:
  case 0x665:
  case 0x668:
  case 0x66B:
  case 0x675:
  case 0x67B:
  case 0x682:
  case 0x684:
  case 0x68C:
  case 0x698:
  case 0x69B:
  case 0x69C:
  case 0x6A0:
  case 0x6A1:
  case 0x6A8:
  case 0x6AB:
  case 0x6B3:
  case 0x6B5:
  case 0x6BB:
  case 0x6BD:
  case 0x6BF:
  case 0x6C1:
  case 0x6C3:
  case 0x6C5:
  case 0x6D2:
  case 0x6E6:
  case 0x6EC:
  case 0x6F1:
  case 0x701:
  case 0x703:
  case 0x70B:
  case 0x71B:
  case 0x71F:
  case 0x73D:
  case 0x741:
  case 0x747:
  case 0x768:
  case 0x773:
  case 0x774:
  case 0x786:
  case 0x78A:
  case 0x790:
  case 0x7B7:
  case 0x7BA:
  case 0x7BD:
  case 0x7BE:
  case 0x7C1:
  case 0x7DF:
  case 0x7E4:
  case 0x7F4:
  case 0x7FA:
  case 0x7FF:
  case 0x804:
  case 0x808:
  case 0x80E:
  case 0x813:
  case 0x816:
  case 0x818:
  case 0x81F:
  case 0x828:
  case 0x82A:
  case 0x82C:
  case 0x830:
  case 0x833:
  case 0x846:
  case 0x849:
  case 0x850:
  case 0x852:
  case 0x856:
  case 0x85C:
  case 0x85F:
  case 0x862:
  case 0x865:
  case 0x868:
  case 0x86E:
  case 0x872:
  case 0x874:
  case 0x876:
  case 0x878:
  case 0x880:
  case 0x88D:
  case 0x890:
  case 0x892:
  case 0x895:
  case 0x897:
  case 0x899:
  case 0x89D:
  case 0x8A1:
  case 0x8A3:
  case 0x8A5:
  case 0x8A9:
  case 0x8AB:
  case 0x8AD:
  case 0x8B1:
  case 0x8B3:
  case 0x8B5:
  case 0x8B9:
  case 0x8BB:
  case 0x8C3:
  case 0x8D3:
  case 0x8D5:
  case 0x8D7:
  case 0x8D9:
  case 0x8DB:
  case 0x8E7:
  case 0x8E8:
  case 0x8FE:
  case 0x911:
  case 0x914:
  case 0x917:
  case 0x91C:
  case 0x91D:
  case 0x923:
  case 0x968:
    this->particle->flags = 13;
    break;

  default:
    if (this->id >= 0x9B5) {
      bool renderBefore = false;
      g_luaScript->Call("IsRenderBeforeCharacter", "d>b", this->id - 0x9B5, &renderBefore);
      if (renderBefore) {
        this->particle->flags = 13;
      }
    }
    break;
  }
}
