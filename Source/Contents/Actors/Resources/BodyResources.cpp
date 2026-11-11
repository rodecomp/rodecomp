#include "Client/Session/Session.h"
#include "Utility/Cipher/EncryptedName.h"
#include "Contents/Actors/Resources/EquipmentResourceMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"
#include "Utility/Cipher/CryptName.h"
#include "Utility/Cipher/Obfuscate.h"

#include <cstddef>
#include <cstdint>
#include <string>


// OFFSET: 0x00DC7440
// FUNCTION: fn_22794
template <>
const char* EncryptedName<9, 0xA5C5D1A3B9457B53ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 9, 0xA5C5D1A3B9457B53ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7470
// FUNCTION: fn_22795
template <>
const char* EncryptedName<10, 0xED5143E7C1B39315ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xED5143E7C1B39315ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC74A0
// FUNCTION: fn_22796
template <>
const char* EncryptedName<10, 0xEB3D4FAB0FC183DBULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xEB3D4FAB0FC183DBULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC74D0
// FUNCTION: fn_22797
template <>
const char* EncryptedName<10, 0xE3FB23537D2785BFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xE3FB23537D2785BFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7500
// FUNCTION: fn_22798
template <>
const char* EncryptedName<10, 0xDD69EDFFA18F537DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xDD69EDFFA18F537DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7530
// FUNCTION: fn_22799
template <>
const char* EncryptedName<10, 0xD98F03A52D35636DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xD98F03A52D35636DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7560
// FUNCTION: fn_22800
template <>
const char* EncryptedName<10, 0xD3E5516F89F161D1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xD3E5516F89F161D1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7590
// FUNCTION: fn_22801
template <>
const char* EncryptedName<10, 0xCBD7DDAFDF87F549ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xCBD7DDAFDF87F549ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC75C0
// FUNCTION: fn_22802
template <>
const char* EncryptedName<10, 0xC98BB32777F94371ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xC98BB32777F94371ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC75F0
// FUNCTION: fn_22803
template <>
const char* EncryptedName<10, 0xC7FB6DF18577D34BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xC7FB6DF18577D34BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7620
// FUNCTION: fn_22804
template <>
const char* EncryptedName<10, 0xC74BB3BB110FB10FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xC74BB3BB110FB10FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7650
// FUNCTION: fn_22805
template <>
const char* EncryptedName<10, 0xC733B7A333F9F571ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xC733B7A333F9F571ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7680
// FUNCTION: fn_22806
template <>
const char* EncryptedName<10, 0xC71BB3CDEB499DD1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xC71BB3CDEB499DD1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC76B0
// FUNCTION: fn_22807
template <>
const char* EncryptedName<10, 0xC52FB5FBB94FF917ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xC52FB5FBB94FF917ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC76E0
// FUNCTION: fn_22808
template <>
const char* EncryptedName<10, 0xC39FB559A17D3963ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xC39FB559A17D3963ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7710
// FUNCTION: fn_22809
template <>
const char* EncryptedName<10, 0xFFBF43D1FFD5FF49ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xFFBF43D1FFD5FF49ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7740
// FUNCTION: fn_22810
template <>
const char* EncryptedName<10, 0xB7A76BF9CD41F99FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xB7A76BF9CD41F99FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7770
// FUNCTION: fn_22811
template <>
const char* EncryptedName<10, 0xB3D5C99B2B05FFEFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xB3D5C99B2B05FFEFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC77A0
// FUNCTION: fn_22812
template <>
const char* EncryptedName<10, 0xB3CF9BEB7963DB89ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xB3CF9BEB7963DB89ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC77D0
// FUNCTION: fn_22813
template <>
const char* EncryptedName<10, 0xFB2BC75769E777D3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xFB2BC75769E777D3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7800
// FUNCTION: fn_22814
template <>
const char* EncryptedName<10, 0xAFCD4F69FBDFE157ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xAFCD4F69FBDFE157ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7830
// FUNCTION: fn_22815
template <>
const char* EncryptedName<10, 0xA7471985F93F9BDFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xA7471985F93F9BDFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7860
// FUNCTION: fn_22816
template <>
const char* EncryptedName<10, 0xA5F51F3989DBB99FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xA5F51F3989DBB99FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7890
// FUNCTION: fn_22817
template <>
const char* EncryptedName<10, 0x9D8F2D37BFFD61B7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x9D8F2D37BFFD61B7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC78C0
// FUNCTION: fn_22818
template <>
const char* EncryptedName<10, 0x9B9D51AD3515F779ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x9B9D51AD3515F779ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC78F0
// FUNCTION: fn_22819
template <>
const char* EncryptedName<10, 0x93E7A3A1F587CDC5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x93E7A3A1F587CDC5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7920
// FUNCTION: fn_22820
template <>
const char* EncryptedName<10, 0x936F3D2FFDFD87F5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x936F3D2FFDFD87F5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7950
// FUNCTION: fn_22821
template <>
const char* EncryptedName<10, 0x899D11BB83695B0DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x899D11BB83695B0DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7980
// FUNCTION: fn_22822
template <>
const char* EncryptedName<10, 0x8741152D8FB34B4FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x8741152D8FB34B4FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC79B0
// FUNCTION: fn_22823
template <>
const char* EncryptedName<10, 0xF5657D1F3DE5CDC7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xF5657D1F3DE5CDC7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC79E0
// FUNCTION: fn_22824
template <>
const char* EncryptedName<10, 0xF535E561137F138DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xF535E561137F138DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7A10
// FUNCTION: fn_22825
template <>
const char* EncryptedName<10, 0x17359BF953C367B3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x17359BF953C367B3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7A40
// FUNCTION: fn_22826
template <>
const char* EncryptedName<10, 0x190B8705C141D1CFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x190B8705C141D1CFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7A70
// FUNCTION: fn_22827
template <>
const char* EncryptedName<10, 0x2F9B717365C1250FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x2F9B717365C1250FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7AA0
// FUNCTION: fn_22828
template <>
const char* EncryptedName<10, 0x2FF3C5036D070127ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x2FF3C5036D070127ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7AD0
// FUNCTION: fn_22829
template <>
const char* EncryptedName<10, 0x31956551B311FD95ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x31956551B311FD95ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7B00
// FUNCTION: fn_22830
template <>
const char* EncryptedName<10, 0x3375B1C7DBB5FD55ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x3375B1C7DBB5FD55ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7B30
// FUNCTION: fn_22831
template <>
const char* EncryptedName<10, 0x33832D4D3FDFBDBFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x33832D4D3FDFBDBFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7B60
// FUNCTION: fn_22832
template <>
const char* EncryptedName<10, 0x3F5315DFE32B4FAFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x3F5315DFE32B4FAFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7B90
// FUNCTION: fn_22833
template <>
const char* EncryptedName<10, 0x3FAF85D995034FD7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x3FAF85D995034FD7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7BC0
// FUNCTION: fn_22834
template <>
const char* EncryptedName<10, 0x4755C1355575FD37ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x4755C1355575FD37ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7BF0
// FUNCTION: fn_22835
template <>
const char* EncryptedName<10, 0x490FFB43A12F0DB9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x490FFB43A12F0DB9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7C20
// FUNCTION: fn_22836
template <>
const char* EncryptedName<10, 0x4FE54379493FB5D5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x4FE54379493FB5D5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7C50
// FUNCTION: fn_22837
template <>
const char* EncryptedName<10, 0x51C57BB9F1DD9B0FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x51C57BB9F1DD9B0FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7C80
// FUNCTION: fn_22838
template <>
const char* EncryptedName<10, 0x59278F63D79F3DA1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x59278F63D79F3DA1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7CB0
// FUNCTION: fn_22839
template <>
const char* EncryptedName<10, 0x5B832361BDA77197ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x5B832361BDA77197ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7CE0
// FUNCTION: fn_22840
template <>
const char* EncryptedName<10, 0x67DD5BAB1919D9D1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x67DD5BAB1919D9D1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7D10
// FUNCTION: fn_22841
template <>
const char* EncryptedName<10, 0x6961FF056999A303ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x6961FF056999A303ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7D40
// FUNCTION: fn_22842
template <>
const char* EncryptedName<10, 0x6967633B9D89092BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x6967633B9D89092BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7D70
// FUNCTION: fn_22843
template <>
const char* EncryptedName<10, 0x69FD1D9733D3CFC1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x69FD1D9733D3CFC1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7DA0
// FUNCTION: fn_22844
template <>
const char* EncryptedName<10, 0x6BC977B3B5993503ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x6BC977B3B5993503ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7DD0
// FUNCTION: fn_22845
template <>
const char* EncryptedName<10, 0x732B390D63FD47D7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x732B390D63FD47D7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7E00
// FUNCTION: fn_22846
template <>
const char* EncryptedName<10, 0x7989830DE73B9DBDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x7989830DE73B9DBDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7E30
// FUNCTION: fn_22847
template <>
const char* EncryptedName<10, 0x79DF094DD9616575ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x79DF094DD9616575ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7E60
// FUNCTION: fn_22848
template <>
const char* EncryptedName<10, 0x7D61612937C1592DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x7D61612937C1592DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7E90
// FUNCTION: fn_22849
template <>
const char* EncryptedName<10, 0x7D750DADBDE9C3F1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x7D750DADBDE9C3F1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7EC0
// FUNCTION: fn_22850
template <>
const char* EncryptedName<10, 0x7FE563BD3FB7F925ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x7FE563BD3FB7F925ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7EF0
// FUNCTION: fn_22851
template <>
const char* EncryptedName<10, 0x0B5F6D6B23D5AFA7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x0B5F6D6B23D5AFA7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7F20
// FUNCTION: fn_22852
template <>
const char* EncryptedName<10, 0x0DF5B33D9F21EB79ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x0DF5B33D9F21EB79ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7F50
// FUNCTION: fn_22853
template <>
const char* EncryptedName<10, 0x0F0F8B6B8F9D19D1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x0F0F8B6B8F9D19D1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7F80
// FUNCTION: fn_22854
template <>
const char* EncryptedName<16, 0xE1F181F923B759E7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 16, 0xE1F181F923B759E7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7FB0
// FUNCTION: fn_22855
template <>
const char* EncryptedName<16, 0xFBDBDB13A77191BFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 16, 0xFBDBDB13A77191BFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC7FE0
// FUNCTION: fn_22856
template <>
const char* EncryptedName<16, 0x978B51F50DC5FD25ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 16, 0x978B51F50DC5FD25ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8010
// FUNCTION: fn_22857
template <>
const char* EncryptedName<16, 0x2B7BAB0B6971E3E9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 16, 0x2B7BAB0B6971E3E9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8040
// FUNCTION: fn_22858
template <>
const char* EncryptedName<16, 0x47B7BF55EB471557ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 16, 0x47B7BF55EB471557ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8070
// FUNCTION: fn_22859
template <>
const char* EncryptedName<18, 0xE5FDE3151BED831FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xE5FDE3151BED831FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC80A0
// FUNCTION: fn_22860
template <>
const char* EncryptedName<18, 0xD97BB73901ED99BFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xD97BB73901ED99BFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC80D0
// FUNCTION: fn_22861
template <>
const char* EncryptedName<18, 0xBD3F430159B551FDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xBD3F430159B551FDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8100
// FUNCTION: fn_22862
template <>
const char* EncryptedName<18, 0xBBD983C7FD611FE3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xBBD983C7FD611FE3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8130
// FUNCTION: fn_22863
template <>
const char* EncryptedName<18, 0xFBAD074BDB2B77A1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xFBAD074BDB2B77A1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8160
// FUNCTION: fn_22864
template <>
const char* EncryptedName<18, 0xB739073FE921AD6FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xB739073FE921AD6FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8190
// FUNCTION: fn_22865
template <>
const char* EncryptedName<18, 0xA1ED494BF319772DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xA1ED494BF319772DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC81C0
// FUNCTION: fn_22866
template <>
const char* EncryptedName<18, 0x9BE3EF737387AD79ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x9BE3EF737387AD79ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC81F0
// FUNCTION: fn_22867
template <>
const char* EncryptedName<18, 0x9B5B959B954D9333ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x9B5B959B954D9333ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8220
// FUNCTION: fn_22868
template <>
const char* EncryptedName<18, 0x99157F159BB99BD9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x99157F159BB99BD9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8250
// FUNCTION: fn_22869
template <>
const char* EncryptedName<18, 0x91651D81F559F971ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x91651D81F559F971ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8280
// FUNCTION: fn_22870
template <>
const char* EncryptedName<18, 0x8F4B518D839D1581ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x8F4B518D839D1581ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC82B0
// FUNCTION: fn_22871
template <>
const char* EncryptedName<18, 0x8F3F2FA959977F9FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x8F3F2FA959977F9FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC82E0
// FUNCTION: fn_22872
template <>
const char* EncryptedName<18, 0x8F1B5BFD5F6175B5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x8F1B5BFD5F6175B5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8310
// FUNCTION: fn_22873
template <>
const char* EncryptedName<18, 0x8DC5D10DAB1585BDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x8DC5D10DAB1585BDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8340
// FUNCTION: fn_22874
template <>
const char* EncryptedName<18, 0x17FFC98B398FAF61ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x17FFC98B398FAF61ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8370
// FUNCTION: fn_22875
template <>
const char* EncryptedName<18, 0x1D6D75EDE7CD311FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x1D6D75EDE7CD311FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC83A0
// FUNCTION: fn_22876
template <>
const char* EncryptedName<18, 0x1F01A13D8D915327ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x1F01A13D8D915327ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC83D0
// FUNCTION: fn_22877
template <>
const char* EncryptedName<18, 0x1F79132B518FD389ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x1F79132B518FD389ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8400
// FUNCTION: fn_22878
template <>
const char* EncryptedName<18, 0x21156F4141E35DD9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x21156F4141E35DD9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8430
// FUNCTION: fn_22879
template <>
const char* EncryptedName<18, 0x251761D7370BA5C3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x251761D7370BA5C3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8460
// FUNCTION: fn_22880
template <>
const char* EncryptedName<18, 0x2F1337956DC3CFD5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x2F1337956DC3CFD5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8490
// FUNCTION: fn_22881
template <>
const char* EncryptedName<18, 0x038535BB092533B7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x038535BB092533B7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC84C0
// FUNCTION: fn_22882
template <>
const char* EncryptedName<18, 0x393D916561BDC391ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x393D916561BDC391ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC84F0
// FUNCTION: fn_22883
template <>
const char* EncryptedName<18, 0x3B6D9BFD27A7C7F7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x3B6D9BFD27A7C7F7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8520
// FUNCTION: fn_22884
template <>
const char* EncryptedName<18, 0x410DD52D6731BFFDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x410DD52D6731BFFDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8550
// FUNCTION: fn_22885
template <>
const char* EncryptedName<18, 0x45E3F103CBB5F157ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x45E3F103CBB5F157ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8580
// FUNCTION: fn_22886
template <>
const char* EncryptedName<18, 0x050DF16FC75B25B9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x050DF16FC75B25B9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC85B0
// FUNCTION: fn_22887
template <>
const char* EncryptedName<18, 0x51372B6F4BA14B23ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x51372B6F4BA14B23ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC85E0
// FUNCTION: fn_22888
template <>
const char* EncryptedName<18, 0x51EB99CBCB075F35ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x51EB99CBCB075F35ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8610
// FUNCTION: fn_22889
template <>
const char* EncryptedName<18, 0x5B8F49AF09390D1BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x5B8F49AF09390D1BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8640
// FUNCTION: fn_22890
template <>
const char* EncryptedName<18, 0x73D1674B61DF7B0BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x73D1674B61DF7B0BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8670
// FUNCTION: fn_22891
template <>
const char* EncryptedName<18, 0x078333BB9725B7C9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x078333BB9725B7C9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC86A0
// FUNCTION: fn_22892
template <>
const char* EncryptedName<18, 0x798B394FF5FF434DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x798B394FF5FF434DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC86D0
// FUNCTION: fn_22893
template <>
const char* EncryptedName<18, 0x07B733370F612D0FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x07B733370F612D0FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8700
// FUNCTION: fn_22894
template <>
const char* EncryptedName<18, 0x7FCD77F9C7CBD14DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x7FCD77F9C7CBD14DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8730
// FUNCTION: fn_22895
template <>
const char* EncryptedName<18, 0x0BEB63A5C905B3A7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x0BEB63A5C905B3A7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8760
// FUNCTION: fn_22896
template <>
const char* EncryptedName<20, 0x33CD2BDD5B3927EDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 20, 0x33CD2BDD5B3927EDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8790
// FUNCTION: fn_22897
template <>
const char* EncryptedName<20, 0x656FAB972B3D21BBULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 20, 0x656FAB972B3D21BBULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC87C0
// FUNCTION: fn_22898
template <>
const char* EncryptedName<20, 0x7BAD7791F50F5D8DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 20, 0x7BAD7791F50F5D8DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC87F0
// FUNCTION: fn_22899
template <>
const char* EncryptedName<22, 0xD9FF511B2BB921E7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0xD9FF511B2BB921E7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8820
// FUNCTION: fn_22900
template <>
const char* EncryptedName<22, 0xCF391B0F8B9D95DBULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0xCF391B0F8B9D95DBULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8850
// FUNCTION: fn_22901
template <>
const char* EncryptedName<22, 0xC7912587FFF31B71ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0xC7912587FFF31B71ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8880
// FUNCTION: fn_22902
template <>
const char* EncryptedName<22, 0xABED6971354375C3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0xABED6971354375C3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC88B0
// FUNCTION: fn_22903
template <>
const char* EncryptedName<22, 0x8B65CD61012B5931ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x8B65CD61012B5931ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC88E0
// FUNCTION: fn_22904
template <>
const char* EncryptedName<22, 0x85D9CFCF0FBB71ABULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x85D9CFCF0FBB71ABULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8910
// FUNCTION: fn_22905
template <>
const char* EncryptedName<22, 0xF7530137154D8FE5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0xF7530137154D8FE5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8940
// FUNCTION: fn_22906
template <>
const char* EncryptedName<22, 0x151F77EBEBBB2147ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x151F77EBEBBB2147ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8970
// FUNCTION: fn_22907
template <>
const char* EncryptedName<22, 0x01E7898D9D235DDFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x01E7898D9D235DDFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC89A0
// FUNCTION: fn_22908
template <>
const char* EncryptedName<22, 0x3939079FF3395587ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x3939079FF3395587ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC89D0
// FUNCTION: fn_22909
template <>
const char* EncryptedName<22, 0x3D8387EF97A37DCFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x3D8387EF97A37DCFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8A00
// FUNCTION: fn_22910
template <>
const char* EncryptedName<22, 0x4917EB6D71B701ABULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x4917EB6D71B701ABULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8A30
// FUNCTION: fn_22911
template <>
const char* EncryptedName<22, 0x61FF87ED7BF39BE1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x61FF87ED7BF39BE1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8A60
// FUNCTION: fn_22912
template <>
const char* EncryptedName<22, 0x71B7691737C5A1AFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x71B7691737C5A1AFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8A90
// FUNCTION: fn_22913
template <>
const char* EncryptedName<22, 0x795383DD11AF7DA7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x795383DD11AF7DA7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8AC0
// FUNCTION: fn_22914
template <>
const char* EncryptedName<23, 0xC91F9D493F75FDA3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 23, 0xC91F9D493F75FDA3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8AF0
// FUNCTION: fn_22915
template <>
const char* EncryptedName<23, 0xF33D3D3B0BD9C53DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 23, 0xF33D3D3B0BD9C53DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8B20
// FUNCTION: fn_22916
template <>
const char* EncryptedName<23, 0x7959FFBF652D07BFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 23, 0x7959FFBF652D07BFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8B50
// FUNCTION: fn_22917
template <>
const char* EncryptedName<24, 0xA1DFCDAD555BB16DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 24, 0xA1DFCDAD555BB16DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8B80
// FUNCTION: fn_22918
template <>
const char* EncryptedName<24, 0x35F59F6F3DD1C965ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 24, 0x35F59F6F3DD1C965ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8BB0
// FUNCTION: fn_22919
template <>
const char* EncryptedName<25, 0xC1FD95811D1F4FA1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0xC1FD95811D1F4FA1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8BE0
// FUNCTION: fn_22920
template <>
const char* EncryptedName<25, 0xB70319D3A78D39CDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0xB70319D3A78D39CDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8C10
// FUNCTION: fn_22921
template <>
const char* EncryptedName<25, 0xAFF9332FD397533FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0xAFF9332FD397533FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8C40
// FUNCTION: fn_22922
template <>
const char* EncryptedName<25, 0x01C9FBA7819FFBD7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0x01C9FBA7819FFBD7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8C70
// FUNCTION: fn_22923
template <>
const char* EncryptedName<25, 0x6D05D5271B5D65C3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0x6D05D5271B5D65C3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8CA0
// FUNCTION: fn_22924
template <>
const char* EncryptedName<25, 0x0789EBC9F5C1C59FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0x0789EBC9F5C1C59FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8CD0
// FUNCTION: fn_22925
template <>
const char* EncryptedName<26, 0xBF0F412941076FADULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 26, 0xBF0F412941076FADULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8D00
// FUNCTION: fn_22926
template <>
const char* EncryptedName<26, 0x8BA52B970DD531ADULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 26, 0x8BA52B970DD531ADULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8D30
// FUNCTION: fn_22927
template <>
const char* EncryptedName<26, 0x29AB0B59651BDFD5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 26, 0x29AB0B59651BDFD5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8D60
// FUNCTION: fn_22928
template <>
const char* EncryptedName<26, 0x795787B7779D19DBULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 26, 0x795787B7779D19DBULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8D90
// FUNCTION: fn_22929
template <>
const char* EncryptedName<26, 0x7DC10333F96DE3E7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 26, 0x7DC10333F96DE3E7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8DC0
// FUNCTION: fn_22930
template <>
const char* EncryptedName<28, 0xD1538513DF975701ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0xD1538513DF975701ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8DF0
// FUNCTION: fn_22931
template <>
const char* EncryptedName<28, 0xCB3FBD4183DB8323ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0xCB3FBD4183DB8323ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8E20
// FUNCTION: fn_22932
template <>
const char* EncryptedName<28, 0xA1A55B4D1597EDD3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0xA1A55B4D1597EDD3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8E50
// FUNCTION: fn_22933
template <>
const char* EncryptedName<28, 0x999F5191852D2F6BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x999F5191852D2F6BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8E80
// FUNCTION: fn_22934
template <>
const char* EncryptedName<28, 0x91E147EBBD47C39FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x91E147EBBD47C39FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8EB0
// FUNCTION: fn_22935
template <>
const char* EncryptedName<28, 0x1BF915E1F37DFDFDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x1BF915E1F37DFDFDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8EE0
// FUNCTION: fn_22936
template <>
const char* EncryptedName<28, 0x01FF9313695DD9DFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x01FF9313695DD9DFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8F10
// FUNCTION: fn_22937
template <>
const char* EncryptedName<28, 0x49C3759B77DBD3C1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x49C3759B77DBD3C1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8F40
// FUNCTION: fn_22938
template <>
const char* EncryptedName<28, 0x4F1791FB1503C713ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x4F1791FB1503C713ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8F70
// FUNCTION: fn_22939
template <>
const char* EncryptedName<28, 0x6B3301E1A3079125ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x6B3301E1A3079125ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8FA0
// FUNCTION: fn_22940
template <>
const char* EncryptedName<28, 0x6DED3DD1CF75AD9BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x6DED3DD1CF75AD9BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC8FD0
// FUNCTION: fn_22941
template <>
const char* EncryptedName<28, 0x0753A7E1EFA9E139ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x0753A7E1EFA9E139ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9000
// FUNCTION: fn_22942
template <>
const char* EncryptedName<28, 0x7DB1050779034DE7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x7DB1050779034DE7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9030
// FUNCTION: fn_22943
template <>
const char* EncryptedName<28, 0x7FC1015DFFCF6D19ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0x7FC1015DFFCF6D19ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9060
// FUNCTION: fn_22944
template <>
const char* EncryptedName<29, 0x6BA557130BDFE997ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 29, 0x6BA557130BDFE997ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9090
// FUNCTION: fn_22945
template <>
const char* EncryptedName<29, 0x07A7D947CF7323BFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 29, 0x07A7D947CF7323BFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC90C0
// FUNCTION: fn_22946
template <>
const char* EncryptedName<30, 0xCFB9671DEF4BD9E7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 30, 0xCFB9671DEF4BD9E7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC90F0
// FUNCTION: fn_22947
template <>
const char* EncryptedName<30, 0xB59BADC5B365617DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 30, 0xB59BADC5B365617DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9120
// FUNCTION: fn_22948
template <>
const char* EncryptedName<30, 0x31E3CF9D358D8705ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 30, 0x31E3CF9D358D8705ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9150
// FUNCTION: fn_22949
template <>
const char* EncryptedName<31, 0xD7BBEB2DDB295D11ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 31, 0xD7BBEB2DDB295D11ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9180
// FUNCTION: fn_22950
template <>
const char* EncryptedName<31, 0x194F8339AF753FADULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 31, 0x194F8339AF753FADULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC91B0
// FUNCTION: fn_22951
template <>
const char* EncryptedName<33, 0xE9FBAB53D5A1BB17ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 33, 0xE9FBAB53D5A1BB17ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC91E0
// FUNCTION: fn_22952
template <>
const char* EncryptedName<33, 0xF9B1EF9F7961EFF5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 33, 0xF9B1EF9F7961EFF5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9210
// FUNCTION: fn_22953
template <>
const char* EncryptedName<33, 0x8DFF0B4B67D9FDE1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 33, 0x8DFF0B4B67D9FDE1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9240
// FUNCTION: fn_22954
template <>
const char* EncryptedName<33, 0x6B3917C939150B9BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 33, 0x6B3917C939150B9BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9270
// FUNCTION: fn_22955
template <>
const char* EncryptedName<34, 0x9B17C9EFDBB39F2FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 34, 0x9B17C9EFDBB39F2FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC92A0
// FUNCTION: fn_22956
template <>
const char* EncryptedName<34, 0x955D4DF94DE9BB07ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 34, 0x955D4DF94DE9BB07ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC92D0
// FUNCTION: fn_22957
template <>
const char* EncryptedName<34, 0x2B477F9B5B39B301ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 34, 0x2B477F9B5B39B301ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9300
// FUNCTION: fn_22958
template <>
const char* EncryptedName<34, 0x2D3B15774149D5DDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 34, 0x2D3B15774149D5DDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9330
// FUNCTION: fn_22959
template <>
const char* EncryptedName<34, 0x69D345DB950F336DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 34, 0x69D345DB950F336DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9360
// FUNCTION: fn_22960
template <>
const char* EncryptedName<35, 0x2F2103792383718BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 35, 0x2F2103792383718BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9390
// FUNCTION: fn_22961
template <>
const char* EncryptedName<37, 0x77D925073D6F19B9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 37, 0x77D925073D6F19B9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC93C0
// FUNCTION: fn_22962
template <>
const char* EncryptedName<42, 0x4349ADE96FEB27E5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 42, 0x4349ADE96FEB27E5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC93F0
// FUNCTION: fn_22963
template <>
const char* EncryptedName<44, 0xA55B7BEF3D7377E3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 44, 0xA55B7BEF3D7377E3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9420
// FUNCTION: fn_22964
template <>
const char* EncryptedName<44, 0x79A58B0F2173CF49ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 44, 0x79A58B0F2173CF49ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9450
// FUNCTION: fn_22965
template <>
const char* EncryptedName<46, 0xF5496753BDE7C5BDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 46, 0xF5496753BDE7C5BDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9480
// FUNCTION: fn_22966
template <>
const char* EncryptedName<11, 0x61F59D5D41E76519ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 11, 0x61F59D5D41E76519ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC94B0
// FUNCTION: fn_22967
template <>
const char* EncryptedName<12, 0xD97DC927AFE911F1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0xD97DC927AFE911F1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC94E0
// FUNCTION: fn_22968
template <>
const char* EncryptedName<12, 0xC757D96143298511ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0xC757D96143298511ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9510
// FUNCTION: fn_22969
template <>
const char* EncryptedName<12, 0x91E7A7DFA58F9B6BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0x91E7A7DFA58F9B6BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9540
// FUNCTION: fn_22970
template <>
const char* EncryptedName<12, 0x8773593DB181D573ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0x8773593DB181D573ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9570
// FUNCTION: fn_22971
template <>
const char* EncryptedName<12, 0x19BBC5910555FB97ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0x19BBC5910555FB97ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC95A0
// FUNCTION: fn_22972
template <>
const char* EncryptedName<12, 0x2529F16B0375530DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0x2529F16B0375530DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC95D0
// FUNCTION: fn_22973
template <>
const char* EncryptedName<12, 0x37D76D8F495113C3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0x37D76D8F495113C3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9600
// FUNCTION: fn_22974
template <>
const char* EncryptedName<13, 0x4B670715D543C1C3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 13, 0x4B670715D543C1C3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9630
// FUNCTION: fn_22975
template <>
const char* EncryptedName<14, 0xEB074B7FBDA3BFD5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xEB074B7FBDA3BFD5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9660
// FUNCTION: fn_22976
template <>
const char* EncryptedName<14, 0xE51B29334D353B81ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xE51B29334D353B81ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9690
// FUNCTION: fn_22977
template <>
const char* EncryptedName<14, 0xE50B2935073DD3E9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xE50B2935073DD3E9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC96C0
// FUNCTION: fn_22978
template <>
const char* EncryptedName<14, 0xD9115915B1919533ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xD9115915B1919533ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC96F0
// FUNCTION: fn_22979
template <>
const char* EncryptedName<14, 0xFD4F85BBA7E37D4FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xFD4F85BBA7E37D4FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9720
// FUNCTION: fn_22980
template <>
const char* EncryptedName<14, 0xD3755DD95F47A909ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xD3755DD95F47A909ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9750
// FUNCTION: fn_22981
template <>
const char* EncryptedName<14, 0xCBD1F3239BBBEDF7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xCBD1F3239BBBEDF7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9780
// FUNCTION: fn_22982
template <>
const char* EncryptedName<14, 0xC399B7091589B50BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xC399B7091589B50BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC97B0
// FUNCTION: fn_22983
template <>
const char* EncryptedName<14, 0xBDADDD87C5A50B6BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xBDADDD87C5A50B6BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC97E0
// FUNCTION: fn_22984
template <>
const char* EncryptedName<14, 0xB96B77B7255F8569ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xB96B77B7255F8569ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9810
// FUNCTION: fn_22985
template <>
const char* EncryptedName<14, 0xB5F3F779237315F3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xB5F3F779237315F3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9840
// FUNCTION: fn_22986
template <>
const char* EncryptedName<14, 0xB5D7B5AF4FEDFF57ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xB5D7B5AF4FEDFF57ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9870
// FUNCTION: fn_22987
template <>
const char* EncryptedName<14, 0xB3F1A10745C1F105ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xB3F1A10745C1F105ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC98A0
// FUNCTION: fn_22988
template <>
const char* EncryptedName<14, 0xAFA5617F178B6185ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xAFA5617F178B6185ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC98D0
// FUNCTION: fn_22989
template <>
const char* EncryptedName<14, 0xA15B555BE9CD2B81ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xA15B555BE9CD2B81ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9900
// FUNCTION: fn_22990
template <>
const char* EncryptedName<14, 0x9DEFC105A1EB2F71ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x9DEFC105A1EB2F71ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9930
// FUNCTION: fn_22991
template <>
const char* EncryptedName<14, 0x91DF1F5D3B014D45ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x91DF1F5D3B014D45ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9960
// FUNCTION: fn_22992
template <>
const char* EncryptedName<14, 0x91493751EF33B5FDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x91493751EF33B5FDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9990
// FUNCTION: fn_22993
template <>
const char* EncryptedName<14, 0x8BBF792B47E72173ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x8BBF792B47E72173ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC99C0
// FUNCTION: fn_22994
template <>
const char* EncryptedName<14, 0x8B3FEB5B8BD5C3C3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x8B3FEB5B8BD5C3C3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC99F0
// FUNCTION: fn_22995
template <>
const char* EncryptedName<14, 0x8B3FA35B5BDDDF89ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x8B3FA35B5BDDDF89ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9A20
// FUNCTION: fn_22996
template <>
const char* EncryptedName<14, 0x8B03E17DA967E943ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x8B03E17DA967E943ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9A50
// FUNCTION: fn_22997
template <>
const char* EncryptedName<14, 0x8971CB79C18D8B27ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x8971CB79C18D8B27ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9A80
// FUNCTION: fn_22998
template <>
const char* EncryptedName<14, 0x894FEDABAFCF45E3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x894FEDABAFCF45E3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9AB0
// FUNCTION: fn_22999
template <>
const char* EncryptedName<14, 0x875FCDF1E7A1795FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x875FCDF1E7A1795FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9AE0
// FUNCTION: fn_23000
template <>
const char* EncryptedName<14, 0x85FB93CF2F494179ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x85FB93CF2F494179ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9B10
// FUNCTION: fn_23001
template <>
const char* EncryptedName<14, 0x812BE72BCD5707B7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x812BE72BCD5707B7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9B40
// FUNCTION: fn_23002
template <>
const char* EncryptedName<14, 0xFF7D83CB791339D9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xFF7D83CB791339D9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9B70
// FUNCTION: fn_23003
template <>
const char* EncryptedName<14, 0x113DC179F1C795C9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x113DC179F1C795C9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9BA0
// FUNCTION: fn_23004
template <>
const char* EncryptedName<14, 0x11CB35636377DD07ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x11CB35636377DD07ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9BD0
// FUNCTION: fn_23005
template <>
const char* EncryptedName<14, 0x1B3B756FE399E1D9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x1B3B756FE399E1D9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9C00
// FUNCTION: fn_23006
template <>
const char* EncryptedName<14, 0x1FDB81175D1557E3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x1FDB81175D1557E3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9C30
// FUNCTION: fn_23007
template <>
const char* EncryptedName<14, 0x2B034DCBB1812785ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x2B034DCBB1812785ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9C60
// FUNCTION: fn_23008
template <>
const char* EncryptedName<14, 0x2B8725D7E95325F5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x2B8725D7E95325F5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9C90
// FUNCTION: fn_23009
template <>
const char* EncryptedName<14, 0x2D33791F67AD47DFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x2D33791F67AD47DFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9CC0
// FUNCTION: fn_23010
template <>
const char* EncryptedName<14, 0x2D394DBD0F332733ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x2D394DBD0F332733ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9CF0
// FUNCTION: fn_23011
template <>
const char* EncryptedName<14, 0x2F2701A7E56D3D89ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x2F2701A7E56D3D89ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9D20
// FUNCTION: fn_23012
template <>
const char* EncryptedName<14, 0x2F371DDDADDD53CFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x2F371DDDADDD53CFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9D50
// FUNCTION: fn_23013
template <>
const char* EncryptedName<14, 0x03014749B127914FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x03014749B127914FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9D80
// FUNCTION: fn_23014
template <>
const char* EncryptedName<14, 0x31D7C1EDA345E36DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x31D7C1EDA345E36DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9DB0
// FUNCTION: fn_23015
template <>
const char* EncryptedName<14, 0x31F5C32DD5C53779ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x31F5C32DD5C53779ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9DE0
// FUNCTION: fn_23016
template <>
const char* EncryptedName<14, 0x39E7F13755252F8FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x39E7F13755252F8FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9E10
// FUNCTION: fn_23017
template <>
const char* EncryptedName<14, 0x3B915119EFD523EFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x3B915119EFD523EFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9E40
// FUNCTION: fn_23018
template <>
const char* EncryptedName<14, 0x3D075BD5256F9B6FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x3D075BD5256F9B6FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9E70
// FUNCTION: fn_23019
template <>
const char* EncryptedName<14, 0x3F557BE333F95F6DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x3F557BE333F95F6DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9EA0
// FUNCTION: fn_23020
template <>
const char* EncryptedName<14, 0x451FD72783630709ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x451FD72783630709ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9ED0
// FUNCTION: fn_23021
template <>
const char* EncryptedName<14, 0x474DC581D5E98357ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x474DC581D5E98357ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9F00
// FUNCTION: fn_23022
template <>
const char* EncryptedName<14, 0x4B1D078937DB892BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x4B1D078937DB892BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9F30
// FUNCTION: fn_23023
template <>
const char* EncryptedName<14, 0x5D6B3B1DEB2397B1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x5D6B3B1DEB2397B1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9F60
// FUNCTION: fn_23024
template <>
const char* EncryptedName<14, 0x5D6B8FCBE3FB97A1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x5D6B8FCBE3FB97A1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9F90
// FUNCTION: fn_23025
template <>
const char* EncryptedName<14, 0x5FFD7FABB5D1EF57ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x5FFD7FABB5D1EF57ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9FC0
// FUNCTION: fn_23026
template <>
const char* EncryptedName<14, 0x07112F85A39F4FABULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x07112F85A39F4FABULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DC9FF0
// FUNCTION: fn_23027
template <>
const char* EncryptedName<14, 0x7155E5EF0DFB0D03ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x7155E5EF0DFB0D03ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DCA020
// FUNCTION: fn_23028
template <>
const char* EncryptedName<14, 0x71A509DB43792991ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x71A509DB43792991ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DCA050
// FUNCTION: fn_23029
template <>
const char* EncryptedName<14, 0x072D2D872D259D5BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x072D2D872D259D5BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DCA080
// FUNCTION: fn_23030
template <>
const char* EncryptedName<14, 0x73C32959FDFFA51BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x73C32959FDFFA51BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DCA0B0
// FUNCTION: fn_23031
template <>
const char* EncryptedName<14, 0x078143534999254DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x078143534999254DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DCA0E0
// FUNCTION: fn_23032
template <>
const char* EncryptedName<14, 0x7D993F650D2785FFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x7D993F650D2785FFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DCA110
// FUNCTION: fn_23033
template <>
const char* EncryptedName<14, 0x7DBBD74FA36F3BCBULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x7DBBD74FA36F3BCBULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00DCA140
// FUNCTION: fn_23034
template <>
const char* EncryptedName<14, 0x0D7FA94DD32361A9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x0D7FA94DD32361A9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E281B0
// FUNCTION: fn_23949
template <>
EncryptedName<14, 0xB5F3F779237315F3ULL>::EncryptedName(const char (&source)[14]) : m_encrypted(true) {
  for (std::uint64_t index = 0; index < 14; ++index) {
    this->m_text[index] = source[index];
  }
}

