#include "Client/Session.h"
#include "Contents/Configuration/ServiceOptions.h"
#include "Contents/EquipmentResourceMgr.h"
#include "Contents/Items/ItemDescription.h"
#include "Contents/Skills/SkillInfo.h"
#include "Contents/Sound/SoundMgr.h"

#include <string>
#include <utility>

// OFFSET: 0x00E13A20
// FUNCTION: fn_23824
std::string CSession::GetRobeActName(int job, int sex, int robe, bool riding, int unused, int costumeJob) {
  return g_equipmentResourceMgr->GetRobeResourceName(0, job, sex, robe, riding, unused, costumeJob);
}

// OFFSET: 0x00E13A90
// FUNCTION: fn_23825
std::string CSession::GetRobeSprName(int job, int sex, int robe, bool riding, int unused, int costumeJob) {
  return g_equipmentResourceMgr->GetRobeResourceName(1, job, sex, robe, riding, unused, costumeJob);
}

// OFFSET: 0x00E13B20
// FUNCTION: fn_23827
std::string CSession::GetRobeName(unsigned int robe) {
  if (robe >= this->m_robeNames.size()) {
    return this->m_robeNames[0];
  }

  return this->m_robeNames[robe];
}

// OFFSET: 0x00E13BF0
// FUNCTION: fn_23828
int CSession::GetItemDescription40(int id) {
  return FindItemDescription(id).unknown_0040;
}

// OFFSET: 0x00E13C10
// FUNCTION: fn_23829
std::uint32_t CSession::GetUnknown0420(int index) {
  return this->unknown_0420[index];
}

