#include "Game.h"

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
}