// OFFSET: 0x00E28390
// FUNCTION: fn_23950
template <>
const char* EncryptedName<10, 0xD9115915B1919533ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xD9115915B1919533ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E283C0
// FUNCTION: fn_23951
template <>
const char* EncryptedName<10, 0xBDADDD87C5A50B6BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xBDADDD87C5A50B6BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E283F0
// FUNCTION: fn_23952
template <>
const char* EncryptedName<10, 0xFBAD074BDB2B77A1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xFBAD074BDB2B77A1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28420
// FUNCTION: fn_23953
template <>
const char* EncryptedName<10, 0xA55B7BEF3D7377E3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0xA55B7BEF3D7377E3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28450
// FUNCTION: fn_23954
template <>
const char* EncryptedName<10, 0x99157F159BB99BD9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x99157F159BB99BD9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28480
// FUNCTION: fn_23955
template <>
const char* EncryptedName<10, 0x31E3CF9D358D8705ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x31E3CF9D358D8705ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E284B0
// FUNCTION: fn_23956
template <>
const char* EncryptedName<10, 0x451FD72783630709ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x451FD72783630709ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E284E0
// FUNCTION: fn_23957
template <>
const char* EncryptedName<10, 0x050DF16FC75B25B9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x050DF16FC75B25B9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28510
// FUNCTION: fn_23958
template <>
const char* EncryptedName<10, 0x6BA557130BDFE997ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x6BA557130BDFE997ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28540
// FUNCTION: fn_23959
template <>
const char* EncryptedName<10, 0x73C32959FDFFA51BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x73C32959FDFFA51BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28570
// FUNCTION: fn_23960
template <>
const char* EncryptedName<10, 0x0789EBC9F5C1C59FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x0789EBC9F5C1C59FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E285A0
// FUNCTION: fn_23961
template <>
const char* EncryptedName<10, 0x79A58B0F2173CF49ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 10, 0x79A58B0F2173CF49ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E285D0
// FUNCTION: fn_23962
template <>
const char* EncryptedName<16, 0xD3E5516F89F161D1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 16, 0xD3E5516F89F161D1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28600
// FUNCTION: fn_23963
template <>
const char* EncryptedName<16, 0xA7471985F93F9BDFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 16, 0xA7471985F93F9BDFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28630
// FUNCTION: fn_23964
template <>
const char* EncryptedName<16, 0x91E7A7DFA58F9B6BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 16, 0x91E7A7DFA58F9B6BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28660
// FUNCTION: fn_23965
template <>
const char* EncryptedName<16, 0x2D33791F67AD47DFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 16, 0x2D33791F67AD47DFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28690
// FUNCTION: fn_23966
template <>
const char* EncryptedName<16, 0x45E3F103CBB5F157ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 16, 0x45E3F103CBB5F157ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E286C0
// FUNCTION: fn_23967
template <>
const char* EncryptedName<18, 0xE51B29334D353B81ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xE51B29334D353B81ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E286F0
// FUNCTION: fn_23968
template <>
const char* EncryptedName<18, 0xC39FB559A17D3963ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xC39FB559A17D3963ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28720
// FUNCTION: fn_23969
template <>
const char* EncryptedName<18, 0xB3F1A10745C1F105ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xB3F1A10745C1F105ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28750
// FUNCTION: fn_23970
template <>
const char* EncryptedName<18, 0xABED6971354375C3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xABED6971354375C3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28780
// FUNCTION: fn_23971
template <>
const char* EncryptedName<18, 0x91DF1F5D3B014D45ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x91DF1F5D3B014D45ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E287B0
// FUNCTION: fn_23972
template <>
const char* EncryptedName<18, 0x8B3FA35B5BDDDF89ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x8B3FA35B5BDDDF89ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E287E0
// FUNCTION: fn_23973
template <>
const char* EncryptedName<18, 0x899D11BB83695B0DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x899D11BB83695B0DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28810
// FUNCTION: fn_23974
template <>
const char* EncryptedName<18, 0xF5496753BDE7C5BDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0xF5496753BDE7C5BDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28840
// FUNCTION: fn_23975
template <>
const char* EncryptedName<18, 0x01C9FBA7819FFBD7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x01C9FBA7819FFBD7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28870
// FUNCTION: fn_23976
template <>
const char* EncryptedName<18, 0x2F371DDDADDD53CFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x2F371DDDADDD53CFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E288A0
// FUNCTION: fn_23977
template <>
const char* EncryptedName<18, 0x35F59F6F3DD1C965ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x35F59F6F3DD1C965ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E288D0
// FUNCTION: fn_23978
template <>
const char* EncryptedName<18, 0x3D8387EF97A37DCFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x3D8387EF97A37DCFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28900
// FUNCTION: fn_23979
template <>
const char* EncryptedName<18, 0x47B7BF55EB471557ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x47B7BF55EB471557ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28930
// FUNCTION: fn_23980
template <>
const char* EncryptedName<18, 0x6B3917C939150B9BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x6B3917C939150B9BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28960
// FUNCTION: fn_23981
template <>
const char* EncryptedName<18, 0x6D05D5271B5D65C3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x6D05D5271B5D65C3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28990
// FUNCTION: fn_23982
template <>
const char* EncryptedName<18, 0x795787B7779D19DBULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x795787B7779D19DBULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E289C0
// FUNCTION: fn_23983
template <>
const char* EncryptedName<18, 0x7959FFBF652D07BFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x7959FFBF652D07BFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E289F0
// FUNCTION: fn_23984
template <>
const char* EncryptedName<18, 0x7FE563BD3FB7F925ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x7FE563BD3FB7F925ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28A20
// FUNCTION: fn_23985
template <>
const char* EncryptedName<18, 0x0DF5B33D9F21EB79ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 18, 0x0DF5B33D9F21EB79ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28A50
// FUNCTION: fn_23986
template <>
const char* EncryptedName<20, 0xA15B555BE9CD2B81ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 20, 0xA15B555BE9CD2B81ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28A80
// FUNCTION: fn_23987
template <>
const char* EncryptedName<20, 0x2F2103792383718BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 20, 0x2F2103792383718BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28AB0
// FUNCTION: fn_23988
template <>
const char* EncryptedName<20, 0x2F2701A7E56D3D89ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 20, 0x2F2701A7E56D3D89ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28AE0
// FUNCTION: fn_23989
template <>
const char* EncryptedName<20, 0x3B6D9BFD27A7C7F7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 20, 0x3B6D9BFD27A7C7F7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28B10
// FUNCTION: fn_23990
template <>
const char* EncryptedName<22, 0xD97DC927AFE911F1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0xD97DC927AFE911F1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28B40
// FUNCTION: fn_23991
template <>
const char* EncryptedName<22, 0xBF0F412941076FADULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0xBF0F412941076FADULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28B70
// FUNCTION: fn_23992
template <>
const char* EncryptedName<22, 0x5D6B3B1DEB2397B1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x5D6B3B1DEB2397B1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28BA0
// FUNCTION: fn_23993
template <>
const char* EncryptedName<22, 0x07112F85A39F4FABULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x07112F85A39F4FABULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28BD0
// FUNCTION: fn_23994
template <>
const char* EncryptedName<22, 0x7DB1050779034DE7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 22, 0x7DB1050779034DE7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28C00
// FUNCTION: fn_23995
template <>
const char* EncryptedName<23, 0xF9B1EF9F7961EFF5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 23, 0xF9B1EF9F7961EFF5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28C30
// FUNCTION: fn_23996
template <>
const char* EncryptedName<23, 0x2B8725D7E95325F5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 23, 0x2B8725D7E95325F5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28C60
// FUNCTION: fn_23997
template <>
const char* EncryptedName<23, 0x71B7691737C5A1AFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 23, 0x71B7691737C5A1AFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28C90
// FUNCTION: fn_23998
template <>
const char* EncryptedName<24, 0x3BBB5F0F8F216F31ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 24, 0x3BBB5F0F8F216F31ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28CC0
// FUNCTION: fn_23999
template <>
const char* EncryptedName<24, 0x4B1D078937DB892BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 24, 0x4B1D078937DB892BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28CF0
// FUNCTION: fn_24000
template <>
const char* EncryptedName<24, 0x61F59D5D41E76519ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 24, 0x61F59D5D41E76519ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28D20
// FUNCTION: fn_24001
template <>
const char* EncryptedName<25, 0x8BA52B970DD531ADULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0x8BA52B970DD531ADULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28D50
// FUNCTION: fn_24002
template <>
const char* EncryptedName<25, 0x2529F16B0375530DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0x2529F16B0375530DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28D80
// FUNCTION: fn_24003
template <>
const char* EncryptedName<25, 0x29AB0B59651BDFD5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0x29AB0B59651BDFD5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28DB0
// FUNCTION: fn_24004
template <>
const char* EncryptedName<25, 0x0D7FA94DD32361A9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 25, 0x0D7FA94DD32361A9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28DE0
// FUNCTION: fn_24005
template <>
const char* EncryptedName<26, 0xED730315539785C7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 26, 0xED730315539785C7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28E10
// FUNCTION: fn_24006
template <>
const char* EncryptedName<26, 0xD51953E763B94747ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 26, 0xD51953E763B94747ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28E40
// FUNCTION: fn_24007
template <>
const char* EncryptedName<26, 0xA1DFCDAD555BB16DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 26, 0xA1DFCDAD555BB16DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28E70
// FUNCTION: fn_24008
template <>
const char* EncryptedName<26, 0x8DFF0B4B67D9FDE1ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 26, 0x8DFF0B4B67D9FDE1ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28EA0
// FUNCTION: fn_24009
template <>
const char* EncryptedName<26, 0x194F8339AF753FADULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 26, 0x194F8339AF753FADULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28ED0
// FUNCTION: fn_24010
template <>
const char* EncryptedName<27, 0xCFB9671DEF4BD9E7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 27, 0xCFB9671DEF4BD9E7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28F00
// FUNCTION: fn_24011
template <>
const char* EncryptedName<28, 0xE9FBAB53D5A1BB17ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 28, 0xE9FBAB53D5A1BB17ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28F30
// FUNCTION: fn_24012
template <>
const char* EncryptedName<29, 0xD7BBEB2DDB295D11ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 29, 0xD7BBEB2DDB295D11ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28F60
// FUNCTION: fn_24013
template <>
const char* EncryptedName<30, 0xA5C5D1A3B9457B53ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 30, 0xA5C5D1A3B9457B53ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28F90
// FUNCTION: fn_24014
template <>
const char* EncryptedName<30, 0x39E7F13755252F8FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 30, 0x39E7F13755252F8FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28FC0
// FUNCTION: fn_24015
template <>
const char* EncryptedName<30, 0x77D925073D6F19B9ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 30, 0x77D925073D6F19B9ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E28FF0
// FUNCTION: fn_24016
template <>
const char* EncryptedName<31, 0x2F3D75DFCF678FF3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 31, 0x2F3D75DFCF678FF3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29020
// FUNCTION: fn_24017
template <>
const char* EncryptedName<31, 0x7155E5EF0DFB0D03ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 31, 0x7155E5EF0DFB0D03ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29050
// FUNCTION: fn_24018
template <>
const char* EncryptedName<32, 0x4349ADE96FEB27E5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 32, 0x4349ADE96FEB27E5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29080
// FUNCTION: fn_24019
template <>
const char* EncryptedName<33, 0x17FB0F6BFF3BE93FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 33, 0x17FB0F6BFF3BE93FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E290B0
// FUNCTION: fn_24020
template <>
const char* EncryptedName<33, 0x51372B6F4BA14B23ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 33, 0x51372B6F4BA14B23ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E290E0
// FUNCTION: fn_24021
template <>
const char* EncryptedName<33, 0x6F9D21B96D71C1FFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 33, 0x6F9D21B96D71C1FFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29110
// FUNCTION: fn_24022
template <>
const char* EncryptedName<33, 0x7DC10333F96DE3E7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 33, 0x7DC10333F96DE3E7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29140
// FUNCTION: fn_24023
template <>
const char* EncryptedName<35, 0xAFF9332FD397533FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 35, 0xAFF9332FD397533FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29170
// FUNCTION: fn_24024
template <>
const char* EncryptedName<37, 0xCBD7DDAFDF87F549ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 37, 0xCBD7DDAFDF87F549ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E291A0
// FUNCTION: fn_24025
template <>
const char* EncryptedName<39, 0x999F5191852D2F6BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 39, 0x999F5191852D2F6BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E291D0
// FUNCTION: fn_24026
template <>
const char* EncryptedName<42, 0x1F79132B518FD389ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 42, 0x1F79132B518FD389ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29200
// FUNCTION: fn_24027
template <>
const char* EncryptedName<44, 0xB70319D3A78D39CDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 44, 0xB70319D3A78D39CDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29230
// FUNCTION: fn_24028
template <>
const char* EncryptedName<44, 0x8F61434F0F79DDD7ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 44, 0x8F61434F0F79DDD7ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29260
// FUNCTION: fn_24029
template <>
const char* EncryptedName<46, 0x4B670715D543C1C3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 46, 0x4B670715D543C1C3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29290
// FUNCTION: fn_24030
template <>
const char* EncryptedName<12, 0xEB074B7FBDA3BFD5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0xEB074B7FBDA3BFD5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E292C0
// FUNCTION: fn_24031
template <>
const char* EncryptedName<12, 0xB59BADC5B365617DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0xB59BADC5B365617DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E292F0
// FUNCTION: fn_24032
template <>
const char* EncryptedName<12, 0x936F3D2FFDFD87F5ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0x936F3D2FFDFD87F5ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29320
// FUNCTION: fn_24033
template <>
const char* EncryptedName<12, 0xF535E561137F138DULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0xF535E561137F138DULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29350
// FUNCTION: fn_24034
template <>
const char* EncryptedName<12, 0x3D075BD5256F9B6FULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0x3D075BD5256F9B6FULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29380
// FUNCTION: fn_24035
template <>
const char* EncryptedName<12, 0x5FFD7FABB5D1EF57ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0x5FFD7FABB5D1EF57ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E293B0
// FUNCTION: fn_24036
template <>
const char* EncryptedName<12, 0x6B3301E1A3079125ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 12, 0x6B3301E1A3079125ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E293E0
// FUNCTION: fn_24037
template <>
const char* EncryptedName<14, 0xC7FB6DF18577D34BULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xC7FB6DF18577D34BULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29410
// FUNCTION: fn_24038
template <>
const char* EncryptedName<14, 0xBD3F430159B551FDULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0xBD3F430159B551FDULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29440
// FUNCTION: fn_24039
template <>
const char* EncryptedName<14, 0x01E7898D9D235DDFULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x01E7898D9D235DDFULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29470
// FUNCTION: fn_24040
template <>
const char* EncryptedName<14, 0x251761D7370BA5C3ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x251761D7370BA5C3ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E294A0
// FUNCTION: fn_24041
template <>
const char* EncryptedName<14, 0x2B477F9B5B39B301ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x2B477F9B5B39B301ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E294D0
// FUNCTION: fn_24042
template <>
const char* EncryptedName<14, 0x6961FF056999A303ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x6961FF056999A303ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E29500
// FUNCTION: fn_24043
template <>
const char* EncryptedName<14, 0x6BC977B3B5993503ULL>::Text() {
  if (this->m_encrypted) {
    utility::XorBytes(this->m_text, 14, 0x6BC977B3B5993503ULL);
    this->m_encrypted = false;
  }

  return reinterpret_cast<const char*>(this->m_text);
}

