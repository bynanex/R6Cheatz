#pragma once
#include "Cheat.h"
class OffsetsManger :public Singleton<OffsetsManger> {
public:
	void InstallOffset();
};

namespace OFFSET
{
	extern uint64_t uDX11Base;
	extern uint64_t uStatusManager;
	extern uint64_t uIsInGame;
	extern uint64_t m_iTeamNum;
	extern uint64_t uGameManager;
	extern uint64_t uEntitylist;
	extern uint64_t uGameRender;
	extern uint64_t uEngineLink;
	extern uint64_t uEngineLink_Engine;
	extern uint64_t uEngine_Camera;
	extern uint64_t uEntityBone[];
	extern uint64_t uViewRight;
	extern uint64_t uViewUp;
	extern uint64_t uViewForward;
	extern uint64_t uViewTranslation;
	extern uint64_t uViewFOVX;
	extern uint64_t uViewFOVY;
	extern uint64_t uEntitylistCount;
	extern uint64_t uEntityInfo;
	extern uint64_t uMainComponent;
	extern uint64_t uChildComponent;
	extern uint64_t uHealthFromChildComponent;
	extern uint64_t uSkeleton;
	extern uint64_t uProfileManager;
	extern uint64_t uProfileManagerContainer;
	extern uint64_t uLocalentity;
	extern uint64_t uReplication;
	extern uint64_t uReplicationTeam;
	extern uint64_t uReplicationName;
	extern uint64_t uViewAngle;
	extern uint64_t uViewAngle_2;
	extern uint64_t uWeapon_1;
	extern uint64_t uWeapon_2;
	extern uint64_t uReloadTime;
	extern uint64_t uCurrtWeapon;
	extern uint64_t uRecoilOverWrite;
	extern uint64_t uCurrtRecoil;
	extern uint64_t uSpread;
	extern uint64_t uRecoilPitch;
	extern uint64_t uRecoilYaw;
	extern uint64_t uRapidFire;
}
