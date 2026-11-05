#include "Client/Session.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Contents/Jobs/JobId.h"
#include "Utility/Cipher/XorBytes.h"
#include "Utility/Text/FormatString.h"

#include <cstddef>
#include <cstdint>
#include <string>

template <std::size_t Length, std::uint64_t Key>
class EncryptedName {
public:
  // INLINED
  explicit EncryptedName(const char (&source)[Length]) : m_encrypted(true) {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = source[index];
    }
  }

  ~EncryptedName() {
    for (std::uint64_t index = 0; index < Length; ++index) {
      this->m_text[index] = 0;
    }
  }

  const char* Text() {
    if (this->m_encrypted) {
      utility::XorBytes(this->m_text, Length, Key);
      this->m_encrypted = false;
    }

    return reinterpret_cast<const char*>(this->m_text);
  }

private:
  /* 0000 */ std::uint8_t m_text[Length];
  bool m_encrypted;
};

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
  sexName = g_session.GetSexName(sex);
  jobName = g_session.GetBodyJobName(spriteJob);

  if (secondJob == true) {
    result = this->GetBodyRacePrefix(spriteJob + 3950) +
             FormatOwnedString("\\몸통\\%s\\costume_%d\\%s_%s_%d.%s", sexName.c_str(), 1, jobName.c_str(), sexName.c_str(), 1, g_equipmentResourceExtensions[type]);
  } else {
    result = this->GetBodyRacePrefix(spriteJob + 3950) +
             FormatOwnedString("\\몸통\\%s\\%s_%s.%s", sexName.c_str(), jobName.c_str(), sexName.c_str(), g_equipmentResourceExtensions[type]);
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
  const char source[] = {'\x9B', '\x3A', '\xE5', '\xD7', '\x32', '\xC8', '\x71', '\xC7', '\xFC', '\x5D', '\xDF', '\x98', '\x62', '\x2D'};
  thread_local EncryptedName<sizeof(source), 0x072D2D872D259D5BULL> name(source);
  return name;
}

// OFFSET: 0x00DCA240
// FUNCTION: fn_23036
EncryptedName<18, 0x9BE3EF737387AD79ULL>& PriestBodyPath1() {
  const char source[] = {'\xBE', '\x6C', '\x3F', '\xDD', '\xCE', '\x55', '\x25', '\x35', '\x25', '\x6A', '\x46', '\xCB', '\xDD', '\x52', '\x59', '\x5D', '\xD7', '\xAD'};
  thread_local EncryptedName<sizeof(source), 0x9BE3EF737387AD79ULL> name(source);
  return name;
}

// OFFSET: 0x00DCA310
// FUNCTION: fn_23037
EncryptedName<14, 0xB96B77B7255F8569ULL>& AssassinBodyPath1() {
  const char source[] = {'\xD7', '\x6B', '\xE3', '\x99', '\x0A', '\xB2', '\x37', '\x07', '\x87', '\x39', '\xE3', '\x98', '\x72', '\x77'};
  thread_local EncryptedName<sizeof(source), 0xB96B77B7255F8569ULL> name(source);
  return name;
}

// OFFSET: 0x00DCA3E0
// FUNCTION: fn_23038
EncryptedName<14, 0x11CB35636377DD07ULL>& SageBodyPath1() {
  const char source[] = {'\xBB', '\x61', '\xB7', '\xAF', '\xA2', '\xC3', '\x97', '\xAD', '\xBB', '\x1D', '\xBB', '\xA2', '\x95', '\x35'};
  thread_local EncryptedName<sizeof(source), 0x11CB35636377DD07ULL> name(source);
  return name;
}

// OFFSET: 0x00DCA4B0
// FUNCTION: fn_23039
EncryptedName<10, 0x4755C1355575FD37ULL>& BardBodyPath1() {
  const char source[] = {'\x8E', '\x24', '\xC0', '\xB0', '\x69', '\x78', '\x8C', '\xF2', '\xD2', '\xFD'};
  thread_local EncryptedName<sizeof(source), 0x4755C1355575FD37ULL> name(source);
  return name;
}

// OFFSET: 0x00DCA580
// FUNCTION: fn_23040
EncryptedName<10, 0x9D8F2D37BFFD61B7ULL>& DancerBodyPath1() {
  const char source[] = {'\x0E', '\xCA', '\x35', '\x4E', '\x6B', '\x94', '\x24', '\x55', '\x46', '\x61'};
  thread_local EncryptedName<sizeof(source), 0x9D8F2D37BFFD61B7ULL> name(source);
  return name;
}

// OFFSET: 0x00DCA650
// FUNCTION: fn_23041
EncryptedName<10, 0x79DF094DD9616575ULL>& MonkBodyPath1() {
  const char source[] = {'\xCD', '\x9C', '\xA4', '\x70', '\x11', '\xB1', '\x26', '\xBC', '\xDC', '\x65'};
  thread_local EncryptedName<sizeof(source), 0x79DF094DD9616575ULL> name(source);
  return name;
}

// OFFSET: 0x00DCA720
// FUNCTION: fn_23042
EncryptedName<18, 0x038535BB092533B7ULL>& AlchemistBodyPath1() {
  const char source[] = {'\x08', '\x9F', '\x94', '\xD4', '\x07', '\xCF', '\x3E', '\xE4', '\xEB', '\x8C', '\x89', '\xB8', '\x66', '\x89', '\x7F', '\xB8', '\x50', '\x33'};
  thread_local EncryptedName<sizeof(source), 0x038535BB092533B7ULL> name(source);
  return name;
}

// OFFSET: 0x00DCA7F0
// FUNCTION: fn_23043
EncryptedName<10, 0xFB2BC75769E777D3ULL>& RogueBodyPath1() {
  const char source[] = {'\x64', '\xB9', '\x56', '\xBE', '\x0B', '\x70', '\xE5', '\x4A', '\x04', '\x77'};
  thread_local EncryptedName<sizeof(source), 0xFB2BC75769E777D3ULL> name(source);
  return name;
}

// OFFSET: 0x00DCA8C0
// FUNCTION: fn_23044
EncryptedName<14, 0xB5D7B5AF4FEDFF57ULL>& NoviceBodyPath1() {
  const char source[] = {'\x94', '\x35', '\x57', '\xF7', '\x6F', '\x6F', '\x8B', '\x76', '\x9D', '\x45', '\x55', '\x8F', '\x75', '\xB5'};
  thread_local EncryptedName<sizeof(source), 0xB5D7B5AF4FEDFF57ULL> name(source);
  return name;
}

// OFFSET: 0x00DCA990
// FUNCTION: fn_23045
EncryptedName<10, 0x9B9D51AD3515F779ULL>& SwordmanBodyPath1() {
  const char source[] = {'\xC9', '\x3C', '\xAE', '\xD2', '\xF1', '\xE1', '\x56', '\x20', '\x9E', '\xF7'};
  thread_local EncryptedName<sizeof(source), 0x9B9D51AD3515F779ULL> name(source);
  return name;
}

// OFFSET: 0x00DCAA60
// FUNCTION: fn_23046
EncryptedName<14, 0xB5F3F779237315F3ULL>& MagicianBodyPath1() {
  const char source[] = {'\x4B', '\xA3', '\xCA', '\xDE', '\xC2', '\x10', '\xAF', '\x0D', '\x45', '\xAC', '\x8E', '\x98', '\x9E', '\xF7'};
  thread_local EncryptedName<sizeof(source), 0xB5F3F779237315F3ULL> name(source);
  return name;
}

// OFFSET: 0x00DCAB30
// FUNCTION: fn_23047
EncryptedName<10, 0x67DD5BAB1919D9D1ULL>& ArcherBodyPath1() {
  const char source[] = {'\x60', '\x1A', '\xA5', '\xEF', '\xF7', '\xEA', '\x1E', '\xDB', '\x27', '\xD9'};
  thread_local EncryptedName<sizeof(source), 0x67DD5BAB1919D9D1ULL> name(source);
  return name;
}

// OFFSET: 0x00DCAC00
// FUNCTION: fn_23048
EncryptedName<14, 0x91493751EF33B5FDULL>& AcolyteBodyPath1() {
  const char source[] = {'\x41', '\x0F', '\xF2', '\x18', '\x91', '\xED', '\x15', '\x2D', '\x47', '\x74', '\xC4', '\x2F', '\x8B', '\x37'};
  thread_local EncryptedName<sizeof(source), 0x91493751EF33B5FDULL> name(source);
  return name;
}

// OFFSET: 0x00DCACD0
// FUNCTION: fn_23049
EncryptedName<10, 0x6BC977B3B5993503ULL>& MerchantBodyPath1() {
  const char source[] = {'\xB8', '\xC6', '\x59', '\x7B', '\xEF', '\xCC', '\x3A', '\xAB', '\xCD', '\x35'};
  thread_local EncryptedName<sizeof(source), 0x6BC977B3B5993503ULL> name(source);
  return name;
}

// OFFSET: 0x00DCADA0
// FUNCTION: fn_23050
EncryptedName<10, 0xB7A76BF9CD41F99FULL>& ThiefBodyPath1() {
  const char source[] = {'\x2A', '\x4C', '\xF4', '\x02', '\xA5', '\xDE', '\x12', '\x02', '\x50', '\xF9'};
  thread_local EncryptedName<sizeof(source), 0xB7A76BF9CD41F99FULL> name(source);
  return name;
}

// OFFSET: 0x00DCAE70
// FUNCTION: fn_23051
EncryptedName<10, 0x732B390D63FD47D7ULL>& ThiefBodyPath2() {
  const char source[] = {'\x62', '\xF2', '\x48', '\xAC', '\x51', '\x8C', '\x9E', '\xC6', '\x18', '\x47'};
  thread_local EncryptedName<sizeof(source), 0x732B390D63FD47D7ULL> name(source);
  return name;
}

// OFFSET: 0x00DCAF40
// FUNCTION: fn_23052
EncryptedName<14, 0x1FDB81175D1557E3ULL>& NoviceBodyPath2() {
  const char source[] = {'\x20', '\x9D', '\xAF', '\xE5', '\xD7', '\x5B', '\x87', '\xDC', '\x29', '\xED', '\xAD', '\x9D', '\xCD', '\x81'};
  thread_local EncryptedName<sizeof(source), 0x1FDB81175D1557E3ULL> name(source);
  return name;
}

// OFFSET: 0x00DCB010
// FUNCTION: fn_23053
EncryptedName<10, 0xE3FB23537D2785BFULL>& SwordmanBodyPath2() {
  const char source[] = {'\x0F', '\x4E', '\x9C', '\x9A', '\x0F', '\x93', '\x30', '\x58', '\x58', '\x85'};
  thread_local EncryptedName<sizeof(source), 0xE3FB23537D2785BFULL> name(source);
  return name;
}

// OFFSET: 0x00DCB0E0
// FUNCTION: fn_23054
EncryptedName<14, 0x8BBF792B47E72173ULL>& MagicianBodyPath2() {
  const char source[] = {'\xCB', '\x97', '\x5E', '\xBA', '\x90', '\x9E', '\xE3', '\x33', '\xC5', '\x98', '\x1A', '\xFC', '\xCC', '\x79'};
  thread_local EncryptedName<sizeof(source), 0x8BBF792B47E72173ULL> name(source);
  return name;
}

// OFFSET: 0x00DCB1B0
// FUNCTION: fn_23055
EncryptedName<10, 0xB3CF9BEB7963DB89ULL>& ArcherBodyPath2() {
  const char source[] = {'\x38', '\x18', '\xDF', '\x8F', '\xB7', '\x2A', '\x0C', '\x0F', '\x7F', '\xDB'};
  thread_local EncryptedName<sizeof(source), 0xB3CF9BEB7963DB89ULL> name(source);
  return name;
}

// OFFSET: 0x00DCB280
// FUNCTION: fn_23056
EncryptedName<14, 0x71A509DB43792991ULL>& AcolyteBodyPath2() {
  const char source[] = {'\x2D', '\x93', '\xB8', '\xB4', '\x1B', '\xD3', '\xF9', '\xCD', '\x2B', '\xE8', '\x8E', '\x83', '\x01', '\x09'};
  thread_local EncryptedName<sizeof(source), 0x71A509DB43792991ULL> name(source);
  return name;
}

// OFFSET: 0x00DCB350
// FUNCTION: fn_23057
EncryptedName<10, 0xDD69EDFFA18F537DULL>& MerchantBodyPath2() {
  const char source[] = {'\xC6', '\xA0', '\x4F', '\x6F', '\xA3', '\x56', '\x9A', '\x1D', '\xB3', '\x53'};
  thread_local EncryptedName<sizeof(source), 0xDD69EDFFA18F537DULL> name(source);
  return name;
}

// OFFSET: 0x00DCB420
// FUNCTION: fn_23058
EncryptedName<10, 0xFFBF43D1FFD5FF49ULL>& ThiefBodyPath3() {
  const char source[] = {'\xFC', '\x4A', '\x60', '\x30', '\x8D', '\xF6', '\x0A', '\x4A', '\x86', '\xFF'};
  thread_local EncryptedName<sizeof(source), 0xFFBF43D1FFD5FF49ULL> name(source);
  return name;
}

// OFFSET: 0x00DCB4F0
// FUNCTION: fn_23059
EncryptedName<10, 0xEB3D4FAB0FC183DBULL>& KnightBodyPath1() {
  const char source[] = {'\x6A', '\x61', '\x7A', '\xE8', '\xF7', '\xFE', '\xDF', '\x50', '\x3C', '\x83'};
  thread_local EncryptedName<sizeof(source), 0xEB3D4FAB0FC183DBULL> name(source);
  return name;
}

// OFFSET: 0x00DCB5C0
// FUNCTION: fn_23060
EncryptedName<18, 0x8F3F2FA959977F9FULL>& PriestBodyPath2() {
  const char source[] = {'\x58', '\xBE', '\x2F', '\xF7', '\x14', '\x95', '\xF9', '\x21', '\xC3', '\xB8', '\x56', '\xE1', '\x07', '\x92', '\x85', '\x49', '\x31', '\x7F'};
  thread_local EncryptedName<sizeof(source), 0x8F3F2FA959977F9FULL> name(source);
  return name;
}

// OFFSET: 0x00DCB690
// FUNCTION: fn_23061
EncryptedName<14, 0xE51B29334D353B81ULL>& WizardBodyPath2() {
  const char source[] = {'\x41', '\x9C', '\xF5', '\xB7', '\x86', '\xCC', '\x47', '\x25', '\x26', '\xFB', '\xCF', '\xF8', '\xD6', '\x29'};
  thread_local EncryptedName<sizeof(source), 0xE51B29334D353B81ULL> name(source);
  return name;
}

// OFFSET: 0x00DCB760
// FUNCTION: fn_23062
EncryptedName<14, 0x8B3FA35B5BDDDF89ULL>& BlacksmithBodyPath1() {
  const char source[] = {'\x48', '\x79', '\x1E', '\xED', '\xEB', '\x5B', '\x63', '\x4A', '\x2F', '\x1C', '\x6B', '\xEB', '\xA3', '\xA3'};
  thread_local EncryptedName<sizeof(source), 0x8B3FA35B5BDDDF89ULL> name(source);
  return name;
}

// OFFSET: 0x00DCB830
// FUNCTION: fn_23063
EncryptedName<10, 0x7FE563BD3FB7F925ULL>& HunterBodyPath1() {
  const char source[] = {'\xE2', '\x1C', '\x72', '\xF2', '\xE1', '\xA4', '\x00', '\xBA', '\xE8', '\xF9'};
  thread_local EncryptedName<sizeof(source), 0x7FE563BD3FB7F925ULL> name(source);
  return name;
}

// OFFSET: 0x00DCB900
// FUNCTION: fn_23064
EncryptedName<14, 0x5D6B3B1DEB2397B1ULL>& AssassinBodyPath2() {
  const char source[] = {'\x0F', '\x79', '\x9F', '\x57', '\xA0', '\xFE', '\x37', '\xE3', '\x5F', '\x2B', '\x9F', '\x56', '\xD8', '\x3B'};
  thread_local EncryptedName<sizeof(source), 0x5D6B3B1DEB2397B1ULL> name(source);
  return name;
}

// OFFSET: 0x00DCB9D0
// FUNCTION: fn_23065
EncryptedName<28, 0x7DB1050779034DE7ULL>& PecoKnightBodyPath1() {
  const char source[] = {'\x21', '\xA9', '\xC7', '\xA3', '\xC1', '\xE1', '\x75', '\xA7', '\xB8', '\xFC', '\xE1', '\xC2', '\xE0', '\x59', '\x77', '\x99', '\x23', '\x97', '\xC5', '\x9D', '\xC3', '\xDF', '\xEE', '\xCC', '\x05', '\xF6', '\xE4', '\x79'};
  thread_local EncryptedName<sizeof(source), 0x7DB1050779034DE7ULL> name(source);
  return name;
}

// OFFSET: 0x00DCBAB0
// FUNCTION: fn_23066
EncryptedName<22, 0x01E7898D9D235DDFULL>& CrusaderBodyPath1() {
  const char source[] = {'\x1A', '\xF4', '\x94', '\x7A', '\x31', '\x35', '\x27', '\xCD', '\x6B', '\xA8', '\x7F', '\x58', '\x24', '\x3E', '\x00', '\xBD', '\x63', '\x9D', '\xEF', '\x29', '\x78', '\x89'};
  thread_local EncryptedName<sizeof(source), 0x01E7898D9D235DDFULL> name(source);
  return name;
}

// OFFSET: 0x00DCBB80
// FUNCTION: fn_23067
EncryptedName<10, 0x0DF5B33D9F21EB79ULL>& MonkBodyPath2() {
  const char source[] = {'\xC1', '\x12', '\xE4', '\x36', '\x61', '\x0B', '\x0C', '\xC8', '\xD0', '\xEB'};
  thread_local EncryptedName<sizeof(source), 0x0DF5B33D9F21EB79ULL> name(source);
  return name;
}

// OFFSET: 0x00DCBC50
// FUNCTION: fn_23068
EncryptedName<14, 0x9DEFC105A1EB2F71ULL>& SageBodyPath2() {
  const char source[] = {'\xCD', '\x93', '\x2B', '\x6D', '\xC4', '\x37', '\xB3', '\x21', '\xCD', '\xEF', '\x27', '\x60', '\xF3', '\xC1'};
  thread_local EncryptedName<sizeof(source), 0x9DEFC105A1EB2F71ULL> name(source);
  return name;
}

// OFFSET: 0x00DCBD20
// FUNCTION: fn_23069
EncryptedName<10, 0x6961FF056999A303ULL>& RogueBodyPath2() {
  const char source[] = {'\xB4', '\x6D', '\x28', '\xBE', '\x59', '\x48', '\xAF', '\xD8', '\xD4', '\xA3'};
  thread_local EncryptedName<sizeof(source), 0x6961FF056999A303ULL> name(source);
  return name;
}

// OFFSET: 0x00DCBDF0
// FUNCTION: fn_23070
EncryptedName<18, 0xBD3F430159B551FDULL>& AlchemistBodyPath2() {
  const char source[] = {'\x42', '\xFD', '\x04', '\x84', '\xBD', '\xB9', '\x84', '\x5A', '\xA1', '\xEE', '\x19', '\xE8', '\xDC', '\xFF', '\xC5', '\x06', '\x1A', '\x51'};
  thread_local EncryptedName<sizeof(source), 0xBD3F430159B551FDULL> name(source);
  return name;
}

// OFFSET: 0x00DCBEC0
// FUNCTION: fn_23071
EncryptedName<10, 0xC7FB6DF18577D34BULL>& BardBodyPath2() {
  const char source[] = {'\xF2', '\x0A', '\xC2', '\x60', '\xAD', '\xD4', '\x22', '\x72', '\xAE', '\xD3'};
  thread_local EncryptedName<sizeof(source), 0xC7FB6DF18577D34BULL> name(source);
  return name;
}

// OFFSET: 0x00DCBF90
// FUNCTION: fn_23072
EncryptedName<10, 0x5B832361BDA77197ULL>& DancerBodyPath2() {
  const char source[] = {'\x2E', '\xDA', '\x6F', '\x4C', '\x3D', '\x9A', '\x28', '\x93', '\x66', '\x71'};
  thread_local EncryptedName<sizeof(source), 0x5B832361BDA77197ULL> name(source);
  return name;
}