// OFFSET: 0x00E3D300
// FUNCTION: fn_24186
bool CSession::IsSecondJob(int job) {
  return job >= JT_RUNE_KNIGHT_2ND && job < JT_SECOND_JOB_END;
}

// OFFSET: 0x00E0A3A0
// FUNCTION: fn_23795
std::string CSession::GetBodyJobName(int job) {
  return this->m_bodyJobNames[job];
}

// OFFSET: 0x00BAB5D0
// FUNCTION: fn_19752
std::string EquipmentResourceMgr::GetBodyImfName(int job, int sex, int, int costumeJob) {
  const int spriteJob = g_session.GetSpriteJob(job, costumeJob, true);
  std::string sexName;
  std::string jobName;
  std::string result;

  sexName = static_cast<const std::string&>(g_session.GetSexName(sex));
  jobName = static_cast<const std::string&>(g_session.GetImfName(spriteJob));
  result = static_cast<const std::string&>(FormatOwnedString("%s_%s.imf", jobName.c_str(), sexName.c_str()));

  return result;
}

// OFFSET: 0x00BAB850
// FUNCTION: fn_19753
std::string EquipmentResourceMgr::GetBodyResourceName(int type, int job, int sex, int costumeJob, int costume) {
  std::string result;
  std::string sexName;
  std::string jobName;

  const bool secondJob = g_session.IsSecondJob(costumeJob);
  const int spriteJob = g_session.GetSpriteJob(job, costumeJob, true);
  sexName = static_cast<const std::string&>(g_session.GetSexName(sex));
  jobName = static_cast<const std::string&>(g_session.GetBodyJobName(spriteJob));

  if (secondJob == true) {
    result = static_cast<const std::string&>(this->GetBodyRacePrefix(spriteJob + 3950)) +
             static_cast<const std::string&>(FormatOwnedString("\\몸통\\%s\\costume_%d\\%s_%s_%d.%s", sexName.c_str(), 1, jobName.c_str(), sexName.c_str(), 1, g_equipmentResourceExtensions[type]));
  } else {
    result = static_cast<const std::string&>(this->GetBodyRacePrefix(spriteJob + 3950)) +
             static_cast<const std::string&>(FormatOwnedString("\\몸통\\%s\\%s_%s.%s", sexName.c_str(), jobName.c_str(), sexName.c_str(), g_equipmentResourceExtensions[type]));
  }

  if (costume == 2) {
    std::size_t found = result.rfind("마도기어_");
    if (found != std::string::npos) {
      result.replace(found, 9, "마도아머_", 9);
    }

    found = result.rfind("MEISTER_MADOGEAR1");
    if (found != std::string::npos) {
      result.replace(found, 17, "MEISTER_MADOGEAR2", 17);
    }
  }

  return result;
}

