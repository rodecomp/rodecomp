#ifndef RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICEENUMS_H
#define RAGNAROK_SOURCE_CONTENTS_CONFIGURATION_SERVICEENUMS_H

enum ServiceType : int {
  ServiceKorea = 0,
  ServiceJapan = 2,
  ServiceChina = 3,
  ServiceTaiwan = 4,
  ServiceThai = 5,
  ServiceRussia = 14,
  ServiceVietnam = 15,
  ServiceChile = 17,
  ServiceAmerica = 1,
  ServiceIndonesia = 6,
  ServicePhilippine = 7,
  ServiceMalaysia = 8,
  ServiceSingapore = 9,
  ServiceGermany = 10,
  ServiceIndia = 11,
  ServiceBrazil = 12,
  ServiceAustralia = 13,
  ServiceFrance = 18,
  ServiceUae = 19,
  ServiceEu = 20,
};

enum Language : signed char {
  LANGUAGE_KOREAN = 0,
  LANGUAGE_JAPANESE = 2,
  LANGUAGE_CHINESE = 3,
  LANGUAGE_TAIWANESE = 4,
  LANGUAGE_THAI = 5,
  LANGUAGE_VIETNAMESE = 7,
  LANGUAGE_ARABIC = 12,
};

enum ServerType : int {
  ServerPrimary = 0,
  ServerSakray = 1,
  ServerLocal = 2,
  ServerInstantEvent = 3,
  ServerPvp = 4,
  ServerPk = 5,
  ServerStage = 13,
};

#endif