// OFFSET: 0x00DCC060
// FUNCTION: fn_23073
EncryptedName<34, 0x2B477F9B5B39B301ULL>& NewPecoCrusaderBodyPath1() {
  const char source[] = {'\xBC', '\x76', '\xFF', '\xBF', '\x5F', '\xA5', '\x82', '\x82', '\xB6', '\x54', '\x85', '\xE7', '\x5B', '\xB3', '\xF3', '\xDE', '\x5D', '\x0E', '\xFC', '\x9D', '\x7F', '\xBB', '\x9D', '\xEE', '\xA8', '\x04', '\xDE', '\xE7', '\x27', '\xBF', '\x8B', '\x9F', '\xF4', '\xB3'};
  thread_local EncryptedName<sizeof(source), 0x2B477F9B5B39B301ULL> name(source);
  return name;
}

// OFFSET: 0x00DCC140
// FUNCTION: fn_23074
EncryptedName<14, 0x2B8725D7E95325F5ULL>& NoviceBodyPath3() {
  const char source[] = {'\x36', '\xEF', '\xE9', '\x51', '\x17', '\xFF', '\xDB', '\xE8', '\x3F', '\x9F', '\xEB', '\x29', '\x0D', '\x25'};
  thread_local EncryptedName<sizeof(source), 0x2B8725D7E95325F5ULL> name(source);
  return name;
}

// OFFSET: 0x00DCC210
// FUNCTION: fn_23075
EncryptedName<10, 0x899D11BB83695B0DULL>& SwordmanBodyPath3() {
  const char source[] = {'\xBD', '\x90', '\xD2', '\x64', '\xE7', '\xA1', '\x56', '\x32', '\xEA', '\x5B'};
  thread_local EncryptedName<sizeof(source), 0x899D11BB83695B0DULL> name(source);
  return name;
}

// OFFSET: 0x00DCC2E0
// FUNCTION: fn_23076
EncryptedName<14, 0xBDADDD87C5A50B6BULL>& MagicianBodyPath3() {
  const char source[] = {'\xD3', '\xBD', '\x1C', '\x38', '\x3C', '\x3A', '\xF1', '\x05', '\xDD', '\xB2', '\x58', '\x7E', '\x60', '\xDD'};
  thread_local EncryptedName<sizeof(source), 0xBDADDD87C5A50B6BULL> name(source);
  return name;
}

// OFFSET: 0x00DCC3B0
// FUNCTION: fn_23077
EncryptedName<10, 0x7989830DE73B9DBDULL>& ArcherBodyPath3() {
  const char source[] = {'\x0C', '\x5E', '\x87', '\x11', '\x51', '\x32', '\x4A', '\xC5', '\x4B', '\x9D'};
  thread_local EncryptedName<sizeof(source), 0x7989830DE73B9DBDULL> name(source);
  return name;
}

// OFFSET: 0x00DCC550
// FUNCTION: fn_23078
EncryptedName<10, 0xA7471985F93F9BDFULL>& MerchantBodyPath3() {
  const char source[] = {'\x64', '\x68', '\xFF', '\x37', '\xD9', '\xA2', '\xB4', '\x67', '\x11', '\x9B'};
  thread_local EncryptedName<sizeof(source), 0xA7471985F93F9BDFULL> name(source);
  return name;
}

// OFFSET: 0x00DCC620
// FUNCTION: fn_23079
EncryptedName<10, 0xF535E561137F138DULL>& ThiefBodyPath4() {
  const char source[] = {'\x38', '\xA6', '\xCA', '\xDC', '\x3D', '\x50', '\x80', '\x40', '\x42', '\x13'};
  thread_local EncryptedName<sizeof(source), 0xF535E561137F138DULL> name(source);
  return name;
}

// OFFSET: 0x00DCC6F0
// FUNCTION: fn_23080
EncryptedName<10, 0xD3E5516F89F161D1ULL>& KnightBodyPath2() {
  const char source[] = {'\x60', '\x83', '\x4A', '\x6E', '\x33', '\xE0', '\x07', '\x68', '\x36', '\x61'};
  thread_local EncryptedName<sizeof(source), 0xD3E5516F89F161D1ULL> name(source);
  return name;
}

// OFFSET: 0x00DCC7C0
// FUNCTION: fn_23081
EncryptedName<18, 0x45E3F103CBB5F157ULL>& PriestBodyPath3() {
  const char source[] = {'\x90', '\x30', '\x0D', '\x65', '\xBE', '\x4B', '\x25', '\xEB', '\x0B', '\x36', '\x74', '\x73', '\xAD', '\x4C', '\x59', '\x83', '\xF9', '\xF1'};
  thread_local EncryptedName<sizeof(source), 0x45E3F103CBB5F157ULL> name(source);
  return name;
}

// OFFSET: 0x00DCC890
// FUNCTION: fn_23082
EncryptedName<14, 0x07112F85A39F4FABULL>& WizardBodyPath3() {
  const char source[] = {'\x6B', '\xE8', '\x5F', '\x59', '\x30', '\xCA', '\x4D', '\xC7', '\x0C', '\x8F', '\x65', '\x16', '\x60', '\x2F'};
  thread_local EncryptedName<sizeof(source), 0x07112F85A39F4FABULL> name(source);
  return name;
}

// OFFSET: 0x00DCC960
// FUNCTION: fn_23083
EncryptedName<14, 0x2D33791F67AD47DFULL>& BlacksmithBodyPath2() {
  const char source[] = {'\x1E', '\xE1', '\x6E', '\xD1', '\xAF', '\x81', '\x6F', '\xEC', '\x79', '\x84', '\x1B', '\xD7', '\xE7', '\x79'};
  thread_local EncryptedName<sizeof(source), 0x2D33791F67AD47DFULL> name(source);
  return name;
}

// OFFSET: 0x00DCCA30
// FUNCTION: fn_23084
EncryptedName<10, 0x936F3D2FFDFD87F5ULL>& HunterBodyPath2() {
  const char source[] = {'\x32', '\x62', '\x38', '\x30', '\x73', '\xFA', '\x8A', '\x56', '\x38', '\x87'};
  thread_local EncryptedName<sizeof(source), 0x936F3D2FFDFD87F5ULL> name(source);
  return name;
}

// OFFSET: 0x00DCCB00
// FUNCTION: fn_23085
EncryptedName<14, 0x2F2701A7E56D3D89ULL>& AssassinBodyPath3() {
  const char source[] = {'\x37', '\xD3', '\xD1', '\x59', '\x1A', '\xC4', '\x7B', '\x91', '\x67', '\x81', '\xD1', '\x58', '\x62', '\x01'};
  thread_local EncryptedName<sizeof(source), 0x2F2701A7E56D3D89ULL> name(source);
  return name;
}

// OFFSET: 0x00DCCBD0
// FUNCTION: fn_23086
EncryptedName<28, 0x6B3301E1A3079125ULL>& PecoKnightBodyPath2() {
  const char source[] = {'\xE3', '\x75', '\xC3', '\x79', '\x27', '\xE5', '\xF7', '\xB1', '\x7A', '\x20', '\xE5', '\x18', '\x06', '\x5D', '\xF5', '\x8F', '\xE1', '\x4B', '\xC1', '\x47', '\x25', '\xDB', '\x6C', '\xDA', '\xC7', '\x2A', '\xE0', '\xA3'};
  thread_local EncryptedName<sizeof(source), 0x6B3301E1A3079125ULL> name(source);
  return name;
}

// OFFSET: 0x00DCCCB0
// FUNCTION: fn_23087
EncryptedName<22, 0x71B7691737C5A1AFULL>& CrusaderBodyPath2() {
  const char source[] = {'\x6A', '\x08', '\x72', '\xD0', '\xAB', '\xD5', '\x77', '\xBD', '\x1B', '\x54', '\x99', '\xF2', '\xBE', '\xDE', '\x50', '\xCD', '\x13', '\x61', '\x09', '\x83', '\xE2', '\x69'};
  thread_local EncryptedName<sizeof(source), 0x71B7691737C5A1AFULL> name(source);
  return name;
}

// OFFSET: 0x00DCCD80
// FUNCTION: fn_23088
EncryptedName<10, 0xC39FB559A17D3963ULL>& MonkBodyPath3() {
  const char source[] = {'\xDB', '\xC0', '\xB8', '\x08', '\x05', '\x0D', '\x66', '\x06', '\xCA', '\x39'};
  thread_local EncryptedName<sizeof(source), 0xC39FB559A17D3963ULL> name(source);
  return name;
}

// OFFSET: 0x00DCCE50
// FUNCTION: fn_23089
EncryptedName<14, 0xFF7D83CB791339D9ULL>& SageBodyPath3() {
  const char source[] = {'\x65', '\x85', '\xD3', '\xB5', '\x0A', '\x75', '\x21', '\x43', '\x65', '\xF9', '\xDF', '\xB8', '\x3D', '\x83'};
  thread_local EncryptedName<sizeof(source), 0xFF7D83CB791339D9ULL> name(source);
  return name;
}

// OFFSET: 0x00DCCF20
// FUNCTION: fn_23090
EncryptedName<10, 0xED5143E7C1B39315ULL>& RogueBodyPath3() {
  const char source[] = {'\xA2', '\x5D', '\x02', '\x16', '\xBB', '\xF4', '\x9F', '\x5C', '\xC2', '\x93'};
  thread_local EncryptedName<sizeof(source), 0xED5143E7C1B39315ULL> name(source);
  return name;
}

// OFFSET: 0x00DCCFF0
// FUNCTION: fn_23091
EncryptedName<18, 0x393D916561BDC391ULL>& AlchemistBodyPath3() {
  const char source[] = {'\x2E', '\x6F', '\x0C', '\xBC', '\xD9', '\x6B', '\x86', '\xDE', '\xCD', '\x7C', '\x11', '\xD0', '\xB8', '\x2D', '\xC7', '\x82', '\x76', '\xC3'};
  thread_local EncryptedName<sizeof(source), 0x393D916561BDC391ULL> name(source);
  return name;
}

// OFFSET: 0x00DCD0C0
// FUNCTION: fn_23092
EncryptedName<10, 0x31956551B311FD95ULL>& BardBodyPath3() {
  const char source[] = {'\x2C', '\x24', '\xA4', '\x56', '\x0D', '\xDC', '\x4C', '\x84', '\x70', '\xFD'};
  thread_local EncryptedName<sizeof(source), 0x31956551B311FD95ULL> name(source);
  return name;
}

// OFFSET: 0x00DCD190
// FUNCTION: fn_23093
EncryptedName<10, 0x8741152D8FB34B4FULL>& DancerBodyPath3() {
  const char source[] = {'\xF6', '\xE0', '\x7B', '\x7E', '\x71', '\xAC', '\xEA', '\x4F', '\xBE', '\x4B'};
  thread_local EncryptedName<sizeof(source), 0x8741152D8FB34B4FULL> name(source);
  return name;
}

// OFFSET: 0x00DCD260
// FUNCTION: fn_23094
EncryptedName<34, 0x955D4DF94DE9BB07ULL>& OldPecoCrusaderBodyPath1() {
  const char source[] = {'\xB6', '\x03', '\x2F', '\xA9', '\x3D', '\x97', '\x98', '\x3C', '\xB0', '\x5C', '\x55', '\xF1', '\x39', '\x81', '\xE9', '\x60', '\x5B', '\x0A', '\x51', '\x8B', '\x1D', '\x89', '\x87', '\x50', '\xAE', '\x0C', '\x0E', '\xF1', '\x45', '\x8D', '\x91', '\x21', '\xF2', '\xBB'};
  thread_local EncryptedName<sizeof(source), 0x955D4DF94DE9BB07ULL> name(source);
  return name;
}

// OFFSET: 0x00DCD340
// FUNCTION: fn_23095
EncryptedName<14, 0xC399B7091589B50BULL>& NoviceBodyPath4() {
  const char source[] = {'\xC8', '\x7F', '\x33', '\xAD', '\xC9', '\x6D', '\xC5', '\x00', '\xC1', '\x0F', '\x31', '\xD5', '\xD3', '\xB7'};
  thread_local EncryptedName<sizeof(source), 0xC399B7091589B50BULL> name(source);
  return name;
}

// OFFSET: 0x00DCD410
// FUNCTION: fn_23096
EncryptedName<18, 0xB739073FE921AD6FULL>& TaekwonBodyPath1() {
  const char source[] = {'\xAA', '\x6F', '\x90', '\x2E', '\x83', '\xD5', '\x8A', '\x55', '\x33', '\x68', '\xE3', '\x58', '\xF8', '\xBB', '\xEB', '\x04', '\x8D', '\xAD'};
  thread_local EncryptedName<sizeof(source), 0xB739073FE921AD6FULL> name(source);
  return name;
}

// OFFSET: 0x00DCD4E0
// FUNCTION: fn_23097
EncryptedName<18, 0x17FFC98B398FAF61ULL>& TaekwonBodyPath2() {
  const char source[] = {'\xA4', '\x6D', '\x3E', '\xFE', '\x37', '\x1B', '\x4C', '\xF5', '\x3D', '\x6A', '\x4D', '\x88', '\x4C', '\x75', '\x2D', '\xA4', '\x83', '\xAF'};
  thread_local EncryptedName<sizeof(source), 0x17FFC98B398FAF61ULL> name(source);
  return name;
}

// OFFSET: 0x00DCD5B0
// FUNCTION: fn_23098
EncryptedName<18, 0x9B5B959B954D9333ULL>& TaekwonBodyPath3() {
  const char source[] = {'\xF6', '\x51', '\xFC', '\x52', '\x27', '\x47', '\xE8', '\x79', '\x6F', '\x56', '\x8F', '\x24', '\x5C', '\x29', '\x89', '\x28', '\xD1', '\x93'};
  thread_local EncryptedName<sizeof(source), 0x9B5B959B954D9333ULL> name(source);
  return name;
}

// OFFSET: 0x00DCD680
// FUNCTION: fn_23099
EncryptedName<14, 0xAFA5617F178B6185ULL>& WizardBodyPath4() {
  const char source[] = {'\x45', '\xC6', '\x4B', '\xED', '\xCA', '\x84', '\xF9', '\x6F', '\x22', '\xA1', '\x71', '\xA2', '\x9A', '\x61'};
  thread_local EncryptedName<sizeof(source), 0xAFA5617F178B6185ULL> name(source);
  return name;
}

// OFFSET: 0x00DCD750
// FUNCTION: fn_23100
EncryptedName<14, 0x078143534999254DULL>& AcolyteBodyPath3() {
  const char source[] = {'\xF1', '\x9F', '\x58', '\xBE', '\x93', '\x99', '\xDD', '\xBB', '\xF7', '\xE4', '\x6E', '\x89', '\x89', '\x43'};
  thread_local EncryptedName<sizeof(source), 0x078143534999254DULL> name(source);
  return name;
}

// OFFSET: 0x00DCD820
// FUNCTION: fn_23101
EncryptedName<10, 0x3F5315DFE32B4FAFULL>& KnightBodyPath3() {
  const char source[] = {'\x1E', '\xAD', '\x90', '\x04', '\x83', '\xA4', '\xB1', '\x84', '\x48', '\x4F'};
  thread_local EncryptedName<sizeof(source), 0x3F5315DFE32B4FAFULL> name(source);
  return name;
}

// OFFSET: 0x00DCD8F0
// FUNCTION: fn_23102
EncryptedName<14, 0xE50B2935073DD3E9ULL>& SageBodyPath4() {
  const char source[] = {'\x55', '\x6F', '\xFD', '\xCB', '\xF4', '\xDF', '\x57', '\x59', '\x55', '\x13', '\xF1', '\xC6', '\xC3', '\x29'};
  thread_local EncryptedName<sizeof(source), 0xE50B2935073DD3E9ULL> name(source);
  return name;
}

// OFFSET: 0x00DCD9C0
// FUNCTION: fn_23103
EncryptedName<10, 0x7D750DADBDE9C3F1ULL>& KnightBodyPath4() {
  const char source[] = {'\x40', '\x21', '\x52', '\x5A', '\xF1', '\xBC', '\x97', '\xC6', '\x16', '\xC3'};
  thread_local EncryptedName<sizeof(source), 0x7D750DADBDE9C3F1ULL> name(source);
  return name;
}

// OFFSET: 0x00DCDA90
// FUNCTION: fn_23104
EncryptedName<10, 0x33832D4D3FDFBDBFULL>& KnightBodyPath5() {
  const char source[] = {'\x0E', '\x5F', '\x64', '\xD8', '\x11', '\x9C', '\x61', '\x88', '\x58', '\xBD'};
  thread_local EncryptedName<sizeof(source), 0x33832D4D3FDFBDBFULL> name(source);
  return name;
}

// OFFSET: 0x00DCDB60
// FUNCTION: fn_23105
EncryptedName<28, 0x1BF915E1F37DFDFDULL>& PecoKnightBodyPath3() {
  const char source[] = {'\x3B', '\x19', '\xB9', '\x29', '\x27', '\xF1', '\x3D', '\xC1', '\xA2', '\x4C', '\x9F', '\x48', '\x06', '\x49', '\x3F', '\xFF', '\x39', '\x27', '\xBB', '\x17', '\x25', '\xCF', '\xA6', '\xAA', '\x1F', '\x46', '\x9A', '\xF3'};
  thread_local EncryptedName<sizeof(source), 0x1BF915E1F37DFDFDULL> name(source);
  return name;
}

// OFFSET: 0x00DCDC40
// FUNCTION: fn_23106
EncryptedName<28, 0x01FF9313695DD9DFULL>& PecoKnightBodyPath4() {
  const char source[] = {'\x19', '\x3D', '\x99', '\xB3', '\xD5', '\x77', '\x3B', '\xDB', '\x80', '\x68', '\xBF', '\xD2', '\xF4', '\xCF', '\x39', '\xE5', '\x1B', '\x03', '\x9B', '\x8D', '\xD7', '\x49', '\xA0', '\xB0', '\x3D', '\x62', '\xBA', '\x69'};
  thread_local EncryptedName<sizeof(source), 0x01FF9313695DD9DFULL> name(source);
  return name;
}

// OFFSET: 0x00DCDD20
// FUNCTION: fn_23107
EncryptedName<28, 0x6DED3DD1CF75AD9BULL>& PecoKnightBodyPath5() {
  const char source[] = {'\x5D', '\x49', '\xB1', '\x15', '\x17', '\xD9', '\x29', '\xB7', '\xC4', '\x1C', '\x97', '\x74', '\x36', '\x61', '\x2B', '\x89', '\x5F', '\x77', '\xB3', '\x2B', '\x15', '\xE7', '\xB2', '\xDC', '\x79', '\x16', '\x92', '\xCF'};
  thread_local EncryptedName<sizeof(source), 0x6DED3DD1CF75AD9BULL> name(source);
  return name;
}

// OFFSET: 0x00DCDE00
// FUNCTION: fn_23108
EncryptedName<28, 0x49C3759B77DBD3C1ULL>& PecoKnightBodyPath6() {
  const char source[] = {'\x07', '\x37', '\x1F', '\xAD', '\x5D', '\x91', '\x07', '\x93', '\x9E', '\x62', '\x39', '\xCC', '\x7C', '\x29', '\x05', '\xAD', '\x05', '\x09', '\x1D', '\x93', '\x5F', '\xAF', '\x9C', '\xF8', '\x23', '\x68', '\x3C', '\x77'};
  thread_local EncryptedName<sizeof(source), 0x49C3759B77DBD3C1ULL> name(source);
  return name;
}

// OFFSET: 0x00DCDEE0
// FUNCTION: fn_23109
EncryptedName<28, 0xD1538513DF975701ULL>& PecoKnightBodyPath7() {
  const char source[] = {'\xC7', '\xB3', '\x53', '\x05', '\xD5', '\x61', '\x97', '\x0B', '\x5E', '\xE6', '\x75', '\x64', '\xF4', '\xD9', '\x95', '\x35', '\xC5', '\x8D', '\x51', '\x3B', '\xD7', '\x5F', '\x0C', '\x60', '\xE3', '\xEC', '\x70', '\xDF'};
  thread_local EncryptedName<sizeof(source), 0xD1538513DF975701ULL> name(source);
  return name;
}

// OFFSET: 0x00DCDFC0
// FUNCTION: fn_23110
EncryptedName<28, 0x91E147EBBD47C39FULL>& PecoKnightBodyPath8() {
  const char source[] = {'\x59', '\x27', '\x83', '\x67', '\x2D', '\xA3', '\x25', '\x4B', '\xC0', '\x72', '\xA5', '\x06', '\x0C', '\x1B', '\x27', '\x75', '\x5B', '\x19', '\x81', '\x59', '\x2F', '\x9D', '\xBE', '\x20', '\x7D', '\x78', '\xA0', '\xBD'};
  thread_local EncryptedName<sizeof(source), 0x91E147EBBD47C39FULL> name(source);
  return name;
}