// OFFSET: 0x00BABEF0
// FUNCTION: fn_19754
std::string EquipmentResourceMgr::GetBodyRacePrefix(int job) {
  if (g_session.IsDoramJob(job) == true) {
    return "도람족";
  }
  return "인간족";
}

// OFFSET: 0x00DCA170
// FUNCTION: fn_23035
EncryptedName<14, 0x072D2D872D259D5BULL>& WizardBodyPath1() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x072D2D872D259D5BULL);
}

// OFFSET: 0x00DCA240
// FUNCTION: fn_23036
EncryptedName<18, 0x9BE3EF737387AD79ULL>& PriestBodyPath1() {
  OBFUSCATE_STRING_BODY("프리스트\\프리스트", 0x9BE3EF737387AD79ULL);
}

// OFFSET: 0x00DCA310
// FUNCTION: fn_23037
EncryptedName<14, 0xB96B77B7255F8569ULL>& AssassinBodyPath1() {
  OBFUSCATE_STRING_BODY("어세신\\어세신", 0xB96B77B7255F8569ULL);
}

// OFFSET: 0x00DCA3E0
// FUNCTION: fn_23038
EncryptedName<14, 0x11CB35636377DD07ULL>& SageBodyPath1() {
  OBFUSCATE_STRING_BODY("세이지\\세이지", 0x11CB35636377DD07ULL);
}

// OFFSET: 0x00DCA4B0
// FUNCTION: fn_23039
EncryptedName<10, 0x4755C1355575FD37ULL>& BardBodyPath1() {
  OBFUSCATE_STRING_BODY("바드\\바드", 0x4755C1355575FD37ULL);
}

// OFFSET: 0x00DCA580
// FUNCTION: fn_23040
EncryptedName<10, 0x9D8F2D37BFFD61B7ULL>& DancerBodyPath1() {
  OBFUSCATE_STRING_BODY("무희\\무희", 0x9D8F2D37BFFD61B7ULL);
}

// OFFSET: 0x00DCA650
// FUNCTION: fn_23041
EncryptedName<10, 0x79DF094DD9616575ULL>& MonkBodyPath1() {
  OBFUSCATE_STRING_BODY("몽크\\몽크", 0x79DF094DD9616575ULL);
}

// OFFSET: 0x00DCA720
// FUNCTION: fn_23042
EncryptedName<18, 0x038535BB092533B7ULL>& AlchemistBodyPath1() {
  OBFUSCATE_STRING_BODY("연금술사\\연금술사", 0x038535BB092533B7ULL);
}

// OFFSET: 0x00DCA7F0
// FUNCTION: fn_23043
EncryptedName<10, 0xFB2BC75769E777D3ULL>& RogueBodyPath1() {
  OBFUSCATE_STRING_BODY("로그\\로그", 0xFB2BC75769E777D3ULL);
}

// OFFSET: 0x00DCA8C0
// FUNCTION: fn_23044
EncryptedName<14, 0xB5D7B5AF4FEDFF57ULL>& NoviceBodyPath1() {
  OBFUSCATE_STRING_BODY("초보자\\초보자", 0xB5D7B5AF4FEDFF57ULL);
}

// OFFSET: 0x00DCA990
// FUNCTION: fn_23045
EncryptedName<10, 0x9B9D51AD3515F779ULL>& SwordmanBodyPath1() {
  OBFUSCATE_STRING_BODY("검사\\검사", 0x9B9D51AD3515F779ULL);
}

// OFFSET: 0x00DCAA60
// FUNCTION: fn_23046
EncryptedName<14, 0xB5F3F779237315F3ULL>& MagicianBodyPath1() {
  OBFUSCATE_STRING_BODY("마법사\\마법사", 0xB5F3F779237315F3ULL);
}

// OFFSET: 0x00DCAB30
// FUNCTION: fn_23047
EncryptedName<10, 0x67DD5BAB1919D9D1ULL>& ArcherBodyPath1() {
  OBFUSCATE_STRING_BODY("궁수\\궁수", 0x67DD5BAB1919D9D1ULL);
}

// OFFSET: 0x00DCAC00
// FUNCTION: fn_23048
EncryptedName<14, 0x91493751EF33B5FDULL>& AcolyteBodyPath1() {
  OBFUSCATE_STRING_BODY("성직자\\성직자", 0x91493751EF33B5FDULL);
}

// OFFSET: 0x00DCACD0
// FUNCTION: fn_23049
EncryptedName<10, 0x6BC977B3B5993503ULL>& MerchantBodyPath1() {
  OBFUSCATE_STRING_BODY("상인\\상인", 0x6BC977B3B5993503ULL);
}

// OFFSET: 0x00DCADA0
// FUNCTION: fn_23050
EncryptedName<10, 0xB7A76BF9CD41F99FULL>& ThiefBodyPath1() {
  OBFUSCATE_STRING_BODY("도둑\\도둑", 0xB7A76BF9CD41F99FULL);
}

// OFFSET: 0x00DCAE70
// FUNCTION: fn_23051
EncryptedName<10, 0x732B390D63FD47D7ULL>& ThiefBodyPath2() {
  OBFUSCATE_STRING_BODY("도둑\\도둑", 0x732B390D63FD47D7ULL);
}

// OFFSET: 0x00DCAF40
// FUNCTION: fn_23052
EncryptedName<14, 0x1FDB81175D1557E3ULL>& NoviceBodyPath2() {
  OBFUSCATE_STRING_BODY("초보자\\초보자", 0x1FDB81175D1557E3ULL);
}

// OFFSET: 0x00DCB010
// FUNCTION: fn_23053
EncryptedName<10, 0xE3FB23537D2785BFULL>& SwordmanBodyPath2() {
  OBFUSCATE_STRING_BODY("검사\\검사", 0xE3FB23537D2785BFULL);
}

// OFFSET: 0x00DCB0E0
// FUNCTION: fn_23054
EncryptedName<14, 0x8BBF792B47E72173ULL>& MagicianBodyPath2() {
  OBFUSCATE_STRING_BODY("마법사\\마법사", 0x8BBF792B47E72173ULL);
}

// OFFSET: 0x00DCB1B0
// FUNCTION: fn_23055
EncryptedName<10, 0xB3CF9BEB7963DB89ULL>& ArcherBodyPath2() {
  OBFUSCATE_STRING_BODY("궁수\\궁수", 0xB3CF9BEB7963DB89ULL);
}

// OFFSET: 0x00DCB280
// FUNCTION: fn_23056
EncryptedName<14, 0x71A509DB43792991ULL>& AcolyteBodyPath2() {
  OBFUSCATE_STRING_BODY("성직자\\성직자", 0x71A509DB43792991ULL);
}

// OFFSET: 0x00DCB350
// FUNCTION: fn_23057
EncryptedName<10, 0xDD69EDFFA18F537DULL>& MerchantBodyPath2() {
  OBFUSCATE_STRING_BODY("상인\\상인", 0xDD69EDFFA18F537DULL);
}

// OFFSET: 0x00DCB420
// FUNCTION: fn_23058
EncryptedName<10, 0xFFBF43D1FFD5FF49ULL>& ThiefBodyPath3() {
  OBFUSCATE_STRING_BODY("도둑\\도둑", 0xFFBF43D1FFD5FF49ULL);
}

// OFFSET: 0x00DCB4F0
// FUNCTION: fn_23059
EncryptedName<10, 0xEB3D4FAB0FC183DBULL>& KnightBodyPath1() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0xEB3D4FAB0FC183DBULL);
}

// OFFSET: 0x00DCB5C0
// FUNCTION: fn_23060
EncryptedName<18, 0x8F3F2FA959977F9FULL>& PriestBodyPath2() {
  OBFUSCATE_STRING_BODY("프리스트\\프리스트", 0x8F3F2FA959977F9FULL);
}

// OFFSET: 0x00DCB690
// FUNCTION: fn_23061
EncryptedName<14, 0xE51B29334D353B81ULL>& WizardBodyPath2() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0xE51B29334D353B81ULL);
}

// OFFSET: 0x00DCB760
// FUNCTION: fn_23062
EncryptedName<14, 0x8B3FA35B5BDDDF89ULL>& BlacksmithBodyPath1() {
  OBFUSCATE_STRING_BODY("제철공\\제철공", 0x8B3FA35B5BDDDF89ULL);
}

// OFFSET: 0x00DCB830
// FUNCTION: fn_23063
EncryptedName<10, 0x7FE563BD3FB7F925ULL>& HunterBodyPath1() {
  OBFUSCATE_STRING_BODY("헌터\\헌터", 0x7FE563BD3FB7F925ULL);
}

// OFFSET: 0x00DCB900
// FUNCTION: fn_23064
EncryptedName<14, 0x5D6B3B1DEB2397B1ULL>& AssassinBodyPath2() {
  OBFUSCATE_STRING_BODY("어세신\\어세신", 0x5D6B3B1DEB2397B1ULL);
}

// OFFSET: 0x00DCB9D0
// FUNCTION: fn_23065
EncryptedName<28, 0x7DB1050779034DE7ULL>& PecoKnightBodyPath1() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0x7DB1050779034DE7ULL);
}

// OFFSET: 0x00DCBAB0
// FUNCTION: fn_23066
EncryptedName<22, 0x01E7898D9D235DDFULL>& CrusaderBodyPath1() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0x01E7898D9D235DDFULL);
}

// OFFSET: 0x00DCBB80
// FUNCTION: fn_23067
EncryptedName<10, 0x0DF5B33D9F21EB79ULL>& MonkBodyPath2() {
  OBFUSCATE_STRING_BODY("몽크\\몽크", 0x0DF5B33D9F21EB79ULL);
}

// OFFSET: 0x00DCBC50
// FUNCTION: fn_23068
EncryptedName<14, 0x9DEFC105A1EB2F71ULL>& SageBodyPath2() {
  OBFUSCATE_STRING_BODY("세이지\\세이지", 0x9DEFC105A1EB2F71ULL);
}

// OFFSET: 0x00DCBD20
// FUNCTION: fn_23069
EncryptedName<10, 0x6961FF056999A303ULL>& RogueBodyPath2() {
  OBFUSCATE_STRING_BODY("로그\\로그", 0x6961FF056999A303ULL);
}

// OFFSET: 0x00DCBDF0
// FUNCTION: fn_23070
EncryptedName<18, 0xBD3F430159B551FDULL>& AlchemistBodyPath2() {
  OBFUSCATE_STRING_BODY("연금술사\\연금술사", 0xBD3F430159B551FDULL);
}

// OFFSET: 0x00DCBEC0
// FUNCTION: fn_23071
EncryptedName<10, 0xC7FB6DF18577D34BULL>& BardBodyPath2() {
  OBFUSCATE_STRING_BODY("바드\\바드", 0xC7FB6DF18577D34BULL);
}

// OFFSET: 0x00DCBF90
// FUNCTION: fn_23072
EncryptedName<10, 0x5B832361BDA77197ULL>& DancerBodyPath2() {
  OBFUSCATE_STRING_BODY("무희\\무희", 0x5B832361BDA77197ULL);
}

// OFFSET: 0x00DCC060
// FUNCTION: fn_23073
EncryptedName<34, 0x2B477F9B5B39B301ULL>& NewPecoCrusaderBodyPath1() {
  OBFUSCATE_STRING_BODY("신페코크루세이더\\신페코크루세이더", 0x2B477F9B5B39B301ULL);
}

// OFFSET: 0x00DCC140
// FUNCTION: fn_23074
EncryptedName<14, 0x2B8725D7E95325F5ULL>& NoviceBodyPath3() {
  OBFUSCATE_STRING_BODY("초보자\\초보자", 0x2B8725D7E95325F5ULL);
}

// OFFSET: 0x00DCC210
// FUNCTION: fn_23075
EncryptedName<10, 0x899D11BB83695B0DULL>& SwordmanBodyPath3() {
  OBFUSCATE_STRING_BODY("검사\\검사", 0x899D11BB83695B0DULL);
}

// OFFSET: 0x00DCC2E0
// FUNCTION: fn_23076
EncryptedName<14, 0xBDADDD87C5A50B6BULL>& MagicianBodyPath3() {
  OBFUSCATE_STRING_BODY("마법사\\마법사", 0xBDADDD87C5A50B6BULL);
}

// OFFSET: 0x00DCC3B0
// FUNCTION: fn_23077
EncryptedName<10, 0x7989830DE73B9DBDULL>& ArcherBodyPath3() {
  OBFUSCATE_STRING_BODY("궁수\\궁수", 0x7989830DE73B9DBDULL);
}

// OFFSET: 0x00DCC550
// FUNCTION: fn_23078
EncryptedName<10, 0xA7471985F93F9BDFULL>& MerchantBodyPath3() {
  OBFUSCATE_STRING_BODY("상인\\상인", 0xA7471985F93F9BDFULL);
}

// OFFSET: 0x00DCC620
// FUNCTION: fn_23079
EncryptedName<10, 0xF535E561137F138DULL>& ThiefBodyPath4() {
  OBFUSCATE_STRING_BODY("도둑\\도둑", 0xF535E561137F138DULL);
}

// OFFSET: 0x00DCC6F0
// FUNCTION: fn_23080
EncryptedName<10, 0xD3E5516F89F161D1ULL>& KnightBodyPath2() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0xD3E5516F89F161D1ULL);
}

// OFFSET: 0x00DCC7C0
// FUNCTION: fn_23081
EncryptedName<18, 0x45E3F103CBB5F157ULL>& PriestBodyPath3() {
  OBFUSCATE_STRING_BODY("프리스트\\프리스트", 0x45E3F103CBB5F157ULL);
}

// OFFSET: 0x00DCC890
// FUNCTION: fn_23082
EncryptedName<14, 0x07112F85A39F4FABULL>& WizardBodyPath3() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x07112F85A39F4FABULL);
}

