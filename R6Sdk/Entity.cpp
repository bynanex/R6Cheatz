#include "pch.h"
#include "Cheat.h"
#include "StringCovert.h"
string RPMString(DWORD64 address) {
	if (!(void*)address)
		return string("BOT");
	
	string nameString;
	char name[30];
	memcpy(name,(void*)address,30);
	for (int i = 0; i < sizeof(name); i++) {
		if (!name[i])
			break;
		if ((int)name[i] >= 32 && (int)name[i] <= 126)
			nameString += name[i];
		else
			break;
	}
	return nameString;
}
std::string CBaseEntity::GetPlayerName()
{
	//return RPMString(CUtils::ReadPtr<DWORD64>({ (uint64_t)this,OFFSET::uReplication,OFFSET::uReplicationName }, false));
	return (DWORD64)this == (DWORD64)G::LocalPlayer ? RPMString(CUtils::ReadPtr<DWORD64>({ (uint64_t)this,OFFSET::uReplication,OFFSET::uReplicationName }, false)) : RPMString(CUtils::ReadPtr<DWORD64>({ (uint64_t)this,OFFSET::uEntityInfo,OFFSET::uReplication,OFFSET::uReplicationName }, false));
}
int CBaseEntity::GetTeamNumber()
{
	//return CUtils::ReadPtr<int>({ (uint64_t)this,OFFSET::uReplication,OFFSET::uReplicationTeam }, false);
	return (DWORD64)this == (DWORD64)G::LocalPlayer ? CUtils::ReadPtr<int>({ (uint64_t)this,OFFSET::uReplication,OFFSET::uReplicationTeam }, false) : CUtils::ReadPtr<int>({ (uint64_t)this,OFFSET::uEntityInfo,OFFSET::uReplication,OFFSET::uReplicationTeam }, false);
}
Vector4D CBaseEntity::GetViewAngle()
{
	return CUtils::ReadPtr<Vector4D>({ (uint64_t)this,OFFSET::uSkeleton,OFFSET::uViewAngle,OFFSET::uViewAngle_2 }, false);
}
void CBaseEntity::SetViewAngle(Vector4D angle)
{
	CUtils::WritePtr<Vector4D>({ (uint64_t)this,OFFSET::uSkeleton,OFFSET::uViewAngle,OFFSET::uViewAngle_2 },angle,false);
}
void CBaseEntity::SetFireNum(float val)
{
	uintptr_t weapon = this->GetWeapon();
	if (!weapon)
		return;
	CUtils::WritePtr<float>({ (uint64_t)weapon,OFFSET::uCurrtWeapon,OFFSET::uRapidFire }, val, false);
}
uintptr_t CBaseEntity::GetWeapon()
{
	return CUtils::ReadPtr<uintptr_t>({ (uint64_t)this,OFFSET::uWeapon_1,OFFSET::uWeapon_2}, false);
}
Vector2D CBaseEntity::GetRecoil()
{
	float x = this->GetRecoilPitch();
	float y = this->GetRecoilYaw();
	return Vector2D(x,y);
}
void CBaseEntity::EnableRecoilOverWrite(bool Enable)
{
	uintptr_t weapon = this->GetWeapon();
	if (!weapon)
		return;
	CUtils::WritePtr<bool>({ (uint64_t)weapon,OFFSET::uCurrtWeapon,OFFSET::uRecoilOverWrite }, Enable, false);
}
void CBaseEntity::ZeroRecoil()
{
	uintptr_t weapon = this->GetWeapon();
	if (!weapon)
		return;
	CUtils::WritePtr<float>({ (uint64_t)weapon,OFFSET::uCurrtWeapon,OFFSET::uCurrtRecoil },1.0f,false);
}
void CBaseEntity::ZeroSpread()
{
	uintptr_t weapon = this->GetWeapon();
	if (!weapon)
		return;
	CUtils::WritePtr<float>({ (uint64_t)weapon,OFFSET::uCurrtWeapon,OFFSET::uSpread }, 0.f, false);
}
float CBaseEntity::GetRecoilPitch()
{
	uintptr_t weapon = this->GetWeapon();
	if (!weapon)
		return 0.f;
	return CUtils::ReadPtr<float>({ (uint64_t)weapon,OFFSET::uCurrtWeapon,OFFSET::uRecoilPitch }, false);
}
float CBaseEntity::GetRecoilYaw()
{
	uintptr_t weapon = this->GetWeapon();
	if (!weapon)
		return 0.f;
	return CUtils::ReadPtr<float>({ (uint64_t)weapon,OFFSET::uCurrtWeapon,OFFSET::uRecoilYaw }, false);
}
float CBaseEntity::GetSpeed()
{
	return CUtils::ReadPtr<float>({ (uint64_t)this,0x30,0x30,0x38 ,0x78}, false);
}
void CBaseEntity::SedSpeed(float speed)
{
	CUtils::WritePtr<int>({ (uint64_t)this,0x30,0x30,0x38 ,0x78 }, speed, false);
}
void CBaseEntity::SetDamage(int dmg)
{
	CUtils::WritePtr<int>({ (uint64_t)I::Engine->GetGameManager(),0x1F8,0xD8,0x48,0x130,0x130,0x0,0x40 }, dmg, false);
	CUtils::WritePtr<int>({ (uint64_t)I::Engine->GetGameManager(),0x23D8,0x130,0x0,0x40 }, dmg, false);
}
void CBaseEntity::InstantReload()
{
	uintptr_t weapon = this->GetWeapon();
	if (!weapon)
		return;
	*(float*)(weapon + OFFSET::uReloadTime) = 0.001f;
}
bool CBaseEntity::IsReload()
{
	uintptr_t weapon = this->GetWeapon();
	if (!weapon)
		return false;
	return *(float*)(weapon + OFFSET::uReloadTime) > 0.001f;
}
Vector CBaseEntity::GetHead()
{
	return this->GetBonePostionByID(BONE_HEAD);
}

Vector CBaseEntity::GetFeet()
{
	return this->GetBonePostionByID(BONE_FEET);
}

Vector CBaseEntity::GetBonePostionByID(int id)
{
	return CUtils::ReadPtr<Vector>({ (uint64_t)this,OFFSET::uSkeleton,OFFSET::uEntityBone[id] }, false);
}

int CBaseEntity::GetHealth()
{
	float m_health = CUtils::ReadPtr<float>({ (uint64_t)this,OFFSET::uEntityInfo,OFFSET::uMainComponent,OFFSET::uChildComponent,OFFSET::uHealthFromChildComponent }, false);
	m_health *= 100;
	return (int)round(m_health);
}