// OFFSET: 0x00DCE0A0
// FUNCTION: fn_23111
EncryptedName<28, 0x4F1791FB1503C713ULL>& PecoKnightBodyPath9() {
  const char source[] = {'\xD5', '\x23', '\xC7', '\xCF', '\x3D', '\x75', '\xD3', '\x95', '\x4C', '\x76', '\xE1', '\xAE', '\x1C', '\xCD', '\xD1', '\xAB', '\xD7', '\x1D', '\xC5', '\xF1', '\x3F', '\x4B', '\x48', '\xFE', '\xF1', '\x7C', '\xE4', '\x15'};
  thread_local EncryptedName<sizeof(source), 0x4F1791FB1503C713ULL> name(source);
  return name;
}

// OFFSET: 0x00DCE180
// FUNCTION: fn_23112
EncryptedName<28, 0xA1A55B4D1597EDD3ULL>& PecoKnightBodyPath10() {
  const char source[] = {'\x15', '\x09', '\x53', '\xCF', '\x8B', '\xBF', '\x61', '\x7B', '\x8C', '\x5C', '\x75', '\xAE', '\xAA', '\x07', '\x63', '\x45', '\x17', '\x37', '\x51', '\xF1', '\x89', '\x81', '\xFA', '\x10', '\x31', '\x56', '\x70', '\x15'};
  thread_local EncryptedName<sizeof(source), 0xA1A55B4D1597EDD3ULL> name(source);
  return name;
}

// OFFSET: 0x00DCE260
// FUNCTION: fn_23113
EncryptedName<28, 0x0753A7E1EFA9E139ULL>& PecoKnightBodyPath11() {
  const char source[] = {'\xFF', '\x05', '\x6D', '\x35', '\x27', '\x43', '\x97', '\xDD', '\x66', '\x50', '\x4B', '\x54', '\x06', '\xFB', '\x95', '\xE3', '\xFD', '\x3B', '\x6F', '\x0B', '\x25', '\x7D', '\x0C', '\xB6', '\xDB', '\x5A', '\x4E', '\xEF'};
  thread_local EncryptedName<sizeof(source), 0x0753A7E1EFA9E139ULL> name(source);
  return name;
}

// OFFSET: 0x00DCE340
// FUNCTION: fn_23114
EncryptedName<28, 0x7FC1015DFFCF6D19ULL>& PecoKnightBodyPath12() {
  const char source[] = {'\xDF', '\x89', '\x0B', '\x25', '\x9B', '\xE5', '\x05', '\xA5', '\x46', '\xDC', '\x2D', '\x44', '\xBA', '\x5D', '\x07', '\x9B', '\xDD', '\xB7', '\x09', '\x1B', '\x99', '\xDB', '\x9E', '\xCE', '\xFB', '\xD6', '\x28', '\xFF'};
  thread_local EncryptedName<sizeof(source), 0x7FC1015DFFCF6D19ULL> name(source);
  return name;
}

// OFFSET: 0x00DCE420
// FUNCTION: fn_23115
EncryptedName<28, 0xCB3FBD4183DB8323ULL>& PecoKnightBodyPath13() {
  const char source[] = {'\xE5', '\x67', '\x1F', '\x59', '\x87', '\x59', '\xFB', '\x11', '\x7C', '\x32', '\x39', '\x38', '\xA6', '\xE1', '\xF9', '\x2F', '\xE7', '\x59', '\x1D', '\x67', '\x85', '\x67', '\x60', '\x7A', '\xC1', '\x38', '\x3C', '\x83'};
  thread_local EncryptedName<sizeof(source), 0xCB3FBD4183DB8323ULL> name(source);
  return name;
}

// OFFSET: 0x00DCE500
// FUNCTION: fn_23116
EncryptedName<18, 0x7FCD77F9C7CBD14DULL>& PriestBodyPath4() {
  const char source[] = {'\x8A', '\x10', '\x73', '\x69', '\x44', '\xCD', '\x0B', '\xD1', '\x11', '\x16', '\x0A', '\x7F', '\x57', '\xCA', '\x77', '\xB9', '\xE3', '\xD1'};
  thread_local EncryptedName<sizeof(source), 0x7FCD77F9C7CBD14DULL> name(source);
  return name;
}

// OFFSET: 0x00DCE5D0
// FUNCTION: fn_23117
EncryptedName<18, 0x5B8F49AF09390D1BULL>& PriestBodyPath5() {
  const char source[] = {'\xDC', '\xCC', '\x81', '\xA7', '\x12', '\xF3', '\x49', '\xF5', '\x47', '\xCA', '\xF8', '\xB1', '\x01', '\xF4', '\x35', '\x9D', '\xB5', '\x0D'};
  thread_local EncryptedName<sizeof(source), 0x5B8F49AF09390D1BULL> name(source);
  return name;
}

// OFFSET: 0x00DCE6A0
// FUNCTION: fn_23118
EncryptedName<14, 0x3B915119EFD523EFULL>& WizardBodyPath5() {
  const char source[] = {'\x2F', '\x84', '\x15', '\x15', '\xAC', '\xB4', '\xCD', '\xFB', '\x48', '\xE3', '\x2F', '\x5A', '\xFC', '\x51'};
  thread_local EncryptedName<sizeof(source), 0x3B915119EFD523EFULL> name(source);
  return name;
}

// OFFSET: 0x00DCE770
// FUNCTION: fn_23119
EncryptedName<14, 0x474DC581D5E98357ULL>& WizardBodyPath6() {
  const char source[] = {'\x97', '\x24', '\x29', '\x2F', '\x34', '\x20', '\x11', '\x87', '\xF0', '\x43', '\x13', '\x60', '\x64', '\xC5'};
  thread_local EncryptedName<sizeof(source), 0x474DC581D5E98357ULL> name(source);
  return name;
}

// OFFSET: 0x00DCE840
// FUNCTION: fn_23120
EncryptedName<14, 0x3F557BE333F95F6DULL>& BlacksmithBodyPath3() {
  const char source[] = {'\xAC', '\xF9', '\x3A', '\x85', '\x53', '\x83', '\x09', '\xFE', '\xCB', '\x9C', '\x4F', '\x83', '\x1B', '\x7B'};
  thread_local EncryptedName<sizeof(source), 0x3F557BE333F95F6DULL> name(source);
  return name;
}

// OFFSET: 0x00DCE910
// FUNCTION: fn_23121
EncryptedName<14, 0x8B3FEB5B8BD5C3C3ULL>& BlacksmithBodyPath4() {
  const char source[] = {'\x02', '\x65', '\x16', '\x3D', '\xEB', '\x13', '\x63', '\x4A', '\x65', '\x00', '\x63', '\x3B', '\xA3', '\xEB'};
  thread_local EncryptedName<sizeof(source), 0x8B3FEB5B8BD5C3C3ULL> name(source);
  return name;
}

// OFFSET: 0x00DCE9E0
// FUNCTION: fn_23122
EncryptedName<18, 0x0BEB63A5C905B3A7ULL>& MadogearBodyPath1() {
  const char source[] = {'\x1F', '\x05', '\xB0', '\x7C', '\x14', '\x81', '\x55', '\xE5', '\xFB', '\x0B', '\xB3', '\x7C', '\x10', '\xD2', '\x09', '\xB5', '\x49', '\xB3'};
  thread_local EncryptedName<sizeof(source), 0x0BEB63A5C905B3A7ULL> name(source);
  return name;
}

// OFFSET: 0x00DCEAB0
// FUNCTION: fn_23123
EncryptedName<18, 0x078333BB9725B7C9ULL>& MadogearBodyPath2() {
  const char source[] = {'\x71', '\x01', '\x90', '\x22', '\x0A', '\xD1', '\x3D', '\xE9', '\x95', '\x0F', '\x93', '\x22', '\x0E', '\x82', '\x61', '\xB9', '\x27', '\xB7'};
  thread_local EncryptedName<sizeof(source), 0x078333BB9725B7C9ULL> name(source);
  return name;
}

// OFFSET: 0x00DCEB80
// FUNCTION: fn_23124
EncryptedName<18, 0x8F1B5BFD5F6175B5ULL>& MadogearBodyPath3() {
  const char source[] = {'\x0D', '\xC3', '\xD4', '\xEA', '\x4C', '\xB9', '\xA5', '\x61', '\xE9', '\xCD', '\xD7', '\xEA', '\x48', '\xEA', '\xF9', '\x31', '\x5B', '\x75'};
  thread_local EncryptedName<sizeof(source), 0x8F1B5BFD5F6175B5ULL> name(source);
  return name;
}

// OFFSET: 0x00DCEC50
// FUNCTION: fn_23125
EncryptedName<10, 0x69FD1D9733D3CFC1ULL>& HunterBodyPath3() {
  const char source[] = {'\x06', '\x2A', '\x16', '\xFE', '\xCB', '\xDA', '\x18', '\xAC', '\x0C', '\xCF'};
  thread_local EncryptedName<sizeof(source), 0x69FD1D9733D3CFC1ULL> name(source);
  return name;
}

// OFFSET: 0x00DCED20
// FUNCTION: fn_23126
EncryptedName<10, 0xF5657D1F3DE5CDC7ULL>& HunterBodyPath4() {
  const char source[] = {'\x00', '\x28', '\x20', '\xF0', '\x43', '\xBA', '\x80', '\x30', '\x0A', '\xCD'};
  thread_local EncryptedName<sizeof(source), 0xF5657D1F3DE5CDC7ULL> name(source);
  return name;
}

// OFFSET: 0x00DCEDF0
// FUNCTION: fn_23127
EncryptedName<22, 0x8B65CD61012B5931ULL>& RangerWolfBodyPath1() {
  const char source[] = {'\x86', '\xE0', '\xEB', '\xCF', '\xA0', '\x63', '\xD1', '\x4A', '\x85', '\xB2', '\x77', '\xB6', '\xD8', '\x0D', '\xAB', '\x4A', '\x9F', '\xED', '\xEA', '\xB5', '\x8A', '\xCD'};
  thread_local EncryptedName<sizeof(source), 0x8B65CD61012B5931ULL> name(source);
  return name;
}

// OFFSET: 0x00DCEEC0
// FUNCTION: fn_23128
EncryptedName<22, 0xF7530137154D8FE5ULL>& RangerWolfBodyPath2() {
  const char source[] = {'\x52', '\x36', '\x8D', '\xDB', '\xF6', '\xAF', '\xE7', '\x36', '\x51', '\x64', '\x11', '\xA2', '\x8E', '\xC1', '\x9D', '\x36', '\x4B', '\x3B', '\x8C', '\xA1', '\xDC', '\x01'};
  thread_local EncryptedName<sizeof(source), 0xF7530137154D8FE5ULL> name(source);
  return name;
}

// OFFSET: 0x00DCEF90
// FUNCTION: fn_23129
EncryptedName<22, 0x61FF87ED7BF39BE1ULL>& RangerWolfBodyPath3() {
  const char source[] = {'\x56', '\x22', '\x33', '\xB5', '\x2C', '\x29', '\x4B', '\xA0', '\x55', '\x70', '\xAF', '\xCC', '\x54', '\x47', '\x31', '\xA0', '\x4F', '\x2F', '\x32', '\xCF', '\x06', '\x87'};
  thread_local EncryptedName<sizeof(source), 0x61FF87ED7BF39BE1ULL> name(source);
  return name;
}

// OFFSET: 0x00DCF060
// FUNCTION: fn_23130
EncryptedName<14, 0x7D993F650D2785FFULL>& AssassinBodyPath4() {
  const char source[] = {'\x41', '\x6B', '\x9B', '\xB1', '\xD8', '\xFA', '\xC5', '\xC3', '\x11', '\x39', '\x9B', '\xB0', '\xA0', '\x3F'};
  thread_local EncryptedName<sizeof(source), 0x7D993F650D2785FFULL> name(source);
  return name;
}

// OFFSET: 0x00DCF130
// FUNCTION: fn_23131
EncryptedName<14, 0x2D394DBD0F332733ULL>& AssassinBodyPath5() {
  const char source[] = {'\x8D', '\xC9', '\x8F', '\xB3', '\x00', '\x88', '\x65', '\x93', '\xDD', '\x9B', '\x8F', '\xB2', '\x78', '\x4D'};
  thread_local EncryptedName<sizeof(source), 0x2D394DBD0F332733ULL> name(source);
  return name;
}

// OFFSET: 0x00DCF200
// FUNCTION: fn_23132
EncryptedName<22, 0x151F77EBEBBB2147ULL>& CrusaderBodyPath3() {
  const char source[] = {'\x82', '\x88', '\x0C', '\x0C', '\x57', '\xCB', '\xDF', '\xD9', '\xF3', '\xD4', '\xE7', '\x2E', '\x42', '\xC0', '\xF8', '\xA9', '\xFB', '\xE1', '\x77', '\x5F', '\x1E', '\x77'};
  thread_local EncryptedName<sizeof(source), 0x151F77EBEBBB2147ULL> name(source);
  return name;
}

// OFFSET: 0x00DCF2D0
// FUNCTION: fn_23133
EncryptedName<22, 0xC7912587FFF31B71ULL>& CrusaderBodyPath4() {
  const char source[] = {'\xB4', '\xB2', '\x44', '\x18', '\x3B', '\x99', '\x51', '\x0B', '\xC5', '\xEE', '\xAF', '\x3A', '\x2E', '\x92', '\x76', '\x7B', '\xCD', '\xDB', '\x3F', '\x4B', '\x72', '\x25'};
  thread_local EncryptedName<sizeof(source), 0xC7912587FFF31B71ULL> name(source);
  return name;
}

// OFFSET: 0x00DCF3A0
// FUNCTION: fn_23134
EncryptedName<34, 0x69D345DB950F336DULL>& NewPecoCrusaderBodyPath2() {
  const char source[] = {'\xD0', '\xF6', '\xC9', '\x71', '\x1F', '\x9F', '\x16', '\xC0', '\xDA', '\xD4', '\xB3', '\x29', '\x1B', '\x89', '\x67', '\x9C', '\x31', '\x8E', '\xCA', '\x53', '\x3F', '\x81', '\x09', '\xAC', '\xC4', '\x84', '\xE8', '\x29', '\x67', '\x85', '\x1F', '\xDD', '\x98', '\x33'};
  thread_local EncryptedName<sizeof(source), 0x69D345DB950F336DULL> name(source);
  return name;
}

// OFFSET: 0x00DCF480
// FUNCTION: fn_23135
EncryptedName<34, 0x9B17C9EFDBB39F2FULL>& NewPecoCrusaderBodyPath3() {
  const char source[] = {'\x92', '\x5A', '\x75', '\x3F', '\x2B', '\x13', '\xD2', '\x32', '\x98', '\x78', '\x0F', '\x67', '\x2F', '\x05', '\xA3', '\x6E', '\x73', '\x22', '\x76', '\x1D', '\x0B', '\x0D', '\xCD', '\x5E', '\x86', '\x28', '\x54', '\x67', '\x53', '\x09', '\xDB', '\x2F', '\xDA', '\x9F'};
  thread_local EncryptedName<sizeof(source), 0x9B17C9EFDBB39F2FULL> name(source);
  return name;
}

// OFFSET: 0x00DCF560
// FUNCTION: fn_23136
EncryptedName<34, 0x2D3B15774149D5DDULL>& NewPecoCrusaderBodyPath4() {
  const char source[] = {'\x60', '\x10', '\x8F', '\xA5', '\xB3', '\xCF', '\xFE', '\x84', '\x6A', '\x32', '\xF5', '\xFD', '\xB7', '\xD9', '\x8F', '\xD8', '\x81', '\x68', '\x8C', '\x87', '\x93', '\xD1', '\xE1', '\xE8', '\x74', '\x62', '\xAE', '\xFD', '\xCB', '\xD5', '\xF7', '\x99', '\x28', '\xD5'};
  thread_local EncryptedName<sizeof(source), 0x2D3B15774149D5DDULL> name(source);
  return name;
}

// OFFSET: 0x00DCF640
// FUNCTION: fn_23137
EncryptedName<10, 0xD98F03A52D35636DULL>& MonkBodyPath4() {
  const char source[] = {'\xD5', '\x9A', '\xF0', '\x84', '\xF9', '\xBB', '\x76', '\x1C', '\xC4', '\x63'};
  thread_local EncryptedName<sizeof(source), 0xD98F03A52D35636DULL> name(source);
  return name;
}

// OFFSET: 0x00DCF710
// FUNCTION: fn_23138
EncryptedName<10, 0x0F0F8B6B8F9D19D1ULL>& MonkBodyPath5() {
  const char source[] = {'\x69', '\xE0', '\x58', '\x26', '\x37', '\x33', '\xF6', '\xCA', '\x78', '\x19'};
  thread_local EncryptedName<sizeof(source), 0x0F0F8B6B8F9D19D1ULL> name(source);
  return name;
}

// OFFSET: 0x00DCF7E0
// FUNCTION: fn_23139
EncryptedName<14, 0xD3755DD95F47A909ULL>& SageBodyPath5() {
  const char source[] = {'\xB5', '\x15', '\x87', '\x93', '\x18', '\xAB', '\x29', '\x6F', '\xB5', '\x69', '\x8B', '\x9E', '\x2F', '\x5D'};
  thread_local EncryptedName<sizeof(source), 0xD3755DD95F47A909ULL> name(source);
  return name;
}

// OFFSET: 0x00DCF8B0
// FUNCTION: fn_23140
EncryptedName<14, 0x31D7C1EDA345E36DULL>& SageBodyPath6() {
  const char source[] = {'\xD1', '\x5F', '\x85', '\x6F', '\x2C', '\x37', '\x8B', '\x8D', '\xD1', '\x23', '\x89', '\x62', '\x1B', '\xC1'};
  thread_local EncryptedName<sizeof(source), 0x31D7C1EDA345E36DULL> name(source);
  return name;
}

// OFFSET: 0x00DCF980
// FUNCTION: fn_23141
EncryptedName<10, 0x3FAF85D995034FD7ULL>& RogueBodyPath4() {
  const char source[] = {'\x60', '\x81', '\xB2', '\x42', '\x85', '\x32', '\x61', '\x8E', '\x00', '\x4F'};
  thread_local EncryptedName<sizeof(source), 0x3FAF85D995034FD7ULL> name(source);
  return name;
}

// OFFSET: 0x00DCFA50
// FUNCTION: fn_23142
EncryptedName<10, 0x490FFB43A12F0DB9ULL>& RogueBodyPath5() {
  const char source[] = {'\x0E', '\xC3', '\x9E', '\x76', '\x1F', '\x4C', '\xC1', '\xF8', '\x6E', '\x0D'};
  thread_local EncryptedName<sizeof(source), 0x490FFB43A12F0DB9ULL> name(source);
  return name;
}

// OFFSET: 0x00DCFB20
// FUNCTION: fn_23143
EncryptedName<18, 0x91651D81F559F971ULL>& AlchemistBodyPath4() {
  const char source[] = {'\xCE', '\x55', '\xE8', '\x28', '\x3D', '\xE7', '\xDE', '\x76', '\x2D', '\x46', '\xF5', '\x44', '\x5C', '\xA1', '\x9F', '\x2A', '\x96', '\xF9'};
  thread_local EncryptedName<sizeof(source), 0x91651D81F559F971ULL> name(source);
  return name;
}

// OFFSET: 0x00DCFBF0
// FUNCTION: fn_23144
EncryptedName<18, 0x73D1674B61DF7B0BULL>& AlchemistBodyPath5() {
  const char source[] = {'\xB4', '\xD7', '\x6E', '\xBC', '\xF7', '\x9D', '\x6A', '\x94', '\x57', '\xC4', '\x73', '\xD0', '\x96', '\xDB', '\x2B', '\xC8', '\xEC', '\x7B'};
  thread_local EncryptedName<sizeof(source), 0x73D1674B61DF7B0BULL> name(source);
  return name;
}

// OFFSET: 0x00DCFCC0
// FUNCTION: fn_23145
EncryptedName<10, 0x2FF3C5036D070127ULL>& DancerBodyPath4() {
  const char source[] = {'\x9E', '\xAA', '\xCF', '\x9C', '\x5F', '\x7C', '\x58', '\xE7', '\xD6', '\x01'};
  thread_local EncryptedName<sizeof(source), 0x2FF3C5036D070127ULL> name(source);
  return name;
}

// OFFSET: 0x00DCFD90
// FUNCTION: fn_23146
EncryptedName<10, 0x17359BF953C367B3ULL>& DancerBodyPath5() {
  const char source[] = {'\x0A', '\xCC', '\x0B', '\xA2', '\xA5', '\x22', '\x9E', '\xDF', '\x42', '\x67'};
  thread_local EncryptedName<sizeof(source), 0x17359BF953C367B3ULL> name(source);
  return name;
}