// OFFSET: 0x00DCC960
// FUNCTION: fn_23083
EncryptedName<14, 0x2D33791F67AD47DFULL>& BlacksmithBodyPath2() {
  OBFUSCATE_STRING_BODY("제철공\\제철공", 0x2D33791F67AD47DFULL);
}

// OFFSET: 0x00DCCA30
// FUNCTION: fn_23084
EncryptedName<10, 0x936F3D2FFDFD87F5ULL>& HunterBodyPath2() {
  OBFUSCATE_STRING_BODY("헌터\\헌터", 0x936F3D2FFDFD87F5ULL);
}

// OFFSET: 0x00DCCB00
// FUNCTION: fn_23085
EncryptedName<14, 0x2F2701A7E56D3D89ULL>& AssassinBodyPath3() {
  OBFUSCATE_STRING_BODY("어세신\\어세신", 0x2F2701A7E56D3D89ULL);
}

// OFFSET: 0x00DCCBD0
// FUNCTION: fn_23086
EncryptedName<28, 0x6B3301E1A3079125ULL>& PecoKnightBodyPath2() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0x6B3301E1A3079125ULL);
}

// OFFSET: 0x00DCCCB0
// FUNCTION: fn_23087
EncryptedName<22, 0x71B7691737C5A1AFULL>& CrusaderBodyPath2() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0x71B7691737C5A1AFULL);
}

// OFFSET: 0x00DCCD80
// FUNCTION: fn_23088
EncryptedName<10, 0xC39FB559A17D3963ULL>& MonkBodyPath3() {
  OBFUSCATE_STRING_BODY("몽크\\몽크", 0xC39FB559A17D3963ULL);
}

// OFFSET: 0x00DCCE50
// FUNCTION: fn_23089
EncryptedName<14, 0xFF7D83CB791339D9ULL>& SageBodyPath3() {
  OBFUSCATE_STRING_BODY("세이지\\세이지", 0xFF7D83CB791339D9ULL);
}

// OFFSET: 0x00DCCF20
// FUNCTION: fn_23090
EncryptedName<10, 0xED5143E7C1B39315ULL>& RogueBodyPath3() {
  OBFUSCATE_STRING_BODY("로그\\로그", 0xED5143E7C1B39315ULL);
}

// OFFSET: 0x00DCCFF0
// FUNCTION: fn_23091
EncryptedName<18, 0x393D916561BDC391ULL>& AlchemistBodyPath3() {
  OBFUSCATE_STRING_BODY("연금술사\\연금술사", 0x393D916561BDC391ULL);
}

// OFFSET: 0x00DCD0C0
// FUNCTION: fn_23092
EncryptedName<10, 0x31956551B311FD95ULL>& BardBodyPath3() {
  OBFUSCATE_STRING_BODY("바드\\바드", 0x31956551B311FD95ULL);
}

// OFFSET: 0x00DCD190
// FUNCTION: fn_23093
EncryptedName<10, 0x8741152D8FB34B4FULL>& DancerBodyPath3() {
  OBFUSCATE_STRING_BODY("무희\\무희", 0x8741152D8FB34B4FULL);
}

// OFFSET: 0x00DCD260
// FUNCTION: fn_23094
EncryptedName<34, 0x955D4DF94DE9BB07ULL>& OldPecoCrusaderBodyPath1() {
  OBFUSCATE_STRING_BODY("구페코크루세이더\\구페코크루세이더", 0x955D4DF94DE9BB07ULL);
}

// OFFSET: 0x00DCD340
// FUNCTION: fn_23095
EncryptedName<14, 0xC399B7091589B50BULL>& NoviceBodyPath4() {
  OBFUSCATE_STRING_BODY("초보자\\초보자", 0xC399B7091589B50BULL);
}

// OFFSET: 0x00DCD410
// FUNCTION: fn_23096
EncryptedName<18, 0xB739073FE921AD6FULL>& TaekwonBodyPath1() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0xB739073FE921AD6FULL);
}

// OFFSET: 0x00DCD4E0
// FUNCTION: fn_23097
EncryptedName<18, 0x17FFC98B398FAF61ULL>& TaekwonBodyPath2() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0x17FFC98B398FAF61ULL);
}

// OFFSET: 0x00DCD5B0
// FUNCTION: fn_23098
EncryptedName<18, 0x9B5B959B954D9333ULL>& TaekwonBodyPath3() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0x9B5B959B954D9333ULL);
}

// OFFSET: 0x00DCD680
// FUNCTION: fn_23099
EncryptedName<14, 0xAFA5617F178B6185ULL>& WizardBodyPath4() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0xAFA5617F178B6185ULL);
}

// OFFSET: 0x00DCD750
// FUNCTION: fn_23100
EncryptedName<14, 0x078143534999254DULL>& AcolyteBodyPath3() {
  OBFUSCATE_STRING_BODY("성직자\\성직자", 0x078143534999254DULL);
}

// OFFSET: 0x00DCD820
// FUNCTION: fn_23101
EncryptedName<10, 0x3F5315DFE32B4FAFULL>& KnightBodyPath3() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0x3F5315DFE32B4FAFULL);
}

// OFFSET: 0x00DCD8F0
// FUNCTION: fn_23102
EncryptedName<14, 0xE50B2935073DD3E9ULL>& SageBodyPath4() {
  OBFUSCATE_STRING_BODY("세이지\\세이지", 0xE50B2935073DD3E9ULL);
}

// OFFSET: 0x00DCD9C0
// FUNCTION: fn_23103
EncryptedName<10, 0x7D750DADBDE9C3F1ULL>& KnightBodyPath4() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0x7D750DADBDE9C3F1ULL);
}

// OFFSET: 0x00DCDA90
// FUNCTION: fn_23104
EncryptedName<10, 0x33832D4D3FDFBDBFULL>& KnightBodyPath5() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0x33832D4D3FDFBDBFULL);
}

// OFFSET: 0x00DCDB60
// FUNCTION: fn_23105
EncryptedName<28, 0x1BF915E1F37DFDFDULL>& PecoKnightBodyPath3() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0x1BF915E1F37DFDFDULL);
}

// OFFSET: 0x00DCDC40
// FUNCTION: fn_23106
EncryptedName<28, 0x01FF9313695DD9DFULL>& PecoKnightBodyPath4() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0x01FF9313695DD9DFULL);
}

// OFFSET: 0x00DCDD20
// FUNCTION: fn_23107
EncryptedName<28, 0x6DED3DD1CF75AD9BULL>& PecoKnightBodyPath5() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0x6DED3DD1CF75AD9BULL);
}

// OFFSET: 0x00DCDE00
// FUNCTION: fn_23108
EncryptedName<28, 0x49C3759B77DBD3C1ULL>& PecoKnightBodyPath6() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0x49C3759B77DBD3C1ULL);
}

// OFFSET: 0x00DCDEE0
// FUNCTION: fn_23109
EncryptedName<28, 0xD1538513DF975701ULL>& PecoKnightBodyPath7() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0xD1538513DF975701ULL);
}

// OFFSET: 0x00DCDFC0
// FUNCTION: fn_23110
EncryptedName<28, 0x91E147EBBD47C39FULL>& PecoKnightBodyPath8() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0x91E147EBBD47C39FULL);
}

// OFFSET: 0x00DCE0A0
// FUNCTION: fn_23111
EncryptedName<28, 0x4F1791FB1503C713ULL>& PecoKnightBodyPath9() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0x4F1791FB1503C713ULL);
}

// OFFSET: 0x00DCE180
// FUNCTION: fn_23112
EncryptedName<28, 0xA1A55B4D1597EDD3ULL>& PecoKnightBodyPath10() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0xA1A55B4D1597EDD3ULL);
}

// OFFSET: 0x00DCE260
// FUNCTION: fn_23113
EncryptedName<28, 0x0753A7E1EFA9E139ULL>& PecoKnightBodyPath11() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0x0753A7E1EFA9E139ULL);
}

// OFFSET: 0x00DCE340
// FUNCTION: fn_23114
EncryptedName<28, 0x7FC1015DFFCF6D19ULL>& PecoKnightBodyPath12() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0x7FC1015DFFCF6D19ULL);
}

// OFFSET: 0x00DCE420
// FUNCTION: fn_23115
EncryptedName<28, 0xCB3FBD4183DB8323ULL>& PecoKnightBodyPath13() {
  OBFUSCATE_STRING_BODY("페코페코_기사\\페코페코_기사", 0xCB3FBD4183DB8323ULL);
}

// OFFSET: 0x00DCE500
// FUNCTION: fn_23116
EncryptedName<18, 0x7FCD77F9C7CBD14DULL>& PriestBodyPath4() {
  OBFUSCATE_STRING_BODY("프리스트\\프리스트", 0x7FCD77F9C7CBD14DULL);
}

// OFFSET: 0x00DCE5D0
// FUNCTION: fn_23117
EncryptedName<18, 0x5B8F49AF09390D1BULL>& PriestBodyPath5() {
  OBFUSCATE_STRING_BODY("프리스트\\프리스트", 0x5B8F49AF09390D1BULL);
}

// OFFSET: 0x00DCE6A0
// FUNCTION: fn_23118
EncryptedName<14, 0x3B915119EFD523EFULL>& WizardBodyPath5() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x3B915119EFD523EFULL);
}

// OFFSET: 0x00DCE770
// FUNCTION: fn_23119
EncryptedName<14, 0x474DC581D5E98357ULL>& WizardBodyPath6() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x474DC581D5E98357ULL);
}

// OFFSET: 0x00DCE840
// FUNCTION: fn_23120
EncryptedName<14, 0x3F557BE333F95F6DULL>& BlacksmithBodyPath3() {
  OBFUSCATE_STRING_BODY("제철공\\제철공", 0x3F557BE333F95F6DULL);
}

// OFFSET: 0x00DCE910
// FUNCTION: fn_23121
EncryptedName<14, 0x8B3FEB5B8BD5C3C3ULL>& BlacksmithBodyPath4() {
  OBFUSCATE_STRING_BODY("제철공\\제철공", 0x8B3FEB5B8BD5C3C3ULL);
}

// OFFSET: 0x00DCE9E0
// FUNCTION: fn_23122
EncryptedName<18, 0x0BEB63A5C905B3A7ULL>& MadogearBodyPath1() {
  OBFUSCATE_STRING_BODY("마도기어\\마도기어", 0x0BEB63A5C905B3A7ULL);
}

// OFFSET: 0x00DCEAB0
// FUNCTION: fn_23123
EncryptedName<18, 0x078333BB9725B7C9ULL>& MadogearBodyPath2() {
  OBFUSCATE_STRING_BODY("마도기어\\마도기어", 0x078333BB9725B7C9ULL);
}

// OFFSET: 0x00DCEB80
// FUNCTION: fn_23124
EncryptedName<18, 0x8F1B5BFD5F6175B5ULL>& MadogearBodyPath3() {
  OBFUSCATE_STRING_BODY("마도기어\\마도기어", 0x8F1B5BFD5F6175B5ULL);
}

// OFFSET: 0x00DCEC50
// FUNCTION: fn_23125
EncryptedName<10, 0x69FD1D9733D3CFC1ULL>& HunterBodyPath3() {
  OBFUSCATE_STRING_BODY("헌터\\헌터", 0x69FD1D9733D3CFC1ULL);
}

// OFFSET: 0x00DCED20
// FUNCTION: fn_23126
EncryptedName<10, 0xF5657D1F3DE5CDC7ULL>& HunterBodyPath4() {
  OBFUSCATE_STRING_BODY("헌터\\헌터", 0xF5657D1F3DE5CDC7ULL);
}

// OFFSET: 0x00DCEDF0
// FUNCTION: fn_23127
EncryptedName<22, 0x8B65CD61012B5931ULL>& RangerWolfBodyPath1() {
  OBFUSCATE_STRING_BODY("레인져늑대\\레인져늑대", 0x8B65CD61012B5931ULL);
}

// OFFSET: 0x00DCEEC0
// FUNCTION: fn_23128
EncryptedName<22, 0xF7530137154D8FE5ULL>& RangerWolfBodyPath2() {
  OBFUSCATE_STRING_BODY("레인져늑대\\레인져늑대", 0xF7530137154D8FE5ULL);
}

// OFFSET: 0x00DCEF90
// FUNCTION: fn_23129
EncryptedName<22, 0x61FF87ED7BF39BE1ULL>& RangerWolfBodyPath3() {
  OBFUSCATE_STRING_BODY("레인져늑대\\레인져늑대", 0x61FF87ED7BF39BE1ULL);
}

// OFFSET: 0x00DCF060
// FUNCTION: fn_23130
EncryptedName<14, 0x7D993F650D2785FFULL>& AssassinBodyPath4() {
  OBFUSCATE_STRING_BODY("어세신\\어세신", 0x7D993F650D2785FFULL);
}

// OFFSET: 0x00DCF130
// FUNCTION: fn_23131
EncryptedName<14, 0x2D394DBD0F332733ULL>& AssassinBodyPath5() {
  OBFUSCATE_STRING_BODY("어세신\\어세신", 0x2D394DBD0F332733ULL);
}

// OFFSET: 0x00DCF200
// FUNCTION: fn_23132
EncryptedName<22, 0x151F77EBEBBB2147ULL>& CrusaderBodyPath3() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0x151F77EBEBBB2147ULL);
}

// OFFSET: 0x00DCF2D0
// FUNCTION: fn_23133
EncryptedName<22, 0xC7912587FFF31B71ULL>& CrusaderBodyPath4() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0xC7912587FFF31B71ULL);
}

// OFFSET: 0x00DCF3A0
// FUNCTION: fn_23134
EncryptedName<34, 0x69D345DB950F336DULL>& NewPecoCrusaderBodyPath2() {
  OBFUSCATE_STRING_BODY("신페코크루세이더\\신페코크루세이더", 0x69D345DB950F336DULL);
}

// OFFSET: 0x00DCF480
// FUNCTION: fn_23135
EncryptedName<34, 0x9B17C9EFDBB39F2FULL>& NewPecoCrusaderBodyPath3() {
  OBFUSCATE_STRING_BODY("신페코크루세이더\\신페코크루세이더", 0x9B17C9EFDBB39F2FULL);
}

// OFFSET: 0x00DCF560
// FUNCTION: fn_23136
EncryptedName<34, 0x2D3B15774149D5DDULL>& NewPecoCrusaderBodyPath4() {
  OBFUSCATE_STRING_BODY("신페코크루세이더\\신페코크루세이더", 0x2D3B15774149D5DDULL);
}

// OFFSET: 0x00DCF640
// FUNCTION: fn_23137
EncryptedName<10, 0xD98F03A52D35636DULL>& MonkBodyPath4() {
  OBFUSCATE_STRING_BODY("몽크\\몽크", 0xD98F03A52D35636DULL);
}

// OFFSET: 0x00DCF710
// FUNCTION: fn_23138
EncryptedName<10, 0x0F0F8B6B8F9D19D1ULL>& MonkBodyPath5() {
  OBFUSCATE_STRING_BODY("몽크\\몽크", 0x0F0F8B6B8F9D19D1ULL);
}

// OFFSET: 0x00DCF7E0
// FUNCTION: fn_23139
EncryptedName<14, 0xD3755DD95F47A909ULL>& SageBodyPath5() {
  OBFUSCATE_STRING_BODY("세이지\\세이지", 0xD3755DD95F47A909ULL);
}

// OFFSET: 0x00DCF8B0
// FUNCTION: fn_23140
EncryptedName<14, 0x31D7C1EDA345E36DULL>& SageBodyPath6() {
  OBFUSCATE_STRING_BODY("세이지\\세이지", 0x31D7C1EDA345E36DULL);
}

// OFFSET: 0x00DCF980
// FUNCTION: fn_23141
EncryptedName<10, 0x3FAF85D995034FD7ULL>& RogueBodyPath4() {
  OBFUSCATE_STRING_BODY("로그\\로그", 0x3FAF85D995034FD7ULL);
}

// OFFSET: 0x00DCFA50
// FUNCTION: fn_23142
EncryptedName<10, 0x490FFB43A12F0DB9ULL>& RogueBodyPath5() {
  OBFUSCATE_STRING_BODY("로그\\로그", 0x490FFB43A12F0DB9ULL);
}

// OFFSET: 0x00DCFB20
// FUNCTION: fn_23143
EncryptedName<18, 0x91651D81F559F971ULL>& AlchemistBodyPath4() {
  OBFUSCATE_STRING_BODY("연금술사\\연금술사", 0x91651D81F559F971ULL);
}

// OFFSET: 0x00DCFBF0
// FUNCTION: fn_23144
EncryptedName<18, 0x73D1674B61DF7B0BULL>& AlchemistBodyPath5() {
  OBFUSCATE_STRING_BODY("연금술사\\연금술사", 0x73D1674B61DF7B0BULL);
}

// OFFSET: 0x00DCFCC0
// FUNCTION: fn_23145
EncryptedName<10, 0x2FF3C5036D070127ULL>& DancerBodyPath4() {
  OBFUSCATE_STRING_BODY("무희\\무희", 0x2FF3C5036D070127ULL);
}

// OFFSET: 0x00DCFD90
// FUNCTION: fn_23146
EncryptedName<10, 0x17359BF953C367B3ULL>& DancerBodyPath5() {
  OBFUSCATE_STRING_BODY("무희\\무희", 0x17359BF953C367B3ULL);
}

