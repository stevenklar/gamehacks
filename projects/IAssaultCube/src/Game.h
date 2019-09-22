#pragma once

#include <cstdint>
#include "Windows.h"
#include "Numerics.h"

class playerentPtr
{
public:
	class playerent* localPlayer; //0x0000
}; //Size: 0x0004

class playerent
{
public:
	virtual void Function0(); //
	virtual ~playerent(); //
	virtual void Function2(); //
	virtual void Function3(); //
	virtual void OnCollision(); //
	virtual void OnMoved(const Vec3& dist); //
	virtual void Function6(); //
	virtual void Function7(); //
	virtual void Function8(); //
	virtual void Function9(); //

	Vec3 m_Head; //0x0004 
	Vec3 m_Velocity; //0x0010 
	char _0x001C[24];
	Vec3 m_Position; //0x0034 
	Vec3 m_View; //0x0040
	char pad_004C[36]; //0x004C
	int32_t m_IsMoving; //0x0070
	char pad_0074[12]; //0x0074
	int32_t speed; //0x0080
	char pad_0084[116]; //0x0084
	int32_t m_Health; //0x00F8
	int32_t m_Vest; //0x00FC
	char pad_0100[40]; //0x0100
	int32_t m_AmmoMags; //0x0128
	char pad_012C[16]; //0x012C
	int32_t m_SecAmmo; //0x013C
	char pad_0140[16]; //0x0140
	int32_t m_Ammo; //0x0150
	char pad_0154[4]; //0x0154
	int32_t m_Flashbangs; //0x0158
	char pad_015C[28]; //0x015C
	int32_t weaponTimer; //0x0178
	char pad_017C[36]; //0x017C
	int32_t bulletsShot; //0x01A0
	char pad_01A4[129]; //0x01A4
	char name[17]; //0x0225

public:
	void godmode()
	{
		this->m_Health = 1337;
		this->m_Vest = 1337;
		this->m_Ammo = 1337;
		this->m_AmmoMags = 1337;
		this->m_Flashbangs = 1337;
		this->m_SecAmmo = 1337;
	}
}; //Size: 0x0512

class EntityList
{
public:
	playerent* entities[31];
}; //Size: 0x00F0


class ScreenSettings
{
public:
	__int32 m_Width; //0x0000 
	__int32 m_Height; //0x0004 
	char _0x0008[8];
	__int32 m_GraphicDetail; //0x0010 VIDEO: INSANE - MEDIUM = 8, LOWER = 0 
	__int32 m_AntiAlias; //0x0014  VIDEO: INSANE = 8, HIGH = 4, LOWER = 0
	__int32 m_VSync; //0x0018 
	BYTE m_Fullscreen; //0x001C 
	char _0x001D[23];
	__int32 m_Gamma; //0x0034 
	__int32 m_MaxFPS; //0x0038 
	char _0x003C[12];
	__int32 m_GameSpeed; //0x0048   100 is normal
	bool m_Paused; //0x004C
	char _0x004C[8];
	__int32 m_GameStarts;

	static ScreenSettings* GetInstance()
	{
		return reinterpret_cast<ScreenSettings*>(0x510C94);
	}
};

class Game
{
public:
	static const DWORD m_ViewMatrix = 0x501AE8;
	static const DWORD m_EntityList = 0x50F4F8;
	static const DWORD m_NumOfPlayers = 0x50F500;
	static const DWORD m_LocalPlayer = 0x50F4F4;

	playerent* GetLocalPlayer();
	playerent** GetEntityList();
	int GetNumberOfPlayers();
};

bool IsValidPtr(void* pPointer);
bool IsVisible(Vec3 vFrom, Vec3 vTo);