// OFFSET: 0x00DCFE60
// FUNCTION: fn_23147
EncryptedName<10, 0xC52FB5FBB94FF917ULL>& BardBodyPath4() {
  const char source[] = {'\xAE', '\x20', '\xFA', '\x5C', '\xA7', '\x0C', '\xF6', '\x70', '\xF2', '\xF9'};
  thread_local EncryptedName<sizeof(source), 0xC52FB5FBB94FF917ULL> name(source);
  return name;
}

// OFFSET: 0x00DCFF30
// FUNCTION: fn_23148
EncryptedName<10, 0x93E7A3A1F587CDC5ULL>& BardBodyPath5() {
  const char source[] = {'\x7C', '\x14', '\x32', '\x10', '\xFD', '\x1A', '\x3E', '\x26', '\x20', '\xCD'};
  thread_local EncryptedName<sizeof(source), 0x93E7A3A1F587CDC5ULL> name(source);
  return name;
}

// OFFSET: 0x00DD0000
// FUNCTION: fn_23149
EncryptedName<10, 0x59278F63D79F3DA1ULL>& KnightBodyPath6() {
  const char source[] = {'\x10', '\xDF', '\x24', '\x30', '\x3F', '\x3E', '\xC5', '\xE2', '\x46', '\x3D'};
  thread_local EncryptedName<sizeof(source), 0x59278F63D79F3DA1ULL> name(source);
  return name;
}