// OFFSET: 0x00DCFE60
// FUNCTION: fn_23147
EncryptedName<10, 0xC52FB5FBB94FF917ULL>& BardBodyPath4() {
  OBFUSCATE_STRING_BODY("바드\\바드", 0xC52FB5FBB94FF917ULL);
}

// OFFSET: 0x00DCFF30
// FUNCTION: fn_23148
EncryptedName<10, 0x93E7A3A1F587CDC5ULL>& BardBodyPath5() {
  OBFUSCATE_STRING_BODY("바드\\바드", 0x93E7A3A1F587CDC5ULL);
}

// OFFSET: 0x00DD0000
// FUNCTION: fn_23149
EncryptedName<10, 0x59278F63D79F3DA1ULL>& KnightBodyPath6() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0x59278F63D79F3DA1ULL);
}

// OFFSET: 0x00DD00D0
// FUNCTION: fn_23150
EncryptedName<14, 0x812BE72BCD5707B7ULL>& WizardBodyPath7() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x812BE72BCD5707B7ULL);
}

// OFFSET: 0x00DD01A0
// FUNCTION: fn_23151
EncryptedName<10, 0x3375B1C7DBB5FD55ULL>& HunterBodyPath5() {
  OBFUSCATE_STRING_BODY("헌터\\헌터", 0x3375B1C7DBB5FD55ULL);
}

// OFFSET: 0x00DD0270
// FUNCTION: fn_23152
EncryptedName<18, 0x1D6D75EDE7CD311FULL>& PriestBodyPath6() {
  OBFUSCATE_STRING_BODY("프리스트\\프리스트", 0x1D6D75EDE7CD311FULL);
}

// OFFSET: 0x00DD0340
// FUNCTION: fn_23153
EncryptedName<14, 0xCBD1F3239BBBEDF7ULL>& BlacksmithBodyPath5() {
  OBFUSCATE_STRING_BODY("제철공\\제철공", 0xCBD1F3239BBBEDF7ULL);
}

// OFFSET: 0x00DD0410
// FUNCTION: fn_23154
EncryptedName<14, 0x7DBBD74FA36F3BCBULL>& AssassinBodyPath6() {
  OBFUSCATE_STRING_BODY("어세신\\어세신", 0x7DBBD74FA36F3BCBULL);
}

// OFFSET: 0x00DD04E0
// FUNCTION: fn_23155
EncryptedName<22, 0x4917EB6D71B701ABULL>& CrusaderBodyPath5() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0x4917EB6D71B701ABULL);
}

// OFFSET: 0x00DD05B0
// FUNCTION: fn_23156
EncryptedName<14, 0xFD4F85BBA7E37D4FULL>& SageBodyPath7() {
  OBFUSCATE_STRING_BODY("세이지\\세이지", 0xFD4F85BBA7E37D4FULL);
}

// OFFSET: 0x00DD0680
// FUNCTION: fn_23157
EncryptedName<10, 0x51C57BB9F1DD9B0FULL>& BardBodyPath6() {
  OBFUSCATE_STRING_BODY("바드\\바드", 0x51C57BB9F1DD9B0FULL);
}

// OFFSET: 0x00DD0750
// FUNCTION: fn_23158
EncryptedName<10, 0xC733B7A333F9F571ULL>& DancerBodyPath6() {
  OBFUSCATE_STRING_BODY("무희\\무희", 0xC733B7A333F9F571ULL);
}

// OFFSET: 0x00DD0820
// FUNCTION: fn_23159
EncryptedName<10, 0x7D61612937C1592DULL>& MonkBodyPath6() {
  OBFUSCATE_STRING_BODY("몽크\\몽크", 0x7D61612937C1592DULL);
}

// OFFSET: 0x00DD08F0
// FUNCTION: fn_23160
EncryptedName<18, 0x8F4B518D839D1581ULL>& AlchemistBodyPath6() {
  OBFUSCATE_STRING_BODY("연금술사\\연금술사", 0x8F4B518D839D1581ULL);
}

// OFFSET: 0x00DD09C0
// FUNCTION: fn_23161
EncryptedName<10, 0xCBD7DDAFDF87F549ULL>& RogueBodyPath6() {
  OBFUSCATE_STRING_BODY("로그\\로그", 0xCBD7DDAFDF87F549ULL);
}

// OFFSET: 0x00DD0A90
// FUNCTION: fn_23162
EncryptedName<14, 0x39E7F13755252F8FULL>& NoviceBodyPath5() {
  OBFUSCATE_STRING_BODY("초보자\\초보자", 0x39E7F13755252F8FULL);
}

// OFFSET: 0x00DD0B60
// FUNCTION: fn_23163
EncryptedName<14, 0x3D075BD5256F9B6FULL>& NoviceBodyPath6() {
  OBFUSCATE_STRING_BODY("초보자\\초보자", 0x3D075BD5256F9B6FULL);
}

// OFFSET: 0x00DD0C30
// FUNCTION: fn_23164
EncryptedName<14, 0xEB074B7FBDA3BFD5ULL>& NoviceBodyPath7() {
  OBFUSCATE_STRING_BODY("초보자\\초보자", 0xEB074B7FBDA3BFD5ULL);
}

// OFFSET: 0x00DD0D00
// FUNCTION: fn_23165
EncryptedName<14, 0x03014749B127914FULL>& NoviceBodyPath8() {
  OBFUSCATE_STRING_BODY("초보자\\초보자", 0x03014749B127914FULL);
}

// OFFSET: 0x00DD0DD0
// FUNCTION: fn_23166
EncryptedName<18, 0x251761D7370BA5C3ULL>& PriestBodyPath7() {
  OBFUSCATE_STRING_BODY("프리스트\\프리스트", 0x251761D7370BA5C3ULL);
}

// OFFSET: 0x00DD0EA0
// FUNCTION: fn_23167
EncryptedName<14, 0x2B034DCBB1812785ULL>& WizardBodyPath8() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x2B034DCBB1812785ULL);
}

// OFFSET: 0x00DD0F70
// FUNCTION: fn_23168
EncryptedName<14, 0x85FB93CF2F494179ULL>& BlacksmithBodyPath6() {
  OBFUSCATE_STRING_BODY("제철공\\제철공", 0x85FB93CF2F494179ULL);
}

// OFFSET: 0x00DD1040
// FUNCTION: fn_23169
EncryptedName<14, 0x894FEDABAFCF45E3ULL>& BlacksmithBodyPath7() {
  OBFUSCATE_STRING_BODY("제철공\\제철공", 0x894FEDABAFCF45E3ULL);
}

// OFFSET: 0x00DD1110
// FUNCTION: fn_23170
EncryptedName<14, 0x8B03E17DA967E943ULL>& BlacksmithBodyPath8() {
  OBFUSCATE_STRING_BODY("제철공\\제철공", 0x8B03E17DA967E943ULL);
}

// OFFSET: 0x00DD11E0
// FUNCTION: fn_23171
EncryptedName<10, 0x190B8705C141D1CFULL>& HunterBodyPath6() {
  OBFUSCATE_STRING_BODY("헌터\\헌터", 0x190B8705C141D1CFULL);
}

// OFFSET: 0x00DD12B0
// FUNCTION: fn_23172
EncryptedName<10, 0xC98BB32777F94371ULL>& HunterBodyPath7() {
  OBFUSCATE_STRING_BODY("헌터\\헌터", 0xC98BB32777F94371ULL);
}

// OFFSET: 0x00DD1380
// FUNCTION: fn_23173
EncryptedName<10, 0xA5F51F3989DBB99FULL>& KnightBodyPath7() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0xA5F51F3989DBB99FULL);
}

// OFFSET: 0x00DD1450
// FUNCTION: fn_23174
EncryptedName<10, 0xB3D5C99B2B05FFEFULL>& KnightBodyPath8() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0xB3D5C99B2B05FFEFULL);
}

// OFFSET: 0x00DD1520
// FUNCTION: fn_23175
EncryptedName<10, 0xC71BB3CDEB499DD1ULL>& KnightBodyPath9() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0xC71BB3CDEB499DD1ULL);
}

// OFFSET: 0x00DD15F0
// FUNCTION: fn_23176
EncryptedName<22, 0x3939079FF3395587ULL>& CrusaderBodyPath6() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0x3939079FF3395587ULL);
}

// OFFSET: 0x00DD16C0
// FUNCTION: fn_23177
EncryptedName<22, 0xCF391B0F8B9D95DBULL>& CrusaderBodyPath7() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0xCF391B0F8B9D95DBULL);
}

// OFFSET: 0x00DD1790
// FUNCTION: fn_23178
EncryptedName<10, 0x6967633B9D89092BULL>& KnightBodyPath10() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0x6967633B9D89092BULL);
}

// OFFSET: 0x00DD1860
// FUNCTION: fn_23179
EncryptedName<10, 0xAFCD4F69FBDFE157ULL>& KnightBodyPath11() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0xAFCD4F69FBDFE157ULL);
}

// OFFSET: 0x00DD1930
// FUNCTION: fn_23180
EncryptedName<22, 0x85D9CFCF0FBB71ABULL>& CrusaderBodyPath8() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0x85D9CFCF0FBB71ABULL);
}

// OFFSET: 0x00DD1A00
// FUNCTION: fn_23181
EncryptedName<22, 0x795383DD11AF7DA7ULL>& CrusaderBodyPath9() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0x795383DD11AF7DA7ULL);
}

// OFFSET: 0x00DD1AD0
// FUNCTION: fn_23182
EncryptedName<22, 0xD9FF511B2BB921E7ULL>& CrusaderBodyPath10() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0xD9FF511B2BB921E7ULL);
}

// OFFSET: 0x00DD1BA0
// FUNCTION: fn_23183
EncryptedName<16, 0x978B51F50DC5FD25ULL>& KagerouBodyPath1() {
  OBFUSCATE_STRING_BODY("kagerou\\kagerou", 0x978B51F50DC5FD25ULL);
}

// OFFSET: 0x00DD1C60
// FUNCTION: fn_23184
EncryptedName<12, 0xC757D96143298511ULL>& OboroBodyPath1() {
  OBFUSCATE_STRING_BODY("oboro\\oboro", 0xC757D96143298511ULL);
}

// OFFSET: 0x00DD1D30
// FUNCTION: fn_23185
EncryptedName<16, 0xE1F181F923B759E7ULL>& KagerouBodyPath2() {
  OBFUSCATE_STRING_BODY("kagerou\\kagerou", 0xE1F181F923B759E7ULL);
}

// OFFSET: 0x00DD1DF0
// FUNCTION: fn_23186
EncryptedName<12, 0x37D76D8F495113C3ULL>& OboroBodyPath2() {
  OBFUSCATE_STRING_BODY("oboro\\oboro", 0x37D76D8F495113C3ULL);
}

// OFFSET: 0x00DD1EC0
// FUNCTION: fn_23187
EncryptedName<20, 0x7BAD7791F50F5D8DULL>& RebellionBodyPath1() {
  OBFUSCATE_STRING_BODY("rebellion\\rebellion", 0x7BAD7791F50F5D8DULL);
}

// OFFSET: 0x00DD1F90
// FUNCTION: fn_23188
EncryptedName<18, 0x2F1337956DC3CFD5ULL>& SummonerBodyPath1() {
  OBFUSCATE_STRING_BODY("summoner\\summoner", 0x2F1337956DC3CFD5ULL);
}

// OFFSET: 0x00DD2060
// FUNCTION: fn_23189
EncryptedName<23, 0xC91F9D493F75FDA3ULL>& SummonerCartBodyPath1() {
  OBFUSCATE_STRING_BODY("summoner\\cart_summoner", 0xC91F9D493F75FDA3ULL);
}

// OFFSET: 0x00DD2140
// FUNCTION: fn_23190
EncryptedName<18, 0x1F01A13D8D915327ULL>& SummonerBodyPath2() {
  OBFUSCATE_STRING_BODY("summoner\\summoner", 0x1F01A13D8D915327ULL);
}

// OFFSET: 0x00DD2210
// FUNCTION: fn_23191
EncryptedName<23, 0xF33D3D3B0BD9C53DULL>& SummonerCartBodyPath2() {
  OBFUSCATE_STRING_BODY("summoner\\cart_summoner", 0xF33D3D3B0BD9C53DULL);
}

// OFFSET: 0x00DD22F0
// FUNCTION: fn_23192
EncryptedName<10, 0xC74BB3BB110FB10FULL>& NinjaBodyPath1() {
  OBFUSCATE_STRING_BODY("닌자\\닌자", 0xC74BB3BB110FB10FULL);
}

// OFFSET: 0x00DD23C0
// FUNCTION: fn_23193
EncryptedName<10, 0x0B5F6D6B23D5AFA7ULL>& NinjaBodyPath2() {
  OBFUSCATE_STRING_BODY("닌자\\닌자", 0x0B5F6D6B23D5AFA7ULL);
}

// OFFSET: 0x00DD2490
// FUNCTION: fn_23194
EncryptedName<16, 0x2B7BAB0B6971E3E9ULL>& KagerouBodyPath3() {
  OBFUSCATE_STRING_BODY("kagerou\\kagerou", 0x2B7BAB0B6971E3E9ULL);
}

// OFFSET: 0x00DD2550
// FUNCTION: fn_23195
EncryptedName<12, 0x8773593DB181D573ULL>& OboroBodyPath3() {
  OBFUSCATE_STRING_BODY("oboro\\oboro", 0x8773593DB181D573ULL);
}

// OFFSET: 0x00DD2620
// FUNCTION: fn_23196
EncryptedName<16, 0xFBDBDB13A77191BFULL>& KagerouBodyPath4() {
  OBFUSCATE_STRING_BODY("kagerou\\kagerou", 0xFBDBDB13A77191BFULL);
}

// OFFSET: 0x00DD26E0
// FUNCTION: fn_23197
EncryptedName<12, 0x19BBC5910555FB97ULL>& OboroBodyPath4() {
  OBFUSCATE_STRING_BODY("oboro\\oboro", 0x19BBC5910555FB97ULL);
}

// OFFSET: 0x00DD27B0
// FUNCTION: fn_23198
EncryptedName<18, 0xBBD983C7FD611FE3ULL>& TaekwonBodyPath4() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0xBBD983C7FD611FE3ULL);
}

// OFFSET: 0x00DD2880
// FUNCTION: fn_23199
EncryptedName<18, 0xA1ED494BF319772DULL>& TaekwonBodyPath5() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0xA1ED494BF319772DULL);
}

// OFFSET: 0x00DD2950
// FUNCTION: fn_23200
EncryptedName<18, 0x798B394FF5FF434DULL>& TaekwonBodyPath6() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0x798B394FF5FF434DULL);
}

// OFFSET: 0x00DD2A20
// FUNCTION: fn_23201
EncryptedName<18, 0x410DD52D6731BFFDULL>& TaekwonBodyPath7() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0x410DD52D6731BFFDULL);
}

// OFFSET: 0x00DD2AF0
// FUNCTION: fn_23202
EncryptedName<18, 0x07B733370F612D0FULL>& TaekwonBodyPath8() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0x07B733370F612D0FULL);
}

// OFFSET: 0x00DD2BC0
// FUNCTION: fn_23203
EncryptedName<14, 0x113DC179F1C795C9ULL>& WizardBodyPath9() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x113DC179F1C795C9ULL);
}

// OFFSET: 0x00DD2C90
// FUNCTION: fn_23204
EncryptedName<14, 0x31F5C32DD5C53779ULL>& WizardBodyPath10() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x31F5C32DD5C53779ULL);
}

// OFFSET: 0x00DD2D60
// FUNCTION: fn_23205
EncryptedName<10, 0x2F9B717365C1250FULL>& GunslingerBodyPath1() {
  OBFUSCATE_STRING_BODY("건너\\건너", 0x2F9B717365C1250FULL);
}

// OFFSET: 0x00DD2E30
// FUNCTION: fn_23206
EncryptedName<10, 0x4FE54379493FB5D5ULL>& GunslingerBodyPath2() {
  OBFUSCATE_STRING_BODY("건너\\건너", 0x4FE54379493FB5D5ULL);
}

// OFFSET: 0x00DD2F00
// FUNCTION: fn_23207
EncryptedName<20, 0x656FAB972B3D21BBULL>& RebellionBodyPath2() {
  OBFUSCATE_STRING_BODY("rebellion\\rebellion", 0x656FAB972B3D21BBULL);
}

// OFFSET: 0x00DD2FD0
// FUNCTION: fn_23208
EncryptedName<20, 0x33CD2BDD5B3927EDULL>& RebellionBodyPath3() {
  OBFUSCATE_STRING_BODY("rebellion\\rebellion", 0x33CD2BDD5B3927EDULL);
}

// OFFSET: 0x00DD30A0
// FUNCTION: fn_23209
EncryptedName<18, 0xD97BB73901ED99BFULL>& TaekwonBodyPath9() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0xD97BB73901ED99BFULL);
}

// OFFSET: 0x00DD3170
// FUNCTION: fn_23210
EncryptedName<14, 0x5D6B8FCBE3FB97A1ULL>& WizardBodyPath11() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x5D6B8FCBE3FB97A1ULL);
}

// OFFSET: 0x00DD3240
// FUNCTION: fn_23211
EncryptedName<18, 0xE5FDE3151BED831FULL>& TaekwonBodyPath10() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0xE5FDE3151BED831FULL);
}

// OFFSET: 0x00DD3310
// FUNCTION: fn_23212
EncryptedName<14, 0x8971CB79C18D8B27ULL>& WizardBodyPath12() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x8971CB79C18D8B27ULL);
}

