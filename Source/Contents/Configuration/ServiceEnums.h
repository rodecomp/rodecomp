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

enum ServerType : int {
  ServerPrimary = 0,
  ServerSakray = 1,
  ServerLocal = 2,
  ServerPk = 5,
};

#endif