// OFFSET: 0x00DD00D0
// FUNCTION: fn_23150
EncryptedName<14, 0x812BE72BCD5707B7ULL>& WizardBodyPath7() {
  const char source[] = {'\x77', '\xA0', '\x97', '\x37', '\x9E', '\x02', '\x77', '\x41', '\x10', '\xC7', '\xAD', '\x78', '\xCE', '\xE7'};
  thread_local EncryptedName<sizeof(source), 0x812BE72BCD5707B7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD01A0
// FUNCTION: fn_23151
EncryptedName<10, 0x3375B1C7DBB5FD55ULL>& HunterBodyPath5() {
  const char source[] = {'\x92', '\x18', '\x70', '\x16', '\x9B', '\x76', '\x90', '\xF6', '\x98', '\xFD'};
  thread_local EncryptedName<sizeof(source), 0x3375B1C7DBB5FD55ULL> name(source);
  return name;
}

// OFFSET: 0x00DD0270
// FUNCTION: fn_23152
EncryptedName<18, 0x1D6D75EDE7CD311FULL>& PriestBodyPath6() {
  const char source[] = {'\xD8', '\xF0', '\x75', '\x49', '\x50', '\xCF', '\xAB', '\xB3', '\x43', '\xF6', '\x0C', '\x5F', '\x43', '\xC8', '\xD7', '\xDB', '\xB1', '\x31'};
  thread_local EncryptedName<sizeof(source), 0x1D6D75EDE7CD311FULL> name(source);
  return name;
}

// OFFSET: 0x00DD0340
// FUNCTION: fn_23153
EncryptedName<14, 0xCBD1F3239BBBEDF7ULL>& BlacksmithBodyPath5() {
  const char source[] = {'\x36', '\x4B', '\x78', '\x2D', '\x93', '\x0B', '\x8D', '\x0A', '\x51', '\x2E', '\x0D', '\x2B', '\xDB', '\xF3'};
  thread_local EncryptedName<sizeof(source), 0xCBD1F3239BBBEDF7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD0410
// FUNCTION: fn_23154
EncryptedName<14, 0x7DBBD74FA36F3BCBULL>& AssassinBodyPath6() {
  const char source[] = {'\x75', '\xD5', '\xD3', '\x1F', '\xF2', '\x12', '\xE7', '\xC3', '\x25', '\x87', '\xD3', '\x1E', '\x8A', '\xD7'};
  thread_local EncryptedName<sizeof(source), 0x7DBBD74FA36F3BCBULL> name(source);
  return name;
}

// OFFSET: 0x00DD04E0
// FUNCTION: fn_23155
EncryptedName<22, 0x4917EB6D71B701ABULL>& CrusaderBodyPath5() {
  const char source[] = {'\x6E', '\xA8', '\x00', '\x96', '\xD1', '\x57', '\xD7', '\x85', '\x1F', '\xF4', '\xEB', '\xB4', '\xC4', '\x5C', '\xF0', '\xF5', '\x17', '\xC1', '\x7B', '\xC5', '\x98', '\xEB'};
  thread_local EncryptedName<sizeof(source), 0x4917EB6D71B701ABULL> name(source);
  return name;
}

// OFFSET: 0x00DD05B0
// FUNCTION: fn_23156
EncryptedName<14, 0xFD4F85BBA7E37D4FULL>& SageBodyPath7() {
  const char source[] = {'\xF3', '\xC1', '\x23', '\x6B', '\x7A', '\x73', '\x13', '\x41', '\xF3', '\xBD', '\x2F', '\x66', '\x4D', '\x85'};
  thread_local EncryptedName<sizeof(source), 0xFD4F85BBA7E37D4FULL> name(source);
  return name;
}

// OFFSET: 0x00DD0680
// FUNCTION: fn_23157
EncryptedName<10, 0x51C57BB9F1DD9B0FULL>& BardBodyPath6() {
  const char source[] = {'\xB6', '\x42', '\x68', '\x14', '\xE5', '\xC2', '\x1C', '\xE4', '\xEA', '\x9B'};
  thread_local EncryptedName<sizeof(source), 0x51C57BB9F1DD9B0FULL> name(source);
  return name;
}

// OFFSET: 0x00DD0750
// FUNCTION: fn_23158
EncryptedName<10, 0xC733B7A333F9F571ULL>& DancerBodyPath6() {
  const char source[] = {'\xC8', '\x5E', '\x31', '\xC2', '\xFF', '\x0E', '\x98', '\x0F', '\x80', '\xF5'};
  thread_local EncryptedName<sizeof(source), 0xC733B7A333F9F571ULL> name(source);
  return name;
}

// OFFSET: 0x00DD0820
// FUNCTION: fn_23159
EncryptedName<10, 0x7D61612937C1592DULL>& MonkBodyPath6() {
  const char source[] = {'\x95', '\xA0', '\x04', '\x9E', '\x75', '\xD9', '\x98', '\xB8', '\x84', '\x59'};
  thread_local EncryptedName<sizeof(source), 0x7D61612937C1592DULL> name(source);
  return name;
}

// OFFSET: 0x00DD08F0
// FUNCTION: fn_23160
EncryptedName<18, 0x8F4B518D839D1581ULL>& AlchemistBodyPath6() {
  const char source[] = {'\x3E', '\xB9', '\x2C', '\x5E', '\x31', '\xAB', '\xF0', '\x68', '\xDD', '\xAA', '\x31', '\x32', '\x50', '\xED', '\xB1', '\x34', '\x66', '\x15'};
  thread_local EncryptedName<sizeof(source), 0x8F4B518D839D1581ULL> name(source);
  return name;
}

// OFFSET: 0x00DD09C0
// FUNCTION: fn_23161
EncryptedName<10, 0xCBD7DDAFDF87F549ULL>& RogueBodyPath6() {
  const char source[] = {'\xFE', '\x3B', '\x36', '\x08', '\xF3', '\x6A', '\x19', '\x7A', '\x9E', '\xF5'};
  thread_local EncryptedName<sizeof(source), 0xCBD7DDAFDF87F549ULL> name(source);
  return name;
}

// OFFSET: 0x00DD0A90
// FUNCTION: fn_23162
EncryptedName<14, 0x39E7F13755252F8FULL>& NoviceBodyPath5() {
  const char source[] = {'\x4C', '\xE5', '\x9F', '\xED', '\xF7', '\x2B', '\xBB', '\xFA', '\x45', '\x95', '\x9D', '\x95', '\xED', '\xF1'};
  thread_local EncryptedName<sizeof(source), 0x39E7F13755252F8FULL> name(source);
  return name;
}

// OFFSET: 0x00DD0B60
// FUNCTION: fn_23163
EncryptedName<14, 0x3D075BD5256F9B6FULL>& NoviceBodyPath6() {
  const char source[] = {'\xAC', '\x51', '\xD5', '\x9D', '\x15', '\x81', '\x5B', '\xFE', '\xA5', '\x21', '\xD7', '\xE5', '\x0F', '\x5B'};
  thread_local EncryptedName<sizeof(source), 0x3D075BD5256F9B6FULL> name(source);
  return name;
}

// OFFSET: 0x00DD0C30
// FUNCTION: fn_23164
EncryptedName<14, 0xEB074B7FBDA3BFD5ULL>& NoviceBodyPath7() {
  const char source[] = {'\x16', '\x75', '\x19', '\x05', '\xBF', '\x91', '\x5B', '\x28', '\x1F', '\x05', '\x1B', '\x7D', '\xA5', '\x4B'};
  thread_local EncryptedName<sizeof(source), 0xEB074B7FBDA3BFD5ULL> name(source);
  return name;
}

// OFFSET: 0x00DD0D00
// FUNCTION: fn_23165
EncryptedName<14, 0x03014749B127914FULL>& NoviceBodyPath8() {
  const char source[] = {'\x8C', '\x5B', '\x9D', '\x09', '\x89', '\x9D', '\x5D', '\xC0', '\x85', '\x2B', '\x9F', '\x71', '\x93', '\x47'};
  thread_local EncryptedName<sizeof(source), 0x03014749B127914FULL> name(source);
  return name;
}

// OFFSET: 0x00DD0DD0
// FUNCTION: fn_23166
EncryptedName<18, 0x251761D7370BA5C3ULL>& PriestBodyPath7() {
  const char source[] = {'\x04', '\x64', '\xB3', '\x99', '\x6A', '\xDB', '\xD1', '\x8B', '\x9F', '\x62', '\xCA', '\x8F', '\x79', '\xDC', '\xAD', '\xE3', '\x6D', '\xA5'};
  thread_local EncryptedName<sizeof(source), 0x251761D7370BA5C3ULL> name(source);
  return name;
}

// OFFSET: 0x00DD0EA0
// FUNCTION: fn_23167
EncryptedName<14, 0x2B034DCBB1812785ULL>& WizardBodyPath8() {
  const char source[] = {'\x45', '\x80', '\x41', '\x4B', '\x7E', '\xA8', '\x5F', '\xEB', '\x22', '\xE7', '\x7B', '\x04', '\x2E', '\x4D'};
  thread_local EncryptedName<sizeof(source), 0x2B034DCBB1812785ULL> name(source);
  return name;
}

// OFFSET: 0x00DD0F70
// FUNCTION: fn_23168
EncryptedName<14, 0x85FB93CF2F494179ULL>& BlacksmithBodyPath6() {
  const char source[] = {'\xB8', '\xE7', '\x8A', '\x99', '\x7F', '\x6B', '\xA7', '\x44', '\xDF', '\x82', '\xFF', '\x9F', '\x37', '\x93'};
  thread_local EncryptedName<sizeof(source), 0x85FB93CF2F494179ULL> name(source);
  return name;
}

// OFFSET: 0x00DD1040
// FUNCTION: fn_23169
EncryptedName<14, 0x894FEDABAFCF45E3ULL>& BlacksmithBodyPath7() {
  const char source[] = {'\x22', '\xE3', '\x0C', '\x19', '\x1B', '\x15', '\x13', '\x48', '\x45', '\x86', '\x79', '\x1F', '\x53', '\xED'};
  thread_local EncryptedName<sizeof(source), 0x894FEDABAFCF45E3ULL> name(source);
  return name;
}

// OFFSET: 0x00DD1110
// FUNCTION: fn_23170
EncryptedName<14, 0x8B03E17DA967E943ULL>& BlacksmithBodyPath8() {
  const char source[] = {'\x82', '\x4F', '\xA4', '\x1F', '\xCD', '\x19', '\x5F', '\x4A', '\xE5', '\x2A', '\xD1', '\x19', '\x85', '\xE1'};
  thread_local EncryptedName<sizeof(source), 0x8B03E17DA967E943ULL> name(source);
  return name;
}

// OFFSET: 0x00DD11E0
// FUNCTION: fn_23171
EncryptedName<10, 0x190B8705C141D1CFULL>& HunterBodyPath6() {
  const char source[] = {'\x08', '\x34', '\x84', '\x0C', '\x59', '\x40', '\xEE', '\xDC', '\x02', '\xD1'};
  thread_local EncryptedName<sizeof(source), 0x190B8705C141D1CFULL> name(source);
  return name;
}

// OFFSET: 0x00DD12B0
// FUNCTION: fn_23172
EncryptedName<10, 0xC98BB32777F94371ULL>& HunterBodyPath7() {
  const char source[] = {'\xB6', '\xA6', '\x3C', '\xBA', '\x7B', '\x74', '\x6E', '\x0C', '\xBC', '\x43'};
  thread_local EncryptedName<sizeof(source), 0xC98BB32777F94371ULL> name(source);
  return name;
}

// OFFSET: 0x00DD1380
// FUNCTION: fn_23173
EncryptedName<10, 0xA5F51F3989DBB99FULL>& KnightBodyPath7() {
  const char source[] = {'\x2E', '\x5B', '\x60', '\x6E', '\x65', '\xAE', '\x17', '\x1E', '\x78', '\xB9'};
  thread_local EncryptedName<sizeof(source), 0xA5F51F3989DBB99FULL> name(source);
  return name;
}

// OFFSET: 0x00DD1450
// FUNCTION: fn_23174
EncryptedName<10, 0xB3D5C99B2B05FFEFULL>& KnightBodyPath8() {
  const char source[] = {'\x5E', '\x1D', '\xBE', '\xCC', '\xC7', '\x78', '\x37', '\x08', '\x08', '\xFF'};
  thread_local EncryptedName<sizeof(source), 0xB3D5C99B2B05FFEFULL> name(source);
  return name;
}

// OFFSET: 0x00DD1520
// FUNCTION: fn_23175
EncryptedName<10, 0xC71BB3CDEB499DD1ULL>& KnightBodyPath9() {
  const char source[] = {'\x60', '\x7F', '\xF2', '\x0C', '\x91', '\x02', '\xF9', '\x7C', '\x36', '\x9D'};
  thread_local EncryptedName<sizeof(source), 0xC71BB3CDEB499DD1ULL> name(source);
  return name;
}

// OFFSET: 0x00DD15F0
// FUNCTION: fn_23176
EncryptedName<22, 0x3939079FF3395587ULL>& CrusaderBodyPath6() {
  const char source[] = {'\x42', '\xFC', '\x8E', '\x14', '\x23', '\xBB', '\xF9', '\xF5', '\x33', '\xA0', '\x65', '\x36', '\x36', '\xB0', '\xDE', '\x85', '\x3B', '\x95', '\xF5', '\x47', '\x6A', '\x07'};
  thread_local EncryptedName<sizeof(source), 0x3939079FF3395587ULL> name(source);
  return name;
}

// OFFSET: 0x00DD16C0
// FUNCTION: fn_23177
EncryptedName<22, 0xCF391B0F8B9D95DBULL>& CrusaderBodyPath7() {
  const char source[] = {'\x1E', '\x3C', '\x2A', '\x6C', '\xB3', '\xA7', '\xF9', '\x03', '\x6F', '\x60', '\xC1', '\x4E', '\xA6', '\xAC', '\xDE', '\x73', '\x67', '\x55', '\x51', '\x3F', '\xFA', '\x1B'};
  thread_local EncryptedName<sizeof(source), 0xCF391B0F8B9D95DBULL> name(source);
  return name;
}

// OFFSET: 0x00DD1790
// FUNCTION: fn_23178
EncryptedName<10, 0x6967633B9D89092BULL>& KnightBodyPath10() {
  const char source[] = {'\x9A', '\xEB', '\x32', '\x7A', '\x67', '\xD2', '\x85', '\xD2', '\xCC', '\x09'};
  thread_local EncryptedName<sizeof(source), 0x6967633B9D89092BULL> name(source);
  return name;
}

// OFFSET: 0x00DD1860
// FUNCTION: fn_23179
EncryptedName<10, 0xAFCD4F69FBDFE157ULL>& KnightBodyPath11() {
  const char source[] = {'\xE6', '\x03', '\x64', '\x1C', '\x35', '\xFE', '\x2F', '\x14', '\xB0', '\xE1'};
  thread_local EncryptedName<sizeof(source), 0xAFCD4F69FBDFE157ULL> name(source);
  return name;
}

// OFFSET: 0x00DD1930
// FUNCTION: fn_23180
EncryptedName<22, 0x85D9CFCF0FBB71ABULL>& CrusaderBodyPath8() {
  const char source[] = {'\x6E', '\xD8', '\x0C', '\xE8', '\x73', '\x73', '\x19', '\x49', '\x1F', '\x84', '\xE7', '\xCA', '\x66', '\x78', '\x3E', '\x39', '\x17', '\xB1', '\x77', '\xBB', '\x3A', '\xCF'};
  thread_local EncryptedName<sizeof(source), 0x85D9CFCF0FBB71ABULL> name(source);
  return name;
}

// OFFSET: 0x00DD1A00
// FUNCTION: fn_23181
EncryptedName<22, 0x795383DD11AF7DA7ULL>& CrusaderBodyPath9() {
  const char source[] = {'\x62', '\xD4', '\x18', '\xF6', '\x61', '\x3F', '\x93', '\xB5', '\x13', '\x88', '\xF3', '\xD4', '\x74', '\x34', '\xB4', '\xC5', '\x1B', '\xBD', '\x63', '\xA5', '\x28', '\x83'};
  thread_local EncryptedName<sizeof(source), 0x795383DD11AF7DA7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD1AD0
// FUNCTION: fn_23182
EncryptedName<22, 0xD9FF511B2BB921E7ULL>& CrusaderBodyPath10() {
  const char source[] = {'\x22', '\x88', '\x0E', '\xCC', '\xA7', '\xED', '\x3F', '\x15', '\x53', '\xD4', '\xE5', '\xEE', '\xB2', '\xE6', '\x18', '\x65', '\x5B', '\xE1', '\x75', '\x9F', '\xEE', '\x51'};
  thread_local EncryptedName<sizeof(source), 0xD9FF511B2BB921E7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD1BA0
// FUNCTION: fn_23183
EncryptedName<16, 0x978B51F50DC5FD25ULL>& KagerouBodyPath1() {
  const char source[] = {'\x4E', '\x9C', '\xA2', '\x68', '\x87', '\x3E', '\xFE', '\xCB', '\x4E', '\x9C', '\xA2', '\x68', '\x87', '\x3E', '\xFE', '\x97'};
  thread_local EncryptedName<sizeof(source), 0x978B51F50DC5FD25ULL> name(source);
  return name;
}

// OFFSET: 0x00DD1C60
// FUNCTION: fn_23184
EncryptedName<12, 0xC757D96143298511ULL>& OboroBodyPath1() {
  const char source[] = {'\x7E', '\xE7', '\x46', '\x31', '\x0E', '\x85', '\x38', '\xA5', '\x7E', '\xF7', '\x46', '\x43'};
  thread_local EncryptedName<sizeof(source), 0xC757D96143298511ULL> name(source);
  return name;
}

// OFFSET: 0x00DD1D30
// FUNCTION: fn_23185
EncryptedName<16, 0xE1F181F923B759E7ULL>& KagerouBodyPath2() {
  const char source[] = {'\x8C', '\x38', '\xD0', '\x46', '\x8B', '\xEE', '\x84', '\xBD', '\x8C', '\x38', '\xD0', '\x46', '\x8B', '\xEE', '\x84', '\xE1'};
  thread_local EncryptedName<sizeof(source), 0xE1F181F923B759E7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD1DF0
// FUNCTION: fn_23186
EncryptedName<12, 0x37D76D8F495113C3ULL>& OboroBodyPath2() {
  const char source[] = {'\xAC', '\x71', '\x3E', '\x3B', '\xE0', '\x31', '\xB8', '\x55', '\xAC', '\x61', '\x3E', '\x49'};
  thread_local EncryptedName<sizeof(source), 0x37D76D8F495113C3ULL> name(source);
  return name;
}

// OFFSET: 0x00DD1EC0
// FUNCTION: fn_23187
EncryptedName<20, 0x7BAD7791F50F5D8DULL>& RebellionBodyPath1() {
  const char source[] = {'\xFF', '\x38', '\x6D', '\x90', '\xFD', '\x1B', '\xC4', '\x14', '\xE3', '\x01', '\x7D', '\x90', '\xF3', '\x12', '\xC1', '\x17', '\xE4', '\x32', '\x61', '\xF5'};
  thread_local EncryptedName<sizeof(source), 0x7BAD7791F50F5D8DULL> name(source);
  return name;
}

// OFFSET: 0x00DD1F90
// FUNCTION: fn_23188
EncryptedName<18, 0x2F1337956DC3CFD5ULL>& SummonerBodyPath1() {
  const char source[] = {'\xA6', '\xBA', '\xAE', '\x00', '\xFA', '\x59', '\x76', '\x5D', '\x89', '\xBC', '\xB6', '\x00', '\xF8', '\x58', '\x7D', '\x4A', '\xA7', '\xCF'};
  thread_local EncryptedName<sizeof(source), 0x2F1337956DC3CFD5ULL> name(source);
  return name;
}

// OFFSET: 0x00DD2060
// FUNCTION: fn_23189
EncryptedName<23, 0xC91F9D493F75FDA3ULL>& SummonerCartBodyPath1() {
  const char source[] = {'\xD0', '\x88', '\x18', '\x52', '\x26', '\xF3', '\x7A', '\xBB', '\xFF', '\x9E', '\x14', '\x4D', '\x3D', '\xC2', '\x6C', '\xBC', '\xCE', '\x90', '\x1A', '\x51', '\x2C', '\xEF', '\x1F'};
  thread_local EncryptedName<sizeof(source), 0xC91F9D493F75FDA3ULL> name(source);
  return name;
}

// OFFSET: 0x00DD2140
// FUNCTION: fn_23190
EncryptedName<18, 0x1F01A13D8D915327ULL>& SummonerBodyPath2() {
  const char source[] = {'\x54', '\x26', '\xFC', '\xE0', '\x52', '\xCF', '\x64', '\x6D', '\x7B', '\x20', '\xE4', '\xE0', '\x50', '\xCE', '\x6F', '\x7A', '\x55', '\x53'};
  thread_local EncryptedName<sizeof(source), 0x1F01A13D8D915327ULL> name(source);
  return name;
}

// OFFSET: 0x00DD2210
// FUNCTION: fn_23191
EncryptedName<23, 0xF33D3D3B0BD9C53DULL>& SummonerCartBodyPath2() {
  const char source[] = {'\x4E', '\xB0', '\xB4', '\x66', '\x54', '\x53', '\x58', '\x81', '\x61', '\xA6', '\xB8', '\x79', '\x4F', '\x62', '\x4E', '\x86', '\x50', '\xA8', '\xB6', '\x65', '\x5E', '\x4F', '\x3D'};
  thread_local EncryptedName<sizeof(source), 0xF33D3D3B0BD9C53DULL> name(source);
  return name;
}

// OFFSET: 0x00DD22F0
// FUNCTION: fn_23192
EncryptedName<10, 0xC74BB3BB110FB10FULL>& NinjaBodyPath1() {
  const char source[] = {'\xBB', '\x60', '\xCF', '\xCB', '\xE7', '\x07', '\x9A', '\x07', '\xD5', '\xB1'};
  thread_local EncryptedName<sizeof(source), 0xC74BB3BB110FB10FULL> name(source);
  return name;
}

// OFFSET: 0x00DD23C0
// FUNCTION: fn_23193
EncryptedName<10, 0x0B5F6D6B23D5AFA7ULL>& NinjaBodyPath2() {
  const char source[] = {'\x13', '\x7E', '\x15', '\xF9', '\x37', '\xD9', '\x8E', '\xCB', '\x7D', '\xAF'};
  thread_local EncryptedName<sizeof(source), 0x0B5F6D6B23D5AFA7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD2490
// FUNCTION: fn_23194
EncryptedName<16, 0x2B7BAB0B6971E3E9ULL>& KagerouBodyPath3() {
  const char source[] = {'\x82', '\x82', '\x16', '\x0C', '\x79', '\xC4', '\x0E', '\x77', '\x82', '\x82', '\x16', '\x0C', '\x79', '\xC4', '\x0E', '\x2B'};
  thread_local EncryptedName<sizeof(source), 0x2B7BAB0B6971E3E9ULL> name(source);
  return name;
}

// OFFSET: 0x00DD2550
// FUNCTION: fn_23195
EncryptedName<12, 0x8773593DB181D573ULL>& OboroBodyPath3() {
  const char source[] = {'\x1C', '\xB7', '\xEE', '\xC3', '\x52', '\x05', '\x1C', '\xE5', '\x1C', '\xA7', '\xEE', '\xB1'};
  thread_local EncryptedName<sizeof(source), 0x8773593DB181D573ULL> name(source);
  return name;
}

// OFFSET: 0x00DD2620
// FUNCTION: fn_23196
EncryptedName<16, 0xFBDBDB13A77191BFULL>& KagerouBodyPath4() {
  const char source[] = {'\xD4', '\xF0', '\x16', '\xC2', '\x61', '\xB4', '\xAE', '\xA7', '\xD4', '\xF0', '\x16', '\xC2', '\x61', '\xB4', '\xAE', '\xFB'};
  thread_local EncryptedName<sizeof(source), 0xFBDBDB13A77191BFULL> name(source);
  return name;
}

// OFFSET: 0x00DD26E0
// FUNCTION: fn_23197
EncryptedName<12, 0x19BBC5910555FB97ULL>& OboroBodyPath4() {
  const char source[] = {'\xF8', '\x99', '\x3A', '\x77', '\xFE', '\x99', '\xD4', '\x7B', '\xF8', '\x89', '\x3A', '\x05'};
  thread_local EncryptedName<sizeof(source), 0x19BBC5910555FB97ULL> name(source);
  return name;
}

// OFFSET: 0x00DD27B0
// FUNCTION: fn_23198
EncryptedName<18, 0xBBD983C7FD611FE3ULL>& TaekwonBodyPath4() {
  const char source[] = {'\x26', '\xDD', '\xD0', '\x3A', '\x7B', '\x51', '\x6A', '\x59', '\xBF', '\xDA', '\xA3', '\x4C', '\x00', '\x3F', '\x0B', '\x08', '\x01', '\x1F'};
  thread_local EncryptedName<sizeof(source), 0xBBD983C7FD611FE3ULL> name(source);
  return name;
}

// OFFSET: 0x00DD2880
// FUNCTION: fn_23199
EncryptedName<18, 0xA1ED494BF319772DULL>& TaekwonBodyPath5() {
  const char source[] = {'\xE8', '\xB5', '\xA8', '\x34', '\xF7', '\x9B', '\x5E', '\x43', '\x71', '\xB2', '\xDB', '\x42', '\x8C', '\xF5', '\x3F', '\x12', '\xCF', '\x77'};
  thread_local EncryptedName<sizeof(source), 0xA1ED494BF319772DULL> name(source);
  return name;
}

// OFFSET: 0x00DD2950
// FUNCTION: fn_23200
EncryptedName<18, 0x798B394FF5FF434DULL>& TaekwonBodyPath6() {
  const char source[] = {'\x88', '\x81', '\x4E', '\x32', '\xF3', '\xEB', '\x38', '\x9B', '\x11', '\x86', '\x3D', '\x44', '\x88', '\x85', '\x59', '\xCA', '\xAF', '\x43'};
  thread_local EncryptedName<sizeof(source), 0x798B394FF5FF434DULL> name(source);
  return name;
}

// OFFSET: 0x00DD2A20
// FUNCTION: fn_23201
EncryptedName<18, 0x410DD52D6731BFFDULL>& TaekwonBodyPath7() {
  const char source[] = {'\x38', '\x7D', '\x80', '\xA0', '\x91', '\x07', '\xBE', '\xA3', '\xA1', '\x7A', '\xF3', '\xD6', '\xEA', '\x69', '\xDF', '\xF2', '\x1F', '\xBF'};
  thread_local EncryptedName<sizeof(source), 0x410DD52D6731BFFDULL> name(source);
  return name;
}

// OFFSET: 0x00DD2AF0
// FUNCTION: fn_23202
EncryptedName<18, 0x07B733370F612D0FULL>& TaekwonBodyPath8() {
  const char source[] = {'\xCA', '\xEF', '\xD0', '\xC8', '\x8B', '\xE1', '\x04', '\xE5', '\x53', '\xE8', '\xA3', '\xBE', '\xF0', '\x8F', '\x65', '\xB4', '\xED', '\x2D'};
  thread_local EncryptedName<sizeof(source), 0x07B733370F612D0FULL> name(source);
  return name;
}

// OFFSET: 0x00DD2BC0
// FUNCTION: fn_23203
EncryptedName<14, 0x113DC179F1C795C9ULL>& WizardBodyPath9() {
  const char source[] = {'\x09', '\x32', '\x07', '\x0B', '\xCC', '\x24', '\x61', '\xD1', '\x6E', '\x55', '\x3D', '\x44', '\x9C', '\xC1'};
  thread_local EncryptedName<sizeof(source), 0x113DC179F1C795C9ULL> name(source);
  return name;
}

// OFFSET: 0x00DD2C90
// FUNCTION: fn_23204
EncryptedName<14, 0x31F5C32DD5C53779ULL>& WizardBodyPath10() {
  const char source[] = {'\xB9', '\x90', '\x05', '\x2F', '\x98', '\x26', '\xA9', '\xF1', '\xDE', '\xF7', '\x3F', '\x60', '\xC8', '\xC3'};
  thread_local EncryptedName<sizeof(source), 0x31F5C32DD5C53779ULL> name(source);
  return name;
}

// OFFSET: 0x00DD2D60
// FUNCTION: fn_23205
EncryptedName<10, 0x2F9B717365C1250FULL>& GunslingerBodyPath1() {
  const char source[] = {'\xBF', '\xE2', '\x72', '\xAF', '\x2F', '\xC1', '\x5C', '\x9C', '\xC5', '\x25'};
  thread_local EncryptedName<sizeof(source), 0x2F9B717365C1250FULL> name(source);
  return name;
}

// OFFSET: 0x00DD2E30
// FUNCTION: fn_23206
EncryptedName<10, 0x4FE54379493FB5D5ULL>& GunslingerBodyPath2() {
  const char source[] = {'\x65', '\x72', '\x8C', '\x83', '\x25', '\xF3', '\x22', '\xFC', '\x1F', '\xB5'};
  thread_local EncryptedName<sizeof(source), 0x4FE54379493FB5D5ULL> name(source);
  return name;
}

// OFFSET: 0x00DD2F00
// FUNCTION: fn_23207
EncryptedName<20, 0x656FAB972B3D21BBULL>& RebellionBodyPath2() {
  const char source[] = {'\xC9', '\x44', '\x5F', '\x4E', '\xFB', '\xC7', '\x06', '\x0A', '\xD5', '\x7D', '\x4F', '\x4E', '\xF5', '\xCE', '\x03', '\x09', '\xD2', '\x4E', '\x53', '\x2B'};
  thread_local EncryptedName<sizeof(source), 0x656FAB972B3D21BBULL> name(source);
  return name;
}

// OFFSET: 0x00DD2FD0
// FUNCTION: fn_23208
EncryptedName<20, 0x33CD2BDD5B3927EDULL>& RebellionBodyPath3() {
  const char source[] = {'\x9F', '\x42', '\x5B', '\x3E', '\xB1', '\x47', '\xA4', '\x5C', '\x83', '\x7B', '\x4B', '\x3E', '\xBF', '\x4E', '\xA1', '\x5F', '\x84', '\x48', '\x57', '\x5B'};
  thread_local EncryptedName<sizeof(source), 0x33CD2BDD5B3927EDULL> name(source);
  return name;
}

// OFFSET: 0x00DD30A0
// FUNCTION: fn_23209
EncryptedName<18, 0xD97BB73901ED99BFULL>& TaekwonBodyPath9() {
  const char source[] = {'\x7A', '\x5B', '\x5C', '\xC6', '\x85', '\x65', '\xC8', '\x3B', '\xE3', '\x5C', '\x2F', '\xB0', '\xFE', '\x0B', '\xA9', '\x6A', '\x5D', '\x99'};
  thread_local EncryptedName<sizeof(source), 0xD97BB73901ED99BFULL> name(source);
  return name;
}

// OFFSET: 0x00DD3170
// FUNCTION: fn_23210
EncryptedName<14, 0x5D6B8FCBE3FB97A1ULL>& WizardBodyPath11() {
  const char source[] = {'\x61', '\x30', '\x3B', '\x19', '\x7E', '\x6A', '\x37', '\x9D', '\x06', '\x57', '\x01', '\x56', '\x2E', '\x8F'};
  thread_local EncryptedName<sizeof(source), 0x5D6B8FCBE3FB97A1ULL> name(source);
  return name;
}

// OFFSET: 0x00DD3240
// FUNCTION: fn_23211
EncryptedName<18, 0xE5FDE3151BED831FULL>& TaekwonBodyPath10() {
  const char source[] = {'\xDA', '\x41', '\x5C', '\xDC', '\xA9', '\x31', '\x4E', '\x07', '\x43', '\x46', '\x2F', '\xAA', '\xD2', '\x5F', '\x2F', '\x56', '\xFD', '\x83'};
  thread_local EncryptedName<sizeof(source), 0xE5FDE3151BED831FULL> name(source);
  return name;
}

// OFFSET: 0x00DD3310
// FUNCTION: fn_23212
EncryptedName<14, 0x8971CB79C18D8B27ULL>& WizardBodyPath12() {
  const char source[] = {'\xE7', '\x2C', '\x4D', '\x3B', '\xCC', '\x2E', '\x2D', '\x49', '\x80', '\x4B', '\x77', '\x74', '\x9C', '\xCB'};
  thread_local EncryptedName<sizeof(source), 0x8971CB79C18D8B27ULL> name(source);
  return name;
}

// OFFSET: 0x00DD33E0
// FUNCTION: fn_23213
EncryptedName<18, 0x8DC5D10DAB1585BDULL>& TaekwonBodyPath11() {
  const char source[] = {'\x78', '\x47', '\xA4', '\x6C', '\xB1', '\x03', '\x76', '\x6F', '\xE1', '\x40', '\xD7', '\x1A', '\xCA', '\x6D', '\x17', '\x3E', '\x5F', '\x85'};
  thread_local EncryptedName<sizeof(source), 0x8DC5D10DAB1585BDULL> name(source);
  return name;
}

// OFFSET: 0x00DD34B0
// FUNCTION: fn_23214
EncryptedName<18, 0x21156F4141E35DD9ULL>& TaekwonBodyPath12() {
  const char source[] = {'\x1C', '\x9F', '\x52', '\x86', '\xFD', '\xBD', '\xA6', '\xC3', '\x85', '\x98', '\x21', '\xF0', '\x86', '\xD3', '\xC7', '\x92', '\x3B', '\x5D'};
  thread_local EncryptedName<sizeof(source), 0x21156F4141E35DD9ULL> name(source);
  return name;
}

// OFFSET: 0x00DD3580
// FUNCTION: fn_23215
EncryptedName<18, 0x51EB99CBCB075F35ULL>& TaekwonBodyPath13() {
  const char source[] = {'\xF0', '\x9D', '\xB6', '\x0C', '\x77', '\x4B', '\x58', '\xB3', '\x69', '\x9A', '\xC5', '\x7A', '\x0C', '\x25', '\x39', '\xE2', '\xD7', '\x5F'};
  thread_local EncryptedName<sizeof(source), 0x51EB99CBCB075F35ULL> name(source);
  return name;
}

// OFFSET: 0x00DD3650
// FUNCTION: fn_23216
EncryptedName<14, 0x1B3B756FE399E1D9ULL>& WizardBodyPath13() {
  const char source[] = {'\x19', '\x46', '\x59', '\x19', '\xDA', '\x90', '\x67', '\xDB', '\x7E', '\x21', '\x63', '\x56', '\x8A', '\x75'};
  thread_local EncryptedName<sizeof(source), 0x1B3B756FE399E1D9ULL> name(source);
  return name;
}

// OFFSET: 0x00DD3720
// FUNCTION: fn_23217
EncryptedName<18, 0x3B6D9BFD27A7C7F7ULL>& TaekwonBodyPath14() {
  const char source[] = {'\x32', '\x05', '\x16', '\xE0', '\x41', '\x49', '\xDE', '\xD9', '\xAB', '\x02', '\x65', '\x96', '\x3A', '\x27', '\xBF', '\x88', '\x15', '\xC7'};
  thread_local EncryptedName<sizeof(source), 0x3B6D9BFD27A7C7F7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD37F0
// FUNCTION: fn_23218
EncryptedName<14, 0x451FD72783630709ULL>& WizardBodyPath14() {
  const char source[] = {'\xC9', '\xA0', '\xA3', '\x79', '\x92', '\x32', '\x43', '\x85', '\xAE', '\xC7', '\x99', '\x36', '\xC2', '\xD7'};
  thread_local EncryptedName<sizeof(source), 0x451FD72783630709ULL> name(source);
  return name;
}

// OFFSET: 0x00DD38C0
// FUNCTION: fn_23219
EncryptedName<14, 0x73C32959FDFFA51BULL>& WizardBodyPath15() {
  const char source[] = {'\xDB', '\x02', '\x3F', '\x07', '\xEC', '\xCC', '\x9F', '\xB3', '\xBC', '\x65', '\x05', '\x48', '\xBC', '\x29'};
  thread_local EncryptedName<sizeof(source), 0x73C32959FDFFA51BULL> name(source);
  return name;
}

// OFFSET: 0x00DD3990
// FUNCTION: fn_23220
EncryptedName<14, 0xA15B555BE9CD2B81ULL>& WizardBodyPath16() {
  const char source[] = {'\x41', '\x8C', '\x0D', '\x13', '\xEE', '\xB0', '\x07', '\x61', '\x26', '\xEB', '\x37', '\x5C', '\xBE', '\x55'};
  thread_local EncryptedName<sizeof(source), 0xA15B555BE9CD2B81ULL> name(source);
  return name;
}

// OFFSET: 0x00DD3A60
// FUNCTION: fn_23221
EncryptedName<14, 0xD9115915B1919533ULL>& SageBodyPath8() {
  const char source[] = {'\x8F', '\x29', '\x51', '\x7D', '\xD4', '\xAF', '\x4D', '\x65', '\x8F', '\x55', '\x5D', '\x70', '\xE3', '\x59'};
  thread_local EncryptedName<sizeof(source), 0xD9115915B1919533ULL> name(source);
  return name;
}

// OFFSET: 0x00DD3B30
// FUNCTION: fn_23222
EncryptedName<14, 0xB3F1A10745C1F105ULL>& SageBodyPath9() {
  const char source[] = {'\xB9', '\x4D', '\x01', '\x89', '\xC6', '\x57', '\xAD', '\x0F', '\xB9', '\x31', '\x0D', '\x84', '\xF1', '\xA1'};
  thread_local EncryptedName<sizeof(source), 0xB3F1A10745C1F105ULL> name(source);
  return name;
}

// OFFSET: 0x00DD3C00
// FUNCTION: fn_23223
EncryptedName<18, 0x99157F159BB99BD9ULL>& WindhawkBodyPath1() {
  const char source[] = {'\x8E', '\xD2', '\xF7', '\xDF', '\x5D', '\x3E', '\x42', '\xD2', '\x85', '\xCC', '\xF0', '\xD5', '\x51', '\x37', '\x54', '\xCE', '\x92', '\x9B'};
  thread_local EncryptedName<sizeof(source), 0x99157F159BB99BD9ULL> name(source);
  return name;
}

// OFFSET: 0x00DD3CD0
// FUNCTION: fn_23224
EncryptedName<23, 0x7959FFBF652D07BFULL>& WolfWindhawkBodyPath1() {
  const char source[] = {'\xE8', '\x4E', '\x63', '\x21', '\xF7', '\xBE', '\x0E', '\x32', '\xE3', '\x50', '\x62', '\x29', '\xF9', '\xA0', '\x0E', '\x30', '\xF1', '\x43', '\x65', '\x24', '\xE8', '\xB4', '\x59'};
  thread_local EncryptedName<sizeof(source), 0x7959FFBF652D07BFULL> name(source);
  return name;
}

// OFFSET: 0x00DD3DB0
// FUNCTION: fn_23225
EncryptedName<25, 0x01C9FBA7819FFBD7ULL>& WindhawkRidingBodyPath1() {
  const char source[] = {'\x80', '\xB2', '\xD1', '\xC5', '\xEF', '\xBA', '\x9E', '\x4A', '\x8B', '\xAC', '\xD6', '\xCF', '\xE3', '\xB3', '\x88', '\x56', '\x9C', '\xA4', '\xCD', '\xC8', '\xE3', '\xB2', '\x87', '\x46', '\xD7'};
  thread_local EncryptedName<sizeof(source), 0x01C9FBA7819FFBD7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD3E90
// FUNCTION: fn_23226
EncryptedName<22, 0xABED6971354375C3ULL>& TroubadourBodyPath1() {
  const char source[] = {'\x97', '\x27', '\x0C', '\x60', '\x33', '\x28', '\xA9', '\xE4', '\x96', '\x27', '\x1F', '\x61', '\x23', '\x26', '\xB8', '\xE9', '\x82', '\x31', '\x0C', '\x60', '\x23', '\x69'};
  thread_local EncryptedName<sizeof(source), 0xABED6971354375C3ULL> name(source);
  return name;
}

// OFFSET: 0x00DD3F60
// FUNCTION: fn_23227
EncryptedName<29, 0x6BA557130BDFE997ULL>& TroubadourRidingBodyPath1() {
  const char source[] = {'\xC3', '\xBB', '\x90', '\x5E', '\x51', '\x16', '\xE1', '\x24', '\xC2', '\xBB', '\x83', '\x5F', '\x41', '\x18', '\xF0', '\x29', '\xD6', '\xAD', '\x90', '\x5E', '\x41', '\x08', '\xF7', '\x22', '\xD3', '\xA0', '\x91', '\x4C', '\x13'};
  thread_local EncryptedName<sizeof(source), 0x6BA557130BDFE997ULL> name(source);
  return name;
}

// OFFSET: 0x00DD4040
// FUNCTION: fn_23228
EncryptedName<18, 0xFBAD074BDB2B77A1ULL>& TrouvereBodyPath1() {
  const char source[] = {'\xF5', '\x25', '\x64', '\x8E', '\x1D', '\x42', '\xFF', '\xBE', '\xFD', '\x23', '\x79', '\x94', '\x1E', '\x51', '\xE8', '\xA9', '\xE4', '\x77'};
  thread_local EncryptedName<sizeof(source), 0xFBAD074BDB2B77A1ULL> name(source);
  return name;
}

// OFFSET: 0x00DD4110
// FUNCTION: fn_23229
EncryptedName<25, 0x6D05D5271B5D65C3ULL>& TrouvereRidingBodyPath1() {
  const char source[] = {'\x97', '\x37', '\x12', '\x4E', '\x71', '\x90', '\x57', '\x28', '\x9F', '\x31', '\x0F', '\x54', '\x72', '\x83', '\x40', '\x3F', '\x86', '\x3A', '\x0F', '\x52', '\x63', '\x9C', '\x4B', '\x2A', '\xC3'};
  thread_local EncryptedName<sizeof(source), 0x6D05D5271B5D65C3ULL> name(source);
  return name;
}

// OFFSET: 0x00DD41F0
// FUNCTION: fn_23230
EncryptedName<18, 0x050DF16FC75B25B9ULL>& CardinalBodyPath1() {
  const char source[] = {'\xFA', '\x64', '\x09', '\x83', '\x26', '\xBF', '\x4C', '\x49', '\xE5', '\x66', '\x1A', '\x95', '\x2B', '\xB8', '\x43', '\x44', '\xF5', '\x25'};
  thread_local EncryptedName<sizeof(source), 0x050DF16FC75B25B9ULL> name(source);
  return name;
}

// OFFSET: 0x00DD42C0
// FUNCTION: fn_23231
EncryptedName<25, 0x0789EBC9F5C1C59FULL>& CardinalRidingBodyPath1() {
  const char source[] = {'\xDC', '\x84', '\x93', '\xB1', '\x80', '\xA5', '\xC8', '\x4B', '\xC3', '\x86', '\x80', '\xA7', '\x8D', '\xA2', '\xC7', '\x46', '\xD3', '\x9A', '\x93', '\xBC', '\x8D', '\xA2', '\xC7', '\x40', '\x9F'};
  thread_local EncryptedName<sizeof(source), 0x0789EBC9F5C1C59FULL> name(source);
  return name;
}

// OFFSET: 0x00DD4530
// FUNCTION: fn_23233
EncryptedName<22, 0x3D8387EF97A37DCFULL>& InquisitorBodyPath1() {
  const char source[] = {'\x86', '\x33', '\xF2', '\xC2', '\xA6', '\xD4', '\xCA', '\x69', '\x80', '\x2F', '\xFF', '\xDE', '\xA1', '\xD6', '\xD6', '\x74', '\x9C', '\x34', '\xF7', '\xD8', '\xBD', '\x87'};
  thread_local EncryptedName<sizeof(source), 0x3D8387EF97A37DCFULL> name(source);
  return name;
}

// OFFSET: 0x00DD4600
// FUNCTION: fn_23234
EncryptedName<29, 0x07A7D947CF7323BFULL>& InquisitorRidingBodyPath1() {
  const char source[] = {'\xF6', '\x6D', '\x22', '\x9A', '\x0E', '\x8A', '\xEE', '\x53', '\xF0', '\x71', '\x2F', '\x86', '\x09', '\x88', '\xF2', '\x4E', '\xEC', '\x6A', '\x27', '\x80', '\x15', '\x86', '\xF5', '\x4E', '\xFB', '\x6A', '\x3D', '\x88', '\x47'};
  thread_local EncryptedName<sizeof(source), 0x07A7D947CF7323BFULL> name(source);
  return name;
}

// OFFSET: 0x00DD46E0
// FUNCTION: fn_23235
EncryptedName<12, 0xD97DC927AFE911F1ULL>& BioloBodyPath1() {
  const char source[] = {'\xB3', '\x58', '\xA6', '\xE3', '\x68', '\x95', '\x3F', '\x90', '\xBE', '\x5D', '\xA6', '\xAF'};
  thread_local EncryptedName<sizeof(source), 0xD97DC927AFE911F1ULL> name(source);
  return name;
}

// OFFSET: 0x00DD47B0
// FUNCTION: fn_23236
EncryptedName<26, 0x8BA52B970DD531ADULL>& BioloRidingBodyPath1() {
  const char source[] = {'\xEF', '\x78', '\x9A', '\x41', '\xD8', '\x74', '\xF7', '\xC2', '\xE9', '\x78', '\x9B', '\x4A', '\xCB', '\x69', '\xEC', '\xC4', '\xE1', '\x7E', '\x8A', '\x5F', '\xDE', '\x6F', '\xEC', '\xC5', '\xEA', '\x31'};
  thread_local EncryptedName<sizeof(source), 0x8BA52B970DD531ADULL> name(source);
  return name;
}

// OFFSET: 0x00DD4890
// FUNCTION: fn_23237
EncryptedName<16, 0x47B7BF55EB471557ULL>& MeisterBodyPath1() {
  const char source[] = {'\x1A', '\x50', '\x0E', '\xB8', '\x01', '\xFA', '\xE5', '\x1B', '\x1A', '\x50', '\x0E', '\xB8', '\x01', '\xFA', '\xE5', '\x47'};
  thread_local EncryptedName<sizeof(source), 0x47B7BF55EB471557ULL> name(source);
  return name;
}

// OFFSET: 0x00DD4950
// FUNCTION: fn_23238
EncryptedName<30, 0xCFB9671DEF4BD9E7ULL>& MeisterRidingBodyPath1() {
  const char source[] = {'\xAA', '\x9C', '\x02', '\xBC', '\x49', '\x22', '\xEB', '\x90', '\xB5', '\x90', '\x0F', '\xA6', '\x53', '\x20', '\xE5', '\x82', '\xA2', '\x90', '\x18', '\xBB', '\x58', '\x35', '\xE6', '\x9D', '\xAE', '\x9D', '\x02', '\xA1', '\x5A', '\x67'};
  thread_local EncryptedName<sizeof(source), 0xCFB9671DEF4BD9E7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD4A30
// FUNCTION: fn_23239
EncryptedName<35, 0x2F2103792383718BULL>& MeisterMadogear1BodyPath1() {
  const char source[] = {'\xC6', '\x34', '\xCA', '\x70', '\x2D', '\x46', '\x73', '\x70', '\xC6', '\x30', '\xC7', '\x6C', '\x3E', '\x46', '\x60', '\x7D', '\xD7', '\x3C', '\xC6', '\x6A', '\x2A', '\x57', '\x64', '\x7D', '\xD4', '\x3C', '\xC2', '\x67', '\x36', '\x44', '\x64', '\x6E', '\xD9', '\x40', '\x83'};
  thread_local EncryptedName<sizeof(source), 0x2F2103792383718BULL> name(source);
  return name;
}

// OFFSET: 0x00DD4B10
// FUNCTION: fn_23240
EncryptedName<28, 0x999F5191852D2F6BULL>& DragonKnightBodyPath1() {
  const char source[] = {'\x2F', '\x7D', '\x6C', '\xC2', '\xDE', '\x1F', '\xC0', '\xD2', '\x25', '\x66', '\x6A', '\xCD', '\xC5', '\x0D', '\xDB', '\xCB', '\x2A', '\x68', '\x62', '\xCB', '\xCE', '\x1A', '\xD1', '\xD0', '\x2C', '\x67', '\x79', '\x85'};
  thread_local EncryptedName<sizeof(source), 0x999F5191852D2F6BULL> name(source);
  return name;
}

// OFFSET: 0x00DD4BF0
// FUNCTION: fn_23241
EncryptedName<42, 0x4349ADE96FEB27E5ULL>& DragonKnightRidingBodyPath1() {
  const char source[] = {'\xA1', '\x75', '\xAA', '\x28', '\xA6', '\xE3', '\x16', '\x08', '\xAB', '\x6E', '\xAC', '\x27', '\xBD', '\xF2', '\x1B', '\x0A', '\xA1', '\x6E', '\xA5', '\x28', '\xB5', '\xE9', '\x1B', '\x02', '\xA2', '\x68', '\xA5', '\x30', '\xA2', '\xE3', '\x00', '\x04', '\xAD', '\x73', '\xB4', '\x3D', '\xA0', '\xE9', '\x00', '\x0D', '\xA2', '\x27'};
  thread_local EncryptedName<sizeof(source), 0x4349ADE96FEB27E5ULL> name(source);
  return name;
}

// OFFSET: 0x00DD4CD0
// FUNCTION: fn_23242
EncryptedName<44, 0xA55B7BEF3D7377E3ULL>& DragonKnightChickenBodyPath1() {
  const char source[] = {'\xA7', '\x25', '\x32', '\x7A', '\xA0', '\x35', '\x04', '\xEE', '\xAD', '\x3E', '\x34', '\x75', '\xBB', '\x24', '\x18', '\xED', '\xAA', '\x34', '\x38', '\x78', '\xA1', '\x27', '\x1F', '\xF7', '\xA2', '\x30', '\x3C', '\x73', '\xB0', '\x30', '\x15', '\xEC', '\xA4', '\x3F', '\x27', '\x62', '\xAC', '\x33', '\x12', '\xE6', '\xA8', '\x32', '\x3D', '\x3D'};
  thread_local EncryptedName<sizeof(source), 0xA55B7BEF3D7377E3ULL> name(source);
  return name;
}

// OFFSET: 0x00DD4DB0
// FUNCTION: fn_23243
EncryptedName<30, 0x31E3CF9D358D8705ULL>& ImperialGuardBodyPath1() {
  const char source[] = {'\x4C', '\xCA', '\xDD', '\x70', '\xCF', '\x86', '\xA2', '\x7D', '\x5A', '\xC0', '\xD8', '\x74', '\xCF', '\x8B', '\xBF', '\x78', '\x48', '\xD7', '\xC8', '\x67', '\xD4', '\x8E', '\xAF', '\x6E', '\x42', '\xD2', '\xCC', '\x67', '\xD9', '\xCF'};
  thread_local EncryptedName<sizeof(source), 0x31E3CF9D358D8705ULL> name(source);
  return name;
}

// OFFSET: 0x00DD4E90
// FUNCTION: fn_23244
EncryptedName<44, 0x79A58B0F2173CF49ULL>& ImperialGuardRidingBodyPath1() {
  const char source[] = {'\x00', '\x82', '\x23', '\x64', '\x5D', '\xC2', '\xE4', '\x35', '\x16', '\x88', '\x26', '\x60', '\x5D', '\xCF', '\xFA', '\x2B', '\x00', '\x8B', '\x3A', '\x6F', '\x48', '\xD7', '\xEC', '\x34', '\x19', '\x8A', '\x21', '\x68', '\x4E', '\xC7', '\xFA', '\x3E', '\x1C', '\x8E', '\x21', '\x65', '\x50', '\xD9', '\xEC', '\x3D', '\x00', '\x81', '\x34', '\x21'};
  thread_local EncryptedName<sizeof(source), 0x79A58B0F2173CF49ULL> name(source);
  return name;
}

// OFFSET: 0x00DD4F70
// FUNCTION: fn_23245
EncryptedName<46, 0xF5496753BDE7C5BDULL>& ImperialGuardChickenBodyPath1() {
  const char source[] = {'\xF4', '\x88', '\xB7', '\xF8', '\x01', '\x2E', '\x08', '\xB9', '\xE2', '\x82', '\xB2', '\xFC', '\x01', '\x23', '\x16', '\xB6', '\xF5', '\x8C', '\xA4', '\xF6', '\x16', '\x29', '\x15', '\xBC', '\xF0', '\x95', '\xA2', '\xEF', '\x1A', '\x26', '\x05', '\xAA', '\xFA', '\x90', '\xA6', '\xEF', '\x17', '\x38', '\x0A', '\xBD', '\xF4', '\x86', '\xAC', '\xF8', '\x1D', '\x67'};
  thread_local EncryptedName<sizeof(source), 0xF5496753BDE7C5BDULL> name(source);
  return name;
}

// OFFSET: 0x00DD5060
// FUNCTION: fn_23246
EncryptedName<26, 0x795787B7779D19DBULL>& ShadowCrossBodyPath1() {
  const char source[] = {'\x88', '\x51', '\xDC', '\x33', '\xF8', '\xD0', '\x08', '\x3A', '\x89', '\x56', '\xCE', '\x24', '\xEB', '\xD4', '\x1F', '\x38', '\x9F', '\x56', '\xCA', '\x28', '\xF4', '\xD5', '\x18', '\x2A', '\x88', '\x19'};
  thread_local EncryptedName<sizeof(source), 0x795787B7779D19DBULL> name(source);
  return name;
}

// OFFSET: 0x00DD5140
// FUNCTION: fn_23247
EncryptedName<33, 0x6B3917C939150B9BULL>& ShadowCrossRidingBodyPath1() {
  const char source[] = {'\xC8', '\x43', '\x54', '\x7D', '\x86', '\x40', '\x66', '\x28', '\xC9', '\x44', '\x46', '\x6A', '\x95', '\x44', '\x71', '\x2A', '\xDF', '\x44', '\x42', '\x66', '\x8A', '\x45', '\x76', '\x38', '\xC8', '\x54', '\x47', '\x70', '\x8D', '\x5E', '\x77', '\x2C', '\x9B'};
  thread_local EncryptedName<sizeof(source), 0x6B3917C939150B9BULL> name(source);
  return name;
}

// OFFSET: 0x00DD5210
// FUNCTION: fn_23248
EncryptedName<26, 0x29AB0B59651BDFD5ULL>& AbyssChaserBodyPath1() {
  const char source[] = {'\x94', '\x9D', '\x42', '\x36', '\x0A', '\x54', '\xE8', '\x61', '\x94', '\x8C', '\x5E', '\x37', '\x05', '\x4A', '\xE9', '\x70', '\x86', '\x8C', '\x44', '\x26', '\x11', '\x4A', '\xF8', '\x6C', '\x87', '\xDF'};
  thread_local EncryptedName<sizeof(source), 0x29AB0B59651BDFD5ULL> name(source);
  return name;
}

// OFFSET: 0x00DD52F0
// FUNCTION: fn_23249
EncryptedName<33, 0xF9B1EF9F7961EFF5ULL>& AbyssChaserRidingBodyPath1() {
  const char source[] = {'\xB4', '\xAD', '\x38', '\x2A', '\xCC', '\xB0', '\xF2', '\xB1', '\xB4', '\xBC', '\x24', '\x2B', '\xC3', '\xAE', '\xF3', '\xA0', '\xA6', '\xBC', '\x3E', '\x3A', '\xD7', '\xAE', '\xE2', '\xBC', '\xA7', '\xB0', '\x33', '\x30', '\xDB', '\xA6', '\xFF', '\xBE', '\xF5'};
  thread_local EncryptedName<sizeof(source), 0xF9B1EF9F7961EFF5ULL> name(source);
  return name;
}

// OFFSET: 0x00DD53C0
// FUNCTION: fn_23250
EncryptedName<24, 0xA1DFCDAD555BB16DULL>& SkyEmperorBodyPath1() {
  const char source[] = {'\x3E', '\xFA', '\x02', '\x0A', '\xE8', '\x80', '\x8F', '\xE4', '\x3F', '\xFE', '\x09', '\x09', '\xFE', '\x86', '\x86', '\xFE', '\x28', '\xFC', '\x0B', '\x10', '\xFF', '\x82', '\x8D', '\xA1'};
  thread_local EncryptedName<sizeof(source), 0xA1DFCDAD555BB16DULL> name(source);
  return name;
}

// OFFSET: 0x00DD5490
// FUNCTION: fn_23251
EncryptedName<25, 0xAFF9332FD397533FULL>& SkyEmperor2BodyPath1() {
  const char source[] = {'\x6C', '\x18', '\xCE', '\x8C', '\x6A', '\x7E', '\xA9', '\xEA', '\x6D', '\x1C', '\xC5', '\x8F', '\x7C', '\x78', '\xA0', '\xF0', '\x7A', '\x1E', '\xC7', '\x96', '\x7D', '\x7C', '\xAB', '\x9D', '\x3F'};
  thread_local EncryptedName<sizeof(source), 0xAFF9332FD397533FULL> name(source);
  return name;
}

// OFFSET: 0x00DD5570
// FUNCTION: fn_23252
EncryptedName<31, 0x194F8339AF753FADULL>& SkyEmperorRidingBodyPath1() {
  const char source[] = {'\xFE', '\x74', '\x2C', '\xF0', '\x7C', '\xCE', '\x1F', '\x5C', '\xFF', '\x70', '\x27', '\xF3', '\x6A', '\xC8', '\x16', '\x46', '\xE8', '\x72', '\x25', '\xEA', '\x6B', '\xCC', '\x1D', '\x46', '\xFF', '\x76', '\x31', '\xE6', '\x77', '\xC4', '\x4F'};
  thread_local EncryptedName<sizeof(source), 0x194F8339AF753FADULL> name(source);
  return name;
}

// OFFSET: 0x00DD5650
// FUNCTION: fn_23253
EncryptedName<26, 0x7DC10333F96DE3E7ULL>& SoulAsceticBodyPath1() {
  const char source[] = {'\xB4', '\xAC', '\x38', '\xB5', '\x6C', '\x42', '\x92', '\x3E', '\xA2', '\xB7', '\x24', '\xBA', '\x6F', '\x50', '\x8E', '\x28', '\xAB', '\xBC', '\x2C', '\xAA', '\x70', '\x46', '\x95', '\x34', '\xA4', '\xE3'};
  thread_local EncryptedName<sizeof(source), 0x7DC10333F96DE3E7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD5730
// FUNCTION: fn_23254
EncryptedName<33, 0xE9FBAB53D5A1BB17ULL>& SoulAsceticRidingBodyPath1() {
  const char source[] = {'\x44', '\xF4', '\xF4', '\x99', '\x0C', '\xEA', '\xA8', '\xAA', '\x52', '\xEF', '\xE8', '\x96', '\x0F', '\xF8', '\xB4', '\xBC', '\x5B', '\xE4', '\xE0', '\x86', '\x10', '\xEE', '\xAF', '\xA0', '\x54', '\xE4', '\xF3', '\x9C', '\x17', '\xE2', '\xB5', '\xAE', '\x17'};
  thread_local EncryptedName<sizeof(source), 0xE9FBAB53D5A1BB17ULL> name(source);
  return name;
}

// OFFSET: 0x00DD5800
// FUNCTION: fn_23255
EncryptedName<18, 0x51372B6F4BA14B23ULL>& ShinkiroBodyPath1() {
  const char source[] = {'\x70', '\x03', '\xE8', '\x05', '\x24', '\x62', '\x65', '\x1E', '\x7F', '\x18', '\xE9', '\x02', '\x21', '\x60', '\x7E', '\x03', '\x6C', '\x4B'};
  thread_local EncryptedName<sizeof(source), 0x51372B6F4BA14B23ULL> name(source);
  return name;
}

// OFFSET: 0x00DD58D0
// FUNCTION: fn_23256
EncryptedName<25, 0xB70319D3A78D39CDULL>& ShinkiroRidingBodyPath1() {
  const char source[] = {'\x9E', '\x71', '\xC4', '\xE9', '\x98', '\x50', '\x51', '\xF8', '\x91', '\x6A', '\xC5', '\xEE', '\x9D', '\x52', '\x4A', '\xE5', '\x82', '\x66', '\xDF', '\xEE', '\x97', '\x50', '\x4D', '\xF0', '\xCD'};
  thread_local EncryptedName<sizeof(source), 0xB70319D3A78D39CDULL> name(source);
  return name;
}

// OFFSET: 0x00DD59B0
// FUNCTION: fn_23257
EncryptedName<18, 0x1F79132B518FD389ULL>& ShiranuiBodyPath1() {
  const char source[] = {'\xDA', '\x9B', '\xC6', '\x03', '\x6A', '\x5D', '\x2C', '\x56', '\xD5', '\x80', '\xC7', '\x18', '\x79', '\x52', '\x37', '\x4A', '\xC0', '\xD3'};
  thread_local EncryptedName<sizeof(source), 0x1F79132B518FD389ULL> name(source);
  return name;
}

// OFFSET: 0x00DD5A80
// FUNCTION: fn_23258
EncryptedName<25, 0xC1FD95811D1F4FA1ULL>& ShiranuiRidingBodyPath1() {
  const char source[] = {'\xF2', '\x07', '\x56', '\x4F', '\xC0', '\xDB', '\xA8', '\x88', '\xFD', '\x1C', '\x57', '\x54', '\xD3', '\xD4', '\xB3', '\x94', '\xE8', '\x10', '\x4D', '\x54', '\xC5', '\xDC', '\xB3', '\x86', '\xA1'};
  thread_local EncryptedName<sizeof(source), 0xC1FD95811D1F4FA1ULL> name(source);
  return name;
}

// OFFSET: 0x00DD5B60
// FUNCTION: fn_23259
EncryptedName<24, 0x35F59F6F3DD1C965ULL>& NightWatchBodyPath1() {
  const char source[] = {'\x2B', '\x80', '\x96', '\x75', '\x3B', '\xC0', '\xA2', '\x74', '\x31', '\x8A', '\x99', '\x61', '\x21', '\xD6', '\xB2', '\x7D', '\x31', '\x96', '\x86', '\x7C', '\x3B', '\xDC', '\xBD', '\x35'};
  thread_local EncryptedName<sizeof(source), 0x35F59F6F3DD1C965ULL> name(source);
  return name;
}

// OFFSET: 0x00DD5C30
// FUNCTION: fn_23260
EncryptedName<31, 0xD7BBEB2DDB295D11ULL>& NightWatchRidingBodyPath1() {
  const char source[] = {'\x5F', '\x14', '\x6E', '\x93', '\x79', '\xB4', '\xEC', '\x96', '\x45', '\x1E', '\x61', '\x87', '\x63', '\xA2', '\xFC', '\x9F', '\x45', '\x02', '\x7E', '\x9A', '\x79', '\xA8', '\xF3', '\x88', '\x43', '\x14', '\x6D', '\x92', '\x63', '\xAC', '\xBB'};
  thread_local EncryptedName<sizeof(source), 0xD7BBEB2DDB295D11ULL> name(source);
  return name;
}

// OFFSET: 0x00DD5D10
// FUNCTION: fn_23261
EncryptedName<26, 0xBF0F412941076FADULL>& HyperNoviceBodyPath1() {
  const char source[] = {'\xE5', '\x36', '\x57', '\x04', '\x7B', '\x1E', '\x41', '\xF0', '\xFB', '\x26', '\x44', '\x04', '\x75', '\x09', '\x56', '\xEF', '\xE8', '\x3D', '\x58', '\x0F', '\x66', '\x17', '\x46', '\xFC', '\xE8', '\x6F'};
  thread_local EncryptedName<sizeof(source), 0xBF0F412941076FADULL> name(source);
  return name;
}

// OFFSET: 0x00DD5DF0
// FUNCTION: fn_23262
EncryptedName<33, 0x8DFF0B4B67D9FDE1ULL>& HyperNoviceRidingBodyPath1() {
  const char source[] = {'\xA9', '\xA4', '\x89', '\x22', '\x19', '\x54', '\xB1', '\xC2', '\xB7', '\xB4', '\x9A', '\x22', '\x17', '\x43', '\xA6', '\xDD', '\xA4', '\xAF', '\x86', '\x29', '\x04', '\x5D', '\xB6', '\xCE', '\xA4', '\xA2', '\x8B', '\x2E', '\x0F', '\x42', '\xB1', '\xCA', '\xE1'};
  thread_local EncryptedName<sizeof(source), 0x8DFF0B4B67D9FDE1ULL> name(source);
  return name;
}

// OFFSET: 0x00DD5EC0
// FUNCTION: fn_23263
EncryptedName<30, 0xB59BADC5B365617DULL>& SpiritHandlerBodyPath1() {
  const char source[] = {'\x2E', '\x31', '\x2C', '\xE1', '\x8C', '\xF9', '\xC4', '\xFD', '\x3C', '\x2F', '\x21', '\xFF', '\x80', '\xFF', '\xC7', '\xE6', '\x2D', '\x28', '\x37', '\xFA', '\x91', '\xF2', '\xD3', '\xF4', '\x33', '\x25', '\x29', '\xF6', '\x97', '\xAD'};
  thread_local EncryptedName<sizeof(source), 0xB59BADC5B365617DULL> name(source);
  return name;
}

// OFFSET: 0x00DD5FA0
// FUNCTION: fn_23264
EncryptedName<37, 0x77D925073D6F19B9ULL>& SpiritHandlerRidingBodyPath1() {
  const char source[] = {'\xEA', '\x49', '\x26', '\x6F', '\x4E', '\x71', '\x86', '\x3F', '\xF8', '\x57', '\x2B', '\x71', '\x42', '\x77', '\x85', '\x24', '\xE9', '\x50', '\x3D', '\x74', '\x53', '\x7A', '\x91', '\x36', '\xF7', '\x5D', '\x23', '\x78', '\x55', '\x7A', '\x8B', '\x3E', '\xFD', '\x50', '\x21', '\x7A', '\x07'};
  thread_local EncryptedName<sizeof(source), 0x77D925073D6F19B9ULL> name(source);
  return name;
}

// OFFSET: 0x00DD6080
// FUNCTION: fn_23265
EncryptedName<12, 0x91E7A7DFA58F9B6BULL>& DruidBodyPath1() {
  const char source[] = {'\x2F', '\xC9', '\xDA', '\xEC', '\x9B', '\xFB', '\xA3', '\xC3', '\x3E', '\xD2', '\xCB', '\xA5'};
  thread_local EncryptedName<sizeof(source), 0x91E7A7DFA58F9B6BULL> name(source);
  return name;
}

// OFFSET: 0x00DD6150
// FUNCTION: fn_23266
EncryptedName<12, 0x2529F16B0375530DULL>& DruidBodyPath2() {
  const char source[] = {'\x49', '\x01', '\x20', '\x4A', '\x2F', '\xAD', '\x6D', '\x77', '\x58', '\x1A', '\x31', '\x03'};
  thread_local EncryptedName<sizeof(source), 0x2529F16B0375530DULL> name(source);
  return name;
}

// OFFSET: 0x00DD6220
// FUNCTION: fn_23267
EncryptedName<14, 0x91DF1F5D3B014D45ULL>& KarnosBodyPath1() {
  const char source[] = {'\x0E', '\x0C', '\x53', '\x75', '\x12', '\x4C', '\x83', '\xDA', '\x04', '\x1F', '\x4F', '\x74', '\x0E', '\x1F'};
  thread_local EncryptedName<sizeof(source), 0x91DF1F5D3B014D45ULL> name(source);
  return name;
}

// OFFSET: 0x00DD62F0
// FUNCTION: fn_23268
EncryptedName<14, 0x0D7FA94DD32361A9ULL>& KarnosBodyPath2() {
  const char source[] = {'\xE2', '\x20', '\x71', '\x9D', '\x02', '\xFA', '\x23', '\x46', '\xE8', '\x33', '\x6D', '\x9C', '\x1E', '\xA9'};
  thread_local EncryptedName<sizeof(source), 0x0D7FA94DD32361A9ULL> name(source);
  return name;
}

// OFFSET: 0x00DD63C0
// FUNCTION: fn_23269
EncryptedName<14, 0x2F371DDDADDD53CFULL>& AliteaBodyPath1() {
  const char source[] = {'\x8E', '\x1F', '\x94', '\xF9', '\x98', '\x5C', '\x6B', '\x6E', '\x83', '\x1A', '\x89', '\xE8', '\x9C', '\x1D'};
  thread_local EncryptedName<sizeof(source), 0x2F371DDDADDD53CFULL> name(source);
  return name;
}

// OFFSET: 0x00DD6490
// FUNCTION: fn_23270
EncryptedName<6, 0x2F3D75DFCF678FF3ULL>& DruidJobName1() {
  const char source[] = {'\xB7', '\xDD', '\x32', '\x86', '\x9B', '\x75'};
  thread_local EncryptedName<sizeof(source), 0x2F3D75DFCF678FF3ULL> name(source);
  return name;
}

// OFFSET: 0x00DD6560
// FUNCTION: fn_23271
EncryptedName<6, 0x3BBB5F0F8F216F31ULL>& DruidJobName2() {
  const char source[] = {'\x75', '\x3D', '\x74', '\xC6', '\x4B', '\x5F'};
  thread_local EncryptedName<sizeof(source), 0x3BBB5F0F8F216F31ULL> name(source);
  return name;
}

// OFFSET: 0x00DD6630
// FUNCTION: fn_23272
EncryptedName<7, 0x6F9D21B96D71C1FFULL>& KarnosJobName1() {
  const char source[] = {'\xB4', '\x80', '\x23', '\x23', '\xF6', '\x72', '\x9D'};
  thread_local EncryptedName<sizeof(source), 0x6F9D21B96D71C1FFULL> name(source);
  return name;
}

// OFFSET: 0x00DD6700
// FUNCTION: fn_23273
EncryptedName<7, 0xED730315539785C7ULL>& KarnosJobName2() {
  const char source[] = {'\x8C', '\xC4', '\xC5', '\x1D', '\x5A', '\x50', '\x73'};
  thread_local EncryptedName<sizeof(source), 0xED730315539785C7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD67D0
// FUNCTION: fn_23274
EncryptedName<7, 0x8F61434F0F79DDD7ULL>& AliteaJobName1() {
  const char source[] = {'\x96', '\x91', '\x30', '\x5B', '\x0A', '\x02', '\x61'};
  thread_local EncryptedName<sizeof(source), 0x8F61434F0F79DDD7ULL> name(source);
  return name;
}

// OFFSET: 0x00DD68A0
// FUNCTION: fn_23275
EncryptedName<9, 0xA5C5D1A3B9457B53ULL>& WerewolfJobName1() {
  const char source[] = {'\x04', '\x3E', '\x17', '\xFC', '\xF4', '\x9E', '\x89', '\xE3', '\x53'};
  thread_local EncryptedName<sizeof(source), 0xA5C5D1A3B9457B53ULL> name(source);
  return name;
}

// OFFSET: 0x00DD6A40
// FUNCTION: fn_23277
EncryptedName<13, 0x4B670715D543C1C3ULL>& DruidRidingName() {
  const char source[] = {'\x87', '\x93', '\x16', '\x9C', '\x51', '\x58', '\x35', '\x02', '\x87', '\x88', '\x0D', '\x92', '\x15'};
  thread_local EncryptedName<sizeof(source), 0x4B670715D543C1C3ULL> name(source);
  return name;
}

// OFFSET: 0x00DD6B10
// FUNCTION: fn_23278
EncryptedName<14, 0x7155E5EF0DFB0D03ULL>& KarnosRidingName() {
  const char source[] = {'\x48', '\x4C', '\xA9', '\x43', '\xA0', '\xB6', '\x0A', '\x23', '\x4A', '\x49', '\xB2', '\x43', '\xA8', '\xE5'};
  thread_local EncryptedName<sizeof(source), 0x7155E5EF0DFB0D03ULL> name(source);
  return name;
}

// OFFSET: 0x00DD6BE0
// FUNCTION: fn_23279
EncryptedName<14, 0x4B1D078937DB892BULL>& AliteaRidingName() {
  const char source[] = {'\x6A', '\xC5', '\x92', '\x63', '\xCC', '\x46', '\x42', '\x19', '\x62', '\xCD', '\x92', '\x79', '\xCE', '\x07'};
  thread_local EncryptedName<sizeof(source), 0x4B1D078937DB892BULL> name(source);
  return name;
}

// OFFSET: 0x00E29530
// FUNCTION: fn_24044
EncryptedName<14, 0x6BC977B3B5993503ULL>& PoringSuperNovice2SpriteName() {
  const char source[] = {'\xC0', '\xFF', '\x23', '\x0D', '\x73', '\xAD', '\x95', '\xA8', '\xC9', '\x8F', '\x21', '\x75', '\x69', '\x77'};
  thread_local EncryptedName<sizeof(source), 0x6BC977B3B5993503ULL> name(source);
  return name;
}

// OFFSET: 0x00E29600
// FUNCTION: fn_24045
EncryptedName<14, 0x91493751EF33B5FDULL>& PoringSuperNovice2BabySpriteName() {
  const char source[] = {'\x3E', '\x7F', '\x89', '\x57', '\x91', '\xED', '\x15', '\x52', '\x37', '\x0F', '\x8B', '\x2F', '\x8B', '\x37'};
  thread_local EncryptedName<sizeof(source), 0x91493751EF33B5FDULL> name(source);
  return name;
}

// OFFSET: 0x00E296D0
// FUNCTION: fn_24046
EncryptedName<18, 0x0DF5B33D9F21EB79ULL>& SheepPriestBabySpriteName() {
  const char source[] = {'\xBE', '\x2A', '\x99', '\x31', '\x80', '\x09', '\x33', '\xA3', '\x25', '\x2C', '\xE0', '\x27', '\x93', '\x0E', '\x4F', '\xCB', '\xD7', '\xEB'};
  thread_local EncryptedName<sizeof(source), 0x0DF5B33D9F21EB79ULL> name(source);
  return name;
}

// OFFSET: 0x00E297A0
// FUNCTION: fn_24047
EncryptedName<14, 0x01E7898D9D235DDFULL>& FoxWizardBabySpriteName() {
  const char source[] = {'\x1F', '\xFA', '\xE3', '\x67', '\x38', '\x6C', '\xBB', '\xC1', '\x78', '\x9D', '\xD9', '\x28', '\x68', '\x89'};
  thread_local EncryptedName<sizeof(source), 0x01E7898D9D235DDFULL> name(source);
  return name;
}

// OFFSET: 0x00E29870
// FUNCTION: fn_24048
EncryptedName<14, 0x6961FF056999A303ULL>& PigBlacksmithBabySpriteName() {
  const char source[] = {'\xC2', '\x05', '\x5A', '\xDF', '\xB5', '\x07', '\x3D', '\xA8', '\xA5', '\x60', '\x2F', '\xD9', '\xFD', '\xFF'};
  thread_local EncryptedName<sizeof(source), 0x6961FF056999A303ULL> name(source);
  return name;
}

// OFFSET: 0x00E29940
// FUNCTION: fn_24049
EncryptedName<14, 0x9DEFC105A1EB2F71ULL>& PigMechanicSpriteName() {
  const char source[] = {'\xC8', '\xE3', '\x2F', '\x68', '\xB1', '\x11', '\xB3', '\x24', '\xBD', '\xEB', '\x22', '\x15', '\xD5', '\xC1'};
  thread_local EncryptedName<sizeof(source), 0x9DEFC105A1EB2F71ULL> name(source);
  return name;
}

// OFFSET: 0x00E29A10
// FUNCTION: fn_24050
EncryptedName<14, 0xC7FB6DF18577D34BULL>& PigMechanicBabySpriteName() {
  const char source[] = {'\xF2', '\x1F', '\xB3', '\x4C', '\x45', '\xBD', '\xA7', '\x7E', '\x87', '\x17', '\xBE', '\x31', '\x21', '\x6D'};
  thread_local EncryptedName<sizeof(source), 0xC7FB6DF18577D34BULL> name(source);
  return name;
}

// OFFSET: 0x00E29AE0
// FUNCTION: fn_24051
EncryptedName<14, 0xBD3F430159B551FDULL>& OstrichRangerSpriteName() {
  const char source[] = {'\x4A', '\xE8', '\x75', '\x97', '\xC0', '\xED', '\x63', '\x0A', '\x44', '\x91', '\x7B', '\x98', '\xAF', '\x43'};
  thread_local EncryptedName<sizeof(source), 0xBD3F430159B551FDULL> name(source);
  return name;
}

// OFFSET: 0x00E29BB0
// FUNCTION: fn_24052
EncryptedName<14, 0x2B477F9B5B39B301ULL>& OstrichRangerBabySpriteName() {
  const char source[] = {'\xB6', '\x0A', '\xF9', '\x95', '\x5A', '\xD1', '\x1B', '\x9C', '\xB8', '\x73', '\xF7', '\x9A', '\x35', '\x7F'};
  thread_local EncryptedName<sizeof(source), 0x2B477F9B5B39B301ULL> name(source);
  return name;
}

// OFFSET: 0x00E29C80
// FUNCTION: fn_24053
EncryptedName<10, 0x5B832361BDA77197ULL>& LionKnightSpriteName() {
  const char source[] = {'\x26', '\x93', '\x1C', '\x5A', '\x3D', '\x92', '\x61', '\xE0', '\x70', '\x71'};
  thread_local EncryptedName<sizeof(source), 0x5B832361BDA77197ULL> name(source);
  return name;
}

// OFFSET: 0x00E29D50
// FUNCTION: fn_24054
EncryptedName<10, 0xEB3D4FAB0FC183DBULL>& LionKnightBabySpriteName() {
  const char source[] = {'\x6A', '\x61', '\x7A', '\xE8', '\xF7', '\xFE', '\xDF', '\x50', '\x3C', '\x83'};
  thread_local EncryptedName<sizeof(source), 0xEB3D4FAB0FC183DBULL> name(source);
  return name;
}

// OFFSET: 0x00E29E20
// FUNCTION: fn_24055
EncryptedName<10, 0xFFBF43D1FFD5FF49ULL>& LionKnightHighSpriteName() {
  const char source[] = {'\xF8', '\x1D', '\x6E', '\x18', '\x8D', '\xF2', '\x5D', '\x44', '\xAE', '\xFF'};
  thread_local EncryptedName<sizeof(source), 0xFFBF43D1FFD5FF49ULL> name(source);
  return name;
}

// OFFSET: 0x00E29EF0
// FUNCTION: fn_24056
EncryptedName<18, 0xE51B29334D353B81ULL>& LionRoyalGuardSpriteName() {
  const char source[] = {'\x36', '\xF5', '\x8B', '\xAF', '\x83', '\x88', '\xAE', '\x00', '\xDD', '\x8C', '\xFB', '\xF3', '\xD1', '\x99', '\xBA', '\x50', '\x64', '\x3B'};
  thread_local EncryptedName<sizeof(source), 0xE51B29334D353B81ULL> name(source);
  return name;
}

// OFFSET: 0x00E29FC0
// FUNCTION: fn_24057
EncryptedName<18, 0x8F3F2FA959977F9FULL>& LionRoyalGuardBabySpriteName() {
  const char source[] = {'\x28', '\xB1', '\x29', '\xBB', '\x19', '\x8E', '\x8A', '\x6A', '\xC3', '\xC8', '\x59', '\xE7', '\x4B', '\x9F', '\x9E', '\x3A', '\x7A', '\x7F'};
  thread_local EncryptedName<sizeof(source), 0x8F3F2FA959977F9FULL> name(source);
  return name;
}

// OFFSET: 0x00E2A090
// FUNCTION: fn_24058
EncryptedName<18, 0x7FE563BD3FB7F925ULL>& LionRuneKnightSpriteName() {
  const char source[] = {'\x92', '\x10', '\x04', '\x95', '\x7D', '\xAF', '\x23', '\xD1', '\x79', '\x4E', '\x5E', '\x8C', '\x17', '\xA3', '\x29', '\xB9', '\x8B', '\xF9'};
  thread_local EncryptedName<sizeof(source), 0x7FE563BD3FB7F925ULL> name(source);
  return name;
}

// OFFSET: 0x00E2A160
// FUNCTION: fn_24059
EncryptedName<18, 0x8B3FA35B5BDDDF89ULL>& LionRuneKnightBabySpriteName() {
  const char source[] = {'\x3E', '\x36', '\x6E', '\xF1', '\x9B', '\x6F', '\xF9', '\x25', '\xD5', '\x68', '\x34', '\xE8', '\xF1', '\x63', '\xF3', '\x4D', '\x27', '\xDF'};
  thread_local EncryptedName<sizeof(source), 0x8B3FA35B5BDDDF89ULL> name(source);
  return name;
}

// OFFSET: 0x00E2A230
// FUNCTION: fn_24060
EncryptedName<22, 0x7DB1050779034DE7ULL>& LionCrusaderSpriteName() {
  const char source[] = {'\x22', '\xE4', '\xB4', '\x9E', '\xBB', '\xB9', '\x71', '\xB1', '\x53', '\xB8', '\x5F', '\xBC', '\xAE', '\xB2', '\x56', '\xC1', '\x5B', '\x8D', '\xCF', '\xCD', '\xF2', '\x05'};
  thread_local EncryptedName<sizeof(source), 0x7DB1050779034DE7ULL> name(source);
  return name;
}

// OFFSET: 0x00E2A300
// FUNCTION: fn_24061
EncryptedName<22, 0x5D6B3B1DEB2397B1ULL>& LionCrusaderBabySpriteName() {
  const char source[] = {'\x74', '\x3E', '\x94', '\x0C', '\xA1', '\x87', '\xAB', '\x91', '\x05', '\x62', '\x7F', '\x2E', '\xB4', '\x8C', '\x8C', '\xE1', '\x0D', '\x57', '\xEF', '\x5F', '\xE8', '\x3B'};
  thread_local EncryptedName<sizeof(source), 0x5D6B3B1DEB2397B1ULL> name(source);
  return name;
}

// OFFSET: 0x00E2A3D0
// FUNCTION: fn_24062
EncryptedName<22, 0x07112F85A39F4FABULL>& LionCrusaderHighSpriteName() {
  const char source[] = {'\x6E', '\xE6', '\x28', '\x44', '\x39', '\x93', '\xD1', '\xCB', '\x1F', '\xBA', '\xC3', '\x66', '\x2C', '\x98', '\xF6', '\xBB', '\x17', '\x8F', '\x53', '\x17', '\x70', '\x2F'};
  thread_local EncryptedName<sizeof(source), 0x07112F85A39F4FABULL> name(source);
  return name;
}

// OFFSET: 0x00E2A4A0
// FUNCTION: fn_24063
EncryptedName<16, 0x45E3F103CBB5F157ULL>& KagerouSpriteName() {
  const char source[] = {'\x3C', '\x90', '\xD2', '\xAE', '\x71', '\x9E', '\x96', '\x19', '\x3C', '\x90', '\xD2', '\xAE', '\x71', '\x9E', '\x96', '\x45'};
  thread_local EncryptedName<sizeof(source), 0x45E3F103CBB5F157ULL> name(source);
  return name;
}

// OFFSET: 0x00E2A560
// FUNCTION: fn_24064
EncryptedName<12, 0x936F3D2FFDFD87F5ULL>& OboroSpriteName() {
  const char source[] = {'\x9A', '\xE5', '\x92', '\x8F', '\x40', '\x61', '\x00', '\xF1', '\x9A', '\xF5', '\x92', '\xFD'};
  thread_local EncryptedName<sizeof(source), 0x936F3D2FFDFD87F5ULL> name(source);
  return name;
}

// OFFSET: 0x00E2A630
// FUNCTION: fn_24065
EncryptedName<16, 0x2D33791F67AD47DFULL>& FrogKagerouSpriteName() {
  const char source[] = {'\xB4', '\x26', '\xCA', '\x02', '\x6D', '\x16', '\x46', '\x71', '\xB4', '\x26', '\xCA', '\x02', '\x6D', '\x16', '\x46', '\x2D'};
  thread_local EncryptedName<sizeof(source), 0x2D33791F67AD47DFULL> name(source);
  return name;
}

// OFFSET: 0x00E2A6F0
// FUNCTION: fn_24066
EncryptedName<12, 0x6B3301E1A3079125ULL>& FrogOboroSpriteName() {
  const char source[] = {'\x4A', '\xF3', '\x68', '\xD1', '\x8E', '\x5D', '\x5C', '\x09', '\x4A', '\xE3', '\x68', '\xA3'};
  thread_local EncryptedName<sizeof(source), 0x6B3301E1A3079125ULL> name(source);
  return name;
}

// OFFSET: 0x00E2A7C0
// FUNCTION: fn_24067
EncryptedName<20, 0x2F2701A7E56D3D89ULL>& RebellionSpriteName() {
  const char source[] = {'\xFB', '\x58', '\x0F', '\x80', '\xCB', '\x6D', '\x4E', '\x40', '\xE7', '\x61', '\x1F', '\x80', '\xC5', '\x64', '\x4B', '\x43', '\xE0', '\x52', '\x03', '\xE5'};
  thread_local EncryptedName<sizeof(source), 0x2F2701A7E56D3D89ULL> name(source);
  return name;
}

// OFFSET: 0x00E2A890
// FUNCTION: fn_24068
EncryptedName<18, 0xC39FB559A17D3963ULL>& DoramSummonerSpriteName() {
  const char source[] = {'\x10', '\x4C', '\x10', '\xCC', '\x36', '\xDB', '\xFA', '\xB1', '\x3F', '\x4A', '\x08', '\xCC', '\x34', '\xDA', '\xF1', '\xA6', '\x11', '\x39'};
  thread_local EncryptedName<sizeof(source), 0xC39FB559A17D3963ULL> name(source);
  return name;
}

// OFFSET: 0x00E2A960
// FUNCTION: fn_24069
EncryptedName<23, 0x71B7691737C5A1AFULL>& DoramCartSummonerSpriteName() {
  const char source[] = {'\xDC', '\xD4', '\xA8', '\x5A', '\x78', '\x07', '\xD2', '\x03', '\xF3', '\xC2', '\xA4', '\x45', '\x63', '\x36', '\xC4', '\x04', '\xC2', '\xCC', '\xAA', '\x59', '\x72', '\x1B', '\xB7'};
  thread_local EncryptedName<sizeof(source), 0x71B7691737C5A1AFULL> name(source);
  return name;
}

// OFFSET: 0x00E2AA40
// FUNCTION: fn_24070
EncryptedName<18, 0x899D11BB83695B0DULL>& DoramSummonerBabySpriteName() {
  const char source[] = {'\x7E', '\x2E', '\x04', '\xEE', '\xD4', '\x7F', '\xF8', '\xFB', '\x51', '\x28', '\x1C', '\xEE', '\xD6', '\x7E', '\xF3', '\xEC', '\x7F', '\x5B'};
  thread_local EncryptedName<sizeof(source), 0x899D11BB83695B0DULL> name(source);
  return name;
}

// OFFSET: 0x00E2AB10
// FUNCTION: fn_24071
EncryptedName<23, 0x2B8725D7E95325F5ULL>& DoramCartSummonerBabySpriteName() {
  const char source[] = {'\x86', '\x50', '\x3E', '\x84', '\xB8', '\x4B', '\xE2', '\x59', '\xA9', '\x46', '\x32', '\x9B', '\xA3', '\x7A', '\xF4', '\x5E', '\x98', '\x48', '\x3C', '\x87', '\xB2', '\x57', '\x87'};
  thread_local EncryptedName<sizeof(source), 0x2B8725D7E95325F5ULL> name(source);
  return name;
}

// OFFSET: 0x00E2ABF0
// FUNCTION: fn_24072
EncryptedName<10, 0x7989830DE73B9DBDULL>& NinjaBabySpriteName() {
  const char source[] = {'\x09', '\x4C', '\xFB', '\x3D', '\x51', '\x37', '\x58', '\xB9', '\x67', '\x9D'};
  thread_local EncryptedName<sizeof(source), 0x7989830DE73B9DBDULL> name(source);
  return name;
}

// OFFSET: 0x00E2ACC0
// FUNCTION: fn_24073
EncryptedName<10, 0xBDADDD87C5A50B6BULL>& FrogNinjaBabySpriteName() {
  const char source[] = {'\xDF', '\xDA', '\x65', '\x1F', '\xDB', '\x69', '\x7C', '\x7D', '\xB1', '\x0B'};
  thread_local EncryptedName<sizeof(source), 0xBDADDD87C5A50B6BULL> name(source);
  return name;
}

// OFFSET: 0x00E2AD90
// FUNCTION: fn_24074
EncryptedName<16, 0xA7471985F93F9BDFULL>& KagerouBabySpriteName() {
  const char source[] = {'\xB4', '\xFA', '\x58', '\x9C', '\xF7', '\x76', '\x32', '\xFB', '\xB4', '\xFA', '\x58', '\x9C', '\xF7', '\x76', '\x32', '\xA7'};
  thread_local EncryptedName<sizeof(source), 0xA7471985F93F9BDFULL> name(source);
  return name;
}

// OFFSET: 0x00E2AE50
// FUNCTION: fn_24075
EncryptedName<12, 0x5FFD7FABB5D1EF57ULL>& OboroBabySpriteName() {
  const char source[] = {'\x38', '\x8D', '\xBE', '\xC7', '\xC4', '\x23', '\x92', '\x3D', '\x38', '\x9D', '\xBE', '\xB5'};
  thread_local EncryptedName<sizeof(source), 0x5FFD7FABB5D1EF57ULL> name(source);
  return name;
}

// OFFSET: 0x00E2AF20
// FUNCTION: fn_24076
EncryptedName<16, 0xD3E5516F89F161D1ULL>& FrogKagerouBabySpriteName() {
  const char source[] = {'\xBA', '\x00', '\x96', '\xEC', '\x1D', '\x3E', '\x90', '\x8F', '\xBA', '\x00', '\x96', '\xEC', '\x1D', '\x3E', '\x90', '\xD3'};
  thread_local EncryptedName<sizeof(source), 0xD3E5516F89F161D1ULL> name(source);
  return name;
}

// OFFSET: 0x00E2AFE0
// FUNCTION: fn_24077
EncryptedName<12, 0xF535E561137F138DULL>& FrogOboroBabySpriteName() {
  const char source[] = {'\xE2', '\x71', '\x10', '\x61', '\x0E', '\xB9', '\x5A', '\x97', '\xE2', '\x61', '\x10', '\x13'};
  thread_local EncryptedName<sizeof(source), 0xF535E561137F138DULL> name(source);
  return name;
}

// OFFSET: 0x00E2B0B0
// FUNCTION: fn_24078
EncryptedName<18, 0xABED6971354375C3ULL>& TaekwonBabySpriteName() {
  const char source[] = {'\x06', '\xB7', '\xF2', '\xF2', '\xCD', '\xBB', '\x5E', '\x49', '\x9F', '\xB0', '\x81', '\x84', '\xB6', '\xD5', '\x3F', '\x18', '\x21', '\x75'};
  thread_local EncryptedName<sizeof(source), 0xABED6971354375C3ULL> name(source);
  return name;
}

// OFFSET: 0x00E2B180
// FUNCTION: fn_24079
EncryptedName<18, 0x01C9FBA7819FFBD7ULL>& PoringTaekwonBabySpriteName() {
  const char source[] = {'\x12', '\x39', '\x2E', '\x46', '\x1B', '\x29', '\x7A', '\xE3', '\x8B', '\x3E', '\x5D', '\x30', '\x60', '\x47', '\x1B', '\xB2', '\x35', '\xFB'};
  thread_local EncryptedName<sizeof(source), 0x01C9FBA7819FFBD7ULL> name(source);
  return name;
}

// OFFSET: 0x00E2B250
// FUNCTION: fn_24080
EncryptedName<10, 0xFBAD074BDB2B77A1ULL>& StarGladiatorBabySpriteName() {
  const char source[] = {'\x10', '\xB0', '\x97', '\x61', '\x17', '\xB6', '\x6A', '\x47', '\x1B', '\x77'};
  thread_local EncryptedName<sizeof(source), 0xFBAD074BDB2B77A1ULL> name(source);
  return name;
}

// OFFSET: 0x00E2B320
// FUNCTION: fn_24081
EncryptedName<10, 0x6BA557130BDFE997ULL>& StarGladiator2BabySpriteName() {
  const char source[] = {'\x26', '\x2E', '\x63', '\xB1', '\x4F', '\xE6', '\x62', '\xD7', '\x2D', '\xE9'};
  thread_local EncryptedName<sizeof(source), 0x6BA557130BDFE997ULL> name(source);
  return name;
}

// OFFSET: 0x00E2B3F0
// FUNCTION: fn_24082
EncryptedName<10, 0x050DF16FC75B25B9ULL>& PoringStarGladiatorBabySpriteName() {
  const char source[] = {'\x08', '\xE2', '\xE7', '\x7D', '\x33', '\x40', '\xCA', '\xB9', '\x03', '\x25'};
  thread_local EncryptedName<sizeof(source), 0x050DF16FC75B25B9ULL> name(source);
  return name;
}

// OFFSET: 0x00E2B4C0
// FUNCTION: fn_24083
EncryptedName<18, 0x6D05D5271B5D65C3ULL>& SoulLinkerBabySpriteName() {
  const char source[] = {'\x7F', '\xB7', '\xE2', '\xF4', '\x9F', '\x60', '\xC1', '\xD2', '\x9F', '\xD9', '\x8F', '\xA4', '\xC8', '\x6D', '\xB0', '\xA9', '\x7C', '\x65'};
  thread_local EncryptedName<sizeof(source), 0x6D05D5271B5D65C3ULL> name(source);
  return name;
}

// OFFSET: 0x00E2B590
// FUNCTION: fn_24084
EncryptedName<18, 0x3D8387EF97A37DCFULL>& FrogSoulLinkerBabySpriteName() {
  const char source[] = {'\x73', '\xAF', '\x1C', '\x78', '\x57', '\x32', '\x47', '\x82', '\x93', '\xC1', '\x71', '\x28', '\x00', '\x3F', '\x36', '\xF9', '\x70', '\x7D'};
  thread_local EncryptedName<sizeof(source), 0x3D8387EF97A37DCFULL> name(source);
  return name;
}

// OFFSET: 0x00E2E2E0
// FUNCTION: fn_24138
EncryptedName<24, 0x3BBB5F0F8F216F31ULL>& NightWatchBodyPath2() {
  const char source[] = {'\x7F', '\x26', '\x66', '\xC7', '\x5B', '\x00', '\xEC', '\x7A', '\x65', '\x2C', '\x69', '\xD3', '\x41', '\x16', '\xFC', '\x73', '\x65', '\x30', '\x76', '\xCE', '\x5B', '\x1C', '\xF3', '\x3B'};
  thread_local EncryptedName<sizeof(source), 0x3BBB5F0F8F216F31ULL> name(source);
  return name;
}

// OFFSET: 0x00E2E3B0
// FUNCTION: fn_24139
EncryptedName<31, 0x2F3D75DFCF678FF3ULL>& NightWatchRidingBodyPath2() {
  const char source[] = {'\xBD', '\xC6', '\x20', '\x87', '\x8B', '\x2A', '\x6A', '\x6E', '\xA7', '\xCC', '\x2F', '\x93', '\x91', '\x3C', '\x7A', '\x67', '\xA7', '\xD0', '\x30', '\x8E', '\x8B', '\x36', '\x75', '\x70', '\xA1', '\xC6', '\x23', '\x86', '\x91', '\x32', '\x3D'};
  thread_local EncryptedName<sizeof(source), 0x2F3D75DFCF678FF3ULL> name(source);
  return name;
}

// OFFSET: 0x00E2E490
// FUNCTION: fn_24140
EncryptedName<26, 0xED730315539785C7ULL>& HyperNoviceBodyPath2() {
  const char source[] = {'\x8F', '\xDC', '\xC7', '\x16', '\x47', '\x5C', '\x3D', '\xA2', '\x91', '\xCC', '\xD4', '\x16', '\x49', '\x4B', '\x2A', '\xBD', '\x82', '\xD7', '\xC8', '\x1D', '\x5A', '\x55', '\x3A', '\xAE', '\x82', '\x85'};
  thread_local EncryptedName<sizeof(source), 0xED730315539785C7ULL> name(source);
  return name;
}

// OFFSET: 0x00E2E570
// FUNCTION: fn_24141
EncryptedName<33, 0x6F9D21B96D71C1FFULL>& HyperNoviceRidingBodyPath2() {
  const char source[] = {'\xB7', '\x98', '\x21', '\x28', '\xEB', '\x7E', '\xD3', '\x20', '\xA9', '\x88', '\x32', '\x28', '\xE5', '\x69', '\xC4', '\x3F', '\xBA', '\x93', '\x2E', '\x23', '\xF6', '\x77', '\xD4', '\x2C', '\xBA', '\x9E', '\x23', '\x24', '\xFD', '\x68', '\xD3', '\x28', '\xFF'};
  thread_local EncryptedName<sizeof(source), 0x6F9D21B96D71C1FFULL> name(source);
  return name;
}

// OFFSET: 0x00E2E640
// FUNCTION: fn_24142
EncryptedName<30, 0x39E7F13755252F8FULL>& SpiritHandlerBodyPath2() {
  const char source[] = {'\xDC', '\x7F', '\x6C', '\x07', '\x7E', '\xA5', '\xB8', '\x71', '\xCE', '\x61', '\x61', '\x19', '\x72', '\xA3', '\xBB', '\x6A', '\xDF', '\x66', '\x77', '\x1C', '\x63', '\xAE', '\xAF', '\x78', '\xC1', '\x6B', '\x69', '\x10', '\x65', '\xF1'};
  thread_local EncryptedName<sizeof(source), 0x39E7F13755252F8FULL> name(source);
  return name;
}

// OFFSET: 0x00E2E720
// FUNCTION: fn_24143
EncryptedName<37, 0xCBD7DDAFDF87F549ULL>& SpiritHandlerRidingBodyPath2() {
  const char source[] = {'\x1A', '\xA5', '\xCE', '\x8D', '\xE6', '\x89', '\x88', '\x83', '\x08', '\xBB', '\xC3', '\x93', '\xEA', '\x8F', '\x8B', '\x98', '\x19', '\xBC', '\xD5', '\x96', '\xFB', '\x82', '\x9F', '\x8A', '\x07', '\xB1', '\xCB', '\x9A', '\xFD', '\x82', '\x85', '\x82', '\x0D', '\xBC', '\xC9', '\x98', '\xAF'};
  thread_local EncryptedName<sizeof(source), 0xCBD7DDAFDF87F549ULL> name(source);
  return name;
}

// OFFSET: 0x00E2E800
// FUNCTION: fn_24144
EncryptedName<12, 0xEB074B7FBDA3BFD5ULL>& DruidBodyPath3() {
  const char source[] = {'\x91', '\xED', '\xF6', '\xF4', '\x3B', '\x17', '\x43', '\xB9', '\x80', '\xF6', '\xE7', '\xBD'};
  thread_local EncryptedName<sizeof(source), 0xEB074B7FBDA3BFD5ULL> name(source);
  return name;
}

// OFFSET: 0x00E2E8D0
// FUNCTION: fn_24145
EncryptedName<12, 0x3D075BD5256F9B6FULL>& DruidBodyPath4() {
  const char source[] = {'\x2B', '\xC9', '\x3A', '\x6C', '\x91', '\x07', '\x43', '\x6F', '\x3A', '\xD2', '\x2B', '\x25'};
  thread_local EncryptedName<sizeof(source), 0x3D075BD5256F9B6FULL> name(source);
  return name;
}

// OFFSET: 0x00E2E9A0
// FUNCTION: fn_24146
EncryptedName<14, 0x251761D7370BA5C3ULL>& KarnosBodyPath3() {
  const char source[] = {'\x88', '\xE4', '\x59', '\x79', '\x98', '\x32', '\x4B', '\x6E', '\x82', '\xF7', '\x45', '\x78', '\x84', '\x61'};
  thread_local EncryptedName<sizeof(source), 0x251761D7370BA5C3ULL> name(source);
  return name;
}

// OFFSET: 0x00E2EA70
// FUNCTION: fn_24147
EncryptedName<14, 0x03014749B127914FULL>& KarnosBodyPath4() {
  const char source[] = {'\x04', '\xD0', '\x75', '\xFF', '\x06', '\x14', '\x5D', '\x48', '\x0E', '\xC3', '\x69', '\xFE', '\x1A', '\x47'};
  thread_local EncryptedName<sizeof(source), 0x03014749B127914FULL> name(source);
  return name;
}

// OFFSET: 0x00E2EB40
// FUNCTION: fn_24148
EncryptedName<14, 0x85FB93CF2F494179ULL>& AliteaBodyPath2() {
  const char source[] = {'\x38', '\x0D', '\x00', '\x7B', '\x8A', '\xD2', '\xA7', '\xC4', '\x35', '\x08', '\x1D', '\x6A', '\x8E', '\x93'};
  thread_local EncryptedName<sizeof(source), 0x85FB93CF2F494179ULL> name(source);
  return name;
}

// OFFSET: 0x00DD6970
// FUNCTION: fn_23276
EncryptedName<11, 0x61F59D5D41E76519ULL>& WereRaptorJobName1() {
  const char source[] = {'\x4E', '\x20', '\xB5', '\x04', '\x0F', '\xDC', '\xA5', '\x35', '\x56', '\x37', '\xE7'};
  thread_local EncryptedName<sizeof(source), 0x61F59D5D41E76519ULL> name(source);
  return name;
}
