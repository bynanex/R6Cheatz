#include "pch.h"
#include "Cheat.h"
#include "Aimbot.h"
CBaseEntity* m_ent;
Vector4D CAimBot::GetAngle(Vector viewTranslation, Vector enemyHead, Vector4D originalAngle)
{
	Vector aimAngle = VectorAngles(viewTranslation - enemyHead);
	Vector currentAngle = ToEuler(originalAngle);
	currentAngle *= (float)(180.f / M_PI);
	Vector currentAngleV = Vector(currentAngle.z, currentAngle.y, currentAngle.x);

	Vector smoothedAngle = Vector(aimAngle.x, aimAngle.y, aimAngle.z);
	smoothedAngle.x = GetDifference(currentAngleV.x, smoothedAngle.x);
	smoothedAngle.y = GetDifference(currentAngleV.y, smoothedAngle.y);
	smoothedAngle *= (Vars.Aim.aimSmooth * 0.1f);
	currentAngleV.x += smoothedAngle.x;
	currentAngleV.y += smoothedAngle.y;

	if (currentAngleV.x > 89.0f && currentAngleV.x <= 180.0f)
	{
		currentAngleV.x = 89.0f;
	}
	while (currentAngleV.x > 180.f)
	{
		currentAngleV.x -= 360.f;
	}
	while (currentAngleV.x < -89.0f)
	{
		currentAngleV.x = -89.0f;
	}
	while (currentAngleV.y > 180.f)
	{
		currentAngleV.y -= 360.f;
	}
	while (currentAngleV.y < -180.f)
	{
		currentAngleV.y += 360.f;
	}

	aimAngle *= (float)(M_PI / 180.f);
	currentAngle *= (float)(M_PI / 180.f);
	currentAngleV *= (float)(M_PI / 180.f);

	Vector4D newQuaternion = QuaternionFromYPR(currentAngleV.y, currentAngleV.x, 0);
	return newQuaternion;
}
void CAimBot::DropTarget()
{
	if (m_ent)
	{
		if (m_ent->GetHealth() <= 0)
		{
			m_ent = NULL;
			return;
		}
		float distance = I::Engine->GetCamera()->GetViewTranslation().Distance(m_ent->GetHead());
		float fov = W2SDistance(m_ent->GetHead());
		if (fov > Vars.Aim.Fov)
			m_ent = NULL;
		/*
		if (Vars.Aim.Hitboxs[0]) //head
		{
			return;
		}
		else if (Vars.Aim.Hitboxs[1]) //Neck
		{
			return;
		}
		else if (Vars.Aim.Hitboxs[2]) //abdomen
		{
			return;
		}
		*/
	}
	else
	{
		m_ent = NULL;
	}
}

void CAimBot::FindTarget()
{
	float bestFov = Vars.Aim.Fov;
	float bestDistance = 9999999.0f;
	float AntiAimFov = 180.f;
	int bestHp = 100;
	m_besthitbox = 0;
	int LocalPlayerTeam = G::LocalPlayer->GetTeamNumber();
	Array<CBaseEntity*> Entitylist = I::Engine->GetEntities();
	for (uint32_t i = 0; i < I::Engine->GetEntitiesCount(); i++)
	{
		auto pEntity = Entitylist[i];
		if (pEntity)
		{
			int hp = pEntity->GetHealth();
			if (hp > 0 && pEntity->GetTeamNumber() != LocalPlayerTeam)
			{
				float distance = I::Engine->GetCamera()->GetViewTranslation().Distance(pEntity->GetHead());
				float fov = W2SDistance(pEntity->GetHead());
				if (fov < bestFov)
					m_BastFovTarget = pEntity;
				if (fov > bestFov)
					continue;
				if (Vars.Aim.TargetMethod == 1 && distance > bestDistance)
					continue;
				if (Vars.Aim.TargetMethod == 2 && hp > bestHp)
					continue;
				bool found = false;
				if (Vars.Aim.Hitboxs[0])
				{
					found = true;
					m_BestPoint = pEntity->GetBonePostionByID(BONE_HEAD);
					m_besthitbox = BONE_HEAD;
				}
				else if (Vars.Aim.Hitboxs[1])
				{
					found = true;
					m_BestPoint = pEntity->GetBonePostionByID(BONE_NECK);
					m_besthitbox = BONE_NECK;
				}
				else if (Vars.Aim.Hitboxs[2])
				{
					found = true;
					m_BestPoint = pEntity->GetBonePostionByID(BONE_HAND);
					m_besthitbox = BONE_HAND;
				}
				else if (Vars.Aim.Hitboxs[3])
				{
					found = true;
					m_BestPoint = pEntity->GetBonePostionByID(BONE_CHEST);
					m_besthitbox = BONE_CHEST;
				}
				else if (Vars.Aim.Hitboxs[4])
				{
					found = true;
					m_BestPoint = pEntity->GetBonePostionByID(BONE_STOMACH);
					m_besthitbox = BONE_STOMACH;
				}
				else if (Vars.Aim.Hitboxs[5])
				{
					found = true;
					m_BestPoint = pEntity->GetBonePostionByID(BONE_PELVIS);
					m_besthitbox = BONE_PELVIS;
				}
				else if (Vars.Aim.Hitboxs[6])
				{
					found = true;
					m_BestPoint = pEntity->GetBonePostionByID(BONE_FEET);
					m_besthitbox = BONE_FEET;
				}
				if (found)
				{
					bestFov = fov;
					bestDistance = distance;
					bestHp = hp;
					m_ent = pEntity;
				}
			}
		}
	}
}

void CAimBot::DoAimbot()
{
	aimboting = false;
	if (!G::LocalPlayer->GetWeapon())
		return;
	DropTarget();

	if (G::LocalPlayer->IsReload() || G::PressedKeys[KEY_R])
		return;
	if (!Vars.Aim.Hitboxs[0] && !Vars.Aim.Hitboxs[2] && !Vars.Aim.Hitboxs[3] && !Vars.Aim.Hitboxs[4] && !Vars.Aim.Hitboxs[5] && !Vars.Aim.Hitboxs[6])
		return;
	if (Vars.Aim.EnableAimBotKey && !G::PressedKeys[Vars.Aim.AimBotKey])
		return;
	if (!m_ent)
	{
		shoting = false;
		bool tmp = true;
		if (Vars.Aim.OneShot)
		{
			if (G::PressedKeys[MOUSE_LEFT])
				tmp = false;
		}
		if (tmp)
			FindTarget();
	}
	else
	{
		if (!m_ent)
			return;
		if (m_ent->GetHealth() <= 0)
			return;
		if (Vars.Aim.NoRecoil)
			G::LocalPlayer->ZeroRecoil();
		if (Vars.Aim.NoSpread)
			G::LocalPlayer->ZeroSpread();
		Vector4D LocalViewAngle = G::LocalPlayer->GetViewAngle();
		Vector ViewTranslation = I::Engine->GetCamera()->GetViewTranslation();
		Vector4D aim_angle = GetAngle(ViewTranslation, m_ent->GetBonePostionByID(m_besthitbox), LocalViewAngle);

		G::LocalPlayer->SetViewAngle(aim_angle);
		aimboting = true;
	}
}