// OFFSET: 0x00DD33E0
// FUNCTION: fn_23213
EncryptedName<18, 0x8DC5D10DAB1585BDULL>& TaekwonBodyPath11() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0x8DC5D10DAB1585BDULL);
}

// OFFSET: 0x00DD34B0
// FUNCTION: fn_23214
EncryptedName<18, 0x21156F4141E35DD9ULL>& TaekwonBodyPath12() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0x21156F4141E35DD9ULL);
}

// OFFSET: 0x00DD3580
// FUNCTION: fn_23215
EncryptedName<18, 0x51EB99CBCB075F35ULL>& TaekwonBodyPath13() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0x51EB99CBCB075F35ULL);
}

// OFFSET: 0x00DD3650
// FUNCTION: fn_23216
EncryptedName<14, 0x1B3B756FE399E1D9ULL>& WizardBodyPath13() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x1B3B756FE399E1D9ULL);
}

// OFFSET: 0x00DD3720
// FUNCTION: fn_23217
EncryptedName<18, 0x3B6D9BFD27A7C7F7ULL>& TaekwonBodyPath14() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0x3B6D9BFD27A7C7F7ULL);
}

// OFFSET: 0x00DD37F0
// FUNCTION: fn_23218
EncryptedName<14, 0x451FD72783630709ULL>& WizardBodyPath14() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x451FD72783630709ULL);
}

// OFFSET: 0x00DD38C0
// FUNCTION: fn_23219
EncryptedName<14, 0x73C32959FDFFA51BULL>& WizardBodyPath15() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x73C32959FDFFA51BULL);
}

// OFFSET: 0x00DD3990
// FUNCTION: fn_23220
EncryptedName<14, 0xA15B555BE9CD2B81ULL>& WizardBodyPath16() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0xA15B555BE9CD2B81ULL);
}

// OFFSET: 0x00DD3A60
// FUNCTION: fn_23221
EncryptedName<14, 0xD9115915B1919533ULL>& SageBodyPath8() {
  OBFUSCATE_STRING_BODY("세이지\\세이지", 0xD9115915B1919533ULL);
}

// OFFSET: 0x00DD3B30
// FUNCTION: fn_23222
EncryptedName<14, 0xB3F1A10745C1F105ULL>& SageBodyPath9() {
  OBFUSCATE_STRING_BODY("세이지\\세이지", 0xB3F1A10745C1F105ULL);
}

// OFFSET: 0x00DD3C00
// FUNCTION: fn_23223
EncryptedName<18, 0x99157F159BB99BD9ULL>& WindhawkBodyPath1() {
  OBFUSCATE_STRING_BODY("WINDHAWK\\WINDHAWK", 0x99157F159BB99BD9ULL);
}

// OFFSET: 0x00DD3CD0
// FUNCTION: fn_23224
EncryptedName<23, 0x7959FFBF652D07BFULL>& WolfWindhawkBodyPath1() {
  OBFUSCATE_STRING_BODY("WINDHAWK\\WOLF_WINDHAWK", 0x7959FFBF652D07BFULL);
}

// OFFSET: 0x00DD3DB0
// FUNCTION: fn_23225
EncryptedName<25, 0x01C9FBA7819FFBD7ULL>& WindhawkRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("WINDHAWK\\WINDHAWK_RIDING", 0x01C9FBA7819FFBD7ULL);
}

// OFFSET: 0x00DD3E90
// FUNCTION: fn_23226
EncryptedName<22, 0xABED6971354375C3ULL>& TroubadourBodyPath1() {
  OBFUSCATE_STRING_BODY("TROUBADOUR\\TROUBADOUR", 0xABED6971354375C3ULL);
}

// OFFSET: 0x00DD3F60
// FUNCTION: fn_23227
EncryptedName<29, 0x6BA557130BDFE997ULL>& TroubadourRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("TROUBADOUR\\TROUBADOUR_RIDING", 0x6BA557130BDFE997ULL);
}

// OFFSET: 0x00DD4040
// FUNCTION: fn_23228
EncryptedName<18, 0xFBAD074BDB2B77A1ULL>& TrouvereBodyPath1() {
  OBFUSCATE_STRING_BODY("TROUVERE\\TROUVERE", 0xFBAD074BDB2B77A1ULL);
}

// OFFSET: 0x00DD4110
// FUNCTION: fn_23229
EncryptedName<25, 0x6D05D5271B5D65C3ULL>& TrouvereRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("TROUVERE\\TROUVERE_RIDING", 0x6D05D5271B5D65C3ULL);
}

// OFFSET: 0x00DD41F0
// FUNCTION: fn_23230
EncryptedName<18, 0x050DF16FC75B25B9ULL>& CardinalBodyPath1() {
  OBFUSCATE_STRING_BODY("CARDINAL\\CARDINAL", 0x050DF16FC75B25B9ULL);
}

// OFFSET: 0x00DD42C0
// FUNCTION: fn_23231
EncryptedName<25, 0x0789EBC9F5C1C59FULL>& CardinalRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("CARDINAL\\CARDINAL_RIDING", 0x0789EBC9F5C1C59FULL);
}

// OFFSET: 0x00DD4530
// FUNCTION: fn_23233
EncryptedName<22, 0x3D8387EF97A37DCFULL>& InquisitorBodyPath1() {
  OBFUSCATE_STRING_BODY("INQUISITOR\\INQUISITOR", 0x3D8387EF97A37DCFULL);
}

// OFFSET: 0x00DD4600
// FUNCTION: fn_23234
EncryptedName<29, 0x07A7D947CF7323BFULL>& InquisitorRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("INQUISITOR\\INQUISITOR_RIDING", 0x07A7D947CF7323BFULL);
}

// OFFSET: 0x00DD46E0
// FUNCTION: fn_23235
EncryptedName<12, 0xD97DC927AFE911F1ULL>& BioloBodyPath1() {
  OBFUSCATE_STRING_BODY("BIOLO\\BIOLO", 0xD97DC927AFE911F1ULL);
}

// OFFSET: 0x00DD47B0
// FUNCTION: fn_23236
EncryptedName<26, 0x8BA52B970DD531ADULL>& BioloRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("BIOLO_RIDING\\BIOLO_RIDING", 0x8BA52B970DD531ADULL);
}

// OFFSET: 0x00DD4890
// FUNCTION: fn_23237
EncryptedName<16, 0x47B7BF55EB471557ULL>& MeisterBodyPath1() {
  OBFUSCATE_STRING_BODY("MEISTER\\MEISTER", 0x47B7BF55EB471557ULL);
}

// OFFSET: 0x00DD4950
// FUNCTION: fn_23238
EncryptedName<30, 0xCFB9671DEF4BD9E7ULL>& MeisterRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("MEISTER_RIDING\\MEISTER_RIDING", 0xCFB9671DEF4BD9E7ULL);
}

// OFFSET: 0x00DD4A30
// FUNCTION: fn_23239
EncryptedName<35, 0x2F2103792383718BULL>& MeisterMadogear1BodyPath1() {
  OBFUSCATE_STRING_BODY("MEISTER_MADOGEAR\\MEISTER_MADOGEAR1", 0x2F2103792383718BULL);
}

// OFFSET: 0x00DD4B10
// FUNCTION: fn_23240
EncryptedName<28, 0x999F5191852D2F6BULL>& DragonKnightBodyPath1() {
  OBFUSCATE_STRING_BODY("DRAGON_KNIGHT\\DRAGON_KNIGHT", 0x999F5191852D2F6BULL);
}

// OFFSET: 0x00DD4BF0
// FUNCTION: fn_23241
EncryptedName<42, 0x4349ADE96FEB27E5ULL>& DragonKnightRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("DRAGON_KNIGHT_RIDING\\DRAGON_KNIGHT_RIDING", 0x4349ADE96FEB27E5ULL);
}

// OFFSET: 0x00DD4CD0
// FUNCTION: fn_23242
EncryptedName<44, 0xA55B7BEF3D7377E3ULL>& DragonKnightChickenBodyPath1() {
  OBFUSCATE_STRING_BODY("DRAGON_KNIGHT_CHICKEN\\DRAGON_KNIGHT_CHICKEN", 0xA55B7BEF3D7377E3ULL);
}

// OFFSET: 0x00DD4DB0
// FUNCTION: fn_23243
EncryptedName<30, 0x31E3CF9D358D8705ULL>& ImperialGuardBodyPath1() {
  OBFUSCATE_STRING_BODY("IMPERIAL_GUARD\\IMPERIAL_GUARD", 0x31E3CF9D358D8705ULL);
}

// OFFSET: 0x00DD4E90
// FUNCTION: fn_23244
EncryptedName<44, 0x79A58B0F2173CF49ULL>& ImperialGuardRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("IMPERIAL_GUARD_RIDING\\IMPERIAL_GUARD_RIDING", 0x79A58B0F2173CF49ULL);
}

// OFFSET: 0x00DD4F70
// FUNCTION: fn_23245
EncryptedName<46, 0xF5496753BDE7C5BDULL>& ImperialGuardChickenBodyPath1() {
  OBFUSCATE_STRING_BODY("IMPERIAL_GUARD_CHICKEN\\IMPERIAL_GUARD_CHICKEN", 0xF5496753BDE7C5BDULL);
}

// OFFSET: 0x00DD5060
// FUNCTION: fn_23246
EncryptedName<26, 0x795787B7779D19DBULL>& ShadowCrossBodyPath1() {
  OBFUSCATE_STRING_BODY("SHADOW_CROSS\\SHADOW_CROSS", 0x795787B7779D19DBULL);
}

// OFFSET: 0x00DD5140
// FUNCTION: fn_23247
EncryptedName<33, 0x6B3917C939150B9BULL>& ShadowCrossRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("SHADOW_CROSS\\SHADOW_CROSS_RIDING", 0x6B3917C939150B9BULL);
}

// OFFSET: 0x00DD5210
// FUNCTION: fn_23248
EncryptedName<26, 0x29AB0B59651BDFD5ULL>& AbyssChaserBodyPath1() {
  OBFUSCATE_STRING_BODY("ABYSS_CHASER\\ABYSS_CHASER", 0x29AB0B59651BDFD5ULL);
}

// OFFSET: 0x00DD52F0
// FUNCTION: fn_23249
EncryptedName<33, 0xF9B1EF9F7961EFF5ULL>& AbyssChaserRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("ABYSS_CHASER\\ABYSS_CHASER_RIDING", 0xF9B1EF9F7961EFF5ULL);
}

// OFFSET: 0x00DD53C0
// FUNCTION: fn_23250
EncryptedName<24, 0xA1DFCDAD555BB16DULL>& SkyEmperorBodyPath1() {
  OBFUSCATE_STRING_BODY("SKY_EMPEROR\\SKY_EMPEROR", 0xA1DFCDAD555BB16DULL);
}

// OFFSET: 0x00DD5490
// FUNCTION: fn_23251
EncryptedName<25, 0xAFF9332FD397533FULL>& SkyEmperor2BodyPath1() {
  OBFUSCATE_STRING_BODY("SKY_EMPEROR\\SKY_EMPEROR2", 0xAFF9332FD397533FULL);
}

// OFFSET: 0x00DD5570
// FUNCTION: fn_23252
EncryptedName<31, 0x194F8339AF753FADULL>& SkyEmperorRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("SKY_EMPEROR\\SKY_EMPEROR_RIDING", 0x194F8339AF753FADULL);
}

// OFFSET: 0x00DD5650
// FUNCTION: fn_23253
EncryptedName<26, 0x7DC10333F96DE3E7ULL>& SoulAsceticBodyPath1() {
  OBFUSCATE_STRING_BODY("SOUL_ASCETIC\\SOUL_ASCETIC", 0x7DC10333F96DE3E7ULL);
}

// OFFSET: 0x00DD5730
// FUNCTION: fn_23254
EncryptedName<33, 0xE9FBAB53D5A1BB17ULL>& SoulAsceticRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("SOUL_ASCETIC\\SOUL_ASCETIC_RIDING", 0xE9FBAB53D5A1BB17ULL);
}

// OFFSET: 0x00DD5800
// FUNCTION: fn_23255
EncryptedName<18, 0x51372B6F4BA14B23ULL>& ShinkiroBodyPath1() {
  OBFUSCATE_STRING_BODY("SHINKIRO\\SHINKIRO", 0x51372B6F4BA14B23ULL);
}

// OFFSET: 0x00DD58D0
// FUNCTION: fn_23256
EncryptedName<25, 0xB70319D3A78D39CDULL>& ShinkiroRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("SHINKIRO\\SHINKIRO_RIDING", 0xB70319D3A78D39CDULL);
}

// OFFSET: 0x00DD59B0
// FUNCTION: fn_23257
EncryptedName<18, 0x1F79132B518FD389ULL>& ShiranuiBodyPath1() {
  OBFUSCATE_STRING_BODY("SHIRANUI\\SHIRANUI", 0x1F79132B518FD389ULL);
}

// OFFSET: 0x00DD5A80
// FUNCTION: fn_23258
EncryptedName<25, 0xC1FD95811D1F4FA1ULL>& ShiranuiRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("SHIRANUI\\SHIRANUI_RIDING", 0xC1FD95811D1F4FA1ULL);
}

// OFFSET: 0x00DD5B60
// FUNCTION: fn_23259
EncryptedName<24, 0x35F59F6F3DD1C965ULL>& NightWatchBodyPath1() {
  OBFUSCATE_STRING_BODY("NIGHT_WATCH\\NIGHT_WATCH", 0x35F59F6F3DD1C965ULL);
}

// OFFSET: 0x00DD5C30
// FUNCTION: fn_23260
EncryptedName<31, 0xD7BBEB2DDB295D11ULL>& NightWatchRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("NIGHT_WATCH\\NIGHT_WATCH_RIDING", 0xD7BBEB2DDB295D11ULL);
}

// OFFSET: 0x00DD5D10
// FUNCTION: fn_23261
EncryptedName<26, 0xBF0F412941076FADULL>& HyperNoviceBodyPath1() {
  OBFUSCATE_STRING_BODY("HYPER_NOVICE\\HYPER_NOVICE", 0xBF0F412941076FADULL);
}

// OFFSET: 0x00DD5DF0
// FUNCTION: fn_23262
EncryptedName<33, 0x8DFF0B4B67D9FDE1ULL>& HyperNoviceRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("HYPER_NOVICE\\HYPER_NOVICE_RIDING", 0x8DFF0B4B67D9FDE1ULL);
}

// OFFSET: 0x00DD5EC0
// FUNCTION: fn_23263
EncryptedName<30, 0xB59BADC5B365617DULL>& SpiritHandlerBodyPath1() {
  OBFUSCATE_STRING_BODY("SPIRIT_HANDLER\\SPIRIT_HANDLER", 0xB59BADC5B365617DULL);
}

// OFFSET: 0x00DD5FA0
// FUNCTION: fn_23264
EncryptedName<37, 0x77D925073D6F19B9ULL>& SpiritHandlerRidingBodyPath1() {
  OBFUSCATE_STRING_BODY("SPIRIT_HANDLER\\SPIRIT_HANDLER_RIDING", 0x77D925073D6F19B9ULL);
}

// OFFSET: 0x00DD6080
// FUNCTION: fn_23265
EncryptedName<12, 0x91E7A7DFA58F9B6BULL>& DruidBodyPath1() {
  OBFUSCATE_STRING_BODY("DRUID\\DRUID", 0x91E7A7DFA58F9B6BULL);
}

// OFFSET: 0x00DD6150
// FUNCTION: fn_23266
EncryptedName<12, 0x2529F16B0375530DULL>& DruidBodyPath2() {
  OBFUSCATE_STRING_BODY("DRUID\\DRUID", 0x2529F16B0375530DULL);
}

// OFFSET: 0x00DD6220
// FUNCTION: fn_23267
EncryptedName<14, 0x91DF1F5D3B014D45ULL>& KarnosBodyPath1() {
  OBFUSCATE_STRING_BODY("KARNOS\\KARNOS", 0x91DF1F5D3B014D45ULL);
}

// OFFSET: 0x00DD62F0
// FUNCTION: fn_23268
EncryptedName<14, 0x0D7FA94DD32361A9ULL>& KarnosBodyPath2() {
  OBFUSCATE_STRING_BODY("KARNOS\\KARNOS", 0x0D7FA94DD32361A9ULL);
}

// OFFSET: 0x00DD63C0
// FUNCTION: fn_23269
EncryptedName<14, 0x2F371DDDADDD53CFULL>& AliteaBodyPath1() {
  OBFUSCATE_STRING_BODY("ALITEA\\ALITEA", 0x2F371DDDADDD53CFULL);
}

// OFFSET: 0x00DD6490
// FUNCTION: fn_23270
EncryptedName<6, 0x2F3D75DFCF678FF3ULL>& DruidJobName1() {
  OBFUSCATE_STRING_BODY("DRUID", 0x2F3D75DFCF678FF3ULL);
}

// OFFSET: 0x00DD6560
// FUNCTION: fn_23271
EncryptedName<6, 0x3BBB5F0F8F216F31ULL>& DruidJobName2() {
  OBFUSCATE_STRING_BODY("DRUID", 0x3BBB5F0F8F216F31ULL);
}

// OFFSET: 0x00DD6630
// FUNCTION: fn_23272
EncryptedName<7, 0x6F9D21B96D71C1FFULL>& KarnosJobName1() {
  OBFUSCATE_STRING_BODY("KARNOS", 0x6F9D21B96D71C1FFULL);
}

