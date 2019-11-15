#include "pch.h"
#include "Offset.h"
uint64_t OFFSET::uDX11Base;
uint64_t OFFSET::uStatusManager;
uint64_t OFFSET::uGameManager;
uint64_t OFFSET::uIsInGame;
uint64_t OFFSET::m_iTeamNum;
uint64_t OFFSET::uEntitylist;
uint64_t OFFSET::uGameRender;
uint64_t OFFSET::uEngineLink;
uint64_t OFFSET::uEngineLink_Engine;
uint64_t OFFSET::uEngine_Camera;
uint64_t OFFSET::uEntityBone[] = { /*head*/ 0x670, /*neck*/ 0xF40, /*hand*/ 0x6A0, /*chest*/ 0xFC0,  /*stomach*/ 0xF80, /*pelvis*/ 0xFA0, /*feet*/ 0x6C0 };
uint64_t OFFSET::uViewRight;
uint64_t OFFSET::uViewUp;
uint64_t OFFSET::uViewForward;
uint64_t OFFSET::uViewTranslation;
uint64_t OFFSET::uViewFOVX;
uint64_t OFFSET::uViewFOVY;
uint64_t OFFSET::uEntitylistCount;
uint64_t OFFSET::uEntityInfo;
uint64_t OFFSET::uMainComponent;
uint64_t OFFSET::uChildComponent;
uint64_t OFFSET::uHealthFromChildComponent;
uint64_t OFFSET::uSkeleton;
uint64_t OFFSET::uProfileManager;
uint64_t OFFSET::uProfileManagerContainer;
uint64_t OFFSET::uLocalentity;
uint64_t OFFSET::uReplication;
uint64_t OFFSET::uReplicationTeam;
uint64_t OFFSET::uReplicationName;
uint64_t OFFSET::uViewAngle;
uint64_t OFFSET::uViewAngle_2;
uint64_t OFFSET::uWeapon_1;
uint64_t OFFSET::uWeapon_2;
uint64_t OFFSET::uCurrtWeapon;
uint64_t OFFSET::uReloadTime;
uint64_t OFFSET::uRecoilOverWrite;
uint64_t OFFSET::uCurrtRecoil;
uint64_t OFFSET::uSpread;
uint64_t OFFSET::uRecoilPitch;
uint64_t OFFSET::uRecoilYaw;
uint64_t OFFSET::uRapidFire;
void OffsetsManger::InstallOffset()
{
	DbgPrintA("InstallOffset Start");
	uint64_t uFindPos = (uint64_t)CUtils::PatternScan((void*)G::GameImageBase, XorString("4C 8B 35 ?? ?? ?? ?? 0f 28 7c 24 30 0f 28 74 24 40 4d 85 f6 "));
	uFindPos = uFindPos + *(uint32_t*)(uFindPos + 3) + 7;
	OFFSET::uDX11Base = CUtils::Read<uint64_t>(CUtils::ReadPtr<DWORD64>({ uFindPos, 0x710,0x8,0x720,0x28 }, true));
	DbgPrintA("uDX11Base:%llX", OFFSET::uDX11Base);

	uFindPos = (uint64_t)CUtils::PatternScan((void*)G::GameImageBase, XorString("48 8b 0d ?? ?? ?? ?? e8 ?? ?? ?? ?? 49 8b 8e ?? ?? ?? ?? e8"));
	OFFSET::uStatusManager = uFindPos + *(uint32_t*)(uFindPos + 3) + 7 - G::GameImageBase;
	DbgPrintA("uStatusManager:%llX", OFFSET::uStatusManager);

	uFindPos = (uint64_t)CUtils::PatternScan((void*)G::GameImageBase, XorString("48 8b 05 ?? ?? ?? ?? 83 b8 ?? ?? ?? ?? ?? 76 ?? 80 79"));
	OFFSET::uGameManager = uFindPos + *(uint32_t*)(uFindPos + 3) + 7 - G::GameImageBase;
	DbgPrintA("uGameManager:%llX", OFFSET::uGameManager);

	uFindPos = (uint64_t)CUtils::PatternScan((void*)G::GameImageBase, XorString("48 8b 05 ?? ?? ?? ?? 48 87 38"));
	OFFSET::uGameRender = uFindPos + *(uint32_t*)(uFindPos + 3) + 7 - G::GameImageBase;
	DbgPrintA("uGameRender:%llX", OFFSET::uGameRender);

	uFindPos = (uint64_t)CUtils::PatternScan((void*)G::GameImageBase, XorString("48 8b 05 ?? ?? ?? ?? 45 8b 9A"));
	OFFSET::uProfileManager = uFindPos + *(uint32_t*)(uFindPos + 3) + 7 - G::GameImageBase;
	DbgPrintA("uProfileManager:%llX", OFFSET::uProfileManager);

	uFindPos = (uint64_t)CUtils::PatternScan((void*)G::GameImageBase, XorString("48 8B 88 ?? ?? ?? ?? 40 88 B9 ?? ?? ?? ??"));
	OFFSET::uViewAngle = *(uint32_t*)(uFindPos + 3);
	DbgPrintA("uViewAngle:%llX", OFFSET::uViewAngle);
	
	//OFFSET::uProfileManager = 0x5212338;
	//--------------------------------------------Ó²±àÂë
	OFFSET::uIsInGame = 0x374;
	OFFSET::uEntitylist = 0x1C8;
	OFFSET::uEngineLink = 0xE0;
	OFFSET::uEngineLink_Engine = 0x1D8;
	OFFSET::uEngine_Camera = 0x8;
	OFFSET::uViewRight = 0xD0;
	OFFSET::uViewUp = 0xE0;
	OFFSET::uViewForward = 0xF0;
	OFFSET::uViewTranslation = 0x100;
	OFFSET::uViewFOVX = 0x110;
	OFFSET::uViewFOVY = 0x124;
	OFFSET::uEntitylistCount = 0x1D0;
	OFFSET::uEntityInfo = 0x28;
	OFFSET::uMainComponent = 0xd8;
	OFFSET::uChildComponent = 0x8;
	OFFSET::uHealthFromChildComponent = 0x238;
	OFFSET::uSkeleton = 0x20;
	OFFSET::uProfileManagerContainer = 0x68;
	OFFSET::uLocalentity = 0x28;
	OFFSET::uReplication = 0xA8;
	OFFSET::uReplicationTeam = 0x19E;
	OFFSET::uReplicationName = 0x1C8;
	OFFSET::uViewAngle_2 = 0xC0;
	OFFSET::uWeapon_1 = 0x78;
	OFFSET::uWeapon_2 = 0xC8;
	OFFSET::uCurrtWeapon = 0x208;
	OFFSET::uReloadTime = 0x24A0;
	OFFSET::uRecoilOverWrite = 0x168;
	OFFSET::uRapidFire = 0x248C;
	OFFSET::uCurrtRecoil = 0xC0;
	OFFSET::uSpread = 0xB0;
	OFFSET::uRecoilPitch = 0x14C;
	OFFSET::uRecoilYaw = 0x15C;
	//--------------------------------------------µ÷ÊÔÊä³ö
}
