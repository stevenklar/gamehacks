#include "pch.h"
#include "Game.h"
#include <gl\GL.h>

bool IsValidPtr(void* pPointer)
{
	if (pPointer && HIWORD(pPointer))
	{
		if (!IsBadReadPtr(pPointer, sizeof(DWORD_PTR)))
			return true;
	}
	return false;
}

bool IsVisible(Vec3 vFrom, Vec3 vTo)
{
#ifdef IX_WIN32
	__asm
	{
		push vTo.z;
		push vTo.y;
		push vTo.x;

		push vFrom.z;
		push vFrom.y;
		push vFrom.x;

		xor cl, cl;   //Tracer
		xor eax, eax;
		mov ebx, 0x48ABD0;
		call ebx;
		add esp, 0x18;
	}
#else
	return false;
#endif
}

void EngineDrawString(char* pText, int x, int y, int r, int g, int b, int pUnknown, int pUnknown2)
{
#ifdef IX_WIN32
	__asm
	{
		push pUnknown2;
		push pUnknown;
		push b;
		push r;
		push y;
		push x;
		push pText;
		mov eax, g;
		mov ecx, 0x41A150; // OFFSET_ENGINE_DRAWTEXT
		call ecx;
		add esp, 0x1C;
	}
#endif
}

void DrawString(float x, float y, float r, float g, float b, char* pText, ...)
{
	va_list va_alist;

	char buf[256];

	va_start(va_alist, pText);
	_vsnprintf_s(buf, sizeof(buf), pText, va_alist);
	va_end(va_alist);

	float extraScale = 0.3F;

	glScalef(extraScale, extraScale, 1);
	EngineDrawString(buf, x / extraScale, y / extraScale, r, g, b, -1, -1);
	glScalef(1 / extraScale, 1 / extraScale, 1);
}

playerent* Game::GetLocalPlayer()
{
	return *reinterpret_cast<playerent**>(m_LocalPlayer);
}

playerent** Game::GetEntityList()
{
	EntityList *pEntityList = *reinterpret_cast<EntityList**>(m_EntityList);
	return pEntityList->entities;
}

#include <memory>
int Game::GetNumberOfPlayers()
{
	return *reinterpret_cast<int*>(m_NumOfPlayers);
}