// OFFSET: 0x00DD6700
// FUNCTION: fn_23273
EncryptedName<7, 0xED730315539785C7ULL>& KarnosJobName2() {
  OBFUSCATE_STRING_BODY("KARNOS", 0xED730315539785C7ULL);
}

// OFFSET: 0x00DD67D0
// FUNCTION: fn_23274
EncryptedName<7, 0x8F61434F0F79DDD7ULL>& AliteaJobName1() {
  OBFUSCATE_STRING_BODY("ALITEA", 0x8F61434F0F79DDD7ULL);
}

// OFFSET: 0x00DD68A0
// FUNCTION: fn_23275
EncryptedName<9, 0xA5C5D1A3B9457B53ULL>& WerewolfJobName1() {
  OBFUSCATE_STRING_BODY("WEREWOLF", 0xA5C5D1A3B9457B53ULL);
}

// OFFSET: 0x00DD6A40
// FUNCTION: fn_23277
EncryptedName<13, 0x4B670715D543C1C3ULL>& DruidRidingName() {
  OBFUSCATE_STRING_BODY("DRUID_RIDING", 0x4B670715D543C1C3ULL);
}

// OFFSET: 0x00DD6B10
// FUNCTION: fn_23278
EncryptedName<14, 0x7155E5EF0DFB0D03ULL>& KarnosRidingName() {
  OBFUSCATE_STRING_BODY("KARNOS_RIDING", 0x7155E5EF0DFB0D03ULL);
}

// OFFSET: 0x00DD6BE0
// FUNCTION: fn_23279
EncryptedName<14, 0x4B1D078937DB892BULL>& AliteaRidingName() {
  OBFUSCATE_STRING_BODY("ALITEA_RIDING", 0x4B1D078937DB892BULL);
}

// OFFSET: 0x00E29530
// FUNCTION: fn_24044
EncryptedName<14, 0x6BC977B3B5993503ULL>& PoringSuperNovice2SpriteName() {
  OBFUSCATE_STRING_BODY("초보자\\초보자", 0x6BC977B3B5993503ULL);
}

// OFFSET: 0x00E29600
// FUNCTION: fn_24045
EncryptedName<14, 0x91493751EF33B5FDULL>& PoringSuperNovice2BabySpriteName() {
  OBFUSCATE_STRING_BODY("초보자\\초보자", 0x91493751EF33B5FDULL);
}

// OFFSET: 0x00E296D0
// FUNCTION: fn_24046
EncryptedName<18, 0x0DF5B33D9F21EB79ULL>& SheepPriestBabySpriteName() {
  OBFUSCATE_STRING_BODY("프리스트\\프리스트", 0x0DF5B33D9F21EB79ULL);
}

// OFFSET: 0x00E297A0
// FUNCTION: fn_24047
EncryptedName<14, 0x01E7898D9D235DDFULL>& FoxWizardBabySpriteName() {
  OBFUSCATE_STRING_BODY("위저드\\위저드", 0x01E7898D9D235DDFULL);
}

// OFFSET: 0x00E29870
// FUNCTION: fn_24048
EncryptedName<14, 0x6961FF056999A303ULL>& PigBlacksmithBabySpriteName() {
  OBFUSCATE_STRING_BODY("제철공\\제철공", 0x6961FF056999A303ULL);
}

// OFFSET: 0x00E29940
// FUNCTION: fn_24049
EncryptedName<14, 0x9DEFC105A1EB2F71ULL>& PigMechanicSpriteName() {
  OBFUSCATE_STRING_BODY("미케닉\\미케닉", 0x9DEFC105A1EB2F71ULL);
}

// OFFSET: 0x00E29A10
// FUNCTION: fn_24050
EncryptedName<14, 0xC7FB6DF18577D34BULL>& PigMechanicBabySpriteName() {
  OBFUSCATE_STRING_BODY("미케닉\\미케닉", 0xC7FB6DF18577D34BULL);
}

// OFFSET: 0x00E29AE0
// FUNCTION: fn_24051
EncryptedName<14, 0xBD3F430159B551FDULL>& OstrichRangerSpriteName() {
  OBFUSCATE_STRING_BODY("레인져\\레인져", 0xBD3F430159B551FDULL);
}

// OFFSET: 0x00E29BB0
// FUNCTION: fn_24052
EncryptedName<14, 0x2B477F9B5B39B301ULL>& OstrichRangerBabySpriteName() {
  OBFUSCATE_STRING_BODY("레인져\\레인져", 0x2B477F9B5B39B301ULL);
}

// OFFSET: 0x00E29C80
// FUNCTION: fn_24053
EncryptedName<10, 0x5B832361BDA77197ULL>& LionKnightSpriteName() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0x5B832361BDA77197ULL);
}

// OFFSET: 0x00E29D50
// FUNCTION: fn_24054
EncryptedName<10, 0xEB3D4FAB0FC183DBULL>& LionKnightBabySpriteName() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0xEB3D4FAB0FC183DBULL);
}

// OFFSET: 0x00E29E20
// FUNCTION: fn_24055
EncryptedName<10, 0xFFBF43D1FFD5FF49ULL>& LionKnightHighSpriteName() {
  OBFUSCATE_STRING_BODY("기사\\기사", 0xFFBF43D1FFD5FF49ULL);
}

// OFFSET: 0x00E29EF0
// FUNCTION: fn_24056
EncryptedName<18, 0xE51B29334D353B81ULL>& LionRoyalGuardSpriteName() {
  OBFUSCATE_STRING_BODY("로얄가드\\로얄가드", 0xE51B29334D353B81ULL);
}

// OFFSET: 0x00E29FC0
// FUNCTION: fn_24057
EncryptedName<18, 0x8F3F2FA959977F9FULL>& LionRoyalGuardBabySpriteName() {
  OBFUSCATE_STRING_BODY("로얄가드\\로얄가드", 0x8F3F2FA959977F9FULL);
}

// OFFSET: 0x00E2A090
// FUNCTION: fn_24058
EncryptedName<18, 0x7FE563BD3FB7F925ULL>& LionRuneKnightSpriteName() {
  OBFUSCATE_STRING_BODY("룬나이트\\룬나이트", 0x7FE563BD3FB7F925ULL);
}

// OFFSET: 0x00E2A160
// FUNCTION: fn_24059
EncryptedName<18, 0x8B3FA35B5BDDDF89ULL>& LionRuneKnightBabySpriteName() {
  OBFUSCATE_STRING_BODY("룬나이트\\룬나이트", 0x8B3FA35B5BDDDF89ULL);
}

// OFFSET: 0x00E2A230
// FUNCTION: fn_24060
EncryptedName<22, 0x7DB1050779034DE7ULL>& LionCrusaderSpriteName() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0x7DB1050779034DE7ULL);
}

// OFFSET: 0x00E2A300
// FUNCTION: fn_24061
EncryptedName<22, 0x5D6B3B1DEB2397B1ULL>& LionCrusaderBabySpriteName() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0x5D6B3B1DEB2397B1ULL);
}

// OFFSET: 0x00E2A3D0
// FUNCTION: fn_24062
EncryptedName<22, 0x07112F85A39F4FABULL>& LionCrusaderHighSpriteName() {
  OBFUSCATE_STRING_BODY("크루세이더\\크루세이더", 0x07112F85A39F4FABULL);
}

// OFFSET: 0x00E2A4A0
// FUNCTION: fn_24063
EncryptedName<16, 0x45E3F103CBB5F157ULL>& KagerouSpriteName() {
  OBFUSCATE_STRING_BODY("kagerou\\kagerou", 0x45E3F103CBB5F157ULL);
}

// OFFSET: 0x00E2A560
// FUNCTION: fn_24064
EncryptedName<12, 0x936F3D2FFDFD87F5ULL>& OboroSpriteName() {
  OBFUSCATE_STRING_BODY("oboro\\oboro", 0x936F3D2FFDFD87F5ULL);
}

// OFFSET: 0x00E2A630
// FUNCTION: fn_24065
EncryptedName<16, 0x2D33791F67AD47DFULL>& FrogKagerouSpriteName() {
  OBFUSCATE_STRING_BODY("kagerou\\kagerou", 0x2D33791F67AD47DFULL);
}

// OFFSET: 0x00E2A6F0
// FUNCTION: fn_24066
EncryptedName<12, 0x6B3301E1A3079125ULL>& FrogOboroSpriteName() {
  OBFUSCATE_STRING_BODY("oboro\\oboro", 0x6B3301E1A3079125ULL);
}

// OFFSET: 0x00E2A7C0
// FUNCTION: fn_24067
EncryptedName<20, 0x2F2701A7E56D3D89ULL>& RebellionSpriteName() {
  OBFUSCATE_STRING_BODY("rebellion\\rebellion", 0x2F2701A7E56D3D89ULL);
}

// OFFSET: 0x00E2A890
// FUNCTION: fn_24068
EncryptedName<18, 0xC39FB559A17D3963ULL>& DoramSummonerSpriteName() {
  OBFUSCATE_STRING_BODY("summoner\\summoner", 0xC39FB559A17D3963ULL);
}

// OFFSET: 0x00E2A960
// FUNCTION: fn_24069
EncryptedName<23, 0x71B7691737C5A1AFULL>& DoramCartSummonerSpriteName() {
  OBFUSCATE_STRING_BODY("summoner\\cart_summoner", 0x71B7691737C5A1AFULL);
}

// OFFSET: 0x00E2AA40
// FUNCTION: fn_24070
EncryptedName<18, 0x899D11BB83695B0DULL>& DoramSummonerBabySpriteName() {
  OBFUSCATE_STRING_BODY("summoner\\summoner", 0x899D11BB83695B0DULL);
}

// OFFSET: 0x00E2AB10
// FUNCTION: fn_24071
EncryptedName<23, 0x2B8725D7E95325F5ULL>& DoramCartSummonerBabySpriteName() {
  OBFUSCATE_STRING_BODY("summoner\\cart_summoner", 0x2B8725D7E95325F5ULL);
}

// OFFSET: 0x00E2ABF0
// FUNCTION: fn_24072
EncryptedName<10, 0x7989830DE73B9DBDULL>& NinjaBabySpriteName() {
  OBFUSCATE_STRING_BODY("닌자\\닌자", 0x7989830DE73B9DBDULL);
}

// OFFSET: 0x00E2ACC0
// FUNCTION: fn_24073
EncryptedName<10, 0xBDADDD87C5A50B6BULL>& FrogNinjaBabySpriteName() {
  OBFUSCATE_STRING_BODY("닌자\\닌자", 0xBDADDD87C5A50B6BULL);
}

// OFFSET: 0x00E2AD90
// FUNCTION: fn_24074
EncryptedName<16, 0xA7471985F93F9BDFULL>& KagerouBabySpriteName() {
  OBFUSCATE_STRING_BODY("kagerou\\kagerou", 0xA7471985F93F9BDFULL);
}

// OFFSET: 0x00E2AE50
// FUNCTION: fn_24075
EncryptedName<12, 0x5FFD7FABB5D1EF57ULL>& OboroBabySpriteName() {
  OBFUSCATE_STRING_BODY("oboro\\oboro", 0x5FFD7FABB5D1EF57ULL);
}

// OFFSET: 0x00E2AF20
// FUNCTION: fn_24076
EncryptedName<16, 0xD3E5516F89F161D1ULL>& FrogKagerouBabySpriteName() {
  OBFUSCATE_STRING_BODY("kagerou\\kagerou", 0xD3E5516F89F161D1ULL);
}

// OFFSET: 0x00E2AFE0
// FUNCTION: fn_24077
EncryptedName<12, 0xF535E561137F138DULL>& FrogOboroBabySpriteName() {
  OBFUSCATE_STRING_BODY("oboro\\oboro", 0xF535E561137F138DULL);
}

// OFFSET: 0x00E2B0B0
// FUNCTION: fn_24078
EncryptedName<18, 0xABED6971354375C3ULL>& TaekwonBabySpriteName() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0xABED6971354375C3ULL);
}

// OFFSET: 0x00E2B180
// FUNCTION: fn_24079
EncryptedName<18, 0x01C9FBA7819FFBD7ULL>& PoringTaekwonBabySpriteName() {
  OBFUSCATE_STRING_BODY("태권소년\\태권소년", 0x01C9FBA7819FFBD7ULL);
}

// OFFSET: 0x00E2B250
// FUNCTION: fn_24080
EncryptedName<10, 0xFBAD074BDB2B77A1ULL>& StarGladiatorBabySpriteName() {
  OBFUSCATE_STRING_BODY("권성\\권성", 0xFBAD074BDB2B77A1ULL);
}

// OFFSET: 0x00E2B320
// FUNCTION: fn_24081
EncryptedName<10, 0x6BA557130BDFE997ULL>& StarGladiator2BabySpriteName() {
  OBFUSCATE_STRING_BODY("권성\\권성", 0x6BA557130BDFE997ULL);
}

// OFFSET: 0x00E2B3F0
// FUNCTION: fn_24082
EncryptedName<10, 0x050DF16FC75B25B9ULL>& PoringStarGladiatorBabySpriteName() {
  OBFUSCATE_STRING_BODY("권성\\권성", 0x050DF16FC75B25B9ULL);
}

// OFFSET: 0x00E2B4C0
// FUNCTION: fn_24083
EncryptedName<18, 0x6D05D5271B5D65C3ULL>& SoulLinkerBabySpriteName() {
  OBFUSCATE_STRING_BODY("소울링커\\소울링커", 0x6D05D5271B5D65C3ULL);
}

// OFFSET: 0x00E2B590
// FUNCTION: fn_24084
EncryptedName<18, 0x3D8387EF97A37DCFULL>& FrogSoulLinkerBabySpriteName() {
  OBFUSCATE_STRING_BODY("소울링커\\소울링커", 0x3D8387EF97A37DCFULL);
}

// OFFSET: 0x00E2E2E0
// FUNCTION: fn_24138
EncryptedName<24, 0x3BBB5F0F8F216F31ULL>& NightWatchBodyPath2() {
  OBFUSCATE_STRING_BODY("NIGHT_WATCH\\NIGHT_WATCH", 0x3BBB5F0F8F216F31ULL);
}

// OFFSET: 0x00E2E3B0
// FUNCTION: fn_24139
EncryptedName<31, 0x2F3D75DFCF678FF3ULL>& NightWatchRidingBodyPath2() {
  OBFUSCATE_STRING_BODY("NIGHT_WATCH\\NIGHT_WATCH_RIDING", 0x2F3D75DFCF678FF3ULL);
}

// OFFSET: 0x00E2E490
// FUNCTION: fn_24140
EncryptedName<26, 0xED730315539785C7ULL>& HyperNoviceBodyPath2() {
  OBFUSCATE_STRING_BODY("HYPER_NOVICE\\HYPER_NOVICE", 0xED730315539785C7ULL);
}

// OFFSET: 0x00E2E570
// FUNCTION: fn_24141
EncryptedName<33, 0x6F9D21B96D71C1FFULL>& HyperNoviceRidingBodyPath2() {
  OBFUSCATE_STRING_BODY("HYPER_NOVICE\\HYPER_NOVICE_RIDING", 0x6F9D21B96D71C1FFULL);
}

// OFFSET: 0x00E2E640
// FUNCTION: fn_24142
EncryptedName<30, 0x39E7F13755252F8FULL>& SpiritHandlerBodyPath2() {
  OBFUSCATE_STRING_BODY("SPIRIT_HANDLER\\SPIRIT_HANDLER", 0x39E7F13755252F8FULL);
}

// OFFSET: 0x00E2E720
// FUNCTION: fn_24143
EncryptedName<37, 0xCBD7DDAFDF87F549ULL>& SpiritHandlerRidingBodyPath2() {
  OBFUSCATE_STRING_BODY("SPIRIT_HANDLER\\SPIRIT_HANDLER_RIDING", 0xCBD7DDAFDF87F549ULL);
}

// OFFSET: 0x00E2E800
// FUNCTION: fn_24144
EncryptedName<12, 0xEB074B7FBDA3BFD5ULL>& DruidBodyPath3() {
  OBFUSCATE_STRING_BODY("DRUID\\DRUID", 0xEB074B7FBDA3BFD5ULL);
}

// OFFSET: 0x00E2E8D0
// FUNCTION: fn_24145
EncryptedName<12, 0x3D075BD5256F9B6FULL>& DruidBodyPath4() {
  OBFUSCATE_STRING_BODY("DRUID\\DRUID", 0x3D075BD5256F9B6FULL);
}

// OFFSET: 0x00E2E9A0
// FUNCTION: fn_24146
EncryptedName<14, 0x251761D7370BA5C3ULL>& KarnosBodyPath3() {
  OBFUSCATE_STRING_BODY("KARNOS\\KARNOS", 0x251761D7370BA5C3ULL);
}

// OFFSET: 0x00E2EA70
// FUNCTION: fn_24147
EncryptedName<14, 0x03014749B127914FULL>& KarnosBodyPath4() {
  OBFUSCATE_STRING_BODY("KARNOS\\KARNOS", 0x03014749B127914FULL);
}

// OFFSET: 0x00E2EB40
// FUNCTION: fn_24148
EncryptedName<14, 0x85FB93CF2F494179ULL>& AliteaBodyPath2() {
  OBFUSCATE_STRING_BODY("ALITEA\\ALITEA", 0x85FB93CF2F494179ULL);
}

// OFFSET: 0x00DD6970
// FUNCTION: fn_23276
EncryptedName<11, 0x61F59D5D41E76519ULL>& WereRaptorJobName1() {
  OBFUSCATE_STRING_BODY("WERERAPTOR", 0x61F59D5D41E76519ULL);
}
