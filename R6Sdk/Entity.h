#pragma once
#include "Cheat.h"

class CBaseEntity {
	template<class T>
	T* GetFieldPointer(int offset) {
		return (T*)((DWORD)this + offset);
	}
	template<class T>
	inline T GetFieldValue(int offset) {
		return *(T*)((DWORD)this + offset);
	}
public:
	float GetEntityHealth();
	std::string GetPlayerName();
	int GetTeamNumber();
	Vector4D GetViewAngle();
	void SetViewAngle(Vector4D angle);
	void SetFireNum(float val);
	uintptr_t GetWeapon();
	Vector2D GetRecoil();
	void EnableRecoilOverWrite(bool Enable);
	void ZeroRecoil();
	void ZeroSpread();
	float GetRecoilPitch();
	float GetRecoilYaw();
	float GetSpeed();
	void SedSpeed(float speed);
	void SetDamage(int dmg);
	void InstantReload();
	bool IsReload();
	Vector GetHead();
	Vector GetFeet();
	Vector GetBonePostionByID(int id);
	int GetHealth();
	string GetName();

	
};