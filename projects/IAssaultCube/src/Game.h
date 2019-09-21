#pragma once

#include "Windows.h"
#include "Numerics.h"

class Game
{
public:
	static const DWORD m_ViewMatrix = 0x501AE8;
	static const DWORD m_EntityList = 0x50F4F8;
	static const DWORD m_NumOfPlayers = 0x50F500;
	static const DWORD m_LocalPlayer = 0x50F4F4;
};

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

bool IsValidPtr(void* pPointer);
bool IsVisible(Vec3 vFrom, Vec3 vTo);
