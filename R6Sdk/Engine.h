#pragma once
#include "Cheat.h"
class Memory
{
public:
	template <typename Type, typename Base, typename Offset>
	static inline Type Ptr(Base base, Offset offset)
	{
		static_assert(std::is_pointer<Type>::value || std::is_integral<Type>::value, "Type must be a pointer or address");
		static_assert(std::is_pointer<Base>::value || std::is_integral<Base>::value, "Base must be a pointer or address");
		static_assert(std::is_pointer<Offset>::value || std::is_integral<Offset>::value, "Offset must be a pointer or address");

		return base ? reinterpret_cast<Type>((reinterpret_cast<uint64_t>(base) + static_cast<uint64_t>(offset))) : nullptr;
	}

	template <typename Type>
	static bool IsValidPtr(Type* ptr)
	{
		return (ptr && sizeof(ptr)) ? true : false;
	}

	static bool IsValidPtr(void* ptr)
	{
		return (ptr && sizeof(ptr)) ? true : false;
	}
};
template <typename T>
class Array
{
private:
	T* m_pBuffer;
	uint64_t m_size;

public:
	uint32_t GetSize()
	{
		return m_size;
	}

	const T& operator [](uint64_t i)
	{
		if (Memory::IsValidPtr<T>(m_pBuffer))
			return m_pBuffer[i];

		return nullptr;
	}
};
class Camera
{
public:
	Vector GetViewRight();
	Vector GetViewUp();
	Vector GetViewForward();
	Vector GetViewTranslation();
	float GetViewFovX();
	float GetViewFovY();
};
class GameEngine {
public:
	uintptr_t GetGameStatus();
	uintptr_t GetGameManager();
	Array<CBaseEntity*> GetEntities();
	uint16_t GetEntitiesCount();
	CBaseEntity* GetLocalPlayer();
	Camera* GetCamera();
	bool IsInGame();
	bool WorldToScreen(const Vector& origin, Vector2D& screen);
};