// OFFSET: 0x00E13C30
// FUNCTION: fn_23830
void CSession::GetSkillActionInfo(int skillId, int& effectId, int& motion, int variant, int level) {
  effectId = 16;
  motion = 8;
  const int beginEffect = this->GetBeginEffectId(skillId);
  switch (skillId) {
  case 3006:
  case 3009:
    motion = 36;
    break;
  case 3011:
  case 3013:
  case 3014:
  case 3015:
  case 3016:
  case 3017:
  case 3018:
  case 3021:
  case 3026:
  case 3027:
  case 3029:
    motion = 37;
    break;
  }

  if (beginEffect != -1) {
    effectId = beginEffect;
    return;
  }

  switch (skillId) {
  case 1:
  case 2:
  case 3:
  case 4:
  case 6:
  case 8:
  case 36:
  case 37:
  case 38:
  case 39:
  case 40:
  case 41:
  case 43:
  case 44:
  case 48:
  case 49:
  case 50:
  case 52:
  case 53:
  case 94:
  case 95:
  case 96:
  case 97:
  case 98:
  case 99:
  case 100:
  case 101:
  case 102:
  case 103:
  case 104:
  case 105:
  case 106:
  case 107:
  case 109:
  case 111:
  case 112:
  case 113:
  case 114:
  case 126:
  case 127:
  case 128:
  case 129:
  case 130:
  case 131:
  case 132:
  case 133:
  case 134:
  case 135:
  case 137:
  case 138:
  case 139:
  case 140:
  case 459:
  case 486:
  case 2033:
    break;
  case 5:
  case 7:
  case 42:
  case 46:
  case 110:
  case 499:
  case 1009:
  case 8202:
    motion = 2;
    break;
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
  case 16:
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
  case 22:
  case 23:
  case 24:
  case 25:
  case 26:
  case 27:
  case 28:
  case 30:
  case 31:
  case 32:
  case 33:
  case 35:
  case 54:
  case 66:
  case 67:
  case 68:
  case 69:
  case 70:
  case 72:
  case 73:
  case 74:
  case 75:
  case 76:
  case 77:
  case 78:
  case 79:
  case 80:
  case 81:
  case 82:
  case 83:
  case 84:
  case 85:
  case 86:
  case 87:
  case 88:
  case 89:
  case 90:
  case 91:
  case 92:
  case 108:
  case 156:
  case 157:
  case 254:
  case 255:
  case 256:
  case 261:
  case 262:
  case 266:
  case 267:
  case 277:
  case 279:
  case 280:
  case 281:
  case 282:
  case 283:
  case 285:
  case 286:
  case 287:
  case 288:
  case 289:
  case 290:
  case 307:
  case 309:
  case 310:
  case 312:
  case 313:
  case 339:
  case 340:
  case 341:
  case 342:
  case 365:
  case 373:
  case 374:
  case 375:
  case 400:
  case 401:
  case 402:
  case 403:
  case 405:
  case 482:
  case 484:
  case 669:
  case 694:
  case 707:
  case 723:
  case 729:
  case 731:
  case 738:
  case 740:
  case 741:
  case 769:
  case 770:
  case 1006:
  case 1015:
  case 2001:
  case 2004:
  case 2006:
  case 2008:
  case 2011:
  case 2012:
  case 2014:
  case 2015:
  case 2018:
  case 2019:
  case 2020:
  case 2021:
  case 2036:
  case 2040:
  case 2043:
  case 2044:
  case 2046:
  case 2051:
  case 2053:
  case 2054:
  case 2057:
  case 2301:
  case 2533:
  case 2534:
  case 2536:
  case 2537:
  case 2579:
  case 2584:
  case 5004:
  case 5028:
  case 5042:
  case 5210:
  case 5268:
  case 8222:
  case 8445:
  case 8448:
  case 8451:
  case 8454:
  case 8457:
    effectId = 12;
    goto variant;
  case 29:
  case 690:
  case 708:
  case 720:
  case 728:
  case 2201:
  case 2203:
  case 2205:
  case 2207:
  case 2210:
  case 2211:
  case 2216:
  case 2230:
  case 2233:
  case 2237:
  case 2243:
  case 2244:
  case 2246:
    effectId = 12;
    motion = 7;
    break;
  case 34:
  case 689:
    effectId = 12;
    motion = 0;
    break;
  case 45:
    effectId = 43;
    return;
  case 47:
    effectId = -1;
    motion = 18;
    break;
  case 51:
  case 55:
  case 60:
  case 63:
  case 64:
  case 93:
  case 158:
  case 159:
  case 160:
  case 161:
  case 162:
  case 163:
  case 164:
  case 165:
  case 166:
  case 167:
  case 168:
  case 169:
  case 170:
  case 171:
  case 172:
  case 173:
  case 174:
  case 175:
  case 176:
  case 177:
  case 178:
  case 179:
  case 180:
  case 181:
  case 182:
  case 183:
  case 184:
  case 185:
  case 186:
  case 187:
  case 188:
  case 189:
  case 190:
  case 191:
  case 192:
  case 258:
  case 272:
  case 273:
  case 338:
  case 347:
  case 368:
  case 370:
  case 371:
  case 372:
  case 398:
  case 399:
  case 495:
  case 654:
  case 663:
  case 666:
  case 667:
  case 668:
  case 675:
  case 676:
  case 678:
  case 721:
  case 724:
  case 757:
  case 758:
  case 759:
  case 760:
  case 762:
  case 782:
  case 783:
  case 1016:
  case 2005:
  case 2038:
  case 2278:
  case 5214:
  case 5215:
  case 5216:
  case 5217:
  case 5218:
  case 5220:
  case 5221:
  case 5222:
  case 5225:
  case 5227:
  case 5229:
  case 5230:
  case 5232:
  case 5233:
  case 5234:
  case 5235:
  case 5237:
  case 5241:
  case 5244:
  case 5246:
  case 5247:
  case 5254:
  case 5269:
  case 5271:
  case 5272:
  case 5273:
  case 5275:
  case 5278:
  case 5280:
  case 5281:
  case 5282:
  case 5313:
  case 5315:
  case 5316:
  case 5318:
  case 5319:
  case 5329:
  case 5332:
  case 5338:
  case 5339:
  case 5340:
  case 5341:
  case 5342:
  case 5343:
  case 5344:
  case 5345:
  case 5346:
  case 5347:
  case 5348:
  case 5357:
  case 5361:
  case 5367:
  case 5368:
  case 5369:
  case 5370:
  case 5371:
  case 5372:
  case 5373:
  case 5375:
  case 5376:
  case 5377:
  case 5378:
  case 5379:
  case 5380:
  case 5382:
  case 5389:
  case 5390:
  case 5391:
  case 5392:
  case 5393:
  case 8444:
  case 8447:
  case 8450:
  case 8453:
  case 8456:
  case 8603:
  case 8604:
  case 8605:
    effectId = -1;
    return;
  case 56:
  case 58:
  case 62:
  case 2055:
    effectId = -1;
    motion = 2;
    break;
  case 57:
    effectId = 144;
    motion = 2;
    break;
  case 59:
    effectId = -1;
    motion = 12;
    break;
  case 61:
  case 152:
  case 1004:
    effectId = -1;
    motion = 11;
    break;
  case 115:
  case 116:
  case 117:
  case 118:
  case 119:
  case 120:
  case 121:
  case 122:
  case 123:
  case 124:
  case 125:
  case 688:
  case 2239:
  case 2253:
  case 2254:
    motion = 5;
    break;
  case 136:
  case 737:
  case 2022:
  case 2056:
  case 2202:
  case 2212:
  case 2214:
  case 2236:
  case 2317:
  case 2321:
    effectId = -1;
    motion = 7;
    break;
  case 141:
  case 344:
  case 345:
  case 346:
  case 350:
  case 379:
  case 406:
  case 661:
  case 712:
    effectId = 59;
    return;
  case 149:
    motion = 11;
    break;
  case 151:
    effectId = -1;
    motion = 5;
    break;
  case 196:
  case 209:
  case 249:
  case 270:
  case 349:
  case 8220:
    motion = 0;
    effectId = -1;
    return;
  case 230:
  case 231:
  case 232:
  case 234:
  case 235:
  case 236:
  case 237:
  case 268:
  case 479:
    motion = 0;
    effectId = 12;
    goto variant;
  case 243:
  case 244:
  case 247:
    motion = 0;
    effectId = 342;
    return;
  case 271:
    effectId = level < 4016 ? 328 : 510;
    break;
  case 306:
  case 308:
  case 311:
  case 361:
  case 362:
  case 367:
  case 383:
  case 480:
  case 483:
  case 489:
  case 691:
  case 2013:
  case 2050:
  case 2515:
  case 2582:
  case 2585:
  case 2587:
  case 5073:
  case 5381:
    effectId = 58;
    return;
  case 317:
  case 319:
  case 320:
  case 321:
  case 322:
  case 411:
  case 427:
  case 434:
  case 444:
  case 445:
  case 447:
  case 448:
  case 449:
  case 450:
  case 451:
  case 452:
  case 453:
  case 454:
  case 455:
  case 456:
  case 457:
  case 458:
  case 460:
  case 461:
  case 462:
  case 463:
  case 464:
  case 465:
  case 467:
  case 468:
  case 469:
  case 470:
  case 471:
  case 472:
  case 494:
  case 572:
  case 573:
  case 574:
  case 575:
  case 2039:
  case 2299:
  case 2300:
  case 2303:
  case 2596:
  case 2597:
  case 2598:
  case 2599:
  case 2605:
  case 2607:
  case 2608:
    effectId = 441;
    return;
  case 325:
  case 327:
  case 328:
  case 329:
  case 330:
  case 334:
  case 336:
  case 408:
  case 409:
  case 410:
  case 5063:
  case 5064:
  case 5065:
    effectId = 342;
    return;
  case 335:
    effectId = 343;
    return;
  case 382:
  case 394:
    motion = 18;
    effectId = 58;
    return;
  case 389:
    motion = 0;
    effectId = 1195;
    return;
  case 397:
    effectId = 502;
    motion = 11;
    break;
  case 425:
  case 493:
    motion = 0;
    effectId = 441;
    return;
  case 426:
  case 5023:
    motion = 19;
    effectId = -1;
    return;
  case 446:
    motion = 12;
    effectId = 12;
    goto variant;
  case 475:
    motion = 0;
    effectId = 59;
    return;
  case 478:
  case 490:
    motion = 28;
    effectId = 12;
    goto variant;
  case 496:
    motion = 7;
    effectId = 573;
    return;
  case 497:
    motion = 7;
    effectId = 574;
    return;
  case 498:
    motion = 7;
    effectId = 575;
    return;
  case 503:
  case 504:
  case 505:
  case 512:
  case 513:
  case 514:
  case 515:
  case 517:
    motion = 41;
    effectId = 441;
    return;
  case 518:
  case 519:
  case 520:
  case 521:
    motion = 42;
    effectId = 441;
    return;
  case 525:
  case 532:
  case 543:
    motion = 37;
    effectId = 441;
    return;
  case 534:
  case 535:
  case 536:
    motion = 37;
    effectId = 55;
    return;
  case 537:
  case 538:
  case 539:
    motion = 37;
    effectId = 54;
    return;
  case 540:
  case 541:
  case 542:
    motion = 37;
    effectId = 57;
    return;
  case 555:
  case 2290:
  case 2304:
    motion = 0;
    effectId = 501;
    return;
  case 735:
  case 2476:
  case 2480:
  case 2494:
  case 2495:
  case 2496:
  case 2497:
    motion = -1;
    effectId = 911;
    return;
  case 736:
  case 739:
  case 742:
  case 2256:
  case 2258:
  case 2260:
  case 2262:
  case 2268:
  case 2447:
  case 2448:
  case 2449:
  case 2450:
  case 2451:
  case 2452:
  case 2453:
  case 2455:
  case 2456:
  case 2457:
  case 2458:
  case 2459:
  case 2460:
    motion = -1;
    effectId = 12;
    goto variant;
  case 744:
  case 2444:
    motion = -1;
    effectId = 57;
    return;
  case 745:
  case 2270:
  case 2443:
    motion = -1;
    effectId = 55;
    return;
  case 1001:
  case 1010:
  case 1014:
    motion = 0;
    effectId = 58;
    return;
  case 1008:
    effectId = 54;
    return;
  case 1011:
  case 2413:
    motion = 16;
    effectId = -1;
    return;
  case 1017:
    effectId = 56;
    return;
  case 1018:
    effectId = 55;
    return;
  case 1019:
    effectId = 57;
    return;
  case 2002:
    effectId = 12;
    motion = 2;
    break;
  case 2032:
  case 2234:
  case 5002:
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 2041:
  case 2042:
  case 2045:
  case 2047:
  case 2048:
  case 5072:
    effectId = 830;
    return;
  case 2052:
    effectId = 1560;
    return;
  case 2204:
    effectId = 1449;
    motion = 7;
    break;
  case 2206:
  case 2231:
  case 2313:
  case 2314:
  case 2315:
  case 2318:
  case 2319:
  case 2320:
  case 2324:
  case 2325:
  case 5078:
    effectId = 58;
    motion = 7;
    break;
  case 2209:
  case 2240:
  case 2286:
    effectId = 441;
    motion = 7;
    break;
  case 2213:
    effectId = 1418;
    motion = 7;
    break;
  case 2217:
    effectId = 805;
    motion = 7;
    break;
  case 2222:
  case 2223:
  case 2224:
  case 2229:
    effectId = g_serviceOptions.Get(229) ? 1430 : 1260;
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    motion = 7;
    break;
  case 2241:
  case 2242:
  case 2477:
  case 2520:
    effectId = -1;
    motion = -1;
    break;
  case 2259:
    motion = 2;
    effectId = 12;
    goto variant;
  case 2261:
    motion = 2;
    effectId = -1;
    return;
  case 2267:
    motion = -1;
    effectId = 775;
    return;
  case 2271:
  case 2273:
  case 2275:
    motion = -1;
    effectId = 58;
    return;
  case 2272:
    motion = -1;
    effectId = 54;
    return;
  case 2274:
    motion = -1;
    effectId = 441;
    return;
  case 2280:
    motion = 2;
    effectId = 775;
    return;
  case 2287:
    effectId = 826;
    motion = 7;
    break;
  case 2288:
    effectId = 441;
    motion = 18;
    break;
  case 2292:
  case 2293:
  case 2294:
  case 2295:
  case 2296:
  case 2297:
  case 2298:
    motion = 7;
    effectId = 441;
    return;
  case 2302:
    effectId = 1556;
    return;
  case 2316:
    effectId = 882;
    motion = 7;
    break;
  case 2322:
    effectId = 830;
    motion = 7;
    break;
  case 2323:
    effectId = 1709;
    motion = 7;
    break;
  case 2326:
  case 2327:
  case 2328:
  case 2329:
  case 2330:
  case 2332:
  case 2338:
  case 2340:
  case 2341:
  case 2344:
  case 2345:
  case 2346:
  case 2347:
  case 2348:
  case 2518:
    motion = -1;
    effectId = 831;
    return;
  case 2343:
  case 2492:
  case 2517:
    motion = -1;
    effectId = -1;
    return;
  case 2350:
  case 2351:
  case 2417:
  case 2420:
  case 2422:
  case 2424:
  case 2425:
    motion = 16;
    effectId = 840;
    return;
  case 2352:
    motion = 16;
    effectId = 840;
    g_soundMgr->PlayWave("effect\\달빛세레나데.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 2381:
  case 2382:
    motion = 16;
    effectId = 844;
    return;
  case 2383:
    motion = 16;
    effectId = 441;
    return;
  case 2414:
    motion = 16;
    effectId = 1656;
    return;
  case 2418:
    motion = 16;
    effectId = 1286;
    return;
  case 2419:
    motion = 16;
    effectId = 840;
    g_soundMgr->PlayWave("effect\\나락의노래.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 2421:
  case 2423:
    motion = 16;
    effectId = 841;
    return;
  case 2426:
  case 2427:
  case 2428:
  case 2430:
  case 2432:
  case 2433:
  case 2434:
    motion = 16;
    effectId = 843;
    return;
  case 2429:
    effectId = -1;
    motion = 16;
    break;
  case 2431:
    motion = 16;
    effectId = 843;
    g_soundMgr->PlayWave("effect\\레라드의이슬.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 2446:
    motion = -1;
    effectId = 1664;
    return;
  case 2454:
    motion = 7;
    effectId = 1565;
    return;
  case 2478:
    motion = -1;
    effectId = 1677;
    return;
  case 2479:
  case 2483:
  case 2486:
  case 2490:
    motion = 7;
    effectId = 911;
    return;
  case 2481:
  case 2485:
    motion = 12;
    effectId = 911;
    return;
  case 2482:
    motion = 7;
    effectId = -1;
    return;
  case 2564:
    motion = 46;
    effectId = -1;
    return;
  case 2581:
    motion = 19;
    effectId = 58;
    return;
  case 2600:
  case 2601:
  case 2602:
  case 2603:
  case 2604:
  case 2606:
  case 2610:
    effectId = 454;
    return;
  case 5026:
  case 5027:
  case 5032:
  case 5033:
  case 5035:
  case 5036:
  case 5038:
  case 5040:
  case 5043:
    motion = 50;
    effectId = -1;
    return;
  case 5238:
    effectId = 1986;
    return;
  case 5239:
    effectId = 1998;
    return;
  case 5242:
    effectId = 1963;
    return;
  case 5279:
    effectId = 1959;
    return;
  case 5284:
    effectId = 1932;
    return;
  case 5294:
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    effectId = -1;
    return;
  case 5302:
    effectId = 1938;
    return;
  case 5303:
    effectId = 1918;
    return;
  case 5304:
    effectId = 1902;
    return;
  case 5305:
    effectId = 1910;
    return;
  case 5321:
    effectId = -1;
    motion = -1;
    g_soundMgr->PlayWave("effect\\EF_BeginSpell.wav", 0.0f, 0.0f, 0.0f, 250, 40, 1.0f, 0);
    break;
  case 5331:
    effectId = 1872;
    return;
  case 5333:
    effectId = 1876;
    return;
  case 5335:
    effectId = 1923;
    return;
  case 5384:
  case 5385:
    effectId = 2249;
    return;
  case 5387:
    effectId = 2240;
    return;
  case 5388:
    effectId = 2255;
    return;
  case 8401:
  case 8425:
  case 8426:
  case 8428:
    motion = 7;
    effectId = 55;
    return;
  case 8430:
  case 8431:
  case 8433:
    motion = 7;
    effectId = 54;
    return;
  case 8434:
  case 8435:
    motion = 7;
    effectId = 57;
    return;
  case 8442:
    motion = 7;
    effectId = 56;
    return;
  default:
    if (static_cast<unsigned int>(skillId - 5401) <= 106 || static_cast<unsigned int>(skillId - 6001) <= 5 || static_cast<unsigned int>(skillId - 6525) <= 81) {
      effectId = -1;
    }
    {
      const std::pair<bool, int> action = CSkillActionMgr::GetInstance()->ActionSlot01(skillId);
      if (action.first == true) {
        motion = action.second;
      }
    }
    break;
  }

  if (effectId != 12) {
    return;
  }

variant:
  switch (variant) {
  case 1:
    effectId = 54;
    return;
  case 2:
    effectId = 56;
    return;
  case 3:
    effectId = 55;
    return;
  case 4:
    effectId = 57;
    return;
  case 5:
    effectId = 59;
    return;
  case 6:
    effectId = 58;
    return;
  }
}

// OFFSET: 0x00E15270
// FUNCTION: fn_23831
void CSession::GetSkillActionInfo2(int skillId, int& effectId, int& motion) {
  effectId = 16;
  motion = 7;
  const int beginEffect = this->GetBeginEffectId(skillId);
  switch (skillId) {
  case 3004:
    motion = 9;
    break;
  case 3005:
  case 3006:
  case 3007:
  case 3009:
  case 3020:
  case 3022:
  case 3023:
    motion = 36;
    break;
  case 3011:
  case 3013:
  case 3014:
  case 3015:
  case 3016:
  case 3017:
  case 3018:
  case 3021:
  case 3026:
  case 3027:
  case 3029:
    motion = 37;
    break;
  }

  if (beginEffect != -1) {
    effectId = beginEffect;
    return;
  }

  switch (skillId) {
  case 1:
  case 2:
  case 3:
  case 4:
  case 6:
  case 8:
  case 36:
  case 37:
  case 38:
  case 39:
  case 40:
  case 41:
  case 43:
  case 44:
  case 48:
  case 49:
  case 50:
  case 52:
  case 53:
  case 355:
    return;
  case 5:
  case 7:
  case 42:
  case 46:
  case 366:
  case 499:
  case 1009:
  case 8202:
    motion = 2;
    return;
  case 9:
  case 10:
  case 11:
  case 12:
  case 13:
  case 14:
  case 15:
  case 16:
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
  case 22:
  case 23:
  case 24:
  case 25:
  case 26:
  case 27:
  case 30:
  case 31:
  case 32:
  case 33:
  case 35:
  case 54:
  case 65:
  case 66:
  case 67:
  case 68:
  case 69:
  case 70:
  case 71:
  case 72:
  case 73:
  case 74:
  case 75:
  case 76:
  case 77:
  case 78:
  case 79:
  case 80:
  case 81:
  case 82:
  case 83:
  case 84:
  case 85:
  case 86:
  case 87:
  case 88:
  case 89:
  case 90:
  case 91:
  case 92:
  case 340:
  case 341:
  case 342:
  case 400:
  case 483:
  case 669:
  case 694:
  case 723:
  case 769:
  case 770:
  case 1006:
  case 2533:
  case 2534:
  case 2536:
  case 2537:
  case 8222:
    effectId = 12;
    return;
  case 28:
  case 34:
  case 136:
  case 156:
  case 157:
  case 334:
  case 335:
  case 408:
  case 487:
  case 489:
  case 689:
  case 2051:
  case 5063:
  case 5064:
  case 5065:
    effectId = -1;
    motion = 7;
    return;
  case 29:
  case 151:
  case 304:
  case 690:
    effectId = -1;
    motion = 0;
    return;
  case 45:
    effectId = 43;
    return;
  case 47:
  case 212:
  case 214:
  case 219:
  case 253:
  case 266:
  case 267:
  case 338:
  case 367:
  case 368:
  case 379:
  case 382:
  case 406:
  case 502:
  case 503:
  case 512:
  case 513:
  case 514:
  case 515:
  case 518:
  case 519:
  case 520:
  case 661:
  case 674:
  case 712:
  case 728:
  case 732:
  case 740:
  case 1001:
  case 1005:
  case 2004:
  case 2005:
  case 2006:
  case 2017:
  case 2023:
  case 2030:
  case 2031:
  case 2036:
  case 2037:
  case 2233:
  case 2236:
  case 2259:
  case 2261:
  case 2280:
  case 2284:
  case 2288:
  case 2298:
  case 2314:
  case 2317:
  case 2320:
  case 2324:
  case 2342:
  case 2476:
  case 5245:
  case 5248:
  case 5249:
  case 5250:
  case 5277:
  case 5283:
  case 5287:
  case 5289:
  case 5294:
  case 5295:
  case 5296:
  case 5344:
  case 5345:
  case 5346:
  case 5348:
  case 8601:
  case 8602:
  case 8603:
  case 8604:
  case 8605:
    motion = 2;
    effectId = -1;
    return;
  case 51:
  case 55:
  case 60:
  case 63:
  case 64:
  case 93:
  case 94:
  case 95:
  case 96:
  case 97:
  case 98:
  case 99:
  case 100:
  case 101:
  case 102:
  case 103:
  case 104:
  case 105:
  case 106:
  case 107:
  case 108:
  case 110:
  case 111:
  case 112:
  case 113:
  case 114:
  case 126:
  case 127:
  case 128:
  case 129:
  case 130:
  case 131:
  case 132:
  case 133:
  case 134:
  case 135:
  case 137:
  case 138:
  case 139:
  case 140:
  case 141:
  case 153:
  case 158:
  case 159:
  case 160:
  case 161:
  case 162:
  case 163:
  case 164:
  case 165:
  case 166:
  case 167:
  case 168:
  case 169:
  case 170:
  case 171:
  case 172:
  case 173:
  case 174:
  case 175:
  case 176:
  case 177:
  case 178:
  case 179:
  case 180:
  case 181:
  case 182:
  case 183:
  case 184:
  case 185:
  case 186:
  case 187:
  case 188:
  case 189:
  case 190:
  case 191:
  case 192:
  case 234:
  case 235:
  case 236:
  case 237:
  case 258:
  case 264:
  case 271:
  case 272:
  case 273:
  case 371:
  case 383:
  case 459:
  case 479:
  case 486:
  case 495:
  case 544:
  case 654:
  case 663:
  case 666:
  case 667:
  case 668:
  case 675:
  case 676:
  case 678:
  case 721:
  case 724:
  case 757:
  case 758:
  case 759:
  case 760:
  case 762:
  case 780:
  case 783:
  case 2033:
  case 2038:
  case 2040:
  case 2055:
  case 2056:
  case 2326:
  case 2328:
  case 2329:
  case 2332:
  case 2335:
  case 2338:
  case 2341:
  case 2519:
  case 2584:
  case 2592:
  case 2602:
  case 2604:
  case 5030:
  case 5036:
  case 5043:
  case 5205:
  case 5210:
  case 5214:
  case 5220:
  case 5230:
  case 5233:
  case 5234:
  case 5235:
  case 5237:
  case 5251:
  case 5252:
  case 5253:
  case 5265:
  case 5273:
  case 5274:
  case 5284:
  case 5290:
  case 5326:
  case 5329:
  case 5336:
  case 5338:
  case 5339:
  case 5347:
  case 5356:
  case 5357:
  case 5369:
  case 5373:
  case 5380:
  case 5389:
  case 5390:
  case 5391:
  case 5392:
  case 5393:
  case 8425:
  case 8426:
  case 8428:
  case 8430:
  case 8431:
  case 8433:
  case 8434:
  case 8435:
  case 8442:
  case 8443:
  case 8444:
  case 8445:
  case 8446:
  case 8447:
  case 8448:
  case 8449:
  case 8450:
  case 8451:
  case 8452:
  case 8453:
  case 8454:
  case 8455:
  case 8456:
  case 8457:
    effectId = -1;
    return;
  case 56:
  case 58:
  case 61:
  case 62:
  case 109:
  case 263:
  case 370:
  case 372:
  case 397:
  case 398:
  case 399:
  case 484:
  case 1016:
  case 2002:
  case 2022:
  case 5314:
  case 5315:
  case 5316:
  case 5320:
  case 5322:
  case 5382:
    effectId = -1;
    motion = 2;
    return;
  case 57:
    effectId = 144;
    motion = 2;
    return;
  case 59:
  case 149:
  case 152:
  case 1004:
    effectId = -1;
    motion = 12;
    return;
  case 115:
  case 116:
  case 117:
  case 118:
  case 119:
  case 120:
  case 121:
  case 122:
  case 123:
  case 124:
  case 125:
  case 688:
  case 2238:
  case 2239:
  case 2249:
  case 2250:
  case 2251:
  case 2252:
  case 2253:
  case 2254:
  case 5335:
    motion = 5;
    return;
  case 196:
  case 209:
  case 249:
  case 252:
  case 257:
  case 268:
  case 349:
  case 350:
  case 356:
  case 380:
  case 384:
  case 387:
  case 411:
  case 444:
  case 8204:
  case 8219:
  case 8220:
    motion = 0;
    effectId = -1;
    return;
  case 229:
  case 230:
  case 231:
  case 232:
  case 250:
  case 251:
  case 480:
  case 2480:
  case 2493:
  case 5340:
  case 5341:
  case 5342:
  case 5343:
    motion = 12;
    effectId = -1;
    return;
  case 243:
  case 244:
  case 247:
  case 337:
  case 344:
  case 345:
  case 346:
  case 365:
  case 373:
  case 374:
  case 375:
  case 381:
  case 389:
  case 402:
  case 403:
  case 425:
  case 427:
  case 434:
  case 462:
  case 463:
  case 464:
  case 465:
  case 467:
  case 468:
  case 469:
  case 470:
  case 471:
  case 472:
  case 485:
  case 493:
  case 720:
  case 731:
  case 735:
  case 736:
  case 737:
  case 738:
  case 739:
  case 741:
  case 742:
  case 744:
  case 745:
  case 2001:
  case 2008:
  case 2009:
  case 2012:
  case 2013:
  case 2014:
  case 2019:
  case 2020:
  case 2021:
  case 2202:
  case 2203:
  case 2204:
  case 2211:
  case 2212:
  case 2214:
  case 2217:
  case 2230:
  case 2231:
  case 2237:
  case 2243:
  case 2244:
  case 2246:
  case 2286:
  case 2287:
  case 2289:
  case 2290:
  case 2291:
  case 2292:
  case 2293:
  case 2294:
  case 2295:
  case 2296:
  case 2297:
  case 2299:
  case 2300:
  case 2301:
  case 2302:
  case 2303:
  case 2304:
  case 2309:
  case 2310:
  case 2315:
  case 2316:
  case 2318:
  case 2319:
  case 2321:
  case 2322:
  case 2325:
  case 2350:
  case 2351:
  case 2352:
  case 2381:
  case 2382:
  case 2383:
  case 2413:
  case 2414:
  case 2417:
  case 2418:
  case 2419:
  case 2420:
  case 2422:
  case 2426:
  case 2429:
  case 2443:
  case 2444:
  case 2446:
  case 2447:
  case 2448:
  case 2449:
  case 2450:
  case 2451:
  case 2452:
  case 2453:
  case 2454:
  case 2455:
  case 2456:
  case 2457:
  case 2458:
  case 2459:
  case 2460:
  case 2477:
  case 2479:
  case 2483:
  case 2492:
  case 2518:
  case 5004:
  case 5078:
  case 8009:
  case 8012:
  case 8401:
    motion = 7;
    effectId = -1;
    return;
  case 256:
  case 261:
  case 269:
  case 369:
  case 401:
  case 730:
  case 1015:
  case 2334:
    motion = 0;
    return;
  case 306:
  case 307:
  case 308:
  case 309:
  case 310:
  case 311:
  case 312:
  case 313:
  case 325:
  case 327:
  case 328:
  case 329:
  case 330:
  case 395:
  case 396:
  case 488:
    effectId = -1;
    motion = 16;
    return;
  case 316:
  case 324:
  case 394:
  case 530:
  case 2258:
  case 2307:
  case 2308:
  case 2312:
  case 5330:
    motion = 9;
    effectId = -1;
    return;
  case 317:
  case 319:
  case 320:
  case 321:
  case 322:
    effectId = -1;
    motion = 17;
    return;
  case 347:
    effectId = 454;
    return;
  case 357:
  case 359:
  case 360:
  case 361:
  case 362:
  case 405:
  case 482:
  case 691:
  case 8206:
  case 8414:
  case 8415:
  case 8416:
  case 8417:
  case 8418:
  case 8419:
  case 8420:
  case 8421:
  case 8422:
  case 8423:
  case 8424:
    motion = 7;
    return;
  case 364:
    motion = 1;
    effectId = -1;
    return;
  case 412:
  case 2015:
  case 2285:
    motion = 21;
    effectId = -1;
    return;
  case 413:
  case 2337:
  case 2576:
    motion = 30;
    effectId = -1;
    return;
  case 414:
  case 2478:
    motion = 22;
    effectId = -1;
    return;
  case 415:
  case 2313:
    motion = 31;
    effectId = -1;
    return;
  case 416:
  case 445:
  case 447:
  case 448:
  case 449:
  case 450:
  case 451:
  case 452:
  case 453:
  case 454:
  case 455:
  case 456:
  case 457:
  case 458:
  case 460:
  case 461:
  case 494:
  case 572:
  case 573:
  case 574:
  case 575:
  case 2333:
  case 2340:
  case 2596:
  case 2597:
  case 2598:
  case 2599:
    motion = 23;
    effectId = -1;
    return;
  case 417:
  case 2311:
  case 2344:
  case 2345:
  case 2346:
  case 2347:
  case 2348:
  case 2593:
    motion = 32;
    effectId = -1;
    return;
  case 418:
  case 2018:
  case 2343:
    motion = 24;
    effectId = -1;
    return;
  case 419:
    motion = 26;
    effectId = -1;
    return;
  case 420:
  case 2323:
    motion = 25;
    effectId = -1;
    return;
  case 421:
  case 2336:
    motion = 27;
    effectId = -1;
    return;
  case 426:
  case 2581:
  case 5023:
    motion = 20;
    effectId = -1;
    return;
  case 428:
  case 429:
  case 430:
    motion = 34;
    effectId = -1;
    return;
  case 431:
  case 432:
  case 433:
    motion = 33;
    effectId = -1;
    return;
  case 446:
  case 478:
  case 490:
  case 2215:
  case 2218:
  case 2219:
  case 2220:
  case 2221:
  case 2240:
  case 2241:
  case 2242:
  case 2415:
  case 2416:
  case 2481:
  case 2482:
  case 2484:
  case 2485:
  case 2486:
  case 2490:
  case 2516:
  case 5219:
  case 5223:
  case 5224:
  case 5226:
  case 5231:
  case 5236:
  case 5243:
  case 5244:
    motion = -1;
    effectId = -1;
    return;
  case 500:
  case 2256:
  case 2260:
  case 2271:
  case 2272:
    motion = 39;
    effectId = -1;
    return;
  case 501:
  case 504:
  case 505:
  case 506:
  case 516:
  case 517:
    motion = 38;
    effectId = -1;
    return;
  case 508:
  case 521:
    motion = 40;
    effectId = -1;
    return;
  case 523:
  case 524:
  case 525:
  case 526:
  case 531:
  case 534:
  case 539:
  case 540:
  case 541:
  case 542:
  case 543:
  case 3006:
  case 3007:
  case 3008:
    motion = 36;
    effectId = -1;
    return;
  case 527:
  case 528:
  case 529:
  case 532:
  case 535:
  case 536:
  case 537:
  case 538:
    motion = 35;
    effectId = -1;
    return;
  case 707:
  case 2003:
  case 2011:
    motion = 11;
    return;
  case 719:
    effectId = 49;
    return;
  case 1011:
    motion = 16;
    effectId = -1;
    return;
  case 1013:
    motion = 5;
    effectId = -1;
    return;
  case 2027:
    motion = 7;
    effectId = 768;
    return;
  case 2028:
    motion = -1;
    return;
  case 2029:
    motion = 2;
    effectId = 768;
    return;
  case 2032:
    motion = 7;
    effectId = 771;
    return;
  case 2034:
  case 2262:
  case 8404:
  case 8406:
    motion = 7;
    effectId = 441;
    return;
  case 2257:
  case 2445:
    motion = 39;
    effectId = 441;
    return;
  case 2263:
  case 2264:
  case 2265:
    motion = -1;
    effectId = 441;
    return;
  case 2267:
    motion = 5;
    effectId = 441;
    return;
  case 2268:
  case 2273:
  case 2274:
    motion = 43;
    effectId = 441;
    return;
  case 2269:
    motion = 25;
    effectId = 441;
    return;
  case 2270:
    motion = 44;
    effectId = 548;
    return;
  case 2275:
    motion = 43;
    effectId = 58;
    return;
  case 2278:
    motion = 12;
    effectId = 12;
    return;
  case 2279:
    motion = 2;
    effectId = 12;
    return;
  case 2327:
  case 2330:
    motion = 18;
    effectId = -1;
    return;
  case 2494:
  case 2495:
  case 2496:
  case 2497:
  case 8407:
  case 8409:
    motion = 7;
    effectId = 911;
    return;
  case 2520:
    motion = -1;
    effectId = 548;
    return;
  case 2580:
    motion = 51;
    effectId = -1;
    return;
  case 5021:
    motion = 49;
    effectId = -1;
    return;
  case 8410:
    motion = 7;
    effectId = 56;
    return;
  }

  if (static_cast<unsigned int>(skillId - 5401) <= 106 || static_cast<unsigned int>(skillId - 6001) <= 5 || static_cast<unsigned int>(skillId - 6525) <= 81) {
    effectId = -1;
  }

  const std::pair<bool, int> action = CSkillActionMgr::GetInstance()->ActionSlot02(skillId);
  if (action.first == true) {
    motion = action.second;
  }
}
