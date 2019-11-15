#include "pch.h"
#include "Cheat.h"
struct VMatrixW2S
{
	float m[4][4];

	inline float* operator[](int i)
	{
		return m[i];
	}

	inline const float* operator[](int i) const
	{
		return m[i];
	}
};
uintptr_t GameEngine::GetGameStatus()
{
	return *(uintptr_t *)(G::GameImageBase + OFFSET::uStatusManager);
}
uintptr_t GameEngine::GetGameManager()
{
	return *(uintptr_t*)(G::GameImageBase + OFFSET::uGameManager);
}
Array<CBaseEntity*> GameEngine::GetEntities()
{
	return *(Array<CBaseEntity*>*)(this->GetGameManager() + OFFSET::uEntitylist);
}
uint16_t GameEngine::GetEntitiesCount()
{
	return *(uint16_t*)(this->GetGameManager() + OFFSET::uEntitylistCount);
}

CBaseEntity* GameEngine::GetLocalPlayer()
{
	auto pProfile = *(uintptr_t*)(G::GameImageBase + OFFSET::uProfileManager);
	return CUtils::ReadPtr<CBaseEntity*>({ (uint64_t)pProfile,OFFSET::uProfileManagerContainer,0x0,OFFSET::uLocalentity}, false);
	//localpalyer 读了 uLocalentity(0x28) 所以以后都不需要读 0x28 而敌人没有读,所以敌人要读0x28
}
Camera* GameEngine::GetCamera()
{
	auto GameRenderer = *(uintptr_t*)(G::GameImageBase + OFFSET::uGameRender);
	return CUtils::ReadPtr<Camera*>({ (uint64_t)GameRenderer, OFFSET::uEngineLink,OFFSET::uEngineLink_Engine,OFFSET::uEngine_Camera }, true);
}

bool GameEngine::IsInGame()
{
	auto m_Status = GetGameStatus();
	return *(bool*)((uintptr_t)m_Status + OFFSET::uIsInGame);
}
bool GameEngine::WorldToScreen(const Vector& origin, Vector2D& screen)
{
	auto camera = I::Engine->GetCamera();
	if (!camera)
		return false;
	Vector temp = origin - camera->GetViewTranslation();
	float x = temp.Dot(camera->GetViewRight());
	float y = temp.Dot(camera->GetViewUp());
	float z = temp.Dot(camera->GetViewForward() * -1);
	ImGuiIO& io = ImGui::GetIO();
	screen.x = (io.DisplaySize.x / 2) * (1 + x / camera->GetViewFovX() / z);
	screen.y = (io.DisplaySize.y / 2) * (1 - y / camera->GetViewFovY() / z);
	return z >= 1.0f;
}

Vector Camera::GetViewRight()
{
	return *(Vector*)((uintptr_t)this + OFFSET::uViewRight);
}
Vector Camera::GetViewUp()
{
	return *(Vector*)((uintptr_t)this + OFFSET::uViewUp);
}
Vector Camera::GetViewForward()
{
	return *(Vector*)((uintptr_t)this + OFFSET::uViewForward);
}
Vector Camera::GetViewTranslation()
{
	return *(Vector*)((uintptr_t)this + OFFSET::uViewTranslation);
}

float Camera::GetViewFovX()
{
	return *(float*)((uintptr_t)this + OFFSET::uViewFOVX);
}

float Camera::GetViewFovY()
{
	return *(float*)((uintptr_t)this + OFFSET::uViewFOVY);
}

