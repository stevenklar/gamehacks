#pragma once

#pragma comment(lib,"OpenGL32.lib")
#include <gl\GL.h>
#include "DxWrapper.h"

float Get3dDistance(Vector3 myCoords, Vector3 enemyCoords)
{
	return static_cast<float>(
		sqrt(
			pow(double(enemyCoords.x - myCoords.x), 2.0) +
			pow(double(enemyCoords.y - myCoords.y), 2.0) +
			pow(double(enemyCoords.z - myCoords.z), 2.0)
		)
	);
}

void GetResolution(int &height, int &width)
{
	RECT rWindow;
	HWND fWindow = GetForegroundWindow();
	GetWindowRect(fWindow, &rWindow);
	
	height = rWindow.right;
	width = rWindow.bottom;
}

bool WorldToScreen(Vector3 pos, Vector2& Out, float* matrix) {
	D3DXVECTOR4 screen = { 0, 0, 0, 0 };

	screen.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	screen.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	screen.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	screen.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];
	/*
	screen.x = In.x * ViewMatrix[0] + In.y * ViewMatrix[4] + In.z * ViewMatrix[8] + ViewMatrix[12];
	screen.y = In.x * ViewMatrix[1] + In.y * ViewMatrix[5] + In.z * ViewMatrix[9] + ViewMatrix[13];
	screen.z = In.x * ViewMatrix[2] + In.y * ViewMatrix[6] + In.z * ViewMatrix[10] + ViewMatrix[14];
	screen.w = In.x * ViewMatrix[3] + In.y * ViewMatrix[7] + In.z * ViewMatrix[11] + ViewMatrix[15];
	*/

	if (screen.w < 0.1f)
		return false;

	/*
	screen.x /= screen.w;
	screen.y /= screen.w;
	screen.z /= screen.w;

	screen.x *= 800 / 2.0f;
	screen.x += 800 / 2.0f;

	screen.y *= 600 / 2.0f;
	screen.y += 600 / 2.0f;
	*/

	Vector3 NDC;
	NDC.x = screen.x / screen.w;
	NDC.y = screen.y / screen.w;
	NDC.z = screen.z / screen.w;

	int windowWidth = 800;
	int windowHeight = 600;

	Out.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	Out.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	return true;
}

bool WorldtoScreen3(Vector3 vOrigin, Vector4& vScreen, float W2Smatrix[4][4], float width, float height)
{
	vScreen.x = (W2Smatrix[0][0] * vOrigin.x) + (W2Smatrix[0][1] * vOrigin.y) + (W2Smatrix[0][2] * vOrigin.z) + (W2Smatrix[0][3] * 1.0f);
	vScreen.y = (W2Smatrix[1][0] * vOrigin.x) + (W2Smatrix[1][1] * vOrigin.y) + (W2Smatrix[1][2] * vOrigin.z) + (W2Smatrix[1][3] * 1.0f);
	vScreen.w = (W2Smatrix[3][0] * vOrigin.x) + (W2Smatrix[3][1] * vOrigin.y) + (W2Smatrix[3][2] * vOrigin.z) + (W2Smatrix[3][3] * 1.0f); //homogenous coordinates

	//object is behind camera (not visible), w = z where z is relative to the camera.
	if (vScreen.w < 0.0f)
		return false;

	//perspective division
	vScreen.x /= vScreen.w;
	vScreen.y /= vScreen.w;

	//viewport transform
	vScreen.x = ((vScreen.x + 1) * (width / 2));
	vScreen.y = ((vScreen.y + 1) * (height / 2));

	return true;
}

bool OpenglWorldToScreen2(Vector3 worldPos, float matrix[16], int width, int height, Vector2 &screenPos)
{

	float screenX = (matrix[0] * worldPos.x) + (matrix[4] * worldPos.y) + (matrix[8] * worldPos.z) + matrix[12];
	float screenY = (matrix[1] * worldPos.x) + (matrix[5] * worldPos.y) + (matrix[9] * worldPos.z) + matrix[13];
	float screenW = (matrix[3] * worldPos.x) + (matrix[7] * worldPos.y) + (matrix[11] * worldPos.z) + matrix[15];

	float camX = width / 2.0f;
	float camY = height / 2.0f;

	float x = camX + (camX * screenX / screenW);
	float y = camY - (camY * screenY / screenW);
	screenPos = Vector2{ x, y };

	return (screenW > 0.01f);
}

bool OpenglWorldToScreen(Vector3 pos, float matrix[16], int windowWidth, int windowHeight, Vector2& screen)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vector4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.01f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;

	//Transform to window coordinates
	screen.x = (windowWidth / 2.0f * NDC.x) + (NDC.x + windowWidth / 2.0f);
	screen.y = -(windowHeight / 2.0f * NDC.y) + (NDC.y + windowHeight / 2.0f);
	return true;
}

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

class Console
{
public:
	Console()
	{
		AllocConsole();
		FILE* pFile = nullptr;
		freopen_s(&pFile, "CONOUT$", "w", stdout);
	}

	~Console()
	{
		FreeConsole();
	}
};

/*
BOOL IsValidEntity(playerent* entity)
{
	if (entity)
	{
		if (entity->vTable == 0x4E4A98 || entity->vTable == 0x4E4AC0)
		{
			return true;
		}
	}
	return false;
}
*/
