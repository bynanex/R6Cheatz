#pragma once

struct Variables
{
	Variables()
	{

	}
	struct
	{
		bool	Opened = false;
	} Menu;
	struct
	{
		bool	Enabled = true;
		bool	Box = true;
		bool	Health = true;
		bool	Skeleton = true;
		bool	AimSpot = false;
		bool	Name = true;
		bool	Distance = true;
		bool	PlayerEnable = true;
		struct
		{
			bool	Enable = true;
			bool	EnemyOnly = true;
			bool	VisibleOnly = false;
			bool	ViewCheck = true;
			int		Alpha = 255;
			float	Zoom = 1.5f;
			int		Type = 0;
			bool	NextResetRadar = false;
			float	MaxDis = 250.f;
		} Radar;
	} Visuals;
	struct
	{
		bool	Enable = true;
		bool	Hitboxs[7] = { true ,true,false,true};
		bool	EnableAimBotKey = true;
		bool	OneShot = true;
		bool	NoRecoil = true;
		bool	NoSpread = true;
		float	Fov = 60.f;
		int     aimSmooth = 5;
		int		AimBotKey = 2;
		int		TargetMethod = 0;
	} Aim;
	struct
	{
		bool	RapidFire = false;
		bool	InstantReload = false;
		bool	NoRecoil = false;
		bool	NoSpread = false;
		bool	MoreDmg = false;
		bool	SpeedHack = false;
		float	SpeedHackVal = 0.5f;
		int		MoreDmgVal = 30;
		float	RapidFireVal = 40.f;
	}Misc;
};
extern Variables Vars;